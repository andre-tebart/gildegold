/* 0x4ce040, the nearest passable cell around (a, b) (rings of the distance d = 0 .. 0x3f): *ox, *oy */

int ai_Unk4ce040(int a, int b, int *ox, int *oy)
{
    int w = *(int *)(*(char **)(ai_Unk5fb5b0 + 0xb0) + 0x20);
    int d, y, x, k, ylo, yhi, xlo, xhi, t;

    for (d = 0; d < 0x40; d++) {
        yhi = w - 1 < b + d ? w - 1 : b + d;
        ylo = b - d < 0 ? 0 : b - d;
        for (y = ylo; y <= yhi; y++) {
            t = b - y;
            k = d - (t < 0 ? -t : t);
            xhi = w - 1 < k + a ? w - 1 : k + a;
            xlo = a - k < 0 ? 0 : a - k;
            for (x = xlo; x <= xhi; x++) {
                if (ai_Unk4cdfe0(x, y) != 0) {
                    *ox = x;
                    *oy = y;
                    return 1;
                }
            }
        }
    }
    return 0;
}
