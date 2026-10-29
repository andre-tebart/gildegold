/* 0x426290, the first loaded bank whose name matches (case-insensitive, at most 50 chars), NULL when
   there is none */

SBBANK *sb_FindBank(char *name)
{
    SBBANK *bank;

    for (bank = sb_Banks; bank && _strnicmp(bank->name, name, 50); bank = bank->next)
        ;
    return bank;
}
