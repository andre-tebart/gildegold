/* 0x49cdd0, eval callback 0x27: decision 0x13 decides the rating */

int ai_Unk49cdd0(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIDEC d;

    *a = 0.0f;
    *b = 0.0f;
    gv_GetTableRecord(0x13, (AIPOS *)&d);
    if (d.cur != 0)
        return ai_Unk4a4c10(a, b, i, (AIPER *)p, &d);
    return ai_Unk4a49a0(a, b, i, (AIPER *)p, f, o1, o2);
}
