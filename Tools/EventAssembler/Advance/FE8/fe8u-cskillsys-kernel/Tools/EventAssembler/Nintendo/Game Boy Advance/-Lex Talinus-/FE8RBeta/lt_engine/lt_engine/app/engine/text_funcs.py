from typing import List

from app.engine.graphics.text.text_renderer import text_width


def translate(string):
    from app.data.database.database import DB
    if string in DB.translations.keys():
        return DB.translations.get(string).text
    return string


def get_max_width(font_name, text_list):
    return max(text_width(font_name, t) for t in text_list)


def split(font_name, string, num_lines, max_width):
    """
    This takes a string of text and wraps it into multiple lines
    In general, unlike the text_wrap function below, it tries
    to make the lines roughly equal in length,
    and no more than `max_width` pixels in length
    No method to force a skip: (`\n`, {br}) are ignored
    """
    total_length = text_width(font_name, string)
    lines = []
    for line in range(num_lines):
        lines.append([])
    new_line = False
    which_line = 0
    for character in string:
        if new_line and character == ' ':
            which_line += 1
            new_line = False
            if which_line >= len(lines):
                break
            else:
                continue

        if which_line >= len(lines):
            lines.append([])  # This shouldn't happen normally
        lines[which_line].append(character)
        length_so_far = text_width(font_name, ''.join(lines[which_line]))
        if num_lines > 1 and length_so_far >= total_length // num_lines - 5:
            new_line = True
        elif length_so_far >= max_width:
            new_line = True

    return [''.join(line) for line in lines]


def line_chunk(string: str) -> list:
    chunks = string.strip().split(' ')
    chunks = [x for x in chunks if x]  # Remove empty chunks
    return chunks


def line_wrap(font_name, string: str, width: int, pad=False) -> List[str]:
    """
    Adapted from text wrap module
    This takes a string of text and wraps it into multiple lines
    Each line cannot be more than `width` pixels wide
    Splits on spaces.
    No method to force a skip: (`\n`, {br}) are ignored
    """
    assert width > 0
    chunks = line_chunk(string)
    chunks.reverse()
    space_length = text_width(font_name, ' ')

    lines = []
    while chunks:
        # Start the list of chunks that will make up the current line
        # cur_len is the length of all chunks in cur_line
        cur_line = []
        cur_len = 0

        while chunks:
            length = text_width(font_name, chunks[-1])

            # Can at least squeeze this chunk on the current line
            if cur_len + length <= width:
                cur_line.append(chunks.pop())
                cur_len += length
                cur_len += space_length

            # Nope, this line is full
            else:
                break

        if cur_line:
            line = ' '.join(cur_line)
            if pad:
                line += ' '
            lines.append(line)
        else:
            # one chunk is TOO BIG
            lines.append(chunks.pop())

    return lines


if __name__ == '__main__':
    from app.engine.fonts import FONT
    font = FONT['text-white']
    text = 'Hello there, General Kenobi. A pleasure doing business with you'
    lines = line_wrap('text-white', text, 220)
    print(lines)
    lines = split('text-white', text, 2, 220)
    print(lines)
