/* 0x46d040, draws a picture (or a frame of one) at x, y on a surface @deep draft (25 differing instructions of 159; 13 code + 12 regs + 4 order hunks): the code hunks are the x/y register swap read in the previous base-3 session (0x46d157: ours mov esi,[esp+0x18] + mov edi,[esp+0x14], original esi=[esp+0x14] + edi=[esp+0x18], and the movsx/push at 0x46d17a/0x46d18c follow the swap); no meaning error, the source passes (short)x, (short)y, surf, data in that order. spell.py: reload at 0x46d157 (drop the local / use the parameter itself) is already the source form - x and y are the parameters, re-read at the call, no local holds them - and the regs/code entries name no source form. Q2 fails only on d3io_NoopCallback, an empty no-op triangle filler with no evidence at all (assigned to d3_TriFuncs[0..5] with different signatures, @deep in d3_io_a.c/d3_engine.c), so no name can be given here. Left: the x/y allocation and the tail at 0x46d208 (listing/c2prio/siblings) plus that name. */

void d2_DrawPic(x, y, pic, surf)
int x;
int y;
int pic;
void *surf;
{
    int sel, type, t;
    void *data = 0;

    if (pic > d2_NPics)
        return;
    sel = -1;
    t = d2_Pics[pic].type;
    if ((t == 5 || t == 8) && d2_Pics[pic].offset == 0) {
        if (d2_Pics[d2_Pics[pic].base].data == 0)
            data = d2_GetPic(pic);
        sel = pic;
        pic = d2_Pics[pic].base;
    } else {
        if (d2_Pics[pic].data == 0)
            data = d2_GetPic(pic);
    }
    if (d2_Pics[pic].flags & 2)
        pic += d2_PicBase;
    if (data == 0)
        data = d2_Pics[pic].data;
    if (sel == -1)
        type = d2_Pics[pic].type;
    else
        type = d2_Pics[sel].type;
    if (d2_LockSurface(surf) == 0)
        return;
#ifdef REA_FRAME_DUMP
    REA_DUMP_TRACE((rd_f, "%u DrawPic %d type=%d at %d,%d %dx%d sel=%d\n", GetTickCount(), pic, type, x, y,
                    d2_Pics[pic].w, d2_Pics[pic].h, sel));
#endif
    switch (type) {
    case 1:
    case 4:
        d2_DrawShape((short)x, (short)y, surf, data, 0);
        break;
    case 0x11:
        ((void (*)(int, int, void *, void *))d3io_NoopCallback)(x, y, surf, data);
        break;
    case 5:
    case 8:
        if (sel != -1)
            d2_DrawShape((short)x, (short)y, surf, data, (char)(sel - pic));
        else
            d2_DrawShape((short)x, (short)y, surf, data, 0);
        break;
    }
    d2_AddDirty((short)x, (short)y, d2_Pics[pic].w, d2_Pics[pic].h);
    d2_UnlockSurface(surf);
}
