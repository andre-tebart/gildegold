/* 0x567f50, the per-character step of a patrol / escort event (ROUGH; ev, the character record c, its +0x24 the current wait counter): when the character is idle (+0x24 == 0) with probability by the stat +0x12 the
   news 0x10 is sent (the message type 0x10 with the clock, 0x4fe140 (&clock, 0, 0, 10), the stat 0x5087f0 / 0x4c47e0), else the next waypoint (0x4bda80 (+0x29, +0x2d), the object 0x502020) is walked to: the pause
   table {12, 8, 4, 2, 0} by +0x31 counts +0x32 up (0x524810, 0x5024f0, 0x4bdaa0 wraps the index at the route length), 0x4c3ba0 / 0x4c3d90 set the route index (+0x2d) and the result 0x4c49e0 goes to +0x84;
   a blocked step with the neighbour 0x50a0f0 sends the message type 0xf */

void he_Unk567f50(int ev, unsigned short *c)
{
    int o, w, iv, n, k, t;
    int pause[7];
    unsigned char m[0xf8];

    if (*(int *)(c + 0x12) != 0)
        return;
    o = gm_Unk50a050(c);
    if (*(unsigned char *)(c + 9) < (unsigned)((unsigned short)util_Random(0x20) + 0x28)) {
        memset(m, 0, sizeof(m));
        *(int *)(m + 8) = *(int *)(o + 4);
        *(int *)(m + 0x2c) = U_I((unsigned)&ai_TimeHour);
        *(int *)(m + 0x28) = U_I((unsigned)&ai_Time);
        m[4] = 0x10;
        m[0x36] = 1;
        *(int *)(m + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
        *(short *)(m + 0x34) = *(short *)((unsigned)&ai_Time + 0xc);
        *(int *)(m + 0xc) = *(int *)(m + 8);
        gm_AddTime(m + 0x28, 0, 0, 10);
        *(int *)(m + 0x58) = *(int *)(c + 1);
        *(int *)(m + 0x5c) = 100 - *(unsigned char *)(c + 9);
        *(int *)(m + 0x64) = 5;
        *(int *)(m + 0x84) = 0;
        gm_ItemPrice(*c, 100);
        t = 100 - *(unsigned char *)(c + 9);
        cm_SendMoveGoods(-1, *(int *)(o + 4), U_I((unsigned)&ai_OwnPlayer), t);
        cm_SendLongMessage(m);
        return;
    }
    w = rt_PointAt(*(int *)((char *)c + 0x29), *(int *)((char *)c + 0x2d));
    if (w != 0 && (o = gm_FindBuildingById(*(int *)(w + 4))) != 0) {
        if (*(int *)((char *)c + 0x2d) == 0) {
            pause[1] = 0xc;
            pause[2] = 8;
            pause[3] = 4;
            pause[4] = 2;
            pause[5] = 0;
            k = pause[*(unsigned char *)((char *)c + 0x31) + 1];
            *(unsigned char *)(c + 0x19) = *(unsigned char *)(c + 0x19) + 1;
            if (*(unsigned char *)(c + 0x19) < k)
                return;
            *(unsigned char *)(c + 0x19) = 0;
        }
        if (sv_Unk524810(c, w) != 0 && (gm_IsType10(w) == 0 || *(int *)(c + 3) == *(int *)(U_I((unsigned)&full_data_6f2638) + 1))) {
            iv = rt_PointCount(*(int *)((char *)c + 0x29));
            n = *(int *)((char *)c + 0x2d) + 1;
            if (iv <= n)
                n = 0;
            cm_BeginFieldBlock(c, *(int *)(c + 1));
            cm_AddFieldValue(4, 1, (int)c + (0x2d - U_I((unsigned)&cm_FieldObject)), &n);
            *(int *)(ev + 0x84) = ((int (*)())cm_SendFieldValues)();
            return;
        }
        k = gm_Unk50a0f0(c);
        if (k != 0) {
            memset(m, 0, sizeof(m));
            *(int *)(m + 8) = *(int *)(o + 4);
            *(int *)(m + 0x28) = U_I((unsigned)&ai_Time);
            *(int *)(m + 0x30) = U_I(((unsigned)&ai_Time + 0x8));
            *(int *)(m + 0x58) = *(int *)(c + 1);
            *(int *)(m + 0x2c) = U_I((unsigned)&ai_TimeHour);
            m[4] = 0xf;
            *(int *)(m + 0xc) = -1;
            m[0x36] = 2;
            *(short *)(m + 0x34) = *(short *)((unsigned)&ai_Time + 0xc);
            *(int *)(m + 0x5c) = *(int *)(k + 1);
            *(int *)(m + 0x60) = *(int *)(w + 1);
            m[0x9e] = 1;
            cm_SendLongMessage(m);
        }
    }
}
