/* 0x4758f0, closes a view. The view is reached through D2VOP(view * 0x2ac), the original's byte-offset
   form: the typed index access d2_Views[view] loses the byte match (the offset stays in a register, lessons
   in notes/units/d2.md). */

void d2_CloseView(int view)
{
    D2VIEW *v;
    int i;

    if (view <= 0x30 || view >= 1) {
        v = D2VOP(view * 0x2ac);
        if (D2VOP(view * 0x2ac)->active) {
            for (i = D2VOP(view * 0x2ac)->nwins - 1; i >= 0; i--)
                d2_RemoveWindow(d2_Views[view].wins[i]);
            if (v->viewSurf)
                d2_FreeSurface((void *)v->viewSurf);
            if (v->texSurf)
                d2_FreeSurface((void *)v->texSurf);
            memset(v, 0, sizeof(D2VIEW));
        }
    }
}
