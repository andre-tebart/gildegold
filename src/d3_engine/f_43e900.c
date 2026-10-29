/* 0x43e900, writes an event table */

void d3e_SaveEvents(void *f, D3EVENT *t)
{
    int i, n;

    if (t) {
        n = 0;
        for (i = 0; i < D3E_NEVENTS; i++)
            if (t[i].fn)
                n++;
        bio_wr_i(f, n);
        for (i = 0; i < D3E_NEVENTS; i++, t++) {
            if (t->fn) {
                bio_wr_s(f, d3e_EventName(i));
                bio_wr_s(f, t->name);
            }
        }
    } else
        bio_wr_i(f, 0);
}
