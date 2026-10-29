/* 0x4e8730, sends the office news 0x1c19 about the office p (+0 the index as an unsigned short, +2 the kind 5 .. 7, +9 a flag) with the number
   v + 0x253, or v + 0x27b (0x28 more) when the flag is set, to the characters of the kinds 6 and 7 (the text channels 0x5fd / 0x5fe) */

void amt_SendOfficeNews(unsigned short *p, int v)
{
    char buf[512];
    int *q;
    char k;

    k = ((char *)p)[2];
    if (k == 6 || k == 7 || k == 5) {
        txt_Format(buf, 0x1c19, *p, 0x1c1b, (((char *)p)[9] != 0 ? 0x27b : 0x253) + v);
        q = ai_CharInts[0];
        do {
            if (((char *)q)[-2] == 6)
                he_ShowNews(*q, -1, buf, 0, 0x5fd, ch_EmptyName);
            else if (((char *)q)[-2] == 7)
                he_ShowNews(*q, -1, buf, 0, 0x5fe, ch_EmptyName);
            q += 0x87;
        } while ((int)q < (int)((char *)ai_CharInts + 768 * 0x21c));
    }
}
