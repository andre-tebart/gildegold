/* 0x4f1d50, counts the characters (kind 0x14 at +2, the object at +0x16c = o) of the graveyard o; *n = the count; 1 while it is below the building's two limit bytes (+0x231 + +0x232), -1 for another building or no record */

int unk_Unk4f1d50(unsigned char *o, int *n)
{
    char *b;
    unsigned char lim1, lim2;
    int cnt;
    char *c;
    int *p;

    b = ai_BTypes + o[0] + o[0] * 0x24c;
    if (b[0] != 0x1d)
        return -1;
    if (*(int *)(o + 0x65) == 0)
        return -1;
    lim1 = (unsigned char)b[0x232];
    lim2 = (unsigned char)b[0x231];
    cnt = 0;
    for (p = (int *)(ai_Chars[0] + 0x16c); (int)p < ((unsigned)((unsigned)ai_Chars + 0x65402) + 0x16a); p += 0x87) {
        c = (char *)p - 0x16c;
        if (*(short *)c != -1 && c[2] == 0x14 && (unsigned char *)*p == o)
            cnt++;
    }
    if (n != 0)
        *n = cnt;
    return cnt < (int)(lim1 + lim2);
}
