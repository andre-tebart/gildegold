/* 0x4a0940, execute callback 0x38: hands the request (s1->unk10, s1->unk04, 0) of s1 (kind 7 / 0x16) to
   0x4e9ce0 */

int ai_Unk4a0940(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8REQ r;

    if (s1->kind != 7 && s1->kind != 0x16)
        return 0;
    r.kind = s1->unk10;
    r.a = s1->unk04;
    r.b = 0;
    amt_ExecuteMayorPowerMessage(p, &r);
    return 0x38;
}
