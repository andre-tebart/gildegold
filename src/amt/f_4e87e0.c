/* 0x4e87e0, amt_SendOfficeNews with the text 0x1c21 (no second text): the character p lost the office v (the election replaces it, or it no longer
   fits the kind, 0x4e9260); nothing when p is 0 */

void amt_SendOfficeLost(unsigned short *p, int v)
{
    char buf[512];
    int *q;
    char k;

    if (p != 0 && ((k = ((char *)p)[2]) == 6 || k == 7 || k == 5)) {
        txt_Format(buf, 0x1c21, *p, (((char *)p)[9] != 0 ? 0x27b : 0x253) + v);
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
