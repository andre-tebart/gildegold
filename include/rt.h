struct D3SM;
struct D2SURF;
struct VFSFILE;
struct D3OBJECT;
struct D3VECTOR;
/* Declarations of rt.c (0x4bd950..): routing (module tag "rt_point", "rt_load", "misc\routing_*" dialogs). Names
   are ours. */
#ifndef RT_H
#define RT_H

typedef struct RTPOINT {                /* 0x40 bytes ("rt_point"), a route point in a list of at most 8 */
    int cartId;                         /* 0x00 */
    int buildingId;                     /* 0x04, -1 when new */
    int loadSlots[6];                   /* 0x08, three {good, amount} pairs */
    int unloadSlots[6];                 /* 0x20, three {good, amount} pairs */
    struct RTPOINT *prev;               /* 0x38 */
    struct RTPOINT *next;               /* 0x3c */
} RTPOINT;

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
extern char *d2_Objs;                   /* 0xf8f080, 0x2ec bytes each; @original: d2.h declares D2OBJ *; rt_SetMarkPic (0x4bf360, byte) adds byte offsets to it (d2_Objs + obj * 0x2ec + 8), the D2OBJ fields need d2.h, which rt.c cannot include */
extern char *ai_BTypes;                 /* 0x7e3f30, ai.h: AIBTYPE * (0x24d bytes each, the kind at 0) */
extern unsigned int ai_Unk889400[];     /* 0x889400, the goods of the last routing_choose_good list (0 ends) */
extern unsigned short ai_Unk76fdc4;     /* 0x76fdc4, their number */

extern int sv_PopRequest;                /* 0x6f22a0, closes the dialog */
extern int d2_ClickedId;                /* 0xf8a668 */
extern int d2_PointerObj;                /* 0x5fe710, focused object */
extern int d2_LastWindow;                /* 0x6ed938, window created last */

extern RTPOINT *rt_EditRoute;           /* 0x6f0758, the route being edited */











extern char *cm_FieldObject;              /* 0xa68d2c, the carts (AI update offsets) */
extern int rt_PickedBuilding;                /* 0x6f0754, the building shown */




extern int rt_Unk6f074c;                /* 0x6f074c, the saved screen of 0x4bf5b0 */
extern char *ai_Unk6f26ac;              /* 0x6f26ac, the screen surface */
extern int ai_Unk62c26c;                /* 0x62c26c */
extern struct D2INPUT *d2_FocusedInput;      /* 0x6ed9dc (as d2.h / cm.h) */
extern char ai_PlayerSlots[][0x2f4];      /* 0x7e3f40, 0x2f4 bytes per player (as cm.h / unk_4b6480.h) */
extern char ch_EmptyName[];             /* 0x631090 */
extern int rt_MarkGroup;                /* 0x602a90, a d2 radio group (-1: none) */

void * mem_Alloc(int size, char *tag);                                            /* 0x4223c0, mem.c */
void mem_Free(void *p);                                                          /* 0x422620, mem.c */
char *gm_FindBuildingById(int id);                                                      /* 0x502020, ai.h: AIBLD * */
void d3_PointToWorld(struct D3OBJECT *obj_, struct D3VECTOR *in, struct D3VECTOR *out);                               /* 0x458a20, d3.h: D3 types */
char d3_GetHeightAt(struct D3SM *h, struct D3VECTOR *pos, int *px, int *pz, float *py);           /* 0x45f170, ch.h: D3SM *, D3VECTOR * */
void ai_Unk5a6b70(float *out, int x, int z);                                     /* 0x5a6b70 */
int d2_CreateRadioGroup(int n, ...);                                             /* 0x46c490 */
void d2_RadioAdd(int g, int obj);                                                /* 0x46c560 */
int d2_GetObjValue(int obj);                                                       /* 0x475980 */
int d2_WinOpenPic(short x, short y, int pic, int win);                            /* 0x4731d0 */
int d2_OpenForm(int a, int b, char *name);                                      /* 0x4741f0, d2.c */
void d2_CentreViewWindows(int view);                                                        /* 0x475600 */
int d2_SetForm(int view, int n);                                                   /* 0x475f70 */
int d2_GetWinObj(int view, int idx, int k);                                        /* 0x475b80 */
void d2_CloseView(int view);                                                        /* 0x4758f0 */
int io_PrintText(char *fmt, ...);                                                    /* 0x515960, a text */
char * gm_Query(void *list, int n, ...);                                               /* 0x4ffe90 */
unsigned short * gm_QueryNext(void);                                                        /* 0x4ffcc0 */
int gm_BuildKindObjects(char *b);                                                      /* 0x500b90 */
int sv_RunFrame(int cb, unsigned mask);                                              /* 0x5309a0, the dialog loop */
void d2_RadioSelect(int g, int sel);                                               /* 0x46c630 */
void d2_ClearInput(void);                                                         /* 0x476b70 */
int d2_GetViewWin(int view, int i);                                               /* 0x475ff0 */
int d2_RemoveWindowObjects(int win, int all);                                              /* 0x472d20 */
int sv_Unk528800(int a, int y, int text);                                        /* 0x528800, a tab button */
int gm_ListStock(short *l, int good);                                               /* 0x50ab50 */
int d2_WinOpenInputIcon(int x, int y, int icon, unsigned char flags, int win);   /* 0x469ee0 */
int d2_WinOpenSlider(int x, int y, int len, int p4, int p5, int pic, unsigned int flags, int win); /* 0x46a960 */
void d2_SetObjFont(int obj, int font);                                            /* 0x4760b0 */
void d2_RemoveObj(int obj);                                                      /* 0x46e710 */
int gm_FindObjectInst(int id);                                                      /* 0x4fe980, a cart type (unsigned short kind 0x134..) */
void d2_EnableViewObjects(int view, int on);                                             /* 0x4754d0 */
void sv_ButtonRow(int f, int y, int *out, int n, ...);                         /* 0x52e350, a row of n buttons */
int d2_CreateWinText(short x, short y, char *text, int win);                         /* 0x473650 */
void d2_SetObjEnabled(int obj, int on);                                              /* 0x475e40 */
void d2_SetObjValue(int obj, int a, int b, int c);                                 /* 0x475cb0, @original: d2.c takes a 5th parameter (value2); 139 original call sites push 4 arguments, 18 push 5; rt.c's push 4 */
#define ai_Unk52e350x ((void (*)(int, int, int *, int, int, int, int))sv_ButtonRow) /* 3 button texts */
void d2_SetCaption(int obj, char *text);                                            /* 0x473540 */
void d2_SetSliderFlags(int obj, int a);                                               /* 0x46bdd0 */
void d2_SetList(int id, int n, ...);                                          /* 0x46bca0 */
void d2_SetObjPic(int obj, int pic);                                             /* 0x469250 */
void cm_BeginFieldBlock(char *o, int id);                                              /* 0x4c3ba0, ai.h: void * */
int cm_AddFieldValue(unsigned char size, unsigned char n, unsigned short field, void *v);                               /* 0x4c3d90 */
void cm_SendFieldValues(void);                                                         /* 0x4c49e0 */
void ai_Unk5a78a0(int *m);                                                       /* 0x5a78a0, town map window: open */
void ai_Unk5a6cb0(int *m);                                                       /* 0x5a6cb0 */
void ai_Unk5a7cd0(int *m);                                                       /* 0x5a7cd0, per frame */
void ai_Unk5a82f0(int *m);                                                       /* 0x5a82f0, close */
struct D2SURF * d2_CopySurface(struct D2SURF *surf);                                                    /* 0x479780 */
void d2_BlitPart(int sx, int sy, int w, int h, struct D2SURF *src, int x, int y, struct D2SURF *dst);  /* 0x479480 */
int d2_FreeSurface(struct D2SURF *s);                                                        /* 0x478fc0 */
void sv_Unk52e4b0(int win, int a, int *out, int n, int t1, int t2, int t3);      /* 0x52e4b0 */
void ai_Unk4c6320(char *c);                                                      /* 0x4c6320 */
void ai_Unk4c62a0(char *o, char *pts);                                          /* 0x4c62a0 */
int gm_BuildingGoodsList(char *b, unsigned *out, int kind);                                   /* 0x5098a0 */
unsigned short * gm_Unk50a050(unsigned short *c);                                                    /* 0x50a050, the cart's owner */
int gm_StallGoodsList(char *c, unsigned *out);                                           /* 0x50aa00 */
int gm_FindMarketObject(int id);                                                    /* 0x500ec0 */
int gm_ListAmount(short *l, char *o);                                   /* 0x50aaf0 */
int gm_FreeCapacity(short *st, int good, int n);                                     /* 0x50ab80 */
float gm_GoodsPrice(unsigned id, unsigned char p);                                   /* 0x5089c0, a price */
int gm_Unk509ee0(char *c, char p);                                      /* 0x509ee0, ai.h: AIPER * */
void cm_SendExSellObject(int a, int b, short c, int d, unsigned char e, int f);         /* 0x4c4880, cm.c */
int io_TownDisplayName(char *name, char *out);                                    /* 0x5142e0 */
void txt_Format(char *buf, int id, ...);                                       /* 0x5176d0 */
int he_ShowNews(int pid, int fl, char *text, int unused, int ev, char *text2);           /* 0x53dda0 */
unsigned vfs_fwrite(void *buf, int size, int n, struct VFSFILE *f);                               /* 0x430e30 */
int ls_Read(void *p, int size, int n, int f);                               /* 0x51c920, ls.c: true when the fread returned a positive count */
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
int rt_Unk4bdb30(char *win, int b, RTPOINT *list, RTPOINT *p);
void rt_MarkValue(RTMARK *m);
void rt_ReadMarkValues(void);
int rt_Unk4bdcd0(unsigned char *b, int x, int def);
int rt_Unk4be180(int x, RTPOINT *p);
unsigned int rt_Unk4be3b0(char *b, int x, unsigned int def);
int rt_Unk4be790(unsigned short *bt, int a, int good, int k, int x, int y, int win, int *out);
void rt_RemoveControls(int *out);
void rt_InitControls(int *out);
void rt_Unk4be940(RTPOINT *p, int *win);
void rt_Unk4bf080(char *c, char *win);
void rt_SetMarkPic(RTMARK *m);
void rt_UpdateMarkPics(void);
int rt_FindMark(RTPOINT *p);
void rt_RemoveMark(int i);
void rt_HighlightRoute(RTPOINT *p, char *m);
void rt_Unk4bf5b0(char *c);
void rt_UnloadAtPoint(char *c, int pl);
void rt_Unk4bfc60(char *c, char pl);
int rt_SaveRoutes(void *f);
int rt_LoadRoutes(void *f);

#endif
