/* 0x40bd40, WalkToDummyRotateVerified(ch, dummy, dist): WalkToDummyRotate, but first fails with an
   error when the character chc_DummyBlocked finds within dist of the dummy is in the way */

int chc_WalkToDummyRotateVerified(CHARACTER **ch, D3OBJECT **dummy, int *dist)
{
    if (evt_CurCall && evt_CurCall->fn == chc_WalkToDummyRotateVerified) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_WalkToDummyRotateVerified;
            return 0;
        }
        return 0;
    }
    if (*dummy && *ch) {
        if (chc_DummyBlocked(dummy, dist)) {
            evt_Error(evt_Cur, evt_Cur->pc, "WalkToDummyRotateVerified(): dummy blocked");
            return 0;
        }
        ch_WalkToObjectTurn(*ch, *dummy);
        if (evt_Cur->mode == 1)
            evt_Cur->waitfn = chc_WalkToDummyRotateVerified;
        return 1;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "WalkToDummyRotateVerified(): invalid character or dummy");
    return 0;
}
