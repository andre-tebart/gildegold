/* 0x440170, flood fill */

void d3fl_Fill(char *map, int size, int x, int y, char from, char to)
{
    map[y * size + x] = to;
    if (x + 1 < size && map[y * size + x + 1] == from)
        d3fl_Fill(map, size, x + 1, y, from, to);
    if (x - 1 >= 0 && map[y * size + x - 1] == from)
        d3fl_Fill(map, size, x - 1, y, from, to);
    if (y + 1 < size && map[(y + 1) * size + x] == from)
        d3fl_Fill(map, size, x, y + 1, from, to);
    if (y - 1 >= 0 && map[(y - 1) * size + x] == from)
        d3fl_Fill(map, size, x, y - 1, from, to);
}
