/* 0x4e83b0, the column check of 0x4e8160 (amt_CheckOfficeColumns) for one column: bit 1 (occupied) is set when an office with two candidates has two
   living ones (health +8 > 0, +0x1b1 clear) whose x14 holds an office of the column, or when an application's applicant (appliedOffice +0x166 of the kind
   byte ai_Chars + 2) exists; bit 2 (decided) is set when that applicant is a character of the kinds 6 / 7 with +0x1af clear, or when an office of the
   column is held by one. Returns 1 when the column is decided (the council session 0x4d8e20 runs amt_ElectOffices when it is 0). The tail keeps the
   original's `if ((char)f & 2) return 1;` spelling: VC6 compiles it to movsx / and 2 / shr 1, `(f & 2) != 0` to shr / and (notes/vc6-matching.md) */

int amt_IsColumnDecided(int col)
{
    unsigned int f;
    AMTREC list[6];
    int n, j, k;
    char *a, *b, *ch;

    f = 0;
    n = amt_GetColumnOffices(col, 6, list);
    for (j = 0; j < n; j++) {
        if (list[j].state == 2) {
            a = gm_FindChar(list[j].x14);
            b = gm_FindChar(list[j].owner);
            if (a != 0 && ((AMTCHARVIEW *)a)->health > 0 && a[0x1b1] == 0 && b != 0 && ((AMTCHARVIEW *)b)->health > 0) {
                for (k = 0; k < n; k++) {
                    if (list[k].owner == list[j].x14) {
                        f |= 1;
                        break;
                    }
                }
            }
        } else if (list[j].state == 3 && list[j].cnt > 0) {
            ch = (char *)ai_Chars + 2;
            do {
                if (ch[0x166] == (char)list[j].kind) {
                    f |= 1;
                    if ((ch[0] == 6 || ch[0] == 7) && ch[0x1af] == 0) {
                        f |= 2;
                        break;
                    }
                }
                ch += 0x21c;
            } while ((int)ch < (int)((char *)ai_Chars + 2 + 768 * 0x21c));
        }
    }
    if ((f & 1) != 0 && n > 0) {
        j = 0;
        do {
            a = gm_FindChar(list[j].owner);
            if (a != 0 && (a[2] == 6 || a[2] == 7) && a[0x1b1] == 0) {
                f |= 2;
                break;
            }
            j++;
        } while (j < n);
    }
    if ((char)f & 2)
        return 1;
    return 0;
}
