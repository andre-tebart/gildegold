/* 0x542960, a Ghidra miss (disassembled): starts an offer event between the figures +0xac (p) and +0xb0 (q): sets the offer's own clock (+0xc5, packed, +0x18 units, state word 0xa at +0xc9) and, when flag 2 is set, looks through the other pending 0x2c events of the same kind (+0xc4): one with the same p only gets "0x1bbe" (message 0x5f5) and this event is re-posted at once, an earlier one of another p moves our clock to its time; at the end the offer is created (0x4c5580, kept at +0xd4) and posted (ROUGH) */

void ai_Unk4c5580(int a, int b, int c, int d);

void he_Unk542960(int ev)
{
    int q, p, r;
    unsigned char fl;
    char buf[0x100];

    q = gm_FindChar(*(int *)(ev + 0xb0));
    p = gm_FindChar(*(int *)(ev + 0xac));
    *(HETIME *)(ev + 0xc5) = ai_Time;
    gm_AddTime(ev + 0xc5, 0x18, 0, 0);
    *(short *)(ev + 0xc9) = 0xa;
    *(int *)(ev + 0xcb) = 0;
    *(HETIME *)(ev + 0x52) = ai_Time;
    fl = *(unsigned char *)(ev + 0x78);
    if ((fl & 2) != 0) {
        r = (int)he_FindEvent(1, 0, 0x2c);
        while (r != 0) {
            if (*(int *)(r + 4) != *(int *)(ev + 4) && *(char *)(r + 0xc4) == *(char *)(ev + 0xc4)) {
                if (*(int *)(r + 0xac) == *(int *)(ev + 0xac)) {
                    txt_Format(buf, 0x1bbe, *(unsigned short *)p, *(unsigned short *)q);
                    he_ShowNews(*(int *)(p + 4), *(int *)(p + 4), buf, 0, 0x5f5, (char *)0);
                    *(HETIME *)(ev + 0x52) = ai_Time;
                    *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
                    return;
                }
                if (*(int *)(ev + 0xc5) <= *(int *)(r + 0xc5)) {
                    *(HETIME *)(ev + 0xc5) = *(HETIME *)(r + 0xc5);
                    gm_AddTime(ev + 0xc5, 0x18, 0, 0);
                    *(short *)(ev + 0xc9) = 0xa;
                    *(int *)(ev + 0xcb) = 0;
                }
            }
            r = (int)he_NextEvent();
        }
        *(int *)(ev + 0xd4) = ((int (*)(int, int, int, int))ai_Unk4c5580)(*(int *)(ev + 0xac), *(int *)(ev + 0xb0), *(int *)(ev + 4), 1);
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    }
}
