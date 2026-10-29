/* 0x4f8300, asks 0x45f170 (the scene 0xfd7c50 points to, a, x, y, &y) for a position and divides *x and *y by the scene's cell size (+0x20, over 8); 0 when it fails */

int ord_GetGridCell(char *p, int *x, int *y)
{
    int n;
    int *py;

    py = y;
    n = *(int *)(unk_Ptrfd7c50 + 0x20) / 8;
    if (d3_GetHeightAt(unk_Ptrfd7c50, p, x, y, &y) != 0) {
        *x = *x / n;
        *py = *py / n;
        return 1;
    }
    return 0;
}
