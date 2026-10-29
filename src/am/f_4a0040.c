/* 0x4a0040, execute callback 0x36: hands the request (3, s1->unk04, s1->unk10) of s1 (kind 0x15) to 0x4e9660 */

int ai_Unk4a0040(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8REQ r;

    if (s1->kind == 0x15) {
        r.kind = 3;
        r.a = s1->unk04;
        r.b = s1->unk10;
        amt_ExecutePriceMessage(p, &r);
        return 0x36;
    }
    return 0;
}
