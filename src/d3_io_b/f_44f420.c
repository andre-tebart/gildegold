/* 0x44f420, number of materials */

void bgi_NumMats(void *f, BGIRD *c)
{
    int n;

    bio_rd_i(f, &c->nmats);
    n = c->nmats;
    c->cur = 0;
    if (n > 0)
        c->mats = (D3BGFMAT *)mem_Alloc(n * sizeof(D3BGFMAT), "d3_io_b:BGIMaterialBuffer");
}
