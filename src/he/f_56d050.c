/* 0x56d050, registers the event types of the construction / production group (called by 0x53d4b0): the alloc / run pairs of 3, 4, 5, 8, 9, 0xa, 0xb, 0xc, 0xd, 0xe, 0x4c, 0x4d and the type texts of 3, 4, 8, 9; 0 = ok, 1 = a registration failed */

int ai_Unk56d050(void)
{
    if (he_Unk53d600(3, (void *)he_Unk568ae0, (void *)he_Unk568be0) != 0)
        return 1;
    sv_Unk52b350(3, "Abbau", 1, 0x501, 0xac);
    if (he_Unk53d600(4, (void *)he_Unk5690c0, (void *)he_Unk569210) != 0)
        return 1;
    sv_Unk52b350(4, "Produktion", 1, 0x4ff, 0xac);
    if (he_Unk53d600(5, (void *)ai_Unk56f040, (void *)d3io_NoopCallback) != 0)
        return 1;
    if (he_Unk53d600(8, (void *)he_Unk56a100, (void *)he_Unk56a170) != 0)
        return 1;
    sv_Unk52b350(8, "Suchen", 1, 0x500, 0xac);
    if (he_Unk53d600(9, (void *)he_Unk56a360, (void *)he_Unk56a3d0) != 0)
        return 1;
    sv_Unk52b350(9, "Suchen", 1, 0x500, -1);
    if (he_Unk53d600(10, (void *)d3io_NoopCallback, (void *)he_Unk56a710) != 0)
        return 1;
    if (he_Unk53d600(0xb, (void *)he_Unk56a7b0, (void *)he_Unk56a960) != 0)
        return 1;
    if (he_Unk53d600(0xc, (void *)he_Unk56b380, (void *)he_Unk56b3f0) != 0)
        return 1;
    if (he_Unk53d600(0xd, (void *)he_Unk56b570, (void *)he_Unk56be30) != 0)
        return 1;
    if (he_Unk53d600(0xe, (void *)d3io_NoopCallback, (void *)d3io_NoopCallback) != 0)
        return 1;
    if (he_Unk53d600(0x4c, (void *)he_Unk56cdd0, (void *)he_Unk56ce40) != 0)
        return 1;
    if (he_Unk53d600(0x4d, (void *)ai_Unk56f040, (void *)d3io_NoopCallback) != 0)
        return 1;
    return 0;
}
