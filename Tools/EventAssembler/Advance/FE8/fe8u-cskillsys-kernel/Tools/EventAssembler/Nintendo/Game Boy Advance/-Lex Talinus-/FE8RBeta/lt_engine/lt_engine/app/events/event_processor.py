from __future__ import annotations

import logging
from typing import List, Optional, Tuple

from app.engine.text_evaluator import TextEvaluator
from app.events import event_commands
from app.utilities.typing import NID

class EventIterator():
    def __init__(self, items: List[str]) -> None:
        self.items = items
        self._idx = 0

    def next(self):
        self._idx += 1

    def get(self):
        if self._idx < len(self.items):
            return self.items[self._idx]
        return None

    def empty(self):
        return self._idx >= len(self.items)

    def __str__(self) -> str:
        return "<EventIterator: [%s]>" % ', '.join(self.items)

    def __repr__(self):
        return self.__str__()

    def save(self):
        s_dict = {}
        s_dict['items'] = self.items
        s_dict['idx'] = self._idx
        return s_dict

    @classmethod
    def restore(cls, s_dict):
        items = s_dict['items']
        idx = s_dict['idx']
        self = cls(items)
        self._idx = idx
        return self

class IteratorInfo():
    def __init__(self, nid: NID, line: int, items: List[str]) -> None:
        self.nid = nid
        self.line = line
        self.iterator = EventIterator(items)

    def save(self):
        s_dict = {}
        s_dict['nid'] = self.nid
        s_dict['line'] = self.line
        s_dict['iterator'] = self.iterator.save()
        return s_dict

    @classmethod
    def restore(cls, s_dict):
        self = cls(None, None, None)
        self.nid = s_dict['nid']
        self.line = s_dict['line']
        self.iterator = EventIterator.restore(s_dict['iterator'])
        return self

class EventProcessor():
    def __init__(self, nid: NID, script: str, text_evaluator: TextEvaluator):
        self.nid = nid
        self.script = script
        self.commands: List[event_commands.EventCommand] = event_commands.parse_script_to_commands(script)
        self.command_pointer = 0

        self.logger = logging.getLogger()
        self.text_evaluator = text_evaluator

        self.iterator_stack: List[IteratorInfo] = []

    def _evaluate_all(self, text: str) -> str:
        iterator_context = {}
        for iterator_info in self.iterator_stack:
            iterator_context[iterator_info.nid] = iterator_info.iterator.get()
        return self.text_evaluator._evaluate_all(text, iterator_context)

    def _get_truth(self, command: event_commands.EventCommand) -> bool:
        try:
            cond = command.parameters['Expression']
            cond = self._evaluate_all(cond)
            truth = bool(self.text_evaluator.direct_eval(cond))
        except Exception as e:
            self.logger.error("%s: Could not evaluate {%s} in %s" % (e, cond, command.to_plain_text()))
            truth = False
        self.logger.info("Result: %s" % truth)
        return truth

    def _jump_conditional(self, index: int) -> int:
        """Given an index of a conditional if,
        returns the index of the next command,
        i.e. `end` or `else` if the conditional is false,
        otherwise the next index."""
        base_conditional = self.commands[index]
        if not base_conditional.nid in ('if', 'elif'): # seeking an elif, else, or end
            raise TypeError("%s is not a conditional command" % str(base_conditional))
        # ensure end exists - this will throw if it doesn't
        self._find_end(index)

        truth = self._get_truth(base_conditional)
        if truth:
            return index + 1
        # not true, so seek next clause
        level = 0
        curr_index = index + 1
        while(curr_index) < len(self.commands):
            curr_command = self.commands[curr_index]
            if level == 0:
                if curr_command.nid in ('elif',):
                    return self._jump_conditional(curr_index)
                elif curr_command.nid in ('end', 'else'):
                    return curr_index + 1
            if curr_command.nid == 'if':
                level += 1
            elif curr_command.nid == 'end':
                level -= 1
            curr_index += 1
        # have not found end clause
        # maybe throw instead?
        raise SyntaxError("Line %d: %s has no corresponding terminator" % (index, str(base_conditional)))

    def _find_end(self, index: int) -> int:
        """given an index of an if, elif, else, for, or while command,
        gets the index of the end of the entire block.
        """
        base_conditional = self.commands[index]
        start_command = 'if'
        end_command = 'end'
        if base_conditional.nid in ('if', 'elif', 'else'):
            pass
        elif base_conditional.nid == 'for':
            start_command = 'for'
            end_command = 'endf'
        else:
            raise TypeError("%s is not a conditional command" % str(base_conditional))
        level = 0
        curr_index = index + 1
        while curr_index < len(self.commands):
            curr_command = self.commands[curr_index]
            if curr_command.nid == start_command:
                level += 1
            elif curr_command.nid == end_command:
                if level == 0:
                    return curr_index + 1
                else:
                    level -= 1
            curr_index += 1
        raise SyntaxError("Line %d: %s has no corresponding terminator" % (index, str(base_conditional)))

    def _build_iterator(self, index: int, command: event_commands.EventCommand) -> IteratorInfo:
        iterator_nid = command.parameters['Nid']
        expr = command.parameters['Expression']
        expr = self._evaluate_all(expr)
        try:
            items = self.text_evaluator.direct_eval(expr)
            items = [self._object_to_str(item) for item in items]
            return IteratorInfo(iterator_nid, index, items)
        except Exception as e:
            self.logger.error("%s: Could not evaluate {%s} in %s" % (e, command.parameters['Expression'], command.to_plain_text()))
            raise e

    def fetch_next_command(self) -> Optional[event_commands.EventCommand]:
        if self.command_pointer >= len(self.commands):
            return None
        command = self.commands[self.command_pointer]
        self.logger.debug("Processing Event Line: %s", str(command))

        # conditional and loop handling
        if command.nid in ('if',):
            truth = self._get_truth(command)
            if truth:
                self.command_pointer += 1
            else:
                self.command_pointer = self._jump_conditional(self.command_pointer)
            return self.fetch_next_command()
        elif command.nid in ('elif', 'else'):
            # if we naturally navigate to elif/else,
            # implies we finished an if and should skip to the end
            # (because an if fails, we jump directly into the else)
            self.command_pointer = self._find_end(self.command_pointer)
            return self.fetch_next_command()
        elif command.nid == 'end':
            self.command_pointer += 1
            return self.fetch_next_command()
        elif command.nid in ('for'):
            if not self.iterator_stack or self.command_pointer != self.iterator_stack[-1].line:
                self.iterator_stack.append(self._build_iterator(self.command_pointer, command))
            else:
                self.iterator_stack[-1].iterator.next()
            if self.iterator_stack[-1].iterator.empty():
                self.iterator_stack.pop()
                self.command_pointer = self._find_end(self.command_pointer)
            else:
                self.command_pointer += 1
            return self.fetch_next_command()
        elif command.nid in ('endf'): # jump to iterator
            self.command_pointer = self.iterator_stack[-1].line
            return self.fetch_next_command()

        # ignore comments
        if isinstance(command, event_commands.Comment):
            self.command_pointer += 1
            return self.fetch_next_command()

        # normal command
        self.command_pointer += 1
        # evaluate and process command
        parameters, flags = event_commands.convert_parse(command, self._evaluate_all)
        processed_command = command.__class__(parameters, flags, command.display_values)
        return processed_command

    def finished(self):
        return self.command_pointer >= len(self.commands)

    def get_source_line(self, line: int) -> str:
        return str(self.commands[line])

    def get_current_line(self) -> int:
        return self.command_pointer - 1

    def _object_to_str(self, obj) -> str:
        if hasattr(obj, 'uid'):
            return str(obj.uid)
        elif hasattr(obj, 'nid'):
            return str(obj.nid)
        else:
            return str(obj)

    def save(self):
        s_dict = {}
        s_dict['nid'] = self.nid
        s_dict['script'] = self.script
        s_dict['iterators'] = [info.save() for info in self.iterator_stack]
        s_dict['command_pointer'] = self.command_pointer
        return s_dict

    @classmethod
    def restore(cls, s_dict, text_evaluator: TextEvaluator):
        script = s_dict['script']
        nid = s_dict['nid']
        self = cls(nid, script, text_evaluator)
        self.command_pointer = s_dict['command_pointer']
        self.iterator_stack = [IteratorInfo.restore(info_dict) for info_dict in s_dict['iterators']]
        return self
