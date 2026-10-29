/* 0x4e7b60, initializes the created applicant c (kind 0x13) for the office kind k: rolls its money (the range i2 .. i3, sent
   with the move-goods message 0x10) and its level (the range i4 .. i5, the field delta 0xd) from the ranges of
   amt_GetCreateRanges, sets its kind (field 2 = 3) and the office it comes from (field 0x167 = k), sends the deltas and
   applies for the kind (amt_ApplyForOffice); 1 when done. The two-person event 0x552920 calls it with the character of each
   created handle */

int amt_InitApplicant(char *c, int k)
{
    AMTRANGE r;
    int a, b, three;
    unsigned short x;

    three = 3;
    if (((AMTCHARVIEW *)c)->kind != 0x13)
        return 0;
    if (amt_GetCreateRanges(k, &r) == 0)
        return 0;
    x = util_Random(r.i3 - r.i2 + 1);
    a = x + r.i2;
    cm_SendMoveGoodsUnchecked(((AMTCHARVIEW *)c)->netId, -1, ai_OwnPlayer, a);
    x = util_Random(r.i5 - r.i4 + 1);
    b = x + r.i4;
    cm_BeginFieldBlock(c, ((AMTCHARVIEW *)c)->netId);
    cm_AddFieldDelta(1, 1, 0xd, &b);
    cm_AddFieldDelta(1, 1, 2, &three);
    cm_AddFieldDelta(1, 1, 0x167, &k);
    cm_SendFieldDeltas();
    amt_ApplyForOffice(c, k);
    return 1;
}
