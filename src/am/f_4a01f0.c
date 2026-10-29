/* 0x4a01f0, execute callback 0x37: hands the request (2, s1->unk04, s1->unk10) of s1 (kind 4) to 0x4e9a00 */

int ai_Unk4a01f0(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8REQ r;

    if (s1->kind == 4) {
        r.kind = 2;
        r.a = s1->unk04;
        r.b = s1->unk10;
        amt_ExecuteShareMessage(p, &r);
        return 0x37;
    }
    return 0;
}
