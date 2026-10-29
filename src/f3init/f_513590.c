/* 0x513590, sets the detail level of the scene s (the flags in the byte at +0x71c * 4: 0x10 = high, 8 = middle, 4 = low) from the setting byte 0x6f35c8 + 2 and the frame limits, and tells the renderer (0x43f8a0 the size 0x80 / 0x40, 0x446dc0 the level); the flag f forces the update @deep behaviour + readable, meaning read whole against 0x513590..0x5136a2 and fixed: init_Detail is an unsigned char (the original stores mov BYTE ds:0x61adf8,4/2/1 - the draft's int gave a dword store; ls.c 0x51d2d0 also loads it as a byte, so its extern is unsigned char now), the size is d3fl_DetailScale << d3fl_DetailSizeShift with no & 0x1f (the original shl ebx,cl has no and), and the final test compares the old level computed once (oldlvl = (old>>2)&7) with ((new>>2)&7) as the original's cmp ebp,edx - the draft recomputed it and VC6 emitted an xor/test. Q2 via the d3fl_ aliases, Q7 via the canonical void d3fl_SetShift(D3FLMAP *, unsigned char, char) in cm.h/cm.c. Left: 4 hunks / 60 differing instructions, all register allocation: ours spills oldlvl to [esp+0x14] and keeps lvl in ebp, size in edi, a in ebx where the original keeps lvl=edi, oldlvl=ebp, size=ebx, a=edx (and the set byte in cl vs dl). Tried: declaration orders (lvl/a/size/oldlvl), oldlvl computed after size, oldlvl as unsigned char, the no-fl ((unsigned char *)s)[0x1c70] form - all identical or worse. */


void init_SetDetailLevel(int *s, char f)
{
    unsigned char old;
    int a, lvl, size, oldlvl;
    unsigned char *fl;
    unsigned char set;

    a = 0;
    lvl = 0;
    if (s == 0)
        return;
    fl = (unsigned char *)(s + 0x71c);
    old = *fl;
    oldlvl = (old >> 2) & 7;
    set = ((unsigned char *)&ai_GfxFloor)[2];
    size = d3fl_DetailScale << d3fl_DetailSizeShift;
    if (d3_CurrentScene == 0) {
        if (*s != 0x40 && ai_MapSize < 0x780) {
            if (set == 0) {
                lvl = 4;
                a = 0x80;
                *fl = (old & 0xf3) | 0x10;
                init_Detail = 4;
                goto upd;
            }
            if (set == 1) {
                *fl = (old & 0xeb) | 8;
                init_Detail = 2;
                goto mid;
            }
            if (set != 2)
                goto upd;
        }
        lvl = 1;
        *fl = (old & 0xe7) | 4;
        init_Detail = 1;
    } else if (set == 2) {
        lvl = 1;
        *fl = (old & 0xe7) | 4;
    } else {
        *fl = (old & 0xeb) | 8;
mid:
        lvl = 2;
    }
    a = 0x40;
upd:
    if (a != size || f != 0)
        d3fl_SetDetail(a);
    if (oldlvl != ((*fl >> 2) & 7) || f != 0)
        d3fl_SetShift((struct D3FLMAP *)s, lvl >> 1, 1);
}
