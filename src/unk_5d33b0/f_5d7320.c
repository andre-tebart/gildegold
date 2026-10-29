/* 0x5d7320, loads the loading screen image (275 B): opens path (vfs_Tfopen, "rb", vfs_Root), reads
   0xe100 bytes (ls_Read 0x51c920, 0x78 x 0xa0 RGB triples) and converts them to the 16-bit pixels of
   ai_ScreenImage 0x766320 (dd_RGB, the pixel index runs across both loops), then frees the buffer and
   closes. Returns nothing (the original returns no value; the callers ignore it). */

void ai_LoadScreenImage(char *path)
{
    unsigned char *buf;
    VFSFILE *h;
    int i = 0, x, y;

    h = vfs_Tfopen(path, "rb", vfs_Root);
    if (h == 0)
        return;
    buf = (unsigned char *)mem_Alloc(0xe100, "ls:tmp");
    ls_Read(buf, 0xe100, 1, (int)h);
    for (y = 0; y < 0x78; y++) {
        for (x = 0; x < 0xa0; x++) {
            ai_ScreenImage[i] = dd_RGB(buf[i * 3], buf[i * 3 + 1], buf[i * 3 + 2]);
            i++;
        }
    }
    if (buf != 0)
        mem_Free(buf);
    vfs_fclose(h);
}
