from dataclasses import dataclass

from app.utilities.data import Data, Prefab

@dataclass
class Equation(Prefab):
    nid: str = None
    expression: str = None

class EquationCatalog(Data[Equation]):
    datatype = Equation
