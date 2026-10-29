/* 0x40bbc0, waits until the character has walked there */

int chc_WalkToDummy(CHARACTER **ch, D3OBJECT **dummy)
{
    if (evt_CurCall && evt_CurCall->fn == chc_WalkToDummy) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_WalkToDummy;
            return 0;
        }
        return 0;
    }
    if (*dummy && *ch) {
        if (!mem_IsValid(*dummy) || !mem_IsValid(*ch))
            return 0;
        ch_WalkToObject(*ch, *dummy);
        if (evt_Cur->mode == 1)
            evt_Cur->waitfn = chc_WalkToDummy;
        return 1;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "WalkToDummy(): invalid character or dummy");
    return 0;
}
