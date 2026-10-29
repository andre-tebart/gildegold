/* 0x4264a0, frees the oldest loaded entries (sb_FindOldestEntry) until size bytes fit into the memory
   budget again, subtracting their size from sb_Used; returns 0, -1 when the oldest entry has no data or
   nothing more can be freed */

int sb_FreeMemory(int size)
{
    SBENTRY *e;

    while (sb_Mem - sb_Used < size) {
        e = sb_FindOldestEntry();
        if (e) {
            if (!e->data)
                return -1;
            if (e->type == 1)
                sb_Used -= ((SBDATA *)e->data)->len + 12;
            else
                sb_Used -= ((SBVAR *)e->data)->len + 12;
            mem_Free(e->data);
            e->data = NULL;
        } else if (sb_Mem - sb_Used < size)
            return -1;
    }
    return 0;
}
