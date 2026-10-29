/* 0x4ffbf0, the 0x4ffa10 value of the goods record of the player p with the word at +0 equal to id (0x55 records of 0x80 bytes), -1 when there is none */

float gm_Unk4ffa10();                   /* 0x4ffa10 */
int gm_FreeList();                     /* 0x500140 */
void cm_SendGoodsDelta(char *p, unsigned char k);                     /* 0x4c5710 */

float gm_GoodsValue(int p, unsigned id)
{
    int i;
    unsigned short *q;

    i = 0;
    q = (unsigned short *)(init_StockBlocks + p * 0x2a90);
    do {
        if (*q == id)
            return gm_Unk4ffa10(p, i);
        i++;
        q += 0x40;
    } while (i < 0x55);
    return -1.0f;
}
