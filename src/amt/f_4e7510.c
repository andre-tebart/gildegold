/* 0x4e7510, the character who holds the first office of the kind k (the AI decision 0x490e00 asks for the kind 0x11 holder); 0 when no record has the kind or the
   owner does not exist (gm_FindChar) */

char *amt_GetOfficeHolder(int k)
{
    int i;

    for (i = 0; i < 42; i++)
        if (amt_Table[i].kind == k)
            return gm_FindChar(amt_Table[i].owner);
    return 0;
}
