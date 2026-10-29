/* 0x44f010, an int that is not used */

void bgi_SkipInt(void *f, BGIRD *c)
{
    int t = 0;

    bio_rd_i(f, &t);
}
