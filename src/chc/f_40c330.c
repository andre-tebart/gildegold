/* 0x40c330, GetCharacterHandle(name): returns the character with that name, or 0 after an error (the
   script then works with an empty handle) */

int chc_GetCharacterHandle(char **name)
{
    char buf[0x80];
    CHARACTER *ch = ch_FindByName(*name);

    if (!ch) {
        sprintf(buf, "GetCharacterHandle(): character '%s' not found", *name);
        evt_Error(evt_Cur, evt_Cur->pc, buf);
    }
    return (int)ch;
}
