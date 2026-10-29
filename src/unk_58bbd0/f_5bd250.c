/* 0x5bd250, the picture and text block of a window record (192 B): the record fields +0 the window, +4 / +8 /
   +0xc the pages, +0x10 the entry index, +0x14 the record id. With the window set, the row record 0x4f2720
   (AIPOS: the picture count at +0xc, the text count at +0x18) fills the page +4 with the pictures 0x4731d0
   (0x526); with the entry below 5 the pages +8 / +0xc get the text "%s" (0x62a578, param_1[4] + 0x16b7) and
   0x52eb50 (0, 6, the character 0x770b80 + player * 0x21c, the entry, 0x4df) */

void ai_Unk5bd250(int *param_1)
{
    int i;
    char pos[0x24];

    if (*param_1 != -1) {
        if (gv_GetTableRecord(param_1[5], (int *)pos) != 0 &&
            *(int *)(pos + 0xc) != 0 && *(int *)(pos + 0x18) != 0) {
            d2_SetForm(*param_1, param_1[1]);
            for (i = 0; i < *(int *)(pos + 0xc); i++)
                d2_WinOpenPic(i * 0x10 + 5, 5, 0x526, U_I((unsigned)&d2_LastWindow));
        }
        if (param_1[4] < 5) {
            d2_SetForm(*param_1, param_1[2]);
            io_PrintText((char *)(unsigned)&full_data_62a578, param_1[4] + 0x16b7);
            d2_SetForm(*param_1, param_1[3]);
            sv_Unk52eb50(0, 6, (char *)((unsigned)&ai_Chars + U_W((unsigned)&ai_PlayerId) * 0x21c), param_1[4], 0x4df);
        }
    }
}
