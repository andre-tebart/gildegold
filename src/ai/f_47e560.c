/* 0x47e560, the per-frame loop: runs the update of a share of the 768 characters (a count that follows from
   the game time passed since the last run)
   @deep byte; readability left: Q2 AICHAR.unk024 (int at 0x24; read as the ai.c per-frame decayed value sent as message field 0x24 ('whole = c->unk024; whole = -whole - (int)(whole * -0.89f)') and in am.c 0x494020 as 'sum = a->unk024 + a->unk01c' compared with an n * 70..190 limit, i.e. some accumulating score; no German string, text id, message comment or caller names it). Everything else named this session: ai_Unk47e560 -> ai_UpdateCharacters, ai_Unk47e360 -> ai_MasterUpdate, ai_Unk47e7f0 -> ai_CanAct, ai_Unk48a380 -> ai_UnloadCarts, ai_Unk4fe280 -> gm_MinutesBetween, ai_Unk5fef04 / 5fef08 -> ai_DifficultyValue / ai_DifficultyScale (the float copy of ai_Difficulty and its * 0.0625 + 0.5, both sent by cm_SendRelations). */

void ai_UpdateCharacters(void)
{
    AITIME now;
    AICHAR *c;
    int n, count, idx, whole, t;
    float f;
    char buf[1024];

    now = ai_TimeT;
    n = gm_MinutesBetween(&ai_LastUpdateTime, &now);
    f = n * 12.8f;
    whole = (int)f;
    ai_DifficultyValue = (float)ai_Difficulty;
    ai_DifficultyScale = ai_DifficultyValue * 0.0625f + 0.5f;
    if (n > 0 && ai_TimeT.hour <= 0x16 && ai_TimeT.hour >= 6) {
        if (ai_LastUpdateTime.year < now.year) {
            ai_LastUpdateTime = now;
            return;
        }
        if (n > 0x3c) {
            ai_LastUpdateTime.hour++;
            count = 0x300;
        } else {
            ai_LastUpdateTime = now;
            ai_CountCarry = (f - (float)whole) + ai_CountCarry;
            count = whole;
            if (ai_CountCarry >= 1.0f) {
                ai_CountCarry = ai_CountCarry - 1.0f;
                count = whole + 1;
            }
        }
        sprintf(buf, "Player count: %i, Starting with: %i Count: %i Deltatime: %i", count, ai_UpdateStart, count, n);
        idx = ai_UpdateStart;
        for (; count != 0; count--) {
            if (ai_CharsT[idx].id != 0xffff) {
                c = &ai_CharsT[idx];
                ai_UnloadCarts(c);
                if (ai_TimeT.hour > 6 && ai_TimeT.hour < 0x16 && !(ai_GameFlags & 0x80) &&
                    (c->kind == 5 || (((unsigned char)c->id ^ (unsigned char)ai_TimeT.hour) & 1) == 0))
                    ai_MasterUpdate(c);
                if ((ai_CanAct(c) && c->unk024 != 0) || (c->flags1cc & 0x3000) != 0) {
                    whole = c->unk024;
                    whole = -whole - (int)(whole * -0.89f);
                    cm_BeginFieldBlock(c, c->netId);
                    cm_AddFieldValues(4, 1, 0x24, &whole);
                    cm_SendFieldDeltas();
                }
            }
            idx = (idx + 1) % 0x300;
        }
        ai_UpdateStart = idx;
        if (idx == 0x300) {
            ai_UpdateStart = 0;
            ai_LastUpdateTime.hour++;
            ai_LastUpdateTime.min = 0;
            ai_LastUpdateTime.xy = 0;
            ai_CountCarry = 0.0f;
        }
        sprintf(buf, "Next Player: %i", ai_UpdateStart);
    }
}
