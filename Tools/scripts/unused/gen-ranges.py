#!/usr/bin/python3

import sys

def main(args):
    _range = eval(args[1])

    print("")

    print(f"const struct Vec_8 range_{_range}x{_range}[] =" + " {")

    for i in range(0, 2 * _range - 1):
        line = "    "
        y_dist = i  + 1 - _range
        x_max_dist = _range - abs(y_dist)

        for k in range(0, abs(y_dist) + 1):
            line += "          "

        for j in range(0, 2 * x_max_dist - 1):
            x_dist = j + 1 - x_max_dist

            if x_dist == 0 and y_dist == 0:
                line += "          "
            else:
                line += "{" + f"{x_dist: d}, {y_dist: d}" + "}, "

        print(line)

    print("};")

if __name__ == '__main__':
    main(sys.argv)
