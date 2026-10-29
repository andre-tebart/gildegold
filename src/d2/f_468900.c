/* 0x468900, reads the picture idx from the file path (default: the archive) unless it is in memory */

int d2_LoadPic(int idx, char *path)
{
    char buf[128];
    void *data;

    if (!d2_Pics[idx].data) {
        while (d2_Pics[idx].size + d2_PicBytes > d2_PicLimit)
            if (!d2_FreeOldest())
                break;
        d2_Pics[idx].data = mem_Alloc(d2_Pics[idx].size, "d2:d2_LoadObj");
        if (!d2_Pics[idx].data) {
            sprintf(buf, "d2_Show: Memory could not be allocated for objectID(%i):%s", idx, d2_Pics[idx].name);
            return 0;
        }
        if (path)
            d2_File = fopen(path, "rb");
        else
            d2_File = fopen(d2_GfxPath, "rb");
        if (!d2_File)
            return 0;
        fseek(d2_File, d2_Pics[idx].offset, 0);
        fread(d2_Pics[idx].data, d2_Pics[idx].size, 1, d2_File);
        if (d2_Pics[idx].type < 0x10) {
            data = d2_Pics[idx].data;
            if (d2_PaletteDepth != ((D2PICHDR *)data)->bpp && ((D2PICHDR *)data)->bpp != 0)
                d2_Pics[idx].data = d2_BankConvertTo16(data, d2_PaletteDepth, 0);
        }
        d2_PicBytes += d2_Pics[idx].size;
    }
    fclose(d2_File);
    return 1;
}
