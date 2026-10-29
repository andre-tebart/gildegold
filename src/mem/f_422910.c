/* 0x422910, report and free the blocks still allocated, dump the groups when verbose, free the group list and the table, reset the counters */

void mem_Exit(void)
{
    char buf[1024];
    int i;
    MEMENTRY *e;
    MEMGROUP *g, *n;

    if (mem_Blocks || mem_Bytes || mem_Verbose) {
        if (mem_Blocks || mem_Bytes) {
            sprintf(buf, "[*] Memory-Shutdown-Anomaly: [*]\nUsed %i blocks in %i bytes.\n\n%i block(s) left in %i bytes.",
                    mem_MaxBlocks, mem_MaxBytes, mem_Blocks, mem_Bytes);
            err_Message(buf);
        }
        if (mem_Verbose)
            mem_Dump(NULL);
        for (i = 0, e = mem_Table; i < mem_Max; i++, e++) {
            if (e->ptr) {
                sprintf(buf, "-> Pointer not freed:%s", e->tag);
                free(e->ptr);
                e->ptr = NULL;
            }
        }
    }
    mem_MaxBlocks = 0;
    mem_Blocks = 0;
    mem_MaxBytes = 0;
    mem_Bytes = 0;
    mem_Max = 0;
    for (g = mem_Main.next; g; g = n) {
        n = g->next;
        free(g);
    }
    free(mem_Table);
    mem_Table = NULL;
}
