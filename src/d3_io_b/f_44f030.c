/* 0x44f030 */

void bgi_Bitmap0(void *f, BGIRD *c)
{
    char buf[256];
    char *dot;

    bio_rd_s(f, buf);
    dot = strrchr(buf, '.');
    if (dot)
        *dot = 0;
    strcpy(BGI_M(c).name[0], buf);
}
