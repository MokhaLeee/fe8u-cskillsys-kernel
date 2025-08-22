#include "common-chax.h"
#include "banim-hack.h"
#include "banim-sync.h"
#include "battle-system.h"
#include "kernel-lib.h"
#include "skill-system.h"

enum { LOCAL_SCORE_MAX = 3 };

extern EWRAM_DATA u16 BanimSwitcherBuf[NEW_BATTLE_HIT_MAX];
extern EWRAM_DATA struct BattleAnimDef BanimSwitcherAnimDef[2];

static bool check_end(const struct CustomAnimnSwitcher *it)
{
	if (it->pid == CHARACTER_NONE || it->banim_index <= 0)
		return true;

	return false;
}

STATIC_DECLAR void SetupBanimSwitcherBuf(void)
{
	int i, iround;
	struct CustomAnimnSwitcher act_ref, tar_ref;

	enum { BANIM_SWITCHER_MAX_CHUNKS = 128 };
	u32 attacker_valid_chunks_bitfile[4];
	u32 defender_valid_chunks_bitfile[4];

	memset(BanimSwitcherBuf, 0, sizeof(BanimSwitcherBuf));
	memset(attacker_valid_chunks_bitfile, 0, sizeof(attacker_valid_chunks_bitfile));
	memset(defender_valid_chunks_bitfile, 0, sizeof(defender_valid_chunks_bitfile));

	if (!gpCustomAnimnSwitcher)
		return;

	act_ref.pid = UNIT_CHAR_ID(&gBattleActor.unit);
	act_ref.jid = UNIT_CLASS_ID(&gBattleActor.unit);
	act_ref.wtype = gBattleActor.weaponType;
	act_ref.item = gBattleActor.weaponBefore;
	act_ref.tar_pid = UNIT_CHAR_ID(&gBattleTarget.unit);
	act_ref.tar_jid = UNIT_CLASS_ID(&gBattleTarget.unit);

	tar_ref.pid = act_ref.tar_pid;
	tar_ref.jid = act_ref.tar_jid;
	tar_ref.wtype = gBattleTarget.weaponType;
	tar_ref.item = gBattleTarget.weaponBefore;
	tar_ref.tar_pid = act_ref.pid;
	tar_ref.tar_jid = act_ref.jid;

	for (i = 0; i < BANIM_SWITCHER_MAX_CHUNKS; i++) {
		struct CustomAnimnSwitcher *ref;
		const struct CustomAnimnSwitcher *it = &gpCustomAnimnSwitcher[i];

		if (check_end(it))
			break;

		if (it->act_flag && !(CheckFlag(it->act_flag)))
			continue;

		if (it->pid == act_ref.pid) {
			ref = &act_ref;

			if (it->jid != 0 && it->jid != ref->jid)
				goto check_next;

			if (it->wtype != ref->wtype)
				goto check_next;

			if (it->item != 0 && it->item != ref->item)
				goto check_next;

			if (it->tar_pid != 0 && it->tar_pid != ref->tar_pid)
				goto check_next;

			if (it->tar_jid != 0 && it->tar_jid != ref->tar_jid)
				goto check_next;

			_BIT_SET(attacker_valid_chunks_bitfile, i);
		}

		check_next:
		if (it->pid == tar_ref.pid) {
			ref = &tar_ref;

			if (it->jid != 0 && it->jid != ref->jid)
				goto check_done;

			if (it->wtype != ref->wtype)
				goto check_done;

			if (it->item != 0 && it->item != ref->item)
				goto check_done;

			if (it->tar_pid != 0 && it->tar_pid != ref->tar_pid)
				goto check_done;

			if (it->tar_jid != 0 && it->tar_jid != ref->tar_jid)
				goto check_done;

			_BIT_SET(defender_valid_chunks_bitfile, i);
		}

		check_done:
	}

	for (iround = 0; iround < NEW_BATTLE_HIT_MAX; iround++) {
		int cur_score, final_score;
		struct BattleHit *hit = prBattleHitArray + iround * BattleHitArrayWidth;
		struct CustomAnimnSwitcher *ref;
		u32 *valid_chunks_bitfile;

		if (hit->info & BATTLE_HIT_INFO_END)
			break;

		if (!(hit->info & BATTLE_HIT_INFO_RETALIATION)) {
			valid_chunks_bitfile = attacker_valid_chunks_bitfile;
			ref = &act_ref;
			ref->sid = GetActorEfxSkill(iround);
		} else {
			valid_chunks_bitfile = defender_valid_chunks_bitfile;
			ref = &tar_ref;
			ref->sid = GetTargetEfxSkill(iround);
		}

		final_score = 0;

		for (i = 0; i < BANIM_SWITCHER_MAX_CHUNKS; i++) {
			const struct CustomAnimnSwitcher *it = &gpCustomAnimnSwitcher[i];

			if (check_end(it))
				break;

#if 1
			if (!_BIT_CHK(valid_chunks_bitfile, i))
				continue;
#else
			if (it->pid != ref->pid)
				continue;

			if (it->jid != 0 && it->jid != ref->jid)
				continue;

			if (it->wtype != ref->wtype)
				continue;

			if (it->item != 0 && it->item != ref->item)
				continue;

			if (it->tar_pid != 0 && it->tar_pid != ref->tar_pid)
				continue;

			if (it->tar_jid != 0 && it->tar_jid != ref->tar_jid)
				continue;
#endif

			cur_score = 0;

			if (it->act_when_crit) {
				if (!(hit->attributes & BATTLE_HIT_ATTR_CRIT))
					continue;

				cur_score = 1;
			}

			if (it->act_when_killing) {
				if (!(hit->info & BATTLE_HIT_INFO_KILLS_TARGET))
					continue;

				cur_score = 2;
			}

			if (it->sid != 0) {
				if (it->sid != ref->sid)
					continue;

				cur_score = 3;
			}

			if (final_score < cur_score) {
				BanimSwitcherBuf[iround] = it->banim_index;
				final_score = cur_score;
			}

			if (final_score == LOCAL_SCORE_MAX)
				break;
		}
	}
}

struct BanimSyncHandler *GetBanimSyncHandler_BanimSwitcher(struct Anim *anim)
{
	int round, bid;
	struct BanimSyncHandler *handler = &gBanimSyncHandler;
	struct BattleUnit *bu;

	if (!gpKernelDesigerConfig->banim_switcher_en)
		return NULL;

	if (!IsActorAnim(anim))
		return NULL;

	if (GetBattleAnimArenaFlag())
		return NULL;

	round = anim->nextRoundId - 1;
	if (round == 0)
		SetupBanimSwitcherBuf();

	bid = BanimSwitcherBuf[round];
	if (bid <= 0)
		return NULL;

	bu = (GetAnimPosition(anim) == POS_L) ? gpEkrBattleUnitLeft : gpEkrBattleUnitRight;

	BanimSwitcherAnimDef[0].wtype = bu->weaponType | 0x100;
	BanimSwitcherAnimDef[0].index = bid;
	BanimSwitcherAnimDef[1].wtype = 0;
	BanimSwitcherAnimDef[1].index = 0;

	handler->pid = UNIT_CHAR_ID(&bu->unit);
	handler->jid = UNIT_CLASS_ID(&bu->unit);
	handler->weapon = bu->weaponBefore;
	handler->animdef = BanimSwitcherAnimDef;
	return handler;
}
