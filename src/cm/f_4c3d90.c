/* 0x4c3d90, as 0x4c3d00, value by value */

int cm_AddFieldValue(unsigned char size, unsigned char n, unsigned short field, void *v)
{
    unsigned char *p;
    int o;
    unsigned int len;

    o = ai_Unka68c08;
    if (size != 1 && size != 2 && size != 4)
        return 1;
    len = n * size + 4 + o;
    if (len > 0x76)
        return 1;
    p = &ai_Unka68c20.buf[o];
    ai_Unka68c20.n++;
    ai_Unka68c08 = len;
    p[0] = size;
    p[1] = n;
    *(unsigned short *)(p + 2) = field;
    p += 4;
    if (size == 1) {
        unsigned char *s = (unsigned char *)v;
        for (; n != 0; n--)
            *p++ = *s++;
    } else if (size == 2) {
        short *s = (short *)v, *d = (short *)p;
        for (; n != 0; n--)
            *d++ = *s++;
    } else if (size == 4) {
        int *s = (int *)v, *d = (int *)p;
        for (; n != 0; n--)
            *d++ = *s++;
    }
    return 0;
}
