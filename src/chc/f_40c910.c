/* 0x40c910, GetUp(ch, name): stands the character up with the named animation (ch_StandUp); 1 after an error, else 0 */

int chc_GetUp(CHARACTER **ch, char **name)
{
    if (evt_CurCall && evt_CurCall->fn == chc_GetUp) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_GetUp;
            return 0;
        }
        return 0;
    }
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "GetUp(): Invalid character");
        return 1;
    }
    ch_StandUp(*ch, *name);
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_GetUp;
    return 0;
}
