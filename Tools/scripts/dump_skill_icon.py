#!/bin/python3

import sys, struct, os
import array
from PIL import Image
import base64
from io import BytesIO

skill_sys_rom = "./fe8-kernel-dev.gba"
default_icon_pal = 0x5996F4

def read_palette_from_binary(pal_bytes):
	palette = []
	palette_len = len(pal_bytes)
	for i in range(0, palette_len, 2):
		color = struct.unpack('<H', pal_bytes[i:i+2])[0]
		r = (color & 0x1F) << 3 
		g = ((color >> 5) & 0x1F) << 3
		b = ((color >> 10) & 0x1F) << 3

		palette.append(r)
		palette.append(g)
		palette.append(b)

	return palette

def dump_icon_img(rom_data, img_addr, pal_addr):
	img_addr &= 0x00FFFFFF
	pal_addr &= 0x00FFFFFF

	ntiles_x = 2
	ntiles_y = 2

	width  = ntiles_x * 8
	height = ntiles_y * 8
	img = Image.new('P', (width, height))

	tiles_8x8 = []
	pixels = [0] * (width * height)

	# pal
	pal_bytes = rom_data[pal_addr : (pal_addr + 0x20)]
	pal_data = read_palette_from_binary(pal_bytes)

	# img
	img_bytes = rom_data[img_addr : (img_addr + 0x80)]
	img_data = array.array('B', img_bytes)

	tiles_8x8 = []

	for tile_idx in range(4):
		_tile = []

		for y in range(8):
			for x in range(0, 8, 2):
				offset = tile_idx * (8 * 8 // 2) + y * (8 // 2) + (x // 2)

				byte = img_data[offset]

				pixel1 = byte & 0x0F
				pixel2 = (byte >> 4) & 0x0F

				_tile.append(pixel1)
				_tile.append(pixel2)

		tiles_8x8.append(_tile)

	for tile_idx in range(ntiles_x * ntiles_y):
		base_y = tile_idx // ntiles_x
		base_x = tile_idx % ntiles_x

		real_tile = tiles_8x8[tile_idx]

		for iy in range(8):
			for ix in range(8):
				real_x = ix + base_x * 8
				real_y = iy + base_y * 8

				# print(f"[{tile_idx}] [{real_x} {real_y} {ix} {iy}] = 0x{real_tile[iy * 8 + ix]:04X}")

				pixels[real_x + 0 + real_y * 16] = real_tile[iy * 8 + ix]

	img.putpalette(pal_data)
	img.putdata(pixels)
	return img

def img_to_base64(image):
	buffered = BytesIO()
	image.save(buffered, format="PNG")
	img_base64 = base64.b64encode(buffered.getvalue()).decode('utf-8')
	return img_base64

def dump_icon_base64(rom_data, img_addr, pal_addr):
	img = dump_icon_img(rom_data, img_addr, pal_addr)
	img_base64 = img_to_base64(img)
	return img_base64

if __name__ == '__main__':
	with open(skill_sys_rom, 'rb') as f:
		rom_data = f.read()

	prefix   = sys.argv[1]
	img_addr = eval(sys.argv[2])
	pal_addr = 0x5996F4 # default_icon_pal

	img = dump_icon_img(rom_data, img_addr, pal_addr)
	img_to_base64(img)
	img.save(f"{prefix}.png")
