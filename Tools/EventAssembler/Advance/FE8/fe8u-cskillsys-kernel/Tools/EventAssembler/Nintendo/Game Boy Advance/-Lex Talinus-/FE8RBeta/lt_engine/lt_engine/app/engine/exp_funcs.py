from app.data.database.database import DB

@staticmethod
def has_autopromote(unit) -> bool:
    unit_klass = DB.classes.get(unit.klass)
    return (DB.constants.value('auto_promote') or 'AutoPromote' in unit.tags) and \
        unit_klass.turns_into and 'NoAutoPromote' not in unit.tags

@staticmethod
def can_give_exp(unit, exp: int) -> bool:
    unit_klass = DB.classes.get(unit.klass)
    if unit.level < unit_klass.max_level:
        return True
    if exp < 0:
        return True
    if has_autopromote(unit):
        return True
    return False
