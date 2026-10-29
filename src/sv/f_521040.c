/* 0x521040, checks that the stock of the character o (gm_SumSlotAmounts of its inventory +0x178 and the good t) covers n;
   otherwise a message box with the text 0x75 (t); 1 when ok */

int sv_CheckGoodAmount(char *o, int n, unsigned t)
{
    char buf[0x40];

    if (gm_SumSlotAmounts(((SVCHARREC *)o)->inventory, t) < n && n != 0) {
        txt_Format(buf, 0x75, t & 0xff);
        sv_MessageBox(buf, 0);
        return 0;
    }
    return 1;
}
