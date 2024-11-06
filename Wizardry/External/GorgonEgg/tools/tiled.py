
"""Minimal support for Tiled .tmx files."""

import xml.etree.ElementTree as ET
from pathlib import Path


def read_properties(elem: ET.Element) -> dict[str, str]:
  """Read an element's properties into a dict."""
  props = {}

  if (p := elem.find("properties")) is not None:

    for child in p.findall("property"):

      name = child.attrib.get("name")
      value = child.attrib.get("value")

      if name is None or value is None:
        continue

      props[name] = value

  return props


class TiledLayer:
  """
  A barebones representation of a Tiled layer.

  This only supports orthogonal layers that have embedded csv data.
  """

  name: str
  width: int
  height: int

  properties: dict[str, str]

  data: list[int]

  def from_elem(elem: ET.Element):
    """Create a TiledLayer from an xml element."""
    tl = TiledLayer()

    tl.name = elem.get("name", "")
    tl.properties = read_properties(elem)
    tl.width = int(elem.get("width", 0))
    tl.height = int(elem.get("height", 0))

    if (dt := elem.find("data")) is None:
      raise ValueError(
          f"Unable to find map data for layer {tl.name}"
        )

    if dt.get("encoding") != "csv":
      raise ValueError(
          f"Layer '{tl.name}' must have data encoded as csv."
        )

    tl.data = [int(c) for c in dt.text.strip().split(",")]

    return tl


class TiledTileset:
  """
  A barebones representation of a Tiled tileset.

  This only supports embedded tilesets with external single images.
  """

  first: int
  name: str
  file: Path
  tilecount: int
  properties: dict[str, str]

  def from_elem(elem: ET.Element):
    """Create a TiledTileset from an xml element."""
    tt = TiledTileset()

    tt.first = int(elem.get("firstgid", 0))
    tt.name = elem.get("name", "")
    tt.tilecount = int(elem.get("tilecount", 0))
    tt.properties = read_properties(elem)
    if (
        ((image := elem.find("image")) is None)
        or ((source := image.get("source")) is None)
      ):
      raise ValueError(
          f"Unable to find tileset image for '{tt.name}'"
        )

    tt.file = Path(source)

    return tt


class TiledMap:
  """A barebones representation of a Tiled .tmx map."""

  width: int
  height: int

  properties: dict[str, str]

  layers: list[TiledLayer]
  tilesets: list[TiledTileset]

  def from_file(file: Path):
    """Create a TiledMap from a file."""
    tree = ET.parse(file)
    root = tree.getroot()

    tm = TiledMap()

    tm.width = int(root.get("width", 0))
    tm.height = int(root.get("height", 0))

    tm.properties = read_properties(root)

    def get(cls: object, name: str) -> list[object]:
      """Get all of a child element."""
      return [
          cls.from_elem(e) for e in root.findall(name)
        ]

    tm.layers = get(TiledLayer, "layer")
    tm.tilesets = get(TiledTileset, "tileset")

    return tm
