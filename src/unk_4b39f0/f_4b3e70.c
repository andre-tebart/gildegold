/* 0x4b3e70, the dialog "special\CHOOSE_MISSION": lists the mission types of kind, offers the one picked
   (0x4b3d50); returns its type (0xff: none) */

typedef struct G13OBJ {                 /* 0x2ec bytes, d2.h's D2OBJ (d2.h conflicts with am.h's vfs views) */
    char pad000[0x2e8];
    void *user;                         /* 0x2e8 (D2OBJ: pad2e8) */
} G13OBJ;

typedef struct G13RADIO {               /* 0x8c bytes, d2.h's D2RADIO */
    int unk00;
    int n;                              /* 0x04 */
    int sel;                            /* 0x08, -1 = none */
    int objs[0x20];                     /* 0x0c */
} G13RADIO;

extern struct D2OBJ *d2_Objs;                 /* 0xf8f080 */
extern G13RADIO d2_Radios[8];           /* 0xfa5aa0 */
extern int d2_PointerObj;                /* 0x5fe710, focused object */
extern int d2_LastWindow;                /* 0x6ed938 */
extern unsigned char kb_Last;           /* 0x1468410 */
extern int sv_FrameMask;
void d2_CreateScrollButtons(int x, int y, int win, int obj, int pic);                      /* 0x472270 */
int d2_GetWinObj(int view, int idx, int k);                                      /* 0x475b80 */
int d2_CreateRadioGroup(int n, ...);                                             /* 0x46c490 */
void d2_RadioAdd(int g, int obj);                                                /* 0x46c560 */
void d2_RadioClear(int g);                                                       /* 0x46c600 */
void d2_MenuNavigate(int g);                                                        /* 0x46c6c0 */

int ai_ChooseMissionType(int kind)
{
    int chosen, view, win, g, o;
    G13TYPE *t;

    chosen = 0xff;
    view = d2_OpenForm(0, 0, "special\\CHOOSE_MISSION");
    d2_CentreViewWindows(view);
    d2_SetForm(view, 1);
    io_PrintText((char *)0x1ace);
    d2_SetForm(view, 2);
    win = d2_LastWindow;
    d2_SetForm(view, 0);
    d2_CreateScrollButtons(-1, -1, d2_LastWindow, win, 0x767);
    d2_SetForm(view, 2);
    g = d2_CreateRadioGroup(0);
    for (t = ai_FindMissionType(kind, 0); t != 0; t = ai_FindMissionType(kind, t)) {
        o = d2_GetWinObj(view, 2, io_PrintText("%ia[%s]$N", t->text));
        d2_ObjsV[o].user = t;
        d2_RadioAdd(g, o);
    }
    while (sv_RunFrame((void (*)(void))ai_ChooseMissionType, 0x1c6)) {
        d2_MenuNavigate(g);
        if (dm_L.rup != 0 || kb_Last == 1 || d2_ClickedId == 1)
            sv_PopRequest = 1;
        if (d2_ClickedId != -1 && d2_Radios[g].sel != -1
            && d2_PointerObj == d2_Radios[g].objs[d2_Radios[g].sel]) {
            t = (G13TYPE *)d2_ObjsV[d2_Radios[g].objs[d2_Radios[g].sel]].user;
            if (ai_Unk4b3d50(t->type, sv_FrameMask)) {
                chosen = t->type;
                sv_PopRequest = 1;
            }
        }
    }
    d2_RadioClear(g);
    d2_CloseView(view);
    return chosen;
}
