/* 0x40c0d0, PlayAnimationScript(ch, name, n, frame, script): queues the animation name n times and lets
   the action run script at that frame (chc_AniScriptCB, group -1 = none); 1 after an error, else 0 */

int chc_PlayAnimationScript(CHARACTER **ch, char **name, int *n, int *frame, char **script)
{
    CHACTION *a;

    if (evt_CurCall && evt_CurCall->fn == chc_PlayAnimationScript) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_PlayAnimationScript;
            return 0;
        }
        return 0;
    }
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "PlayCharacterAniScript(): invalid character");
        return 1;
    }
    a = ch_AddActionCB(*ch, (CHACTFUNC)chc_AniScriptCB, *frame, 0x2e, *n);
    if (a) {
        if (strlen(*name) >= 0x5f) {
            evt_Error(evt_Cur, evt_Cur->pc, "PlayCharacterAniScript(): script name too long");
            return 1;
        }
        strncpy(a->u.name, *name, 0x3f);
        strncpy(a->scriptName, *script, 0x5f);
        a->scriptAction.group = evt_NoGroup;
    }
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_PlayAnimationScript;
    return 0;
}
