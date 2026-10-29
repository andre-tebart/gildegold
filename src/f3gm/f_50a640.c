/* 0x50a640, the worth of the character id: its objects (0x509e60) plus the buildings it owns (query kind 3 of the owner): 0x509b10 and 0x509f80 per building; -1 for a free slot */


int gm_GetWorth(unsigned short id)
{
    int v, a, b;
    unsigned char *bl;

    if (id > 0x2ff)
        return -1;
    if (*(short *)ai_Chars[id] != -1) {
        v = gm_GetGoodsValue(ai_Chars[id]);
        bl = gm_BuildQuery(1, 3, id);
        while (bl != 0) {
            a = gm_EquipmentValue(bl, (char)bl[0x5c]);
            b = gm_Unk509f80((char *)bl);
            v = v + a + b;
            bl = gm_BuildNext();
        }
        return v;
    }
    return -1;
}
