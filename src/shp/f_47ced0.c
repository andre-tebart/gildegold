/* 0x47ced0, stores three parameters */

void d2_SetBlitParams(void *dst, void *src, void *mask, short w, short h, short n)
{
    d2_BlitH = (unsigned short)h;
    d2_BlitW = (unsigned short)w;
    d2_BlitN = (unsigned char)n;
}
