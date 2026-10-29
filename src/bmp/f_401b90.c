/* 0x401b90, writes a bottom-up 24-bit bitmap from top-down RGB data */

unsigned char bmp_Save24(char *path, int w, int h, unsigned char *data)
{
    struct {
        BITMAPINFOHEADER bmiHeader;
        RGBQUAD bmiColors[1];
    } bi;
    BITMAPFILEHEADER bf;
    VFSFILE *f;
    unsigned char *row, *src;
    int x, y;

    BMP_ZERO(&bi, sizeof(bi));
    BMP_ZERO(&bf, sizeof(bf));
    bf.bfType = 0x4d42;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biYPelsPerMeter = 1;
    bf.bfSize = w * h * 3 + sizeof(bf) + sizeof(bi);
    bf.bfReserved2 = 0;
    bf.bfReserved1 = 0;
    bi.bmiColors[0].rgbRed = 0;
    bi.bmiColors[0].rgbGreen = 0;
    bi.bmiColors[0].rgbBlue = 0;
    bi.bmiHeader.biCompression = BI_RGB;
    bi.bmiHeader.biSizeImage = 0;
    bi.bmiHeader.biClrUsed = 0;
    bi.bmiHeader.biClrImportant = 0;
    bi.bmiHeader.biXPelsPerMeter = 1;
    bf.bfOffBits = sizeof(bf) + sizeof(bi);
    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth = w;
    bi.bmiHeader.biHeight = h;
    bi.bmiHeader.biBitCount = 24;
    f = vfs_Tfopen(path, "wbn", (int)vfs_Root);
    if (f) {
    row = mem_Alloc(w * 3, "!bmp:SaveBMP24(temp_row)");
    vfs_fwrite(&bf, sizeof(bf), 1, f);
    vfs_fwrite(&bi, sizeof(bi), 1, f);
    for (y = h - 1; y >= 0; y--) {
        for (x = 0; x < w; x++) {
            row[x * 3] = data[(y * w + x) * 3 + 2];
            row[x * 3 + 1] = data[(y * w + x) * 3 + 1];
            row[x * 3 + 2] = data[(y * w + x) * 3];
        }
        vfs_fwrite(row, w * 3, 1, f);
    }
    vfs_fclose(f);
    mem_Free(row);
    return 1;
    }
    return 0;
}
