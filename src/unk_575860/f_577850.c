/* 0x577850, -4 without a player / run record, else the run window's controls 1 and 2 (0x475f70) with the text
   0x602568 (0x515960) and 0 */

int ai_Unk577850(void)
{
    char *per;

    per = *(char **)(S + 0x1c);
    if (!per)
        return -4;
    if (!*(int *)(per + 0x74))
        return -4;
    d2_SetForm(*(int *)(*(char **)(per + 0x74) + 8), 1);
    io_PrintText((char *)(unsigned)"$C");
    d2_SetForm(R8, 2);
    io_PrintText((char *)(unsigned)"$C");
    return 0;
}
