/* 0x567810, "he_OpenHelpEvents": reads the tutorial hint script "<name>\project\game\tutorial\<param>.ini" (0x627e08 with 0x624690: "%s\tutorial\%s.ini") into the table 0x6fa260 (128 entries of four ints: due day, due
   time, due clock tail, text id): per line (lines containing "//" are comments) the ':' separated fields are the text label (0x42e770; an unknown label: the message "he_OpenHelpEvents(): invalid textlabel:%s"), and the three
   numbers; an unopenable file gives the message box "he_OpenHelpEvents(): Could not open help-definition!" (ROUGH) */

void he_Unk567810(char *param)
{
    char sep[2] = ":";
    char path[260];
    char line[0x100], label[0x100], msg[0x400];
    int *e;
    FILE *f;
    char *t;
    int k, i;
    char c;

    sprintf(path, "%s\\tutorial\\%s.ini", "\\project\\game\\", param);
    for (e = (int *)(unsigned)&full_data_6fa260, i = 0; i < 0x200; i++)
        e[i] = 0;
    f = fopen(path, "rt");
    if (f == 0) {
        MessageBoxA(0, "he_OpenHelpEvents(): Could not open help-definition!", "Error", 0x30);
        return;
    }
    e = (int *)(unsigned)&full_data_6fa260;
    for (;;) {
        memset(line, 0, sizeof(line));
        fgets(line, 0x100, f);
        if (strstr(line, "//") == 0) {
            k = 0;
            for (t = strtok(line, sep); t != 0; t = strtok(0, sep)) {
                if (k == 0) {
                    i = 0;
                    do {
                        c = t[i];
                        label[i] = c;
                        i++;
                    } while (c != 0);
                    e[3] = txt_FindKey(label);
                    if (e[3] == -1)
                        sprintf(msg, "he_OpenHelpEvents(): invalid textlabel:%s", label);
                } else if (k == 1) {
                    e[0] = atoi(t);
                } else if (k == 2) {
                    e[1] = atoi(t);
                } else if (k == 3) {
                    e[2] = atoi(t);
                }
                k++;
            }
            e += 4;
            if ((int)e > ((unsigned)&full_data_6fa260 + 0x800))
                return;
        }
        if (feof(f)) {
            fclose(f);
            return;
        }
    }
}
