/* 0x40bfc0, PlayAnimationSound(ch, name, n, frame, sound): queues the animation name n times and plays
   sound at that frame through the chc_AniSoundCB callback; 1 after an error when the character is
   invalid, 0 otherwise */

int chc_PlayAnimationSound(CHARACTER **ch, char **name, int *n, int *frame, char **sound)
{
    CHACTION *a;

    if (evt_CurCall && evt_CurCall->fn == chc_PlayAnimationSound) {
        if ((*ch)->actions) {
            evt_Cur->waitfn = chc_PlayAnimationSound;
            return 0;
        }
        return 0;
    }
    if (!*ch) {
        evt_Error(evt_Cur, evt_Cur->pc, "PlayCharacterAniSound(): invalid character");
        return 1;
    }
    a = ch_AddActionCB(*ch, (CHACTFUNC)chc_AniSoundCB, *frame, 0x2e, *n);
    if (a) {
        strcpy(a->u.name, *name);
        strcpy(CH_ACTOBJ(a)->name2, *sound);
    }
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = chc_PlayAnimationSound;
    return 0;
}
