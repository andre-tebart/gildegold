/* 0x54d1e0, a Ghidra miss: looks the figure of a record up in a table of 16 entries of 12 bytes (first word: a building of 0x501190): finds the entry whose building id (+1) is the figure's job id (+0x184 + 0x2c) and that has an owner (word +0x27 != 0xffff); stores its index in *out and returns 1, else 0 */

int ai_CanAct();
void cm_SendExSellObject(int a, int b, short c, int d, unsigned char e, int f);

int he_Unk54d1e0(int rec, int *tab, int *out)
{
    int i;
    int *e;

    if (*(unsigned short *)rec == 0xffff || *(unsigned char *)(rec + 8) == 0 || *(unsigned char *)(rec + 2) != 0)
        return 0;
    if (*(int *)(rec + 0x184) == 0 || ai_CanAct(rec) == 0)
        return 0;
    e = tab;
    for (i = 0; i < 16; i++) {
        if (*e == 0)
            return 0;
        if (*(int *)(*e + 1) == *(int *)(*(int *)(rec + 0x184) + 0x2c) && *(unsigned short *)(*e + 0x27) != 0xffff) {
            *out = i;
            return 1;
        }
        e += 3;
    }
    return 0;
}
