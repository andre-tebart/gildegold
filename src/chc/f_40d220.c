/* 0x40d220, FindInCharacterName(ch, s): 1 when s occurs in the character's name, else 0 (also 0 after an error) */

int chc_FindInCharacterName(CHARACTER **ch, char **s)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "FindInCharacterName(): invalid character");
        return 0;
    }
    return strstr((*ch)->name, *s) != 0;
}
