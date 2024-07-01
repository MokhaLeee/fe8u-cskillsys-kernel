#!/usr/bin/python3

import sys, re

def main(args):
    for line in sys.stdin.readlines():
        line = line.strip()

        line = re.sub('1 ', '0 ', line)
        line = re.sub('3 ', '2 ', line)
        line = re.sub('5 ', '4 ', line)
        line = re.sub('7 ', '6 ', line)
        line = re.sub('9 ', '8 ', line)
        line = re.sub('B ', 'A ', line)
        line = re.sub('D ', 'C ', line)
        line = re.sub('F ', 'E ', line)

        print(f"{line}")

if __name__ == '__main__':
    main(sys.argv)
