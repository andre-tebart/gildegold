/* 0x4e78f0, the creation ranges of the characters the office events create (0x4e7aa0) and set up (0x4e7b60), by the office level of
   the kind k (1 .. 9): the age range (s0 .. s1, the 4th argument of cm_SendCreateCharacter, 16 .. 32), the starting money range
   (i2 .. i3, the amount of the 0x4c4830 message, a round count of AMT_MONEY_UNIT), the range of the level field 0xd (i4 .. i5,
   3 .. 8) and the job group (i6 .. i7, the 2nd argument of gm_TitleNumber, 2 .. 6); 1 when the level has a row */

int amt_GetCreateRanges(int k, AMTRANGE *r)
{
    AMTKIND a;

    if (amt_GetKind(k, &a) != 0) {
        switch (a.level) {
        case 1:
        case 2:
            r->s0 = 0x10;
            r->s1 = 0x14;
            r->i2 = 10000 * AMT_MONEY_UNIT;
            r->i3 = 20000 * AMT_MONEY_UNIT;
            r->i4 = 3;
            r->i5 = 4;
            r->i6 = 2;
            r->i7 = 3;
            return 1;
        case 3:
        case 4:
            r->s0 = 0x14;
            r->s1 = 0x18;
            r->i2 = 20000 * AMT_MONEY_UNIT;
            r->i3 = 30000 * AMT_MONEY_UNIT;
            r->i4 = 4;
            r->i5 = 5;
            r->i6 = 3;
            r->i7 = 4;
            return 1;
        case 5:
            r->s0 = 0x18;
            r->s1 = 0x1c;
            r->i2 = 30000 * AMT_MONEY_UNIT;
            r->i3 = 40000 * AMT_MONEY_UNIT;
            r->i4 = 5;
            r->i5 = 6;
            r->i6 = 3;
            r->i7 = 4;
            return 1;
        case 6:
            r->s0 = 0x18;
            r->s1 = 0x1c;
            r->i2 = 35000 * AMT_MONEY_UNIT;
            r->i3 = 50000 * AMT_MONEY_UNIT;
            r->i4 = 6;
            r->i5 = 7;
            r->i6 = 3;
            r->i7 = 5;
            return 1;
        case 7:
        case 8:
        case 9:
            r->s0 = 0x1c;
            r->s1 = 0x20;
            r->i2 = 50000 * AMT_MONEY_UNIT;
            r->i3 = 75000 * AMT_MONEY_UNIT;
            r->i4 = 7;
            r->i5 = 8;
            r->i6 = 5;
            r->i7 = 6;
            return 1;
        }
    }
    return 0;
}
