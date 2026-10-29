/* 0x426630, the last bank in the list (where sb_LoadSampleBank appends), NULL when the list is empty,
   &sb_DummyBank when the driver is closed */

SBBANK *sb_LastBank(void)
{
    SBBANK *bank = sb_Banks;

    if (!sb_Driver)
        return &sb_DummyBank;
    if (bank)
        while (bank->next)
            bank = bank->next;
    return bank;
}
