/* 0x40c870, SitDownAtOnce(ch, name): SitDown, but marks the action (flags & 1) so the animation starts at once */

int chc_SitDownAtOnce(CHARACTER **ch, char **name)
{
    CHACTION *a;

    if (evt_CurCall && evt_CurCall->fn == chc_SitDownAtOnce) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_SitDownAtOnce;
            return 0;
        }
        return 0;
    }
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "SitDown(): Invalid character");
        return 1;
    }
    a = ch_SitDown(*ch, *name);
    if (a)
        a->scriptAction.flags |= 1;
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_SitDownAtOnce;
    return 0;
}
