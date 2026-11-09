struct D3SM;
struct D2SURF;
struct VFSFILE;
struct D3OBJECT;
struct D3VECTOR;
/* Declarations of rt.c (0x4bd950..): routing (module tag "rt_point", "rt_load", "misc\routing_*" dialogs). Names
   are ours. */
#ifndef RT_H
#define RT_H

#include "rt_types.h"                /* RTPOINT */
#include "d2_types.h"                /* D2OBJ (d2_Objs) */

typedef struct RTINV {                  /* 0xb8 bytes, filled by 0x5098a0 / 0x50aa00 */
    char pad00[0xb4];
    char *stock;                        /* 0xb4 */
} RTINV;

typedef struct RTMARK {                 /* 8 bytes, rt_Marks[8] */
    int obj;                            /* 0x00, a d2 picture object (-1: none) */
    RTPOINT *p;                         /* 0x04 */
} RTMARK;

extern RTMARK rt_Marks[8];          /* 0xf812a0 */
extern int rt_MarkCount;                /* 0x6f0750, the marks used */
extern unsigned char ai_OwnPlayer;      /* 0x6f1bc5, the player */
#ifndef DATA_VIEWS_H
extern char ai_Unkfd7c50[];            /* 0xfd7c50 (as cm.h) */
#endif
extern D2OBJ *d2_Objs;  /* 0xf8f080, 0x2ec bytes each (d2.c); this header's unit adds byte offsets through d2_ObjsC */

extern int d2_ClickedId;                /* 0xf8a668 */
extern int d2_LastWindow;                /* 0x6ed938, window created last */

extern RTPOINT *rt_EditRoute;           /* 0x6f0758, the route being edited */











extern int rt_PickedBuilding;                /* 0x6f0754, the building shown */




extern int rt_Unk6f074c;                /* 0x6f074c, the saved screen of 0x4bf5b0 */
extern char *rt_MapSurface;              /* 0x6f26ac, the screen surface */
extern int ai_MapScrollEnabled;                /* 0x62c26c */
extern struct D2INPUT *d2_FocusedInput;      /* 0x6ed9dc (as d2.h / cm.h) */
extern char ch_EmptyName[];             /* 0x631090 */
extern int rt_MarkGroup;                /* 0x602a90, a d2 radio group (-1: none) */

void d3_PointToWorld(struct D3OBJECT *obj_, struct D3VECTOR *in, struct D3VECTOR *out);                               /* 0x458a20, d3.h: D3 types */
char d3_GetHeightAt(struct D3SM *h, struct D3VECTOR *pos, int *px, int *pz, float *py);           /* 0x45f170, ch.h: D3SM *, D3VECTOR * */
int d2_CreateRadioGroup(int n, ...);                                             /* 0x46c490 */
void d2_RadioAdd(int g, int obj);                                                /* 0x46c560 */
int d2_GetObjValue(int obj);                                                       /* 0x475980 */
int d2_WinOpenPic(short x, short y, int pic, int win);                            /* 0x4731d0 */
int d2_OpenForm(int a, int b, char *name);                                      /* 0x4741f0, d2.c */
void d2_CentreViewWindows(int view);                                                        /* 0x475600 */
int d2_SetForm(int view, int n);                                                   /* 0x475f70 */
int d2_GetWinObj(int view, int idx, int k);                                        /* 0x475b80 */
void d2_CloseView(int view);                                                        /* 0x4758f0 */
void d2_RadioSelect(int g, int sel);                                               /* 0x46c630 */
void d2_ClearInput(void);                                                         /* 0x476b70 */
int d2_GetViewWin(int view, int i);                                               /* 0x475ff0 */
int d2_RemoveWindowObjects(int win, int all);                                              /* 0x472d20 */
int d2_WinOpenInputIcon(int x, int y, int icon, unsigned char flags, int win);   /* 0x469ee0 */
int d2_WinOpenSlider(int x, int y, int len, int p4, int p5, int pic, unsigned int flags, int win); /* 0x46a960 */
void d2_SetObjFont(int obj, int font);                                            /* 0x4760b0 */
void d2_RemoveObj(int obj);                                                      /* 0x46e710 */
void d2_EnableViewObjects(int view, int on);                                             /* 0x4754d0 */
int d2_CreateWinText(short x, short y, char *text, int win);                         /* 0x473650 */
void d2_SetObjEnabled(int obj, int on);                                              /* 0x475e40 */
void d2_SetObjValue(int obj, int a, int b, int c, int d);                         /* 0x475cb0, d2.c: the 5th parameter (value2) is pushed by 18 of the original call sites; rt.c's push 4 (d2_SetObjValue4 in its head) */
#define ai_Unk52e350x ((void (*)(int, int, int *, int, int, int, int))sv_ButtonRow) /* 3 button texts */
void d2_SetCaption(int obj, char *text);                                            /* 0x473540 */
void d2_SetSliderFlags(int obj, int a);                                               /* 0x46bdd0 */
void d2_SetList(int id, int n, ...);                                          /* 0x46bca0 */
void d2_SetObjPic(int obj, int pic);                                             /* 0x469250 */
struct D2SURF * d2_CopySurface(struct D2SURF *surf);                                                    /* 0x479780 */
void d2_BlitPart(int sx, int sy, int w, int h, struct D2SURF *src, int x, int y, struct D2SURF *dst);  /* 0x479480 */
int d2_FreeSurface(struct D2SURF *s);                                                        /* 0x478fc0 */
void d2_RadioClear(int g);                                                      /* 0x46c600, d2.h: D2RADIO * */

RTPOINT *rt_AddPoint(RTPOINT *p);
int rt_RemovePoint(RTPOINT *p);
void rt_FreePoints(RTPOINT *p);
RTPOINT *rt_CopyPoints(RTPOINT *p);
int rt_PointIndex(RTPOINT *p, RTPOINT *q);
RTPOINT *rt_PointAt(RTPOINT *p, int n);
int rt_PointCount(RTPOINT *p);
void rt_ClearMarkGroup(void);
void rt_BuildMarkGroup(void);
struct LCMAPWIN;
int rt_PutRouteMark(struct LCMAPWIN *win, int cart, RTPOINT *list, RTPOINT *p);
void rt_MarkValue(RTMARK *m);
void rt_ReadMarkValues(void);
struct AIBLD;
int rt_ChooseGoodsDialog(struct AIBLD *b, int x, int def);
int rt_ChooseRouteGood(int x, RTPOINT *p);
unsigned int rt_ChooseTabGood(struct AIBLD *b, int x, unsigned int def);
int rt_OpenSlotControls(unsigned short *bt, int a, int good, int k, int x, int y, int win, int *out);
void rt_RemoveControls(int *out);
void rt_InitControls(int *out);
void rt_Unk4be940(RTPOINT *p, int *win);
void rt_Unk4bf080(char *c, char *win);
void rt_SetMarkPic(RTMARK *m);
void rt_UpdateMarkPics(void);
int rt_FindMark(RTPOINT *p);
void rt_RemoveMark(int i);
void rt_HighlightRoute(RTPOINT *p, char *m);
void rt_EditCartRoute(char *c);
void rt_UnloadAtPoint(char *c, int pl);
void rt_LoadAtPoint(char *c, char pl);
int rt_SaveRoutes(void *f);
int rt_LoadRoutes(void *f);

#endif
