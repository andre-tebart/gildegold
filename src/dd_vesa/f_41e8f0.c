/* 0x41e8f0, draws a line (0x41e760) into the drawing surface when the lock succeeds */

void dd_Line(int x0, int y0, int x1, int y1, int color)
{
    if (dd_Lock()) {
        dd_DrawLine16(x0, y0, x1, y1, color);
        dd_Unlock();
    }
}
