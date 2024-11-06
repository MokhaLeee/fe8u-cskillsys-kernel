
"""Convert .tmx tilemap files -> .c/.h source/headers."""

import sys
from argparse import ArgumentParser
from pathlib import Path
from itertools import islice
from typing import Iterable

from tiled import TiledMap


def read_tile(raw: int) -> tuple[int, bool, bool]:
  """
  Convert a raw gid into its tile and flipping flags.

  This only handles x/y flipping.
  """
  gid = raw & ~(0b1111 << 28)
  xflip = False if ((raw & (1 << 31)) == 0) else True
  yflip = False if ((raw & (1 << 30)) == 0) else True

  return (gid, xflip, yflip)


def export_tile(gid: int, palette: int, xflip: bool, yflip: bool):
  """Convert a tile into a GBA tile entry."""
  return (
      gid
      | (int(xflip) << 10)
      | (int(yflip) << 11)
      | (palette << 12)
    )


def convert_int(val: str) -> int:
  """Convert a number string into a Python int."""
  return int(val, 16 if val.startswith("0x") else 10)


def emit_c(layer: list[int], name: str, width: int, height: int) -> list[str]:
  """Create a C representation of a layer."""
  lines = [
      "",
      f"const u16 {name}[] = {{",
      f"  0x{height - 1:02X}{width - 1:02X},"
    ]
  # The tilemap rows have their bottommost row first -> topmost last.
  # This will break if there is somehow a tilemap size/data mismatch.
  rows = reversed(list(batched(layer, width)))
  for r, row in enumerate(rows):
    line_text = []
    for tile in row:
      line_text.append(f"0x{tile:04X}")
    lines.append(
        "  " + ", ".join(line_text) + ("," if r != height - 1 else "")
      )
  lines.append("};")
  return lines


def batched(iterable: Iterable, n: int) -> Iterable:
  """
  Group data from an iterable into tuples.

  Taken from https://docs.python.org/3/library/itertools.html#itertools.batched
  """
  it = iter(iterable)
  while (batch := tuple(islice(it, n))):
    yield batch


def read_tmx(tmx_path: Path) -> tuple[str, str]:
  """Turn a .tmx tilemap into C files."""
  tm = TiledMap.from_file(tmx_path)

  tile_ranges = []
  for tileset in tm.tilesets:

    start = tileset.first
    end = start + tileset.tilecount
    offset, palette = 0, 0

    for k, v in tileset.properties.items():
      match k.lower():
        case "offset":
          offset = convert_int(v)
        case "palette":
          palette = convert_int(v)

    tile_ranges.append((range(start, end), offset, palette))

  def gen_default() -> list:
    """Generate a default tile layer."""
    return [None for y in range(tm.height) for x in range(tm.width)]

  bg_layers = [gen_default(), gen_default(), gen_default(), gen_default()]

  # Working from the furthest-back to the front.
  for layer in reversed(tm.layers):

    layer_id = 0
    fill = export_tile(0, 0, False, False)
    base = 0x0000

    for k, v in layer.properties.items():
      match k.lower():
        case "fill value":
          fill = convert_int(v)
        case "layer":
          layer_id = convert_int(v)
        case "tile base":
          base = convert_int(v)

    bl = bg_layers[layer_id]

    for t, tile in enumerate(layer.data):

      gid, xflip, yflip = read_tile(tile)

      # Fill in blank tiles with our fill value.
      if gid == 0:
        if bl[t] is None:
          bl[t] = fill

      # Nonblank tiles
      else:

        # Find the tileset that the tile belongs to and
        # use that to get the right tile.
        for tile_range, offset, palette in tile_ranges:

          if gid in tile_range:

            base_index = (offset - base) // 0x20
            index = gid - tile_range[0]

            bl[t] = export_tile(base_index + index, palette, xflip, yflip)

            break

        # If we didn't find our tile, throw an error.
        else:

          x, y = t % tm.width, t // tm.width

          raise ValueError(
              f"Unable to find tile {tile:04X} ({x}, {y}) "
              f"in layer '{layer.name}'."
            )

  c_lines = ["", '#include "gbafe.h"', ""]
  h_lines = ["", '#include "gbafe.h"', ""]

  # Patch any holes in layers (skipping fully-blank layers)
  # before assembling our outputs.
  for b, bl in enumerate(bg_layers):

    if not any(bl):
      continue

    for t, tile in enumerate(bl):
      if tile is None:
        bl[t] = export_tile(0, 0, False, False)

    c_lines.extend(emit_c(bl, tmx_path.stem + f"_{b}", tm.width, tm.height))
    h_lines.append(f"extern const u16 {tmx_path.stem}_{b}[];")

  return "\n".join(c_lines), "\n".join(h_lines)


def main() -> int:
  """Read args and parse a tilemap."""
  parser = ArgumentParser()
  parser.add_argument(
      "tmxfile",
      type=Path,
      help="the input .tmx file",
    )
  parser.add_argument(
      "outfile_c",
      type=Path,
      help="the output .c file",
    )
  parser.add_argument(
      "outfile_h",
      type=Path,
      nargs="?",
      help="the output .h file",
    )
  args = parser.parse_args()

  if not args.tmxfile.exists():
    sys.exit(f"Unable to find file '{args.tmxfile}'.")

  out_c, out_h = read_tmx(args.tmxfile)

  with open(args.outfile_c, "w") as o_c:
    o_c.write(out_c)

  if args.outfile_h is not None:
    with open(args.outfile_h, "w") as o_h:
      o_h.write(out_h)

  return 0


if __name__ == '__main__':
  sys.exit(main())
