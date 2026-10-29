/* 0x4079b0, walk actions: checks the next path cells against the nearby characters others[0..n-1] and,
   if one blocks the way, marks them in the ground map and searches a new path around them ("ch_GetNewPath"
   is the mem_Alloc tag). Returns the new path length, 0 nothing to do, -1 failed, -2 no way around.
   @deep B3 draft, 2291/2768 bytes, 382 differing instructions of 842. Read hunk by hunk against tools/asmdiff and the original 0x4079b0..0x40847f: same statements, one stack slot more (frame 0x9bc vs 0x9b8) and the scalars permuted over the slots (ours tx/tz 0x1c/0x20 vs 0x20/0x24, gx/gz 0x2c/0x30 vs 0x30/0x2c, sx/cx 0x34/0x38 vs 0x20/0x24), oa in edi vs esi; plus two scheduled forms read as the same source: the contacts loop base (ours shares the memset's edx = &ch->contacts, original lea ecx,[ebp+0x94] and stores ch at [ecx-4]) and the rest-buffer loop (original walks path by index in edi and rest by a pointer at rest+1, [ecx-1]/[ecx], ours q at [eax]/[eax+1]). spell menu: 20 reload entries, all re-reading ch/others[i] fields the source already re-reads; dup-tail 407a28/407b3d/408420 are the k==last?0xd:1 and retryAt!=-1?0xd:1 calls already written as two-armed calls; 74 slots hunks. Leads tried this session, one build each: dead locals fx/fz/sz/ret removed (original slot sharing; byte-identical, kept); rest[nrest*2] counter form instead of q++ (2253 bytes but 432 asm lines, reverted - the original does walk the buffer by pointer); contacts store order and the transport?cur-2:cur if/else (byte-identical, reverted). Q2 gap left: u.walk.unk0fc - written -1 by all four path (re)builders (0x408387 ch_GetNewPath, 0x408652 ch_PathToTarget, 0x408984 ch_InitWalk, 0x409d1e ch_ActWalkCells) and read nowhere: the whole ch TU disassembly has only those four [reg+0xfc] references and chc.c never touches 0xfc, so no role name can be evidenced. Everything else named: CHARACTER.contactSteps (0x8e), CHARACTER.speedScale (0x1a4), u.walk.flags + CH_WALK_* (0x160), CHARACTER.pathType (0x240, the path_Find unit type), CH_ACTIONS reads the typed ch->actions (Q4 gone). */

int ch_GetNewPath(CHARACTER *ch, int n, CHARACTER **others)
{
    int res = 0;
    CHACTION *a = CH_ACTIONS(ch);
    int scene = d3t_SceneIndex(ch->scene);
    D3SM *h;
    int steps, cx, cz, tx, tz, bx, bz, gx, gz;
    int i, j, k, r, e, last, nrest, cnt, m, sx;
    float y, d, dx, dy, dz;
    CHACTION *oa;
    CHWAYPT wp[32];
    unsigned char *p, *q;
    unsigned char rest[0x800];

    if (a->type != '-')
        return -1;
    h = (D3SM *)ch_GetHMap(ch);
    ch_CopyVec((D3VECTOR *)ch_PathMap, (D3VECTOR *)h);
    steps = (int)(50.0f / h->cell.x + 0.5);
    if (steps > 10)
        steps = 10;
    if (d3_DefaultScene[scene].map)
        ch_PathMap->cell.x = 60.0f;
    else
        ch_PathMap->cell.x = 60.0f;
    p = (unsigned char *)a->u.walk.path + a->u.walk.cur * 2;
    ch_CellToCell(h, p[0], p[1], ch_PathMap, &cx, &cz);
    if (ch->transport)
        ch_PosToCell(ch_PathMap, &CH_TRANSPORT(ch)->obj->pos, &tx, &tz);
    ch_PosToCell(ch_PathMap, &ch->obj->pos, &bx, &bz);
    for (i = 0; i < n; i++) {
        if (res & 1)
            break;
        oa = CH_ACTIONS(others[i]);
        for (k = 0; k < 16; k++)
            if (ch->contacts[k].ch == others[i] && ch->contacts[k].flags & 1)
                break;
        ch_GetWaypoints(h, others[i], wp);
        for (j = 0; j < 32; j++) {
            if (!wp[j].flags)
                break;
            if (wp[j].flags & 2) {
                if (wp[j].x == cx && wp[j].z == cz) {
                    res = 1;
                    break;
                }
            } else if (wp[j].flags & 1) {
                if (wp[j].x == bx && wp[j].z == bz) {
                    res = 5;
                    break;
                }
                if (wp[j].x == cx && wp[j].z == cz) {
                    if (!oa || oa->type != '-' || oa->u.walk.retryAt != -1) {
                        res = 5;
                        break;
                    }
                    for (k = 0; k < 16 && ch->contacts[k].ch; k++)
                        if (others[i]->contacts[k].ch == ch && others[i]->contacts[k].flags & 2)
                            goto linked;
                    dx = ch->obj->pos.x - others[i]->obj->pos.x;
                    dy = ch->obj->pos.y - others[i]->obj->pos.y;
                    dz = ch->obj->pos.z - others[i]->obj->pos.z;
                    d = sqrt(dx * dx + dy * dy + dz * dz);
                    ch->speedScale = d = (float)(d * 0.01f) * 0.5f;
                    ch->speedScale = min(1.0f, d);
                    res = 2;
                    break;
                linked:
                    res = 1;
                    break;
                }
                if (oa && oa->type == '-' && oa->u.walk.retryAt == -1)
                    continue;
                if (wp[j].x == tx && wp[j].z == tz) {
                    res = 1;
                    break;
                }
            } else if (wp[j].flags & 8) {
                if (wp[j].x == cx && wp[j].z == cz) {
                    if (!oa || oa->type != '-' || oa->u.walk.retryAt != -1) {
                        res = 5;
                        break;
                    }
                    for (k = 0; k < 16 && ch->contacts[k].ch; k++)
                        if (others[i]->contacts[k].ch == ch && others[i]->contacts[k].flags & 2) {
                            res = 1;
                            goto next;
                        }
                    dx = ch->obj->pos.x - CH_TRANSPORT(others[i])->obj->pos.x;
                    dy = ch->obj->pos.y - CH_TRANSPORT(others[i])->obj->pos.y;
                    dz = ch->obj->pos.z - CH_TRANSPORT(others[i])->obj->pos.z;
                    d = sqrt(dx * dx + dy * dy + dz * dz);
                    ch->speedScale = d = (float)(d * 0.01f) * 0.5f;
                    ch->speedScale = min(1.0f, d);
                    res = 2;
                } else if (wp[j].x == tx && wp[j].z == tz) {
                    res = 1;
                    break;
                }
            }
        next:
            ;
        }
    }
    if (res) {
        memset(ch->contacts, 0, 16 * sizeof(CHCONTACT));
        for (i = 0; i < n; i++) {
            ch->contacts[i].flags = res;
            ch->contacts[i].ch = others[i];
        }
        if (res & 1) {
            if (ch->speedScale < 0.5)
                ch->speedScale = 0.5f;
            for (i = 0; i < n; i++) {
                oa = CH_ACTIONS(others[i]);
                if (oa && oa->type == '-' && oa->u.walk.path && oa->u.walk.retryAt == -1) {
                    r = steps;
                    if (r > 3)
                        r = 3;
                    k = others[i]->transport ? oa->u.walk.cur - 2 : oa->u.walk.cur;
                    if (k < 1)
                        k = 1;
                    for (e = k + 2; k < e && k < oa->u.walk.pathLen; k++) {
                        gx = ((unsigned char *)oa->u.walk.path)[k * 2];
                        gz = ((unsigned char *)oa->u.walk.path)[k * 2 + 1];
                        if (k == oa->u.walk.pathLen - 1)
                            ch_SetGroundDiamond(gx, gz, scene, 0xd, r);
                        else
                            ch_SetGroundDiamond(gx, gz, scene, 1, r);
                    }
                }
            }
            for (i = 0; i < n; i++) {
                oa = CH_ACTIONS(others[i]);
                if (oa && oa->type == '-' && oa->u.walk.path && oa->u.walk.retryAt == -1) {
                    if (d3_GetHeightAt(h, &others[i]->obj->pos, &gx, &gz, &y))
                        ch_SetGroundDiamond(gx, gz, scene, 0, steps);
                    if (others[i]->transport && d3_GetHeightAt(h, &CH_TRANSPORT(others[i])->obj->pos, &gx, &gz, &y))
                        ch_SetGroundDiamond(gx, gz, scene, 0, steps * 2);
                } else {
                    if (d3_GetHeightAt(h, &others[i]->obj->pos, &gx, &gz, &y)) {
                        r = min(9, steps);
                        if (oa && oa->u.walk.retryAt != -1)
                            ch_SetGroundDiamond(gx, gz, scene, 0, r);
                        else
                            ch_SetGroundDiamond(gx, gz, scene, 0xd, r);
                    }
                    if (others[i]->transport && d3_GetHeightAt(h, &CH_TRANSPORT(others[i])->obj->pos, &gx, &gz, &y))
                        ch_SetGroundDiamond(gx, gz, scene, 0xd, steps * 2);
                }
            }
            a = CH_ACTIONS(ch);
            if (d3_GetHeightAt(h, &a->ch->obj->pos, &sx, &cx, &y)) {
                last = min(a->u.walk.cur + 16, a->u.walk.pathLen - 1);
                nrest = 0;
                q = rest;
                for (k = last + 1; k < a->u.walk.pathLen; k++) {
                    q[0] = ((unsigned char *)a->u.walk.path)[k * 2];
                    q[1] = ((unsigned char *)a->u.walk.path)[k * 2 + 1];
                    q += 2;
                    nrest++;
                }
                gx = ((unsigned char *)a->u.walk.path)[last * 2];
                gz = ((unsigned char *)a->u.walk.path)[last * 2 + 1];
                m = ch->scene == d3_Scenes || d3_Terrain ? 5 : 2;
                if (!ch_FindFreeCell(h, &ch->obj->pos, &a->u.walk.dest, gx, gz, &cz, &i, steps * m,
                                     a->u.walk.flags | 1)) {
                    ch_RestoreGround((struct D3HMAP *)h);
                    return -1;
                }
                gx = cz;
                gz = i;
                ch_Path.xy = ch_PathBuf;
                ch_Path.max = 0x200;
                if (h->entries[h->size * cx + sx].type == 0xd) {
                    m = ch->scene == d3_Scenes || d3_Terrain ? 5 : 2;
                    if (ch_FindFreeCell(h, &ch->obj->pos, &a->u.walk.dest, sx, cx, &cz, &i, steps * m,
                                        a->u.walk.flags | 1)) {
                        sx = cz;
                        cx = i;
                    }
                }
                if (path_Find(&ch_Path, (PMAP *)h, sx, cx, gx, gz, ch->pathType) == 0xffff) {
                    ch_RestoreGround((struct D3HMAP *)h);
                    return -1;
                }
                cnt = ch_Path.count;
                if (a->u.walk.path) {
                    mem_Free(a->u.walk.path);
                    a->u.walk.path = 0;
                }
                a->u.walk.path = mem_Alloc((cnt + nrest) * 2, "ch_GetNewPath");
                tz = ch_ThinPath(ch, (CHPATH *)&ch_Path, (CHPATHPT *)a->u.walk.path, (CHWALKDEF *)h);
                for (k = 0; k < nrest; k++) {
                    ((unsigned char *)a->u.walk.path)[(tz + k) * 2] = rest[k * 2];
                    ((unsigned char *)a->u.walk.path)[(tz + k) * 2 + 1] = rest[k * 2 + 1];
                }
                a->u.walk.pathLen = tz + nrest;
                a->u.walk.cur = 0;
                a->u.walk.unk0fc = -1;
                gx = ((unsigned char *)a->u.walk.path)[a->u.walk.pathLen * 2 - 2];
                gz = ((unsigned char *)a->u.walk.path)[a->u.walk.pathLen * 2 - 1];
                d3sm_GetVertex(h, gx, gz, &a->u.walk.end);
                a->u.walk.end.x += ch_PathOffset.x;
                a->u.walk.end.y += ch_PathOffset.y;
                a->u.walk.end.z += ch_PathOffset.z;
                if (a->u.walk.pathLen >= 2
                    && ((unsigned char *)a->u.walk.path)[a->u.walk.pathLen * 2 - 2]
                       == ((unsigned char *)a->u.walk.path)[a->u.walk.pathLen * 2 - 4]
                    && ((unsigned char *)a->u.walk.path)[a->u.walk.pathLen * 2 - 1]
                       == ((unsigned char *)a->u.walk.path)[a->u.walk.pathLen * 2 - 3])
                    a->u.walk.pathLen--;
                ch_RestoreGround((struct D3HMAP *)h);
                ch->contactSteps = 4;
                if (sx == gx && cx == gz)
                    return -2;
                return tz;
            }
        }
    }
    return 0;
}
