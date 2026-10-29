/* 0x44f770, a string that is not used */

void bgi_SkipStr(void *f, BGIRD *c)
{
    char buf[256];

    bio_rd_s(f, buf);
}
