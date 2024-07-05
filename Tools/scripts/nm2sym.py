#!/usr/bin/python3

import sys

def main(args):

    for line in sys.stdin.readlines():
        print(f"{line[0:8]} {line[11:-1]}")

if __name__ == '__main__':
    main(sys.argv)
