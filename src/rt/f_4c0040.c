/* 0x4c0040, loads the routes (0x4bff40) into the carts (their id in cartId, consecutive points per cart) @deep behaviour: 168 code hunks, all shifts of the same stream - frame 0xc vs the original 0x8 (one extra int local), esi/ebx/ebp register allocation throughout and the load loop's pointer arithmetic (add esi,0x8 vs lea esi,[ebx+0x8]); the statements (n = fread; buf = mem_Alloc(n << 6, "rt_load"); the four freads per point; the cart chain via rt_AddPoint with cartId/buildingId/slots) match the disassembly. Needs frame/register steering. Q2 (ls_Read, gm_FindObjectInst) and Q4 (c+0x29 / c+2 raw) stay open */

int rt_LoadRoutes(void *f)
{
    RTPOINT *buf;
    RTPOINT *r;
    RTPOINT *q;
    char *c;
    int i;
    int n;

    if (!ls_Read(&n, 4, 1, f))
        return 0;
    buf = mem_Alloc(n << 6, "rt_load");
    for (i = 0; i < n; i++) {
        if (!ls_Read(&buf[i].cartId, 4, 1, f))
            return 0;
        if (!ls_Read(&buf[i].buildingId, 4, 1, f))
            return 0;
        if (!ls_Read(buf[i].loadSlots, 0x18, 1, f))
            return 0;
        if (!ls_Read(buf[i].unloadSlots, 0x18, 1, f))
            return 0;
    }
    for (i = 0; i < n; ) {
        r = &buf[i];
        c = gm_FindObjectInst(r->cartId);
        *(RTPOINT **)(c + 0x29) = 0;
        do {
            q = rt_AddPoint(*(RTPOINT **)(c + 0x29));
            if (*(RTPOINT **)(c + 0x29) == 0)
                *(RTPOINT **)(c + 0x29) = q;
            q->buildingId = r->buildingId;
            q->cartId = r->cartId;
            i++;
            memcpy(q->loadSlots, r->loadSlots, sizeof(q->loadSlots));
            memcpy(q->unloadSlots, r->unloadSlots, sizeof(q->unloadSlots));
            r++;
            if (i >= n)
                goto done;
        } while (r != 0 && *(int *)(c + 2) == r->cartId);
    }
done:
    mem_Free(buf);
    return 1;
}
