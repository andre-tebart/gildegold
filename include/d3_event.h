/* d3_event.h -- unit d3_event's declarations: the 3D engine's object / scene events ("d3e:", 0x43e6b0..0x43eab0).
   Included by d3_engine.h at the place its event block had (it needs d3_types.h's D3EVHANDLER / D3EVENT); d3e_LoadEvents'
   prototype is still in d3_int_io.h, the others have none (callers in d3_event.c only). */
#ifndef D3_EVENT_H
#define D3_EVENT_H
/* D3E_NEVENTS, D3EVHANDLER, D3EVENT: d3_types.h */
typedef struct D3EVNAME {           /* 0x24 bytes */
    char name[0x20];
    int id;
} D3EVNAME;
typedef D3EVHANDLER (*D3EVRESOLVE)(void *obj, int id, char *name);
extern D3EVNAME d3e_Events[8];              /* 0x5fb800 */
extern D3EVRESOLVE d3e_Resolve;             /* 0x5fb920 */
void d3e_SaveEvents(void *f, D3EVENT *t);                                                       /* 0x43e900 */
void d3e_LoadSceneEvents(void *f);                                                              /* 0x43ea10 */

#endif
