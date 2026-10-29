/* 0x424f30 */
unsigned short path_Find(PATHOUT *out, PMAP *map, int sx, int sy, int ex, int ey, int type)
{
    unsigned short start, n;

    if (map && ex <= map->size && ey <= map->size && ex >= 0 && ey >= 0) {
        out->count = 0;
        if (path_Mode) {
            start = path_Search1(map, sx, sy, ex, ey, type);
            if (start != 0xffff)
                for (n = start; out->count < out->max && n != 0xffff; n = map->nodes[n].parent) {
                    out->xy[out->count * 2] = path_Mask & n;
                    out->xy[out->count * 2 + 1] = n >> path_Shift;
                    out->count++;
                }
        } else {
            start = path_Search2(map, sx, sy, ex, ey, type);
            if (start != 0xffff)
                for (n = start; out->count < out->max && n != 0xffff; n = map->nodes[n].parent) {
                    out->xy[out->count * 2] = path_Mask & n;
                    out->xy[out->count * 2 + 1] = n >> path_Shift;
                    out->count++;
                }
        }
        return start;
    }
    return 0xffff;
}
