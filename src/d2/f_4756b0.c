/* 0x4756b0, places a window: flags 1 = centred in x, 2 = centred in y @deep B3 draft, readable; 57/52 instructions, 27 differing. All hunks read: the whole difference is the parent load - ours hoists mov edx,[ecx+0x26c] (w->parent) above test bl,2 and folds the two d2_MoveObj tails (push edx once), the original re-reads w->parent inside each arm between the y computation and the pushes (menu: reload at 4756e8, dup-tail at 475728). Tried per the d2n note with no change: locals per branch, early return, d2_Wins[win] / D2VOP forms for one or both parent reads, re-taking w, dummy volatile; the menu prescriptions are the same class. x/y computation and the flags meaning match. Remaining: VC6 CSE / tail-merge scheduling -> deep (listing/slotmap) */

void d2_PlaceWindow(int win, unsigned char flags)
{
    D2WIN *w = &d2_Wins[win];
    int x;

    if (flags & 1)
        x = d2_HalfScreenW - w->w / 2;
    else
        x = w->x;
    if (flags & 2)
        d2_MoveObj(x, d2_HalfScreenH - w->h / 2, w->parent);
    else
        d2_MoveObj(x, w->y, w->parent);
}
