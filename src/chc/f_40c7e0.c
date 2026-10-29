/* 0x40c7e0, SitDown(ch, name): sits the character down with the named animation (ch_SitDown); 1 after an error, else 0 */

int chc_SitDown(CHARACTER **ch, char **name)
{
    if (evt_CurCall && evt_CurCall->fn == chc_SitDown) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_SitDown;
            return 0;
        }
        return 0;
    }
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "SitDown(): Invalid character");
        return 1;
    }
    ch_SitDown(*ch, *name);
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_SitDown;
    return 0;
}
