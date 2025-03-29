#include "common-chax.h"
#include "item-sys.h"
#include "skill-system.h"

bool IsDuraItem(int item)
{
	switch (ITEM_INDEX(item)) {
	case CONFIG_ITEM_INDEX_SKILL_SCROLL:
	case CONFIG_ITEM_INDEX_SKILL_SCROLL_FEB:
		return true;

	default:
		return false;
	}
}

char *GetDuraItemName(int item)
{
	if (IsSkillScrollItem(item))
		return GetSkillScrollItemName(item);

	Errorf("Invalid dura-item: %x", item);
	return NULL;
}

int GetDuraItemDescId(int item)
{
	if (IsSkillScrollItem(item))
		return GetSkillScrollItemDescId(item);

	Errorf("Invalid dura-item: %x", item);
	return 0;
}

int GetDuraItemUseDescId(int item)
{
	if (IsSkillScrollItem(item))
		return GetSkillScrollItemUseDescId(item);

	Errorf("Invalid dura-item: %x", item);
	return 0;
}

int GetDuraItemIconId(int item)
{
	if (IsSkillScrollItem(item))
		return GetSkillScrollItemIconId(item);

	Errorf("Invalid dura-item: %x", item);
	return 0;
}
