#!/usr/bin/python3

import os, sys

def main(args):
    try:
        sym = args[1]
    except IndexError:
        sys.exit(f"Usage: {sys.args[0]} SYM")

    print(f'\n@ Auto generated by {args[0]}')

    with open(sym, 'r') as f:
        for line in f.readlines():
            ptr  = f"0x{line[0:8]}"
            name = line[9:-1]

            if eval(ptr) & 1:
                print(f"fun {ptr}, {name}")
            else:
                print(f"dat {ptr}, {name}")

    print("")

if __name__ == '__main__':
    main(sys.argv)
