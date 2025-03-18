#include "common-chax.h"
#include "stat-screen.h"
#include "wrank-bonus.h"
#include "constants/texts.h"

static _DECL_INFO RText_Sword, RText_Lance, RText_Axe, RText_Bow, RText_Anima, RText_Light, RText_Dark, RText_Staff;
static _DECL_INFO RText_Skill1, RText_Skill2, RText_Skill3, RText_Skill4, RText_Skill5, RText_Skill6, RText_Skill7, RText_Skill8;
static _DECL_INFO RText_Name, RText_Class, RText_Level, RText_Exp, RText_Hp;

_DECL_INFO *const RTextSkillPage_MokhaPlanA = &RText_Sword;

static _DECL_INFO RText_Sword = {
	NULL, &RText_Lance, &RText_Name, &RText_Skill1,
	0x66, 0x18, 0x561,
	NULL, HbPopuplate_WrankBonus
};

static _DECL_INFO RText_Lance = {
	&RText_Sword, &RText_Axe, &RText_Name, &RText_Skill2,
	0x66, 0x28, 0x562,
	NULL, HbPopuplate_WrankBonus
};

static _DECL_INFO RText_Axe = {
	&RText_Lance, &RText_Bow, &RText_Name, &RText_Skill3,
	0x66, 0x38, 0x563,
	NULL, HbPopuplate_WrankBonus
};

static _DECL_INFO RText_Bow = {
	&RText_Axe, &RText_Anima, &RText_Name, &RText_Skill4,
	0x66, 0x48, 0x564,
	NULL, HbPopuplate_WrankBonus
};

static _DECL_INFO RText_Anima = {
	&RText_Bow, &RText_Light, &RText_Class, &RText_Skill5,
	0x66, 0x58, 0x565,
	NULL, HbPopuplate_WrankBonus
};

static _DECL_INFO RText_Light = {
	&RText_Anima, &RText_Dark, &RText_Name, &RText_Skill6,
	0x66, 0x68, 0x566,
	NULL, HbPopuplate_WrankBonus
};

static _DECL_INFO RText_Dark = {
	&RText_Light, &RText_Staff, &RText_Exp, &RText_Skill7,
	0x66, 0x78, 0x567,
	NULL, HbPopuplate_WrankBonus
};

static _DECL_INFO RText_Staff = {
	&RText_Dark, NULL, &RText_Exp, &RText_Skill8,
	0x66, 0x88, 0x568,
	NULL, HbPopuplate_WrankBonus
};

/* Line #2 */
static _DECL_INFO RText_Skill1 = {
	&RText_Skill8, &RText_Skill2, &RText_Sword, &RText_Name,
	0xA6, 0x18, 0,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

static _DECL_INFO RText_Skill2 = {
	&RText_Skill1, &RText_Skill3, &RText_Lance, &RText_Name,
	0xA6, 0x28, 1,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

static _DECL_INFO RText_Skill3 = {
	&RText_Skill2, &RText_Skill4, &RText_Axe, &RText_Name,
	0xA6, 0x38, 2,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

static _DECL_INFO RText_Skill4 = {
	&RText_Skill3, &RText_Skill5, &RText_Bow, &RText_Name,
	0xA6, 0x48, 3,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

static _DECL_INFO RText_Skill5 = {
	&RText_Skill4, &RText_Skill6, &RText_Anima, &RText_Name,
	0xA6, 0x58, 4,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

static _DECL_INFO RText_Skill6 = {
	&RText_Skill5, &RText_Skill7, &RText_Light, &RText_Class,
	0xA6, 0x68, 5,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

static _DECL_INFO RText_Skill7 = {
	&RText_Skill6, &RText_Skill8, &RText_Dark, &RText_Level,
	0xA6, 0x78, 6,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

static _DECL_INFO RText_Skill8 = {
	&RText_Skill7, &RText_Skill1, &RText_Staff, &RText_Hp,
	0xA6, 0x88, 7,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

/* Page left */
static _DECL_INFO RText_Name = {
	&RText_Hp, &RText_Class, NULL, &RText_Anima,
	0x18, 0x50, 0,
	NULL, HbPopulate_SSCharacter
};

static _DECL_INFO RText_Class = {
	&RText_Name, &RText_Level, NULL, &RText_Light,
	0x06, 0x68, 0,
	NULL, HbPopulate_SSClass
};

static _DECL_INFO RText_Level = {
	&RText_Class, &RText_Hp, NULL, &RText_Exp,
	0x06, 0x78, 0x542,
	NULL, NULL
};

static _DECL_INFO RText_Exp = {
	&RText_Class, &RText_Hp, &RText_Level, &RText_Dark,
	0x26, 0x78, 0x543,
	NULL, NULL
};

static _DECL_INFO RText_Hp = {
	&RText_Level, &RText_Name, NULL, &RText_Staff,
	0x06, 0x88, 0x544,
	NULL, NULL
};
