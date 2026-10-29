/* 0x43ea10, reads the event table of the default scene */

void d3e_LoadSceneEvents(void *f)
{
    int n, i, id;
    char hname[0x100];
    char ename[0x100];

    bio_rd_i(f, &n);
    for (i = 0; i < n; i++) {
        bio_rd_s(f, ename);
        id = d3e_EventId(ename);
        bio_rd_s(f, hname);
        d3e_RegisterSceneEvent(d3_DefaultScene, id, d3e_Resolve(d3_DefaultScene, id, hname), hname);
    }
}
