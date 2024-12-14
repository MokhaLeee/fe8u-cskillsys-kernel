from __future__ import annotations
from typing import Optional, Tuple, List
from app.data.validation.utils import LTType

class Error():
    _type = 'ERROR'
    _props: List[Tuple[LTType | str, str]]
    _origin: Optional[Tuple[LTType | str, str]] = None
    _msg: Tuple[str, str] = ("Message", '')
    tmplt: Error.Templates

    def __init__(self, dtype: Optional[LTType | str]=None, value: Optional[str]=None) -> None:
        if dtype is not None and value is not None:
            self._origin = (dtype, value)
        self._props = []
        self.tmplt = Error.Templates(self)

    def copy(self) -> Error:
        if self._origin:
            copy = Error(*self._origin)
        else:
            copy = Error()
        copy._props = self._props.copy()
        copy._msg = self._msg
        copy._type = self._type
        return copy

    def concat(self, err: Error) -> Error:
        copy = self.copy()
        if err._origin:
            copy._props.append(err._origin)
        copy._props += err._props.copy()
        copy._msg = err._msg
        return copy

    def msg(self, msg: str) -> Error:
        copy = self.copy()
        copy._msg = ("Message", msg)
        return copy

    def field(self, name: str) -> Error:
        return self.prop('Field', name)

    def prop(self, dtype: LTType | str, value: str) -> Error:
        copy = self.copy()
        copy._props.append((dtype, value))
        return copy

    def __add__(self, other: Error) -> Error:
        return self.concat(other)

    def __str__(self) -> str:
        def in_(prop: Tuple[LTType | str, str], origin=False):
            dtype, value = prop
            if isinstance(dtype, str):
                dstr = dtype
            else:
                dstr = dtype.name.lower().capitalize()
                if dstr.endswith('s'):
                    dstr = dstr[:-1]
            if not origin:
                return "In %s '%s':" % (dstr, value)
            return "In [%s:'%s']:" % (dstr, value)
        msgs = []
        if self._origin:
            origin_msg = "%s: %s" % (self._type, in_(self._origin, True))
            msgs.append(origin_msg)
        msgs += [in_(prop) for prop in self._props]
        msgs.append(self._msg[1])
        return '\n    '.join(msgs)

    def to_warn(self):
        copy = self.copy()
        copy._type = "WARNING"
        return copy

    def to_err(self):
        copy = self.copy()
        copy._type = "ERROR"
        return copy

    class Templates():
        def __init__(self, parent: Error) -> None:
            self.parent = parent

        def not_type(self, dtype: LTType | str, val: str) -> Error:
            dstr = dtype if isinstance(dtype, str) else dtype.name
            msg = "No %s with name '%s'" % (dstr, val)
            return self.parent.msg(msg)

        def null_f(self, name) -> Error:
            msg = "Field %s is not set or set to empty" % name
            return self.parent.msg(msg)

class Warning(Error):
    _type = 'WARNING'
