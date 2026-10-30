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
- ai.h includes headers again (user decision, 2026-10-30): any include from ai.h (an empty file too)
  flips ai_Unk483a60 (0x483a60) byte -> shape (obj / ok stack slots 0x10 / 0x18 swap): a declaration-set
  tie-break, not evidence of the original's declarations; its drop is accepted (promoted). ai.h includes
  ai_types.h (AITIME, AISTORE: one copy, the AI_TIME_TYPE / AI_STORE_TYPE guards gone), AIACTW / AIACT
  moved to am_types.h and ai declares ai_Unkf83ec0 as defined (AIACT[]). The exception covers only that
  kind of byte match (one any unrelated declaration change flips).
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
The 19 views left by that pass (2026-10-30, second pass): all 19 done, none kept, no byte or shape function lost:
- ai_JudgeCatholic / ai_JudgeProtestant: the definition (gv) is `struct AICHAR *` now, every copy too; am /
  hst read ->netId (cc67618).
- cm_FieldObject: f3gm / f3init form the field offsets as `(char *)p - cm_FieldObject + off` (79b236a).
- ai_MarketBuilding: f3gm stores `(struct AIBLD *)gm_BuildQuery(..)`, reads `(char *)ai_MarketBuilding + 0x5d`
  (f3gm does not see AIBLD) (c0773a0). ai_Objects: f3gm uses the struct AIOBJ * (1356a6e).
- ai_Bauplaetze: G15BK in rd_types.h (f3init's copy gone), ls reads ai_Bauplaetze[i] (42f686a).
- ai_Unk75e130: cm defines it `int` (the year of a clock copy that is split over cm / ls objects
  0x75e130..0x75e13e: one AITIME would need the four definitions in one unit, open) (f9be835).
- ai_Unk6f0454: f3gm defines `char[3]` (its own byte reader writes [0..2]), sv clears [0..2] (8e598cb).
- d3_SortBuf / d3_SortPtr: D3SORTENT in d3_types.h, d3_sm's D3SMSORT gone (a339698).
- d2_Objs / d2_LastWindowPtr / d2_Pics: D2PIC / D2CTRL / D2OBJ in d2_types.h (D2OBJ.user at 0x2e8),
  G12WIN / G13OBJ / D2OBJVIEW / D2WINVIEW / D2PICVIEW gone (30442f6). unk_5d33b0 still has D2SURFVIEW
  (d2.c defines d2_ScreenSurf as `void *`: a definition to retype) and D2FADEVIEW, AICHARVIEW.
- ls_SavePath: ls defines it as an LSSAVE record (ls_types.h), main's LSSAVEVIEW / NETHDR copies gone (2021623).

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
3. **Retyped copies still in other headers.** (second pass: 239fbc6, 71d328c, 01605c8, fda000a) A
   script over every unit's include closure removed each copy of another unit's symbol from a header
   when every includer of that header also sees another declaration: 64 function copies, 16 data
   copies, then cm.h's 60 and unk_5d33b0.h's 5 after the new includes. No byte or shape function moved.
   Kept with their reason: d3_par.h's d3_ParEnd / d3_ParHead (d3_par does not compile without them).
   d3_interface's and unk_4591a0's rename-around-include hacks (d3_SetBlendState_int,
   d3_WorldMatrix_xv) went with them. Left: about 1330 function and 270 data copies that are the only
   declaration some includer sees (its include set lacks the owner's header); they go as those units
   include the owners' headers (dcp headers conflicts UNIT OWNER.h, then rerun the redundancy pass).
4. **Headers written** (2021623, 94b7dc7, e1a4736, 2c3836f, aa3e42f, ee5a142, baa83bb): include/ls.h (48 functions, 28 data;
   ls_SavePath is an LSSAVE record: path, main's network block, NETHDR at 0xa0, the version text),
   include/ls_types.h (NETHDR, LSSAVE), include/f3init.h (62, 30), include/sv.h (137, 164). The units
   that had only duplicates in the way include them (ls: am, f3gm, f3init, gv, he, sv, unk_575860,
   unk_58bbd0, main, unk_5d33b0; f3init: cm, d3_interface, d3_vec, he, ls, main, unk_575860,
   unk_589f10, unk_58bbd0; sv: cm, ls, main). cm calls sv_Unk52eaa0 through an int view (@original:
   ai_Unk4d4340 0x4d4340 byte). main's NETHDR / LSSAVEVIEW are gone; 0x936460 is cm's cm_NetGame
   (CMNETGAME in cm_types.h), main's gap blocks of it are gone (main_HostNetworkGame shape -> byte).
   Left: sv.h in f3init (sv_PanelShown: sv defines `unsigned short *`, f3init its INIT_OBJ * view),
   he / unk_575860 / unk_5d33b0 / unk_58bbd0 (draft calls with fewer arguments than sv's definitions:
   sv_Unk524770, sv_Unk52eb50, sv_CreateFormSlider, sv_MessageBox), unk_589f10 (its head uses the
   copies before its last include: move them, then include).
5. **gv's D3COLOR declarations.** Done (3b44540): D3COLOR in d3_types.h, gv includes d3_tools.h,
   its two no-prototype copies are gone; its d3t_*K casts stay (@original, 0x4ee360).
6. **Includes in main.** (d0b6e13) main includes mem.h, d3_d3d.h, io_text.h, d3_snd.h, d3_tools.h, ahm.h, gv.h
   now (and ls.h, f3init.h, sv.h). Still unsettled (src/main/open.md has the details): ch.h / chc.h,
   hmt.h, amt.h, hst.h, cm.h (main's drafts use results of void functions), sblib.h (sb_PlaySample:
   sblib.h and gfx.h disagree), unk_575860.h.
7. **open.md**: done for 38 units (92a91c2): a "headers (2026-10-30, typing pass 2)" line under Open
   work, what head.c includes, which casts / views keep @original evidence, which are views of the
   weak definitions of item 1.
