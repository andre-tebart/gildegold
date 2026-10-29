/* 0x4f2fe0, the text of the table record i into out (0x5176d0 with the texts 0x13fe + 5 * (b0) / 0x13fd + ..): by the second byte of the record 0: 0 as is, 1 / 2 / 3 add 0x13dd / 0x13e6 / 0x13ec to the last field; 0 for another value or no record */

int gv_TableRecordText(char *out, int i)
{
    unsigned rec[9];
    int b;

    if (gv_GetTableRecord(i, (int *)rec) != 0) {
        b = (rec[0] & 0xff) * 5;
        switch ((rec[0] >> 8) & 0xff) {
        case 0:
            txt_Format(out, b + 0x13fe, b + 0x13fd, rec[8]);
            return 1;
        case 1:
            rec[8] = rec[8] + 0x13dd;
            break;
        case 2:
            rec[8] = rec[8] + 0x13e6;
            break;
        case 3:
            rec[8] = rec[8] + 0x13ec;
            break;
        default:
            return 0;
        }
        txt_Format(out, b + 0x13fe, b + 0x13fd, rec[8]);
        return 1;
    }
    return 0;
}
