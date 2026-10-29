/* 0x4f0180, frees a graveyard record g: its graves (+0x1c), zombies (+0x20) and itself */

void gv_FreeGraveyard(int *g)
{
    if (g != 0) {
        if (g[7] != 0) {
            mem_Free((void *)g[7]);
            g[7] = 0;
        }
        if (g[8] != 0) {
            mem_Free((void *)g[8]);
            g[8] = 0;
        }
        mem_Free(g);
    }
}
