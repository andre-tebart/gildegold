/* 0x40bec0, PlayAnimation(ch, name, n): queues the animation name n times (ch_QueueAnim); 1 after an
   error when the character is invalid, 0 otherwise (the script waits while its actions run) */

int chc_PlayAnimation(CHARACTER **ch, char **name, int *n)
{
    if (evt_CurCall && evt_CurCall->fn == chc_PlayAnimation) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_PlayAnimation;
            return 0;
        }
        return 0;
    }
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "PlayCharacterAni(): invalid character or dummy");
        return 1;
    }
    ch_QueueAnim(*ch, *name, *n);
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_PlayAnimation;
    return 0;
}
