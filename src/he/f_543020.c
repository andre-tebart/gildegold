/* 0x543020, a Ghidra miss (disassembled): the outcome of a quarrel between the figures +0xac / +0xb0 (a, b): re-posts in 0x30 units, takes 0x14 relation points of b towards a (0x4c4b00), tells a (news 0x175a) and sends the message 0x4f2e30 (0x13, 1, ..) with the staff record (+0x170) of b or a (ROUGH) */

void he_Unk543020(int ev)
{
    int a, b, t, id;
    char buf[0x200];
    unsigned char fl;

    a = gm_FindChar(*(int *)(ev + 0xac));
    b = gm_FindChar(*(int *)(ev + 0xb0));
    fl = *(unsigned char *)(ev + 0x78);
    *(int *)(ev + 0x84) = -1;
    if ((fl & 2) != 0) {
        if (a == 0 || b == 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            cm_SendExSetHE(-1, ev);
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0x30, 0, 0);
        cm_SendExSetHE(0, ev);
        cm_SendRelations(*(int *)(b + 4), *(int *)(a + 4), -20, 1);
        if (*(char *)(a + 2) == 6 || *(char *)(a + 2) == 7) {
            txt_Format(buf, 0x175a, *(unsigned short *)b, *(unsigned short *)b);
            he_ShowNews(*(int *)(a + 4), -1, buf, 0, 0x5fb, (char *)(unsigned)&ch_EmptyName);
        }
        t = *(int *)(b + 0x170);
        id = -1;
        if (t != 0) {
            id = *(int *)(t + 1);
        } else {
            t = *(int *)(a + 0x170);
            if (t != 0)
                id = *(int *)(t + 1);
        }
        unk_Unk4f2e30(0x13, 1, *(int *)(ev + 0xb0), *(int *)(ev + 0xac), id);
    }
}
