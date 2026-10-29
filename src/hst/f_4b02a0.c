/* 0x4b02a0, the VERMOEGEN_STEUER text command (callback 0x601b50[6], name 0x601070[6]) "-n": every character
   passing ai_IsLivingCharacter pays n percent of his wealth (0x50a640) into the stock (object 0x115) of building
   0x50b010(1)
   @deep B: 31 code hunks / 80 instructions, all shape, meaning read and equal (bld = gm_FirstBuilding(1), the stock object 0x115 of gm_Query, the '-n' guard, b = atoi, f = b*0.01f, the 0..0x300 loop over ai_IsLivingCharacter with gm_GetWorth and cm_SendMoveGoodsUnchecked(o->objId, netId, 0, (int)(b*f))). The first hunk is the frame: the original opens with push ecx (a 4-byte local before the loop, popped at every return), ours with push ebx only, so every slot after it is shifted by 4 (4b02cf / 4b02e6 code hunks are the same statements at [esp+0x10]/[esp+0x14] vs [esp+0x14]/[esp+0x18]) and 4b0319 is the same rotation (slots). Tried this session: nothing source-level yet - the float f and int b are the only locals; the 4-byte push-ecx slot is a VC6 allocation choice (vargen / slot building), not a meaning error. Q2 blocked: gm_FirstBuilding's canonical definition returns unsigned char * while every caller uses AIBLD * (type debt in f3gm.c, as in ai_CommandCityTreasury's reason) - shared-header decision. Q1 has evidence (callback 0x601b50[6], name 0x601070[6] VERMOEGEN_STEUER), Q3/Q4/Q5/Q6/Q7 pass. */

int ai_CommandWealthTax(int a, int b, char *fmt)
{
    AIBLD *bld;
    AIOBJ *o;
    float f;
    int i;

    bld = gm_FirstBuilding(1);
    if (bld == 0)
        return 0;
    o = gm_Query(bld->objList, 2, 6, 0, 0x115);
    if (o == 0)
        return 0;
    if (*fmt != '-')
        return 0;
    b = atoi(fmt + 1);
    f = b * 0.01f;
    for (i = 0; i < 0x300; i++) {
        if (ai_IsLivingCharacter((unsigned short)i)) {
            b = gm_GetWorth(ai_Chars[i].id);
            cm_SendMoveGoodsUnchecked(o->objId, ai_Chars[i].netId, 0, (int)(b * f));
        }
    }
    return 1;
}
