def resolve_weapon(unit):
    if unit:
        return unit.get_weapon()
    return None