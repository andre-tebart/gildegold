/* 0x49c170, decide callback 0x23 */

int ai_Unk49c170(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    if (o1->kind == 1 && f == 3)
        return 0x23;
    return 0;
}
