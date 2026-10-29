/* 0x479f10, copies the row table to the end of a shape */

void d2_SetLineTable(int *rows, D2SHP *shp)
{
    memcpy((char *)shp + shp->lineoff, rows, shp->h * 4);
}
