/* 0x4741a0, raises the windows of view v to the top of the draw order */

void d2_RaiseViewWindows(int v)
{
    int i;

    for (i = 0; i < d2_Views[v].nwins; i++)
        d2_RaiseWindow(d2_Views[v].wins[i]);
}
