/* 0x479cd0, sets the clip rectangle of surf to the w x h rectangle at (x, y): left / top / right / bottom (the
   same rectangle d2_SetClipRect sets on the screen globals); a NULL surf is ignored */

void d2_SetSurfaceClip(int x, int y, int w, int h, D2SURF *surf)
{
    if (surf != 0) {
        surf->left = x;
        surf->top = y;
        surf->right = x + w;
        surf->bottom = y + h;
    }
}
