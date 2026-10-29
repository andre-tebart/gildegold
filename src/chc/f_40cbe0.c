/* 0x40cbe0, StopCharacter(ch): removes all actions of the character (ch_StopActions); 0, also after an error */

int chc_StopCharacter(CHARACTER **ch)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "StopCharacter(): Invalid character");
        return 0;
    }
    ch_StopActions(*ch);
    return 0;
}
