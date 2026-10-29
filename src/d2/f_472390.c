/* 0x472390, creates a window; returns its index, -1 if there is no free one, 0 if a surface could not be made @deep draft: 638/976 bytes differ, 121 asm lines after this session's two meaning fixes (win->w = w, the surface desc[1] = w; 740 bytes / 167 lines before), hunks code 121 / regs 58. Read against the original 0x472390..: the free-window scan (counter/pointer in ebp/eax vs eax/ecx), the index scaling lea chain, the flags register (ours ecx/bh vs original ebx), the -1 materialisation (or ebx,-1 vs or eax,-1) and the y+h add order at 472611 are register allocation / scheduling; the 4724a1 desc push order now reads right. Tried: the operand order win->h + win->y -> win->y + win->h at 472611 (identical bytes, reverted). Q2 left, no writer anywhere in src/ (name gaps): D2WIN.unk268 (written 2 here, compared == 1 at d2.c:1967), D2WIN.unk274 (written -1 here only). The two child-object slots are cornerObj / extraObj (0x3bc / 0x3c0). Named this session: d2_CreateWindow (from d2_CreateWindow), d2_LastWindow / d2_LastWindowPtr (d2_LastWindow / d2_LastWindowPtr, the window created last). */

int d2_CreateWindow(x, y, w, h, flags)
short x;
short y;
short w;
short h;
unsigned int flags;
{
    int desc[16];
    int i, j, ob;
    D2WIN *win;
    int *p;
    void *surf;

    d2_WindowsOpen++;
    if (d2_WindowsOpen > 0x60)
        d2_WindowsOpen = 1;
    j = 0;
    p = &d2_Wins[0].active;
    while (1) {
        if (!*p)
            break;
        p += 0xf1;
        j++;
        if ((int)p >= (int)&d2_Wins[0x60].active)
            return -1;
    }
    if (j == -1)
        return j;
    i = j;
    win = &d2_Wins[i];
    win->y = y;
    win->x = x;
    win->w = w;
    win->h = h;
    win->flags = flags;
    win->ml = 4;
    win->mr = 4;
    win->mt = 4;
    win->mb = 4;
    win->spr = 0;
    win->scrollStep = 4;
    win->visible = 0;
    if (flags & 4)
        win->frame = d2_WinBorderPic;
    if (flags & 0x200) {
        win->contentHeight = h;
        win->scrollY = -h;
        win->appliedScrollY = -h;
    }
    if (flags & 1)
        win->alpha = 0.65f;
    if (flags & 0x1000) {
        memset(desc, 0, sizeof desc);
        desc[1] = w;
        desc[2] = h;
        win->surf0 = d2_MakeSurface(desc, 0xb);
        if (!win->surf0)
            return 0;
    }
    if ((win->flags & 1) && (win->flags & 0x1000)) {
        memset(desc, 0, sizeof desc);
        desc[1] = win->w;
        desc[2] = win->h;
        win->surf1 = d2_MakeSurface(desc, 0xb);
        if (!win->surf1)
            return 0;
    }
    if (win->flags & 0x10000) {
        memset(desc, 0, sizeof desc);
        desc[1] = win->w;
        desc[2] = win->h;
        surf = d2_MakeSurface(desc, 0xb);
        win->surf2 = surf;
        d2_ClearSurface(surf);
        if (!win->surf2)
            return 0;
    }
    ob = d2_NewObject();
    d2_Objs[ob].type = 0x40;
    d2_Objs[ob].x = win->x;
    d2_Objs[ob].y = win->y;
    d2_Objs[ob].w = win->w;
    d2_Objs[ob].h = win->h;
    d2_Objs[ob].pic = i + 0x400;
    d2_Objs[ob].input = i;
    d2_Objs[ob].clipy0 = win->y;
    d2_Objs[ob].clipx1 = win->w + win->x;
    d2_Objs[ob].clipx0 = win->x;
    d2_Objs[ob].clipy1 = win->h + win->y;
    d2_Objs[ob].drawFrames = 2;
    d2_Objs[ob].data = win;
    d2_Objs[ob].win = 0;
    win->parent = ob;
    win->backgroundState = 1;
    win->unk268 = 2;
    win->bgpic = -1;
    win->font = d2_DefaultFont;
    win->scrollBar = -1;
    win->cornerObj = -1;
    win->extraObj = -1;
    win->unk274 = -1;
    win->active = 1;
    win->btndown = -1;
    win->btnup = -1;
    win->bar = -1;
    win->objs = mem_Alloc(0x600, "d2:win_obj");
    win->padl = 0;
    win->padt = 0;
    win->padr = 0;
    win->padb = 0;
    if (flags & 8)
        d2_WinOpenPic(3, 3, d2_ScrollMarkerPic, i);
    if (flags & 0x20) {
        d2_WinOpenPic(win->w - 0xc, 3, d2_ScrollMarkerPic + 1, i);
        d2_WinOpenPic(win->w - 0xc, win->h - 10, d2_ScrollMarkerPic + 2, i);
    }
    if (flags & 0x10)
        win->text = mem_Alloc(0x17d0, "d2_t:win->txt");
    d2_LastWindowPtr = win;
    d2_LastWindow = i;
    return i;
}
