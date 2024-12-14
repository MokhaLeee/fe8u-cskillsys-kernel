import json
from pathlib import Path
from typing import List

def rchop(s: str, suffix: str):
    if suffix and s.endswith(suffix):
        return s[:-len(suffix)]
    return s

def parse_order_keys_file(fname: Path) -> List[str]:
    with open(fname) as load_file:
        okeys = json.load(load_file)
        okeys = [rchop(f, '.json') for f in okeys]
        return okeys