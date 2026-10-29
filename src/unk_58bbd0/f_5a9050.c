/* 0x5a9050, a production message from a character pair (162 B): as 0x5a8f90 with the texts 0x134a / 0x1343 /
   0x1344 */

void ai_Unk5a9050(unsigned short *param_1, unsigned short *param_2)
{
    char local[512];
    unsigned int v;

    if (*(char *)(param_2 + 1) == 6 || *(char *)(param_2 + 1) == 7) {
        v = ((int (*)(int, void *))util_Random)(3, *param_1);
        txt_Format(local, (v & 0xffff) + 0x134a);
        v = ((int (*)(int, void *, int))util_Random)(3, *param_2, local);
        txt_Format((char *)(unsigned)&full_data_6f26c8, 0x1343, *param_1,
                     (v & 0xffff) + (*(char *)((char *)param_2 + 9) != 0 ? 3 : 0) + 0x1344);
        he_ShowNews(*(int *)((char *)param_2 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 1, 0x5ec, 0);
    }
}
