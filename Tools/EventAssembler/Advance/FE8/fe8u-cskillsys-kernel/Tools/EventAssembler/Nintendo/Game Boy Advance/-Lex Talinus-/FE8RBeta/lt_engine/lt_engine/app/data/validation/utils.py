from typing import Union
from app.data.database.components import ComponentType
from app.data.database.database_types import DatabaseType
from app.data.resources.resource_types import ResourceType

LTType = Union[DatabaseType, ResourceType, ComponentType]