/* 0x5755c0, copies the INI path `s` to 0x6fa0e0 and loads "Game" / "message_filter_mask" from it (GetPrivateProfileIntA) into 0x6f263c */

void he_Unk5755c0(char *s)
{
    strcpy((char *)(unsigned)&full_data_6fa0e0, s);
    U_I((unsigned)&full_data_6f263c) = GetPrivateProfileIntA("Game", "message_filter_mask", 0, (char *)(unsigned)&full_data_6fa0e0);
}
