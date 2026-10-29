/* 0x44f0f0 */

void bgi_Bitmap2(void *f, BGIRD *c)
{
    char buf[256];
    char *dot;

    bio_rd_s(f, buf);
    dot = strrchr(buf, '.');
    if (dot)
        *dot = 0;
    strcpy(BGI_M(c).name[2], buf);
}
