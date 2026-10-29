/* 0x40c210, PlayAnimationScriptInt(ch, name, n, frame, script, arg): PlayAnimationScript with one int
   argument passed to the script (evt_RunScriptArgs) */

int chc_PlayAnimationScriptInt(CHARACTER **ch, char **name, int *n, int *frame, char **script, int *arg)
{
    CHACTION *a;

    if (evt_CurCall && evt_CurCall->fn == chc_PlayAnimationScriptInt) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_PlayAnimationScriptInt;
            return 0;
        }
        return 0;
    }
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "PlayCharacterAniScriptInt(): invalid character");
        return 1;
    }
    a = ch_AddActionCB(*ch, (CHACTFUNC)chc_AniScriptIntCB, *frame, 0x2e, *n);
    if (a) {
        if (strlen(*name) >= 0x5f) {
            err_Message("PlayCharacterAniScript(): script name too long");
            return 1;
        }
        strncpy(a->u.name, *name, 0x3f);
        strncpy(a->scriptName, *script, 0x5f);
        a->scriptAction.group = evt_NoGroup;
        a->scriptAction.arg = *arg;
    }
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_PlayAnimationScriptInt;
    return 0;
}
