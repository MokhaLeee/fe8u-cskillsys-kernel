#include "common-chax.h"
#include "efx-skill.h"
#include "battle-system.h"
#include "banim-hack.h"

struct ProcC07Handler {
	PROC_HEADER;
	struct Anim *anim;
};

STATIC_DECLAR void LockSetAnimState(struct Anim *anim)
{
	struct Anim *anim1, *anim2;

	anim1 = gAnims[GetAnimPosition(anim) * 2];
	anim2 = gAnims[GetAnimPosition(anim) * 2 + 1];

	anim->state3 |= ANIM_BIT3_BLOCKING;
	anim->state |= ANIM_BIT_FROZEN;

	anim1->state3 |= ANIM_BIT3_BLOCKING;
	anim1->state |= ANIM_BIT_FROZEN;

	anim2->state3 |= ANIM_BIT3_BLOCKING;
	anim2->state |= ANIM_BIT_FROZEN;
}

STATIC_DECLAR void UnlockAnimState(struct Anim *anim)
{
	struct Anim *anim1, *anim2;

	anim1 = gAnims[GetAnimPosition(anim) * 2];
	anim2 = gAnims[GetAnimPosition(anim) * 2 + 1];

	anim->state3 |= ANIM_BIT3_BLOCKEND;
	anim->state &= ~ANIM_BIT_FROZEN;

	anim1->state3 |= ANIM_BIT3_BLOCKEND;
	anim1->state &= ~ANIM_BIT_FROZEN;

	anim2->state3 |= ANIM_BIT3_BLOCKEND;
	anim2->state &= ~ANIM_BIT_FROZEN;
}

STATIC_DECLAR void C07Handler_Init(struct ProcC07Handler *proc)
{
	LockSetAnimState(proc->anim);
}

STATIC_DECLAR void C07Handler_HpCost(struct ProcC07Handler *proc)
{
	int round;

	round = proc->anim->nextRoundId - 1;
	if (round < 0 || round >= NEW_BATTLE_HIT_MAX)
		return;

	if (GetExtBattleHit(round)->hp_cost > 0)
		NewEfxHpCost(proc->anim);
}

STATIC_DECLAR void C07Handler_WaitHpCost(struct ProcC07Handler *proc)
{
	if (!EfxAnimNumberExists() && !EfxHpCostExists())
		Proc_Break(proc);
}

STATIC_DECLAR void C07Handler_EkrSkill(struct ProcC07Handler *proc)
{
	NewEkrSkill(proc->anim);
}

STATIC_DECLAR void C07Handler_End(struct ProcC07Handler *proc)
{
	UnlockAnimState(proc->anim);
}

STATIC_DECLAR const struct ProcCmd ProcScr_BanimC07Handler[] = {
	PROC_NAME("C07Handler"),
	PROC_YIELD,
	PROC_CALL(C07Handler_Init),

	/**
	 * Hp cost
	 */
	PROC_YIELD,
	PROC_CALL(C07Handler_HpCost),
	PROC_YIELD,
	PROC_REPEAT(C07Handler_WaitHpCost),

	/**
	 * Ekr-skill
	 */
	PROC_YIELD,
	PROC_CALL(C07Handler_EkrSkill),
	PROC_YIELD,
	PROC_WHILE(EkrSkillExists),

	PROC_YIELD,
	PROC_CALL(C07Handler_End),
	PROC_END
};

/**
 * C07: Start attack animation
 */
void Banim_C07(struct Anim *anim)
{
	struct ProcC07Handler *proc;

	/* We need to block the script */
	anim->pScrCurrent = anim->pScrCurrent - 1;

	if (!(anim->state3 & ANIM_BIT3_BLOCKING)) {
		anim->state3 |= ANIM_BIT3_BLOCKING;

		if (GetAISLayerId(anim) == 0) {
			proc = Proc_Start(ProcScr_BanimC07Handler, PROC_TREE_3);
			proc->anim = anim;
		}
	}

	if ((anim->state3 & ANIM_BIT3_BLOCKING)) {
		if (anim->state3 & ANIM_BIT3_BLOCKEND) {
			anim->state3 &= ~ANIM_BIT3_BLOCKING;
			anim->state3 &= ~ANIM_BIT3_BLOCKEND;
			anim->pScrCurrent = anim->pScrCurrent + 1;
		}
	}
}
