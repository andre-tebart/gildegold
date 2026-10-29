/* 0x472c80, size of the frame pieces of a window (0 if it has no frame) */

void d2_GetFrameSize(win, w, h)
D2WIN *win;
int *w;
int *h;
{
    void *pic;

    if (win->flags & 4) {
        pic = d2_GetPic(win->frame);
        *h = d2_GetFrame(pic, 4)->h;
        *w = d2_GetFrame(pic, 6)->w;
        return;
    }
    *h = 0;
    *w = 0;
}
