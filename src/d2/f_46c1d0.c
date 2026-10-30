/* 0x46c1d0, centres an object (flag centred) horizontally on its window or on the screen @deep draft (59/144 bytes; 67 instructions both sides): after the dup-tail pass only register allocation and epilogue placement differ - ours keeps obj in edi and win in ebx where the original has obj in ebx and win in edi (one bijective swap), the entry pushes are esi,edi vs ebx,esi,edi, and our two arms share pop edi/pop esi/ret where the original duplicates the whole tail; semantics read equal against 0x46c1d0 and its Ghidra pseudocode. spell.py dup-tail tried: statements in both arms (116 bytes differ, worse prologue), pull after the branch (best, 59), swap branch polarity (86); also tried d2_Objs[obj] subscripts instead of the local pointer, a named D2WIN *win local, the early-return !centred form and an a?b:c argument (all the same 59). Readability passes Q1..Q7. Left: VC6 register allocation and the tail merge - listing/c2prio/siblings. */

void d2_CenterObj(int obj)
{
    D2OBJ *o = &d2_Objs[obj];
    int x;

    if (o->centred) {
        if (o->win)
            x = o->win->w / 2 - o->w / 2 + o->win->x;
        else
            x = d2_ScreenW / 2 - o->w / 2;
        d2_MoveObjI(x, o->y, obj);
    }
}
