/* 0x44c510, sets the directory the stock objects are searched in */

void d3io_SetStockDir(char *path)
{
    d3io_StockDir = vfs_FindDir(path, vfs_Root);
}
