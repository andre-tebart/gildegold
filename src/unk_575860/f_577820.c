/* 0x577820, -4 without a player / run record, else 0x53d800 (player) and 0 */

int ai_Unk577820(void)
{
    char *per;

    per = *(char **)(S + 0x1c);
    if (!per)
        return -4;
    if (!*(int *)(per + 0x74))
        return -4;
    he_Unk53d800(per);
    return 0;
}
