/* 0x4bff40, saves the routes of all carts (0x4ffe90(0, 2, 7, 4, 0x1d)): the number of points, then each point @deep behaviour: all 15 code hunks are the five vfs_fwrite tests (ours test eax,eax; je return 0 from 'if (!vfs_fwrite(...))', the original test eax,eax; ja body from a '> 0' test with jump-to-body layout) plus the q null test the original keeps separate (mov eax,[eax+0x29]; test; je before moving esi); the statements and the saved order (cartId, buildingId, loadSlots 0x18, unloadSlots 0x18) are the original's. Rewriting the five guards as '> 0' needs the restructured layout - deep. Q2 (gm_Query/4ffcc0, vfs_fwrite) and Q4 (c+0x29 raw) stay open */

int rt_SaveRoutes(void *f)
{
    char *c;
    RTPOINT *q;
    int n;

    n = 0;
    for (c = gm_Query(0, 2, 7, 4, 0x1d); c != 0; c = gm_QueryNext()) {
        if (*(RTPOINT **)(c + 0x29) != 0)
            n += rt_PointCount(*(RTPOINT **)(c + 0x29));
    }
    if (!vfs_fwrite(&n, 4, 1, f))
        return 0;
    for (c = gm_Query(0, 2, 7, 4, 0x1d); c != 0; c = gm_QueryNext()) {
        for (q = *(RTPOINT **)(c + 0x29); q != 0; q = q->next) {
            if (!vfs_fwrite(&q->cartId, 4, 1, f))
                return 0;
            if (!vfs_fwrite(&q->buildingId, 4, 1, f))
                return 0;
            if (!vfs_fwrite(q->loadSlots, 0x18, 1, f))
                return 0;
            if (!vfs_fwrite(q->unloadSlots, 0x18, 1, f))
                return 0;
        }
    }
    return 1;
}
