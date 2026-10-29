/* 0x4eaa00, announces the office of the kind k as free (0x4e6680, the kind 3) */

void amt_Unk4eaa00(unsigned int k)
{
    int i;

    i = 0;
    do {
        if (amt_Table[i].kind == k) {
            amt_SendOfficeMessage((char)amt_Table[i].idx, 0, 0, 3, 0xff);
            return;
        }
        i++;
    } while (i < 30);
}
