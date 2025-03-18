For now (2024.07.22), kernel supports 64 debuff, and 126 superposable debuff. The former use `[Unit + 0x30]` 8 bits and the latter use an allocated memory.

# 1. Unit status expansion

In vanilla, unit status (buff & debuffs) use a u8 byte at Unit Struct +0x30, 4 bits for status-index, 4 bits for status-duration. As a result, unit can only hold no more than 15 status, since there have been 13 status used in vanilla, hackers can only make 2 more status in vanilla routine.

For now, we have expanded unit status index to 6 bits, which can support up to 63 different states simultaneously. Meanwhile, through a series of modifications, we successfully used the remaining 2 bits to make the status last for up to 4 turns.

At the same time, a table is build to store status informations, such as, how unit’s atk decrese during suffering this status or how long will this status continue, etc, which can be found from the pointer `gpDebuffInfos`.

```c
enum DEBUFF_POSITIVE_TYPE {
    /* DebuffInfo::positive_type */
    STATUS_DEBUFF_NONE,
    STATUS_DEBUFF_NEGATIVE,
    STATUS_DEBUFF_POSITIVE,

    STATUS_DEBUFF_NONE_NO_CALC,
};

enum STATUS_DEBUFF_TICK_TYPE {
    /* DebuffInfo::tick_type */
    STATUS_DEBUFF_NO_TICK = 0,
    STATUS_DEBUFF_TICK_ON_ENEMY = 1,
    STATUS_DEBUFF_TICK_ON_ALLY = 2,
};

struct DebuffInfo {
    const u8 *img;
    void (*on_draw)(struct Unit *unit, int ix, int iy);
    u16 name, desc;

    u8 positive_type;
    u8 tick_type;
    u8 duration;

    u8 _pad_;

    struct {
        u8 speed;
        u8 r, g, b;
    } efx_config;

    struct {
        s8 pow, mag, skl, spd, def, res, lck, mov;
    } unit_status;

    struct {
        s8 atk, def, hit, avo, crit, silencer, dodge;
    } battle_status;

    u8 cannot_move;
};

extern struct DebuffInfo const *const gpDebuffInfos;
```

Common API:
```c
// <debuff.h>
enum UNIT_STATUS_IDENTIFIER {
    /* Expand here */
    NEW_UNIT_STATUS_PIERCE_ARMOR = 14,
    NEW_UNIT_STATUS_PIERCE_MAGIC,
    NEW_UNIT_STATUS_HEAVY_GRAVITY,
    NEW_UNIT_STATUS_WEAKEN,
    NEW_UNIT_STATUS_AVOID,
    NEW_UNIT_STATUS_AVOID_PLUS,
    NEW_UNIT_STATUS_PANIC,

    NEW_UNIT_STATUS_MAX = 64
};

int GetUnitStatusIndex(struct Unit *unit);
int GetUnitStatusDuration(struct Unit *unit);
void SetUnitStatusIndex(struct Unit *unit, int status);
void SetUnitStatusDuration(struct Unit *unit, int duration);

// <bmunit.h>
void SetUnitStatus(struct Unit *unit, int statusId);
void SetUnitStatusExt(struct Unit *unit, int status, int duration);
```

# 2. StatDebuff

Since each character can only hold one debuff at the same time, which will make it difficult to handle the superposition of multiple debuffs. We have allocated another area (`sStatDebuffStatusAlly`, `sStatDebuffStatusEnemy`, `sStatDebuffStatusNpc`) for each unit and save multiple states in the form of bit masks.

For now, this function is used mainly for `DebuffInfo::unit_status` and `DebuffInfo::battle_status`.

Common API:
```c
// <debuff.h>
enum UNIT_STAT_DEBUFF_IDX {
    /* 0/1 is set as a bitmask to identify postive/negative status */
    UNIT_STAT_POSITIVE_BIT0,
    UNIT_STAT_POSITIVE_BIT1,

    UNIT_STAT_DEBUFF_IDX_START,

    UNIT_STAT_BUFF_RING_ATK = UNIT_STAT_DEBUFF_IDX_START,
    UNIT_STAT_BUFF_RING_DEF,
    UNIT_STAT_BUFF_RING_CRT,
    UNIT_STAT_BUFF_RING_AVO,

    UNIT_STAT_DEBUFF_POW,
    UNIT_STAT_DEBUFF_MAG,
    UNIT_STAT_DEBUFF_SKL,
    UNIT_STAT_DEBUFF_SPD,
    UNIT_STAT_DEBUFF_LCK,
    UNIT_STAT_DEBUFF_DEF,
    UNIT_STAT_DEBUFF_RES,
    UNIT_STAT_DEBUFF_MOV,

    UNIT_STAT_BUFF_POW,
    UNIT_STAT_BUFF_MAG,
    UNIT_STAT_BUFF_SKL,
    UNIT_STAT_BUFF_SPD,
    UNIT_STAT_BUFF_LCK,
    UNIT_STAT_BUFF_DEF,
    UNIT_STAT_BUFF_RES,
    UNIT_STAT_BUFF_MOV,

    UNIT_STAT_BUFF_INIT_POW,
    UNIT_STAT_BUFF_INIT_MAG,
    UNIT_STAT_BUFF_INIT_SKL,
    UNIT_STAT_BUFF_INIT_SPD,
    UNIT_STAT_BUFF_INIT_LCK,
    UNIT_STAT_BUFF_INIT_DEF,
    UNIT_STAT_BUFF_INIT_RES,
    UNIT_STAT_BUFF_INIT_MOV,

    UNIT_STAT_BUFF_OATHROUSE_POW,
    UNIT_STAT_BUFF_OATHROUSE_MAG,
    UNIT_STAT_BUFF_OATHROUSE_SKL,
    UNIT_STAT_BUFF_OATHROUSE_SPD,
    UNIT_STAT_BUFF_OATHROUSE_LCK,
    UNIT_STAT_BUFF_OATHROUSE_DEF,
    UNIT_STAT_BUFF_OATHROUSE_RES,
    UNIT_STAT_BUFF_OATHROUSE_MOV,

    UNIT_STAT_BUFF_JOB_HONE,
    UNIT_STAT_BUFF_JOB_FORTIFY,

    UNIT_STAT_DEBUFF_AversaNight,
    UNIT_STAT_DEBUFF_YuneWhispers,

    UNIT_STAT_BUFF_PLUSMINUS,

    UNIT_STAT_BUFF_ROUSE_POW,
    UNIT_STAT_BUFF_ROUSE_MAG,
    UNIT_STAT_BUFF_ROUSE_SKL,
    UNIT_STAT_BUFF_ROUSE_SPD,
    UNIT_STAT_BUFF_ROUSE_LCK,
    UNIT_STAT_BUFF_ROUSE_DEF,
    UNIT_STAT_BUFF_ROUSE_RES,
    UNIT_STAT_BUFF_ROUSE_MOV,

    UNIT_STAT_BUFF_RALLY_POW,
    UNIT_STAT_BUFF_RALLY_MAG,
    UNIT_STAT_BUFF_RALLY_SKL,
    UNIT_STAT_BUFF_RALLY_SPD,
    UNIT_STAT_BUFF_RALLY_LCK,
    UNIT_STAT_BUFF_RALLY_DEF,
    UNIT_STAT_BUFF_RALLY_RES,
    UNIT_STAT_BUFF_RALLY_MOV,

    UNIT_STAT_BUFF_INDOOR_MARCH_MOV,

    UNIT_STAT_DEBUFF_MAX_REAL,
    UNIT_STAT_DEBUFF_MAX = 128, /* DO NOT modify this */
};

void SetUnitStatDebuff(struct Unit *unit, enum UNIT_STAT_DEBUFF_IDX debuff);
void ClearUnitStatDebuff(struct Unit *unit, enum UNIT_STAT_DEBUFF_IDX debuff);
bool CheckUnitStatDebuff(struct Unit *unit, enum UNIT_STAT_DEBUFF_IDX debuff);
```

You can also use [event cmd](../Patches/PATCH_EVENTSCRIPT_Debuff.txt) to set or clear debuffs via events.
