/* 0x46cac0, one user less */

void d2_SubRefcount(int pic)
{
    D2PIC *p = &d2_Pics[pic];
    int *r;

    if ((p->type == 5 || p->type == 8) && p->offset == 0)
        pic = p->base;
    r = &d2_Pics[pic].refs;
    if (*r > 0)
        (*r)--;
    else
        err_Message("d2_SubRefcount: invalid refcount!");
}
