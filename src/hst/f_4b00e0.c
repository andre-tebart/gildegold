/* 0x4b00e0, the STADTKASSE text command (callback 0x601b50[5], name 0x601070[5]) "-PLUS_n" / "-MINUS_n":
   moves n percent of the count of the first goods of type 9 in the stock (object 0x115) of building 0x50b010(1)
   to or from that stock (0x4c4830)
   @deep B near: 184/184 instructions, was 96 code hunks (ours 416 vs 445 bytes). The big win this session: the guard as an early return 'if (*fmt != '-') return 0;' plus a block for the sign/name tables (the original puts its return-0 block inline at the top; the if(...) {body} return 0; spelling puts it after and shifted everything). Left, 3 hunks, 8 differing instructions: (1) slots at 0x4b01a6: the original keeps n (the atoi result) in the dead fmt parameter slot [esp+0x30] and v (g->unk0e) in sign's dead slot [esp+0x10], ours swaps them; tried int i,s,v,n; v declared first; v declared inside the 'if (n > 0)' block - all byte-identical to the current form; (2) code at 0x4b0238 and (3) order at 0x4b0269 are the same swap in the fpu pair (original fild n / fmul 0.01f / fimul v, ours fild v / fimul n / fmul 0.01f). Slot/fpu allocation -> vargen perm rounds. Q2 blocked: gm_FirstBuilding's canonical definition returns unsigned char * while every caller (am.c, cm.c, hst.c, f3gm.c) uses AIBLD * (the type debt noted in ai_TokenCityTreasury's @deep reason) - a shared-header/type decision; AIOBJ.unk0e (the stock count) and AIOBJ.unk14 (the object list gm_Query walks) have no role evidence in the sources. Q1 has evidence (callback 0x601b50[5], name 0x601070[5] STADTKASSE), Q3/Q5/Q6/Q7 pass. */

int ai_CommandCityTreasury(int a, int b, char *fmt)
{
    if (*fmt != '-')
        return 0;
    {
        int sign[2] = { -1, 1 };
        char name[2][6] = { "MINUS", "PLUS" };
        int i, s, n, v;
        AIBLD *bld;
        AIOBJ *o, *g;

        fmt++;
        for (i = 0; i < 2; i++) {
            if (memcmp(fmt, name[i], strlen(name[i])) == 0)
                break;
        }
        if (i >= 2)
            return 0;
        s = sign[i];
        fmt += strlen(name[i]);
        if (*fmt != '_')
            return 0;
        n = atoi(fmt + 1);
        if (n > 0) {
            bld = gm_FirstBuilding(1);
            if (bld == 0)
                return 0;
            o = gm_Query(bld->objList, 2, 6, 0, 0x115);
            if (o == 0)
                return 0;
            for (g = gm_Query(o->unk14, 1, 5); g != 0; g = gm_QueryNext()) {
                if (ai_GoodsT[g->id].type == 9)
                    break;
            }
            if (g == 0)
                return 1;
            v = g->unk0e;
            if (v > 0) {
                if (s == -1)
                    cm_SendMoveGoodsUnchecked(s, o->objId, 0, (int)(n * 0.01f * v));
                else if (s == 1)
                    cm_SendMoveGoodsUnchecked(o->objId, -1, 0, (int)(n * 0.01f * v));
            }
        }
        return 1;
    }
}
