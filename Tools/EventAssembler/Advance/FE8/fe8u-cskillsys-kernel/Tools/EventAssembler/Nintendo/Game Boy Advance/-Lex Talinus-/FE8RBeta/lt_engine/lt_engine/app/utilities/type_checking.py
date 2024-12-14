from typing import Any

def check_valid_type(value: Any) -> bool:
    primitive_types = (int, float, str, bool)
    collection_types = (tuple, list, dict, set)
    if value is None:
        return True
    if any(isinstance(value, primitive) for primitive in primitive_types):
        return True
    if any(isinstance(value, collection) for collection in collection_types):
        if isinstance(value, dict):
            return check_valid_type(list(value.keys())) and check_valid_type(list(value.values()))
        else:
            return all(check_valid_type(v) for v in value)
    return False
