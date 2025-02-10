#pragma once

#include "common-chax.h"

struct EfxAnimConf {
	/**
	 * If you want FEB identify your efxanim conf
	 * then you have to add this unused header (dump from skillanimtemplate.dmp)
	 */
	u8 _unused_header[0x150];

	const s16 *frame_confs;
	u16 const *const *tsas;
	u16 const *const *imgs;
	u16 const *const *pals;
	u16 sfx;
	u8 _pad_[2];
};

extern const struct EfxAnimConf EfxSkillVanilla, EfxSkillCombat;
