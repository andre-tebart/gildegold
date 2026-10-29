/* 0x426450, the loaded bank that owns entry e; returns NULL when no bank does, &sb_DummyBank when the
   driver is closed */

SBBANK *sb_GetBankOf(SBENTRY *e)
{
    SBBANK *bank = sb_Banks;
    int i;

    if (!sb_Driver)
        return &sb_DummyBank;
    for (;;) {
        if (!bank)
            return bank;
        for (i = 0; i < bank->count; i++)
            if (&bank->entries[i] == e)
                return bank;
        bank = bank->next;
    }
}
