/* 0x5a8ee0, a production message from a character pair (162 B): for the kind 6 / 7 of the second character
   (+2) the city name 0x7e3f40 + city * 0x2f4 and its text 0x1336 (0x42ec00 (3, ...)), then the text 0x132f
   with the word of param_1 and the flag +9 of param_2 (+3), sent with the +4 id (1, 0x5ec) */

void ai_Unk5a8ee0(unsigned short *param_1, unsigned short *param_2)
{
    unsigned int v;

    if (*(char *)(param_2 + 1) == 6 || *(char *)(param_2 + 1) == 7) {
        v = ((int (*)(int, void *))util_Random)(3, (unsigned)&ai_PlayerSlots + *(unsigned char *)((unsigned)&ai_OwnPlayer) * 0x2f4);
        v = ((int (*)(int, void *, int))util_Random)(3, *param_2, (v & 0xffff) + 0x1336);
        txt_Format((char *)(unsigned)&full_data_6f26c8, 0x132f, *param_1,
                     (v & 0xffff) + (*(char *)((char *)param_2 + 9) != 0 ? 3 : 0) + 0x1330);
        he_ShowNews(*(int *)((char *)param_2 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 1, 0x5ec, 0);
    }
}
