/* 0x40cfd0, CharacterSitting(ch): 1 when the character's sit flag (flags byte 0x10) is set, else 0 (also 0 after an error) */

int chc_CharacterSitting(CHARACTER **ch)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "CharacterSitting(): invalid character");
        return 0;
    }
    if ((*ch)->flags.bytes.lo & 0x10)
        return 1;
    return 0;
}
