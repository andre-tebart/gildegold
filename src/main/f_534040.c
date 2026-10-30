/* 0x534040, sets up the starting buildings of the player p: the objects (0x501190 (1, 0, kind)) of the kinds 6 (the home: the name 0x624ce0 .., the lists 0x4ffe90, four messages 0x4c4880), 0x20, 0x2a, 0x1e, 0x1f,
   0x12, 0x14, 0x32, 0x21, 0x19, 0x34, 0x36 get the owner p (0x502b60 (o, p, p)) and a few start messages (0x4c4880 (id, -1, msg, count, 0x6f1bc5, 0)) whose completion is awaited with 0x52fe00 / 0x4c3b50; ROUGH */

void main_Unk534040(int p)
{
    int o, l, i;

    o = gm_BuildQuery(1, 0, 6);
    if (o != 0) {
        *(int *)(o + 5) = U_I((unsigned)&full_data_624ce0);
        *(int *)(o + 9) = U_I((unsigned)&full_data_624ce4);
        *(int *)(o + 0xd) = U_I((unsigned)&full_data_624ce8);
        *(int *)(o + 0x11) = U_I((unsigned)&full_data_624cec);
        gm_AssignBuilding(o, p, p);
        l = gm_Query(*(int *)(o + 0x5d), 1, 0, 0x13);
        if (l != 0)
            gm_Unk502a60(o, l, p);
        cm_SendExSellObjectI(*(int *)(o + 1), -1, 0x15, 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
        cm_SendExSellObjectI(*(int *)(o + 1), -1, 0x17, 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
        cm_SendExSellObjectI(*(int *)(o + 1), -1, 0x18, 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
        cm_SendExSellObjectI(*(int *)(o + 1), -1, 0x14, 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
    }
    o = gm_BuildQuery(1, 0, 0x20);
    if (o != 0) {
        *(short *)(o + 0x25) = (short)p;
        *(short *)(o + 0x27) = (short)p;
        i = 3;
        do {
            cm_SendMessage3d(o, 0, 10, 0);
            i--;
        } while (i != 0);
        o = gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x2a);
        if (o != 0) {
            cm_BeginSendGroup();
            cm_SendExSellObjectI(*(int *)(o + 2), -1, 0x1d0, 0x19, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
            cm_EndSendGroup();
            while (cm_SendGroupState() == 0)
                sv_PumpNet();
        }
    }
    o = gm_BuildQuery(1, 0, 0x2a);
    if (o != 0) {
        gm_AssignBuilding(o, p, p);
        o = gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x2a);
        if (o != 0) {
            cm_BeginSendGroup();
            cm_SendExSellObjectI(*(int *)(o + 2), -1, 0x1d0, 0x14, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
            cm_SendExSellObjectI(*(int *)(o + 2), -1, 0x1d7, 0xf, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
            cm_SendExSellObjectI(*(int *)(o + 2), -1, 0x1d6, 0xf, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
            cm_SendExSellObjectI(*(int *)(o + 2), -1, 0x158, 10, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
            cm_EndSendGroup();
            while (cm_SendGroupState() == 0)
                sv_PumpNet();
        }
    }
    o = gm_BuildQuery(1, 0, 0x1e);
    if (o != 0)
        gm_AssignBuilding(o, p, p);
    o = gm_BuildQuery(1, 0, 0x1f);
    if (o != 0)
        gm_AssignBuilding(o, p, p);
    o = gm_BuildQuery(1, 0, 0x12);
    if (o != 0)
        gm_AssignBuilding(o, p, p);
    o = gm_BuildQuery(1, 0, 0x14);
    if (o != 0) {
        gm_AssignBuilding(o, p, p);
        cm_SendExSellObjectI(*(int *)(o + 1), -1, 0xf7, 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
    }
    o = gm_BuildQuery(1, 0, 0x32);
    if (o != 0) {
        gm_AssignBuilding(o, p, p);
        o = gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x2a);
        if (o != 0) {
            cm_BeginSendGroup();
            cm_SendExSellObjectI(*(int *)(o + 2), -1, 0x1cb, 10, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
            cm_SendExSellObjectI(*(int *)(o + 2), -1, 0x1cc, 10, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
            cm_SendExSellObjectI(*(int *)(o + 2), -1, 0x1d0, 10, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
            cm_EndSendGroup();
            while (cm_SendGroupState() == 0)
                sv_PumpNet();
        }
    }
    o = gm_BuildQuery(1, 0, 0x21);
    if (o != 0) {
        gm_AssignBuilding(o, p, p);
        gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x2a);
    }
    o = gm_BuildQuery(1, 0, 0x19);
    if (o != 0) {
        gm_AssignBuilding(o, p, p);
        gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x2a);
    }
    o = gm_BuildQuery(1, 0, 0x34);
    if (o != 0) {
        gm_AssignBuilding(o, p, p);
        o = gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x2a);
        if (o != 0) {
            cm_BeginSendGroup();
            cm_SendExSellObjectI(*(int *)(o + 2), -1, 0x1cb, 10, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
            cm_SendExSellObjectI(*(int *)(o + 2), -1, 0x1cc, 10, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
            cm_SendExSellObjectI(*(int *)(o + 2), -1, 0x1d0, 10, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
            cm_EndSendGroup();
            while (cm_SendGroupState() == 0)
                sv_PumpNet();
        }
    }
    o = gm_BuildQuery(1, 0, 0x36);
    if (o != 0) {
        gm_AssignBuilding(o, p, p);
        gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x2a);
    }
}
