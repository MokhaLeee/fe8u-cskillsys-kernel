#!/usr/bin/python3
# -*- coding: UTF-8 -*-

import os, sys, re

with open(sys.argv[1], "r") as fi:
    i = 1

    print("#pragma once\n")

    for line in fi.readlines():
        line = line.strip()

        if (len(line) == 0):
            continue

        if line[0] == '/':
            continue


        print(f"#define {line.split()[0]} {i}")
        i = i + 1

    print(f"#define SID_MAX {i}")
