# f3gm.c (new, layout 2026-10-23)

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** first; below it the findings of each session, one section per author.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): character income and wages, the game clock, the building / object tables and queries, job / title tables, character helpers, prices and worth, standing / family
- Boundaries: open (layout 2026-10-23, 2nd session): name from the allocation tags "f3_gm:lpGebaeudePROT" / "lpGebaeudeINST" / "lpObjektePROT" / "lpObjekteINST" (0x61aa00 .. 0x61aa38, used by 0x4fe570). One monotonic .data string block from the first user 0x4f9e30 ("misc\statistics_taxes" 0x61774c) through the head icons, "Character_Names.dat" 0x617818, the tags 0x61a9e0 .. 0x61aa38, "Building name too long: %s" 0x61ab2c, "gm_KillSpieler: Killing Player" 0x61abd0, "%1G %A" 0x61ac98, up to "eval_ObjektPreis2Credits:" 0x61ad10 and "_NACHRICHTEN_HS_67" 0x61ade0 (used by 0x50cad0, the last f3gm string): the character income / statistics code (0x4f9e30 .. 0x4fe140), the game clock, the building and object tables (0x4fe370 .. 0x500190, queries 0x4ffe90 / 0x501190), the job / title tables (0x504100 ..), character helpers (0x505230 .. 0x506e70), then the value / price helpers (0x5085b0 .. 0x509ee0: player price table 0x61a8ac, object price 0x5087f0 with the cache in the prototype word +0x38, building income 0x508d80, goods lists 0x5098a0), the worth of a character (0x50a640), the stall lists (0x50aa00 .. 0x50ad70), the standing / family helpers (0x50b010 .. 0x50d0b0: ranks, family tree window 0x50c340, messages 0x50cad0). End: 0x50d0b0 (the next function 0x50d100 uses the strings of f3init.c, tag "f3_init:"). Open: not drafted 0x506890 (1504 B, a 0x1b18-byte frame), 0x507690 (3872 B), 0x50b3a0 (1616 B), 0x50b9f0 (2336 B) (drafted this session, layout 2026-10-23 3rd: 0x504510, 0x50a250, 0x50a6c0, 0x50a970, 0x50af00, 0x50c550, all ROUGH: float operands lost); many ROUGH drafts (see notes/units/f3gm.md); unnamed callbacks / missed starts none here
- Range then: 0x4f9e30..0x50d0b0 (all drafted; 0x506890, 0x507690, 0x50b3a0, 0x50b9f0 are ROUGH, layout 2026-10-23)
- Was: tag "f3_gm:"

## Open work
- **headers (2026-10-30, typing pass 2)**: head.c includes d2_types.h, txt_views.h, gv.h, di_input.h, data_views.h, am_types.h, d3_types.h, amt_types.h, ai_types.h, time_view.h, io_text.h, ls.h; keeps 2 cast / view macros with @original byte evidence (the address and the matched caller are in each comment): ai_Unk5a8560K, util_RandomU; views / casts without byte evidence of their own (weak definitions or drafts' spellings, notes/headers.md debt item 1): ai_PlayerCharC.
- 0x4fe570 gm_LoadTables (draft, unworked): the merge 2026-10-25 turned 149 differing bytes / 62 differing
  instructions into 148 / 65 - not a merge resolution, the body is identical in every branch. Cause isolated to
  the 2026-10-25 base-2 worker's typed `ls_Read` prototype in gv.h (was `int ai_Unk51c920();`, a lint X fix
  across 7 disagreeing declarations): patching only that line back to the K&R form on the pre-merge tree
  reproduces 149 / 62. A function nobody calls it. Kept the typed prototype (lint X = 0, other units gain) and
  recorded the shift here; recheck when 0x4fe570 is matched.
- Drafted in layout 2026-10-23 session 4 (ROUGH, compile, sizes 1472 / 2816 / 1424 / 1536 against 1504 / 3872 / 1616 / 2336): 0x506890 (answers an order: the objects of the character are told to the target, texts 0x1c4a .. 0x1c4e to the players), 0x507690 (creates a character: slot search, defaults, avatar for type 0x10, random skills from 0x5ee160 or a mix of the parents, names from 0x143ac5c / 0x143af70 / 0x143b178), 0x50b3a0 (the layout of the research tree: nodes 0x76fde0 of 0x34 bytes, ranks, x / y spread), 0x50b9f0 (the tree's widgets by node state 0 .. 3). Offsets of the character record used by 0x507690 are in its comment (CF(off, type) macro): +0xa age, +0x54 / +0x50 portrait ids, +0x5c .. 8 partner ids, +0x88 + 12 * i the 14 skills, +0x164 / +0x165 kind / title bytes, +0x16c / +0x170 home objects.
- ROUGH drafts (guessed float arguments, lost __ftol operands, restructured): 0x4fa700 (size 576 vs 592), 0x4faa00, 0x4fae50, 0x4fb0e0, 0x4fbe90, 0x4fccc0, 0x4fd120, 0x4fdfd0, 0x4fec10, 0x4ff190, 0x4ff550, 0x4ffa10, 0x500190, 0x500c20, 0x502a60, 0x502fd0, 0x506180, 0x5064d0, 0x506e70 (1536 vs 1872 B), 0x5075c0. Session 2 added ROUGH drafts: 0x508d80, 0x5087f0 (448 vs 464 B), 0x508a80 (the digit grouping), 0x5091a0 / 0x5092e0 / 0x509400 / 0x5096a0 (clock clamps: the working day 360 .. 1380 minutes = 6:00 .. 23:00, 1020 a day), 0x5098a0 / 0x5099e0, 0x509b10, 0x50ab80, 0x50ad70, 0x50b1e0, 0x50c340, 0x50cdc0, 0x501580 (the instance default bytes written as ints), 0x5020e0, 0x502570, 0x502b60, 0x503530 (the permission bits: a case per building type, bit meanings open), 0x5031b0, 0x503c60, 0x504ec0 (the skill value: jump table over the skills 0 .. 4, bit fields of the word +0x2c).
- Near misses (one local cause): gm_CompareTime 0x4fe210 (the original compares `cmp eax,esi / jge` for `a->day < b->day`, ours `cmp esi,eax / jle`; tried `b > a`, int pointers, locals), gm_FindFreeBuilding 0x5012b0 (the original repeats `cmp eax,ecx / jb / xor eax,eax / ret` after the loop), gm_Unk4ff8c0 0x4ff8c0 (no local pointer in the original: it recomputes p * 0x44.. per use, and loads the arguments into esi / edx), gm_FindObjectProto / gm_FindBuildingKind (1 byte: `lea ecx,[esi+eax+1]` against our `[eax+esi+1]`), gm_ListBuildingsOfType (1 byte, `[ecx+ebx]` against `[ebx+ecx]`), gm_FindMarketObject 0x500ec0 (registers esi / edi swapped for the index and the object), gm_Unk506480 (`jbe` where we emit `je` for `cnt++, 0xb < age`), gm_Unk4f9f60 / 4fa060 (the original keeps a redundant `cmp esi,6 / jl` + its own return block after the slot loop; the draft shares the return), gm_Unk504290 / gm_TitleNumber (the original case blocks are emitted in ascending value order, the jump table covers one more entry (0x10)), gm_Unk503f50 (jump table 1 .. 0x1a, ours 1 .. 0x18: add a case 0x19 / 0x1a that returns 2?).
- Names: gm_ for the game data part (clock, tables, queries); the income / head-icon part before 0x4fe140 is still called unk_Unk4f9e30 .. unk_Unk4fdfd0 (no real names yet); the income kinds: 0x4f9f60 / 0x4fa060 (kinds 1 / 2, wages by the owner's building count at +0x19c), 0x4fa160 (3), 0x4fa2a0 (4), 0x4fa3d0 (5), collected by 0x4fa4b0 and summed by 0x4fabe0 into the history tables 0x91b580 .. 0x91b690.
- Type debt: the unit includes gv.h only; everything else is declared K&R in the file (ai_Unk4ffe90 is the object query gm_Query, ai_Unk501190 the building query gm_BuildQuery: both variadic (count, (kind, value) ..)). ai_Unk50a640 is a float in gv.h but the unit reads it as an int through I50A640. The unit still declares 0x4fca40 with the unk_ name (float result).

## layout 2026-10-23
- The unit was found by the .data string order (one monotonic block 0x61774c .. 0x61ac98) and the tag strings "f3_gm:lp..." of 0x4fe570; unk_4f9e30.c (0x4f9e30 .. 0x4fe140) and f3gm.c (0x4fe140 ..) were first written as two files and joined (check 2083 before and after, link ok).
- Game clock: 14 bytes (`#pragma pack(1)`: int day, ushort hour, int minute, int second; the fields are unaligned: +0 / +4 / +6 / +0xa); 0x4fe140 adds hours / minutes / seconds, 0x4fe210 compares, 0x4fe280 gives the minutes between, 0x4fe2c0 the calendar record (year = day + 0x578, season = day % 4, month = season * 3 + 1).
- Tables: building prototypes 0x7e3f30 (0x58 records of 0x24d bytes, +1 the name, +0x22 count / +0x23 the object words, type byte +0), building instances 0x889f74 (0x100 of 0xa9, +1 id, +0x5d the object list head, +0x61 the scene object, +0x65 a counter or block, +0x27 the owner character), object prototypes 0x91aaf0 (0x2dc of 0x41, +1 the name, +0 kind), object instances 0x7e3f34 (0x5008 of 0x43 bytes: +0 prototype word (0 = free), +2 id, +6 owner id, +0xa the owner's number, +0xe the amount, +0x14 the child list head, +0x3f the next pointer), counts 0x6f1bdc. Queries: 0x4ffe90 (objects: `(list, n, (kind, value) ..)` kinds 0 prototype, 1 owner id, 3 0x50a0b0 value, 4 prototype kind, 5 stop at first, 6 descend into children, 7 scan everything) with 0x4ffcc0 as the iterator (state in 0x6f1bf4 ..); 0x501190 (buildings: kinds 0 type class, 1 id, 2 string, 3 / 4 words +0x25 / +0x27, 5 building type, 6 stop at first) with 0x501030.
- Characters: 0x300 records of 0x21c bytes at 0x770b80: +0 id word (-1 free), +2 kind (0 .. 9 persons, 0xf, 0x11, 0x12, 0x14 ..), +4 id int, +8 active, +9 sex / group, +0xa age word, +0x1c health float, +0x20 lifetime float (age < lifetime: younger), +0x2c skill word (8 bit fields), +0x5c partner, +0x68.. relatives, +0x164 job, +0x165 title, +0x16c master, +0x170 building, +0x178 object list, +0x194 / +0x19c / +0x1a0 / +0x1a4 / +0x1a8 the income bases, +0x1cc flags.
- Matched in this unit: 0x4fa9b0, 0x4fb330, 0x4fd7e0, 0x4fda10, 0x4fde90, 0x4fdfb0 and 0x4fe280 .. 0x4fe350 (clock), 0x4fe7f0, 0x4fe980, 0x4fe9c0, 0x4feb30, 0x4ff950, 0x4ffbf0 .. 0x4ffc90, 0x500080, 0x500140, 0x501000, 0x502020, 0x5024f0, 0x502e30, 0x503480, 0x503f30, 0x504a80, 0x504e00, 0x5048e0, 0x5049c0, 0x505d30, 0x505e30.


## layout 2026-10-23 (session 2)
- Drafted 0x5085b0 .. 0x50d0b0 (about 130 functions, in address order) and the earlier gaps 0x501580, 0x502570, 0x502b60, 0x503530, 0x5020e0, 0x5031b0, 0x503c60, 0x504ec0. Matched at once: 0x5085b0 / 0x5085f0 / 0x508620 / 0x508640 / 0x508660 (the player price table 0x61a8ac, indexed by the word +0x54 of the player slot: PLW54), 0x5089c0, 0x508a20 (its `if (p == 0) return 1.0f` is a pointer test the compiler does not drop when the pointer is computed as `(unsigned char *)ai_Unk770b80 + c * 0x21c`), 0x509060, 0x5093a0, 0x509f20 .. and about 40 more (the list is in names.csv).
- End of the unit: 0x50d0b0. 0x50d100 and on belong to f3init.c (tag "f3_init:", its own .data from 0x61adf8; see notes/tus.md). Value helpers: gm_Unk5087f0 is the price of an object for n pieces (cached in the prototype word +0x38 as value / 32); gm_Unk50a640 the worth of a character (its objects + buildings); gm_Unk50ab50 / 50aaf0 the stock of a stall list.
- Lessons: a callee prototype with a `char` parameter makes VC6 push `mov al, [x]; push eax` without extension (0x512de0 matched only with `int f(char *o, int fl, char c, int d)` declared); a callback returning `char` needs a `char` return type (0x5115b0: `mov al,dl`); `strcpy(dst, expr)` with a pointer expression gives the byte loop with `sub edx,eax` only for some operand shapes (0x50dba0 not found); `x >= 5` and `x > 4` compile differently for a field read (0x511600: write `>= 5`). VC6 inlines `strstr`-like calls never: `FUN_005de6b0` is strstr (non-zero = found), `0x5dfb40` strncmp, `0x5eaac0` _strnicmp, `0x5defb0` strchr.
- Near misses (session 2): 0x50c980 (1 byte: the registers of the counter and the index swapped; tried the declaration order), 0x50ab50 ((a + 2) * 5 is emitted as `add eax,2 / lea` in the original, `lea eax,[eax+eax*4+0xa]` here: `a += 2` made no difference), 0x50d6a0 (ecx / edx swapped), 0x509ee0 (the char parameter: `movsx ecx,BYTE PTR [esp+8]` in the original), 0x50dba0 (strcpy loop shape), 0x50d460 (the original returns 0xffffff00: a return value of the fill loops).
- Ghidra text of 0x504510 for the next session: `iVar4 = gm_TitlesLeft(c[0x164 of the character]); if (iVar4 == 0 || iVar4 > 5) return 0.0; kind = gm_TitleGroup(job) -> 2 (0xb), 3 (0xc / 10), 0 (8), 4 (9), else 1; gm_Unk504ec0(char, kind); then a game clock record a is built (a.day = ftol .., hours = ftol(21..) wrapped at 21 / 7 +6) and compared (0x4fe210) with the character record's clock at +0x174 / +0x28; the result is the float of 0x4fe140 (a + hours, 0, 0)` -- the ftol operands are lost, read the disassembly.
## layout 2026-10-23 (session 3)
- New drafts 0x504510, 0x50a250, 0x50a6c0, 0x50a970, 0x50af00, 0x50c550 (all ROUGH, the float operands of the __ftol calls are lost; sizes 496 / 704 / 320 / 16 / 256 / 1088 B). 0x50c550 reads the person record at +0x166 (job), +0x169 (guild byte 0x1e .. 0x21 -> the guild person 0x17 .. 0x1a), +0x16c (the spouse) and the flags word +0x2c (bits 14..16 faith, 23..24).

## layout 2026-10-23 (session 4)
- 0x4f9764 (ord.c) and 0x528180 (sv.c) are examples of function starts / tails Ghidra gets wrong; the four drafts above were written from REA pseudocode plus the disassembly (varargs calls of 0x5176d0 lose their arguments in the pseudocode: read the pushes).

## match-base 2026-10-24 (base-4 worker, shard 4/5)
- The three worth helpers were one function under two (0x509e60, 0x50a640) or three (0x4fad60) spellings; unified to
  the definition names gm_FindRichest (0x4fad60, the id and worth of the richest active character), gm_GetGoodsValue
  (0x509e60, the value of the character's objects) and gm_GetWorth (0x50a640, goods plus buildings), 208 references.
  hmt.h's gm_FindRichest prototype now matches the definition (`int`, not `void`); the ai.h (AICHAR *) vs cm.h
  (char *) split of gm_GetGoodsValue stays in the lint baseline.

## match-base 2026-10-24 (base-1 worker, shard 1/5)

- Readability renames that cleared Q2 here: ai_Unk770b80 -> ai_Chars (ai.h AICHAR), ai_Unk7e3f30
  -> ai_BTypes (ai.h), ai_Unk91aaf0 -> ai_Goods (ai.h) - the units that do not include ai.h keep
  their char * views (lint_baseline type debt). 0x503480 -> gm_RemoveObjectFromBuilding,
  0x50c520 -> gm_Opinion (ai_Relations[a][b], 0x7f for oneself), 0x508a20 -> gm_HealthFactor
  (1 + hp / 1008 above 42, AICHAR +0x80). d3t_Unk6310e8 / ai_Unk6310e8 -> d3t_CamMove.

## match-base 2026-10-24 (base-1 worker, shard 1/6)

- ai_Unk7d5f80 -> gm_PlayerRecords (the 16 player-like character records, 0xac bytes each, indexed by the
  slot in gm_PlayerRecord 0x506430); gm_ClearBuildings (0x4fe7f0) got its prototype in f3gm.h for ls.c.
  ls.c declares gm_PlayerRecordBytes at 0x7d5f82 for the +2 byte view the save uses.

## match-base 2026-10-25 (base-0 worker, shard 0/6)

- 0x4f2720 calls renamed ai_Unk4f2720 -> gv_GetTableRecord; byte unchanged.
## match-base 2026-10-25 (base-1 worker, shard 1/6)

- 0x4fe1e0 is gm_SetTime at every call site now (ai.c, he.c, main.c, unk_4b39f0.c, f3gm.c): the
  ai_Unk4fe1e0 name is gone, ai.h keeps the AITIME view of the prototype, f3gm.c's redundant K&R
  declaration was dropped (its definition above is the prototype).

## match-base 2026-10-25 (base-4 worker)
- Reference-only renames (no code change; every use updated, build/link/ledger clean): ai_Unk4c3ae0 -> cm_GetSlotState (0x4c3ae0) and ai_Unk4767d0 / ai_Unk476860 -> d2_StartFade / d2_ReleaseFade (0x4767d0 / 0x476860, the old ai_ aliases folded into the d2 names). See notes/units/unk_5d33b0.md for the whole rename list.

- match-base 2026-10-25 (base-1 worker): the stale ai_ aliases of this unit's functions were renamed to
  the canonical names project-wide (gm_BuildQuery, gm_BuildNext, gm_FindBuildingById, gm_Opinion,
  gm_GetGebaeudeHauptraum (was gm_Unk501ce0), gm_BuildingRank, gm_OwnerWorth, gm_SumSlotAmounts,
  gm_FindById, gm_IsHomeType). gm_GetGebaeudeHauptraum's name comes from both AI callers' error text
  ("gm_GetGebaeudeHauptraum() failed !!!"); its 0x501ce0 draft here is void while the callers use the result
  as a pointer - check the return type when this unit is matched.

## match-base 2026-10-25 (base-5 worker, shard 5/6)

- Reference-only rename: ai_Unk5085f0 -> gm_PlayerFactor (0x5085f0, a * the factor of the player slot k);
  f3init.c now calls it through the canonical (int, unsigned char) prototype. Every use updated.

## match-base session 2026-10-25 (base-1, shard 1/6)

Renames only, from the ai.c side (all byte-neutral, the f3gm.c definitions are canonical): gm_Unk50aa00 ->
gm_StallGoodsList, gm_Unk50ab20 -> gm_ListAmountById, gm_Unk5086b0 -> gm_ObjectValue, and the stale ai_Unk
aliases ai_Unk4fe980 -> gm_FindObjectInst, ai_Unk4ffc40 -> gm_FindGoods, ai_Unk500c20 -> gm_CollectOffers,
ai_Unk7e4b20 -> gm_Offers, ai_Unk7e3f3c -> gm_NOffers are gone from ai.h / ai.c (the same addresses and types).

## match-base session 2026-10-26 (base-1 worker, shard 1/10)

- gm_StatisticsScreen (0x4f9e30, was unk_Unk4f9e30): byte + readable now (check 2573 -> 2574, 144 B).
  Meaning fixes that MATCHED: the printed value is *(int *)ai_Time - 0x10 + i (the game year's value), not
  the array address (the GiD draft's address arithmetic); and the format string is the original
  "Runde %i:   %T$N" - the draft's "Runde %i  T  N  0" also kept a .rdata conflict at 0x61774c.
  Q7: gv.h now declares d2_OpenForm(int, int, char *) (d2.h's prototype) and d2_SetPointerSprite (gfx.h)
  instead of the K&R `int d2_OpenForm();` - lint's K&R call count 5387 -> 5382, no byte changed.
  Q4: gv.h gets `extern int ai_TimeYear` at 0x770ae0 (the am.h / cm.h ai_TimeHour pattern).
  Renames: unk_Unk4f9e30 -> gm_StatisticsScreen, unk_Tab91b580 -> gm_IncomeHistory (the 17-round history of
  the five income sums; f3gm.c 0x4fabe0 shifts it, ord.c clears it) in gv.h / f3gm.c / ord.c.

  Second function of the session: gm_IncomeRating (0x4f9ec0, was unk_Unk4f9ec0) - readable, escalated.
  Matching leads kept: the loop bound is i <= 0xf (cmp 0xf / jle; i < 0x10 gave cmp 0x10 / jl) and the
  term is a named float local x with `sum = sum + x` (17 -> 13 differing instructions). What is left is
  the FPU-stack form of the accumulation: the original computes the term, `fstp st(1)` rotates it,
  reloads the sum (`fld [sum]`), adds (`fadd st,st(1)`), stores (`fstp [sum]`), pops the term at the
  loop tail (`fstp st(0)` at 4f9f10, entered by the `jmp` at 4f9f0e) and returns sum + the last term
  (`fadd [sum]` at 4f9f50). Ours does `fadd [sum]` / `fstp [sum]` / `fstp st(0)` and returns the sum
  alone. Byte-identical spellings (do not repeat): (float) cast on the sum, sum += ..., swapped operand
  order, dropping the (float) cast.

## match-base 2026-10-26 (base worker)

- Rename (evidence: 0x4fca40's own comment "changes the health of the character id by d ... returns the new
  health, 0 when the character died"): ai_Unk4fca40 / unk_Unk4fca40 -> gm_AddHealth (the f3gm.c definition and
  every use in src/ and include/, 40 references in 10 files; build 2596/4664 unchanged, lint 0 errors, no grade
  fell). It unblocks the Q2 of the unk_4b6480.c message callbacks that only used it as a callee.

- Rename (evidence: 0x50c550's own comment "how much the person a likes the person b, 0 .. 100"):
  ai_Unk50c550 / unk_Unk50c550 -> gm_Liking (the f3gm.c definition and every use in src/ and include/, 80
  references; build 2596/4664 unchanged, lint 0 errors, no grade fell).

## match-base 2026-10-26 (base session 2)

- Two callee renames, every use updated: unk_Unk504ec0 + ai_Unk504ec0 -> gm_GetSkill (0x504ec0, "the skill value
  0 .. 1 of the character c for the skill number 0 .. 4"; 76 references in 11 files) and ai_Unk4c5710 ->
  cm_SendGoodsDelta (0x4c5710 in cm.c, message 0x40 of a goods record's differences; 8 references). Both
  byte-neutral (check 2603/4664 unchanged, 0 grade regressions).
- amt_HasTownObject (0x4e9400) is declared K&R here (`int f();`): the typed `int amt_HasTownObject(int k);`
  made gm_Unk4fa4b0 one byte worse (48168 -> 48169 differing bytes), so the K&R declaration stays as type
  debt.

## match-base 2026-10-26 (base-0 worker, shard 0/6, session 8)

- gm_Unk50a810 -> gm_FindCharacterByMaster (0x50a810, the first kind 6 / 7 / 1 / 2 character whose job master
  (+0x16c) is m) and gm_Unk50b090 -> gm_ResearchClass (0x50b090, the research class number of a building kind,
  out parameter). gm_Unk50b090 is NOT gm_BuildingClass (0x501d80, the class of a building kind) - the blanket
  rename collided and had to be undone. No byte change; 0 grade regressions.
