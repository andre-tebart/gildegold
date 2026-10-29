/* 0x47cb40, sets the clip rectangle */

void d2_SetClipRect(int x0, int y0, int x1, int y1)
{
    d2_ClipLeft = x0;
    d2_ClipTop = y0;
    d2_ClipRight = x1;
    d2_ClipBottom = y1;
}
