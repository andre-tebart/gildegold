/* 0x44e5a0, checks that a bitmap exists, is square and has power of 2 dimensions; the messages are built
   in a local buffer that nothing reads */

int agi_CheckBitmap(char *name, char report)
{
    char buf[256];
    char *path;
    int w, h, ok = 0;

    path = tex_FindFile(name);
    if (path)
        ok = bmp_GetInfo(path, &w, &h, 0);
    if (!ok) {
        if (report) {
            h = -1;
            w = -1;
            strcpy(buf, path);
            strcat(buf, " is an unknown Bitmap-Type!");
            return 0;
        }
    }
    if (w != h && report) {
        strcpy(buf, path);
        strcat(buf, " is not square!");
        return -2;
    }
    if (!(agi_IsPow2(w) && agi_IsPow2(h)) && report) {
        strcpy(buf, path);
        strcat(buf, " does not have power of 2 dimensions!");
        return 0;
    }
    return 1;
}
