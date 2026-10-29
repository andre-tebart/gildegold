/* 0x5d0e60, a message callback (177 B): param_2 a record (+0x10 set to 1), param_3 the message record
   (+2 the type, +4 the handle, +0 the word, the counters +0x80 .. +0x84). 0x505ca0 (5, pick) gives a
   shuffled 0 .. 4 order; the first index whose counter param_3[0x80 + i] is below 0xfc runs the trial
   step 0x5d08c0 (the record, the index) and is remembered. Type 6: 0x4c5870 (the handle) and the text
   0x5176d0 (0xfe8, the word at *(int *) param_3 * 2 + 0xd71, the index + 0x16b7) through 0x5cf820.
   Returns 1. */

int ai_Unk5d0e60(int param_1, int param_2, int param_3)
{
    int pick[5];
    char text[0x800];
    int i;
    int n;
    int found;

    (void)param_1;
    *(int *)(param_2 + 0x10) = 1;
    gm_Shuffled(5, pick);
    n = 0;
    found = 0;
    for (i = 0; i < 5 && n < 1; i++) {
        if (*(unsigned char *)(param_3 + pick[i] + 0x80) < 0xfc) {
            ai_Unk5d08c0(param_3, pick[i]);
            found = pick[i];
            n++;
        }
    }
    if (*(char *)(param_3 + 2) == 6) {
        ai_Unk4c5870(*(int *)(param_3 + 4));
        txt_Format(text, (char *)0xfe8,
                      *(unsigned short *)(*(int *)param_3) * 2 + 0xd71, found + 0x16b7);
        ai_Unk5cf820((int)text);
    }
    return 1;
}
