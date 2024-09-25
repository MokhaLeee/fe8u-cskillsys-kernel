// SPDX-License-Identifier: GPL-2.0-only

#include "common-chax.h"

extern const u16 Pal_NinianPrayfx[];
extern const u8 Img_NinianPrayfx[];
extern const u8 SpritAnim_NinianPrayfx[];

const struct EventSpriteAnimConf EventSpriteAnimConf_NinianPray = {
    .pal = Pal_NinianPrayfx,
    .img = Img_NinianPrayfx,
    .ap_conf = SpritAnim_NinianPrayfx,
    .oam0 = 0x0000,
    .oam2 = 0x0280,
    .pal_bank = 5,
    .pal_size = 1
};
