#!/usr/bin/python3
# -*- coding: UTF-8 -*-

import os, sys, argparse

start = eval(sys.argv[1])
end   = eval(sys.argv[2])

with open(sys.argv[3], "r") as fi:
    i = start + 1

    for line in fi.readlines():
        line = line.strip()

        if (len(line) == 0):
            continue

        if line[0] == '/':
            continue

        print(f"{(i):02X}={line.split()[0][4:]}")

        i = i + 1
        if i >= end:
            break
