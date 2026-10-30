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

## Debt
1. **Weak definition types made canonical.** The definitions are char views of typed records, and every
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
2. **Parameters spelled weaker than they are.** vfs_Tfopen's third parameter is a VFSDIR * root (the
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
