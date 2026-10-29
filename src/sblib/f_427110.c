/* 0x427110, starts sample s: loads the bank entry's data when it is missing, picks the SBDATA (a type-1
   sample, or a variation item - s->var when the fixed-variation flag is set, else a random one), maps fmt 1
   to ".wav" and 2 to ".mp3", hands data / len to sfx_SetSampleFile and sets volume / pan / loops / delay and
   the restart bookkeeping before sfx_StartSample; return 0, -1 when the bank, the load, the entry type or
   format or the driver handover fails
   @deep behaviour: 20 code hunk lines are one block-layout difference - the original keeps the failure epilogue inline after the sfx_SetSampleFile test (jne + or ebp,-1; pop; mov eax,ebp; ret at 0x4271b5, the four failure sites jump into it) with the success epilogue at the end, ours puts the single end: return ret block at the function end (mov eax,ebp; pop...; ret; or ebp,-1); every source shape (return -1 per site, ret = -1; goto end, end: return ret, if/else, 162 variants in notes/units/sblib.md) gives the shared end block, so the statements and operands agree and only VC6's block layout / ret register role differs (asmdiff, listing) */

int sb_Play(SBSAMPLE *s, int fade, int delay)
{
    int ret = 0;
    SBENTRY *e;
    SBBANK *bank;
    SBDATA *d;
    SBVAR *v;
    char *ext;
    int r;

    if (!sb_Driver)
        return 0;
    e = s->info;
    if (!e->data) {
        bank = sb_GetBankOf(e);
        if (!bank) {
            ret = -1;
            goto end;
        }
        ret = sb_LoadEntry(bank, e);
        if (ret)
            goto end;
    }
    if (s->info->type == 1)
        d = s->info->data;
    else if (s->info->type == 2) {
        v = s->info->data;
        if (s->flags & 4)
            r = s->var;
        else
            r = rand() % v->count;
        d = &v->items[r];
    } else {
        ret = -1;
        goto end;
    }
    if (d->fmt == 1)
        ext = ".wav";
    else if (d->fmt == 2)
        ext = ".mp3";
    else {
        ret = -1;
        goto end;
    }
    if (sfx_SetSampleFile(s->handle, ext, d->data, d->len) == -1) {
        ret = -1;
        goto end;
    }
    if (fade) {
        s->fadestart = SB_TIME;
        s->flags |= 1;
        sfx_SetSampleVolume(s->handle, 0);
    } else
        sfx_SetSampleVolume(s->handle, s->volume);
    sfx_SetSamplePan(s->handle, s->pan);
    if (!(s->flags & 4) && delay) {
        sfx_SetSampleLoopCount(s->handle, 1);
        s->last = 0;
        s->delay = delay;
    } else {
        sfx_SetSampleLoopCount(s->handle, s->loops);
        s->delay = 0;
    }
    s->info->used = s->start = SB_TIME;
    s->flags &= ~8;
    sfx_StartSample(s->handle);
end:
    return ret;
}
