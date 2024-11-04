#!/usr/bin/env python3
# Author: https://github.com/MokhaLeee

import os, re, sys, struct
import huffman

MSG_LENGTH = 0x1400
DEBUG = False

RE_MSGIDX = re.compile(r"^#([0-9a-fA-Fx]+)")
RE_MACRO = re.compile(r"^##\s*(\w+)")
RE_INCLUDE = re.compile(r'#include\s+"([^"]+)"')

class Msg:
    def __init__(self, idx, data, definiation=None):
        self.idx = idx
        self.data = data
        if definiation is not None:
            self.definiation = definiation
        else:
            self.definiation = f"MSG_{idx:03X}"

all_data = []
msg_refs = [-1] * MSG_LENGTH

def debug_printf(fmt, *args):
    if DEBUG:
        print(fmt % args)

def GenerateFreqTable(data):
    freq_table = [0] * 0x10000

    for value in data:
        freq_table[value] += 1

    return freq_table

def load_control_chars(parse_ref):
    control_chars = {}
    with open(parse_ref, 'r', encoding='utf-8') as file:
        for line in file:
            match = re.match(r'\[(.*?)\] = (.+)', line.strip())
            if match:
                key = match.group(1)
                values = tuple(int(v.strip(), 0) for v in match.group(2).split(','))
                control_chars[key] = values
    return control_chars

def text_preprocess(text):
    # remove comments
    text = re.sub(r'^//.*', '', text, flags=re.MULTILINE)
    text = re.sub(r'/\*.*?\*/', '', text, flags=re.DOTALL)
    return text

def text_to_utf8_u16_array(text, control_chars):
    pattern = re.compile(r'\[(.*?)\]')

    text = text_preprocess(text)

    # step1: generate byte array
    byte_array = bytearray()
    pos = 0
    while pos < len(text):
        match = pattern.search(text, pos)
        if match:
            preceding_text = text[pos:match.start()]
            if preceding_text:
                byte_array.extend(bytearray(preceding_text, 'utf-8'))

            control_char = match.group(1)
            if control_char in control_chars:
                for ctrl_ch in control_chars[control_char]:
                    byte_array.append(ctrl_ch)

            pos = match.end()
        else:
            remaining_text = text[pos:]
            if remaining_text:
                byte_array.extend(bytearray(preceding_text, 'utf-8'))
            break

    # step2: generate u16 array
    u16_array = []
    pos = 0
    while pos < len(byte_array):
        # same as textencode::compress_string()
        _ch = byte_array[pos]

        if _ch == 0x80:
            ch1 = byte_array[pos]
            ch2 = byte_array[pos + 1]
            pos = pos + 2

            u16_array.append(ch1)
            u16_array.append(ch2)
        elif _ch == 0x10:
            ch1 = byte_array[pos]
            ch2 = byte_array[pos + 1]
            ch3 = byte_array[pos + 2]
            pos = pos + 3

            u16_array.append(ch1)
            u16_array.append(ch2 | (ch3 << 8))
        elif _ch == 0x23 or _ch == 0x7F or _ch == 0xE9:
            ch1 = byte_array[pos]
            pos = pos + 1

            u16_array.append(ch1)
        elif _ch >= 0x20:
            ch1 = byte_array[pos]
            ch2 = byte_array[pos + 1]
            pos = pos + 2

            u16_array.append(ch1 | (ch2 << 8))
        else:
            ch1 = byte_array[pos]
            pos = pos + 1

            u16_array.append(ch1)

    if u16_array[-1] != 0:
        u16_array.append(0)

    return u16_array

def text_to_u16_array(text, control_chars, encoding_method):
    if encoding_method == 'cp932':
        pass
    else:
        return text_to_utf8_u16_array(text, control_chars)

def process_file(messages, file_path, control_chars, encoding_method, index=0):
    current_index = index

    with open(file_path, 'r', encoding='utf-8') as f:
        lines = f.readlines()

    i = 0
    while i < len(lines):
        stripped = lines[i].strip()

        include_match = RE_INCLUDE.match(stripped)
        if include_match:
            include_file = include_match.group(1)
            include_path = os.path.join(os.path.dirname(file_path), include_file)

            if not os.path.isfile(include_path):
                raise FileNotFoundError(f"Error: File '{include_path}' does not exist.")

            messages, current_index = process_file(messages, include_path, control_chars, encoding_method, current_index)

            i += 1
            continue

        directive_match = RE_MSGIDX.match(stripped)
        if directive_match:
            current_index = int(directive_match.group(1), 16)
            i += 1
            text = []
            while i < len(lines) and not lines[i].startswith("#"):
                text.append(lines[i].rstrip('\n'))
                i += 1
            text = ''.join(text)
            data = text_to_u16_array(text, control_chars, encoding_method)

            if msg_refs[current_index] == -1:
                debug_printf(f"Register to ref: idx=0x{current_index:04X}, val=0x{len(messages):04X}")
                msg_refs[current_index] = len(messages)
                messages.append(Msg(current_index, data))
            else:
                debug_printf(f"Replace for ref: idx=0x{current_index:04X}, val=0x{len(messages):04X}")
                messages[msg_refs[current_index]] = Msg(current_index, data)

            current_index += 1
        elif stripped.startswith('##'):
            macro_match = RE_MACRO.match(stripped)
            if macro_match:
                macro = macro_match.group(1)
                i += 1
                text = []
                while i < len(lines) and not lines[i].startswith("#"):
                    text.append(lines[i].rstrip('\n'))
                    i += 1
                text = ''.join(text)
                data = text_to_u16_array(text, control_chars, encoding_method)

                if msg_refs[current_index] == -1:
                    debug_printf(f"Register to ref: idx=0x{current_index:04X}, val=0x{len(messages):04X}")
                    msg_refs[current_index] = len(messages)
                    messages.append(Msg(current_index, data, macro))
                else:
                    debug_printf(f"Replace for ref: idx=0x{current_index:04X}, val=0x{len(messages):04X}")
                    messages[msg_refs[current_index]] = Msg(current_index, data, macro)

                current_index += 1
        else:
            i += 1

    return messages, current_index

def write_header(messages, header_file):
    header_file.write("#ifndef MSG_H\n#define MSG_H\n\n")
    header_file.write(f"// Auto generated by {sys.argv[0]}\n\n")
    for msg in messages:
        header_file.write(f"#define {msg.definiation} 0x{msg.idx:04X}\n")

    header_file.write(f"\n#define MSG_COUNT 0x{(len(messages)):04X}\n")
    header_file.write("\n#endif /* MSG_H */\n")

def write_all_compressed_data(messages, code_table, data_file):
    for msg in messages:
        data_file.write(f"CompressedText_{msg.definiation}:\n")
        data_file.write( "    BYTE ")
        for data in huffman.CompressData(msg.data, code_table):
            data_file.write(f"0x{data:02X} ")

        data_file.write("\n")

def write_text_table(messages, data_file):
    data_file.write("\n")
    data_file.write("PUSH\n")
    data_file.write("ORG TextTable\n")
    data_file.write("gMsgTableRe:\n")
    for i, msg in enumerate(messages):
        if i % 8 == 0:
            data_file.write("\n    POIN ")
        else:
            data_file.write(" ")

        data_file.write(f"CompressedText_{msg.definiation}")

    data_file.write("\n")
    data_file.write("    ASSERT (TextTableEnd - CURRENTOFFSET)\n")
    data_file.write("POP\n")

def write_huffman_table(huffman_table, data_file):
    data_file.write("ALIGN 4\n")
    data_file.write("gMsgHuffmanTableRe:")
    for i, branch in enumerate(huffman_table):
        if i % 8 == 0:
            data_file.write("\n    WORD ")
        else:
            data_file.write(" ")

        data_file.write(f"0x{branch:08X}")

    data_file.write("\n")
    data_file.write("ALIGN 4\n")
    data_file.write("gMsgHuffmanTableRootRe:\n")
    data_file.write(f"POIN (gMsgHuffmanTableRe + 0x{(len(huffman_table) - 1) * 4:04X})\n")
    data_file.write("\n")

def dump_msg(messages):
    for msg in messages:
        print(f"MSG_{msg.idx:04X}: ", end="")
        for data in msg.data:
            print(f"0x{data:04X} ", end="")
        print("")

def main(args):
    try:
        input_fpath = args[0]
        input_parse_ref = args[1]
        output_data = args[2]
        output_header = args[3]
        encoding_method = args[4]

    except IndexError:
        sys.exit(f"Usage: {sys.argv[0]} <text-main> <defs> <output_data> <output_header> <'cp932' or 'utf8'>")

    control_chars = load_control_chars(input_parse_ref)
    messages = []
    messages, _unused_ = process_file(messages, input_fpath, control_chars, encoding_method)

    for msg in messages:
        all_data.extend(msg.data)

    # generate huffman
    freq_table = GenerateFreqTable(all_data)

    huff_tree = huffman.BuildHuffmanTree(freq_table)

    huffman_table = huffman.BuildHuffmanTable()
    code_table = huffman.build_code_table(huff_tree)

    # output
    with open(output_header, 'w', encoding='utf-8') as header_file:
        write_header(messages, header_file)

    with open(output_data, 'w', encoding='utf-8') as data_file:
        write_huffman_table(huffman_table, data_file)
        data_file.write("{\n")
        write_all_compressed_data(messages, code_table, data_file)
        write_text_table(messages, data_file)
        data_file.write("\n")
        data_file.write("}\n")
if __name__ == '__main__':
	main(sys.argv[1:])
