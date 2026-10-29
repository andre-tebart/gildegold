/* 0x4223c0 */
void *mem_Alloc(int size, char *tag)
{
    char buf[256];
    char group[16];
    MEMGROUP *g, *last;
    void *p;
    char *c;
    int n, i;
    MEMENTRY *e;

    if (!mem_Table)
        return NULL;
    if (!size)
        return NULL;
    mem_Count++;
    g = &mem_Main;
    if (tag && (c = strchr(tag, ':'))) {
        n = c - tag + 1;
        if (n >= 16) {
            sprintf(buf, "m_alloc_debug: GroupInformation too long: %s", tag);
            err_Message(buf);
        }
        strncpy(group, tag, n);
        group[n] = 0;
        g = &mem_Main;
        do {
            if (!strcmp(group, g->name))
                break;
            last = g;
            g = g->next;
        } while (g);
        if (!g) {
            g = malloc(sizeof(MEMGROUP));
            strcpy(g->name, group);
            g->blocks = 0;
            g->bytes = 0;
            g->maxblocks = 0;
            g->maxbytes = 0;
            g->calls = 0;
            g->next = NULL;
            last->next = g;
        }
    }
    g->calls++;
    n = size;
    p = malloc(n);
    if (!p) {
        sprintf(group, "Out of Memory Error: Malloc - Failed %s for %u bytes", tag, size);
        return NULL;
    }
    MEM_ZERO_VAR(p, n);
    if (mem_Blocks < mem_Max) {
        for (i = 0, e = mem_Table; i < mem_Max; e++, i++)
            if (!e->ptr)
                break;
        e->ptr = p;
        e->tag = tag;
        e->size = size;
        e->group = g;
        mem_Bytes += size;
        if (mem_Bytes > mem_MaxBytes)
            mem_MaxBytes = mem_Bytes;
        g->bytes += size;
        if (g->bytes > g->maxbytes)
            g->maxbytes = g->bytes;
        mem_Blocks++;
        if (mem_Blocks > mem_MaxBlocks)
            mem_MaxBlocks = mem_Blocks;
        g->blocks++;
        if (g->blocks > g->maxblocks)
            g->maxblocks = g->blocks;
    }
    return p;
}
