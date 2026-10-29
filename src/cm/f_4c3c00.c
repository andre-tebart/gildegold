/* 0x4c3c00, adds n values of size bytes at field as differences to the object's values */

int cm_AddFieldDelta(int size, int n, short field, void *v)
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
        unsigned char *s = (unsigned char *)v, *b = (unsigned char *)cm_FieldObject + field;
        for (; n != 0; n--)
            *p++ = *s++ - *b++;
    } else if (size == 2) {
        short *s = (short *)v, *b = (short *)(cm_FieldObject + field), *d = (short *)p;
        for (; n != 0; n--)
            *d++ = *s++ - *b++;
    } else if (size == 4) {
        int *s = (int *)v, *b = (int *)(cm_FieldObject + field), *d = (int *)p;
        for (; n != 0; n--)
            *d++ = *s++ - *b++;
    }
    return 0;
}
