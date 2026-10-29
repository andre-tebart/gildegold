/* 0x40cba0, CharacterIdle(ch): 1 when the character has no action left, else 0 (also 0 after an error) */

int chc_CharacterIdle(CHARACTER **ch)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "CharacterIdle(): Invalid character");
        return 0;
    }
    return (*ch)->actions == 0;
}
