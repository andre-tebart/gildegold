/* 0x40cc20, KillCharacterAnimations(ch): stops the current action and removes all others (ch_RemoveAction); 0, also after an error */

int chc_KillCharacterAnimations(CHARACTER **ch)
{
    CHACTION *a;

    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "DeleteCharacterAnimations(): Invalid character");
        return 0;
    }
    if (a = (*ch)->actions) {
        a->stop = 1;
        while (a->next)
            ch_RemoveAction(a->next);
        return 0;
    }
    return 0;
}
