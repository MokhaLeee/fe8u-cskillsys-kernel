#!/bin/python3

import sys
import numpy as np
from PIL import Image

def load_image(file_path):
    img = Image.open(file_path).convert("P", palette=Image.ADAPTIVE, colors=16)
    return np.array(img), img.getpalette()

def replace_colors(source_palette, target_palette, img_array):
    color_map = []
    for color in img_array.flatten():
        src_color = source_palette[color * 3: color * 3 + 3]
        distances = [np.linalg.norm(np.array(src_color) - np.array(target_palette[i * 3:i * 3 + 3])) for i in range(16)]
        closest_color_index = np.argmin(distances)
        color_map.append(closest_color_index)

    return np.array(color_map).reshape(img_array.shape)

img1_array, palette1 = load_image(sys.argv[1])
img2_array, palette2 = load_image(sys.argv[2])

adjusted_img2_array = replace_colors(palette2, palette1, img2_array)

adjusted_img2 = Image.fromarray(adjusted_img2_array, mode='P')
adjusted_img2.putpalette(palette1)
adjusted_img2.save('adjusted_icon2.png')
