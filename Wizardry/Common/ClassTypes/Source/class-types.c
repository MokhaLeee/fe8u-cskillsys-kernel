#include "common-chax.h"
#include "class-types.h"

extern u8 const * const gpKernelClassList_Flier;
extern u8 const * const gpKernelClassList_Cavalry;
extern u8 const * const gpKernelClassList_Armor;
extern u8 const * const gpKernelClassList_Dragon;
extern u8 const * const gpKernelClassList_Beast;

bool CheckSameClassType(u8 jid1, u8 jid2)
{
    bool jdg1, jdg2;

    jdg1 = CheckClassFlier(jid1);
    jdg2 = CheckClassFlier(jid2);
    if (jdg1 == true || jdg2 == true)
        if (jdg1 != jdg2)
            return false;

    jdg1 = CheckClassCavalry(jid1);
    jdg2 = CheckClassCavalry(jid2);
    if (jdg1 == true || jdg2 == true)
        if (jdg1 != jdg2)
            return false;

    jdg1 = CheckClassArmor(jid1);
    jdg2 = CheckClassArmor(jid2);
    if (jdg1 == true || jdg2 == true)
        if (jdg1 != jdg2)
            return false;

    jdg1 = CheckClassDragon(jid1);
    jdg2 = CheckClassDragon(jid2);
    if (jdg1 == true || jdg2 == true)
        if (jdg1 != jdg2)
            return false;

    jdg1 = CheckClassBeast(jid1);
    jdg2 = CheckClassBeast(jid2);
    if (jdg1 == true || jdg2 == true)
        if (jdg1 != jdg2)
            return false;

    /* Both of which are normal jobs */
    return true;
}

bool CheckClassFlier(u8 jid)
{
    const u8 * it;
    for (it = gpKernelClassList_Flier; *it != CLASS_NONE; it++)
        if (*it == jid)
            return true;

    return false;
}

bool CheckClassCavalry(u8 jid)
{
    const u8 * it;
    for (it = gpKernelClassList_Cavalry; *it != CLASS_NONE; it++)
        if (*it == jid)
            return true;

    return false;
}

bool CheckClassArmor(u8 jid)
{
    const u8 * it;
    for (it = gpKernelClassList_Armor; *it != CLASS_NONE; it++)
        if (*it == jid)
            return true;

    return false;
}

bool CheckClassDragon(u8 jid)
{
    const u8 * it;
    for (it = gpKernelClassList_Dragon; *it != CLASS_NONE; it++)
        if (*it == jid)
            return true;

    return false;
}

bool CheckClassBeast(u8 jid)
{
    const u8 * it;
    for (it = gpKernelClassList_Beast; *it != CLASS_NONE; it++)
        if (*it == jid)
            return true;

    return false;
}
