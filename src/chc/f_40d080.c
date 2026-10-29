/* 0x40d080, SetLowPoly(ch, on): shows the low-poly body of the character instead of the normal one; 1 (also after an error) */

int chc_SetLowPoly(CHARACTER **ch, int *on)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetLowPoly(): invalid character or dummy");
        return 1;
    }
    if (*on)
        (*ch)->flags.lowPoly = 1;
    else
        (*ch)->flags.lowPoly = 0;
    return 1;
}
