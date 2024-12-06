#!/usr/bin/python3

import os, sys

def parse_address_ranges(sym):
    """
    Parses a list of address-symbol mappings into sorted address ranges.

    Args:
        data (str): Multiline string containing address-symbol mappings.

    Returns:
        list of tuple: A list of (address, symbol) pairs, sorted by address.
    """
    address_symbol_pairs = []

    with open(sym, 'r') as f:
        for line in f.readlines():
            line = line.strip()
            parts = line.split()
            if len(parts) == 2:
                address = eval(f"0x{parts[0]}") & 0x07FFFFFF
                symbol = parts[1]
                address_symbol_pairs.append((address, symbol))

    return sorted(address_symbol_pairs, key=lambda x: x[0])

def find_address_range(address, ranges):
    """
    Finds the range within which a given address falls.

    Args:
        address (int): The address to locate.
        ranges (list of tuple): List of (address, symbol) pairs.

    Returns:
        tuple: A tuple containing the lower and upper bounds and their symbols.
    """
    address = address & 0x07FFFFFF

    for i in range(len(ranges) - 1):
        lower_addr, lower_symbol = ranges[i]
        upper_addr, upper_symbol = ranges[i + 1]
        
        if lower_addr <= address < upper_addr:
            return (lower_addr, lower_symbol, upper_addr, upper_symbol)

    # Address might fall beyond the last range
    if address >= ranges[-1][0]:
        return (ranges[-1][0], ranges[-1][1], None, None)

    # Address might be before the first range
    return (None, None, ranges[0][0], ranges[0][1])

def protect_ptr(input_address):
    sym = os.path.dirname(os.path.abspath(__file__)) + "/../FE-CLib-Mokha/reference/fireemblem8.sym"

    # Parse the input data
    ranges = parse_address_ranges(sym)

    # Find the range
    lower_addr, lower_symbol, upper_addr, upper_symbol = find_address_range(input_address, ranges)

    # Output result
    if lower_addr is not None and upper_addr is not None:
        print(f"    PROTECT 0x{lower_addr:08X} 0x{upper_addr:08X} // {lower_symbol}")
    else:
        print("Address is outside the range of the data.")

if __name__ == '__main__':
    protect_ptr(eval(sys.argv[1]))
