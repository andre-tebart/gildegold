/* The info panel's quick jump record (sv_QuickJump, sv.h declares it). Its body is in its own header, included
   only by the units that read its fields (sv.c, lc.c): with the body in sv.h, sv_t.c's ai_TutorialInitChapter3
   falls off byte (a declaration-set tie-break). */
#ifndef SV_QUICKJUMP_H
#define SV_QUICKJUMP_H

struct SVQUICKJUMP {                    /* 0x5c bytes, sv_QuickJump: the info panel's pending quick jump (sv_HandleObjects:
                                           "QuickJump.ContactName"). One object: sv_SelectPanelObject's inline strcpy into
                                           contactName makes the original reload record (VC6 forgets what it knew of the
                                           object the bytes are stored into). Whether sv_PanelFlag (0x75e8c0, just before it)
                                           belongs to it is open */
    char *object;                       /* 0x00, the object the info panel shows */
    unsigned short *record;             /* 0x04, its scene record (gm_Query) */
    int unk08;                          /* 0x08, no access found */
    int unk0c;                          /* 0x0c, no access found */
    char contactName[76];               /* 0x10, the panel text: the contact name a quick jump looks up */
};

#endif
