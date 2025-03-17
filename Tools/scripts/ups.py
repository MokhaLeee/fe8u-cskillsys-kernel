#!/usr/bin/env python3
# UPS patcher based on UPS module for Rom Patcher JS v20180930 - Marc Robledo 2017-2018
# Author: MinN

import sys
import zlib

CHECKSUM_TARGET = True  # check the target's checksum
CHECKSUM_PATCH = False  # don't check the patch's checksum because NUPS BUG


class ChecksumError(Exception):
    pass


def encode_vlv(data):
    buffer = bytearray()
    while True:
        x = data & 0x7f
        data = data >> 7
        if data == 0:
            buffer.append(0x80 | x)
            break
        buffer.append(x)
        data -= 1
    return buffer


def read_vlv(array, ptr):
    data = 0
    shift = 1
    while True:
        x = array[ptr]
        ptr += 1
        if x == 2 ** 32:
            raise Exception('Can\'t read UPS VLV at 0x{:X}'.format(ptr-1))
        data += (x & 0x7f) * shift
        if (x & 0x80) != 0:
            break
        shift = shift << 7
        data += shift
    return data, ptr


def get_checksum(patch):
    try:
        with open(patch, "rb") as file:
            patch_file = file.read()
        checksum_input = int.from_bytes(patch_file[-12:-8], byteorder="little")
        checksum_output = int.from_bytes(patch_file[-8:-4], byteorder="little")
        checksum_patch = int.from_bytes(patch_file[-4:], byteorder="little")
        return checksum_input, checksum_output, checksum_patch
    except Exception:
        return -1


def checksum(file):
    with open(file, "rb") as f:
        return zlib.crc32(f.read())


def patch_ups(source, patch, target):
    with open(source, "rb") as file:
        source_file = file.read()
    with open(patch, "rb") as file:
        patch_file = file.read()

    checksum_input = int.from_bytes(patch_file[-12:-8], byteorder="little")
    if checksum_input != zlib.crc32(source_file):
        raise ChecksumError
    checksum_patch = int.from_bytes(patch_file[-4:], byteorder="little")
    if CHECKSUM_PATCH and checksum_patch != zlib.crc32(patch_file[:-4]):
        raise ChecksumError

    ptr = 4
    size_input, ptr = read_vlv(patch_file, ptr)
    size_output, ptr = read_vlv(patch_file, ptr)

    target_file = bytearray(source_file).ljust(size_output, b'\0')

    if len(source_file) != size_input:
        raise ChecksumError

    rom_offset = 0
    while ptr < len(patch_file) - 12:
        offset, ptr = read_vlv(patch_file, ptr)
        diff = []
        while patch_file[ptr] != 0:
            diff.append(patch_file[ptr])
            ptr += 1
        ptr += 1

        rom_offset += offset
        for i in range(len(diff)):
            target_file[rom_offset] = target_file[rom_offset] ^ diff[i]
            rom_offset += 1
        rom_offset += 1
    checksum_output = int.from_bytes(patch_file[-8:-4], byteorder="little")
    if CHECKSUM_TARGET and checksum_output != zlib.crc32(target_file):
        raise ChecksumError

    target_fd = open(target, "wb")
    target_fd.write(target_file)
    target_fd.close()


def make_ups(original, modified, target):
    with open(original, "rb") as file:
        original_file = file.read()
    with open(modified, "rb") as file:
        modified_file = file.read()
    target_file = open(target, "wb")

    po = 0
    pm = 0
    last_diff = 1
    diff_list = []
    while pm < len(modified_file):
        b1 = original_file[po] if po < len(original_file) else 0
        b2 = modified_file[pm]

        po += 1
        pm += 1
        if b1 != b2:
            curr_diff = pm
            xor = bytearray()

            while b1 != b2:
                xor.append(b1 ^ b2)
                if pm == len(modified_file):
                    break
                b1 = original_file[po] if po < len(original_file) else 0
                b2 = modified_file[pm]
                po += 1
                pm += 1

            diff_list.append((curr_diff - last_diff, xor))
            last_diff = curr_diff + len(xor) + 1

    buffer = bytearray()
    buffer += b"UPS1"
    buffer += encode_vlv(len(original_file))
    buffer += encode_vlv(len(modified_file))
    for offset, xor in diff_list:
        buffer += (encode_vlv(offset))
        buffer += xor
        buffer += b"\0"
    buffer += zlib.crc32(original_file).to_bytes(4, byteorder="little")
    buffer += zlib.crc32(modified_file).to_bytes(4, byteorder="little")
    buffer += zlib.crc32(buffer).to_bytes(4, byteorder="little")
    target_file.write(buffer)
    target_file.close()


def help():
    print("Commands:")
    print("ups.py patch unmodified_rom patch_file   target")
    print("ups.py make  unmodified_rom modified_rom target")


def main():
    if len(sys.argv) < 4:
        help()
    elif sys.argv[1] == "patch":
        patch_ups(sys.argv[2], sys.argv[3], sys.argv[4])
    elif sys.argv[1] == "make" and len(sys.argv) >= 4:
        make_ups(sys.argv[2], sys.argv[3], sys.argv[4])
    else:
        help()


if __name__ == '__main__':
    main()
