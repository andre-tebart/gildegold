/* 0x5a8d00, a production message (64 B, a Ghidra-missed start, called from 0x544070): the text 0x20bd and
   0x53dda0 (the +4 id, 0x5ec) for a character of the kind 6 / 7 (+2) */

void ai_Unk5a8d00(int param_1)
{
    if (*(char *)(param_1 + 2) == 6 || *(char *)(param_1 + 2) == 7) {
        txt_Format((char *)(unsigned)&full_data_6f26c8, 0x20bd);
        he_ShowNews(*(int *)(param_1 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 0, 0x5ec, (char *)(unsigned)&ch_EmptyName);
    }
}
