/* 0x472fd0, moves the objects of a window (and the window's parent run) to the top of the draw order list @deep B3: two code hunks (0x472fe9, 0x473027), reordered only - same operations both sides. Original hoists j=-1 out of the loop (or esi,-1 before it), keeps i in ecx and j in esi; ours keeps the mov ecx,-1 at the loop bottom and swaps i=esi / j=ecx, so the loop head and exit order differ. Read against tools/disasm.py 472fd0 and the VC6 listing: the loop, the break path (j=i) and the post-loop first=j are semantically right; the j=-1-at-bottom form is what the source needs. spell.py: no menu rule matched. Tried (each one edit + build, reverted): move j=i into the break block and drop j=-1 (211/272 WORSE), and the same with the original redundant !=0 text (211/272 WORSE). Left: register allocation / loop-invariant -1 store hoisting (vargen / scheduling). */

void d2_RaiseWindow(int win)
{
    D2WIN *w = &d2_Wins[win];
    D2OBJ *tmp[385];
    D2OBJ *o;
    int i, j, first, n;

    first = -1;
    j = -1;
    i = 0x2ff;
    do {
        o = d2_DrawOrder[i];
        if (o != 0) {
            j = i;
            if (o->id == w->parent)
                break;
            if (((D2OBJ *)d2_DrawOrder[i]) != 0 && ((int *)o)[0xc] == (int)w && first == -1)
                first = i;
        }
        i--;
        j = -1;
    } while (i >= 0);
    if (first == -1)
        first = j;
    n = (first - j) * 4 + 4;
    memcpy(tmp, &d2_DrawOrder[j], n);
    memmove(&d2_DrawOrder[j], &d2_DrawOrder[first + 1], 0xbf8 - first * 4);
    i = 0x2ff;
    do {
        if (d2_DrawOrder[i] != 0)
            break;
        i--;
    } while (i >= 0);
    memcpy(&d2_DrawOrder[i + 1], tmp, n);
    for (i = 0; i < w->nobjs; i++)
        d2_RaiseObject(w->objs[i]);
}
