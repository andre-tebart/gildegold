/* 0x565b60, the run handler of the death of a player (ROUGH): state 0 removes the follow object of his figure (0x4284a0 / 0x429e90) and clears his lock 0x524260; state 1 waits until the figure object (+0x184, its +0x28) is gone; state 2 clears the flag +8, the lock, the figure +0x16c and lays the figure down (0x405610, 0x40b2f0 (obj, 0x36, 1): the animation name "bewegung/zu_boden_sinken" at +0xf0), next run at once; state 3 informs the owner 0x4c4e00 (+4, 1) when the flag +0x78 & 2 is set; then freed */

void he_Unk565b60(int ev)
{
    unsigned short *p;
    int q, o, a;
    char *s, *d;

    switch (*(int *)(ev + 0x70)) {
    case 0:
        p = (unsigned short *)gm_FindChar(*(int *)(ev + 0xac));
        if (p != 0) {
            if (*(int *)(p + 0xc2) != 0 && (o = *(int *)(*(int *)(p + 0xc2) + 0x28)) != -1)
                evt_Stop(evt_FindById(o));
            sv_MoveToGroup(0, 0, *p, 0);
            *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
            return;
        }
        break;
    case 1:
        q = gm_FindChar(*(int *)(ev + 0xac));
        if (q != 0) {
            if (*(int *)(q + 0x184) != 0 && *(int *)(*(int *)(q + 0x184) + 0x28) != -1)
                return;
            *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
            return;
        }
        break;
    case 2:
        p = (unsigned short *)gm_FindChar(*(int *)(ev + 0xac));
        if (p != 0) {
            *(char *)(p + 4) = 0;
            sv_MoveToGroup(0, 0, *p, 0);
            *(int *)(p + 0xb6) = 0;
            if (*(int *)(p + 0xc2) != 0) {
                ch_StopActions(*(int *)(p + 0xc2));
                a = ch_AddAction(*(int *)(p + 0xc2), 0x36, 1);
                strcpy((char *)(a + 0xf0), "bewegung/zu_boden_sinken");
            }
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 1, 0, 0);
            *(int *)(ev + 0x70) = *(int *)(ev + 0x70) + 1;
            return;
        }
        break;
    case 3:
        q = gm_FindChar(*(int *)(ev + 0xac));
        if (q != 0 && (*(unsigned char *)(ev + 0x78) & 2) != 0)
            cm_SendKillCharacter(*(int *)(q + 4), 1);
        break;
    case -2:
    case -1:
        break;
    default:
        return;
    }
    he_Unk53e1f0((int *)ev);
}
