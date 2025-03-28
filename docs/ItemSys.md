# Item Effect Revamp

CSkillSys supports the similar functionality as [teraspark' IER hack](https://feuniverse.us/t/a-teraspark-of-assembly/1789/2) with few modification.

1. The item usability & effects are desided by `ItemData::useEffectId` rather than item index itself. The IER table defination:
	```c
	struct IERevamp {
		bool (*usability)(struct Unit *unit, int item);
		bool (*prep_usability)(struct Unit *unit, int item);
		void (*effect)(struct Unit *unit, int item);
		void (*action_effect)(ProcPtr proc, struct Unit *unit, int item);
		void (*prep_effect)(struct ProcPrepItemUse *proc, u16 item);
	};

	extern struct IERevamp const IERevampTable[IER_MAX];
	```
2. Staves that heal based on a value a given value set in item data.

	```
	heal = [unit magic] + [item might] + [ItemData + 0x22 (IER Byte)]
	```

	If heal amount is lower than 10 HP, then it will be forcefully set as 10.

3. A new version of the vulnerary that heals off a given value set in item data.

	```
	heal = [item might] + [ItemData + 0x22 (IER Byte)]
	```

	If heal amount is lower than 10 HP, then it will be forcefully set as 10.

## Known issue
1. **FUCKING NIGHTMARE** item related hard-code functions are not touched. Do NOT edit night-mare.
