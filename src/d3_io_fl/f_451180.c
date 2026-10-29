/* 0x451180, size of a mesh in the fast chunk file (the description supplies the material and attachment names) */

int d3_FLChunkSize(D3BGF *m, D3BGFDESC *d)
{
    int sz, n, i;
    char *p;

    sz = 0x18;
    if (m->nverts > 0)
        sz = (m->nverts * 3 + 0x1b) * 8;
    n = m->ntex;
    if (n > 0xfe)
        sz = sz + m->npolys * 0x34;
    else
        sz = sz + m->npolys * 0x31;
    sz += n * 6;
    for (i = 0; i < n; i++)
        sz += strlen(d->mats[i].name[2]) + 1 + strlen(d->mats[i].name[1]) + 1 + strlen(d->mats[i].name[0]) + 1;
    n = d->nattach;
    sz += n * 0x18;
    for (i = 0; i < n; i++)
        sz += strlen(d->attach[i].name) + 1;
    return sz;
}
