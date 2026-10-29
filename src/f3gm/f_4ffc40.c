/* 0x4ffc40, the goods record of the player p with the word at +0 equal to id, 0 when none */

char *gm_FindGoods(int p, unsigned id)
{
    int i;
    unsigned short *q;

    i = 0;
    q = (unsigned short *)(init_StockBlocks + p * 0x2a90);
    do {
        if (*q == id)
            return init_StockBlocks + i * 0x80 + p * 0x2a90;
        i++;
        q += 0x40;
    } while (i < 0x55);
    return 0;
}
