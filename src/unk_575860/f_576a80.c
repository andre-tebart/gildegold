/* 0x576a80, when `arg` is 1, finds a record (0x501190 (1, 5, 0xa)) and passes it to 0x521960 (r, -1, 0x631090);
   no return value */

void ai_Unk576a80(int arg)
{
    char *r;

    if (arg != 1)
        return;
    r = (char *)gm_BuildQuery(1, 5, 0xa);
    if (r)
        sv_SelectPanelObjectK(r, -1, (char *)(unsigned)&ch_EmptyName);
}
