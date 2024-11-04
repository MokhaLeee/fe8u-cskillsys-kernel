INSTALL
  This patch is for FE8U!

  Buildfile users: Install by using Event Assembler to apply "PrepsAugury.event" to your ROM, or include 
  "PrepsAugury.event" in your buildfile.
  
  FEBuilder users: "Advanced Editors"->"Insert EA"->"Select File". Pick "PrepsAugury.event" and press "Load Script".


WHATITDO?
  This patch adds another option (default: "Tactics") to the preparations menu. Choosing this option will go to 
  another screen where dialogue text is executed. Once the text ends, the game returns to the preparations menu. The 
  idea is to mimic FE7's "Augury" feature, although without the need to pay any money.


CUSTOMIZATION
  The following variables are #defined in "PrepsAugury.event":
    - PREEXT_MenuOptionTextID set this to the TextID of the menu option you want to display in the preparations 
    menu. Default is 0x58A.
    - PREEXT_MenuHelpTextID set this to the TextID of the help text you want to display in the preparations menu 
    when hovering over the menu option. Default is 0x57E.
    - PREEXT_ExtraEntryProc this can be set to a different proc if you want the extra preparations screen entry to 
    execute a different functionality. Default is PREEXT_Procs_Augury, which is the Augury functionality described 
    in "WHATITDO?"
  
  "Tables/Tables.event" contains a pointertable, PREEXT_AuguryText. This table is indexed by chapter. Each pointer 
  points to an array of (Variable, TextID) pairs, where Variable is either a UnitID or Flag. If a unit with this 
  UnitID is alive, or the Flag is set, the TextID's textstring will be displayed during the corresponding chapter's 
  tactics. If multiple strings are to be displayed, they'll be concatenated in the order of appearance in the array. 
  If Variable is 0, the corresponding string will be displayed unconditionally. The array terminates when both 
  Variable and TextID are 0. If a chapter's pointer is 0, tactics will be disabled during that chapter. 
  "Tables/Tables.event" has an example table containing an entry with a (Flag, TextID) pair, two (Variable, TextID) 
  pairs, a no condition TextID, a terminator, and four dummy entries, allowing expansion for up to 8 conditions.
  
  For FEBuilder users: if you wish to increase the size of a chapter's array you'll need to repoint the table's 
  entry to somewhere else in freespace. Before implementing the patch, consider thinking of some limit to how many 
  conditions any chapter's tactics text would likely have, and set every chapter's array to that size (by using 
  dummies to pad out the size) to avoid having to repoint entries later if there's not enough room left inline.
  

CONCLUDING
  23/8/2021.
  This took me far less time (only two days!) than I imagined it would. I made this for an asset trade with 
  Retrogamer, and they allowed this to be shared with everyone else.
  
  27/8/2021
  I added the option to display different text based on whether certain units are alive or certain flags are set. 
  It'll unfortunately make it necessary to plan ahead when using more rigid tools like FEBuilder.
  
  ~Huichelaar