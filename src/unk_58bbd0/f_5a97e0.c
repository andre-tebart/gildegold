/* 0x5a97e0, a graveyard message with three values in two orders (159 B): as 0x5a96e0 with the text 0x414f
   (the table 0x5eecd0 + param_4 * 4), the +4 id (0x5fc) */

void ai_Unk5a97e0(int param_1, unsigned short *param_2, int param_3, int param_4, int param_5, int param_6)
{
    unsigned short v;
    int a;
    int b;
    int r;

    if (*(char *)(param_1 + 2) != 6 && *(char *)(param_1 + 2) != 7)
        return;
    r = ls_GetLanguage();
    if (r == 0) {
        v = *param_2;
        a = param_3;
        b = *(int *)(((unsigned)&ai_Unk5eeab8 + 0x218) + param_4 * 4);
    } else if (r == 1) {
        v = *param_2;
        a = *(int *)(((unsigned)&ai_Unk5eeab8 + 0x218) + param_4 * 4);
        b = param_3;
    } else if (r == 2) {
        v = *param_2;
        a = param_3;
        b = *(int *)(((unsigned)&ai_Unk5eeab8 + 0x218) + param_4 * 4);
    } else {
        goto send;
    }
    txt_Format((char *)(unsigned)&full_data_6f26c8, 0x414f, v, a, b, param_5, param_6);
send:
    he_ShowNews(*(int *)(param_1 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x5fc, (char *)(unsigned)&ch_EmptyName);
}
