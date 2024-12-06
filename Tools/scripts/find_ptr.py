#!/bin/python3
import struct
import sys
import protect_ptr

_ROM = "fe8.gba"

def find_ptr(rom, start_value, end_value):
    with open(rom, 'rb') as f:
        offset = 0
        while True:
            bytes_read = f.read(4)
            if len(bytes_read) < 4:
                break

            data = struct.unpack('<I', bytes_read)[0]

            if start_value <= data <= end_value:

                print(f"    ORG 0x{offset:06X}")
                print(f"    POIN Repointed_ptr")
                protect_ptr.protect_ptr(offset)

                print("")

            offset += 4

def main(args):
    try:
        start = eval(args[1])
    except IndexError:
        sys.exit(f"Usage: {args[0]} START [END]")

    try:
        end = eval(args[2])
    except IndexError:
        end = start

    find_ptr(_ROM, start, end)

if __name__ == '__main__':
    main(sys.argv)
