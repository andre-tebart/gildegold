/* 0x4e8160, checks the six office columns 6 .. 1 (amt_GetColumnOffices): per column, bit 1 (occupied) is set when an office with two candidates has two living
   ones (health +8 > 0) or an application's applicant (appliedOffice +0x168) has the kind 6 / 7 and the flag +0x1b1 clear; bit 2 (decided) is set when an office
   of the column is held by a character of the kind 6 / 7 with +0x1b1 clear. A column that is occupied but not decided runs the election amt_ElectOffices
   (0x4e7c60) once; the function returns 1 when any column was decided. The character scan walks the records ai_Chars + 2 (the kind byte, 0x21c apart) up to
   ai_Chars + 2 + 768 * 0x21c. The remaining difference to the original is the register / block allocation (tools/asmdiff.py)
   @deep B3 draft: 26 code hunks / 4 regs, 353/608 bytes, 30 differing asm lines (156 hunks / 96 lines at session start), function 16 B longer than the original's 592. Meaning fixes this session (all measured): health +8 / +0x1b1 tests are unsigned ('health <= 0' -> jbe), the character scan starts at ai_Chars + 2 (offsets +0x166 / +0 / +0x1af, bound ai_Chars + 2 + 768*0x21c), fl zeroed by six ascending statements before the cols stores, the two 'fl[i] |= 1 / |= 2' stores split (was r | 3), the state-2 branch merged into if (a != 0 && health > 0) { if (b != 0 && health > 0) fl |= 1; else send(0,0,4); } else send(b,0,1);, the tail written as 'if (fl[i] & 2) ret = 1; else if ...' and the redundant '&& n > 0' guard dropped. Readability Q1-Q7 pass (amt_CheckOfficeColumns, the unit's own comment 'the check of 0x4e8160 for the column col'; 0x7d5f82 -> (char *)ai_Chars + 2 + 768 * 0x21c, byte-neutral here). spell.py: 4e8264 / 4e82a4 no menu rule; 4e83a7 dup-tail - polarity swap, both arms and pull-after-branch tried (the polarity form is in, the goto form of the state-2 branch measures 33 lines vs 30). Left: the original's extra 'test cl,cl; je' (its goto-shaped state-2 source), the dl vs cl health register and the wait-loop placement - allocation / scheduling. */

int amt_CheckOfficeColumns(void)
{
    unsigned int fl[6];
    int cols[6];
    AMTREC list[6];
    int ret, i, n, j, r;
    char *a, *b, *ch;

    fl[0] = 0;
    fl[1] = 0;
    fl[2] = 0;
    fl[3] = 0;
    fl[4] = 0;
    fl[5] = 0;
    cols[0] = 6;
    cols[1] = 5;
    cols[2] = 4;
    cols[3] = 3;
    cols[4] = 2;
    cols[5] = 1;
    ret = 0;
    for (i = 0; i < 6; i++) {
        n = amt_GetColumnOffices(cols[i], 6, list);
        for (j = 0; j < n; j++) {
            if (list[j].state == 2) {
                a = gm_FindChar(list[j].x14);
                b = gm_FindChar(list[j].owner);
                if (a != 0 && ((AMTCHARVIEW *)a)->health > 0) {
                    if (b != 0 && ((AMTCHARVIEW *)b)->health > 0)
                        fl[i] |= 1;
                    else {
                        r = amt_SendOfficeMessage((char)list[j].idx, 0, 0, 4, 0xff);
                        if (r != -1) {
                            while (cm_GetSlotState(r) == 0)
                                sv_PumpNet();
                        }
                    }
                } else {
                    r = amt_SendOfficeMessage((char)list[j].idx, b, 0, 1, 0xff);
                    if (r != -1) {
                        while (cm_GetSlotState(r) == 0)
                            sv_PumpNet();
                    }
                }
            } else if (list[j].state == 3 && list[j].cnt > 0) {
                ch = (char *)ai_Chars + 2;     /* the AICHAR kind byte */
                do {
                    if (ch[0x166] == (char)list[j].kind) {
                        fl[i] |= 1;
                        if ((ch[0] == 6 || ch[0] == 7) && ch[0x1af] == 0) {
                            fl[i] |= 2;
                            break;
                        }
                    }
                    ch += 0x21c;
                } while ((int)ch < (int)((char *)ai_Chars + 2 + 768 * 0x21c));
            }
        }
        if ((fl[i] & 1) != 0) {
            for (j = 0; j < n; j++) {
                a = gm_FindChar(list[j].owner);
                if (a != 0 && (a[2] == 6 || a[2] == 7) && a[0x1b1] == 0) {
                    fl[i] |= 2;
                    break;
                }
            }
        }
        if ((fl[i] & 2) != 0)
            ret = 1;
        else if ((fl[i] & 1) != 0 && ret == 0)
            amt_ElectOffices(cols[i], n, list);
    }
    return ret;
}
