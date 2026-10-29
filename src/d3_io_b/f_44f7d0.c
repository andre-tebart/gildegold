/* 0x44f7d0 */

void bgi_SkipDword(void *f, BGIRD *c)
{
    int t;

    vfs_fread(&t, 4, 1, f);
}
