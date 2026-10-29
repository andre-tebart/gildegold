/* 0x5702e0, the run of the event 0x89 (the state machine of the work at the building +0xac by the character +0xb4, the second character +0xb0): -1 / -2 release the character and free; states 0 .. 4 as in the code (0x524260, 0x4c52e0 with the text 0x628ea4, the progress 0x4fe140, 0x504e20 / 0x5a9990 / 0x5a99e0, 0x5a9a30 / 0x5a9af0); ROUGH */

int ai_Unk5a9990();                 /* 0x5a9990 */
int ai_Unk5a99e0();                 /* 0x5a99e0 */
int ai_Unk5a9a30();                 /* 0x5a9a30 */
int ai_Unk5a9af0();                 /* 0x5a9af0 */

void he_Unk5702e0(int ev)
{
    char *per = (char *)gm_FindChar(HM_I(ev, 0xb4));
    int state = HM_I(ev, 0x70);
    int r, b2;
    float f;
    unsigned char *job;

    if (state == -1 || state == -2) {
        if (per != 0)
            sv_MoveToGroup(0, 0, *(unsigned short *)per, 0);
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (per == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    switch (state) {
    case 0:
        sv_MoveToGroup(0, 0, *(unsigned short *)per, ev);
        HM_I(ev, 0x84) = -1;
        HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
        return;
    case 1:
        r = gm_BuildQuery(1, 1, HM_I(ev, 0xac));
        if (r != 0) {
            ai_Unk4c52e0(HM_I(ev, 0xb4), *(int *)(r + 1), -1, 0, 1, (char *)(unsigned)&full_data_628ea4);
            HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
            return;
        }
        HM_I(ev, 0x70) = -1;
        return;
    case 2:
        gm_BuildQuery(1, 1, HM_I(ev, 0xac));
        r = gm_FindChar(HM_I(ev, 0xb4));
        if (r == 0) {
            HM_I(ev, 0x70) = -1;
            return;
        }
        if (HM_I(HM_I(r, 0x184), 0x128) == 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 1, 0, 0);
            b2 = gm_FindChar(HM_I(ev, 0xb0));
            r = gm_FindChar(HM_I(ev, 0xb4));
            f = gm_Mix(r, b2, 2);
            if (b2 != 0) {
                if (1.0 < util_RandomF() + f) {
                    job = *(unsigned char **)(r + 0x16c);
                    HM_I(ev, 0xbc) = 1;
                    he_Unk56fbc0(b2, (char)job[0x5c] / 0x32 + (util_Random(3) & 0xffff) + *(unsigned char *)((unsigned)&ai_BTypes + *job * 0x24d + 0x247) * 2);
                    ai_Unk5a9990(b2);
                    HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
                    return;
                }
                ai_Unk5a99e0(b2);
                HM_I(ev, 0xbc) = 0;
            }
            HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
            return;
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 4, 0);
        return;
    case 3:
        r = gm_FindChar(HM_I(ev, 0xb4));
        if (r != 0 && (r = HM_I(r, 0x16c)) != 0) {
            ai_Unk4c52e0(HM_I(ev, 0xb4), *(int *)(r + 1), -1, 0, 0, (char *)(unsigned)&full_data_628ea4);
            HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
            return;
        }
        HM_I(ev, 0x70) = -1;
        return;
    case 4:
        r = gm_FindChar(HM_I(ev, 0xb4));
        if (r != 0 && HM_I(r, 0x16c) != 0) {
            if (HM_I(HM_I(r, 0x184), 0x128) != 0) {
                *(HETIME *)(ev + 0x52) = ai_Time;
                gm_AddTime(ev + 0x52, 0, 4, 0);
                return;
            }
            gm_BuildQuery(1, 1, HM_I(ev, 0xac));
            b2 = gm_FindChar(HM_I(ev, 0xb0));
            r = gm_FindChar(HM_I(ev, 0xb4));
            if (b2 != 0 && r != 0) {
                if (HM_I(ev, 0xbc) != 0) {
                    f = gm_GetSkill(r, 1);
                    if (1.0 < util_RandomF() + (1.0 - f))
                        ai_Unk5a9a30((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c, r, b2, 0);
                    else
                        ai_Unk5a9a30((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c, r, b2, 1);
                } else
                    ai_Unk5a9af0((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c, r, b2);
            }
            HM_I(ev, 0x70) = -1;
            return;
        }
        HM_I(ev, 0x70) = -1;
        return;
    default:
        HM_I(ev, 0x70) = -1;
        return;
    }
}
