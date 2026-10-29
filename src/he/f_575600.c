/* 0x575600, writes 0x6f263c as "Game" / "message_filter_mask" into the INI file 0x6fa0e0 (WritePrivateProfileStringA) */

void he_SaveMessageFilter(void)
{
    char buf[0xc];

    sprintf(buf, "%d", U_I((unsigned)&full_data_6f263c));
    WritePrivateProfileStringA("Game", "message_filter_mask", buf, (char *)(unsigned)&full_data_6fa0e0);
}
