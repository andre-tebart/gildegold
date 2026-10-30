/* 0x44cd40, drops a reference to a mesh and frees it with its textures when the last one is gone */
void d3iot_DeleteStockObject(D3BGF *m)
{
    int i, t, max;
    char *used;
    int z = 0;

    if (--m->refs <= z) {
        m->prev->next = m->next;
        m->next->prev = m->prev;
        max = -1;
        for (i = z; i < m->npolys; i++)
            if (m->polys[i].tex > max)
                max = m->polys[i].tex;
        used = mem_Alloc(max + 1, "d3_io_t:DeleteStockObject");
        for (i = z; i < m->npolys; i++) {
            t = m->polys[i].tex;
            if (t != -1 && !used[t]) {
                tex_Release((D3TEXHANDLE *)&((D3TEXTURE *)d3_Textures)[t]);
                used[m->polys[i].tex] = 1;
            }
        }
        mem_Free(used);
        mem_Free(m->verts);
        m->verts = 0;
        mem_Free(m->polys);
        m->polys = 0;
        mem_Free(m->texnames);
        m->texnames = 0;
        mem_Free(m);
    }
}
