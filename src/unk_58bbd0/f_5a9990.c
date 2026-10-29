/* 0x5a9990, a graveyard message (69 B): 0x42ec00 (3) picks the text 0x40a9 + the value, the +4 id (1, 0x5fb,
   the key 0x62c364) */

void ai_Unk5a9990(int param_1)
{
    unsigned int v;

    v = util_Random(3);
    txt_Format((char *)(unsigned)&full_data_6f26c8, 0x40a8, (v & 0xffff) + 0x40a9);
    he_ShowNews(*(int *)(param_1 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 1, 0x5fb, (char *)(unsigned)&full_data_62c364);
}
