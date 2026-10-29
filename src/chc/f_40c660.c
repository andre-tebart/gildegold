/* 0x40c660, DropObject(ch, name, frame, obj): drops the object with the named animation at frame (ch_DropObject); 1 after an error (also when the object is not in the scene), else 0 */

int chc_DropObject(CHARACTER **ch, char **name, int *frame, char **obj)
{
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "DropObject(): Invalid character");
        return 1;
    }
    if (*obj && !d3_FindObject(0, 0x40, (int)*obj, 0))
        return 1;
    if (evt_CurCall && evt_CurCall->fn == chc_DropObject) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_DropObject;
            return 0;
        }
        return 0;
    }
    ch_DropObject(*ch, *name, *obj, *frame);
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_DropObject;
    return 0;
}
