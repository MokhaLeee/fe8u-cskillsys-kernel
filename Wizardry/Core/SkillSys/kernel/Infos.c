#include "global.h"

#include "common-chax.h"
#include "skill-system.h"
#include "debug-kit.h"
#include "constants/gfx.h"

static const struct SkillInfo * GetSkillInfo(int list_idx)
{
    switch (list_idx) {
    case Skill_INFO_GENERIC:
        return gpSkillInfos_Generic;

    case Skill_INFO_PERSON:
        return gpSkillInfos_Person;

    case Skill_INFO_JOB:
        return gpSkillInfos_Job;

    case Skill_INFO_ITEM:
        return gpSkillInfos_Item;
    }
    return NULL;
}

const u8 * _GetSkillIconExt(const u8 sid, int list_idx)
{
    const struct SkillInfo * info;
    const u8 * icon = NULL;

    info = GetSkillInfo(list_idx);
    if (info)
        icon = info[SKILL_INDEX_REAL(sid)].icon;

    if (!icon)
        icon = GFX_SkillIcon_WIP;

    return icon;
}

const u8 * GetSkillIcon_Generic(const u8 sid) { return _GetSkillIconExt(sid, Skill_INFO_GENERIC); }
const u8 * GetSkillIcon_Person(const u8 sid)  { return _GetSkillIconExt(sid, Skill_INFO_PERSON); }
const u8 * GetSkillIcon_Job(const u8 sid)     { return _GetSkillIconExt(sid, Skill_INFO_JOB); }
const u8 * GetSkillIcon_Item(const u8 sid)    { return _GetSkillIconExt(sid, Skill_INFO_ITEM); }

u16 GetSkillDescMsg(const u16 sid)
{
    const struct SkillInfo * info = GetSkillInfo(SKILL_INDEX_LIST(sid));
    if (!info)
        return 0;

    return info[SKILL_INDEX_REAL(sid)].desc;
}

u16 GetSkillNameMsg(const u16 sid)
{
    const struct SkillInfo * info = GetSkillInfo(SKILL_INDEX_LIST(sid));
    if (!info)
        return 0;

    return info[SKILL_INDEX_REAL(sid)].name;
}

char * GetSkillDescStr(const u16 sid)
{
    u16 msg = GetSkillDescMsg(sid);
    if (msg != 0)
        return GetStringFromIndex(msg);

    return NULL;
}

char * SkillDescToName(char * str)
{
    char * it;
    for (it = str; *it; ++it)
    {
        if (*it == ':')
        {
            *it = '\0';
            break;
        }
    }
    return str;
}

char * GetSkillNameStrFormDesc(const u16 sid)
{
    char * str;
    u16 msg = GetSkillDescMsg(sid);
    if (msg == 0)
        return "W.I.P";

    str = GetStringFromIndex(msg);
    return SkillDescToName(str);
}

char * GetSkillNameStr(const u16 sid)
{
    u16 msg = GetSkillNameMsg(sid);
    if (msg == 0)
        return GetSkillNameStrFormDesc(sid);

    return GetStringFromIndex(msg);
}

static const struct SkillAnimInfo * GetSkillAnimInfo(int list_idx)
{
    switch (list_idx) {
    case Skill_INFO_GENERIC:
        return gpSkillAnimInfos_Generic;

    case Skill_INFO_PERSON:
        return gpSkillAnimInfos_Person;

    case Skill_INFO_JOB:
        return gpSkillAnimInfos_Job;

    case Skill_INFO_ITEM:
        return gpSkillAnimInfos_Item;
    }
    return NULL;
}

int GetEfxSkillIndex(const u16 sid)
{
    const struct SkillAnimInfo * info = GetSkillAnimInfo(SKILL_INDEX_LIST(sid));
    if (!info)
        return 0;

    return info[SKILL_INDEX_REAL(sid)].aid;
}

int GetEfxSkillPriority(const u16 sid)
{
    u8 priority = 0;
    const struct SkillAnimInfo * info = GetSkillAnimInfo(SKILL_INDEX_LIST(sid));
    if (info)
        priority = info[SKILL_INDEX_REAL(sid)].priority;

    if (priority == 0)
        priority = EFX_PRIORITY_NORMAL;

    return priority;
}

int GetEfxSkillSfx(const u16 sid)
{
    u16 sfx = 0;
    const struct SkillAnimInfo * info = GetSkillAnimInfo(SKILL_INDEX_LIST(sid));
    if (info)
        sfx = info[SKILL_INDEX_REAL(sid)].sfx;

    if (sfx == 0)
        sfx = 0x3D1;

    return sfx;
}

const struct EfxAnimConf * GetEfxSkillConf(const u8 aid)
{
    struct EfxAnimConf const * conf = gpEfxSkillAnims[aid];
    if (!conf)
        conf = &EfxSkillVanilla;

    return conf;
}
