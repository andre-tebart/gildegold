/* Views into packed data objects (the source of truth; edit by hand, `dcp rename` renames).

   The original packs its data area: a name whose address lies inside a larger object is a
   view of that storage, not an object of its own.  Each line is one such name: the type, the
   container, the byte offset and the view's address.  dcp checks that the address is the
   container's plus the offset (L15).  The full link keeps the name inside its container, as
   the original does.  (First written by rea's tools/data_views.py, rea 8cef7d5b.) */
#ifndef DATA_VIEWS_H
#define DATA_VIEWS_H

/* ai_Chars */
#define ai_CharInts              (*(int (*)[768][0x87])((char *)&ai_Chars + 0x4))  /* 0x770b84 */
#define ai_Selected              (*(char (*)[132])((char *)&ai_Chars + 0x188))  /* 0x770d08 */
#define ai_Unk770d8c             (*(char (*)[414196])((char *)&ai_Chars + 0x20c))  /* 0x770d8c */

/* d2_Radios */
#define d2_RadioSel              (*(int (*)[278])((char *)&d2_Radios + 0x8))  /* 0xfa5aa8 */

/* ai_Actions */
#define ai_ActionFlags           (*(unsigned int (*)[22][37])((char *)&ai_Actions + 0x90))  /* 0xf83f50 */

/* cm_SendStates */
#define ls_RefTargets            (*(int (*)[17574])((char *)&cm_SendStates + 0x6))  /* 0xf31166 */

/* d3d_UsedCard */
#define d3d_Caps98d              (*(unsigned char *)((char *)&d3d_UsedCard + 0x30d))  /* 0xfe798d */

/* d3_ParEnd */
#define d3_ParTail               (*(struct D3PARTSYS **)((char *)&d3_ParEnd + 0x318))  /* 0xfb9598 */

/* d3if_ParList (the head node: its next field) */
#define d3_ParHead               (*(struct D3PARTSYS **)((char *)&d3if_ParList + 0x314))  /* 0xfb98f4 */

/* d3_DefaultD3DConfig */
#define d3if_SceneState0           (*(int *)((char *)&d3_DefaultD3DConfig + 0xc))  /* 0x5fb5c0 */

/* d3_Scenes */
#define ai_MapRefs               (*(void **)((char *)&d3_Scenes + 0xac))  /* 0xfd7c4c */
#define init_SceneDirty          (*(char (*)[3])((char *)&d3_Scenes + 0x3d5))  /* 0xfd7f75 */
#define d3_Scene0Flags             (*(unsigned char *)((char *)&d3_Scenes + 0x3d8))  /* 0xfd7f78 */
#define init_SceneHeightMaps     (*(int (*)[202])((char *)&d3_Scenes + 0xb0))  /* 0xfd7c50 */

/* d3_Cfg */
#define d3_SortMax               (*(int *)((char *)&d3_Cfg + 0x18))  /* 0xfe73f8 */

/* d3d_Config */
#define d3_ShadowMode            (*(struct D3BITS3 *)((char *)&d3d_Config + 0x1))  /* 0xfe7671 */

/* sv_ViewPos / sv_ViewRot (the stored camera view, read and copied as ints by the units that save it) */
#define sv_ViewPosX              (*(int *)((char *)&sv_ViewPos + 0x0))  /* 0x75fb80 */
#define sv_ViewPosY              (*(int *)((char *)&sv_ViewPos + 0x4))  /* 0x75fb84 */
#define sv_ViewPosZ              (*(int *)((char *)&sv_ViewPos + 0x8))  /* 0x75fb88 */
#define sv_ViewRotX              (*(int *)((char *)&sv_ViewRot + 0x0))  /* 0x75fb90 */
#define sv_ViewRotY              (*(int *)((char *)&sv_ViewRot + 0x4))  /* 0x75fb94 */
#define sv_ViewRotZ              (*(int *)((char *)&sv_ViewRot + 0x8))  /* 0x75fb98 */

/* sv_Bubbles */
#define sv_BubbleObjects         (*(int (*)[4286])((char *)&sv_Bubbles + 0x8))  /* 0x761fc8 */

#endif
