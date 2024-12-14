from typing import Optional
from app.utilities.data import Data, Prefab
from app.utilities.typing import NID

class NodeMenuEvent(Prefab):
    def __init__(self, nid: str):
        self.nid: str = nid          #This is the unique identifier for the node menu option.
        self.event: Optional[NID] = None       #The event to be called.
        self.option_name: str = ''   #Display name of the menu option. This is what's shown in the menu, but the above is the actual event to call.
        self.visible: bool = False    #Whether the option will appear in the list
        self.enabled: bool = False    #Whether the option can be selected (i.e., if visible but not enabled, will be greyed out)

    @classmethod
    def default(cls):
        return cls('0')


class NodeEventCatalogue(Data[NodeMenuEvent]):
    datatype = NodeMenuEvent