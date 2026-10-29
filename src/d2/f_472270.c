/* 0x472270, creates the two scroll buttons of a window. The body reaches the window through D2WOP(o), the
   window at the byte offset o = win * 0x3c4: the original computes the address as lea/shl from the byte
   offset, so the offset form is what the bytes need (the index form d2_Wins[win] loses the match:
   238/304 bytes differ). */

void d2_CreateScrollButtons(x, y, win, obj, pic)
int x;
int y;
int win;
int obj;
int pic;
{
    int i;
    int o = win * 0x3c4;

    if (x == -1 && y == -1) {
        x = D2WOP(o)->w - 0x20;
        y = D2WOP(o)->h - 0x20;
    }
    i = d2_WinOpenPic(x, y, pic, win);
    D2WOP(o)->btndown = i;
    d2_Objs[i].winIndex = obj;
    d2_Objs[D2WOP(o)->btndown].style = 3;
    i = d2_WinOpenPic(x - 0x20, y, pic + 1, win);
    D2WOP(o)->btnup = i;
    d2_Objs[i].winIndex = obj;
    d2_Objs[D2WOP(o)->btnup].style = 3;
    D2WOP(o)->scrollY = 0;
    d2_Wins[obj].visible = 0x30;
}
