#pragma once

#include "common-chax.h"

extern u8 const *const gpKernelClassList_Flier;
extern u8 const *const gpKernelClassList_Cavalry;
extern u8 const *const gpKernelClassList_Armor;
extern u8 const *const gpKernelClassList_Dragon;
extern u8 const *const gpKernelClassList_Beast;

bool CheckSameClassType(u8 jid1, u8 jid2);

bool CheckClassFlier(u8 jid);
bool CheckClassCavalry(u8 jid);
bool CheckClassArmor(u8 jid);
bool CheckClassDragon(u8 jid);
bool CheckClassBeast(u8 jid);
