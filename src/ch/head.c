/* ch.c -- original file name unknown: characters (ch_* error strings). Starts at 0x402140, right
   after the UDP code; its .data starts at 0x5f1120 with the animation path. Only the start of the file
   (up to 0x4028a0) is here. Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/ch.c); boundaries in decl/text.dcl, notes in src/ch/open.md */
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
/* d3.h declares these two with other types (void / int flag, D3ANIMFLAGS); ch.h has ours */
#define d3_Unk44e4a0 d3_Unk44e4a0_void
#include "d3.h"
#include "d3_anim.h"
#undef d3_Unk44e4a0
#include "d3_sm.h"
#include "path.h"
#include "ch.h"
#define d3anim_StartAnimCh ((int (*)(void *, char *, CHANIMFLAGS))d3anim_StartAnim)  /* 0x435550 (d3.h: D3ANIMFLAGS f): @original this unit passes its field-wise CHANIMFLAGS, 12 bytes like D3ANIMFLAGS, as the original TU did */
#include "data_views.h"
#define d3t_CamMoveI (*(int *)&d3t_CamMove)  /* 0x6310e8, d3t_CamMove (d3_tools.c: D3ANIM *) as the int this unit tests and clears */

#define D3SCENE_UNKB4(s) (*(int *)((char *)(s) + 0xb4))

char ch_AnimPath[0x100] = "x:\\gfx\\engine\\animations\\character";  /* 0x5f1120 */
int ch_LastUpdate = -1;  /* 0x5f1224 */
char ch_ShadowEnabled = 1;  /* 0x5f122c */
float ch_Unk5f1290 = 1.0f;  /* 0x5f1290, 1.0 */
int ch_CurrentScriptGroup = -1;  /* 0x5f1298, the script group that goes with it; -1 = none */
int ch_MorphTimeout = -1;  /* 0x5f129c (-1), the D3ANIMSLOT.timeout of morphs in scene != 0 */
int ch_MotionTimeout = 2;  /* 0x5f12a0 (2), the same for motions */
int ch_Enabled = 1;  /* 0x5f12a4 */
int ch_DirtyX0 = 256;  /* 0x5f12a8, changed ground rectangle */
int ch_DirtyZ0 = 256;  /* 0x5f12ac */

extern D3SCENE d3_Scenes[D3_MAX_SCENES];
extern unsigned char ch_PathBuf[1024];
extern CHMOTION ch_Motions[64];
extern char ch_MotionsTail[0x20];
extern int ch_Ground0[65536];
extern int ch_Ground1[65536];
extern CHFADE ch_Fades[16];
CHACTION * ch_Queue = 0;  /* 0x631050 */
int ch_Time = 0;  /* 0x631054 */
char ch_Unk631058 = 0;  /* 0x631058 */
int ch_SndLib = 0;  /* 0x63105c, sound library for step sounds */
int ch_Unk631060 = 0;  /* 0x631060 */
D3VECTOR ch_PathOffset = {0};  /* 0x631068, added to the last path cell */
int ch_DirtyX1 = 0;  /* 0x63107c */
int ch_DirtyZ1 = 0;  /* 0x631080 */
D3SCENE * ch_Saved1 = 0;  /* 0x631088, the same for ch_Ground1 */
D3SM * ch_PathMap = 0;  /* 0x63108c */
char ch_EmptyName[1] = {0};  /* 0x631090, "" (d3fl_EmptyName) */
D3SCENE d3_Scenes[D3_MAX_SCENES] = {0}; /* 0xfd7ba0 */
unsigned char ch_PathBuf[1024] = {0};  /* 0x14686e0 */
CHMOTION ch_Motions[64] = {0}; /* 0x14696e0, the 64 records end 0x20 before ch_Ground0 (its +0x20 dword is
                                   ai_Unk1469700, the action id ls.c indexes); ch_Path (0x14696e0) is an alias view */
char ch_MotionsTail[0x20] = {0};  /* 0x146a9e0, the 0x20 bytes the ch_ActDefs view (ch_Motions + 0x20, 0x1300 bytes) reaches
                                     past ch_Motions; without a symbol here the full link put ch_Queue at +0x1300 and
                                     ch_Open's memset(ch_ActDefs) zeroed it (anm_Update then crashed on a null queue) */
int ch_Ground0[65536] = {0};  /* 0x146aa00, ground types of d3_Scenes[0] */
int ch_Ground1[65536] = {0};  /* 0x14aaa20, ground types of the default scene */
CHFADE ch_Fades[16] = {0};                /* 0x14eaa20 */

