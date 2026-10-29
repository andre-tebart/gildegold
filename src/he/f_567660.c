/* 0x567660, a Ghidra miss: the run handler that warns a player once (ROUGH): state 0 with the flag word of the player (0x770d48 + id * 0x21c) negative (already warned) waits 4 units; else when his value 0x504510 (id, &out)
   is below 1.0 waits 20 units, else sends the message ('%', the player, +0x36 = 9, +0x60 = 2) 0x4c4b70, sets the sign bit of the flag word and waits 4 units; -1 / -2 free */

float gm_Unk504510(int n, int *out);

void he_Unk567660(int ev)
{
    int st, id;
    unsigned char m[0xf8];
    double v;
    int out[2];

    st = *(int *)(ev + 0x70);
    if (st < -2)
        return;
    if (st < 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (st != 0)
        return;
    id = *(unsigned short *)(ev + 8);
    if (*(int *)(((unsigned)&ai_Selected + 0x40) + id * 0x21c) < 0) {
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 4, 0, 0);
        return;
    }
    v = gm_Unk504510(id, out);
    if (v < 1.0) {
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0x14, 0);
        return;
    }
    memset(m, 0, sizeof(m));
    m[4] = 0x25;
    *(int *)(m + 0xc) = -1;
    m[0x36] = 9;
    *(int *)(m + 0x60) = 2;
    *(int *)(m + 8) = *(int *)((unsigned)&ai_CharInts + id * 0x21c);
    *(int *)(m + 0x5c) = *(int *)((unsigned)&ai_CharInts + id * 0x21c);
    cm_SendLongMessage(m);
    *(unsigned *)(((unsigned)&ai_Selected + 0x40) + id * 0x21c) |= 0x80000000;
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 4, 0, 0);
}
