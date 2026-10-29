/* 0x4c51b0, "cm_RequestChrMoveToUniverse": message 0x30 with a dummy name (< 0x20 characters) */

void cm_RequestChrMoveToUniverse(int a, int b, int c, const char *s)
{
    G17MSG m;

    m.type = 0x30;
    G17_I(m, 0) = a;
    G17_I(m, 4) = b;
    G17_I(m, 8) = c;
    if (strlen(s) < 0x20)
        strcpy((char *)&m.data[0xc], s);
    else
        err_Message("cm_RequestChrMoveToUniverse(): dummyname is too long...");
    ai_Unk4c39c0(&m);
}
