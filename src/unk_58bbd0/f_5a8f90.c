/* 0x5a8f90, a production message from a character pair (162 B): the text of the first record into the 512-byte
   buffer (0x42ec00 (3, the word)), then the text 0x1339 with the word of param_1 and the flag +9 of param_2
   (+3), sent with the +4 id (1, 0x5ec) */

void ai_Unk5a8f90(unsigned short *param_1, unsigned short *param_2)
{
    char local[512];
    unsigned int v;

    if (*(char *)(param_2 + 1) == 6 || *(char *)(param_2 + 1) == 7) {
        v = ((int (*)(int, void *))util_Random)(3, *param_1);
        txt_Format(local, (v & 0xffff) + 0x1340);
        v = ((int (*)(int, void *, int))util_Random)(3, *param_2, local);
        txt_Format((char *)(unsigned)&full_data_6f26c8, 0x1339, *param_1,
                     (v & 0xffff) + (*(char *)((char *)param_2 + 9) != 0 ? 3 : 0) + 0x133a);
        he_ShowNews(*(int *)((char *)param_2 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 1, 0x5ec, 0);
    }
}
