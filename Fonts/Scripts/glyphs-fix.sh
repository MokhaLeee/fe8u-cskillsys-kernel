#!/bin/bash

for file in $(find ../Glyph/ | grep FontItem); do
    ./glyph-fix.py -i $file &
done
