#!/usr/bin/python3
# -*- coding: UTF-8 -*-

import os, sys
import dump_skill_icon

prMsgHuffmanTableRoot=0x0006DC
prMsgHuffmanTable=0x0006E0
prMsgTable=0xA26C
prSkillInfo=0xB2A614
skill_sys_rom = "./fe8-kernel-dev.gba"
skill_macro_txt = "./Patches/combo.skills.txt"
default_icon_pal = 0x5996F4

class SkillInfo:
	def __init__(self, sid, macro, desc_str, icon):
		self.sid = sid
		self.macro = macro
		self.desc_str = desc_str
		self.icon = icon

skill_infos = {}

log_settings = {
	'DEBUG': False,
	'INFO': False,
	'WARNING': True,
	'ERROR': True,
	'CRITICAL': True
}

def logprint(level, *args, **kwargs):
	if log_settings.get(level, False):
		print(f"[{level}]\t", *args, **kwargs)

def get_pr_offset(rom_data, addr):
	return int.from_bytes(rom_data[addr + 0:addr + 4], 'little') & 0x7FFFFFF

"""
Huffman decoding
"""
def HUFFMAN_TREE(rom_data):
	return get_pr_offset(rom_data, prMsgHuffmanTable)

def ROOT_NODE(rom_data):
	pr_root = get_pr_offset(rom_data, prMsgHuffmanTableRoot)
	pr_tree = HUFFMAN_TREE(rom_data)
	return (get_pr_offset(rom_data, pr_root) - pr_tree) // 4

def is_leaf(node_data):
	return (node_data & 0xFFFF0000) == 0xFFFF0000

def build_huffman_table(rom_data):
	huffman_table = []

	huffman_tree_base = HUFFMAN_TREE(rom_data)
	huffman_root = ROOT_NODE(rom_data)

	logprint('DEBUG', f"HUFFMAN_TREE=0x{huffman_tree_base:X}, huffman_root=0x{huffman_root:X}")

	off = huffman_tree_base

	for i in range(huffman_root + 1):
		node_data = int.from_bytes(rom_data[off + 0:off + 4], 'little')

		logprint('DEBUG', f"node:0x{i:03X}=0x{node_data:08X} ")
		if is_leaf(node_data):
			logprint('DEBUG', f"leaf:0x{(node_data & 0xFFFF):04X}")
		else:
			logprint('DEBUG', f"left:0x{(node_data & 0xFFFF):04X}, right:0x{((node_data >> 16) & 0xFFFF):04X}")

		huffman_table.append(node_data)
		off = off + 4

	return huffman_table

def decode_data(rom_data, huffman_table, off):
	decoded_data = []
	bit_pos = -1
	cur_node = ROOT_NODE(rom_data)

	while True:
		if bit_pos < 0:
			current_byte = rom_data[off]
			off = off + 1
			bit_pos = 7

		bit = current_byte & 1
		current_byte = current_byte >> 1
		bit_pos = bit_pos - 1

		logprint('DEBUG', bit)

		if bit == 0:
			cur_node = huffman_table[cur_node] & 0xFFFF # left
		else:
			cur_node = (huffman_table[cur_node] >> 16) & 0xFFFF

		# check for leaf
		if is_leaf(huffman_table[cur_node]):
			data = huffman_table[cur_node] & 0xFFFF

			logprint('INFO', f"get leaf: 0x{cur_node:04X}=0x{data:X}")

			decoded_data.append(data)
			if data == 0:
				break

			cur_node = ROOT_NODE(rom_data)

	return decoded_data

"""
Text process
"""
def parse_string_shiftjis_core(data, cur_idx):

	appr_len = 0

	u16_data = data[cur_idx]

	if u16_data == 0:
		output = ""
	elif u16_data == 1:
		output = " "
	elif u16_data == 2:
		output = ""
	elif u16_data == 31:
		output = ""
	else:
		output = f"[0x{u16_data:04X}]"
		# print("not supported: " + f"{hex(u16_data)}")
		# sys.exit(1)

	return appr_len, output

def parse_ctrl_string_utf8_core(byte_array, cur_idx):
	ch1 = byte_array[cur_idx]
	appr_len = 1

	try:
		ch2 = byte_array[cur_idx + 1]
	except IndexError:
		ch2 = 0

	try:
		ch3 = byte_array[cur_idx + 2]
	except IndexError:
		ch3 = 0

	if ch1 == 0x10:
		appr_len = 3
		u16_data = [ch1, ch2 | ch3 << 8]
	elif ch1 == 0x80:
		appr_len = 2
		u16_data = [ch1, ch2]
	else:
		u16_data = [ch1]

	unused, output = parse_string_shiftjis_core(u16_data, 0)

	return appr_len, output

def data_to_bytes(decoded_data):
	byte_array = bytearray()

	for value in decoded_data:
		lo = value & 0xFF
		hi = (value & 0xFF00) >> 8

		byte_array.append(lo)
		if hi != 0:
			byte_array.append(hi)

	return byte_array

def get_utf8_len(cod):
	if (0b11111000 & cod) == 0b11110000:
		return 4
	elif ((0b11110000 & cod) == 0b11100000):
		return 3
	elif ((0b11100000 & cod) == 0b11000000):
		return 2
	elif ((0b10000000 & cod) == 0x0):
		return 1
	else:
		print("parse utf8 length failed: " + f"{hex(cod)}")
		sys.exit(1)

def parse_string_utf8(decoded_data):
	decoded_bytes = data_to_bytes(decoded_data)

	parsed_string = ""
	index = 0
	while index < len(decoded_bytes):
		decoded = False

		ch1 = decoded_bytes[index]

		try:
			decoded_ch = decoded_bytes[index:index+1].decode('utf-8')
			decoded = True
		except UnicodeDecodeError:
			decoded = False

		if decoded and ch1 >= 0x20 and ch1 < 0x7F:
			parsed_string += decoded_ch
			index += get_utf8_len(ch1)
		else:
			appr_len, output = parse_ctrl_string_utf8_core(decoded_bytes, index)
			index += appr_len
			parsed_string += output

	stripped = parsed_string.replace("#", "[HASH]") # :\
	return stripped.lstrip('\n')

def decode_single_string(msg_idx, rom_data, huffman_table):
	text_table = get_pr_offset(rom_data, prMsgTable)

	pr_encodered_string = get_pr_offset(rom_data, text_table + 4 * msg_idx)
	decoded_data = decode_data(rom_data, huffman_table, pr_encodered_string)
	parsed_string = parse_string_utf8(decoded_data)
	return parsed_string

"""
Icon
"""
def get_sid_icon_md_base64(rom_data, sid):
	skill_info_base = get_pr_offset(rom_data, prSkillInfo)
	skill_info = skill_info_base + 8 * sid
	skill_icon_addr = int.from_bytes(rom_data[skill_info + 0:skill_info + 4], 'little')
	img_base64 = dump_skill_icon.dump_icon_base64(rom_data, skill_icon_addr, default_icon_pal)
	# markdown_content = f'![image](data:image/png;base64,{img_base64})'
	html_content = f'<img src="data:image/png;base64,{img_base64}" alt="Base64 Image" />'
	return html_content

def get_sid_icon_img(rom_data, sid):
	skill_info_base = get_pr_offset(rom_data, prSkillInfo)
	skill_info = skill_info_base + 8 * sid
	skill_icon_addr = int.from_bytes(rom_data[skill_info + 0:skill_info + 4], 'little')

	img = dump_skill_icon.dump_icon_img(rom_data, skill_icon_addr, default_icon_pal)
	return img

"""
SkillSys
"""
def decode_skill_desc(rom_data, sid, huffman_table):
	skill_info_base = get_pr_offset(rom_data, prSkillInfo)
	skill_info = skill_info_base + 8 * sid
	skill_desc = int.from_bytes(rom_data[skill_info + 6:skill_info + 8], 'little')
	return decode_single_string(skill_desc, rom_data, huffman_table)

def find_skill_macro(sid):
	bonus_dict = {}
	with open(skill_macro_txt, 'r', encoding='utf-8') as file:
		for line in file:
			line = line.strip()
			if '=' in line:
				key, macro = line.split('=', 1)

				if int(key, 16) == sid:
					return macro

	return None

def main(args):
	with open(skill_sys_rom, 'rb') as f:
		rom_data = f.read()

	huffman_table = build_huffman_table(rom_data)

	for sid in range(1, 0x400):
		sid_macro = find_skill_macro(sid)

		if sid_macro is None:
			skill_infos[sid] = None
		else:
			skill_icon_fpath = f"gfx/SkillIcon_{sid_macro}.png"
			img = get_sid_icon_img(rom_data, sid)
			img.save(f"./docs/{skill_icon_fpath}")

			skill_infos[sid] = SkillInfo(sid, sid_macro,
						decode_skill_desc(rom_data, sid, huffman_table),
						f"![image]({skill_icon_fpath})")

	# format print
	print("## Equipable skills")
	print("| icon | name | desc |")
	print("| :-------- | :-------- | :-------- |")

	for sid in range(1, 0x100):
		info = skill_infos[sid]
		if info is not None:
			print(f"{info.icon} | {info.macro} | {info.desc_str.split(':', 1)[1]} |")

	print("## Item-boundable skills")
	print("| icon | name | desc |")
	print("| :-------- | :-------- | :-------- |")

	for sid in range(0x300, 0x400):
		info = skill_infos[sid]
		if info is not None:
			print(f"{info.icon} | {info.macro} | {info.desc_str.split(':', 1)[1]} |")

	print("## Generic skills")
	print("| icon | name | desc |")
	print("| :-------- | :-------- | :-------- |")

	for sid in range(0x100, 0x300):
		info = skill_infos[sid]
		if info is not None:
			print(f"{info.icon} | {info.macro} | {info.desc_str.split(':', 1)[1]} |")

if __name__ == '__main__':
	main(sys.argv)
