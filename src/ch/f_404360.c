/* 0x404360, loads the low-poly animations name... of the model attached at unk22c @deep behaviour: only the 12-byte D3ANIMFLAGS init placement is left (code hunk at 36): the original schedules the init stores between the d3_FindAnim(buf) call and its r test, ours before the call, so the zero lives in eax vs the original's ecx and the two result tests are test vs cmp eax,ebx (all other instructions equal). Tried this session: init after the call (kept), before the call, split around it (memset+f.words[0] before, byte ops after) - VC6 hoists the whole block in every source position; memset vs three explicit dword stores; explicit ((int *)&f)[k] vs the kept union view (unchanged code). spell menu 4043d8 dup-tail prescriptions (both arms / pull after the branch / swap polarity) do not apply: the moved instruction is the call, no duplicated tail. Needs scheduling/allocation variants (try.py, slotmap). Readability done: union view removed the Q4 raw *(int *)&f access, no Q failure left. */

/* ---- 0x404360..0x405810 (worker ch2) ---- */

void ch_LoadLowAnims(CHARACTER *ch, int n, ...)
{
    char buf[256];
    char file[256];
    union {
        D3ANIMFLAGS anim;
        int words[3];
    } f;
    char *name;
    int i, r;
    va_list args;

    va_start(args, n);
    for (i = 0; i < n; i++) {
        name = va_arg(args, char *);
        if (*name) {
            sprintf(file, "lowpolycharacter/%s/%s_%s_LOW.baf", CH_MODEL2(ch), name, CH_MODEL2(ch));
            sprintf(buf, "%s_%s_LOW.baf", name, CH_MODEL2(ch));
            vfs_ToSlashes(buf);
            r = d3_FindAnim(buf);
            memset(&f, 0, sizeof(f));
            f.words[0] = 0;
            f.anim.b[1] = 8;
            f.words[1] &= ~3;
            f.anim.b[8] |= 2;
            f.anim.b[0] = 0;
            if (r || d3_LoadAnimFile(file, buf, 1)) {
                d3_AttachAnimF(ch->lowPolyBody->material->animCtl, buf, f.anim);
                d3_ReleaseTexture(ch->lowPolyBody->material->animCtl, buf);
            }
        }
    }
}
