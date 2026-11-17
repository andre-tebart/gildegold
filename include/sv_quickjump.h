/* The info panel's quick jump record (sv_QuickJump, sv.h declares it). Its body is in its own header, included
   only by the units that read its fields (sv.c, lc.c): with the body in sv.h, sv_t.c's ai_TutorialInitChapter3
   falls off byte (a declaration-set tie-break). */
#ifndef SV_QUICKJUMP_H
#define SV_QUICKJUMP_H

struct SVQUICKJUMP {                    /* 0x60 bytes at 0x75e8c0, sv_QuickJump: the info panel's flag and pending quick jump
                                           (sv_HandleObjects: "QuickJump.ContactName"). One object: sv_SelectPanelObject's
                                           inline strcpy into contactName makes the original reload record (VC6 forgets what it
                                           knew of the object the bytes are stored into); the 0x5c bytes from object on sit at
                                           0x75e8c4, off the 32-byte alignment of a common that size: the object starts with
                                           the panel flag (sv.h's sv_PanelFlag) */
    int panelFlag;                      /* 0x00, sv_PanelFlag: 1 while the info panel is up */
    char *object;                       /* 0x04, the object the info panel shows */
    unsigned short *record;             /* 0x08, its scene record (gm_Query) */
    int unk0c;                          /* 0x0c, no access found */
    int unk10;                          /* 0x10, no access found */
    char contactName[76];               /* 0x14, the panel text: the contact name a quick jump looks up */
};

#endif
