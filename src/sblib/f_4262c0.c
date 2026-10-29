/* 0x4262c0, the first loaded bank whose file path contains name, NULL when there is none */

SBBANK *sb_FindBankFile(char *name)
{
    SBBANK *bank;

    for (bank = sb_Banks; bank && !strstr(bank->path, name); bank = bank->next)
        ;
    return bank;
}
