/* 0x472760, scrolls the content of a window. The body reads/writes the window through D2WOP(o), the window
   at the byte offset o = win * 0x3c4, eleven times - that form is what the bytes need (the index form
   D2WOP(o) -> d2_Wins[win] costs 19/160 bytes); one access, d2_Wins[win].scrollRemY += t, is the index form
   and is kept for the same reason (comment in the body). */

void d2_ScrollWindowBy(a, b, win)
int a;
int b;
int win;
{
    int o = win * 0x3c4;
    int t;

    if (D2WOP(o)->scrollY + D2WOP(o)->scrollRemY > 0 && b < 0) {
        t = -(D2WOP(o)->scrollY + D2WOP(o)->scrollRemY);
        if (b > t)
            t = b;
        D2WOP(o)->scrollRemY = t;
    } else if (D2WOP(o)->h + D2WOP(o)->scrollY + D2WOP(o)->scrollRemY < D2WOP(o)->contentHeight && b > 0) {
        t = D2WOP(o)->contentHeight - D2WOP(o)->h - D2WOP(o)->scrollY - D2WOP(o)->scrollRemY;
        if (b < t)
            t = b;
        d2_Wins[win].scrollRemY += t;     /* the index form: no reuse of the loaded scrollRemY, add [mem] */
    }
    D2WOP(o)->scrollRemX += a;
}
