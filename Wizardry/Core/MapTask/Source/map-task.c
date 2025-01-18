#include "common-chax.h"
#include "debuff.h"
#include "map-task.h"
#include "kernel-lib.h"

LYN_REPLACE_CHECK(PutUnitSpriteIconsOam);
void PutUnitSpriteIconsOam(void)
{
	int i;
	bool icon_blinking;

	if (CheckFlag(0x84) != 0)
		return;

	icon_blinking = !((GetGameClock() & 0x3F) >= 0x28);

	PutChapterMarkedTileIconOam();

	for (i = 1; i <= 0xC0; i++) {
		int ix, iy, status;
		struct Unit *unit = GetUnit(i);

		if (!UNIT_IS_VALID(unit) || unit->state & US_HIDDEN)
			continue;

		// GetUnitSpriteHideFlag
		if (!unit->pMapSpriteHandle || (unit->pMapSpriteHandle->config & 0x80))
			continue;

		ix = unit->xPos * 16 - gBmSt.camera.x;
		iy = unit->yPos * 16 - gBmSt.camera.y;

		if (ix < -16 || ix > DISPLAY_WIDTH)
			continue;

		if (iy < -16 || iy > DISPLAY_HEIGHT)
			continue;

		MapTaskVec.x = ix;
		MapTaskVec.y = iy;

		/**
		 * 1. HpBar
		 */
		if (unit->curHP != unit->maxHP) {
			int _cur = unit->curHP;
			int _max = unit->maxHP;

			if (_cur > _max)
				_cur = _max;

			MapTaskPutOamHi(
				MTSKCONF_HPBAR,
				OAM2_PAL(0) + OAM2_LAYER(2) + OAM2_CHR(gpHpBarChr[k_udiv(_cur * 10, _max)]));
		}

		/**
		 * 2. Check on blinking icon
		 */
		if (icon_blinking) {
			/**
			 * 3. Debuff icon
			 */
			status = GetUnitStatusIndex(unit);

			if (status != UNIT_STATUS_NONE) {
				const struct DebuffInfo *info = &gpDebuffInfos[status];

				if (info->on_draw)
					info->on_draw(unit, ix, iy);
				else {
					switch (info->positive_type) {
					case STATUS_DEBUFF_NEGATIVE:
						MapTaskPutOamHi(MTSKCONF_COMM_DEBUFF, OAM2_LAYER(2));
						break;

					case STATUS_DEBUFF_POSITIVE:
						MapTaskPutOamHi(MTSKCONF_COMM_BUFF, OAM2_LAYER(2));
						break;
					}
				}
			} else {
				switch (SimulateStatDebuffPositiveType(unit)) {
				case STATUS_DEBUFF_NEGATIVE:
					MapTaskPutOamHi(MTSKCONF_COMM_DEBUFF, OAM2_LAYER(2));
					break;

				case STATUS_DEBUFF_POSITIVE:
					MapTaskPutOamHi(MTSKCONF_COMM_BUFF, OAM2_LAYER(2));
					break;
				}
			}

			/**
			 * 4. Misc
			 */
			if (unit->state & US_RESCUING) {
				static const u16 pal_idx[] = { 0xC, 0xE, 0xD };
				int faction = unit->rescue >> 6;

				MapTaskPutOamHi(
					MTSKCONF_RESCUING,
					OAM2_PAL(pal_idx[faction]) + OAM2_LAYER(2) + OAM2_CHR(0x60 / 0x20));
			} else if ((UNIT_FACTION(unit) != FACTION_BLUE) && (UNIT_CATTRIBUTES(unit) & CA_BOSS)) {
				MapTaskPutOamHi(
					MTSKCONF_BOSS,
					OAM2_PAL(0) + OAM2_LAYER(2) + OAM2_CHR(0x200 / 0x20));
			} else if (UNIT_CHAR_ID(unit) == GetROMChapterStruct(gPlaySt.chapterIndex)->protectCharacterIndex) {
				MapTaskPutOamHi(
					MTSKCONF_PROTECTION,
					OAM2_PAL(0) + OAM2_LAYER(2) + OAM2_CHR(0x220 / 0x20));
			}
		}
	}
}
