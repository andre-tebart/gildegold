/* 0x5774d0, when `arg` is 0 and the building 0x6f2308 is not already of the table byte 0x12: finds a record
   (0x501190 (1, 5, 0x12)), picks 0x4ffe90 (r+0x5d, 1, 0, 0x74) and passes it (0x521960 (r, q+2, 0x631090)) */

void ai_Unk5774d0(int arg)
{
    char *r;
    char *q;
    int kind;

    if (arg)
        return;
    if (U_I((unsigned)&ai_PlayerBuilding)) {
        kind = *(unsigned char *)U_I((unsigned)&ai_PlayerBuilding);
        if (*(unsigned char *)(*(char **)(unsigned)&ai_BTypes + kind * 0x24d) == 0x12)
            return;
    }
    r = (char *)gm_BuildQuery(1, 5, 0x12);
    if (!r)
        return;
    q = (char *)gm_Query(*(int *)(r + 0x5d), 1, 0, 0x74);
    if (!q)
        return;
    sv_SelectPanelObjectK(r, *(int *)(q + 2), (char *)(unsigned)&ch_EmptyName);
}
