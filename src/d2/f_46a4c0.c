/* 0x46a4c0, opens a slider: flags 1 vertical, 2 horizontal; pic is the first of the icons for its end caps,
   body and thumb */

int d2_OpenSlider(int x, int y, int len, int p4, int p5, int pic, unsigned int flags)
{
    int obj;
    int i, cnt;
    int mw, mh;
    D2PIC *ic0, *ic1, *ic3;
    D2PICHDR *frame;
    int desc[16];

    obj = d2_NewObject();
    ic1 = &d2_Pics[pic + 1];
    ic0 = &d2_Pics[pic];
    ic3 = &d2_Pics[pic + 3];
    if (flags & 1) {
        cnt = len / ic1->h;
        if (len % ic1->h)
            cnt++;
        mh = ic1->h * cnt + ic0->h * 2;
        mw = max(ic1->w, ic0->w);
        mw = max(mw, ic3->w);
        d2_Objs[obj].w = mw;
        d2_Objs[obj].h = ic0->h * 2 + len;
    } else if (flags & 2) {
        cnt = len / ic1->w;
        if (len % ic1->w)
            cnt++;
        mw = ic1->w * cnt + ic0->w * 2;
        mh = max(ic1->h, ic0->h);
        mh = max(mh, ic3->h);
        d2_Objs[obj].w = len + ic0->w * 2;
        d2_Objs[obj].h = mh;
    }
    d2_Objs[obj].type = 0x45;
    d2_Objs[obj].interactive = 1;
    d2_Objs[obj].x = x;
    d2_Objs[obj].y = y;
    d2_Objs[obj].pic = 0;
    d2_Objs[obj].u.s.value = p4;
    d2_Objs[obj].u.s.max = p5;
    d2_Objs[obj].u.s.flags = flags;
    d2_Objs[obj].u.s.min = p4;
    d2_Objs[obj].u.s.len = len;
    d2_Objs[obj].u.s.value2 = p5 / 2;
    d2_Objs[obj].u.s.firstPic = pic;
    d2_Objs[obj].scale = 1;
    d2_Objs[obj].clipy0 = 0;
    d2_Objs[obj].clipy1 = d2_ScreenH;
    d2_Objs[obj].clipx0 = 0;
    d2_Objs[obj].clipx1 = d2_ScreenW;
    memset(desc, 0, sizeof desc);
    desc[1] = mw * 3;
    desc[2] = mh * 3;
    d2_Objs[obj].u.s.img = d2_MakeSurface(desc, 0xb);
    if (!d2_Objs[obj].u.s.img) {
        err_Message("d2_OpenSlider:Could not create surface...");
        return -1;
    }
    frame = d2_GetPic(pic);
    d2_Objs[obj].data = frame;
    d2_ClearSurface(d2_Objs[obj].u.s.img);
    d2_LockSurface(d2_Objs[obj].u.s.img);
    for (i = 0; i < cnt; i++) {
        if (flags & 2) {
            d2_DrawShape(ic1->w * i, 0, d2_Objs[obj].u.s.img, frame, 2);
            d2_DrawShape(ic1->w * i, ic1->h, d2_Objs[obj].u.s.img, frame, 1);
            d2_DrawShape(ic1->w * i, ic1->h * 2, d2_Objs[obj].u.s.img, frame, 4);
        } else if (flags & 1) {
            d2_DrawShape(0, ic1->h * i, d2_Objs[obj].u.s.img, frame, 2);
            d2_DrawShape(ic1->w, ic1->h * i, d2_Objs[obj].u.s.img, frame, 1);
            d2_DrawShape(ic1->w * 2, ic1->h * i, d2_Objs[obj].u.s.img, frame, 4);
        }
    }
    d2_UnlockSurface(d2_Objs[obj].u.s.img);
    return obj;
}
