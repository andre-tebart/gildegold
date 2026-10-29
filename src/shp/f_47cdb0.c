/* 0x47cdb0, draws animation n */

void d2_DrawAnim(int n, void *surf)
{
    short x, y;

    if (d2_Anims[n].bank != 0) {
        x = d2_Anims[n].x;
        y = d2_Anims[n].y;
        if (d2_Anims[n].shadow != 0)
            d2_DrawShapeMode2(x + 6, y + 6, surf, d2_Anims[n].bank, d2_Anims[n].shape);
        d2_DrawShape(x, y, surf, d2_Anims[n].bank, d2_Anims[n].shape);
    }
}
