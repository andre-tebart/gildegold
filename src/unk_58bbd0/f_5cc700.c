/* 0x5cc700, the espionage payment handler (827 B, ROUGH SKELETON: no Ghidra export; from the
   disassembly): param_1 is the message record (the AIMSG: +0 the id, +2 the type 6, +4 the handle, +0xc
   a byte, +0x84 a flag byte, +0x194 the amount). Types other than 6: the amount < 6 returns 0x20; the
   price (int)((float)0x50a640 (the id) * 0x5eb844) against the money 0x509e60 (the record) (too little
   -> 0x20); the payment 0x4c47e0 (-1, the handle, 0x6f1bc5, the price) and the close 0x4c5f60 (-6),
   then the walk over the character records 0x770b80 (the word -2 = 0xffff skips). Type 6: the window at
   0x5cc836. ROUGH SKELETON: the walk body and the window are not written. */

int ai_EspionagePayment2(int param_1)  /* @original: the callers push (c, m); the draft body reads its own byte offsets of the parameters */
{
    int price;
    int ret;

    if (*(char *)(param_1 + 2) != 6) {
        if (*(int *)(param_1 + 0x194) < 6)
            return 0x20;
        price = (int)((float)gm_GetWorth(*(unsigned short *)param_1) * (0.0599999987f));
        if (gm_GetGoodsValue(param_1) < price)
            return 0x20;
        cm_SendMoveGoods(0xffffffff, *(int *)(param_1 + 4), U_I((unsigned)&ai_OwnPlayer), price);
        cm_SendMoneyDelta(*(int *)(param_1 + 4), 0xfffffffa);
        /* ROUGH SKELETON: the walk over the character records 0x770b80 (the word -2 == 0xffff skips)
           and the return value are not written yet */
        return 0x10;
    }
    /* ROUGH SKELETON: type 6 opens the window at 0x5cc836 (the 0x5c1130 menu, the records and the pump
       0x5309a0 are not written yet) */
    ret = 0;
    return ret;
}
