/* 0x478980, width | height << 16 of a 24 bit bmp, -1 if it can not be opened, -2 if it is not 24 bit */

unsigned int d2_GetBmpSize(char *name)
{
    FILE *f;
    BITMAPINFOHEADER bi;

    f = fopen(name, "rb");
    if (f != 0) {
        fseek(f, 0xe, 0);
        fread(&bi, 1, 0x28, f);
        fclose(f);
        if (bi.biPlanes == 1 && bi.biBitCount == 0x18 && bi.biCompression == 0)
            return bi.biHeight << 16 | bi.biWidth;
        return 0xfffffffe;
    }
    return 0xffffffff;
}
