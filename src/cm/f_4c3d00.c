/* 0x4c3d00, adds n values of size bytes at field (a plain copy) */

int cm_AddFieldValues(unsigned char size, unsigned char n, unsigned short field, void *v)
{
    unsigned char *p;
    int o;
    unsigned int k, len;

    o = ai_Unka68c08;
    if (size != 1 && size != 2 && size != 4)
        return 1;
    k = size * n;
    len = k + 4 + o;
    if (len > 0x76)
        return 1;
    p = &ai_Unka68c20.buf[o];
    ai_Unka68c20.n++;
    ai_Unka68c08 = len;
    p[0] = size;
    p[1] = n;
    *(unsigned short *)(p + 2) = field;
    memcpy(p + 4, v, k);
    return 0;
}
