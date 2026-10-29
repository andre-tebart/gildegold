/* 0x4044a0, starts the animation <model>_<name> on the character @deep behaviour: one code hunk left at 404502 (34 instr), the 12-byte D3ANIMFLAGS init; same statements as the original. Original builds b[1] as (loop&1)<<3|0x10 (mov al,[loop]; and al,1; shl al,3; or al,0x10), ours factors it to ((loop&1)|2)<<3, and the original keeps a second dword0 zero store and pushes buf at the block start; the rest (40454c/404562/4045ca/4045ec/404608) is regs-class. Tried this session, one build each: two f.b[1]|= lines (kept, byte-identical to the session baseline), single 0x10|(loop&1)<<3, reversed order, + 0x10, * 8 (gives the original's and/shl/or order but re-schedules the block and raises the byte diff 198->257), extra f.words[0]=0 duplicate dword0 store (->255); union dword view vs ((int*)&f)[1] is byte-neutral and kept. spell 404502 dup-tail entries (both arms / pull after branch / swap polarity) do not describe it: the moved items are stores and the call's push, no duplicated tail. Needs vargen/register+slot scheduling variants. Readability done: union view removed the raw dword cast, CH_SLOT/CH_FRAME replaced by ch->slot/ch->lastActionFrame, no Q failure left. */

int ch_AttachAni(CHARACTER *ch, char *name, unsigned char loop)
{
    char msg[512];
    char file[272];
    char buf[272];
    union {
        D3ANIMFLAGS anim;
        int words[3];
    } f;
    D3ANIMSLOT *s;

    vfs_ToSlashes(name);
    sprintf(file, "character/%s/%s_%s.baf", CH_MODEL(ch), name, CH_MODEL(ch));
    sprintf(buf, "%s_%s", name, CH_MODEL(ch));
    vfs_ToSlashes(buf);
    if (*name) {
        memset(&f, 0, sizeof(f));
        f.anim.b[0] = 0;
        f.anim.b[1] |= 0x10;
        f.anim.b[1] |= (loop & 1) << 3;
        f.words[1] |= 3;
        f.anim.b[8] |= 2;
        if (!d3_FindAnim(buf)) {
            if (strcmp(name, "bewegung/gehen") && strcmp(name, "bewegung/karren_ziehen"))
                d3_LoadAnimFile(file, buf, 0);
            else
                d3_LoadAnimFile(file, buf, 1);
        }
        s = (D3ANIMSLOT *)d3_AttachAnimF(ch->obj->material->animCtl, buf, f.anim);
        if (!s) {
            sprintf(msg, "ch_AttachAni(): Could not attach ani: %s", name);
            return 0;
        }
        if (!d3t_SceneIndex(ch->scene))
            s->timeout = -1;
        else
            s->timeout = ch_MotionTimeout;
    }
    ch->slot = s;
    ch->lastActionFrame = -1;
    return (int)s;
}
