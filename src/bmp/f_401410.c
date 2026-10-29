/* 0x401410, size and depth of an uncompressed or RLE8 8-bit or 24-bit bitmap */
unsigned char bmp_GetInfo(char *path, int *w, int *h, int *bpp)
{
    BITMAPINFOHEADER bi;
    unsigned char ret = 0;
    VFSFILE *f;

    f = vfs_Tfopen(path, "rb", (int)vfs_Root);
    if (f) {
        vfs_fseek(f, sizeof(BITMAPFILEHEADER), SEEK_SET);
        vfs_fread(&bi, 1, sizeof(bi), f);
        vfs_fclose(f);
        if (bi.biPlanes == 1 && (bi.biBitCount == 8 || bi.biBitCount == 24)
            && (bi.biCompression == BI_RLE8 || bi.biCompression == BI_RGB)) {
            if (w)
                *w = bi.biWidth;
            if (h)
                *h = abs(bi.biHeight);
            if (bpp)
                *bpp = bi.biBitCount;
            ret = 1;
        }
    }
    return ret;
}
