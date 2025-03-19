#!/usr/bin/python3

import sys

def xor_encrypt(input_file, output_file, key):
    with open(input_file, "rb") as f:
        data = f.read()
    
    encrypted_data = bytes(b ^ key for b in data)

    with open(output_file, "wb") as f:
        f.write(encrypted_data)

    print(f"done: {input_file} → {output_file}")

if __name__ == "__main__":
    if len(sys.argv) != 4:
		print("usage: python encrypt.py <src> <output> <key(0-255)>")
        sys.exit(1)

    input_rom = sys.argv[1]
    output_rom = sys.argv[2]
    key = int(eval(sys.argv[3])) & 0xFF

    xor_encrypt(input_rom, output_rom, key)
