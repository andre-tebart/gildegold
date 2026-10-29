/* 0x5a96e0, a message with three values in two orders (123 B): for a character of the kind 6 / 7 (+2) the
   text 0x414e with the word of param_2, param_3 and the text table 0x5eecd0 + param_4 * 4 - the order follows
   0x5190a0 (0: param_3 then the table, 1: the table then param_3, 2: as 0; other none), sent with the +4 id
   (0x5fc, the key 0x62c32c) */

void ai_Unk5a96e0(int param_1, unsigned short *param_2, int param_3, int param_4, int param_5, int param_6)
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
    txt_Format((char *)(unsigned)&full_data_6f26c8, 0x414e, v, a, b, param_5, param_6);
send:
    he_ShowNews(*(int *)(param_1 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x5fc, (char *)(unsigned)&full_data_62c32c);
}
