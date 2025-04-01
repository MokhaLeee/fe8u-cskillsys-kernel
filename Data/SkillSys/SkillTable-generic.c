#include "common-chax.h"
#include "skill-system.h"
#include "constants/skills.h"

/* Maximum of 7 skills here */
#ifdef CONFIG_FE8SRR
const struct SkillPreloadPConf gSkillPreloadPData[0x100] = {
    // 01
    [CHARACTER_EIRIKA] = {
        .skills = {
            [0] = SID_BlacksmithPlus,
        },
    },
    // 02
    [CHARACTER_SETH] = {
        .skills = {
            [0] = SID_Adaptable,
        },
    },
    // 03
    [CHARACTER_GILLIAM] = {
        .skills = {
            [0] = SID_Alacrity,
        },
    },
    // 04
    [CHARACTER_FRANZ] = {
        .skills = {
            [0] = SID_Aptitude,
        },
    },
    // 05
    [CHARACTER_MOULDER] = {
        .skills = {
            [0] = SID_Arise,
        },
    },
    // 06
    [CHARACTER_VANESSA] = {
        .skills = {
            [0] = SID_AssignDecoy,
        },
    },
    // 07
    [CHARACTER_ROSS] = {
        .skills = {
            [0] = SID_Canter,
        },
    },
    // 08
    [CHARACTER_NEIMI] = {
        .skills = {
            [0] = SID_Camouflage,
        },
    },
    // 09
    [CHARACTER_COLM] = {
        .skills = {
            [0] = SID_ArmsthriftPlus,
        },
    },
    // 0A
    [CHARACTER_GARCIA] = {
        .skills = {
            [0] = SID_BoundlessVitality,
        },
    },
    // 0B
    [CHARACTER_INNES] = {
        .skills = {
            [0] = SID_Chipper,
        },
    },
    // 0C
    [CHARACTER_LUTE] = {
        .skills = {
            [0] = SID_Comatose,
        },
    },
    // 0D
    [CHARACTER_NATASHA] = {
        .skills = {
            [0] = SID_Counterattack,
        },
    },
    // 0E
    [CHARACTER_CORMAG] = {
        .skills = {
            [0] = SID_CriticalOverload,
        },
    },
    // 0F
    [CHARACTER_EPHRAIM] = {
        .skills = {
            [0] = SID_CriticalPierce,
        },
    },
    // 10
    [CHARACTER_FORDE] = {
        .skills = {
            [0] = SID_DarkHorse,
        },
    },
    // 11
    [CHARACTER_KYLE] = {
        .skills = {
            [0] = SID_Dazzle,
        },
    },
    // 12
    [CHARACTER_AMELIA] = {
        .skills = {
            [0] = SID_Decadon,
        },
    },
    // 13
    [CHARACTER_ARTUR] = {
        .skills = {
            [0] = SID_Debilitator,
        },
    },
    // 14
    [CHARACTER_GERIK] = {
        .skills = {
            [0] = SID_DemolitionExpert,
        },
    },
    // 15
    [CHARACTER_TETHYS] = {
        .skills = {
            [0] = SID_DoubleUp,
        },
    },
    // 16
    [CHARACTER_MARISA] = {
        .skills = {
            [0] = SID_Duel,
        },
    },
    // 17
    [CHARACTER_SALEH] = {
        .skills = {
            [0] = SID_Echo,
        },
    },
    // 18
    [CHARACTER_EWAN] = {
        .skills = {
            [0] = SID_Entrepreneur,
        },
    },
    // 19
    [CHARACTER_LARACHEL] = {
        .skills = {
            [0] = SID_Equalizer,
        },
    },
    // 1A
    [CHARACTER_DOZLA] = {
        .skills = {
            [0] = SID_Fluffy,
        },
    },
    // 1C
    [CHARACTER_RENNAC] = {
        .skills = {
            [0] = SID_Flurry,
        },
    },
    // 1D
    [CHARACTER_DUESSEL] = {
        .skills = {
            [0] = SID_GoddessDance,
        },
    },
    // 1E
    [CHARACTER_MYRRH] = {
        .skills = {
            [0] = SID_GoldDigger,
        },
    },
    // 1F
    [CHARACTER_KNOLL] = {
        .skills = {
            [0] = SID_GoodAsGold,
        },
    },
    // 20
    [CHARACTER_JOSHUA] = {
        .skills = {
            [0] = SID_GoodyBasket,
        },
    },
    // 21
    [CHARACTER_SYRENE] = {
        .skills = {
            [0] = SID_GorillaTactics,
        },
    },
    // 22
    [CHARACTER_TANA] = {
        .skills = {
            [0] = SID_GracegiftPlus,
        },
    },
    // 23
    [CHARACTER_LYON_CC] = {
        .skills = {
            [0] = SID_GrazingBlow,
        },
    },
    // 24
    [CHARACTER_ORSON_CC] = {
        .skills = {
            [0] = SID_HackgiftPlus,
        },
    },
    // 25
    [CHARACTER_GLEN_CC] = {
        .skills = {
            [0] = SID_HazeHunter,
        },
    },
    // 26
    [CHARACTER_SELENA_CC] = {
        .skills = {
            [0] = SID_HedgeFund,
        },
    },
    // 27
    [CHARACTER_VALTER_CC] = {
        .skills = {
            [0] = SID_IronDome,
        },
    },
    // 28
    [CHARACTER_RIEV_CC] = {
        .skills = {
            [0] = SID_KillStreak,
        },
    },
    // 29
    [CHARACTER_CAELLACH_CC] = {
        .skills = {
            [0] = SID_LeadByExample,
        },
    },
    // 2A
    [CHARACTER_FADO_CC] = {
        .skills = {
            [0] = SID_LeadByExample,
        },
    },
    // 2B
    [CHARACTER_ISMAIRE_CC] = {
        .skills = {
            [0] = SID_LimitBreaker,
        },
    },
    // 2C
    [CHARACTER_HAYDEN_CC] = {
        .skills = {
            [0] = SID_LimitBreakerPlus,
        },
    },
    // 40
    [CHARACTER_LYON] = {
        .skills = {
            [0] = SID_Ludopathy,
        },
    },
    // 41
    [CHARACTER_MORVA] = {
        .skills = {
            [0] = SID_LuminaPlus,
        },
    },
    // 42
    [CHARACTER_ORSON_CH5X] = {
        .skills = {
            [0] = SID_MagicBounce,
        },
    },
    // 43
    [CHARACTER_VALTER] = {
        .skills = {
            [0] = SID_MakeAKilling,
        },
    },
    // 44
    [CHARACTER_SELENA] = {
        .skills = {
            [0] = SID_MendArms,
        },
    },
    // 45
    [CHARACTER_VALTER_PROLOGUE] = {
        .skills = {
            [0] = SID_MendArms,
        },
    },
    // 46
    [CHARACTER_BREGUET] = {
        .skills = {
            [0] = SID_Merciless,
        },
    },
    // 47
    [CHARACTER_BONE] = {
        .skills = {
            [0] = SID_Mercurious,
        },
    },
    // 48
    [CHARACTER_BAZBA] = {
        .skills = {
            [0] = SID_Mimic,
        },
    },
    // 4A
    [CHARACTER_SAAR] = {
        .skills = {
            [0] = SID_Mine,
        },
    },
    // 4B
    [CHARACTER_NOVALA] = {
        .skills = {
            [0] = SID_Moody,
        },
    },
    // 4C
    [CHARACTER_MURRAY] = {
        .skills = {
            [0] = SID_MountainClimber,
        },
    },
    // 4D
    [CHARACTER_TIRADO] = {
        .skills = {
            [0] = SID_MountainMan,
        },
    },
    // 4E
    [CHARACTER_BINKS] = {
        .skills = {
            [0] = SID_NecroCopy,
        },
    },
    // 4F
    [CHARACTER_PABLO] = {
        .skills = {
            [0] = SID_NoGuard,
        },
    },
    // 51
    [CHARACTER_AIAS] = {
        .skills = {
            [0] = SID_NoGuardPlus,
        },
    },
    // 52
    [CHARACTER_CARLYLE] = {
        .skills = {
            [0] = SID_Nonconforming,
        },
    },
    // 53
    [CHARACTER_CAELLACH] = {
        .skills = {
            [0] = SID_Obstruct,
        },
    },
    // 54
    [CHARACTER_PABLO_2] = {
        .skills = {
            [0] = SID_Offhand,
        },
    },
    // 57
    [CHARACTER_RIEV] = {
        .skills = {
            [0] = SID_OgreBody,
        },
    },
    // 5A
    [CHARACTER_GHEB] = {
        .skills = {
            [0] = SID_PairUp,
        },
    },
    // 5B
    [CHARACTER_BERAN] = {
        .skills = {
            [0] = SID_PhaseShift,
        },
    },
    // 68
    [CHARACTER_ONEILL] = {
        .skills = {
            [0] = SID_Pickup,
        },
    },
    // 69
    [CHARACTER_GLEN] = {
        .skills = {
            [0] = SID_PiercegiftPlus,
        },
    },
    // 6A
    [CHARACTER_ZONTA] = {
        .skills = {
            [0] = SID_Prodigy,
        },
    },
    // 6B
    [CHARACTER_VIGARDE] = {
        .skills = {
            [0] = SID_QuickHands,
        },
    },
    // 6C
    [CHARACTER_LYON_FINAL] = {
        .skills = {
            [0] = SID_RampUp,
        },
    },
    // 6D
    [CHARACTER_ORSON] = {
        .skills = {
            [0] = SID_Reflex,
        },
    },
};

#else
const struct SkillPreloadPConf gSkillPreloadPData[0x100] = {
    // 01
    [CHARACTER_EIRIKA] = {
        .skills = {
            // [0] = SID_PeoplesKnight,
        },
    },
    // // 02
    // [CHARACTER_SETH] = {
    //     .skills = {
    //         [0] = SID_Adaptable,
    //     },
    // },
    // // 03
    // [CHARACTER_GILLIAM] = {
    //     .skills = {
    //         [0] = SID_Alacrity,
    //     },
    // },
    // // 04
    // [CHARACTER_FRANZ] = {
    //     .skills = {
    //         [0] = SID_Aptitude,
    //     },
    // },
    // // 05
    // [CHARACTER_MOULDER] = {
    //     .skills = {
    //         [0] = SID_Arise,
    //     },
    // },
    // // 06
    // [CHARACTER_VANESSA] = {
    //     .skills = {
    //         [0] = SID_AssignDecoy,
    //     },
    // },
    // // 07
    // [CHARACTER_ROSS] = {
    //     .skills = {
    //         [0] = SID_Canter,
    //     },
    // },
    // // 08
    // [CHARACTER_NEIMI] = {
    //     .skills = {
    //         [0] = SID_Camouflage,
    //     },
    // },
    // // 09
    // [CHARACTER_COLM] = {
    //     .skills = {
    //         [0] = SID_ArmsthriftPlus,
    //     },
    // },
    // // 0A
    // [CHARACTER_GARCIA] = {
    //     .skills = {
    //         [0] = SID_BoundlessVitality,
    //     },
    // },
    // // 0B
    // [CHARACTER_INNES] = {
    //     .skills = {
    //         [0] = SID_Chipper,
    //     },
    // },
    // // 0C
    // [CHARACTER_LUTE] = {
    //     .skills = {
    //         [0] = SID_Comatose,
    //     },
    // },
    // // 0D
    // [CHARACTER_NATASHA] = {
    //     .skills = {
    //         [0] = SID_Counterattack,
    //     },
    // },
    // // 0E
    // [CHARACTER_CORMAG] = {
    //     .skills = {
    //         [0] = SID_CriticalOverload,
    //     },
    // },
    // // 0F
    // [CHARACTER_EPHRAIM] = {
    //     .skills = {
    //         [0] = SID_CriticalPierce,
    //     },
    // },
    // // 10
    // [CHARACTER_FORDE] = {
    //     .skills = {
    //         [0] = SID_DarkHorse,
    //     },
    // },
    // // 11
    // [CHARACTER_KYLE] = {
    //     .skills = {
    //         [0] = SID_Dazzle,
    //     },
    // },
    // // 12
    // [CHARACTER_AMELIA] = {
    //     .skills = {
    //         [0] = SID_Decadon,
    //     },
    // },
    // // 13
    // [CHARACTER_ARTUR] = {
    //     .skills = {
    //         [0] = SID_Debilitator,
    //     },
    // },
    // // 14
    // [CHARACTER_GERIK] = {
    //     .skills = {
    //         [0] = SID_DemolitionExpert,
    //     },
    // },
    // // 15
    // [CHARACTER_TETHYS] = {
    //     .skills = {
    //         [0] = SID_DoubleUp,
    //     },
    // },
    // // 16
    // [CHARACTER_MARISA] = {
    //     .skills = {
    //         [0] = SID_Duel,
    //     },
    // },
    // // 17
    // [CHARACTER_SALEH] = {
    //     .skills = {
    //         [0] = SID_Echo,
    //     },
    // },
    // // 18
    // [CHARACTER_EWAN] = {
    //     .skills = {
    //         [0] = SID_Entrepreneur,
    //     },
    // },
    // // 19
    // [CHARACTER_LARACHEL] = {
    //     .skills = {
    //         [0] = SID_Equalizer,
    //     },
    // },
    // // 1A
    // [CHARACTER_DOZLA] = {
    //     .skills = {
    //         [0] = SID_Fluffy,
    //     },
    // },
    // // 1C
    // [CHARACTER_RENNAC] = {
    //     .skills = {
    //         [0] = SID_Flurry,
    //     },
    // },
    // // 1D
    // [CHARACTER_DUESSEL] = {
    //     .skills = {
    //         [0] = SID_GoddessDance,
    //     },
    // },
    // // 1E
    // [CHARACTER_MYRRH] = {
    //     .skills = {
    //         [0] = SID_GoldDigger,
    //     },
    // },
    // // 1F
    // [CHARACTER_KNOLL] = {
    //     .skills = {
    //         [0] = SID_GoodAsGold,
    //     },
    // },
    // // 20
    // [CHARACTER_JOSHUA] = {
    //     .skills = {
    //         [0] = SID_GoodyBasket,
    //     },
    // },
    // // 21
    // [CHARACTER_SYRENE] = {
    //     .skills = {
    //         [0] = SID_GorillaTactics,
    //     },
    // },
    // // 22
    // [CHARACTER_TANA] = {
    //     .skills = {
    //         [0] = SID_GracegiftPlus,
    //     },
    // },
    // // 23
    // [CHARACTER_LYON_CC] = {
    //     .skills = {
    //         [0] = SID_GrazingBlow,
    //     },
    // },
    // // 24
    // [CHARACTER_ORSON_CC] = {
    //     .skills = {
    //         [0] = SID_HackgiftPlus,
    //     },
    // },
    // // 25
    // [CHARACTER_GLEN_CC] = {
    //     .skills = {
    //         [0] = SID_HazeHunter,
    //     },
    // },
    // // 26
    // [CHARACTER_SELENA_CC] = {
    //     .skills = {
    //         [0] = SID_HedgeFund,
    //     },
    // },
    // // 27
    // [CHARACTER_VALTER_CC] = {
    //     .skills = {
    //         [0] = SID_IronDome,
    //     },
    // },
    // // 28
    // [CHARACTER_RIEV_CC] = {
    //     .skills = {
    //         [0] = SID_KillStreak,
    //     },
    // },
    // // 29
    // [CHARACTER_CAELLACH_CC] = {
    //     .skills = {
    //         [0] = SID_LeadByExample,
    //     },
    // },
    // // 2A
    // [CHARACTER_FADO_CC] = {
    //     .skills = {
    //         [0] = SID_LeadByExample,
    //     },
    // },
    // // 2B
    // [CHARACTER_ISMAIRE_CC] = {
    //     .skills = {
    //         [0] = SID_LimitBreaker,
    //     },
    // },
    // // 2C
    // [CHARACTER_HAYDEN_CC] = {
    //     .skills = {
    //         [0] = SID_LimitBreakerPlus,
    //     },
    // },
    // // 40
    // [CHARACTER_LYON] = {
    //     .skills = {
    //         [0] = SID_Ludopathy,
    //     },
    // },
    // // 41
    // [CHARACTER_MORVA] = {
    //     .skills = {
    //         [0] = SID_LuminaPlus,
    //     },
    // },
    // // 42
    // [CHARACTER_ORSON_CH5X] = {
    //     .skills = {
    //         [0] = SID_MagicBounce,
    //     },
    // },
    // // 43
    // [CHARACTER_VALTER] = {
    //     .skills = {
    //         [0] = SID_MakeAKilling,
    //     },
    // },
    // // 44
    // [CHARACTER_SELENA] = {
    //     .skills = {
    //         [0] = SID_MendArms,
    //     },
    // },
    // // 45
    // [CHARACTER_VALTER_PROLOGUE] = {
    //     .skills = {
    //         [0] = SID_MendArms,
    //     },
    // },
    // // 46
    // [CHARACTER_BREGUET] = {
    //     .skills = {
    //         [0] = SID_Merciless,
    //     },
    // },
    // // 47
    // [CHARACTER_BONE] = {
    //     .skills = {
    //         [0] = SID_Mercurious,
    //     },
    // },
    // // 48
    // [CHARACTER_BAZBA] = {
    //     .skills = {
    //         [0] = SID_Mimic,
    //     },
    // },
    // // 4A
    // [CHARACTER_SAAR] = {
    //     .skills = {
    //         [0] = SID_Mine,
    //     },
    // },
    // // 4B
    // [CHARACTER_NOVALA] = {
    //     .skills = {
    //         [0] = SID_Moody,
    //     },
    // },
    // // 4C
    // [CHARACTER_MURRAY] = {
    //     .skills = {
    //         [0] = SID_MountainClimber,
    //     },
    // },
    // // 4D
    // [CHARACTER_TIRADO] = {
    //     .skills = {
    //         [0] = SID_MountainMan,
    //     },
    // },
    // // 4E
    // [CHARACTER_BINKS] = {
    //     .skills = {
    //         [0] = SID_NecroCopy,
    //     },
    // },
    // // 4F
    // [CHARACTER_PABLO] = {
    //     .skills = {
    //         [0] = SID_NoGuard,
    //     },
    // },
    // // 51
    // [CHARACTER_AIAS] = {
    //     .skills = {
    //         [0] = SID_NoGuardPlus,
    //     },
    // },
    // // 52
    // [CHARACTER_CARLYLE] = {
    //     .skills = {
    //         [0] = SID_Nonconforming,
    //     },
    // },
    // // 53
    // [CHARACTER_CAELLACH] = {
    //     .skills = {
    //         [0] = SID_Obstruct,
    //     },
    // },
    // // 54
    // [CHARACTER_PABLO_2] = {
    //     .skills = {
    //         [0] = SID_Offhand,
    //     },
    // },
    // // 57
    // [CHARACTER_RIEV] = {
    //     .skills = {
    //         [0] = SID_OgreBody,
    //     },
    // },
    // // 5A
    // [CHARACTER_GHEB] = {
    //     .skills = {
    //         [0] = SID_PairUp,
    //     },
    // },
    // // 5B
    // [CHARACTER_BERAN] = {
    //     .skills = {
    //         [0] = SID_PhaseShift,
    //     },
    // },
    // // 68
    // [CHARACTER_ONEILL] = {
    //     .skills = {
    //         [0] = SID_Pickup,
    //     },
    // },
    // // 69
    // [CHARACTER_GLEN] = {
    //     .skills = {
    //         [0] = SID_PiercegiftPlus,
    //     },
    // },
    // // 6A
    // [CHARACTER_ZONTA] = {
    //     .skills = {
    //         [0] = SID_Prodigy,
    //     },
    // },
    // // 6B
    // [CHARACTER_VIGARDE] = {
    //     .skills = {
    //         [0] = SID_QuickHands,
    //     },
    // },
    // // 6C
    // [CHARACTER_LYON_FINAL] = {
    //     .skills = {
    //         [0] = SID_RampUp,
    //     },
    // },
    // // 6D
    // [CHARACTER_ORSON] = {
    //     .skills = {
    //         [0] = SID_Reflex,
    //     },
    // },
};
#endif
const struct SkillPreloadJConf gSkillPreloadJData[0x100] = {
    [CLASS_SNIPER] = {
    },

    [CLASS_SNIPER_F] = {
    },

    [CLASS_PALADIN] = {
    }
};
