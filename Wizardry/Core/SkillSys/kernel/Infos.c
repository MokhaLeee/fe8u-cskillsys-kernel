#include "common-chax.h"
#include "skill-system.h"
#include "debug-kit.h"
#include "constants/gfx.h"

const u8 * _GetSkillIconExt(const u8 sid_real, int list_idx)
{
    const u8 * icon = gpSkillInfos[sid_real + (list_idx << 8)].icon;
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
    return gpSkillInfos[sid].desc;
}

u16 GetSkillNameMsg(const u16 sid)
{
    return gpSkillInfos[sid].name;
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

int GetEfxSkillIndex(const u16 sid)
{
    return gpSkillAnimInfos[sid].aid;
}

int GetEfxSkillPriority(const u16 sid)
{
    u8 priority = gpSkillAnimInfos[sid].priority;

    if (priority == 0)
        priority = EFX_PRIORITY_NORMAL;

    return priority;
}

int GetEfxSkillSfx(const u16 sid)
{
    u16 sfx = gpSkillAnimInfos[sid].sfx;

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
