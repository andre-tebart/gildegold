/* 0x40be00, WalkToDummyVerified(ch, dummy, dist): walks the character to the dummy, but first fails
   with an error when another character blocks the dummy within dist */

int chc_WalkToDummyVerified(CHARACTER **ch, D3OBJECT **dummy, int *dist)
{
    if (evt_CurCall && evt_CurCall->fn == chc_WalkToDummyVerified) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_WalkToDummyVerified;
            return 0;
        }
        return 0;
    }
    if (*dummy && *ch) {
        if (chc_DummyBlocked(dummy, dist)) {
            evt_Error(evt_Cur, evt_Cur->pc, "WalkToDummyVerified(): dummy blocked");
            return 0;
        }
        ch_WalkToObject(*ch, *dummy);
        if (evt_Cur->mode == 1)
            evt_Cur->waitfn = chc_WalkToDummyVerified;
        return 1;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "WalkToDummyVerified(): invalid character or dummy");
    return 0;
}
