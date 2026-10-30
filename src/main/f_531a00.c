/* 0x531a00, the version string: the build date literal "Oct  1 2003" is split at the blanks (strtok) into up to three tokens and the month token is looked up in the month table (Jan .. Dec); the
   index is not used afterwards. Writes "Europa 1400 - Gold Edition - 2.06" to out. The original reserves 0x164 bytes of locals, the month index is dead (date is a 0x100 byte buffer, reasoned from the frame) @deep body complete, frame 0x164 matched (date is char[0x100] + inline strcpy of the literal, sep char[2], months[12][4], tok[3][16] by the frame/offsets), prologue copies and the strcmp/sprintf tail identical (asmdiff: 43 equal ins at the head, 47+ at the tail). Left (13 ins, one hunk at 0x531abb): the token copy loop. The original hoists 'lea esi,[tok]' after the je of the first strtok result and copies with mov edx,esi; mov ecx,eax; add esi,0x10; mov al,[ecx]; inc ecx; mov [edx],al; inc edx; test al,al (src kept in ecx, p advanced before the copy); ours puts lea esi before the test and uses the 'sub edx,eax; mov cl,[eax]; mov [edx+eax],cl; inc eax' dst-src idiom. Tried: tok[n]/n++, char (*p)[0x10] / p++, char *p with p += 0x10 after or before the strcpy (via a q copy, which moved the add before the loop as in the original but not the idiom), p = tok placed before / after the first strtok */

void main_BuildVersionString(char *out)
{
    char sep[2] = " ";
    char months[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    char date[0x100];
    char tok[3][0x10];
    char *t;
    char *p, *q;
    int i;

    strcpy(date, "Oct  1 2003");
    t = strtok(date, sep);
    p = tok[0];
    while (t != 0) {
        q = p;
        p += 0x10;
        strcpy(q, t);
        t = strtok(0, sep);
    }
    for (i = 0; i < 12; i++) {
        if (strcmp(tok[0], months[i]) == 0)
            break;
    }
    sprintf(out, "Europa 1400 - Gold Edition - 2.06");
}
