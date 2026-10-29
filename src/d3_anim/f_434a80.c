/* 0x434a80, bytes used by a cached animation, -1 if not loaded. */

int d3anim_GetSize(D3ANIMDATA *a)
{
    int size = 0;

    if (a && a->frames) {
        if (a->frames->pos)
            size = a->nverts * 12;
        if (a->frames->packed)
            size += a->nverts * 3;
        if (a->frames->normals)
            size += a->nverts * 3;
        return a->nframes * (size + sizeof(D3ANIMFRAME)) + sizeof(D3ANIMDATA);
    }
    return -1;
}
