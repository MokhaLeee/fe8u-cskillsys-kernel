#!/usr/bin/python3
# -*- coding: UTF-8 -*-

import os, sys, argparse

start = eval(sys.argv[1])

with open(sys.argv[2], "r") as fi:
    i = 1
    for line in fi.readlines():
        line = line.strip()

        if (len(line) == 0):
            continue

        if line[0] == '/':
            continue

        print(f"{(i + start):02X}={line.split()[0][4:]}")

        i = i + 1
        if i >= 0xFF:
            break
