/* 0x520f90, checks that the object o has room for n slots (+0x194); otherwise a message box with the text 0x74 (n); 1 when ok */

int sv_CheckMoney(char *o, int n)
{
    char buf[0x80];

    if (((SVCHARREC *)o)->money < n && n != 0) {
        txt_Format(buf, 0x74, n);
        sv_MessageBox(buf, 0);
        return 0;
    }
    return 1;
}
