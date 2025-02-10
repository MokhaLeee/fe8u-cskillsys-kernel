#include "common-chax.h"
#include "skill-system.h"
#include "debug-kit.h"
#include "kernel-lib.h"
#include "utf8.h"
#include "constants/gfx.h"

const u8 *_GetSkillIconExt(const u8 lo, int hi)
{
	const u8 *icon = gpSkillInfos[lo + (hi << 8)].icon;

	if (!icon)
		icon = GFX_SkillIcon_WIP;

	return icon;
}

const u8 *GetSkillIcon1(const u8 sid) { return _GetSkillIconExt(sid, Skill_INFO_GENERIC); }
const u8 *GetSkillIcon2(const u8 sid) { return _GetSkillIconExt(sid, Skill_INFO_PERSON); }
const u8 *GetSkillIcon3(const u8 sid) { return _GetSkillIconExt(sid, Skill_INFO_JOB); }
const u8 *GetSkillIcon4(const u8 sid) { return _GetSkillIconExt(sid, Skill_INFO_ITEM); }

u16 GetSkillDescMsg(const u16 sid)
{
	return gpSkillInfos[sid].desc;
}

u16 GetSkillNameMsg(const u16 sid)
{
	return gpSkillInfos[sid].name;
}

char *GetSkillDescStr(const u16 sid)
{
	char *str = NULL;
	u16 msg = GetSkillDescMsg(sid);

	if (msg != 0)
		str = GetStringFromIndex(msg);

	if (gpKernelDesigerConfig->auto_narrow_font)
		return Utf8ToNarrowFonts(str);
	else
		return str;
}

char *SkillDescToName(char *str)
{
	char *it;

	for (it = str; *it; ++it) {
		if (*it == ':') {
			*it = '\0';
			break;
		}
	}

	/**
	 * As we have changed the value in the buffer,
	 * the cached index should become invalid.
	 */
	sActiveMsg = -1;

	return str;
}

char *GetSkillNameStrFormDesc(const u16 sid)
{
	char *str;
	u16 msg = GetSkillDescMsg(sid);

	if (msg == 0)
		return "W.I.P";

	str = GetStringFromIndex(msg);
	return SkillDescToName(str);
}

char *GetSkillNameStr(const u16 sid)
{
	char *str;
	u16 msg = GetSkillNameMsg(sid);

	if (msg == 0)
		str = GetSkillNameStrFormDesc(sid);
	else
		str = GetStringFromIndex(msg);

	if (gpKernelDesigerConfig->auto_narrow_font)
		return Utf8ToNarrowFonts(str);
	else
		return str;
}

#if 0
int GetEfxSkillPriority(const u16 sid)
{
	return gpEfxSkillAnimPriority[sid];
}
#endif

const struct EfxAnimConf *GetEfxSkillConf(const u8 aid)
{
	struct EfxAnimConf const *conf = gpEfxSkillAnims[aid];

	if (!conf)
		conf = &EfxSkillVanilla;

	return conf;
}
