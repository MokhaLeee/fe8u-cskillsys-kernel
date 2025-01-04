#!/bin/python3

import sys

char_map = {
    "a": b'\xe1\xb5\x83', "b": b'\xe1\xb5\x87', "c": b'\xe1\xb6\x9c', "d": b'\xe1\xb5\x88', "e": b'\xe1\xb5\x89', 
    "f": b'\xe1\xb6\xa0', "g": b'\xe1\xb5\x8d', "h": b'\xca\xb0', "k": b'\xe1\xb5\x8f', "n": b'\xe2\x81\xbf', 
    "o": b'\xe1\xb5\x92', "p": b'\xe1\xb5\x96', "q": b'\xe1\xb5\xa0', "r": b'\xca\xb3', "s": b'\xcb\xa2', 
    "t": b'\xe1\xb5\x97', "u": b'\xe1\xb5\x98', "v": b'\xe1\xb5\x9b', "x": b'\xcb\xa3', "y": b'\xca\xb8', 
    "z": b'\xe1\xb6\xbb', "A": b'\xe1\xb4\x80', "B": b'\xca\x99', "C": b'\xe1\xb4\x84', "D": b'\xe1\xb4\x85', 
    "E": b'\xe1\xb4\x87', "F": b'\xd2\x93', "G": b'\xc9\xa2', "H": b'\xca\x9c', "J": b'\xe1\xb4\x8a', 
    "K": b'\xe1\xb4\x8b', "L": b'\xca\x9f', "O": b'\xe1\xb4\x8f', "P": b'\xe1\xb4\x98', "Q": b'\xc7\xab', 
    "R": b'\xca\x80', "S": b'\xe1\x82\xbd', "U": b'\xe1\xb4\x9c', "V": b'\xe1\xb4\xa0', "X": b'\xe1\x83\xaf', 
    "Y": b'\xca\x8f', "Z": b'\xe1\xb4\xa2', " ": b'\xde\xb0', "0": b'\xe2\x81\xb0', "1": b'\xc2\xb9', 
    "2": b'\xc2\xb2', "3": b'\xc2\xb3', "4": b'\xe2\x81\xb4', "5": b'\xe2\x81\xb5', "6": b'\xe2\x81\xb6', 
    "7": b'\xe2\x81\xb7', "8": b'\xe2\x81\xb8', "9": b'\xe2\x81\xb9', ".": b'\xde\x82', ",": b'\xde\x9b', 
    ":": b'\xde\x98', "+": b'\xde\x88', "-": b'\xde\x87', "/": b'\xde\x8a', "(": b'\xde\x86', 
    ")": b'\xde\x89', "'": b'\xde\x8b', "\"": b'\xde\x8a'
}

def bytes_to_char(byte_seq):
    return byte_seq.decode('utf-8')

def to_superscript(text):
    return ''.join(bytes_to_char(char_map.get(c, c).encode('utf-8')) if isinstance(char_map.get(c, c), str) else bytes_to_char(char_map.get(c, c)) for c in text.strip())

def main(args):
    for line in sys.stdin:
        print(to_superscript(line))

if __name__ == '__main__':
    main(sys.argv)
