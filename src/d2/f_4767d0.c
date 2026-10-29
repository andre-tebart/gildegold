/* 0x4767d0, starts a fade of a sprite over a rectangle; NULL if all 0x20 slots are used @deep B3 close, readable: 83/144 bytes, 9 differing instructions, ONE code hunk left plus fill. The whole difference is the inner 'if (i >= 0x20) return 0' epilogue: the original keeps a second out-of-line xor eax,eax / pop edi / ret (jge to the shared end block not taken), ours jumps to the loop-exhausted return block (jge L65659). Reading verified: the do/while slot scan, the redundant i >= 0x20 check, mem_Alloc('d2_fadeinfo'), the fields x/y/w/h at 0x44..0x50, flags, dur 0x54, sprite = d3if_LoadSprite(name, 1), start/last = tb_Calls, the store into d2_Fades[i], return f. Gained this session: the struct-order assignments f->x, f->y, f->w, f->h (21 -> 9 differing instructions). Menu tried: the 'empty then / write the block inline' form if (i < 0x20) {body} -> 98 bytes / 11 instructions, reverted; 'write the block inline vs after the branch' is the same epilogue-placement decision. Remaining: VC6 block placement / tail-merge of the two return 0s -> deep (listing/c2prio) */

D2FADE *d2_StartFade(int x, int y, int w, int h, char *name, int dur, unsigned int flags)
{
    D2FADE *f;
    int i;

    i = 0;
    do {
        if (d2_Fades[i] == 0) {
            if (i >= 0x20)
                return 0;
            f = (D2FADE *)mem_Alloc(100, "d2_fadeinfo");
            f->x = x;
            f->y = y;
            f->w = w;
            f->h = h;
            f->flags = flags;
            f->dur = dur;
            f->sprite = (D2TEX *)d3if_LoadSprite(name, 1);
            f->start = tb_Calls;
            f->last = tb_Calls;
            d2_Fades[i] = f;
            return f;
        }
        i++;
    } while (i < 0x20);
    return 0;
}
