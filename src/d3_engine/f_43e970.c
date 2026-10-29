/* 0x43e970, reads the event table of an object */

void d3e_LoadEvents(void *f, D3OBJECT *obj)
{
    int n, i, id;
    char hname[0x100];
    char ename[0x100];

    bio_rd_i(f, &n);
    for (i = 0; i < n; i++) {
        bio_rd_s(f, ename);
        id = d3e_EventId(ename);
        bio_rd_s(f, hname);
        if (obj)
            d3e_RegisterEvent(obj, id, d3e_Resolve(obj, id, hname), hname);
    }
}
