/* 0x49c3c0, decide callback 0x24 */

int ai_Unk49c3c0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    if (o1->kind == 1 && f == 4)
        return 0x24;
    return 0;
}
