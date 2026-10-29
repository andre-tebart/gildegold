/* 0x551b70, the run handler of the walk event 0x5512c0 (ROUGH): states -1 / -2 free the record; a record with flag 4 or a pending handle (+0x84, 0x4c3ae0 checks it) does nothing; else the due clock is set to now and posted 5 later, and after the minute word 0x11 the ten slots (+0xac, 16 bytes) are stepped by 0x551340 (counting the ones still walking = 2; when none walks, the minute word is reset and the day counter +0x52 incremented), before it the slots are stepped by 0x551340 (done: flag 4) or 0x551780 and the ones with flag 1 counted, then up to ten tries to fill a slot (0x551440) post the event again 1 later */

int cm_GetSlotState();
void gm_Shuffled(unsigned char n, int *out);

void he_Unk551b70(int ev)
{
    int st = *(int *)(ev + 0x70);
    int n = 0, i, r;
    unsigned char *s;

    if (st == -1 || st == -2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 5, 0);
    *(int *)(ev + 0x84) = -1;
    if (*(unsigned short *)(ev + 0x56) >= 0x11) {
        s = (unsigned char *)(ev + 0xac);
        for (i = 10; i != 0; i--) {
            if (he_Unk551340((int *)s) == 2)
                n++;
            s += 0x10;
        }
        if (n == 0) {
            *(unsigned short *)(ev + 0x56) = (unsigned short)n;
            *(int *)(ev + 0x52) = *(int *)(ev + 0x52) + 1;
        }
        cm_SendExSetHE(0, ev);
        return;
    }
    s = (unsigned char *)(ev + 0xac);
    for (i = 10; i != 0; i--) {
        if ((s[9] & 4) != 0)
            he_Unk551340((int *)s);
        else
            he_Unk551780(s);
        if ((s[9] & 1) != 0)
            n++;
        s += 0x10;
    }
    s = (unsigned char *)(ev + 0xac);
    for (i = 0; i < 10; i++) {
        r = he_Unk551440(s, n, 0);
        *(int *)(ev + 0x84) = r;
        if (r != -1) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 1, 0);
            cm_SendExSetHE(0, ev);
            return;
        }
        s += 0x10;
    }
    cm_SendExSetHE(0, ev);
}
