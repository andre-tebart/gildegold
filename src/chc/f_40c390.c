/* 0x40c390, TakeObject(ch, name, frame, obj): makes the character take the object with the named
   animation at frame (ch_TakeObject); 1 after an error, else 0 */

int chc_TakeObject(CHARACTER **ch, char **name, int *frame, D3OBJECT **obj)
{
    if (evt_CurCall && evt_CurCall->fn == chc_TakeObject) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_TakeObject;
            return 0;
        }
        return 0;
    }
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "TakeObject(): Invalid character");
        return 1;
    }
    if (!*obj) {
        evt_Error(evt_Cur, evt_Cur->pc, "TakeObject(): Invalid dummy");
        return 1;
    }
    ch_TakeObject(*ch, *name, *obj, *frame);
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_TakeObject;
    return 0;
}
