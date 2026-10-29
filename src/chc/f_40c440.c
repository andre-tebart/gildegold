/* 0x40c440, TakeObjectLeft(ch, name, frame, obj): the same with the left hand (ch_TakeObject1) */

int chc_TakeObjectLeft(CHARACTER **ch, char **name, int *frame, D3OBJECT **obj)
{
    if (evt_CurCall && evt_CurCall->fn == chc_TakeObjectLeft) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_TakeObjectLeft;
            return 0;
        }
        return 0;
    }
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "TakeObjectLeft(): Invalid character");
        return 1;
    }
    if (!*obj) {
        evt_Error(evt_Cur, evt_Cur->pc, "TakeObjectLeft(): Invalid dummy");
        return 1;
    }
    ch_TakeObject1(*ch, *name, *obj, *frame);
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_TakeObjectLeft;
    return 0;
}
