/* 0x5210a0, sv_CheckStock: checks that the stock list holds at least n of the good record p (its prototype word; 0x508660) and shows a message box with the text by the prototype (0x142 / 0x115, the two money seats, pick 0x76 / 0x77, else 0x75); flag 4 auto-closes the box. 1 when the stock covers n, 0 after the box */

int sv_CheckStock(short *p, int stock, int n, unsigned t)
{
    int v;
    char buf[0x200];

    v = gm_SumSlotAmounts(stock, t);
    if (v >= n || n == 0)
        return 1;
    if (p != 0) {
        if (*p == 0x142) {
            txt_Format(buf, 0x76, t & 0xff);
            goto show;
        }
        if (*p == 0x115) {
            txt_Format(buf, 0x77, t & 0xff);
            goto show;
        }
    }
    txt_Format(buf, 0x75, t & 0xff);
show:
    sv_MessageBox(buf, 4);
    return 0;
}
