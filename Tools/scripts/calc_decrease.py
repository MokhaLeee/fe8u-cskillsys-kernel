#!/usr/bin/python3
# -*- coding: UTF-8 -*-

i = 0
while 1:
    ref = round(256/(1 - 0.01 * i) - 256)

    print(f"[{i}] = {ref},")

    i = i + 1
    if i >= 100:
        break
