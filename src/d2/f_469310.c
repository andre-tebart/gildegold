/* 0x469310, draws the frame of a dialog with its caption number */

void d2_DrawFrame(D2INPUT *dlg, void *arg)
{
    D2OBJ *o = &d2_Objs[dlg->obj];
    void *pic = d2_GetPic(d2_FramePic);
    D2FRAME *f0 = d2_GetFrame(pic, 0);
    D2FRAME *f1 = d2_GetFrame(pic, 1);
    D2FRAME *f3;
    int x, y;
    char buf[24];

    d2_GetFrame(pic, 2);
    f3 = d2_GetFrame(pic, 3);
    x = o->x - 0x20;
    y = (dlg->frameY - f0->h) - 100;
    d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
    d2_DrawShape(x, y, arg, pic, 0);
    d2_DrawShape(x, f0->h + 100 + y, arg, pic, 1);
    d2_DrawShape(x, f0->h + y, arg, pic, 2);
    d2_AddDirty(x - 0xc, y, 0x40, f1->h + 100 + f0->h);
    y = dm_L.y - (f3->h >> 1);
    x += (f0->w >> 1) - (f3->w >> 1);
    d2_DrawShape(x, y, arg, pic, 3);
    d2_PointerX = x;
    d2_PointerY = y;
    if (dlg->value < 100000 && -100000 < dlg->value) {
        d2_SetFont(d2_DefaultFont + 1);
        itoa(dlg->value, buf, 10);
        d2_DrawTextBox(d2_PointerX, d2_PointerY, f3->w, f3->h, arg, buf, 8);
    }
}
