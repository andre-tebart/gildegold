# ls.c (new, layout 2026-10-23 session 3)

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** first; below it the findings of each session, one section per author.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): save game writer / reader, network transfer
- Boundaries: separated (plausible) (layout 2026-10-23, 3rd session): new unit, tag "ls_" ("ls:tmp" 0x623654, "ls_LoadCharacter(): ...", "ls_SendGame" 0x62376c). Evidence: .data: its first global 0x62364c (language number) and 0x623650 (the save game version) precede "ls:tmp" 0x623654; strings run monotonic through "SCENARIO", "Savegame", "f3g:PlantMap", the two ls_LoadCharacter errors, "*%s.bgf" / "holzfaeller_%s" / "hozfaeller", "f3_alwin_t:d" 0x62375c, "ls_SendGame" 0x62376c, "rbn" 0x623778; the next block (floats and 0xff tables from 0x6237c4, the "sv" unit: "sv_ChangePlayerHE", "f3_sv:OpenBook", "sv_HandleObjects") starts with 0x520ee0 (a registration function): 0x520b80 + 0x360 = 0x520ee0 (corrected in layout 2026-10-23 session 4: 0x520b80 ends with its `ret` at 0x520eb6, and 0x520ec0, a 32-byte tick callback registered by 0x520ee0, is sv.c's first function, so ls.c ends at 0x520ec0). The unit is the save game: the language table 0x519010, the writers (0x5190b0 + 0x5194c0 header + one per subsystem: 0x519bf0 0x519d20 0x51a1a0 0x51a4b0 0x51aff0 0x51b340 0x51b890 0x51b910 0x51bdc0 0x51c2f0 0x51c450 0x51c560 0x51c870) and the readers (0x51c950 + 0x51cee0 header, 0x51d2d0 .. 0x520310, 0x5205f0, 0x5206b0 fix-up), the network send / receive of a save game (0x520a30 / 0x520b80)
- Range then: 0x519010..0x520ec0 (all drafted)
- Was: tag "ls_"

## Open work
- **headers (2026-10-30, typing pass 2)**: head.c includes f3gm.h, gv.h, ai.h, vfs.h, he.h, txt.h, data_views.h, ls.h, d3_types.h, cm_types.h, ch_types.h, rd_types.h, hst_types.h, f3init.h, sv.h; no @original cast or view; views / casts without byte evidence of their own (weak definitions or drafts' spellings, notes/headers.md debt item 1): ai_PlayerCharC.
- Matched: 0x519010, 0x5190a0, 0x51ad30, 0x51ad60, 0x51c920. Near misses: 0x519b50 (5 B: operand order of `[eax+edx]` in the building loop; the original re-reads 0x889f74 after the store: tried `(int)ptr + k`, `k + (int)ptr`, a cached int base), 0x519bf0 (2 B: `[ebx+ecx]` vs `[ecx+ebx]` of the record pointer), 0x51b890 (2 B, the same), 0x51e750 (2 B: `mov ecx` vs `mov eax` of the last word, tried a temporary), 0x51f650 (2 B: `[ebx+esi]` operand order), 0x51fb90 (105 B: the loop over the 0x29-word table uses a pointer one induction step ahead in ours: tried the `if (n > 0) do {} while` shape).
- ROUGH: the writers / readers converted from the pseudocode (0x51b340 .. 0x51c560, 0x51b910 / 0x51f720: the original returns 0 through duplicated inline epilogues (`ja` + pop pop xor ret) where ours shares one block: the results of 0x430e30 / 0x51c920 are compared as unsigned `<= 0` in some functions (ja / jbe) and `== 0` in others, so the source style differs per function). The functions 0x51bdc0 / 0x51fb90 reuse the file handle argument slot as a temporary in the original (Ghidra shows `param_1 = 0`), ours does not.
- 0x51a4b0 / 0x51de70 are the character record (0x21c bytes at 0x770b80) writer / reader: the field list is in the code, one by one.
- Names: unk_Unk<addr>; the unit is the save game ("ls_" tag).
- 0x519bf0 / 0x51b890 fell behaviour -> draft with commit 7b71ec80 (the K&R `unsigned ai_Unk430e30()` was rewritten to the definition's `int`): vfs_fwrite (0x430e30) returns an unsigned count like fwrite - the original compares it unsigned (jbe / ja here, hmt.c casts it to unsigned int) - so the definition's `int` was the draft; `unsigned` everywhere restored both (2026-10-24). Not a stale object.

## layout 2026-10-23 (session 3)
- `LS_W(p, sz)` = `if (ai_Unk430e30(p, sz, 1, f) <= 0) return 0;` (0x430e30 = fwrite, an unsigned result: `jbe`), `LS_R` the same with 0x51c920 (fread, a thin wrapper of 0x430bd0, `n > 0`). The save version is kept in 0x623650 (0x1005a for a new save; the loader accepts 0x10026 .. 0x1005a); the fields added in later versions are guarded by `>= version`: Ghidra's `0x10012 < v` is `v >= 0x10013` in the source (the compare constant changes the bytes: `cmp .., 0x10013; jl`).
- Save order (0x5190b0): header 0x5194c0, the globals, 0x519bf0 (object list), 0x519d20 (buildings), 0x51a1a0 (the 0xac-byte records of 0x7d5f80), 0x51a4b0 (characters), 0x51b340 (players), 0x4ea690, 0x519b50, then when bit 2 is clear 0x4f28d0, 0x51b890 (the two byte maps), 0x51b910 (the global tables), 0x4ed1e0, 0x51bdc0 (the 0x14c-byte records), 0x51c2f0 (the party), 0x51c450, 0x51c560, 0x51c870, 0x51aff0, 0x4b62e0, 0x589aa0, 0x5a8940, 0x4ebfb0, 0x4adb20, 0x4bff40; the loader 0x51c950 reads them in the order 0x51d3d0, 0x51d550, 0x51db00, 0x51de70, 0x51f080, ... (see the comment).
- 0x520a30 (send a save over the network in 0x80-byte blocks, "ls_SendGame") / 0x520b80 (receive: the memory file 0x4311d0 of the buffer 0xf31140).

## match-base 2026-10-24 (base-1 worker, shard 1/5)

Unit ls.c (queue position 2 of shard 1/5 after chc.c):
- Raised: unk_Unk519010 -> ls_LanguageOf and unk_Unk5190a0 -> ls_GetLanguage (byte + readable;
  ai_Unk62364c -> ls_Language, ai_Unk5ee978 -> ls_LanguageNames; callers in main.c, ai.c, gv.c,
  he.c renamed), unk_Unk519bf0 -> ls_WriteObjects (behaviour + readable; ai_Unk7e3f34 ->
  ai_Objects project-wide, the 0x430e30 calls are vfs_fwrite now - ls.c includes vfs.h).
- gv.h's mem_Alloc was `const char *tag`, vfs.h/mem.c have `char *tag`: with both headers in ls.c
  lint errored; gv.h now matches mem.c. vfs.h's vfs_fwrite address comment carries text (Q6).
  Lesson: Q6 was satisfied for vfs_fwrite by the old ls.c prototype comment `/* 0x430e30, ... */` -
  deleting it dropped vfs_fwrite to draft until vfs.h described it.
- Escalated (all @deep reasons in the source): unk_Unk5190b0 (hoisted push esi scheduling; 45
  callee/global names of other units; ROUGH), unk_Unk5194c0 (586 code hunks, frame 0x124 vs the
  draft's 0x50, needs a rewrite from the disassembly), unk_Unk519a00 / unk_Unk519aa0 / unk_Unk519b50
  (register-order / reload shapes, meaning verified equal), unk_Unk519d20 (edi/esi swap through the
  whole body, n in a stack slot) and unk_Unk51a1a0 (base pointer in eax vs original esi).
- 0x519a00 loop: `b = ai_Unk889f74 + k; ...` compiles without the original's reload of the global
  after the store; with the global read directly in the loop condition the reload appears (kept).

## match-base 2026-10-24 (base-1 worker, shard 1/5, session 2)

Same unit, continued (queue position: next was 0x51a4b0; the small items were taken first):
- Raised to byte + readable: ls_ActionIndex (0x51ad30), ls_ActionAt (0x51ad60), ls_WriteAvatars
  (0x51bdb0). Raised to behaviour + readable: ls_ReadRelations (0x51f650, B: regs 4) and
  ls_WriteRelations (0x51b890, B: regs 4); their addresses 0x7e5360 / 0x889f80 / the ends
  0x875360 / 0x919f80 came from the relation matrices, so Q3 cleared with the renames.
- 0x631050 is ch.h's `CHACTION *ch_Queue` (action queue, 0x194-byte records, type at +9, prev /
  next at +0x24 / +0x28 - the save turns the two pointers into indices). ls.c names it
  ls_ActionQueue (char *) instead: including ch.h here fails (it needs d3.h after windows.h /
  d3d.h, and gv.h's K&R `int d3sm_GetVertex();` then collides with ch.h's typed declaration -
  ord.c 0x4f86a0 calls it through the K&R one, so the typed view is blocked for ls.c; type debt).
- Project-wide renames: ai_Unk7e5360 -> ai_Relations (the relations between characters, cm.c
  0x4c8240), ai_Unk889f80 -> ai_RelationMemory (the second term the same handler zeroes /
  re-derives from the relation, added to relation/2 in ai.c), ai_Unk623650 -> ls_Version (the
  save game version; gv.c / gv.h use it too).
- gv.c: unk_Unk4ed1e0 -> gv_WriteAvatars (writes the 0x20 avatar records); its UNK_FW cast-macro
  calls became vfs_fwrite calls (byte + readable, bytes unchanged). The UNK_FR sibling in
  0x4ed270 stays.
- Next in the unit: unk_Unk51a4b0 (0x51a4b0, 1101 asm lines: the 1267-line asmdiff is dominated
  by the original's pointer bias edi = c+2 and its LS_W constant folding; the meaning is the field
  list of the AICHAR records, and most of its raw fields have no evidenced name yet), then
  0x51ad80 (r = a scene object: +0x34 bbox, +0x64 / +0x68 texts, +0x88), 0x51aff0 (the same
  ch_Queue / CHACTION table, needs the typed view above).
- Session 2, later batch: ai_Unk770b80 -> ai_Chars project-wide (348 refs, 16 files; the
  0x21c-byte character records are ai.h's AICHAR ai_Chars[768]; the other units give a
  char [][0x21c] view, so the four views are now in lint_baseline.txt as type debt) - it made
  ls_ReadRelations / ls_WriteRelations readable and cleared Q2 in many units. ls.c gains
  ls_ClearObjectFlag (0x51d530, scene callback clearing flag 4 at +0x213).

## match-base 2026-10-24 (base-1 worker, shard 1/5, session 3)

Queue position 1/5; the small items were taken first as before.

- unk_Unk51a4b0 (0x51a4b0, the character writer) -> match-deep. Meaning verified against the original at
  0x51a5b4..0x51ad15: the field list, order, sizes (1/2/4/5/0x10/0x18/0x20/0xa8), the four reference fields
  (+1 / +4, -1 for none) and the -0x59a / -0x639 offsets. Fixed: the two manual vfs_fwrite checks were `!= 0`
  (je), the original compares unsigned (`jbe`, LS_W); the record loop now uses the natural unbiased pointer
  (c + 0x20c ..) and VC6 biases the induction variable to +2 by itself - the original does exactly that. A
  source written with the bias (`c = ai_CharKinds` at 0x770b82, or `ai_Chars[0] + 2`) is folded back by VC6, so
  do not write the bias in C. Escalated for: slots (the original keeps local_c / cnt / local_4 in the top local
  slot [esp+0x1c], ours puts local_c in [esp+0x20], cnt in [esp+0xc]) and push scheduling (the original hoists
  push esi / 1 / 4 above the je of the table loop and of the pointer-field blocks).
- unk_Unk51ad80 (0x51ad80, a character scene record) -> match-deep. Fixed: blank[0x41] -> blank[0x40] (frame
  0x60 -> 0x5c), the second zero piece at zero+0xc not zero+0x10 (the original writes 0x10 + 0xc overlapping),
  the two final fwrite checks are per branch (`!= 0x40` -> return 0, the original compares and returns in each
  arm), and the q conditions are written from the non-zero case (the original lays out the non-zero arm inline).
  Only hunk left: the 0x1c-byte zero substitute - the original writes 7 mov DWORD PTR [esp+X],0 (0x51aea1), VC6
  always produces rep stosd for us (int[7]={0}, char[0x1c]+memset, a zero loop, two arrays, a struct = {0} and
  a member-wise struct all tried; the last two force ebp). Needs the shape a vargen round can search.
- unk_Unk51aff0 (0x51aff0, the scene records + the 0x194-byte table) -> match-deep, with a meaning fix: the
  draft wrote rec+0x70 (0xa0 bytes), rec+0x2c and rec+0x38; the original writes rec+0xf0, rec+0x24, rec+0x28
  (0x51b262 / 0x51b29f / 0x51b2b9), which is exactly what the reader in unk_Unk51e900 reads back (t+0x3c, t+9,
  t+10). Also: the flag and the rec memset happen once before the fourth loop (0x51b165..0x51b177), and both
  table loops re-read ls_ActionQueue every iteration. Remaining: register allocation / scheduling (the
  original's first count loop runs in eax/ecx before any callee-saved push, which comes only at 0x51b013).
- unk_Unk51b340 (0x51b340, the player writer) -> match-deep, recorded only: four wrong pointer scales from the
  Ghidra conversion - the four block header writes use 0x7d6a60 + k*0xaa4 where the original computes
  0x7d6a60 + k*0x2a90 (0x51b375..0x51b38a), the +0x40 avatar name uses 0x7e3f80 + i*0xbd and the +0x54 word
  0x7e3f94 + i*0x17a where the original uses slot + 0x40 / + 0x54 (0x51b543 / 0x51b55c). 0xaa4 = 0x2a90/4,
  0xbd = 0x2f4/4, 0x17a = 0x2f4/2: Ghidra's lost pointer scale. Fixing only these made unitdiff WORSE
  (261 -> 318 differing instructions), so the function needs the rewrite with the fix. Its first write check is
  signed in the draft (`!= 0`), the original uses unsigned `ja`.
- unk_Unk51c920 -> ls_Read (0x51c920, byte + readable): the wrapper now calls vfs_fread (vfs.h's prototype at
  0x430bd0) with named parameters; the LS_R macro uses it.
- unk_Unk51fb80 -> ls_ReadAvatars (0x51fb80, byte + readable) and unk_Unk4ed270 -> gv_ReadAvatars (gv.c /
  gv.h, the avatar table reader, the counterpart of gv_WriteAvatars); ls.c's local K&R declaration dropped,
  gv.h's prototype is used.
- Blocked here: 0x51d4f0 (byte, draft) needs Q7 for 0x50d380 (declared K&R in cm.h, defined in f3init.c which
  has no header) and Q4 for the 0x889f74 building records (ai_Blds exists in gv.h; ls.c's duplicate
  `extern char *ai_Unk889f74` view can go). A typed cm.h prototype must first be checked against every call site
  (other units pass char* / unsigned short* mixes).

## match-base 2026-10-24 (base-1 worker, shard 1/6, session 4)

Queue position 1; ls.c 26/48 -> 15/48 below behaviour + readable, check count 2510 -> 2511.
- **byte + readable**: ls_ReadObjects (0x51d3d0). The record address must be written as an index
  (`r = (char *)ai_Objects + i * 0x43`): the `off` accumulator form (`off += 0x43`) makes VC6 emit the
  induction-variable init *before* the loop guard and `add ebx` before `inc ebp`; the index form gives the
  original's preheader `xor ebx,ebx` after `jle`, the `jl` back edge and the `lea esi,[ebx+ecx]` operand order
  exactly. 288/288 bytes, no other change.
- **behaviour + readable**: ls_WriteHistory (0x51c450). The draft's `if (vfs_fwrite(...) != 0) { ... } return 0;`
  wrapper made VC6 merge every failure return into one end block (the original has inline `ja` + epilogue after
  each check); with the third value write as LS_W and the two record loops as `for`s with LS_W inside, the first
  372 instructions became equal (260 -> 224 differing bytes); only 8 order hunks (the size/fwrite argument
  scheduling) remain. Q3/Q4 cleared with hst_TextsOn / hst_TextFirst / hst_TextCurrent / hst_KeyFirst /
  hst_KeyNext / hst_Key / hst_Lists (ls_WriteHistory is the counterpart of the reader 0x520200) and an int view
  of hst_Lists (hst.h's G12ROW cannot be included here - hst.h -> am.h -> ai.h conflicts with gv.h).
- **0x51d4f0 -> ls_HookBuildingCB**: byte kept; Q7 and the ai_Blds view resolved as the previous session asked
  (0x50d380 -> init_HookObject, one prototype now in cm.h, f3gm.c, he.c and ls.c, matching the f3init.c
  definition; ai_Unk889f74 -> ai_Blds from gv.h). Q4 escalated: the two raw reads need AIBLD (ai.h; cannot be
  included - 30 compile errors against gv.h's char* views) and a named D3OBJECT +0x200 (d3.h has pad200 there).
- Escalated to match-deep with the reading in the source: 0x51b910 (two loops' induction-variable base; the
  LS_W early-return rewrite alone took 361 -> 81 code hunks), 0x51bdc0 (Ghidra element-index strides: the
  0x14c-byte records are written at +i*0xa6 / +i*0x53 / [i*0x53], the 0xa4- and 0x50-stride pools are stepped by
  0x29 / 0x14 bytes - needs the rewrite, not a patch), 0x51c2f0 (counter in the stack slot vs ecx, IV base
  0x925534 vs the record start), 0x51c560 (the 0x99-byte record copy goes through Ghidra's int[36] +
  local_b/local_7 aliases; the original copies into a 0x99-byte buffer and stores the indices at +0x91/+0x95),
  0x51c870 (IV base after the LS_W restructure), 0x51c950 (version bounds now `>= 0x10033` etc.; the rest is the
  &&-chain shape and register allocation), 0x51cee0 (register/stack shape; version bounds aligned), 0x51d2d0
  (size select and the esi save scheduling).
- Renames (project-wide): unk_Unk51c450 -> ls_WriteHistory, ai_Unk6f0724 -> hst_TextsOn, ai_Unk601058 ->
  hst_TextFirst, ai_Unk601060 -> hst_TextCurrent, ai_Unk601064 -> hst_KeyFirst, ai_Unk60106c -> hst_KeyNext,
  ai_Unkf83520 -> hst_Key, ai_Unkf83540 -> hst_Lists, ai_Unk6f1bdc -> f3gm_ObjectCount (f3gm.c: "the number of
  object instances in use"), unk_Unk51d3d0 -> ls_ReadObjects, ai_Unk50d380 -> init_HookObject, unk_Unk51d4f0 ->
  ls_HookBuildingCB.
- Type debt: ls.c cannot include ai.h (gv.h's char* views of ai_Blds/ai_Chars/ai_Unk501190/... conflict, 30
  errors) nor hst.h (am.h -> ai.h, 37 lint errors); init_HookObject is now the one prototype in cm.h even though
  f3init.c defines it.

## match-base 2026-10-24 (base-1 worker, shard 1/6, session 5)

Queue position 1; ls.c 25/48 -> 26/48 below behaviour + readable, check count 2514 -> 2515.
- **byte + readable**: none new at readable; **byte** (draft readability): ls_ReadPlayerRecords (0x51db00,
  was unk_Unk51db00) -> match-deep, see below.
- **behaviour + readable**: ls_ReadCharacter (0x51e750, was unk_Unk51e750). It was behaviour already; Q1/Q2/Q7
  cleared (ch_AllocAt through a local prototype - ch.h needs d3.h and conflicts with gv.h here, baseline type
  debt) and Q4 cleared with two typed pointer views: `unsigned short *fl = (unsigned short *)(ent + 0x8c)` for
  the flag word and `unsigned char *flagByte` for its low byte (bit 5 set when the flag byte read is 1), and
  `int *ref = (int *)(ent + 0x12c)` for the saved reference index (0x51e900 turns it into a pointer with
  0x505df0 / 0x4ffe90). The pointers MUST be assigned immediately before their first use: declaring and
  assigning them right after ch_AllocAt hoisted the `lea` and swapped ent / f between edi / esi, which dropped
  the function to draft (52 regs + 28 code hunks); with the assignments in place it is back to `regs 6`.
- ls_ReadBuildings (0x51d550, was unk_Unk51d550) -> match-deep: the meaning is verified instruction by
  instruction; the plant map loop now re-reads the pointer field (+0x71) on every read like the original
  (`mov ecx,[esi+0x71]` before each call; 1200 -> 935 differing bytes). The remaining code hunks are source
  shape: the off=0 / k=0 preheader and the k / off stack slots ([esp+0x10] / [esp+0x20] swapped), the K/L/M and
  H/I/J version branches laid out with the old-version arm inline (the original puts the read inline), and the
  i / j register pair of the building-site loop. Q4: the 13 raw accesses need AIBLD's fields (ai.h blocked).
- ls_ReadPlayerRecords (0x51db00) **byte**, Q3/Q4 open -> match-deep. What made it byte: `v` removed so every
  version check reads ls_Version directly (287 instructions became equal; a `v = ls_Version` local lives in
  ebx and reloads at different points), the bounds written as the original's `>= 0x1002c / 0x10014 / 0x10015 /
  0x10018 / 0x10046`, the 0x5e branch with the read inline, and the loop as `do { ... p += 0xac; } while ((int)p
  < 0x7d6a42);`. Q3 stays because the bound is an image literal: an extern at 0x7d6a42 (tried) gives 84
  differing asm lines, `(int)gm_PlayerRecordBytes + 0xac0` doubles the frame - only the literal folds to the
  original's `cmp esi,0x7d6a42; jl`. Q4: the one raw access is `*(int *)(p + 0x5e) = 0x3f800000` (the 1.0
  default), needs the 0xac-byte record fields.
- ls_ReadCharacters (0x51de70, was unk_Unk51de70) -> match-deep: Q4 (8 raw accesses need AICHAR's fields:
  the reference fields +0x54 / +0x18c, the defaulted +0x20c / +0x190 / +0x1cc, the zeroed +0x16c / +0x170 /
  +0x17c) and 467 code hunks = the Ghidra conversion's shape (order, offsets, sizes and version gates read
  against 0x51de70..0x51e74f and correct) - the rewrite that 0x51a4b0 has as @deep. Q1/Q2/Q3/Q6 done.
- Renames (project-wide): unk_Unk51d550 -> ls_ReadBuildings, unk_Unk51db00 -> ls_ReadPlayerRecords,
  unk_Unk51de70 -> ls_ReadCharacters, unk_Unk51e750 -> ls_ReadCharacter, ai_Unk889f74 -> ai_Blds (gv.h's
  existing name; ls.c's duplicate extern dropped, mem_Alloc / gm_ClearBuildings / realloc used instead of the
  K&R ai_Unk4223c0 / ai_Unk4fe7f0 / ai_Unk5e05e6), ai_Unk4efc10 -> gv_AllocGraveyard, ai_Unk4f1aa0 ->
  gv_ReadGraveyard (gv.c / gv.h), ahm_Unk4ed040 -> ahm_ReadRecord (ahm.c / ahm.h), ai_Unk6f1bc0 -> ai_CharCount
  (6 files), ai_Unk7d5f80 -> gm_PlayerRecords (f3gm.c) with gm_PlayerRecordBytes at 0x7d5f82 (ls.c, the +2 byte
  view), ai_Unk6f1bfc / ai_Unk6f1c00 -> ai_JudgeCatholic / ai_JudgeProtestant ("the two judges" in f3init.c),
  ai_Unk6f1c04 -> ai_KindBChars (the eight slots of the kind 0xb characters); gm_ClearBuildings got its
  prototype in f3gm.h and mem_Alloc / realloc come from gv.h / stdlib.
- Type debt added to lint_baseline.txt: ch_AllocAt is `void *` in ls.c and `CHARACTER *` in ch.h (ch.h needs
  d3.h, which conflicts with gv.h on this include path); ahm_ReadRecord keeps a local prototype in ls.c
  because ahm.h's `const char *tag` for mem_Alloc conflicts with gv.h.
- ai_Blds is now used in ls.c (gv.h); ls.c's ai_Unk7d5f82 extern is gone (the byte view is gm_PlayerRecordBytes).

## match-base 2026-10-24 (base-1 worker, shard 1/5, session 6)

Queue position ls.c; check count 2518/4663 before and after; no grade fell.
- **unk_Unk51e900 -> match-deep**. Every hunk read against 0x51e900..0x51f07f. Meaning verified equal
  (the LS_R field order / offsets, the 0x194 memset per record, the version bounds, the object walk
  0x1468ae0..0x14696e0, the owner split < 0x770b80 / >= 0x7d5f80, the two messages to dummy / file,
  the -1 link checks). One type fix: `col` is a **char** in the original (byte store `add al,0x67`, then a
  dword reload at 0x51ecda..0x51ece5), `int col` -> `char col` (650 -> 647 hunks). Left = shape only:
  frame 0x2a0 vs 0x2a4 (the original keeps col in the frame+0xc byte, which pushes part / bld / the buffers
  4 bytes up; VC6 keeps ours in al + movsx), the outer object loop's preheader (ours tests, the original is
  a do-while from the stored constant), our duplicated -1 check in the else arm (the original shares one at
  0x51efe3), the queue pointer hoisted out of the read loop, register allocation.
- **unk_Unk51f080 -> match-deep**, with the source fix that halved it: the slot loop uses TWO base
  pointers - the original keeps s = 0x7e3f88 in the stack slot [esp+0x10] (reloaded into ebx each iteration)
  and p = s - 0x48 in edi, using s for the reads up to s+0x19 and the first 10x8 loop and p for everything
  from p+0x104 on. Writing `p = s - 0x48;` as its own pointer took 180 -> 94 code hunks (156 -> 74 asm
  lines). Also the version bounds are written from the >= side (>= 0x10048, >= 0x10037).
  **The loop bound is on s, not on the slot base**: `while s < 0x7e4b58` after s += 0x2f4 - 4 slots; a
  p-based bound (`p > 0x7e4b57`) reads 5 and was WORSE, reverted. A do-while restructure of the tail was
  also tried and made it worse (duplicated loop head). Left: the s stack slot, the original's xor eax,eax
  in the shared return-0 block, the loop rotation, and the two strcpy inlines with a dst-src bias.
- **unk_Unk51f720 -> match-deep**: a long flat list of literal-address reads; every failed read ends in an
  inline `xor eax,eax; pop...; ret` epilogue in the original (31 of them), ours merges them into one shared
  return-0 block - that is the whole 327-hunk difference plus one IV re-base. Meaning verified (the 26
  leading 4-byte reads, the 5x4 loop 0x91ab04..0x91ac44, the four 0x44 reads, the 8x(2,2,1,4) record loop
  0x91ae62..0x91b461; our `0x91b461 < p` is the original's `jl 0x91b461`). Q3 blocks readability: ~45
  unnamed globals in 0x91b4c0..0x91b690.
- **unk_Unk51fb90 -> match-deep**: meaning verified; left are a store-order pair and the middle-loop IV
  base (the original's pointer is 8 bytes above ours, same 0x74e1e8 start). Q3 on 0x6fab40 / 0x74db40 /
  0x74e1e0 / 0x758ca4.
- **unk_Unk520000 -> match-deep**, two real fixes: the temp buffer is **0x1000** bytes (the original frame
  is 0x1008 and starts with __chkstk; ours declared tmp[0x400] for a 0x408 frame), and the three version
  branches are written from the >= side so the non-zero arm is inline (the original `jl`s to the < arm).
  131 -> 95 code hunks, 100 -> 70 asm lines. Left: the IV base (ours 0x925540, the original 0x925534) and
  i in edi vs the original's esi (entangled with the inlined memcpy's esi).
- **unk_Unk520200 -> match-deep**: the same inline-epilogue shape; meaning verified (hst_TextsOn,
  0xf83520 x16, the 8 word pairs from 0xf83548, the 0xf83657 bound). Q3.
- **unk_Unk520310 -> match-deep**: meaning verified; the IV base only (ours esi = p+4, the original p) plus
  the epilogue shape. 12 image literals.
- **unk_Unk5205f0 -> match-deep**: meaning verified (0x760610, the 24 entries of 0x7604e8 at -0x60 / 0 /
  +0x68 / +0xc8, the 0x10050 gate, 0x5dc0c0 / 0x4ed910); VC6 re-bases our loop pointer to p+0x68.
- Next in the unit: **unk_Unk5206b0** (333 code hunks, frame 0x14 vs the original's 0xc - two extra
  locals - and every pointer re-based by 4, see the first loop's `[esi-0x16c]` vs `[esi-0x170]`), not
  started beyond the head of its diff. Started: the character loop is now written from the record base
  (`r = 0x770b80 .. 0x7d5f80`, stride 0x21c = ai_Chars, p = r + 0x170; the writer/reader offsets are the
  same addresses), 334 -> 333 hunks. The original keeps its IV at 0x770cf0 (= the record + 0x170, our old
  form), so the re-base to +4 is VC6's; the frame difference (2 extra spilled locals in ours) is open.
  A scan of all literal operands in the diff found no difference (0xffff, 0xef, the kind bytes and the
  callback literals agree); the rest is IV re-basing by 4 ([esi-0x16c] vs [esi-0x170], [esi+0x10] vs
  [esi+0xc]) and stack-slot choices ([esp+0x14/0x18/0x1c] vs [esp+0x10/0x14/0x18]).
  Also: the original's first (character) loop IV is the **biased** pointer 0x770cf0 .. 0x7d60f0
  (`cmp esi,0x7d60f0; jl 0x52071d`), i.e. the record + 0x170 as in the old draft; the child-list walk is
  `while (link->+0x3f != 0) link = link->+0x3f;` (0x5207f8..0x520808), rewritten to that shape (no codegen
  change), and the unused n/k locals are gone. The object loop's goods test is `ai_Goods[kind*0x40 + kind]`
  (the original's `shl ecx,6` + `[eax+edx]`), which our `kind * 0x41` matches.

## match-base 2026-10-25

Queue position ls.c; check count 2522/4664 before and after; ls.c 19847 -> 19825 differing bytes;
0x520b80 475 bytes / 114 asm lines -> 443 / 72; no grade fell, nothing WORSE.
- unk_Unk5206b0 -> match-deep (reason in the @deep tag). 333 code hunks, all hunks read (the earlier
  session's literal scan repeated: 0xffff, 0xef, the kind bytes and the callback literals agree); left are
  the IV re-based by 4 ([esi-0x16c] vs [esi-0x170]) and the stack-slot / hoisted push ebp+edi shape.
  Far draft (221 asm lines), no menu pass.
- unk_Unk520a30 -> match-deep, after one meaning fix: the ai_Unk430aa0 close in the `buf == 0` arm takes
  the handle - the original pushes edi at 0x520a9e where our call had no argument. 67 -> 59 asm lines.
  Left: the length expression is a signed divide in the original (`cdq; and edx,0x7f; add eax,edx`) but
  our explicit-mask form gives `sar esi,0x1f; and esi,0x7f`; the fh / sent registers split as ebx / edi
  (original edi / a stack slot), `p` as int vs the original's char* advance, and the push scheduling.
- unk_Unk520b80 -> match-deep, 475/114 -> 443/72, four fixes:
  - 0x75e13c is the 2-byte last field of the clock copy (cm.h: `char ai_Unk75e130[14]`): the original
    loads / stores a word at 0x520cf9 and 0x520d14 (and the loader at 0x51caf8 does too). Our `int` view
    wrote 4 bytes, overran the block by 2 and compiled to a 4-byte store.
  - `v = ls_Version` removed: the original reads the global directly at each check (0x520d2f, 0x520d50);
    the bounds are written as `>= 0x10030`, `< 0x1003d`, `>= 0x10045` and `hdr[0] <= 0x1005a`, matching
    the original's compare constants (this took 114 -> 74 asm lines).
  - ai_Unkf81220 / ai_Unka68d28 are unsigned (cm.h and ls.c): the original compares them `jae` at
    0x520ba1. cm.c compiles unchanged (unitdiff 281/477, 60549 bytes before and after).
  - The character id test is `*(unsigned short *)(p - 2) != 0xffff` (0x520de9 materialises 0xffff;
    our `short != -1` gave `or ecx,-1`).
  Reverted as WORSE (but correct, for match-deep): passing fh to the two ai_Unk430aa0 failure arms - the
  original pushes esi at 0x520db3 / 0x520dd3 before the `jne`, ours pushes at the call site, so unitdiff
  read 469 bytes / 120 asm lines. Same hoisted-push pattern as 0x5190b0.
  Open: the frame is still 0x100 vs the original's 0xfc (one int). The header buffer is `int hdr[0x40]`
  (0x100); the header has fields up to +0x9c so `int hdr[0x3f]` (0xfc) is plausible but needs the title's
  real length before it is a safe change.
- unk_Unk519b50 rose from draft to behaviour through the rebuild (stale grade, no source change); the
  other names.csv rises of this session's `uv run build` results are the same kind.

## match-base 2026-10-25 (session 2)

Queue position ls.c; check count 2522 -> 2523/4664 (0x519aa0 rose to byte as a side effect); no grade
fell, nothing WORSE, lint/ledger/link/eol clean.
- **ls_SendGame (0x520a30) -> behaviour + readable.** 183/320 bytes, 59 asm lines -> 8/336 bytes,
  2 asm lines (one `order` hunk: our `sent = 0` store sits after the jle, the original's between test
  and jle). Leads, each one build: `len / 0x80` instead of the explicit mask (the original's
  cdq/and/add/sar, 183 -> 107); `char *p` (neutral); `cnt++ % 16` with an int cnt (the signed-mod idiom
  and the register-held count, 107 -> 99/33 lines); the increments written p then sent (not sent then p):
  8 bytes in one step, because that order decides the loop allocation. Block-scope `sent` made it
  memory-resident in the dead parameter slot; function scope gave it a register. A bare block before the
  if and a late declaration were tried (WORSE / C89 rejects).
- **ls_FixupAfterLoad (0x5206b0) -> match-deep with the record in the @deep tag.** 663/880 bytes,
  221 asm lines -> 552/896 bytes, 165 lines, code hunks 333 -> 223, frame 0x14 -> 0xc equal.
  Q1/Q2/Q5/Q6/Q7 pass; Q3 gap 0x74e1e0/0x7585e0 and Q4 gap AIOBJ +0x24 as in the tag.
- Renames (project-wide, evidence in the defining units' comments): ai_Unk4e50a0 -> cm_PartyById,
  he_Unk53e400/ai_Unk53e400 -> he_FindEvent, he_Unk56b780/ai_Unk56b780 -> he_ShowConstruction,
  sv_Unk5241d0/ai_Unk5241d0 -> sv_CountGroupMembers, gm_Unk50cc60/ai_Unk50cc60 -> gm_ResetStock,
  ai_Unk6f25ac -> ai_GameState (0 outside a game, 2 while a load re-initialises the events),
  ai_Unk6f1bf8 -> ai_PlayerChar, ai_Unk758600 -> he_RunHandlers, ai_Unk74db40 -> he_QuickList,
  ai_Unkf31166 -> ls_RefTargets. ls.c now uses the units' own names for the already-named callees
  (gm_FindChar / gm_FindById / gm_FindObjectInst / gm_FindBuildingById / gm_BuildQuery /
  d3if_SelectScene / d3_CurrentScene); f3gm.h got the four lookup prototypes, he.h he_ShowConstruction,
  gv.h's he_FindEvent now matches he.h's varargs form and cm.h's d3if_SelectScene is typed.
- Type debt: ls.c still cannot include cm.h (it redefines gv.h's globals: ai_Unk501190 etc.), so its
  cm_* prototypes stay local; he.h IS includable once gv.h's prototype matches. ai_PlayerChar has the
  int-view/pointer-view split the file already had.
- Side effect: 0x519aa0 is byte now; the signed int form of its reference loop bound
  (`(int)p < (int)ls_RefTargets + 0x50000`) was its last difference. 0x519a00 dropped 71 -> 35 bytes.

## match-base 2026-10-25 (base-4 worker)
- ai_Unk5190b0 -> ls_SaveGame, ai_Unk766320 -> ai_ScreenImage (0x766320), ai_Unk6f22bc -> ai_FrameWait. The mem_Alloc tag of 0x5d7320 is "ls:tmp" 0x623654 (the Ghidra s_ls_tmp rendering hid the colon).

## play 2026-10-28 (tutorial crash: the full link's data addresses)
- `ls_ReadPlayerRecords`' loop bound was the original's absolute literal `0x7d6a42`. That only holds
  when the image reproduces the original's addresses; the full link's `.text` is ~0x43000 shorter, so
  `.data` and the COMMON area shift and the band read ~70 records instead of 16, hitting EOF and
  failing the whole `unk_Unk51c950` load ("Tutorial" start -> `main_Exit`). Now
  `(int)((char *)gm_PlayerRecords + 0xac2)` (a byte offset; `gm_PlayerRecords` is `int[]`, so the
  pointer form `gm_PlayerRecords + 0xac2` would step 0xac2 *ints*). Costs the function its `byte`
  grade (71 asm hunks, +16 bytes); the baseline row was lowered to `draft` by hand because
  `uv run ledger promote` refuses a fall.
- `unk_Unk51f080`'s stock-block loop bound read `(unsigned)ai_Unk7d6a64`, but ls.c declares that
  symbol as a *scalar* (`extern unsigned char`), so the bound was the byte value (0) instead of the
  address: the five 0x2a90 blocks were skipped, the .cty position stayed ~0x42b8 bytes behind and
  `amt_LoadAemter` then read a garbage count (0x40000000). With `(unsigned)&ai_Unk7d6a64 + 0xd4d4`
  the loop runs and the diff drops 267 -> 91 asm lines.
- Still open after that: `init_HookObject` dereferences `ai_BTypes` (NULL at that point) while the
  .cty load hooks its scene objects -> `sprintf("gb_%s", <0x1703>)` in `__output`. `ai_BTypes` /
  `ai_Blds` are allocated by `gm_LoadTables`, which in turn writes its five stock blocks through
  `ai_Unk7d6a64` (a 12-byte COMMON symbol here, a 5 x 0x2a90 area in the original) - the same
  split-symbol / absolute-address problem, one level up.

## play 2026-10-28 (session 2: the tutorial start, the data layouts behind it)

The tutorial now starts and runs (`uv run dbg --start tutorial --seconds 90` exits 0); the fixes are
all *data declarations*, not function bodies. The old note above was right about the class of bug but
not about the first witness: a watchpoint on `ai_BTypes` showed `gm_LoadTables` setting it to a heap
pointer and then `ord_ResetStatistics+0x13` zeroing it again.
- **`gm_IncomeHistory` (gv.c) is 0x154 bytes, not 64.** It is five rows of 17 ints (stride 0x44):
  `ord_ResetStatistics` memsets the whole block, and the five current sums
  `ai_Unk91b5c0 / 91b604 / 91b648 / 91b68c / 91b6d0` are the *last int of rows 0..4* (offsets 0x40 /
  0x84 / 0xc8 / 0x10c / 0x150), not separate COMMON objects. With the 64-byte declaration the COMMON
  linker put `ai_BTypes` at +0x50, inside the 0x154-byte memset; `init_HookObject` then read
  `0x1703` as `gb_%s`. f3gm.c now defines the five names as views of `gm_IncomeHistory`; ls.c's
  save writer/reader write the five rows as `gm_IncomeHistory + k * 0x44` (the save format is
  unchanged; this is the `0xaa4`/`0x44` row spellings for 0x51b340 / 0x51f080). The 0x51b340 writer
  still has the known Ghidra pointer-scale errors (0xaa4 for 0x2a90 in its header writes); it is not
  on the load path and stays a match-deep rewrite.
- **The stock area is one 0xd4d0-byte object at 0x7d6a60**, five blocks of 0x2a90 (four header ints
  then 0x55 records of 0x80). f3gm.c now defines `char ai_Unk7d6a60[0xd4d0]` (tentative, COMMON) and
  `ai_Unk7d6a64` / `init_StockBlocks` are views (`+4` / `+0x10`); hst.c's `G12STOCKBLOCK *` view and
  ls.c's writer/reader bases use it too. The old 12-byte `ai_Unk7d6a64` let `ai_Objects` be placed
  inside the span at 0x76e4a0: `unk_Unk51f080`'s first record write (amount at record+4) stored over
  the `ai_Objects` pointer itself, so `ls_FixupAfterLoad` read the word at `[ai_Objects]` with
  `ai_Objects == 1`.
- **`net_Conn` is the full 0x14-byte `NETCONN` at 0x1460ae0** (net.c), and `ai_Unk1460ae0` is only
  the alias of its first field. The old definition was the 4-byte `int ai_Unk1460ae0`; `closed` at
  +0x10 therefore read `ai_CharCount`. In the single-player tutorial `ai_CharCount` was 4, so
  `cm_ExecCommands` took the `net_Conn.closed` branch, failed all slots and popped to the main
  loop. `init_StartNewGame` then got `cm_GetSlotValue(13) == 0` for `ai_JudgeCatholic` and
  dereferenced it (`[eax+4]` with eax=0).
- **The two list heads are fields of their head sentinels, not standalone COMMON pointers.**
  - Stock list: `d3_Unkfc2e00[0x200]`, head at +0x1fc (`d3_StockListHead`). `d3io_LoadObjectFile`
    writes the first object to the sentinel's next field; with a separate pointer the writes and
    `d3io_FindStock`'s reads were 0x2c64 bytes apart, so every `ch_CreateMesh` failed
    (`feldarbeiter_SOLDAT`, `holzfaeller_SOLDAT`, `arbeiter_KARREN`) and `ch_PreloadAnims(NULL, 1,
    "bewegung/karren_ziehen")` read `ch->body` at 0x130.
  - Animation list: `d3_Unkfc2580[0x168]`, head at +0x160 (`d3_AnimListHead`), prev at +0x164.
    The 0x160-byte declaration let `d3anim_Head.next` overlap `d3_Frustum`; `d3_FindAnim` followed
    the float bits 0x3f07ae02 as a node and crashed in `_strcmpi` while loading the first character
    animation.
  - The two head macros are local to d3_engine.c / d3_int_io.c, not in d3_engine.h: putting them in
    the header shifted d3_MirDrawObject's register schedule by 8 bytes (the known header-flip of
    notes/vc6-matching.md). With the macros local, d3_MirDrawObject stays matched.
- Verification: `uv run build` (no debt regression, d3_MirDrawObject MATCHED), `uv run link`,
  `uv run ledger check` (0 errors), `uv run lint` (0 errors), and the 90 s tutorial run above.
  The remaining `ledger check` warnings are the pre-existing data-layout ones. `uv run play --start
  tutorial` was also broken independently: its start check read `cwd` before `prepare()` assigned it;
  it now checks `FULL_EXE`, and `uv run play --no-build --start tutorial --seconds 10` exits 0.
