/* 0x531a00, the version string: the build date literal "Oct  1 2003" is split (0x5dfa45 = strtok, the separators 0x5f70c4) and the month looked up in the month table (Jan .. Dec), the result is unused;
   writes "Europa 1400 - Gold Edition - 2.06" to out; ROUGH (the month table and tokens are read from the disassembly) */

void main_Unk531a00(char *out)
{
    char months[12][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    char date[0x20];
    char tok[3][0x10];
    char *t;
    int i, n;

    strcpy(date, "Oct  1 2003");
    n = 0;
    t = strtok(date, " ");
    while (t != 0 && n < 3) {
        strcpy(tok[n], t);
        n++;
        t = strtok(0, " ");
    }
    for (i = 0; i < 12; i++) {
        if (strcmp(tok[0], months[i]) == 0)
            break;
    }
    sprintf(out, "Europa 1400 - Gold Edition - 2.06");
}
