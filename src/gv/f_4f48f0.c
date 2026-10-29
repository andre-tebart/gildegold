/* 0x4f48f0, the text of a character action a: 1 text 0x1538 (with 0.03 * the 0x50a640 value), 3 text 0x153e (4, the value), otherwise text 0x1535 + 3 * a */

void unk_Unk4f48f0(unsigned short *c, int a, char *out)
{
    int v;

    v = (int)(gm_GetWorth(*c) * 0.03f);
    switch (a) {
    case 1:
        txt_Format(out, 0x1538, v);
        return;
    case 3:
        txt_Format(out, 0x153e, 4, v);
        return;
    default:
        txt_Format(out, a * 3 + 0x1535);
        return;
    }
}
