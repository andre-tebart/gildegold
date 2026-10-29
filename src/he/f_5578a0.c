/* 0x5578a0, the run handler of the training event (ROUGH skeleton): state 0 -> 1 (inc), state 1 waits for the due time, takes the person +0xac (0x4fea50) and its building (0x501190) and the two skill limits from the kind record (+0x240 / +0x241, +0x242 for the person type 0x116), counts +0xb0 (first) then +0xb4 (second) down while the person's skill byte (+0x1c / +0x1d) is below the limit, raising it by the message 0x4c3ba0 / 0x4c3d00 / 0x4c49a0; when both counters are 0 the person's owner gets the news 0x1bba (0x5f6 / 0x60b, with the 0x501b50 contact) and the record is freed; states -2 / -1 free */

void sv_MoveToGroup(unsigned char *o, short *it, int c, char *g);

void he_Unk5578a0(int ev)
{
    int st = *(int *)(ev + 0x70);
    int o = 0, b, kind, lim0, lim1, c, q, own, tid;
    char text[0x400];

    if (st < -2 || st > 1)
        return;
    switch (st) {
    case 0:
        *(int *)(ev + 0x70) = st + 1;
        return;
    case 1:
        if (gm_CompareTime(ev + 0x52, (unsigned)&ai_Time) >= 0)
            return;
        gm_FindById(&o, 0, 0, *(int *)(ev + 0xac));
        if (o == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        b = gm_BuildQuery(1, 1, *(int *)(ev + 0x10));
        if (b == 0) {
            he_Unk53e1f0((int *)ev);
            return;
        }
        kind = U_I((unsigned)&ai_BTypes) + *(unsigned char *)b * 0x24d;
        if (*(unsigned short *)o == 0x116) {
            lim0 = *(unsigned char *)(kind + 0x242);
            lim1 = lim0;
        } else {
            lim0 = *(unsigned char *)(kind + 0x240);
            lim1 = *(unsigned char *)(kind + 0x241);
        }
        if (*(int *)(ev + 0xb0) > 0) {
            *(int *)(ev + 0xb0) = *(int *)(ev + 0xb0) - 1;
            if (*(unsigned char *)(o + 0x1c) < lim0) {
                cm_BeginFieldBlock(o, *(int *)(o + 2));
                cm_AddFieldValues(1, 1, o - U_I((unsigned)&cm_FieldObject) + 0x1c, text);
                cm_SendFieldDeltas();
            }
        } else if (*(int *)(ev + 0xb4) > 0) {
            *(int *)(ev + 0xb4) = *(int *)(ev + 0xb4) - 1;
            if (*(unsigned char *)(o + 0x1d) < lim1) {
                cm_BeginFieldBlock(o, *(int *)(o + 2));
                cm_AddFieldValues(1, 1, o - U_I((unsigned)&cm_FieldObject) + 0x1d, text);
                cm_SendFieldDeltas();
            }
        }
        if (*(int *)(ev + 0xb4) == 0 && *(int *)(ev + 0xb0) == 0) {
            tid = 0x5f6;
            b = gm_BuildQuery(1, 0, *(int *)(ev + 0x10));
            if (b == 0) {
                he_Unk53e1f0((int *)ev);
                return;
            }
            own = gm_FindCharacterByMaster(b);
            if (own != 0 && (*(unsigned char *)(own + 0x1b4) & 1) != 0)
                tid = 0x60b;
            q = gm_BuildingMainObject(b);
            txt_Format(text, 0x1bba, b);
            if (q != 0)
                he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, tid, *(int *)(b + 1), *(int *)(q + 2), 0, 0);
            else
                he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), *(int *)(b + 1), text, 0, tid, -1, 0, 0, 0);
            he_Unk53e1f0((int *)ev);
            return;
        }
        gm_AddTime(ev + 0x52, 0, 0, 1);
        return;
    default:
        he_Unk53e1f0((int *)ev);
        return;
    }
}
