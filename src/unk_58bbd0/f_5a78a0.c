/* 0x5a78a0, the map window's open (317 B): the previous screen 0x6ed930 is saved (+0x27a8), the window
   "Misc\Landkarte2" 0x62c274 (0x4741f0) takes the screen 0x6f26ac, the map screen 0x472aa0 (0x4a, 0x4c, 0x206,
   0x164) is stored at +8 and 0x6ed938, the screen's +0x38c is set; the control bar 0x5a6c40 (the screen, the id
   +0x16, else 0x727 (flag 4) / 0x72b (2) / 0x723 (1 or 8 with the title at +0x5c) / 0x71f), then the form 1 with
   the title 0x515960 and the form 2 with the flag 0x10 */

int d2_OpenForm(); void d2_PlaceMainWindow(); void d2_RaiseWindow(); int io_PrintText(char *fmt, ...);
int d2_SetForm(int view, int n); int d2_CreateChildWindow(short x, short y, short w, short h, unsigned int flags, int win);
void d2_CloseView(int view); void d2_RemoveObj(); void d2_ScrollWindowBy();
int ai_Unk5890c0(); int d2_CreatePicObj(); unsigned char *ai_Unk5a84d0();

void ai_Unk5a78a0(int *m_)
{
    char *p = (char *)m_;
    int view;
    int screen;
    int id;

    *(int *)(p + 0x27a8) = *(int *)((unsigned)&d2_BackgroundTile);
    *(int *)((unsigned)&d2_BackgroundTile) = *(int *)((unsigned)&ai_Unk6f26ac);
    view = d2_OpenForm(0, 0, (char *)(unsigned)&full_data_62c274);
    m_[0] = view;
    d2_PlaceMainWindow(view, 3);
    d2_SetForm(view, 0);
    screen = d2_CreateChildWindow(0x4a, 0x4c, 0x206, 0x164, 0, *(int *)((unsigned)&d2_LastWindow));
    m_[2] = screen;
    *(int *)((unsigned)&d2_LastWindow) = screen;
    *(int *)(*(int *)((unsigned)&d2_LastWindowPtr) + 0x38c) = 1;
    d2_SetForm(view, 0);
    id = m_[0x16];
    if (id == 0) {
        if (*(unsigned char *)(p + 4) & 4)
            id = 0x727;
        else if (*(unsigned char *)(p + 4) & 2)
            id = 0x72b;
        else if ((*(unsigned char *)(p + 4) & 1) && (char *)(p + 0x5c) != 0)
            id = 0x723;
        else if ((*(unsigned char *)(p + 4) & 8) && (char *)(p + 0x5c) != 0)
            id = 0x723;
        else
            id = 0x71f;
    }
    ai_Unk5a6c40(screen, id);
    if ((char *)(p + 0x5c) != 0) {
        d2_SetForm(view, 1);
        d2_RaiseWindow(screen);
        io_PrintText((int)(p + 0x5c));
    }
    if (*(unsigned char *)(p + 4) & 0x10) {
        d2_SetForm(view, 2);
        d2_RaiseWindow(screen);
    }
}
