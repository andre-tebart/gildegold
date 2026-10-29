/* 0x408490, walk actions: finds a path from the character to the target character ("ch:GetNewPath"?) @deep B3 draft, 14/512 bytes, 28 differing instructions of 173 (173/173, same multiset). Only the slot choice is left: which of two symmetric address-taken locals gets the dead parameter slot - original x1 in a's slot (px = &[esp+0x30], z1 = &[esp+0x14], x2/z2 likewise), ours z1 in a's slot and x1 in the low local. Kept this session (measurable gain): z1's clamp written before x1's (independent clamps, same meaning) makes the store schedule and the clamp slots match: 34 -> 28 differing instructions, 16 -> 14 bytes; with it only the 4084ad and 4084d8 slots hunks remain. Tried with no effect, one build each: declaration order (x1,z1,... and z1,x1,...), the copy-back order (x1=ox;z1=oz vs z1=oz;x1=ox). This is the ch4 pattern already in notes/units/ch.md and notes/vc6-matching.md ('two address-taken int x/z locals with swapped stack slots: declaration order, names, scope did not help; only a dead z = 0 or storing z first flipped them (both add code)'). Q2 gap: u.walk.unk0fc - written -1 by the four path builders (0x408387, 0x408652, 0x408984, 0x409d1e) and read nowhere (the whole ch TU disassembly holds only those four [reg+0xfc] references, chc.c never touches 0xfc), so no role name can be evidenced. Q7 fixed: d3sm_FindGround's prototype is in d3_sm.h now. */

int ch_PathToTarget(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    D3SM *h = (D3SM *)ch_GetHMap(ch);
    int x1, z1, x2, z2, ox, oz;
    float y;
    CHARACTER *t;

    if (!d3_GetHeightAt(h, &a->ch->obj->pos, &x1, &z1, &y))
        return 0;
    if (!d3_GetHeightAt(h, &a->u.walk.target->obj->pos, &x2, &z2, &y))
        return 0;
    if (z1 < 1)
        z1 = 1;
    else if (z1 > h->size - 2)
        z1 = h->size - 2;
    if (x1 < 1)
        x1 = 1;
    else if (x1 > h->size - 2)
        x1 = h->size - 2;
    d3sm_FindGround(h, x1, z1, &ox, &oz);
    x1 = ox;
    z1 = oz;
    ch_FindFreeCell(h, &ch->obj->pos, &a->u.walk.dest, x2, z2, &ox, &oz, 4, 0);
    x2 = ox;
    z2 = oz;
    ch_Path.xy = ch_PathBuf;
    ch_Path.max = 0x200;
    if (path_Find(&ch_Path, (PMAP *)h, x1, z1, x2, z2, ch->pathType) == 0xffff)
        return 0;
    if (a->u.walk.path) {
        mem_Free(a->u.walk.path);
        a->u.walk.path = 0;
    }
    a->u.walk.path = mem_Alloc(ch_Path.count * 2, "ch_t:waypoints");
    if (!a->u.walk.path)
        __asm int 3
    a->u.walk.pathLen = ch_ThinPath(ch, (CHPATH *)&ch_Path, (CHPATHPT *)a->u.walk.path, (CHWALKDEF *)h);
    t = a->u.walk.target;
    a->u.walk.cur = 0;
    a->u.walk.unk0fc = -1;
    a->u.walk.tpos.x = t->obj->pos.x;
    a->u.walk.tpos.y = t->obj->pos.y;
    a->u.walk.tpos.z = t->obj->pos.z;
    return 1;
}
