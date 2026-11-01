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
#define ai_Unk770b89             (*(char (*)[347])((char *)&ai_Chars + 0x9))  /* 0x770b89 */
#define ai_Unk770cec             (*(char (*)[28])((char *)&ai_Chars + 0x16c))  /* 0x770cec */
#define ai_Selected              (*(char (*)[132])((char *)&ai_Chars + 0x188))  /* 0x770d08 */
#define ai_Unk770d8c             (*(char (*)[414196])((char *)&ai_Chars + 0x20c))  /* 0x770d8c */

/* gm_PlayerRecords */
#define gm_PlayerRecordBytes     (*(char (*)[2750])((char *)&gm_PlayerRecords + 0x2))  /* 0x7d5f82 */

/* ai_PlayerSlots */
#define ai_Unk7e3f80             (*(int *)((char *)&ai_PlayerSlots + 0x40))  /* 0x7e3f80 */
#define ai_Unk7e3f84             (*(int *)((char *)&ai_PlayerSlots + 0x44))  /* 0x7e3f84 */
#define ai_Unk7e3f94             (*(char (*)[100])((char *)&ai_PlayerSlots + 0x54))  /* 0x7e3f94 */
#define ai_Unk7e3ff8             (*(char (*)[80])((char *)&ai_PlayerSlots + 0xb8))  /* 0x7e3ff8 */
#define ai_Unk7e4048             (*(int (*)[4])((char *)&ai_PlayerSlots + 0x108))  /* 0x7e4048 */
#define ai_Unk7e4058             (*(int (*)[117])((char *)&ai_PlayerSlots + 0x118))  /* 0x7e4058 */
#define ai_Unk7e422c             (*(char (*)[72])((char *)&ai_PlayerSlots + 0x2ec))  /* 0x7e422c */
#define ai_Unk7e4274             (*(AIREC2F4 (*)[3])((char *)&ai_PlayerSlots + 0x334))  /* 0x7e4274 */
#define init_SlotCurrency        (*(short (*)[0x46e])((char *)&ai_PlayerSlots + 0x348))  /* 0x7e4288 */

/* d2_Wins */
#define ai_Unkf8f128             (*(char (*)[20])((char *)&d2_Wins + 0x8))  /* 0xf8f128 */
#define ai_Unkf8f13c             (*(int (*)[1][0xf1])((char *)&d2_Wins + 0x1c))  /* 0xf8f13c */
#define ai_Unkf8f39c             (*(char (*)[91908])((char *)&d2_Wins + 0x27c))  /* 0xf8f39c */

/* d2_Radios */
#define d2_RadioSel              (*(int (*)[278])((char *)&d2_Radios + 0x8))  /* 0xfa5aa8 */

/* d2_Views */
#define ai_Unkfb1030             (*(char (*)[32816])((char *)&d2_Views + 0x10))  /* 0xfb1030 */

/* ai_Unkf83ec0 */
#define ai_ActionFlags           (*(unsigned int (*)[22][37])((char *)&ai_Unkf83ec0 + 0x90))  /* 0xf83f50 */
#define ai_Unkf84c40             (*(float (*)[8])((char *)&ai_Unkf83ec0 + 0xd80))  /* 0xf84c40 */
#define ai_Unkf84c60             (*(float (*)[8])((char *)&ai_Unkf83ec0 + 0xda0))  /* 0xf84c60 */
#define ai_Unkf84cd4             (*(float (*)[8])((char *)&ai_Unkf83ec0 + 0xe14))  /* 0xf84cd4 */
#define ai_Unkf84cf4             (*(float (*)[8])((char *)&ai_Unkf83ec0 + 0xe34))  /* 0xf84cf4 */

/* cm_SendStates */
#define ls_RefTargets            (*(int (*)[17574])((char *)&cm_SendStates + 0x6))  /* 0xf31166 */
#define ai_Unkf42400             (*(char *)((char *)&cm_SendStates + 0x112a0))  /* 0xf42400 */

/* ai_Unk925520 */
#define ai_Unk925538             (*(char (*)[26472])((char *)&ai_Unk925520 + 0x18))  /* 0x925538 */

/* ai_Unk92cae0 */
#define ai_Unk92caf0             (*(char (*)[13744])((char *)&ai_Unk92cae0 + 0x10))  /* 0x92caf0 */

/* d3d_UsedCard */
#define d3d_Caps98d              (*(unsigned char *)((char *)&d3d_UsedCard + 0x30d))  /* 0xfe798d */

/* d3_ParEnd */
#define d3_ParTail               (*(struct D3PARTSYS **)((char *)&d3_ParEnd + 0x318))  /* 0xfb9598 */

/* d3_Mirror */
#define d3_Unkfb9928             (*(unsigned char *)((char *)&d3_Mirror + 0x28))  /* 0xfb9928 */

/* ai_GfxFloor */
#define ai_Unk6f35ca             (*(char *)((char *)&ai_GfxFloor + 0x2))  /* 0x6f35ca */

/* cm_SaveAcks */
#define ai_Unk76fa24             (*(char (*)[60])((char *)&cm_SaveAcks + 0x4))  /* 0x76fa24 */

/* ai_NextEventId */
#define ai_Unk6f261c             (*(int *)((char *)&ai_NextEventId + 0x4))  /* 0x6f261c */

/* d3_ScrollPool */
#define d3_ScrollSpeed           (*(float (*)[15])((char *)&d3_ScrollPool + 0x4))  /* 0x5ebc24 */
#define d3_AnimDiv               (*(unsigned int (*)[12])((char *)&d3_ScrollPool + 0x40))  /* 0x5ebc60 */
#define d3_SnapEpsilon           (*(double *)((char *)&d3_ScrollPool + 0x0))  /* 0x5ebc20 */

/* d3_DefaultD3DConfig */
#define d3if_Unk5fb5c0           (*(int *)((char *)&d3_DefaultD3DConfig + 0xc))  /* 0x5fb5c0 */

/* d3_StockHead */
#define d3_Unkfc4c3c             (*(int *)((char *)&d3_StockHead + 0x1fc))  /* 0xfc4c3c */
#define d3_StockList             (*(D3IOOBJ **)((char *)&d3_StockHead + 0x200))  /* 0xfc4c40 */
#define d3_Unkfc4c40             (*(void **)((char *)&d3_StockHead + 0x200))  /* 0xfc4c40 */
#define d3_FovY                  (*(float *)((char *)&d3_StockHead + 0x20c))  /* 0xfc4c4c */

/* d3_AnimHead */
#define d3_Unkfe75a0             (*(int *)((char *)&d3_AnimHead + 0x160))  /* 0xfe75a0 */
#define d3_AnimList              (*(D3IOANIMDATA **)((char *)&d3_AnimHead + 0x164))  /* 0xfe75a4 */
#define d3_Unkfe75a4             (*(void **)((char *)&d3_AnimHead + 0x164))  /* 0xfe75a4 */

/* d3_Scenes */
#define ai_MapRefs               (*(void **)((char *)&d3_Scenes + 0xac))  /* 0xfd7c4c */
#define ai_Unkfd7c50             (*(char (*)[805])((char *)&d3_Scenes + 0xb0))  /* 0xfd7c50 */
#define init_SceneDirty          (*(char (*)[3])((char *)&d3_Scenes + 0x3d5))  /* 0xfd7f75 */
#define d3_Scene0Flags             (*(unsigned char *)((char *)&d3_Scenes + 0x3d8))  /* 0xfd7f78 */
#define unk_Ptrfd7c50            (*(char **)((char *)&d3_Scenes + 0xb0))  /* 0xfd7c50 */
#define init_SceneHeightMaps     (*(int (*)[202])((char *)&d3_Scenes + 0xb0))  /* 0xfd7c50 */

/* ai_Unk91f700 */
#define ai_Unk91f716             (*(char (*)[23018])((char *)&ai_Unk91f700 + 0x16))  /* 0x91f716 */

/* ai_Unk61a686 */
#define ai_Unk61a688             (*(char (*)[548])((char *)&ai_Unk61a686 + 0x2))  /* 0x61a688 */

/* ai_Unk623ba9 */
#define ai_Unk623baa             (*(int (*)[69])((char *)&ai_Unk623ba9 + 0x1))  /* 0x623baa */

/* ch_Motions */
#define ai_Unk1469700            (*(int (*)[1216])((char *)&ch_Motions + 0x20))  /* 0x1469700 */
#define ch_ActDefs               (*(CHACTDEF (*)[64])((char *)&ch_Motions + 0x20))  /* 0x1469700 */

/* d3_Cfg */
#define d3_SortMax               (*(int *)((char *)&d3_Cfg + 0x18))  /* 0xfe73f8 */
#define d3_Unkfe7400             (*(int *)((char *)&d3_Cfg + 0x20))  /* 0xfe7400 */

/* d3_MipSkip */
#define d3d_ZEnabled             (*(char *)((char *)&d3_MipSkip + 0x1))  /* 0x63342d */
#define d3d_balpha               (*(unsigned char *)((char *)&d3_MipSkip + 0x2))  /* 0x63342e */
#define d3d_glow                 (*(unsigned char *)((char *)&d3_MipSkip + 0x3))  /* 0x63342f */

/* ai_GameState */
#define sv_MenuOpen              (*(unsigned char *)((char *)&ai_GameState + 0x2))  /* 0x6f25ae */

/* d3_AnimPath */
#define d3if_Unkfba944           (*(int *)((char *)&d3_AnimPath + 0x4))  /* 0xfba944 */

/* d3d_Config */
#define d3_ShadowMode            (*(struct D3BITS3 *)((char *)&d3d_Config + 0x1))  /* 0xfe7671 */

/* dd_GBits */
#define dd_GLoss                 (*(unsigned char *)((char *)&dd_GBits + 0x1))  /* 0x1463581 */

/* dd_BBits */
#define dd_Unk1464fc0            (*(int *)((char *)&dd_BBits + 0x3))  /* 0x1464fc0 */

/* sv_ViewPos / sv_ViewRot (the stored camera view, read and copied as ints by the units that save it) */
#define sv_ViewPosX              (*(int *)((char *)&sv_ViewPos + 0x0))  /* 0x75fb80 */
#define sv_ViewPosY              (*(int *)((char *)&sv_ViewPos + 0x4))  /* 0x75fb84 */
#define sv_ViewPosZ              (*(int *)((char *)&sv_ViewPos + 0x8))  /* 0x75fb88 */
#define sv_ViewRotX              (*(int *)((char *)&sv_ViewRot + 0x0))  /* 0x75fb90 */
#define sv_ViewRotY              (*(int *)((char *)&sv_ViewRot + 0x4))  /* 0x75fb94 */
#define sv_ViewRotZ              (*(int *)((char *)&sv_ViewRot + 0x8))  /* 0x75fb98 */

/* ai_Unk75e130 */
#define ai_Unk75e134             (*(int *)((char *)&ai_Unk75e130 + 0x4))  /* 0x75e134 */
#define ai_Unk75e138             (*(int *)((char *)&ai_Unk75e130 + 0x8))  /* 0x75e138 */
#define ai_Unk75e13c             (*(short *)((char *)&ai_Unk75e130 + 0xc))  /* 0x75e13c */

/* ai_Unk76f920 */
#define ai_Unk76f924             (*(int *)((char *)&ai_Unk76f920 + 0x4))  /* 0x76f924 */
#define full_data_76f990         (*(int *)((char *)&ai_Unk76f920 + 0x70))  /* 0x76f990 */
#define full_data_76f994         (*(int *)((char *)&ai_Unk76f920 + 0x74))  /* 0x76f994 */
#define ai_Unk76f9b8             (*(int *)((char *)&ai_Unk76f920 + 0x98))  /* 0x76f9b8 */
#define ai_Unk76f9bc             (*(char (*)[100])((char *)&ai_Unk76f920 + 0x9c))  /* 0x76f9bc */

/* ai_Unk6339db */
#define d3if_Unk6339dc           (*(unsigned int *)((char *)&ai_Unk6339db + 0x1))  /* 0x6339dc */

/* ai_Unk6f0454 */
#define ai_Unk6f0455             (*(char (*)[2])((char *)&ai_Unk6f0454 + 0x1))  /* 0x6f0455 */

/* ai_Unkfa5f04 */
#define d2_PointerX              (*(int *)((char *)&ai_Unkfa5f04 + 0x4))  /* 0xfa5f08 */

#endif
