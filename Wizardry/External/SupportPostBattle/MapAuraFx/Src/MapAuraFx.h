#ifndef MAP_AURA_FX_INCLUDED
#define MAP_AURA_FX_INCLUDED

#include "gbafe.h"

void StartMapAuraFx(void);
void EndMapAuraFx(void);
int  IsMapAuraFxActive(void);
void SetMapAuraFxSpeed(int speed);
void SetMapAuraFxBlend(unsigned blend);
void SetMapAuraFxPalette(const u16 palette[]);
void AddMapAuraFxUnit(struct Unit* unit);

typedef struct Proc Proc;
#define ROOT_PROC_3 (Proc*)(3)

#define BG_LOCATED_TILE(aMap, aX, aY) (&(aMap)[(aX) + (aY) * 0x20])

#endif // MAP_AURA_FX_INCLUDED
