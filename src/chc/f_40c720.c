/* 0x40c720, DropObjectLeft(ch, name, frame, obj): the same with the left hand (ch_DropObject1) */

int chc_DropObjectLeft(CHARACTER **ch, char **name, int *frame, char **obj)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "DropObjectLeft(): Invalid character");
        return 1;
    }
    if (*obj && !d3_FindObject(0, 0x40, (int)*obj, 0))
        return 1;
    if (evt_CurCall && evt_CurCall->fn == chc_DropObjectLeft) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_DropObjectLeft;
            return 0;
        }
        return 0;
    }
    ch_DropObject1(*ch, *name, *obj, *frame);
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_DropObjectLeft;
    return 0;
}
