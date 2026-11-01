/* he.h -- the event bar (0x53d430 ..), see src/game/he.c */
#ifndef HE_H
#define HE_H

void he_Unk53d430(unsigned char *ev);
void he_Unk53d470(unsigned char *ev);
int he_Unk53d4b0(void);
void he_FreeEvents(void);
int he_ReleaseEvents(void);
int he_Unk53d600(int type, void *alloc, void *run);
int he_Unk53d630(void);
int he_Unk53d6f0(int ev, int b, int c, int d, int e);
void he_Unk53d800(int ev);
void he_Unk53d930(void);
void he_CloseEventSlots(void);
void he_Unk53d9f0(int pl);
void he_Unk53dad0(int state);
void he_Unk53daf0(void);
void he_Unk53dcb0(int *sl);
int he_ShowNews(int pid, int fl, char *text, int unused, int ev, char *text2);
void he_MessageBoxQuickjump(int a1, int a2, char *text, char fl, int a5, int a6, int a7, char *jump, char *text2);

unsigned char * he_Unk53e040(unsigned char *m);
void he_Unk53e1f0(int *r);
char *he_NextEvent(void);
char *he_FindEvent(int n, ...);
void he_ShowConstruction(int ev);               /* 0x56b780, the construction state of the building event ev */
void he_Unk53e4e0(int obj);
int he_Unk53e5f0(int e, int name, int obj);
void he_Unk53e780(int rec, char *name, int obj);
void he_Unk53e840(int *e);
void he_Unk53e8b0(unsigned char *r);
void he_Unk53eba0(void);
void he_RemoveGfxInfos(int obj);
void he_Unk53ec10(int obj, int a);
void he_Unk53ec40(unsigned char *r);
void he_Unk53ed40(unsigned short *pl);
void he_Unk53ed70(void);
void he_Unk53edf0(void);
void he_Unk53ee40(void);
void he_Unk53eea0(void);

void he_Unk53ef50(void);
int he_SumApEvents(int key);
int *he_AddApEvent(int a, char *name, int c, int d);  /* 0x53eec0, registers an AP event, returns its counter byte (0 when full) */
int he_Unk53efb0(int key);
void he_PostEvent6d(void);
void he_PostEvent6b(void);
void he_PostEvent7a(void);
void he_PostEvent2b(void);
void he_Unk53f270(void);
void he_Unk53f370(void);
void he_Unk53f430(void);
void he_Unk53f500(void);
unsigned he_Unk53f5b0(void);

void he_Unk540c90(void);
void he_Unk540e40(void);
void he_Unk540fe0(void);
void he_Unk541010(void);
void he_Unk541060(void);
void he_Unk5410f0(void);
void he_Unk541170(void);
void he_Unk541200(int r);
void he_Unk541250(int r, int pl);



void he_Unk541280(int ev, int p2);
void he_Unk5414a0(int p1, unsigned short *p2, int p3, int p4, float p5, int p6);
void he_Unk5417d0(int p1, unsigned short *p2, int p3, int p4, int p5, float p6);
void he_Unk541a80(int r);

void he_Unk544330(int ev);
void he_Unk544550(int ev, unsigned short *p2, unsigned short *p3);
void he_Unk544630(int ev);
void he_Unk5471e0(int ev, unsigned short *p2, unsigned short *p3);

void he_Unk5437c0(int ev);

void he_Unk545640(int ev);

void he_Unk545670(int ev);

void he_Unk544070(int ev);

unsigned he_Unk53f6b0(void);

void he_Unk541b40(unsigned cmd, int a2, int a3, int a4);

void he_Unk542490(int ev);

void he_Unk542590(int ev);

void he_Unk543020(int ev);
void he_Unk543180(int ev);

void he_Unk5432f0(int ev);

void he_Unk5436a0(int ev);

void he_Unk542960(int ev);

void he_Unk542b60(int ev);

void he_Unk542100(int ev);

void he_SaveMessageFilter(void);                                       /* 0x575600 */

/* declared in other units' headers before (moved to the owner) */
int he_Unk553cc0(int p);  /* 0x553cc0 */
int he_Unk553d10(int a, int b);  /* 0x553d10 */
int he_Unk553d60(int a, int b);  /* 0x553d60 */
int he_Unk56f170(int owner);  /* 0x56f170 */
int he_Unk570ea0(int a, int b);  /* 0x570ea0 */
int he_IsMessageKindFiltered(int id);  /* 0x575640 */
int he_Unk555210(char *obj, int unused1, int unused2, int n);  /* 0x555210 */
char *ai_FindCurseEvent(char *a);  /* 0x5741d0 */
int he_Unk553db0(unsigned short *a, int b, int *count);  /* 0x553db0 */

#endif
