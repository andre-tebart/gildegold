/* 0x5a99e0, a graveyard message (69 B): the text 0x40ac with the value, the +4 id (1, 0x5fb, the key 0x62c380) */

void ai_Unk5a99e0(int param_1)
{
    unsigned int v;

    v = util_Random(3);
    txt_Format((char *)(unsigned)&full_data_6f26c8, 0x40ac, (v & 0xffff) + 0x40a9);
    he_ShowNews(*(int *)(param_1 + 4), -1, (char *)(unsigned)&full_data_6f26c8, 1, 0x5fb, (char *)(unsigned)&full_data_62c380);
}
