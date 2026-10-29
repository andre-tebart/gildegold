/* 0x5719e0, the run of the event 0x8e: only the states -2 .. 0; the character +0xc (0x505df0) missing -> free; the cost 0x509ee0 (character, the byte 0x6f1bc5) above +0xac -> 0x4c47e0 (-1, +4 of the character, the byte, +0xac) and free; else each of the five job records +0x68 .. present loses 0x4fca40 (id, -1001), or the character itself when none; free */

void he_Unk5719e0(int ev)
{
    char *per;
    char *r;
    int n, i, found = 0;

    if (HM_I(ev, 0x70) < -2 || HM_I(ev, 0x70) > 0)
        return;
    per = (char *)gm_FindChar(HM_I(ev, 0xc));
    if (per != 0) {
        n = ((int (*)(char *, char))gm_Unk509ee0)(per, *(char *)(unsigned)&ai_OwnPlayer);
        if (HM_I(ev, 0xac) < n) {
            cm_SendMoveGoods(-1, *(int *)(per + 4), *(unsigned char *)(unsigned)&ai_OwnPlayer, HM_I(ev, 0xac));
            he_Unk53e1f0((int *)ev);
            return;
        }
        for (i = 0; i < 5; i++) {
            if (HM_I(per, 0x68 + i * 4) != -1) {
                r = (char *)gm_FindChar(HM_I(per, 0x68 + i * 4));
                if (r != 0) {
                    ((float (*)(unsigned short, int))gm_AddHealth)(*(unsigned short *)r, -1001);
                    found = 1;
                }
            }
        }
        if (!found)
            ((float (*)(unsigned short, int))gm_AddHealth)(*(unsigned short *)per, -1001);
    }
    he_Unk53e1f0((int *)ev);
}
