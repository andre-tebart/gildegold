/* 0x5a9880, a graveyard message with three values in two orders (139 B): as 0x5a97e0 with the text 0x4150 */

void ai_Unk5a9880(int param_1, unsigned short *param_2, int param_3, int param_4)
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
    txt_Format((char *)(unsigned)&full_data_6f26c8, 0x4150, v, a, b);
send:
    he_ShowNews(*(int *)(param_1 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x5fc, (char *)(unsigned)&ch_EmptyName);
}
