import functools
import re


RAW_NEWLINE = '\u2029'
SHIFT_NEWLINE = '\u2028'

def convert_raw_text_newlines(s: str) -> str:
    return s.replace('\u2029', '\n')

def get_next_name(name, names, infix='_'):
    if name not in names:
        return name
    else:
        # Remove the _1 when generating additional names
        name = re.sub(('%s' % infix) + r'\d+$', '', name)
        counter = 1
        while True:
            test_name = name + ('%s%s' % (infix, counter))
            if test_name not in names:
                return test_name
            counter += 1

def get_next_int(name, names):
    if name not in names:
        return name
    else:
        counter = 1
        while True:
            test_name = str(counter)
            if test_name not in names:
                return test_name
            counter += 1

def get_next_generic_nid(name: str, names):
    if name not in names:
        return name
    elif is_int(name):
        counter = int(name) + 1
        while True:
            test_name = str(counter)
            if test_name not in names:
                return test_name
            counter += 1
    else:
        return get_next_name(name, names)

def find_last_number(s: str):
    last_number = re.findall(r'\d+$', s)
    if last_number:
        return int(last_number[-1])
    return None

def get_prefix(s: str):
    last_number = re.findall(r'\d+', s)
    if last_number:
        idx = re.search(r'\d+', s).span(0)[0]
        return s[:idx]
    else:
        idx = s.index('.')
        return s[:idx]

def intify(s: str) -> list:
    vals = s.split(',')
    return [int(i) for i in vals]

def skill_parser(s: str) -> list:
    if s is not None:
        each_skill = [each.split(',') for each in s.split(';')]
        split_line = [[int(s_l[0]), s_l[1]] for s_l in each_skill]
        return split_line
    else:
        return []

def is_int(s: str) -> bool:
    try:
        int(s)
        return True
    except ValueError:
        return False
    except TypeError:
        return False

def is_float(s: str) -> bool:
    try:
        float(s)
        return True
    except ValueError:
        return False
    except TypeError:
        return False

def camel_case(s: str) -> str:
    return functools.reduce(lambda a, b: a + ((b.upper() == b and (a and a[-1].upper() != a[-1])) and (' ' + b) or b), s, '')

def ignore_numbers(s: str) -> str:
    s = ''.join([c for c in s if not c.isdigit()])
    return s.strip()

def camel_to_snake(name: str) -> str:
    """
    https://stackoverflow.com/questions/1175208/elegant-python-function-to-convert-camelcase-to-snake-case
    """
    name = re.sub('(.)([A-Z][a-z]+)', r'\1_\2', name)
    return re.sub('([a-z0-9])([A-Z])', r'\1_\2', name).lower()

def snake_to_readable(s: str) -> str:
    return s.replace('_', ' ').title()

def nested_expr(s, opener, closer):
    # Returns a nested list
    assert opener != closer
    assert len(opener) == 1
    assert len(closer) == 1
    main_list = []
    list_stack = [main_list]
    current_list = main_list
    for character in s:
        if character == opener:
            new_list = []
            list_stack.append(new_list)
            current_list.append(new_list)
            current_list = new_list
        elif character == closer:
            list_stack.pop()
            current_list = list_stack[-1]
        else:
            current_list.append(character)
    return main_list[0]

def matched_expr(s: str, opener: str, closer: str):
    # returns all strings bounded by balanced openers, closers
    # e.g. "{bac{def}jk} {lmno}" would return "[bac{def}jk, lmno]", not "[bac{def, lmno]"
    assert opener != closer
    assert len(opener) == 1
    assert len(closer) == 1
    all_strs = []
    curr = ""
    unclosed = 0
    for character in s:
        if unclosed > 0:
            curr += character
        if character == closer and unclosed > 0:
            unclosed -= 1
            if unclosed == 0:
                all_strs.append(curr)
                curr = ""
        elif character == opener:
            if unclosed == 0:
                curr += character
            unclosed += 1
    return all_strs

def matched_block_expr(s: str, opener: str, closer: str):
    # returns all strings bounded by balanced openers, closers
    # inclduding content not found within the openers and closers
    # e.g. "Hi{bac{def}jk}Waffle{lmno}" would return "[Hi, {bac{def}jk}, Waffle, {lmno}]""
    assert opener != closer
    assert len(opener) == 1
    assert len(closer) == 1
    all_strs = []
    curr = ""
    unclosed = 0
    for character in s:
        if character == closer and unclosed > 0:
            unclosed -= 1
            curr += character
            if unclosed == 0:
                all_strs.append(curr)
                curr = ""
        elif character == opener:
            if unclosed == 0:
                all_strs.append(curr)
                curr = ""
            curr += character
            unclosed += 1
        else:
            curr += character
    return all_strs

def remove_prefix(text: str, prefix: str):
    if text.startswith(prefix):
        return text[len(prefix):]
    return text

def remove_all_matched(s: str, opener: str, closer: str):
    """
    usage: `{d:{eval:f}.{eval:y}.` becomes `{d:..` - useful for determining which level of a nested eval we're in
    https://stackoverflow.com/questions/37528373/how-to-remove-all-text-between-the-outer-parentheses-in-a-string
    """
    assert opener != closer
    assert len(opener) == 1
    assert len(closer) == 1
    n = 1  # run at least once
    rstr = '\\' + opener + '[^' + opener + closer + ']*\\' + closer
    while n:
        s, n = re.subn(rstr, '', s)  # remove non-nested/flat balanced parts
    return s


_MIRRORED_BRACKETS = {
    '[': ']',
    '{': '}',
    '(': ')',
}
for k, v in _MIRRORED_BRACKETS.copy().items():
    _MIRRORED_BRACKETS[v] = k

def mirror_bracket(c: str) -> str:
    return _MIRRORED_BRACKETS.get(c, None)

if __name__ == '__main__':
    # print(camel_to_snake("Direction"))
    # print(camel_to_snake("EntityID"))
    # print(camel_to_snake("Node1"))
    # print(camel_to_snake("OverworldNodeNid"))

    # def recursive_parse(parse_list) -> str:
    #     copy = [""] * len(parse_list)
    #     for idx, nested in enumerate(parse_list):
    #         if isinstance(nested, list):
    #             recursively_parsed = recursive_parse(nested)
    #             copy[idx] = recursively_parsed
    #         else:
    #             copy[idx] = nested
    #     return str('{' + ''.join(copy) + '}')

    # test_str = "See, {e:game.get_unit('{e:unit.nid}')}."
    # to_evaluate = re.findall(r'\{.*\}', test_str)
    # for to_eval in to_evaluate:
    #     res = nested_expr(to_eval, "{", "}")
    #     print(res)
    #     parsed = recursive_parse(res)
    #     print(parsed)
    print(matched_expr('{d:MercenaryHiringList.{v:MercenaryHireChoice}.Class}', '{', '}'))
    print(remove_all_matched('{d:{eval}.{var}.', '{', '}'))
