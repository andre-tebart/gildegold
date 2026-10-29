/* 0x4e5af0, the debug dump of the ints +0x190 .. +0x1a4 of a person, then 0x509e60 (c) and 0x50a640 (its id) */

void ai_Unk4e5af0(char *c, char *buf)
{
    sprintf(buf, "%i\t%i\t%i\t%i\t%i\t%i\t%i\t%i\t", *(int *)(c + 0x190), *(int *)(c + 0x194), *(int *)(c + 0x198), *(int *)(c + 0x19c), *(int *)(c + 0x1a0), *(int *)(c + 0x1a4),
            gm_GetGoodsValue(c), ((int (*)(unsigned short))gm_GetWorth)(*(unsigned short *)c));
}
