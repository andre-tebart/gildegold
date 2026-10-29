/* 0x5a9910, a graveyard text (124 B): 0x5190a0 orders the word of param_1 and param_3 (0 / 2: param_3 first,
   1: the word first, other: no text) and 0x42ec00 (3) picks the text 0x409c + the value; the +4 id of param_2
   sends it (1, 0x5fb, the key 0x62c348) */

void ai_Unk5a9910(unsigned short *param_1, int param_2, unsigned int param_3)
{
    unsigned int u;
    unsigned int v;
    unsigned int r;
    int order;

    order = ls_GetLanguage();
    if (order == 0) {
        u = *param_1;
        v = param_3;
        param_3 = u;
    } else if (order == 1) {
        u = *param_1;
        v = u;
    } else if (order == 2) {
        u = *param_1;
        v = param_3;
        param_3 = u;
    } else {
        goto send;
    }
    r = ((int (*)(int, int, int, int))util_Random)(3, u, v, param_3);
    txt_Format((char *)(unsigned)&full_data_6f26c8, 0x409b, (r & 0xffff) + 0x409c);
send:
    he_ShowNews(*(int *)(param_2 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 1, 0x5fb, (char *)(unsigned)&full_data_62c348);
}
