/* 0x5089c0, the price of the goods id for the player slot p: the goods record's value (+0x38) or the base price */

float gm_GoodsPrice(unsigned id, unsigned char p)
{
    int i;
    unsigned short *r;

    i = 0;
    r = (unsigned short *)(init_StockBlocks + p * 0x2a90);
    do {
        if (*r == id)
            return *(float *)(init_StockBlocks + 0x38 + p * 0x2a90 + i * 0x80);
        i++;
        r += 0x40;
    } while (i < 0x55);
    return gm_ItemPrice(id, 100);
}
