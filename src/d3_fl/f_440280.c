/* 0x440280, the water point of a tile at x, y */

D3FLPOINT *d3fl_WaterPoint(D3FLTILE *t, int x, int y, char flag)
{
    D3FLPOINT *p = 0;
    D3FLWREG *r;

    if (t && t->wregions) {
        for (r = t->wregions; r->y >= 0; r++)
            if (y == r->y && x >= r->x0 && x <= r->x1) {
                if (r->flag == -1)
                    r->flag = flag;
                p = &t->wpoints[r->idx - r->x0 + x];
                break;
            }
    }
    return p;
}
