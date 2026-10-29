/* 0x44f580, three ints that are skipped */

void bgi_SkipInt3(void *f, BGIRD *c)
{
    int i, t;

    for (i = 0; i < 3; i++)
        bio_rd_i(f, &t);
}
