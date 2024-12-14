from typing import Tuple

# TODO(rainlash) Remove on 2024/3/1 or when we feel everyone has had a chance to upgrade
try:
    from typing_extensions import Protocol
except (ModuleNotFoundError, ImportError):
    Protocol = object
    print("You are missing the `typing-extensions` module.\nTry running `pip install -r requirements_editor.txt`")
# End Remove

try:
    from typing_extensions import override
except (ModuleNotFoundError, ImportError):
    def override(func):
        return func
    print("You are missing the `typing-extensions` module.\nTry running `pip install -r requirements_editor.txt`")

Pos = Tuple[int, int]

Point = Tuple[int, int]
Segment = Tuple[Point, Point]

NID = str
UID = int
Color3 = Tuple[int, int, int]
Color4 = Tuple[int, int, int, int]
