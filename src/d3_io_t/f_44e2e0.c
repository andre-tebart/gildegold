/* 0x44e2e0, loads the animation file (name + ".oam") of an object: keys of a path @deep Q2: d3_Unk6333cc (global 0x6333cc, only read here into obj->unk40, no writer, string or caller shows its meaning) and D3IOOBJ.unk40 (written only from that global, meaning unknown); the other names are derivable (vfs_Root for 0x6333fc, the frame stepper 0x4324e0, the anim release 0x433b10) but these two keep Q2 failing, so no invented names */

D3IOPATH *d3_LoadObjectAnim(char *file, D3IOOBJ *obj, char reload)
{
    D3IOPATH *p = 0;
    char *path;
    void *f;
    int magic;
    D3IOKEY **keys;
    int i;
    int n;

    if (obj->path != 0)
        d3anim_FreePath((D3OBJECT *)obj);
    path = d3io_StockFile(file, ".oam");
    if ((path != 0 && obj->type == 4) || obj->type == 3) {
        f = vfs_Tfopen(path, "rb", d3_Unk6333fc);
        bio_rd_i(f, &magic);
        if (magic == 0x3a610001) {
            p = (D3IOPATH *)mem_Alloc(0x44, "d3_io:ObjAnim");
            bio_rd_i(f, &p->nkeys);
            bio_rd_i(f, (int *)&p->flags);
            p->flags.c5 = 0;
            keys = &p->keys;
            p->flags.d1 = reload;
            n = bio_rd_array_quick(f, 0x58, (void **)keys);
            if (n != p->nkeys)
                err_Message("d3_LoadObjectAnim: Wrong number of KeyFrames in saved array!");
            (*keys)[p->nkeys - 1].time = (*keys)[p->nkeys - 2].time;
            p->pos[0] = obj->pos[0];
            p->pos[1] = obj->pos[1];
            p->pos[2] = obj->pos[2];
            p->rot[0] = obj->rot[0];
            p->rot[1] = obj->rot[1];
            p->rot[2] = obj->rot[2];
            for (i = 0; i < p->nkeys; i++)
                (*keys)[i].time *= 3;
            if (p->flags.rev) {
                n = p->nkeys - 1;
                p->key = n;
                p->time = p->keys[n].time - 1;
                p->next = d3anim_NextFrameIo(p->flags, n, 0, n, p->nkeys);
            } else {
                p->next = 1;
                p->key = 0;
                p->time = 0;
            }
        }
        vfs_fclose(f);
    }
    obj->path = p;
    obj->unk40 = d3_Unk6333cc;
    return p;
}
