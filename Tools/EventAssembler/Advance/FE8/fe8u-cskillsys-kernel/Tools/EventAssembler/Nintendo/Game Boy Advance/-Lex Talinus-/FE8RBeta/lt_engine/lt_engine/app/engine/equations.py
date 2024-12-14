import re, functools

from app.data.database.database import DB

class Parser():
    def __init__(self):
        self.equations = {}
        for equation in DB.equations.values():
            if equation.expression:
                self.equations[equation.nid] = self.tokenize(equation.expression)

        self.replacement_dict = self.create_replacement_dict()

        for nid in list(self.equations.keys()):
            expression = self.equations[nid]
            self.fix(nid, expression, self.replacement_dict)

        # Now add these equations as local functions
        for nid in self.equations.keys():
            if not nid.startswith('__'):
                setattr(self, nid.lower(), functools.partial(self.equations[nid], self.equations))

    def tokenize(self, s: str) -> str:
        return re.split('([^a-zA-Z_])', s)

    def create_replacement_dict(self):
        dic = {}
        for stat in DB.stats:
            dic[stat.nid] = ("(unit.stats['%s'] + unit.stat_bonus('%s'))" % (stat.nid, stat.nid))
        for nid in self.equations.keys():
            dic[nid] = ("equations['%s'](equations, unit)" % nid)
        return dic

    def fix(self, lhs, rhs, dic):
        rhs = [dic.get(n, n) for n in rhs]
        rhs = ''.join(rhs)
        rhs = 'int(%s)' % rhs
        exec("def %s(equations, unit): return %s" % (lhs, rhs), self.equations)

    def get(self, lhs, unit):
        if lhs in self.equations:
            return self.equations[lhs](self.equations, unit)
        return 0

    def get_expression(self, expr, unit):
        # For one time use
        # Can't seem to be used with any sub equations
        expr = self.tokenize(expr)
        expr = [self.replacement_dict.get(n, n) for n in expr]
        expr = ''.join(expr)
        expr = 'int(%s)' % expr
        equations = self.equations
        return eval(expr)

    def get_mana(self, unit):
        if hasattr(self, 'mana'):
            return self.mana(unit)
        else:
            return 0

    def get_max_fatigue(self, unit):
        if hasattr(self, 'max_fatigue'):
            return self.max_fatigue(unit)
        else:
            return 10

    def get_initiative(self, unit):
        if hasattr(self, 'initiative'):
            return self.initiative(unit)
        else:
            return 0

    def get_max_guard(self, unit):
        if hasattr(self, 'max_guard'):
            return self.max_guard(unit)
        else:
            return 10

    def get_gauge_inc(self, unit):
        if hasattr(self, 'gauge_increase'):
            return self.gauge_increase(unit)
        else:
            return 2

    def get_guard_exp(self, unit):
        if hasattr(self, 'guard_exp'):
            return self.guard_exp(unit)
        else:
            return 10

PARSER = Parser()

def __getattr__(name):
    if name == 'parser':
        return PARSER
    raise AttributeError(f"module '{__name__}' has no attribute '{name}'")

def clear():
    """
    Recreate the parser. Necessary in order to update equations after the user
    updates them in the equation editor
    """
    global PARSER
    PARSER = Parser()
