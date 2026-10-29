/* 0x50f750, the opening of a new building b (gm_BuildingClass): its owner pays in the kind 0x136 good
   (the prototype kind 9, plus a 500 fee moved to its character through gm_PlayerFactor / cm_SendMoveGoods) or
   the 0x134 good (every other kind except 4 / 0x10 / 0x13); then the building's person is sent
   (cm_SendMessage3d, or a grave for the cemetery kind 0x1d) and - once the send slot is answered
   (cm_RequestChrMoveToUniverse) - placed at its "dummy_EINGANG" entrance object, except for the homes
   (gm_IsHomeType); finally the building is stocked (init_Betrieb)
   @deep B3 draft, 160/174 instructions, 16 differing: ONE code hunk left - the stack cleanup of the entrance message (ours merges add esp,0x10 with the init_Betrieb cleanup into add esp,0x14 after the call, the original restores 0x10 right after cm_RequestChrMoveToUniverse and adds 4 after init_Betrieb); the other 13 differing lines are the alignment fill. Everything else was read against 0x50f750..0x50f902 and fixed (33/432 vs 139/448 differing bytes): p[0x230] is an unsigned char (xor/mov dl), ai_Chars[b->owner][2] is indexed directly (the original folds 0x770b82 into the operand), and the typed views replaced the raw accesses. spell.py: 50f903 dup-tail (the only menu entry) - the two arms / pull-after / polarity prescriptions do not apply: the original HAS two tails (0x50f8d4 and the shared 0x50f8f7) and the else falls through into the shared one, which is exactly what the source writes; no gain from a restructure. Readability done: Q1 init_OpenBuilding, Q2 (cm_RequestChrMoveToUniverse, gv_SendGrave, gm_BuildingMainObject named project-wide), Q4 (INIT_BLD.id / owner, INIT_OBJ.objId, INIT_CHAR.netId), Q6. */


void init_OpenBuilding(INIT_BLD *b)
{
    unsigned char *p;
    char k;
    int h, r, v;
    INIT_CHAR *ch;
    INIT_OBJ *ent;

    gm_BuildingClass(b->type);
    p = (unsigned char *)ai_BTypes + (unsigned)b->type * 0x24d;
    k = *p;
    if (k == 9) {
        cm_SendExSellObject(b->id, -1, 0x136, 1, ai_OwnPlayer, 0);
        v = gm_PlayerFactor(500, ai_OwnPlayer);
        ch = (INIT_CHAR *)ai_Chars[b->owner];
        cm_SendMoveGoods(ch->netId, -1, ai_OwnPlayer, v);
    } else if (k != 4 && k != 0x10 && k != 0x13) {
        cm_SendExSellObject(b->id, -1, 0x134, 1, ai_OwnPlayer, 0);
    }
    k = *p;
    if (k == 9)
        goto stock;
    if (k == 4 || k == 0x10 || k == 0x13) {
        if (k == 0x1d)
            goto cemetery;
        if (ai_Chars[b->owner][2] != 6 && ai_Chars[b->owner][2] != 7)
            cm_SendMessage3d((char *)b, 0, p[0x230], 0);
        h = cm_SendMessage3d((char *)b, 0, p[0x230], 0);
    } else {
        if (k != 0x1d)
            h = cm_SendMessage3d((char *)b, 0, p[0x230], 0);
        else
cemetery:
            h = gv_SendGrave((unsigned char *)b);
    }
    r = cm_GetSlotState(h);
    while (r == 0) {
        sv_PumpNet();
        r = cm_GetSlotState(h);
    }
    ch = (INIT_CHAR *)cm_GetSlotValue(h);
    if (gm_IsHomeType((unsigned char *)b) == 0) {
        ent = (INIT_OBJ *)gm_BuildingMainObject((unsigned char *)b);
        if (ent != 0) {
            cm_RequestChrMoveToUniverse(ch->netId, b->id, ent->objId, "dummy_EINGANG");
            init_Betrieb(b);
            return;
        }
    } else {
        cm_RequestChrMoveToUniverse(ch->netId, b->id, -1, "dummy_EINGANG");
    }
stock:
    init_Betrieb(b);
}
