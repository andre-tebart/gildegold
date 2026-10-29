/* 0x4995d0, evaluate: decision record 0x18, the default rating without it */

int ai_Unk4995d0(float *a, float *b, int i, AIPER *p, int f, AIARG *o1, AIARG *o2)
{
    AIDEC d;

    *a = 0.0f;
    *b = 0.0f;
    gv_GetTableRecord(0x18, &d);
    if (d.cur && (p->kind != 5 || !(p->flags1c8 & 2)))
        return ai_Unk4a4c10(a, b, i, p, &d);
    return ai_Unk4a49a0(a, b, i, p, f, o1, o2);
}
