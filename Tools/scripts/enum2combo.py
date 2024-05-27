#!/usr/bin/python3
# -*- coding: UTF-8 -*-

import os, sys, argparse

with open(sys.argv[1], "r") as fi:
    i = 1
    for line in fi.readlines():
        line = line.strip()

        if (len(line) == 0):
            continue

        if line[0] == '/':
            continue

        print(f"{i:02X}={line.split()[0]}")
        i = i + 1
