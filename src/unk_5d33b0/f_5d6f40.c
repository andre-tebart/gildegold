/* 0x5d6f40, creates one menu button of an options dialog (84 B): a control of the dialog (picture
   0xbf, the caption texts text1 / text2) at x 0x20 and the row's y, centred (d2_Objs[id].centred,
   d2_SetObjWidth re-centres it on the 300 width). The twin of main_Unk5337f0 (row stride 0x2b).
   Returns the control id. */

#define U_W(a) (*(unsigned short *)(a))

/* d2.h's D2SURF (the 0x40-byte screen surface): the two fields the screen shot reads. Type debt: d2.c
   defines d2_ScreenSurf as void * (D2SURF itself is in d2_types.h). */
typedef struct D2SURFVIEW {
    char pad0[0x10];
    int pitch;                          /* +0x10, in pixels */
    char pad1[8];
    unsigned short *bits;               /* +0x1c, pixels (system memory, or while locked) */
} D2SURFVIEW;
extern D2SURFVIEW *d2_ScreenSurf;       /* 0x6ed924, d2.h: D2SURF * */
int d2_LockSurface(D2SURFVIEW *surf);   /* 0x479020, d2.h: D2SURF *, locks the surface (pixels in bits) */
int d2_UnlockSurface(D2SURFVIEW *surf); /* 0x4790e0, d2.h: D2SURF * */
void *mem_Alloc(int size, char *tag);   /* 0x4223c0, d3.h */
void mem_Free(void *p);                 /* 0x422620, d3.h */
extern unsigned int dd_RMask;           /* 0x1464fa4, dd_vesa.h: the red channel mask of the display */
extern unsigned int dd_GMask;           /* 0x1464fb0, dd_vesa.h */
extern unsigned int dd_BMask;           /* 0x146359c, dd_vesa.h */

int ls_Read(void *buf, int size, int count, int f);   /* 0x51c920, ls.c: vfs_fread items */
void sv_SetStatusText(char *s);   /* 0x52e330, sv.c */
void cm_SendExSysMessage(unsigned char id, void *msg);   /* 0x4c4db0, as in cm.h */
DWORD dd_RGB(unsigned char r, unsigned char g, unsigned char b);   /* 0x41e670, as in dd_vesa.h */
unsigned int util_Hash(unsigned char *buf, unsigned int len);      /* 0x401000, as in bio.h */
void sv_PumpNet(void);                                             /* 0x52fe00, sv.c */
void sv_BroadcastText(unsigned fl, char *t);  /* 0x52fcf0, sv.c: sends t as the network message 0xe and
                                                 shows it until the answer, then clears the status line */
int ls_WriteSaveHeader(int f, unsigned flags); /* 0x5194c0, ls.c: writes the save header (the flag word) */
int ls_ReadSaveHeader(int f, int *h);         /* 0x51cee0, ls.c: reads the save header into h */

/* cm.h's save-request state (the 0x0f the network message carries) and the acknowledgement slots. */
extern int cm_RequestedSaveSlot;        /* 0x6f25c8, cm.h: the save slot the request asked for */
extern char cm_RequestedSaveName[];     /* 0x6f25d0, cm.h: the save name the request carried */
extern int cm_SaveAcks[][2];            /* 0x76fa20, cm.h: 8 slots; [0] = the player's netId, [1] = the
                                           save name hash, -1 when empty (message 0x10 fills the first free) */
extern unsigned char ai_NumPlayers;     /* 0x62489c, the player count (ai.c / cm.c / main.c) */
extern unsigned short ai_PlayerId;      /* 0x6f25b8, the local player (cm.h / am.h / gv.h) */
/* ai_SaveWaitText: view of txt_Ptrs (build/mk_txt_views.py) */

/* ai.h's AICHAR (0x21c bytes, ai_CharsC[0x300] at 0x770b80): the fields this unit reads. f3snd.h declares
   ai_CharsC as char[], so ai.h cannot be included here; type debt. */
typedef struct AICHARVIEW {
    unsigned short id;                  /* +0x000, 0xffff when free */
    unsigned char kind;                 /* +0x002, 6 / 7 = a player */
    char pad003;
    unsigned int netId;                 /* +0x004 */
    char pad008[0x15e];
    unsigned char officeKind;           /* +0x166, the kind of the office held (amt.h AMTCHARVIEW.officeKind) */
    char pad167[0x21];
    char selected;                      /* +0x188, the group selection flag (sv.c ai_Selected, cleared by sv_ClearSelection) */
    char pad189[3];
    int pic;                            /* +0x18c, the picture of the character the poker panel shows (reasoned: the picture index) */
    char pad190[0x3c];
    unsigned int flags1cc;             /* +0x1cc, ai.h's flags1cc (bit 0x20000 = the blood revenge) */
    char pad1d0[0x4c];
} AICHARVIEW;

int ai_MenuButton(int row, int text1, int text2)
{
    int id;

    id = d2_WinCreateSlider(0x20, row * 0x2e + 10, 0xbf, text1, text2);
    d2_Objs[id].centred = 1;
    d2_SetObjWidth(id, 300);
    return id;
}
