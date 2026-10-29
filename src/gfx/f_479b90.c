/* 0x479b90, the same, clipped */

int d2_PutPixelClipped(int x, int y, unsigned short color, D2SURF *surf)
{
    if (x >= 0 && x < surf->w && y >= 0 && y < surf->h && x >= surf->left && x < surf->right && y >= surf->top
        && y < surf->bottom) {
        ((unsigned short *)surf->bits)[surf->pitch * y + x] = color;
        return 1;
    }
    return 0;
}
