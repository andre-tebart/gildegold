/* 0x40d1e0, CharacterSitsDown(ch): 1 when the character's sit flag (flags byte 0x10) is set, else 0 (also 0 after an error) */

int chc_CharacterSitsDown(CHARACTER **ch)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "CharacterSitsDown(): invalid character");
        return 0;
    }
    if ((*ch)->flags.bytes.lo & 0x10)
        return 1;
    return 0;
}
