/* 0x44f7f0, the attachment points: byte, name, byte, vector, byte, vector each */

void bgi_Attach(void *f, BGIRD *c)
{
    int n = 0, i;
    char b;

    bio_rd_i(f, &n);
    c->nattach = n;
    for (i = 0; i < n; i++) {
        bio_rd_c(f, &b);
        bio_rd_s(f, c->attach[i].name);
        bio_rd_c(f, &b);
        bio_rd_v3(f, (int *)&c->attach[i].f[0]);
        bio_rd_c(f, &b);
        bio_rd_v3(f, (int *)&c->attach[i].f[3]);
    }
}
