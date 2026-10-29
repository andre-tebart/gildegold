/* 0x44c530, finds the file of a stock object ("*" name ext) in the stock directory */

char *d3io_StockFile(char *name, char *ext)
{
    char buf[0x100];

    strcpy(buf, "*");
    strcat(buf, name);
    strcat(buf, ext);
    return vfs_GetPath(buf, d3io_StockDir, vfs_Root, d3io_StockPath);
}
