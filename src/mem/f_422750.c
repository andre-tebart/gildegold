/* 0x422750, format the memory statistics (per group: max / current blocks and bytes, the filtered blocks) into a local buffer; the original has no output call */

void mem_Dump(char *filter)
{
    char name[16];
    char buf[1024];
    MEMGROUP *g;
    MEMENTRY *e;
    char match;
    char *c;
    int i;

    if (!mem_Bytes && !mem_Blocks)
        sprintf(buf, "[*] Memory-Subsystem-Information:\n--> max_used %i blocks in %i bytes (%ik).",
                mem_MaxBlocks, mem_MaxBytes, mem_MaxBytes >> 10);
    else
        sprintf(buf, "[*] Memory-Subsystem-Information: [*]\n--> max_used %i blocks in %i bytes (%ik). %i block(s) left in %i (%ik) bytes.",
                mem_MaxBlocks, mem_MaxBytes, mem_MaxBytes >> 10, mem_Blocks, mem_Bytes, mem_Bytes >> 10);
    g = &mem_Main;
    do {
        if (filter && strstr(g->name, filter))
            match = 1;
        else
            match = 0;
        if (g->maxblocks > 0 && (g->name[0] != '!' || g->blocks || match) && (!filter || match)) {
            strcpy(name, g->name);
            c = strrchr(name, ':');
            if (c)
                *c = 0;
            if (!g->bytes && !g->blocks)
                sprintf(buf, "[%s] w/ %i (%ik) bytes in %i block(s) (called %i times).",
                        name, g->maxbytes, g->maxbytes >> 10, g->maxblocks, g->calls);
            else
                sprintf(buf, "[%s] -> w/ %i/%i (%ik/%ik) bytes in %i/%i block(s) (called %i times).",
                        name, g->bytes, g->maxbytes, g->bytes >> 10, g->maxbytes >> 10, g->blocks, g->maxblocks, g->calls);
            if (match) {
                for (i = 0, e = mem_Table; i < mem_Max; i++, e++)
                    if (e->ptr && e->group == g)
                        sprintf(buf, "{%s} w/ %i bytes (%ik).", e->tag, e->size, e->size >> 10);
            }
        }
        g = g->next;
    } while (g);
}
