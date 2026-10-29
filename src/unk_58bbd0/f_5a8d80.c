/* 0x5a8d80, two production messages (133 B, a Ghidra-missed start from 0x544070): the body runs for a kind
   (+2) 6 and for every kind except 7 - the original's redundant `== 6 || != 7`, needed for the bytes; the text
   0x20bf with the +4 id of the first character, then the text 0x20c0 with the word of the first record and the
   +4 id of the second (0x5fb) */

void ai_Unk5a8d80(int param_1, int param_2)
{
    if (*(char *)(param_1 + 2) == 6 || *(char *)(param_1 + 2) != 7) {
        txt_Format((char *)(unsigned)&full_data_6f26c8, 0x20bf);
        he_ShowNews(*(int *)(param_1 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x5fb, (char *)(unsigned)&ch_EmptyName);
    }
    if (*(char *)(param_2 + 2) == 6 || *(char *)(param_2 + 2) != 7) {
        txt_Format((char *)(unsigned)&full_data_6f26c8, 0x20c0, *(unsigned short *)param_1);
        he_ShowNews(*(int *)(param_2 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x5fb, (char *)(unsigned)&ch_EmptyName);
    }
}
