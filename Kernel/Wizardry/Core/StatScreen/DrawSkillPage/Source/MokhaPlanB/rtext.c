#include "common-chax.h"
#include "stat-screen.h"
#include "wrank-bonus.h"
#include "constants/texts.h"

static _DECL_INFO RText_Sword, RText_Lance, RText_Axe, RText_Bow, RText_Anima, RText_Light, RText_Dark, RText_Staff;
static _DECL_INFO RText_Name, RText_Class, RText_Level, RText_Exp, RText_Hp;
static _DECL_INFO RText_Skill1, RText_Skill2, RText_Skill3, RText_Skill4, RText_Skill5, RText_Skill6, RText_Skill7, RText_Skill8;
static _DECL_INFO RText_Skill9, RText_Skill10, RText_Skill11, RText_Skill12, RText_Skill13, RText_Skill14, RText_Skill15, RText_Skill16;
static _DECL_INFO RText_Art1, RText_Art2, RText_Art3, RText_Art4, RText_Art5, RText_Art6, RText_Art7, RText_Art8;

_DECL_INFO *const RTextSkillPage_MokhaPlanB = &RText_Sword;

static _DECL_INFO RText_Sword = {
	NULL, &RText_Lance, &RText_Name, &RText_Skill1,
	0x66, 0x18, 0x561,
	NULL, HbPopuplate_WrankBonus
};

static _DECL_INFO RText_Lance = {
	&RText_Sword, &RText_Axe, &RText_Name, &RText_Skill1,
	0x66, 0x28, 0x562,
	NULL, HbPopuplate_WrankBonus
};

static _DECL_INFO RText_Axe = {
	&RText_Lance, &RText_Bow, &RText_Name, &RText_Skill5,
	0x66, 0x38, 0x563,
	NULL, HbPopuplate_WrankBonus
};

static _DECL_INFO RText_Bow = {
	&RText_Axe, &RText_Anima, &RText_Name, &RText_Skill9,
	0x66, 0x48, 0x564,
	NULL, HbPopuplate_WrankBonus
};

static _DECL_INFO RText_Anima = {
	&RText_Bow, &RText_Light, &RText_Class, &RText_Skill13,
	0x66, 0x58, 0x565,
	NULL, HbPopuplate_WrankBonus
};

static _DECL_INFO RText_Light = {
	&RText_Anima, &RText_Dark, &RText_Name, &RText_Art1,
	0x66, 0x68, 0x566,
	NULL, HbPopuplate_WrankBonus
};

static _DECL_INFO RText_Dark = {
	&RText_Light, &RText_Staff, &RText_Exp, &RText_Art1,
	0x66, 0x78, 0x567,
	NULL, HbPopuplate_WrankBonus
};

static _DECL_INFO RText_Staff = {
	&RText_Dark, NULL, &RText_Exp, &RText_Art2,
	0x66, 0x88, 0x568,
	NULL, HbPopuplate_WrankBonus
};

/* Skills */
static _DECL_INFO RText_Skill1 = {
	&RText_Art5, &RText_Skill5, &RText_Lance, &RText_Skill2,
	0xA6, 0x28, 0,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

static _DECL_INFO RText_Skill2 = {
	&RText_Art6, &RText_Skill6, &RText_Skill1, &RText_Skill3,
	0xB6, 0x28, 1,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

static _DECL_INFO RText_Skill3 = {
	&RText_Art7, &RText_Skill7, &RText_Skill2, &RText_Skill4,
	0xC6, 0x28, 2,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

static _DECL_INFO RText_Skill4 = {
	&RText_Art8, &RText_Skill8, &RText_Skill3, &RText_Name,
	0xD6, 0x28, 3,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

static _DECL_INFO RText_Skill5 = {
	&RText_Skill1, &RText_Skill9, &RText_Axe, &RText_Skill6,
	0xA6, 0x38, 4,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

static _DECL_INFO RText_Skill6 = {
	&RText_Skill2, &RText_Skill10, &RText_Skill5, &RText_Skill7,
	0xB6, 0x38, 5,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

static _DECL_INFO RText_Skill7 = {
	&RText_Skill3, &RText_Skill11, &RText_Skill6, &RText_Skill8,
	0xC6, 0x38, 6,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

static _DECL_INFO RText_Skill8 = {
	&RText_Skill4, &RText_Skill12, &RText_Skill7, &RText_Name,
	0xD6, 0x38, 7,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

static _DECL_INFO RText_Skill9 = {
	&RText_Skill5, &RText_Skill13, &RText_Bow, &RText_Skill10,
	0xA6, 0x48, 8,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

static _DECL_INFO RText_Skill10 = {
	&RText_Skill6, &RText_Skill14, &RText_Skill9, &RText_Skill11,
	0xB6, 0x48, 9,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

static _DECL_INFO RText_Skill11 = {
	&RText_Skill7, &RText_Skill15, &RText_Skill10, &RText_Skill12,
	0xC6, 0x48, 10,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

static _DECL_INFO RText_Skill12 = {
	&RText_Skill8, &RText_Skill16, &RText_Skill11, &RText_Name,
	0xD6, 0x48, 11,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

static _DECL_INFO RText_Skill13 = {
	&RText_Skill9, &RText_Art1, &RText_Anima, &RText_Skill14,
	0xA6, 0x58, 12,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

static _DECL_INFO RText_Skill14 = {
	&RText_Skill10, &RText_Art2, &RText_Skill13, &RText_Skill15,
	0xB6, 0x58, 13,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

static _DECL_INFO RText_Skill15 = {
	&RText_Skill11, &RText_Art3, &RText_Skill15, &RText_Skill16,
	0xC6, 0x58, 14,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

static _DECL_INFO RText_Skill16 = {
	&RText_Skill12, &RText_Art4, &RText_Skill16, &RText_Name,
	0xD6, 0x58, 15,
	HbRedirect_SkillPageCommon, HbPopuplate_SkillPageCommon
};

/* Arts */
static _DECL_INFO RText_Art1 = {
	&RText_Skill13, &RText_Art5, &RText_Dark, &RText_Art2,
	0xA6, 0x78, 0,
	HbRedirect_ArtPageOnlyAlly, HbPopuplate_ArtPageCommon
};

static _DECL_INFO RText_Art2 = {
	&RText_Skill14, &RText_Art6, &RText_Art1, &RText_Art3,
	0xB6, 0x78, 1,
	HbRedirect_ArtPageOnlyAlly, HbPopuplate_ArtPageCommon
};

static _DECL_INFO RText_Art3 = {
	&RText_Skill15, &RText_Art7, &RText_Art2, &RText_Art4,
	0xC6, 0x78, 2,
	HbRedirect_ArtPageOnlyAlly, HbPopuplate_ArtPageCommon
};

static _DECL_INFO RText_Art4 = {
	&RText_Skill16, &RText_Art8, &RText_Art3, &RText_Level,
	0xD6, 0x78, 3,
	HbRedirect_ArtPageOnlyAlly, HbPopuplate_ArtPageCommon
};

static _DECL_INFO RText_Art5 = {
	&RText_Art1, &RText_Skill1, &RText_Dark, &RText_Art6,
	0xA6, 0x88, 4,
	HbRedirect_ArtPageOnlyAlly, HbPopuplate_ArtPageCommon
};

static _DECL_INFO RText_Art6 = {
	&RText_Art2, &RText_Skill2, &RText_Art5, &RText_Art7,
	0xB6, 0x88, 5,
	HbRedirect_ArtPageOnlyAlly, HbPopuplate_ArtPageCommon
};

static _DECL_INFO RText_Art7 = {
	&RText_Art3, &RText_Skill3, &RText_Art6, &RText_Art8,
	0xC6, 0x88, 6,
	HbRedirect_ArtPageOnlyAlly, HbPopuplate_ArtPageCommon
};

static _DECL_INFO RText_Art8 = {
	&RText_Art4, &RText_Skill4, &RText_Art7, &RText_Hp,
	0xD6, 0x88, 7,
	HbRedirect_ArtPageOnlyAlly, HbPopuplate_ArtPageCommon
};

/* Page left */
static _DECL_INFO RText_Name = {
	&RText_Hp, &RText_Class, &RText_Skill12, &RText_Anima,
	0x18, 0x50, 0,
	NULL, HbPopulate_SSCharacter
};

static _DECL_INFO RText_Class = {
	&RText_Name, &RText_Level, &RText_Light, &RText_Light,
	0x06, 0x68, 0,
	NULL, HbPopulate_SSClass
};

static _DECL_INFO RText_Level = {
	&RText_Class, &RText_Hp, &RText_Art4, &RText_Exp,
	0x06, 0x78, 0x542,
	NULL, NULL
};

static _DECL_INFO RText_Exp = {
	&RText_Class, &RText_Hp, &RText_Level, &RText_Dark,
	0x26, 0x78, 0x543,
	NULL, NULL
};

static _DECL_INFO RText_Hp = {
	&RText_Level, &RText_Name, &RText_Art8, &RText_Staff,
	0x06, 0x88, 0x544,
	NULL, NULL
};
