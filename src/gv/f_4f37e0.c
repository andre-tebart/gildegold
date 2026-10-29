/* 0x4f37e0, retires the record of the id (0x4f36e0): its counter (short +0x1a) counts down, with mode m = 0 it stays while the counter is not 0, states other than 1 stay unless m forces it; the owner (0x505df0 of +0x16)
   is told (0x5a9430) when the player's pair is removed; the pairs of the id are cleared and the record freed (state 0, ids -1). 3 no record, 2 counting, 1 kept, 0 done */

int unk_Unk4f37e0(int id, int m)
{
    int k, st;
    short cnt;
    int *q;
    int *o;
    unsigned short pl;

    k = unk_Unk4f36e0(id);
    if (k == -1)
        return 3;
    cnt = *(short *)(gv_Graveyard + 0x1a + k * 0x2d);
    if (cnt > 0)
        *(short *)(gv_Graveyard + 0x1a + k * 0x2d) = cnt - 1;
    if (*(short *)(gv_Graveyard + 0x1a + k * 0x2d) != 0 && m == 0)
        return 2;
    st = *(int *)(gv_Graveyard + 0x25 + k * 0x2d);
    if (st != 1 && (m == 0 || st == 0))
        return 1;
    o = (int *)gm_FindChar(*(int *)(gv_Graveyard + 0x16 + k * 0x2d));
    ord_SubGridAmount(*(int *)(gv_Graveyard + 0x21 + k * 0x2d), *(int *)(gv_Graveyard + 0x1c + k * 0x2d));
    q = (int *)(unk_Tab91b6e0 + 4);
    pl = ai_PlayerId;
    do {
        if (id == *q) {
            if (pl != 0xffff && q[-1] == *(int *)(ai_Chars[pl] + 4) && o != 0 && m == 0) {
                ai_Unk5a9430(ai_Chars[pl], o);
                pl = ai_PlayerId;
            }
            q[-1] = -1;
            *q = -1;
        }
        q += 2;
    } while ((int)q < ((unsigned)unk_Tab91b6e0 + 0x4004));
    *(int *)(gv_Graveyard + 0x25 + k * 0x2d) = 0;
    *(int *)(gv_Graveyard + 0x16 + k * 0x2d) = -1;
    *(int *)(gv_Graveyard + k * 0x2d) = -1;
    return 0;
}
