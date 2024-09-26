#include "common-chax.h"
#include "item-sys.h"
#include "skill-system.h"

bool IsDuraItem(int item)
{
	switch (ITEM_INDEX(item)) {
	case CONFIG_ITEM_INDEX_SKILL_SCROLL:
		return true;

	default:
		return false;
	}
}

char *GetDuraItemName(int item)
{
	if (ITEM_INDEX(item) == CONFIG_ITEM_INDEX_SKILL_SCROLL)
		return GetSkillScrollItemName(item);

	Errorf("Invalid dura-item: %x", item);
	return NULL;
}

int GetDuraItemDescId(int item)
{
	if (ITEM_INDEX(item) == CONFIG_ITEM_INDEX_SKILL_SCROLL)
		return GetSkillScrollItemDescId(item);

	Errorf("Invalid dura-item: %x", item);
	return 0;
}

int GetDuraItemUseDescId(int item)
{
	if (ITEM_INDEX(item) == CONFIG_ITEM_INDEX_SKILL_SCROLL)
		return GetSkillScrollItemUseDescId(item);

	Errorf("Invalid dura-item: %x", item);
	return 0;
}

int GetDuraItemIconId(int item)
{
	if (ITEM_INDEX(item) == CONFIG_ITEM_INDEX_SKILL_SCROLL)
		return GetSkillScrollItemIconId(item);

	Errorf("Invalid dura-item: %x", item);
	return 0;
}
