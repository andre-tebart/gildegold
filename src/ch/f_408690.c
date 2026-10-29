/* 0x408690, walk actions: drops the current animation and finds the path to args[0], args[1] in scene args[2] @deep B3 draft, 15/1056 bytes, 32 differing instructions (1056/1056 same multiset): the whole diff is the slot order of the target cell and y - original z,x,t[0],t[1],y,name (t at frame+0x8, y at +0x10), ours z,x,y,t (y +0x8, t +0xc); x/z, name and the frame size (0x74) match. Read against tools/disasm 0x408690 and the VC6 listing (slot map _x=-112 _z=-116 _y=-108 _t=-104 _name=-96). Tried this session, one build each: int t[2] declared after y (byte-identical, the listing's map does not move), name declared first (byte-identical), scalars tx/tz instead of t[2] (24 bytes / 50 asm lines, reverted). The recorded round-7 attempts (declaration orders, y as float[1], 4-byte struct, x/z arrays, t as a struct, dead x=z=0) are in notes/units/ch.md; VC6 puts the address-taken array above the float whatever the declaration order. Q2 gaps: ch_Unk631058 (0x631058, read by 10 sites in ch.c only, never written in any drafted source, no data alias or string names it) and u.walk.unk0fc (written -1 by the four path builders, no reader in the whole ch TU). Q4 gaps: CH_UNK080 (0x80: the struct calls it D3ANIMDATA *next, but ch_TakingObject prints it with %s, so its type needs a decision with evidence) and CH_UNK110 (0x110: a single byte store 0 at 0x4088ec, no reader). Readability advanced for the unit this session: CH_SLOT/CH_FLAGB/CH_FLAGW are gone from ch.c (ch->slot, ch->flags.bytes.lo, ch->flags.all; CH_XSLOT now casts ch->slot). */

/* ---- 0x408690..0x40a900 (worker ch5) ---- */

int ch_InitWalk(CHACTION *a, D3SM *h)
{
    CHARACTER *ch = a->ch;
    int x, z, t[2];             /* t: target cell x, z */
    float y;
    D3ANIMDATA *ad;
    char name[0x60];

    if (ch->morph) {
        sprintf(name, "morph_%i", ch);
        d3_ReleaseTexture(ch->obj->material->animCtl, name);
        ad = (D3ANIMDATA *)d3_FindAnim(name);
        d3anim_Release(ad, 1);
        ch->morph = 0;
    }
    d3t_ReleaseObjectTextures(ch->obj);
    CH_UNK080(ch) = 0;
    ch->morph = 0;
    ch->slot = 0;
    ch->speedScale = 1.0f;
    a->ch->flags.all &= ~8;
    a->u.walk.path = 0;
    a->u.walk.started = 0;
    a->u.walk.retryAt = -1;
    memset(ch->contacts, 0, sizeof ch->contacts);
    t[0] = a->args[0];
    t[1] = a->args[1];
    if (d3t_SceneIndex(ch->scene) != a->args[2]) {
        ch_RemoveAction(a);
        return 0;
    }
    if (a->u.walk.flags & CH_WALK_TARGET)
        a->u.walk.target = ch_FindByName(a->u.walk.tname);
    if (!d3_GetHeightAt(h, &a->ch->obj->pos, &x, &z, &y)) {
        if (ch_Unk631058)
            a->ch->flags.bytes.lo |= 8;
        if (a->u.walk.path) {
            mem_Free(a->u.walk.path);
            a->u.walk.path = 0;
        }
        ch_RemoveAction(a);
        return 0;
    }
    if (x < 1)
        x = 1;
    else if (x > h->size - 2)
        x = h->size - 2;
    if (z < 1)
        z = 1;
    else if (z > h->size - 2)
        z = h->size - 2;
    if (ch->kind & 4)
        a->u.walk.flags &= ~CH_WALK_DEST;
    if (a->u.walk.flags & CH_WALK_DEST)
        ch_FindFreeCell(h, &ch->obj->pos, &a->u.walk.dest, t[0], t[1], &t[1], &t[0], 8, a->u.walk.flags);
    else
        ch_FindFreeCell(h, &ch->obj->pos, &ch->obj->pos, t[0], t[1], &t[1], &t[0], 8, a->u.walk.flags);
    ch_Path.xy = ch_PathBuf;
    ch_Path.max = 0x200;
    if (path_Find(&ch_Path, (PMAP *)h, x, z, t[1], t[0], ch->pathType) == 0xffff) {
        CH_UNK110(a->ch) = 0;
        if (ch_Unk631058)
            a->ch->flags.bytes.lo |= 8;
        if (a->u.walk.path) {
            mem_Free(a->u.walk.path);
            a->u.walk.path = 0;
        }
        if (a->u.walk.fail)
            a->u.walk.fail(a);
        ch_RemoveAction(a);
        return 0;
    }
    a->u.walk.path = mem_Alloc(ch_Path.count * 2, "ch_t:waypoints");
    a->u.walk.pathLen = ch_ThinPath(ch, (CHPATH *)&ch_Path, (CHPATHPT *)a->u.walk.path, (CHWALKDEF *)h);
    a->u.walk.cur = 0;
    a->u.walk.unk0fc = -1;
    d3sm_GetVertex(h, ((unsigned char *)a->u.walk.path)[0], ((unsigned char *)a->u.walk.path)[1], &a->u.walk.end);
    a->u.walk.end.x += ch_PathOffset.x;
    a->u.walk.end.y += ch_PathOffset.y;
    a->u.walk.end.z += ch_PathOffset.z;
    if (a->u.walk.flags & CH_WALK_TARGET) {
        a->u.walk.tpos.x = CH_POS038(a->u.walk.target)->x;
        a->u.walk.tpos.y = CH_POS038(a->u.walk.target)->y;
        a->u.walk.tpos.z = CH_POS038(a->u.walk.target)->z;
    } else {
        d3sm_GetVertex(h, t[1], t[0], &a->u.walk.tpos);
        a->u.walk.end.x += ch_PathOffset.x;     /* sic: end, not tpos */
        a->u.walk.end.y += ch_PathOffset.y;
        a->u.walk.end.z += ch_PathOffset.z;
        if ((a->u.walk.flags & CH_WALK_DEST) && !d3_VecEqual(&a->u.walk.tpos, &a->u.walk.dest, 30.0f)) {
            a->u.walk.dest.x = a->u.walk.tpos.x;
            a->u.walk.dest.y = a->u.walk.tpos.y;
            a->u.walk.dest.z = a->u.walk.tpos.z;
        }
    }
    a->u.walk.started = 0;
    return 1;
}
