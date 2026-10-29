/* 0x40ba10, CreateCharacter(name, x, y, z): creates the character at that position and gives it the
   group of the running script (it is shown only while that group is the current room); returns it, or 0
   after an error when the character could not be loaded. The script arguments arrive as pointers. */
int chc_CreateCharacter(char **name, float *x, float *y, float *z)
{
    D3VECTOR4 pos;
    CHARACTER *ch;

    pos.x = *x;
    pos.y = *y;
    pos.z = *z;
    ch = ch_Create(*name, (D3VECTOR *)&pos);
    if (!ch)
        evt_Error(evt_Cur, evt_Cur->pc, "CreateCharacter(): Could not load character");
    else
        ch->scriptGroup = evt_Cur->group;
    return (int)ch;
}
