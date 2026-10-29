/* 0x43cc10, (re)allocates the mesh data of info from its counts: ntris D3TRIs and nkeys + 8 D3MESHKEYs
   (the last 8 are the bounding box corners that follow the keys); the previous blocks are freed. */

void d3_AllocObjectPolys(D3OBJINFO *info)
{
    if (info->nkeys && info->ntris) {
        if (info->tris) {
            mem_Free(info->tris);
            info->tris = 0;
        }
        info->tris = mem_Alloc(info->ntris * 0x28, "d3:AllocObjectPolys");
        if (info->keys) {
            mem_Free(info->keys);
            info->keys = 0;
        }
        info->keys = mem_Alloc((info->nkeys + 8) * 0x50, "d3:AllocObjectPoints");
    }
}
