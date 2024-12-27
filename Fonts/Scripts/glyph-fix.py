#!/usr/bin/python3
# -*- coding: UTF-8 -*-

import os, sys, argparse
import numpy as np
from numpy import *
from PIL import Image

def main():
    argParser = argparse.ArgumentParser(description = "")
    argParser.add_argument("-i", "--input", help = "input list file")
    arguments = argParser.parse_args()

    fpath = os.path.abspath(arguments.input)
    img = Image.open(fpath)
    arr = np.array(img, dtype=np.uint8)

    fixed = 0

    for i in range(1, 0x10 - 1):
        for j in range(1, 0x10 - 1):
            px = arr[i, j]
            if px == 2:

                [y, x] = i - 1, j
                _px = arr[y, x]
                if (_px == 0):
                    arr[y, x] = 3
                    fixed = 1

                [y, x] = i + 1, j
                _px = arr[y, x]
                if (_px == 0):
                    arr[y, x] = 3
                    fixed = 1

                [y, x] = i, j - 1
                _px = arr[y, x]
                if (_px == 0):
                    arr[y, x] = 3
                    fixed = 1

                [y, x] = i, j + 1
                _px = arr[y, x]
                if (_px == 0):
                    arr[y, x] = 3
                    fixed = 1
    """ 
    for i in range(1, 0x10 - 1):
        for j in range(1, 0x10 - 1):
            print(arr[i, j], end = '')
        print()
    """
    if fixed == 1:
        print("Fix {}".format(fpath))
        _img = Image.fromarray(arr)
        _img.putpalette(img.getpalette())
        _img.save(fpath)

if __name__ == '__main__':
    main()
