from app.data.database.components import Component
from app.data.database.database import DB
from app.engine.game_state import GameState
from app.engine import evaluate
import re
from typing import Dict, List, Tuple

from app.utilities import str_utils
import logging

class TextEvaluator():
    def __init__(self, logger: logging.Logger, game: GameState, unit=None, unit2=None, position=None, local_args=None) -> None:
        self.logger = logger
        self.game: GameState = game
        self.unit = unit
        self.unit2 = unit2
        self.position = position
        self.local_args = local_args or {}

    def direct_eval(self, to_eval):
        return evaluate.evaluate(to_eval, self.unit, self.unit2, self.position, self.local_args, self.game)

    @property
    def created_unit(self):
        return self.local_args.get('created_unit', None)

    @created_unit.setter
    def created_unit(self, created):
        self.local_args['created_unit'] = created

    def _object_to_str(self, obj) -> str:
        if hasattr(obj, 'uid'):
            return str(obj.uid)
        elif hasattr(obj, 'nid'):
            return str(obj.nid)
        else:
            return str(obj)

    def _evaluate_phrase(self, text: str, local_args=None) -> str:
        """Accepts a string such as {eval:expr}, and returns its evaluation
        """
        if re.match(r'\{e:[^{}]*\}', text) or re.match(r'\{eval:[^{}]*\}', text): # eval statement
            return self._evaluate_evals(text)
        elif re.match(r'\{d:[^{}]*\}', text) or re.findall(r'\{data:[^{}]*\}', text):
            return self._evaluate_data(text)
        elif re.match(r'\{f:[^{}]*\}', text) or re.match(r'\{field:[^{}]*\}', text):
            return self._evaluate_unit_fields(text)
        elif re.match(r'\{v:[^{}]*\}', text) or re.match(r'\{var:[^{}]*\}', text):
            return self._evaluate_vars(text)
        elif re.match(r'\{s:[^{}]*\}', text) or re.match(r'\{skill:[^{}]*\}', text):
            return self._evaluate_skill_db(text)
        elif re.match(r'\{i:[^{}]*\}', text) or re.match(r'\{item:[^{}]*\}', text):
            return self._evaluate_item_db(text)
        elif re.match(r'\{[^:{}]*\}', text):
            return self._evaluate_locals(text, local_args or {})
        else:
            return text

    def _evaluate_all(self, text: str, local_args=None) -> str:
        def recursive_parse(parse_list) -> str:
            copy = [""] * len(parse_list)
            for idx, nested in enumerate(parse_list):
                if isinstance(nested, list):
                    recursively_parsed = recursive_parse(nested)
                    copy[idx] = recursively_parsed
                else:
                    copy[idx] = nested
            return str(self._evaluate_phrase('{' + ''.join(copy) + '}', local_args))
        to_evaluate = str_utils.matched_expr(text, '{', '}')
        evaluated = []
        for to_eval in to_evaluate:
            parsed = str_utils.nested_expr(to_eval, '{', '}')
            evaled = recursive_parse(parsed)
            evaluated.append(evaled)
        evaluated_pairs = zip(to_evaluate, evaluated)
        # sort by length so we don't accidentally substitute a shorter inside a longer,
        # e.g. "{e:1} or {e:2 + {e:1}}". which would end up not working
        evaluated_pairs = sorted(evaluated_pairs, key=lambda pair: len(pair[0]), reverse=True)
        for to_eval, evaled in evaluated_pairs:
            text = text.replace(to_eval, evaled)
        return text

    def _evaluate_locals(self, text, local_args: Dict[str, str]) -> str:
        local_args = local_args or {}
        to_evaluate: List[str] = re.findall(r'\{[^{}]*\}', text)
        evaluated = []
        for to_eval in to_evaluate:
            to_eval = to_eval[1:-1]
            if to_eval in local_args:
                evaluated.append(self._object_to_str(local_args[to_eval]))
            elif to_eval in self.local_args:
                evaluated.append(self._object_to_str(self.local_args[to_eval]))
            else:
                evaluated.append('{%s}' % to_eval)
        for idx in range(len(to_evaluate)):
            text = text.replace(to_evaluate[idx], evaluated[idx])
        return text

    def _evaluate_data(self, text) -> str:
        if not self.game:
            return "??"
        # find data fields {d:}
        to_evaluate: List[str] = re.findall(r'\{d:[^{}]*\}', text) + re.findall(r'\{data:[^{}]*\}', text)
        evaluated = []
        for to_eval in to_evaluate:
            to_eval = self.trim_eval_tags(to_eval)
            args = to_eval.split('.')
            try:
                data_nid = args[0]
                # expected syntax:
                # {d:data_nid} if str
                # {d:data_nid.key_nid} if kv dict
                # {d:data_nid.obj_nid.property_nid} if list/table
                # {d:data_nid.[p1=x,p2=y,p3=z].property_nid} if list/table with searching
                if len(args) == 1:
                    resolved_data = self.game.get_data(data_nid)
                elif len(args) == 2:
                    key_nid = args[1]
                    resolved_data = self.game.get_data(data_nid)[key_nid]
                elif len(args) == 3:
                    attr_nid = args[2]
                    if args[1].startswith('[') and args[1].endswith(']'): # searching
                        searches = args[1][1:-1].split(',')
                        search_keys: List[Tuple[str, str]] = []
                        general_search_keys: List[str] = []
                        for term in searches:
                            searchl = term.split('=')
                            if len(searchl) == 1: # single search term, if any match we're good
                                general_search_keys.append(searchl[0].strip())
                            elif len(searchl) == 2: # search key set to value
                                col, val = searchl
                                search_keys.append((col.strip(), val.strip()))
                        for item in self.game.get_data(data_nid)._list:
                            if all([hasattr(item, col) and getattr(item, col) == val for col, val in search_keys]) and \
                                all([any([getattr(item, col) == val for col in dir(item)]) for val in general_search_keys]):
                                return getattr(item, attr_nid)
                        raise ValueError("No data matching" + str(search_keys) + " and " + str(general_search_keys))
                    else:
                        obj_nid = args[1]
                        resolved_data = getattr(self.game.get_data(data_nid).get(obj_nid), attr_nid)
                else:
                    raise ValueError("Bad data format")
                evaluated.append(self._object_to_str(resolved_data))
            except Exception as e:
                self.logger.error("eval failed, failed to parse %s (%s)", to_eval, repr(e))
                evaluated.append('??')
        for idx in range(len(to_evaluate)):
            text = text.replace(to_evaluate[idx], evaluated[idx])
        return text

    def _evaluate_unit_fields(self, text) -> str:
        # find unit fields {f:}
        to_evaluate: List[str] = re.findall(r'\{f:[^{}]*\}', text) + re.findall(r'\{field:[^{}]*\}', text)
        evaluated = []
        for to_eval in to_evaluate:
            to_eval = self.trim_eval_tags(to_eval)
            # expected syntax: {f:[unit.]key, fallback}
            if ',' in to_eval:
                field_text, fallback = to_eval.split(',', 1)
            else:
                field_text, fallback = to_eval, " "

            if '.' in to_eval: # possible unit tag?
                nid, field = field_text.split('.', 1)
            else:
                nid, field = '_unit', field_text

            if nid == '_unit':
                unit = self.unit
            elif nid == '_unit2':
                unit = self.unit2
            elif self.game:
                unit = self.game.get_unit(nid)

            if not unit: # maybe class?
                klass = DB.classes.get(nid)
                if not klass:
                    self.logger.error("eval of {f:%s} failed, unknown unit or class: %s", to_eval, nid)
                    evaluated.append('??')
                    continue
                found_vals = [val for (name, val) in klass.fields if name == field]
                if not any(found_vals):
                    self.logger.error("eval of {f:%s} failed, no such field %s on class %s: %s", to_eval, field, nid)
                    evaluated.append('??')
                    continue
                field_value = found_vals[0]
            else:
                field_value = unit.get_field(field, fallback)
            evaluated.append(self._object_to_str(field_value))
        for idx in range(len(to_evaluate)):
            text = text.replace(to_evaluate[idx], evaluated[idx])
        return text

    def _evaluate_skill_db(self, text) -> str:
        # find skill queries
        to_evaluate: List[str] = re.findall(r'\{s:[^{}]*\}', text) + re.findall(r'\{skill:[^{}]*\}', text)
        evaluated = []
        for to_eval in to_evaluate:
            to_eval = self.trim_eval_tags(to_eval)
            # expected syntax: {s:skill.key}
            if not '.' in to_eval:
                self.logger.error("eval of {s:%s} failed, no period", to_eval)
                evaluated.append('??')
                continue
            skill_nid, attribute = to_eval.split('.', 1)
            skill = DB.skills.get(skill_nid, None)
            if not skill:
                self.logger.error("eval of {s:%s} failed, no such skill %s", to_eval, skill_nid)
                evaluated.append('??')
                continue
            if not hasattr(skill, attribute):
                self.logger.error("eval of {s:%s} failed, no such attribute %s for skill %s", to_eval, attribute, skill_nid)
                evaluated.append('??')
                continue
            attribute_obj = getattr(skill, attribute)
            if isinstance(attribute_obj, Component):
                attribute_obj = attribute_obj.value
            attribute_value = str(attribute_obj)
            evaluated.append(self._object_to_str(attribute_value))
        for idx in range(len(to_evaluate)):
            text = text.replace(to_evaluate[idx], evaluated[idx])
        return text

    def _evaluate_item_db(self, text) -> str:
        # find item queries
        to_evaluate: List[str] = re.findall(r'\{i:[^{}]*\}', text) + re.findall(r'\{item:[^{}]*\}', text)
        evaluated = []
        for to_eval in to_evaluate:
            to_eval = self.trim_eval_tags(to_eval)
            # expected syntax: {i:item.key}
            if not '.' in to_eval:
                self.logger.error("eval of {i:%s} failed, no period", to_eval)
                evaluated.append('??')
                continue
            item_nid, attribute = to_eval.split('.', 1)
            item = DB.items.get(item_nid, None)
            if not item:
                self.logger.error("eval of {i:%s} failed, no such item %s", to_eval, item_nid)
                evaluated.append('??')
                continue
            if not hasattr(item, attribute):
                self.logger.error("eval of {i:%s} failed, no such attribute %s for item %s", to_eval, attribute, item_nid)
                evaluated.append('??')
                continue
            attribute_obj = getattr(item, attribute)
            if isinstance(attribute_obj, Component):
                attribute_obj = attribute_obj.value
            attribute_value = str(attribute_obj)
            evaluated.append(self._object_to_str(attribute_value))
        for idx in range(len(to_evaluate)):
            text = text.replace(to_evaluate[idx], evaluated[idx])
        return text

    def _evaluate_evals(self, text: str) -> str:
        # Set up variables so evals work well
        to_evaluate = re.findall(r'\{e:[^{}]*\}', text) + re.findall(r'\{eval:[^{}]*\}', text)
        evaluated = []
        for to_eval in to_evaluate:
            to_eval = self.trim_eval_tags(to_eval)
            try:
                val = evaluate.evaluate(to_eval, self.unit, self.unit2, self.position, self.local_args, self.game)
                evaluated.append(self._object_to_str(val))
            except Exception as e:
                self.logger.error("Could not evaluate %s (%s)" % (to_eval, e))
                evaluated.append('??')
        for idx in range(len(to_evaluate)):
            text = text.replace(to_evaluate[idx], evaluated[idx])
        return text

    def _evaluate_vars(self, text) -> str:
        if not self.game:
            return "??"
        to_evaluate = re.findall(r'\{v:[^{}]*\}', text) + re.findall(r'\{var:[^{}]*\}', text)
        evaluated = []
        for to_eval in to_evaluate:
            key = self.trim_eval_tags(to_eval)
            if key in self.game.level_vars:
                val = self._object_to_str(self.game.level_vars[key])
            elif key in self.game.game_vars:
                val = self._object_to_str(self.game.game_vars[key])
            else:
                self.logger.error("Could not find var {%s} in self.game.level_vars or self.game.game_vars" % key)
                val = '??'
            evaluated.append(val)
        for idx in range(len(to_evaluate)):
            text = text.replace(to_evaluate[idx], evaluated[idx])
        return text

    def trim_eval_tags(self, text: str) -> str:
        colon_idx = text.index(':')
        return text[colon_idx+1:-1]
