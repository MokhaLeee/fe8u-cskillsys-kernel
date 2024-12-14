

import json
import os
from pathlib import Path

def load_json(path: Path):
    with path.open() as source:
        try:
            loaded = json.load(source)
        except Exception as e:
            raise Exception("Could not read %s" % str(path)) from e
        return loaded
    
def save_json(path: Path, value: str):
    temp_save_loc = path.parent / (path.name + ".tmp")
    with open(temp_save_loc, 'w') as serialize_file:
        json.dump(value, serialize_file, indent=4)
    os.replace(temp_save_loc, path)
