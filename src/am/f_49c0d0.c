/* 0x49c0d0, eval callback for the goods actions 0x1c..0x22: decision 0x11 decides the rating */

int ai_Unk49c0d0(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIDEC d;

    *a = 0.0f;
    *b = 0.0f;
    if (o2->unk10 >= 0x1c && o2->unk10 <= 0x22) {
        gv_GetTableRecord(0x11, (AIPOS *)&d);
        if (d.cur != 0)
            return ai_Unk4a4c10(a, b, i, (AIPER *)p, &d);
        return ai_Unk4a49a0(a, b, i, (AIPER *)p, f, o1, o2);
    }
    return 0;
}
