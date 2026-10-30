# Headers: the typing debt

What the M5 pass of 2026-10-30 (commits fb5bb02 .. 520c492, fix 6fc7677) left open. The rule it applied:
every copy of a symbol in another header repeats the defining unit's spelling (taken from the definition);
a unit whose code was compiled against another view calls it through a cast macro, or reads it through a
`(*(T *)&datum)` view macro, in its head.c, with the byte evidence in the macro's comment. The commit
messages carry the per-symbol evidence.

## VC6 fact behind most of it
The declaration set of a unit moves VC6's /O2 output, independent of the declarations' types: deleting an
uncalled prototype, or replacing copies by an include of the owner's header, flipped d3_MirDrawObject
(0x4550e0) and d3_CloneObject (0x43d180) from byte to shape (4 instructions each, 003e5c2); adding
d3anim_NextFrame to d3_io_t's set moved d3t_CreateSnowMipmaps 129 -> 131 (aceba2a); main_Unk539870 moves
593 <-> 595 on unrelated declaration changes (d107b71). So a copy can not simply go: removing it is a
grade change to verify (all of the unit's functions, byte ones first), and a byte flip means the copy (in
some spelling) stays.

## State after the M5-per-symbol pass (2026-10-30, commits 3f47652 .. 0700500)
The stricter M5 (every declaration against the definition) started at 546; it is **0** now, gate ok.
Done: the untyped `extern unsigned char X;` placeholders of src heads (3f47652, 1411928), the five headers
that did not compile in the all-declarations unit (1beed7b), ~230 function copies as defined with the
callers' views as `<name>K` / `<name>U` / `<name>S` .. casts in their heads (d6126a3, 99725af, 16ac8c8,
755d871, 1838168), ~75 data copies as defined, record pointers / objects by struct tag, the by-value
readers through `X<B|C|I|U|S|V>` views at the end of their head.c (cb005da, 1b8f951, 451bf2a).
Definitions fixed instead: gm_BuildingRank (int), gm_CharacterWages (unsigned short id), d3d_LoadTexture
(+ bpp), the five espionage handlers (+ int *m), ai_Objects (struct AIOBJ *), d2_ResizeWindow /
d2_CreateChildWindow (prototype definitions), ai_Unk61a348 (G7GRP[37], 8ab4fb5), gm_Offers
(AISTORE[88], 0700500).
The last 35 (588a9e8 .. 0700500):
- Types-only headers (`include/<owner>_types.h`, the owner's header includes it where the records stood):
  d2_types.h (D2TEX, D2SURF, D2WIN, D2INPUT, D2RADIO, D2VIEW), d3_types.h (D3SCENE, D3TEXFLAGS),
  ch_types.h (CHMOTION), amt_types.h (AMTREC), hst_types.h (G12ROW), cm_types.h (G17MSG, G17STAT),
  f3snd_types.h (G16AMBIENTON), am_types.h (G7GRP), unk_4b39f0_types.h (G13MIS), unk_5d33b0_types.h
  (OPTIONBYTES, was a type of unk_5d33b0's head), f3init_types.h (INIT_PLAYER, f3init has no header).
  The readers include them and declare the tables as defined.
- ai.h may not include a header: any include from ai.h (an empty file too, any name, any place) flips
  ai_Unk483a60 (0x483a60, byte) to shape (obj / ok stack slots 0x10 / 0x18 swap); so do AIACT in ai's
  set and any added typedef in ai's head. ai_types.h (AITIME, AISTORE) is therefore not included by
  ai.h: ai.h keeps identical copies under the same guards AI_TIME_TYPE / AI_STORE_TYPE (a guard keeps
  the byte). Change both copies together. ai's ai_Unkf83ec0 stays `char[]` (@original, that evidence).
- Header includes instead of copies: ls (ai.h), unk_589f10 (di_input.h), f3init / hst / unk_4b39f0
  (net.h, their local net_Conn struct views gone).
- The texture family: TSFLAGS is a typedef of D3TEXFLAGS (one type); the headers' tex_Load / Create /
  CreateHi / AliasForFlags and d3_Textures spell ts_texture.c's `struct TSTEXTURE *`; the d3 callers
  cast the result to their D3TEXHANDLE / D3FLTEX / D3TEXTURE view at the call and read d3_Textures
  through `(D3TEXTURE *)d3_Textures`. The records themselves are still separate types (D3TEXTURE.flags
  at 0x68 is TSTEXTURE's bitfields, TSTEXTURE.flags is the 0x6c TSFLAGS): merging them is open.
  d3_ClipMesh: d3_fl.h spells the definition, D3FLTILE.clip is also `D3CLIP clipCode` (a union).
- Kept with @original byte evidence (a byte object read as a byte, VC6 loads a 4-byte union's byte with
  a dword load): anm's `unsigned char ai_GfxDetail` (anm_Update 0x4bb884), main's `unsigned char
  ai_GfxResolution` (main_OpenScreen 0x531d43).
- Copies of names without a definition (ai_Unk5fb5b0, ai_Unk1460ae0, ai_Unk6ed92c) agree with each other;
  they are address placeholders of d3_DefaultScene / net_Conn.s / a gfx surface pointer, still to name.
Views added by the pass are `#define X<suffix> (*(OLD (*)..)&X)` / `((T (*)(..))f)` lines in the units'
heads; most are drafts' old spellings (no byte evidence of their own): drop each when its function is raised.

## The evidence-less views (2026-10-30, user decision: drop them, accept draft regressions)
The 96 `this unit reads it as` views had no byte evidence. 8d8d2d5 and af79fa9 drop 75 of them (the uses
read the datum as declared); 2 stay with @original (cm's ai_Unk7603a0V / ai_Unk760420V: ai_Unk4cfeb0
0x4cfeb0 byte). ai_GameState was the weak side: its definition is `unsigned char` now (the byte readers
of unk_5d33b0 / unk_4b39f0 needed the byte, he reads it bytewise). No byte or shape function moved; the
drafts that moved are in the two commit messages.
Left, 19 views whose uses need a type (open, settle with the readers):
- ai_JudgeCatholic / ai_JudgeProtestant (gv/tail.c `char *`): am, hst read `AIPER *` fields (->netId).
  Probably retype the definition to `struct AIPER *`.
- ai_MarketBuilding (AIBLD *, ai) read as int by f3gm; cm_FieldObject (cm `char *`) read as int by f3gm /
  f3init (offset arithmetic `cm_FieldObject - x` passed to cm_AddFieldValue); ai_Objects (struct AIOBJ *)
  read as char * by f3gm; ai_Unk75e130 (cm `char[4]`) read as int by ls.
- d2: d2_ObjsV (G13OBJ, unk_4b39f0 / unk_5d33b0, 103 uses), d2_LastWindowPtrV (G12WIN, hst / unk_5d33b0),
  d2_PicsV (D2PICVIEW, unk_5d33b0): local record views of D2OBJ / D2WIN / D2PIC; map their fields onto
  d2_types.h's records.
- d3_SortBufV / d3_SortPtrV (d3_sm's D3SMSORT): d3_sm does not see d3_engine's D3SORTENT.
- ls: ai_BauplaetzeV (G15BK, ls does not see it), main: ls_SavePathV (struct LSSAVEVIEW of the char[640]:
  wants ls.h, item 4), sv: ai_Unk6f0454V (f3gm defines `char[1]` but writes [0..2]: the size is wrong).

## Debt
1. **Weak definition types made canonical.** (ai_Objects done in 1b8f951; the rest open: the definers
   ahm / f3gm / anm / sv do not see AICHAR / AIBTYPE / AIBLD / AIGOOD / AITIME, so retyping needs ai.h
   or a types header in them first.) The definitions are char views of typed records, and every
   typed reader now goes through a view macro:
   - ai_Chars `char[768][0x21c]` (ahm.c) - really `AICHAR[768]` (ai.h:176, asserted 0x21c); view ai_CharsT
     (ai, am, hmt, hst, main, unk_4a7140, unk_4b39f0, unk_4b6480), ai_CharsC (f3snd, unk_5d33b0).
   - ai_BTypes `char *` (ahm.c) - AIBTYPE *; ai_Blds / ai_Goods `char *` (f3gm.c) - AIBLD * / AIGOOD *;
     views ai_BTypesT / ai_BldsT / ai_GoodsT.
   - ai_Time `char[14]` (anm.c) - AITIME; views ai_TimeT, ai_TimeK (ahm: copies it as 3 ints + a short,
     @original).
   - ai_PlayerBuilding / ai_FallbackBuilding `char *` (sv.c) - AIBLD *; views ai_*BuildingT / *B.
   - ai_PlayerChar `unsigned short *` (ai.c); view ai_PlayerCharC (bytewise: cm, f3gm, f3init, gv, io_text,
     ls, ord, sv).
   Fix: retype the definition to the record type where the defining unit's bytes allow (re-grade the
   defining unit and every reader), then drop the views that become identities. A view stays only with
   @original evidence (a reader whose bytes need the other spelling).
2. **Parameters spelled weaker than they are.** (open; the pass fixed the definitions listed above.) vfs_Tfopen's third parameter is a VFSDIR * root (the
   definition casts it), spelled `int unk` in vfs.h and the definition; callers pass `(int)vfs_Root`
   (ahm, bmp, hmt). d3_SetBlendState: true prototype takes unsigned char, d3_engine keeps the int view
   d3_SetBlendStateI; with the true prototype d3_DrawSortedD3D is 2225 bytes against the original's 2211
   (2229 with the int view) - a matching lead, not settled.
3. **Retyped copies still in other headers.** Copies of other units' functions, now in the owner's
   spelling, remain in ai.h, am.h, anm.h, cm.h, d2.h, gv.h, hst.h, rt.h, unk_4a7140.h, unk_4b6480.h,
   ahm.h (4bd009c), d3.h, d3_engine.h, d3_mir.h, d3_quad.h, d3_fl.h, d3_light.h (003e5c2, 8b5e367),
   f3snd.h, cm.h, gfx.h (sblib / d3_snd copies), cm.h, gv.h, am.h, ch.h (d3_tools copies), and in
   main/head.c. Each is either removable (the unit includes the owner's header instead, no grade moves)
   or needed by the declaration set (a byte flip without it): settle per header, record the needed ones.
4. **Headers not written.** sv.h (311 declarations generated), f3init.h (95), ls.h (74) need the same
   retype-and-cast pass against their copies. Waiting on ls.h: main's NETHDR / LSSAVEVIEW local views
   (ls_SavePath) and main_NetGameRecord (cm's 0x80-byte object at 0x936460).
5. **gv's D3COLOR declarations.** gv's head keeps @original no-prototype declarations of d3t_CreateSmoke /
   d3t_CreateEarth: they take a D3COLOR by value, which VC6 rejects as an incomplete type (C2079) in gv
   (no d3.h). Settle by giving gv the complete type (include order) or keep with the evidence.
6. **Includes now possible in main.** The MPOOLITER, D3OBJBITS and ai_* data clashes are gone, so main
   can try mem.h, ch.h / chc.h, hmt.h, ahm.h, amt.h, gv.h, cm.h, hst.h (main/open.md's "unsettled" list
   predates this; re-run `dcp headers conflicts main ...`).
7. **open.md files not updated** by the pass: the units it touched (ahm, ai, am, anm, ch, cm, d3_*, f3gm,
   f3init, f3snd, gv, hst, io_text, main, rd, sblib, sv, unk_*) carry no note of their new cast / view
   macros. The macro comments and the commit messages are the record until then.
