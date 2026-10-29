/* 0x5a9470, the gypsy delivery text (536 B, ROUGH: the string copies are written as strcpy / strcat): with
   one to three characters param_3 (the 0x41bf header, the kinds 6 / 7 of the first param_2 records get the
   key 0x62c310), the text 0x144a8a8 is copied into 0x6f26c8 and the names (0x41bb, the 0x41bc part once only
   for two or more, the last text 0x144a8b8 / 0x144a8b4) are appended; sent by param_1 + 4 (1, 0x5fb, the key
   0x62c2f4) */

void ai_GypsyDeliveryText(unsigned short *param_1, int param_2, int *param_3)
{
    char local[128];
    int i;
    int done;

    if (param_2 <= 0 || param_2 >= 4)
        return;
    txt_Format((char *)(unsigned)&full_data_6f26c8, 0x41bf, *param_1);
    for (i = 0; i < param_2; i++) {
        if (*(char *)(param_3[i] + 2) == 6 || *(char *)(param_3[i] + 2) == 7)
            he_ShowNews(*(int *)(param_3[i] + 4), -1, (char *)(unsigned)&full_data_6f26c8, 1, 0x5fb, (char *)(unsigned)&full_data_62c310);
    }
    strcpy((char *)(unsigned)&full_data_6f26c8, (char *)*(int *)((unsigned)&full_data_144a8a8));
    if (param_2 < 2) {
        for (i = 0; i < 4; i++) {
            if (i > 0)
                break;
            txt_Format(local, 0x41bb, *(unsigned short *)param_3[i]);
            strcat((char *)(unsigned)&full_data_6f26c8, local);
        }
        strcat((char *)(unsigned)&full_data_6f26c8, (char *)*(int *)((unsigned)&full_data_144a8b4));
    } else {
        for (i = 0; i < 4; i++) {
            if (param_2 - 1 <= i)
                break;
            txt_Format(local, 0x41bb, *(unsigned short *)param_3[i]);
            strcat((char *)(unsigned)&full_data_6f26c8, local);
        }
        done = 0;
        while (i < 4 && !done) {
            txt_Format(local, 0x41bc, *(unsigned short *)param_3[i]);
            strcat((char *)(unsigned)&full_data_6f26c8, local);
            done = 1;
            i++;
        }
        strcat((char *)(unsigned)&full_data_6f26c8, (char *)*(int *)((unsigned)&full_data_144a8b8));
    }
    he_ShowNews(*(int *)((char *)param_1 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 1, 0x5fb, (char *)(unsigned)&full_data_62c2f4);
}
