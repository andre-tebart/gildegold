/* 0x5a8b80, a production message with two values (123 B): for a character of the kind 6 / 7 (+2) the text
   0x20b6 (0x20b7 without +0xc, else 0x20b8 / 0x20b9 by the clock 0x770ae0 > 0x74) into the buffer 0x6f26c8 and
   0x53dda0 (the +4 id, -1, the buffer, 0, 0x5ff, "0x631090") */

void ai_Unk5a8b80(unsigned short *param_1, char *param_2, int param_3, int param_4, int param_5)
{
    int id;

    if (*(char *)(param_2 + 2) == 6 || *(char *)(param_2 + 2) == 7) {
        if (*(char *)(param_2 + 0xc) == 0)
            id = 0x20b7;
        else
            id = (0x75 <= *(int *)((unsigned)&ai_Time)) + 0x20b8;
        txt_Format((char *)(unsigned)&full_data_6f26c8, 0x20b6, *param_1, param_3, param_5, param_4, id);
        he_ShowNews(*(int *)(param_2 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x5ff, (char *)(unsigned)&ch_EmptyName);
    }
}
