/* 0x46ca60, one more user of a picture (a frame counts for its base picture) */

void d2_AddRefcount(int pic)
{
    D2PIC *p = &d2_Pics[pic];

    if ((p->type == 5 || p->type == 8) && p->offset == 0)
        pic = p->base;
    d2_Pics[pic].refs++;
    d2_Pics[pic].lastuse = d3_Time;
}
