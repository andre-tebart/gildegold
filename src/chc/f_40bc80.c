/* 0x40bc80, WalkToDummyRotate(ch, dummy): walks the character to the dummy and turns it into the
   dummy's direction; 1 when the walk was queued, 0 while the command runs or the arguments are invalid */

int chc_WalkToDummyRotate(CHARACTER **ch, D3OBJECT **dummy)
{
    if (evt_CurCall && evt_CurCall->fn == chc_WalkToDummyRotate) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_WalkToDummyRotate;
            return 0;
        }
        return 0;
    }
    if (*dummy && *ch) {
        if (!mem_IsValid(*dummy) || !mem_IsValid(*ch))
            return 0;
        ch_WalkToObjectTurn(*ch, *dummy);
        if (evt_Cur->mode == 1)
            evt_Cur->waitfn = chc_WalkToDummyRotate;
        return 1;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "WalkToDummyRotate(): invalid character or dummy");
    return 0;
}
