# hmt.c (unk_4ac140.c until layout 2026-10-22)

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the findings of each session, one section per author.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "hmt_CreateBinaryFile" tag, historical.ini / historical.dfn; historical events and persons, the council elections
- Boundaries: separated (plausible): start: the table helpers 0x4ac140..0x4ac2f0 have no .data; their const table (.rdata 0x5ed1e8..0x5edb78) lies after unk_4a7140.c's last float 0x5ed1e4 and before hmt's first float 0x5edb78 = defined at the top of hmt.c (layout 2026-10-22, blocked-TU pass); also (older) put here because hmt's .data block starts at 0x600f60 right after unk_4a7140.c's ("Blutrache" 0x600f54); end proven (layout 2026-10-22, session 5): hmt's block ends with "_NACHRICHTEN_NEU_01" 0x601044 (0x4adbb0 / 0x4ade0b), the next block 0x601058 holds the callback table of hst.c
- Range then: 0x4ac140..0x4adf40
- Was: unk_4ac140.c (layout 2026-10-22)

## Open work
- **headers (2026-10-30, typing pass 2)**: head.c includes hmt.h, data_views.h, cm_types.h; keeps 1 cast / view macros with @original byte evidence (the address and the matched caller are in each comment): util_RandomU; views / casts without byte evidence of their own (weak definitions or drafts' spellings, notes/headers.md debt item 1): ai_GoodsT, ai_BldsT, ai_BTypesT, ai_CharsT, ai_TimeT, CM_AddFieldValuesShort.
- Near-misses: 0x4ac170 33 B (the original loads e->unk08 and tail-jumps util_Random(10)), 0x4ac1a0 75 B (defined
  before its callee 0x4ac1e0 in the original? try the order); hmt_Init 0x4aca90 14 B (the original keeps hmt_File.p /
  .k in ecx / edx, never eax, after the call; `ret` local and `!x` made no difference); hmt_Unk4ad510 6 B (register
  only: the table value in eax, ours edx); hmt_Load 0x4ada80 24 B (the success return is the out-of-line block;
  an if / else with an empty then made no difference).
- Drafts far off: hmt_CreateBinaryFile 0x4ac440 (817/1104: the inlined strcat of "historical.ini" and the record loops;
  the original indexes the records with a byte offset +0x2c), hmt_Election 0x4ad1c0 (332/496), hmt_Score 0x4ad3e0
  (196/320, operand / statement order of the weights), hmt_Unk4ad580 (228/544), hmt_Unk4ad7a0 (176/736 from +0x142),
  hmt_Create 0x4adbb0 (303/688).
- Type debt: local prototypes of callees in other units in hmt.h (ai_Unk4fad60, 4e6ec0, 4e6680, 4c4e00, 4c4650,
  4e7550, 4c4db0, 505d30, 503f30, 4c3ae0, 52fe00, 4c3b10, 4e6b40, 4e6b70, 500970, 51c920); AICHAR bytes 0x09 / 0x0d
  and other offsets through HMT_B / HMT_I / HMT_F / HMT_S (an anonymous-union view over pad009 in ai.h changed
  ai_Unk483a60's code); G11_4C3D00 (unsigned short field); HMT_PLAYERW reads the 0x2f4-byte player records through
  ai_Unk7e3f80 (the table at 0x7e3f80 = ai_Unk7e4274 - 0x2f4 is indexed by ai_Unk6f1bc5).
## layout 2026-10-22 (session 3)
- Table G11REC[51] at 0x5ed1e8 (.rdata, 0x30 bytes each: kind 0 / 1, an id, unk08), text ids 0x1e2b / 0x1e35 + n.

## layout 2026-10-22 (session 4)
- Unit: the .data block 0x600f60..0x601044 (hmt_File {-1, -1, 0, 0}, "p_%i" / "k_%i", "hmt_CreateBinaryFile",
  historical.ini, /gamedata/ai/historical.dfn, "_NACHRICHTEN_NEU_00/14/03/01") is hmt's; the table helpers
  0x4ac140..0x4ac2f0 have no .data and were put here (renamed to hmt.c by the tag).
- historical.ini: [main] kcnt / pcnt, sections k_%i (events) / p_%i (persons) with begin / end (years) / vname /
  nname; HMTREC 0x2c bytes (idx, begin, end, vname[16], nname[16]), events numbered first. historical.dfn: magic
  "SaFh" (0x68466153), np, nk, the persons, the events (written p first, allocated k first).
- State: hmt_Person / hmt_Event (0x6f0714 / 0x6f071c, the next record to appear), the previous ones 0x6f0710 /
  0x6f0718, hmt_Unk6f0720 the pending election (0..2); saved / loaded by 0x4adb20 / 0x4ada80.
- Objects 0x26 / 0x27 (0x4e6ec0) hold the current event / person; 0x4ad0a0 creates them (0x4c4650 0x15 / 0x16) when
  the year (ai_Time.unk0 + 1400) reaches begin, 0x4ace40 removes them at end; 0x4adbb0 sets the new character up.
- Elections (years 1..3 of every 4): tables at .rdata 0x5edb78 (object type), 0x5edb84 (group bits), 0x5edb90
  (excluded bits), 0x5edb9c (office); 0x4ad1c0 nominates the four best (0x4ad3e0 score), 0x4ad580 decides.
- VC6: `min(16000.0f, max(1600.0f, X))` (windows.h macros) matched 0x4ac200's thrice-evaluated X; vfs_fread /
  vfs_fwrite results compared as unsigned (`(unsigned)x > 0`) give the jbe / ja of hmt's file code; byte flags
  `> 0` give jbe too (0x4adff0..).

## match-base 2026-10-24 (base-4 worker, shard 4/5)
- The table records read out of .rdata: G11REC.kind 0 is a talent (its id is the skill slot +0x80 + id: ERNDWERKSKUNST 1,
  NACHTUNDNEBEL 2, KAMPF_1..4 3, RHEITORIK 4), kind 1 a profession (id 1..12: SCHMIED .. RAEUBER), kind 2 a historical
  event (BUERGERMEISTER, PEST, ...); 0x0c is a text id, 0x0e the name string, 0x08 (`textSet`, was unk08) selects the
  text block 0x1e2b / 0x1e35 (+n). ai_Unk5ed1e8 -> hmt_Records.
- Names: ai_Unk4ac140/150/1e0 -> hmt_RecordCount / hmt_RandomRecord / hmt_GetRecord; 0x4ad190/0x4ad1b0 ->
  hmt_GetElectionObjType / hmt_SetPendingElection; 0x4adf20 -> hmt_RandomHeadIcon (the value goes into +0x18c, the
  head icon number = index + 0x639); 0x4ad3b0 -> hmt_SendElectionMessage (sub type 0x14 through 0x4c4db0 =
  cm_SendExSysMessage, message 0x20 handled by cm_ExSysMessage). hmt_Unk6f0710 / 18 -> hmt_CurrentPerson / Event (in
  play), hmt_Person / hmt_Event are the next ones, hmt_Unk6f0720 -> hmt_PendingElection. Election tables ->
  hmt_ElectionObjTypes / GroupBits / ExcludedBits / OfficeBits. 0x4ac200 -> hmt_GetPrice: 500 * the character's worth /
  the richest's, clamped 1600..16000, 0 above its goods value.
- 0x50a640 and 0x509e60 each had two spellings (ai_Unk.. / gm_Unk..) and 0x4fad60 a third (unk_Unk4fad60): one
  function each, now gm_GetWorth / gm_GetGoodsValue / gm_FindRichest (208 references renamed, readable count +57).
- Byte + readable this session (12): hmt_RecordCount, hmt_RandomRecord, hmt_GetRecord, hmt_GetElectionObjType,
  hmt_SetPendingElection, hmt_Exit, hmt_Save, hmt_NextEvent, hmt_NextPerson, hmt_RandomHeadIcon,
  hmt_SendElectionMessage, hmt_GetPrice; with the 4 earlier ones the unit is 16/30 byte + readable.
- Escalated (deep): hmt_RandomTextOffset (code 39: the original loads e->textSet into ecx unused and tail-calls
  util_Random(10) with 0x1e2b live in esi), hmt_RecordTextId (code 53: n stays in its argument slot and is reloaded
  after the call, m in esi; ours keeps n in edi), hmt_Init (code 12: hmt_File.p / .k in ecx / edx in the original,
  eax / edx in ours), hmt_Load (code 8: the out-of-line success return, jne over the whole success block).
- Still in the base queue: ai_Unk4ac2f0 (Q2 needs the core ai calls and AICHAR.unk004); hmt_Update / hmt_Begin (byte,
  Q2: ai_Time.unk0, AICHAR.unk004, the 100+ site ai calls); the behaviour drafts hmt_CreateBinaryFile (542 code hunks),
  hmt_Election (175), hmt_Score (107 + slots), hmt_Unk4ad580 (52 + regs), hmt_Unk4ad7a0 (16 + fill + order),
  hmt_Create (43 + Q4 macros), hmt_Unk4ad510 (40).
- Header debt: cm.h declares hmt_SetPendingElection (prototype now matches hmt.h); he.c declares hmt_RecordCount /
  RandomRecord / RandomTextOffset / GetRecord with K&R prototypes (in the lint baseline). he.c must not include hmt.h:
  its own K&R ai_* declarations clash with ai.h through hmt.h -> am.h.

## match-base 2026-10-24 (base-4 worker, session 2, shard 4/5)

- hmt_ApplyRecord (0x4ac2f0, was ai_Unk4ac2f0) byte + readable: the core names below. The record apply stays as
  read: kind 0 raises the skill byte at +0x80 + id through cm_SendSetField (field 0x1c8, value 0x20000), kind 1
  sends cm_SendMoveGoods from every character whose job group is the profession id.
- Project-wide names (all references, 30+ files): cm_BeginFieldBlock 0x4c3ba0, cm_AddFieldValues 0x4c3d00,
  cm_SendMoveGoods 0x4c47e0 (message 0xf, handler 0x4c6f60), cm_SendFieldDeltas 0x4c49a0 (0x16),
  cm_SendSetField 0x4c4a60 (0x19), cm_SendRelations 0x4c4b00 (0x1b), gm_TitleGroup 0x504ad0 (its definition was
  already renamed, call sites were not), ai_OwnPlayer 0x6f1bc5 (the own player slot, "only for the own player"
  in f3gm's comments), cm_SendKillCharacter 0x4c4e00 (message 0x21, handler 0x4c8de0 -> gm_KillSpieler),
  amt_SendOfficeMessage 0x4e6680, amt_GetFirstOffice 0x4e6ec0.
- Character fields: netId (was unk004, every character view AICHAR/AIPER/AIPX/AGCH/AIC4/G5CH; gm_FindById finds
  the character by int +4, the cm messages carry it, 0x4c4450 resolves -2..-4 into it), jobId (was unk164; the
  job byte gm_SetJob writes, gm_TitleGroup maps). AITIME.year (was unk0; 0x578 + it is the calendar year).
- Reverted: ai_Unk505df0 -> gm_FindChar, ai_Unk5176d0 -> io_FormatText, ai_Unk53dda0 -> he_SendEventText. The
  defining units declare short/K&R prototypes (f3gm.c's gm_FindChar is short *, gv.h/amt.h have int f(),
  he.c's callers pass 5 args to the 6-arg he_SendEventText), so the merged name turns a VC6 warning into an error
  (C2371 / C2198). Header debt to fix before those call sites can be renamed: one canonical prototype per unit
  header and the 5-arg @original call sites of 0x53dda0.
- hmt_Begin (0x4ad0a0) byte + readable through cm_SendCreateCharacter (0x4c4650, message 0xb, handler 0x4c6bf0).
- Escalated to match-deep this session: hmt_ClearOffices (register allocation only), hmt_CreateBinaryFile (the
  SDK prototypes brought it from 542 code hunks to 227: the two atoi checks compile as test/jg + out-of-line
  `or eax,-1` returns in the original, our cmp/jle shares one return; record loops walk 0x2c byte offsets; the
  inlined strcat of "historical.ini"), hmt_Score (slots / x87 stack order only; the expression was read against
  the disassembly and is the same: rel doubled by `twice`, age = (+0xd + 1) * 0.1 doubled, w = 0x4e7c40(title) /
  0x4e7c40(0x1b), t = 0x4ea5c0(c, 0x4e6f10(title))).
- 0x4ac440 raw access: the `*(T *)` of hmt_Score is the obj word `*(unsigned short *)obj` (Q4 debt).
- Still in the base queue: hmt_Election (Q2 also blocked by 0x5176d0 / 0x53dda0), hmt_Unk4ad580, hmt_Unk4ad7a0,
  hmt_Create (behaviour drafts), hmt_Update (byte, Q2: the three reverted callee names above).
  hmt_ApplyRecord, hmt_Begin raised; hmt_ClearOffices, hmt_CreateBinaryFile, hmt_Score escalated.
  Next session: `uv run ledger grade next`.

## match-base 2026-10-24 (base-4 worker, session 3, shard 4/5)

- The unit's base queue is empty (5 items resolved, all escalated to match-deep).
- hmt_Update -> deep: byte + Q2 only. The three callees are named (gm_FindChar 0x505df0, io_FormatText
  0x5176d0, he_SendEventText 0x53dda0) but the rename is blocked by the declarations: 0x505df0 is short*
  (f3gm.c definition), AICHAR* (ai.h), char* (ahm.h / amt.h / cm.h) and K&R (gv.h) across 24 units, 0x5176d0
  is void variadic vs int K&R in 6, and 0x53dda0 has a 6-param definition with he.c calling it once with 5 args
  and cm.c twice with 3 (C2198 against the prototype; the @original evidence at those sites = the original's
  pushes). 0x505df0 alone has 591 calls with one direct ->id (am.c:8482).
- hmt_Election -> deep: 175 code hunks, all the same operation spelled differently: frame 8 B small
  (sub esp,0x138 vs 0x140), the scan's character in edi vs esi and the best[] base in ebp vs edi, the fcom
  test `test ah,0x41 / jnp` vs `and eax,0x4100 / jne`, the copy loop 4-esi vs 3-ecx and [eax-8]->[eax] vs
  [eax-4]->[eax+4]. Q2 needs the same rename block + ai_Unk4e7550.
- hmt_Unk4ad580: meaning fix (the original compares hmt_PendingElection unsigned: (unsigned int) < 3, jae);
  52 -> 50 code hunks. The rest: n > 3 vs cmp edi,0x4 / jge, fld/fcomp operand order of the best test, AIMSG
  store order. -> deep.
- hmt_Unk4ad7a0 -> deep: the council loop exits on lea eax,[edi-1] / test / jne in the original (edi = i+2, so
  i != -1) against [edi-2] / jge here, sub ecx,4 and dec edi swapped; the wait loop while (!ai_Unk4c3ae0(h))
  ai_Unk52fe00() is rotated (top test at 4ad977 in the original); fmul 0x5eb7ac before fimul hmt_Unk5ee114.
- hmt_Create: meaning fix (the character scan's i was unsigned short; cmp cx,0x300 / jb cannot come from the
  original's int counter, which strength-reduces to a pointer walk with cmp / jl); 43 -> 17 code hunks. The
  rest: lea edi,[ebp+0x5c] vs rep stos order, the ?: vs if/else shape of the hmt_Find argument. -> deep.

## match-base 2026-10-25 (base-4 worker)
- Reference-only renames (no code change; every use updated, build/link/ledger clean): ai_Unk4c3ae0 -> cm_GetSlotState (0x4c3ae0) and ai_Unk4767d0 / ai_Unk476860 -> d2_StartFade / d2_ReleaseFade (0x4767d0 / 0x476860, the old ai_ aliases folded into the d2 names). See notes/units/unk_5d33b0.md for the whole rename list.
- Reference-only renames (no code change): 0x4e6b40 / 0x4e6b70 ai_Unk4e6b40 / ai_Unk4e6b70 -> amt_SetEventOwner /
  amt_SetPersonOwner (amt.c; hmt_Create's type 0x15 event vs person calls, hmt.h declarations updated).

## match-base 2026-10-25 (base-0 worker, shard 0/6)
- hmt_Update (0x4ace40) is byte + readable now: the ai_Unk53dda0 -> he_ShowNews rename cleared its last Q2 (the callee was a placeholder); the stale @deep tag was removed (uv run ledger grade escalate hmt_Update --clear), the function is out of the deep queue.

