/* 0x47c3d0, converts a shape to 16 bit (p must be 1); 0 if it is of another kind */

void *d2_ShpConvertTo16(D2SHP *s, int p)
{
    if (s->type == 2 && p == 1)
        return d2_ShpConvert24To16(s);
    if (s->type == 0 && p == 1)
        return d2_ShpConvert8To16(s);
    return 0;
}
