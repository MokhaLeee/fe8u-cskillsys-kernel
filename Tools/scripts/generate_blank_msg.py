#!/usr/bin/python3

import sys

start = eval(sys.argv[1])
end   = eval(sys.argv[2])

for i in range(start, end):
    print(f"## MSG_{i:04X}")
    print( "[.][X]")
    print( "")
