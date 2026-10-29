/* 0x49d050, eval callback 0x28: decision 3 decides the rating; none while f is set */

int ai_Unk49d050(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIDEC d;

    if (f != 0) {
        *a = -1e30f;
        *b = -1e30f;
        return 0;
    }
    *a = 0.0f;
    *b = 0.0f;
    gv_GetTableRecord(3, (AIPOS *)&d);
    if (d.cur != 0)
        return ai_Unk4a4c10(a, b, i, (AIPER *)p, &d);
    return ai_Unk4a49a0(a, b, i, (AIPER *)p, 0, o1, o2);
}
