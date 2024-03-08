#include "global.h"
#include "common-chax.h"

#include "skill-system.h"
#include "constants/efx-skills.h"
#include "constants/combat-arts.h"

const struct SkillAnimInfo gEfxCombatArtAnimInfos[0x100] = {
    [CID_WrathStrike] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_Grounder] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_Soulblade] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_BaneOfMonsters] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_Sunder] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_Hexblade] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_HazeSlice] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_FinesseBlade] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_Assassinate] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_FoudroyantStrike] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_TempestLance] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_Knightkneeler] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_ShatterSlash] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_MonsterPiercer] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_SwiftStrikes] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_FrozenLance] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_Vengeance] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_LanceJab] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_Smash] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_HelmSplitter] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_MonsterBreaker] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_FocusedStrike] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_WildAbandon] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_Spike] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_DiamondAxe] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_LightningAxe] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_ArmoredStrike] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_WarMasterStrike] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_FlickeringFlower] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_CurvedShot] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_Deadeye] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_Encloser] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_EncloserPlus] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_MonsterBlast] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_SchismShot] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_BreakShot] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_BreakShotPlus] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_WaningShotPlus] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
    [CID_HuntersVolley] = {
        .aid = EFX_SKILL_DEFAULT,
        .priority = EFX_PRIORITY_HIGHHIGH,
        .sfx = 0x3D1,
    },
};

FEB_IDENTIFIER(gEfxCombatArtAnimInfos);
