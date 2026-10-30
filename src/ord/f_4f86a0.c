/* 0x4f86a0, a random point inside the cell (x, y) of the scene 0xfd7c50 points to (cell size = +0x20 over 8): asks d3sm_GetVertex (0x45f100) for it with the third argument z */

int unk_Unk4f86a0(int x, int y, int z)
{
    int n;
    char *s;
    unsigned a;
    unsigned b;

    s = unk_Ptrfd7c50;
    n = *(int *)(s + 0x20) / 8;
    a = util_Random(n) + n * x;
    b = util_Random(n) + n * y;
    return d3sm_GetVertexI(s, a, b, z);
}
