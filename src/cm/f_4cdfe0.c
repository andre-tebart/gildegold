/* 0x4cdfe0, 1 when the cell (x, y) of the map ai_Unk5fb5b0 + 0xb0 is inside and passable */

int ai_Unk4cdfe0(int x, int y)
{
    char *g = *(char **)(ai_Unk5fb5b0 + 0xb0);
    int w = *(int *)(g + 0x20);

    if (x > w || x < 0 || y < 0 || y > w || x >= w || y >= w)
        return 0;
    if (*(int *)(*(char **)(g + 0x24) + (w * y + x) * 0x1c) == 0 || *(int *)(*(char **)(g + 0x24) + (w * y + x) * 0x1c) == 0xd)
        return 0;
    return 1;
}
