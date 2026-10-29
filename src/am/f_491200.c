/* 0x491200, decision 2 */

int ai_Unk491200(AIPER *p, int a, int *out)
{
    AIDEC d;

    gv_GetTableRecord(2, &d);
    if (p->unk00c == 0 && d.cur == 1) {
        if (p->unk1d0 < 0.4f)
            *out = 2;
        else
            *out = 0;
        return 1;
    }
    if (p->unk00c == 1 && d.cur == 0) {
        if (p->unk1d0 < 0.4f) {
            *out = 2;
            return 1;
        }
        *out = 1;
        return 1;
    }
    if (d.cur == 2 && p->unk1d0 > 0.8f) {
        *out = p->unk00c != 0;
        return 1;
    }
    return 0;
}
