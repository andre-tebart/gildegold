# amt.c (new, layout 2026-10-22)

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** first; below it the findings of each session, one section per author.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): the offices: the table of offices (owner, kind, level), applications, votes, mayors, saves
- Boundaries: separated (plausible): the offices (layout 2026-10-22 session 15): every function from 0x4e5cd0 to 0x4eaa00 has a draft (0x4ea990 was a start Ghidra missed); data 0x60ec90 (rates, kinds) .. 0x60f1a8 (strings "amt_calc_zuenfte()" 0x60f11c, "amt_fio_SaveAemter" 0x60f150, "amt_fio_LoadAemter" 0x60f174, "%s$A" 0x60f1a8) without overlap. End: 0x4eaa50 is the first function of the building kind 0x1e and the next block (0x60f1b0 table, "ahm_" strings) belongs to ahm.c; the seam 0x4eaa00 / 0x4eaa50 rests on subject + .data order only
- Range then: 0x4e5cd0..0x4eaa50
- Was: (new)

## Open work
- All functions 0x4e5cd0 .. 0x4eaa00 have drafts. Rough (structure only): 0x4e5e50, 0x4e7150 / 0x4e7320 (insertion position read from the decompilation), 0x4e7c60 (votes), 0x4e8160,
  0x4e83b0 (always returns 0 in the decompilation), 0x4e88a0, 0x4e8c20, 0x4e8dc0 (build branch, message fields by offset), 0x4e9660, 0x4e9a00, 0x4e9ce0 (message handlers; amt_Tell helper is ours).
- Near misses: 0x4e6030 (89 / 128 B, see below), 0x4e6680 (original builds the message in place; `a` compare is a movsx vs movzx int compare, ours narrows to a byte),
  0x4e75c0 (original keeps the level bytes in esi / edi), 0x4e7aa0 (the original reads the first range as ints at +0 / +2 of the record; stack-sharing of the nested calls),
  0x4e86e0 (loop shape: the counter decrement at the loop end), 0x4ea990 (8 B: frame is 4 bytes bigger than ours), 0x4ea690 / 0x4ea7a0 (file io).
- Data not defined in C: the float tables 0x60ec90 (rates), 0x5edc90 / 0x5edef0 / 0x5edfbc (pay factors, .rdata), the kind records 0x60ed58 (40 x 0x18: +0 kind, +4 column, +8 level, +0xc pay, +0x10 two persons); amt_Flip (0x60f118) is defined.
- Type debt: amt.h repeats the callee prototypes K&R (ai_Unk4c58a0 / 4c4540 / 4c3ae0 / 4c4b00 ..), AMTAPP / AMTMSG / AMTSET / AMTTRADE are pack(1) message structs.

## layout 2026-10-22
- The office table is at 0x925120 (42 records of 0x18: +0 index, +4 owner id or -1, +8 kind (records 0 / 1: 1, then 2, 3, 4, 4, 5, 6, 7, 7, and i - 2 for the records 10 .. 41), +0x14 -1); 0x4e5cd0 clears and fills it.
  0x4e6030 finds the office of a character (first 30 records, by the id at +4) and copies it and its kind record. 0x4e60b0 returns the rate of an office for a character
  (the level of the target office within one above the next rank and above the current one).
- AI_ZERO_AT(offset table, n) (amt.h / cm.h, copy of d3.h D3_ZERO_AT) gives `mov edi, imm` for a global array; AI_ZERO(table, n) uses a pointer temp and does not match.

## layout 2026-10-22 session 15
- Offices: state byte +0x10 (1, 2 = two candidates (owner + x14), 3 = open for applications, 4 = free), +0xc applicant count (< 4). Kinds 1 .. 0x27; 0x1c / 0x1d deputies, 0x1e .. 0x21 mayors of the four towns, 0x22, 0x26 / 0x27 special.
- Character bytes: +2 kind (6 / 7 = player characters), +8 flag, +0x164 rank, +0x166 office kind, +0x167 previous, +0x168 applied-for, +0x169 second rank, +0x1b1 flag, +0x1cc status bits (0x10000000, 0x8000000, 0x4000000 = used the office power).
- Matched: 0x4e65e0 (returns the value of 0x4e6680), 0x4e6600, 0x4e6650, 0x4e6b40 / 6b70 (`do { .. i--; } while (i != 0)`), 0x4e6ba0 (compare the level before the column), 0x4e6f10 (`if (k >= 40) return 0;`), 0x4e6f80,
  0x4e70f0, 0x4e7550, 0x4e78a0 (`if (k >= 0x1b) k = 0x1b; else if (k <= 0) return k;` + `do { .. } while (k > 0)`), 0x4e78f0, 0x4e7b60, 0x4e7c40, 0x4e8530 (`(unsigned char)a[8] > 0`: the char +8 is unsigned),
  0x4e94a0 (`>= 0x1e && <= 0x21`, `>= 3`), 0x4ea8e0 (`>= 2`), 0x4eaa00.
- Missing starts Ghidra had: 0x4ea990 (pay of all offices). Strings: "amt_calc_zuenfte(): could not find a free place..." 0x60f11c.

## match-base 2026-10-24 (base-4 worker, shard 4/6)

- Reference-only: ai_Unk58fd90 -> ai_ObjectAngle in 0x4e8dc0 (the `ai_Unk458a20(j, j + 0x4c, ...)` site);
  amt.h's K&R `int ai_ObjectAngle();` became the matching `void ai_ObjectAngle(void *out, int plot);`
  (the conflict with am.h is fixed, not baselined; the call passes int* / int, no float promotion, bytes
  unchanged).

## match-base 2026-10-25 (base-0 worker, shard 0/6)

- amt_Unk4e5cd0 -> amt_InitTable (byte + readable): it clears and numbers the 42 office records and sets
  their default kinds; 0x4ea7a0 (the table loader) calls it first.
- amt_Unk4e5e50 -> amt_Rating (draft behaviour; readability now fails only Q5 ROUGH), in progress. Q2:
  amt_Unk4e6f10 -> amt_KindColumn (returns amt_Kinds[k].col, 0 for k > 39), amt_Unk4e6f80 -> amt_GetKind
  (copies amt_Kinds[k] to *out), and project-wide ai_Unk4f2720 / unk_Unk4f2720 -> gv_GetTableRecord (0x4f2720
  copies the 9-int record i of the 0x611258 table to out; definition in gv.c, 91 references over 17 files;
  am.h's @original ai_Unk4e6f80 view updated). Q3: the table end 0x5edee8 became
  `AMT_TABA_END ((int)&amt_TabA[15 * 10])` (same signed compare and code, 4 fewer differing bytes). Q4: the
  status dword, the TabB float bits and the kind record's +0x14 float go through the AMT_STATUS / AMT_BITS /
  AMTKINDF views. Q7: amt.h's `int gm_TitleGroup();` became `int gm_TitleGroup(int k);` (am.h's prototype).
  Behaviour: 171 of 187 instructions differ (register allocation all over); the spell menu's dup-tail (explicit
  returns in the arm, merged tail) and reload ([esp+4], parameter a) leads were tried one edit one build each
  with no change; Q5 ROUGH stays (the earlier reading's "not every branch"). Next: the branch reading and the
  allocation.

- amt_Unk4e6030 -> amt_FindOffice (draft behaviour, readable) and escalated to match-deep: it finds the office
  owned by character c and copies the office record and its kind record. Q4: the character id 0xffff marker and
  the netId at +4 are AMTCHARVIEW fields. Behaviour: the full original (0x4e6030..0x4e60a8) was read, the
  statement sequence matches (guard, 30-record loop on amt_Table[i].owner, copy office then amt_Kinds[kind],
  return 1/0); 55 differing of 50/49 instructions from instruction 0 (register allocation). `uv run spell` dup-tail:
  duplicate return 0, merged return, early-return guard/polarity swap tried one build each, same 89/128 bytes.
- amt_Unk4e60b0 -> amt_GetOfficeRate (byte + readable): the rate of the office kind x for character c under the
  level rule; Q4 id field.
- amt_Unk4e6180 -> amt_ApplyForOffice (in progress, draft): applies character c for the office kind; Q4 netId.
  Still Q2/Q7: the callees ai_Unk4c4540 (bracket text), ai_Unk4c4580, ai_Unk4c58a0 (send) live in cm.c
  (0x4c4540 / 0x4c4580 / 0x4c58a0, byte but draft, 68 / 66 / 4 references) - naming them is a project-wide
  merge like 0x447190; the ai_Unk4c58a0() K&R prototype in amt.h waits for the same.
- Alias debt kept: ai_Unk4e6030 (am.c/cm.c/gv.c/he.c/unk_58bbd0.c, G5CH*/void* views) and ai_Unk4e60b0
  (unk_4a7140.c/h, AIPER*/unsigned char view) are the older spellings of the amt_FindOffice / amt_GetOfficeRate
  calls; merging them needs call-site casts.

## match-base 2026-10-25 (base-0 worker, shard 0/6, session 2)

- The office searches are one idiom, found on amt_CheckApplication (0x4e6360): `do { if (p->idx == x) break; p++; i++;
  if ((int)p >= (int)&amt_Table[N]) return v; } while (1);` with N = 30 for the office lookups and 42 for the message
  senders. The while form of the same loop is rotated by VC6 (a guard whose first test is the absolute-address dword
  load + `and 0xff`, the test at the bottom); the do-while(1) keeps the original's test at the top. The bound is
  `(int)p >= (int)&amt_Table[N]` (signed jl / jge, Q3-clean; the `> 0x9253ef` / `< 0x9253f0` literals are gone).
- `i >= 30` / `i >= 42` / `cnt >= 4` instead of `> 29` / `> 41` / `> 3` (the latter cost the 0x1d / 0x29 / 0x3
  compare).
- amt_CheckApplication (0x4e6360) byte + readable; amt_AcceptApplication (0x4e6430) -> match-deep (the prologue's
  push edi, the kind local's slot, the -1 / 4 compare registers; reason in its @deep tag).
- amt_SendOfficeFree (0x4e65e0, kind 3 free), amt_SendOfficeCandidates (0x4e6600, kind 2 with the rival), 
  amt_SendOfficeOwner (0x4e6650, kind 1): byte + readable, named from the message kinds.
- amt_SendOfficeMessage (0x4e6680) byte + readable: the office parameter is `unsigned char` (the original compares
  `p->idx` with a byte compare; `char` gives movsx), the two netIds are locals computed before the search and stored
  into the AMTMSG after it, and AMTMSG gained its 4 trailing pad bytes - the original's local frame is 0x10 for the
  11-byte record because the 15-byte G17B15 copy reads the padding. All 7 declarations of the function
  (amt.h, cm.h, hmt.h, gv.c, he.c, hst.c, unk_58bbd0.c) carry the same prototype.
- amt_CheckOfficeMessage (0x4e6710) byte + readable: AMTMSG fields instead of the raw reads (Q4), AMTMSG.office is
  `unsigned char` (byte compare with the record's idx), the search as above.
- amt_ApplyForOffice (0x4e6180) -> match-deep: the meaning was read against the whole original and is complete; the
  remaining 61 differing instructions are allocation / block order plus the deferred `add esp,0xc` and the 0xff
  store form. Its callees are named now (Q2 was the only readability failure), the kind walk is the byte-pointer
  do-while(1) with the named bound, `i >= 30` and `cnt >= 4`.
- Renames (project-wide, all references): ai_Unk4c4540 -> cm_BeginGroup, ai_Unk4c4580 -> cm_EndGroup (messages 5 / 6,
  the group brackets), ai_Unk4c58a0 -> cm_SendApplication (message 0x44, the 7-byte application; int, the caller uses
  the send slot), ai_Unk4c58e0 -> cm_SendOfficeRecord (message 0x45, the 15-byte office record; int), ai_Unk505df0 ->
  gm_FindChar (the canonical f3gm.h name, 602 references in 24 files, the am.h view macros included).

## match-base 2026-10-25 (base-4 worker)
- Reference-only renames (no code change; every use updated, build/link/ledger clean): ai_Unk4c3ae0 -> cm_GetSlotState (0x4c3ae0) and ai_Unk4767d0 / ai_Unk476860 -> d2_StartFade / d2_ReleaseFade (0x4767d0 / 0x476860, the old ai_ aliases folded into the d2 names). See notes/units/unk_5d33b0.md for the whole rename list.
## match-base 2026-10-25 (base-0 worker, shard 0/6, session 3)

- Shard 0/6, unit amt.c. Check count 2548 -> 2549 (amt_ExecuteSwapMessage matches); grade regressions 0.
  Raised byte + readable: amt_SetEventOwner (0x4e6b40), amt_SetPersonOwner (0x4e6b70) - the office kinds 0x26 / 0x27
  set to a netId by hmt_Create (type 0x15 = historical event / person); amt_SendOfficeSwap (0x4e6ba0, the office-swap
  request of the counter-espionage wrapper 0x5cb1e0); amt_ExecuteSwapMessage (0x4e6c50, zero differing instructions
  after the office-search do-while(1) idiom + the two record pointers p1 / p2 - see notes/vc6-matching.md).
- Escalated to match-deep with reasons in their @deep tags: amt_ExecuteOfficeMessage (0x4e67a0, message 0x45; 135 ->
  136 code hunks, the out-of-line block order and the edi/esi roles; Q2 ai_Unk5a8cb0 has no evidenced name),
  amt_ClearOffices (0x4e6d80) and amt_ClearOfficesIfAll (0x4e6e20) - the loop's induction base (&x14 vs &owner) and
  the hoisted -1 register; amt_GetFirstOffice (0x4e6ec0) and amt_GetOffice (0x4e6f30) - one branch's block placement
  (the original's jl + inline return 0); amt_GetColumnOffices (0x4e6fc0) - only the callee-saved push placement
  (shrink-wrapping) and dec/js vs test/jl left.
- In progress at TIME UP: amt_GetColumnOfficesOwned (0x4e7040) is readable (draft behaviour, 118 -> 103 code hunks with
  the same col spellings / for walk / goto none as its sibling); it is NOT escalated and stays in the base queue.
- Typed views: AMTCHARVIEW gained officeKind (0x166) / prevOffice (0x167) / appliedOffice (0x168) / secondOffice
  (0x169) / money (0x194) and the 0x21c size (so `((AMTCHARVIEW *)ai_Chars)[i]` indexes records); AMTSET.idx is
  unsigned char (the original's byte compare). amt.h's ai_Unk5a8cb0() / ai_Unk5a9330() K&R debts became prototypes.
- Renames: ai_Unk5a9330 -> ai_SendSwapText (0x5a9330, unk_58bbd0.c; its only caller is amt_ExecuteSwapMessage);
  ai_Unk4c5fe0 -> cm_SendTradeMessage (0x4c5fe0, message 0x5c with the AMTTRADE record); ai_Unk770ae4 -> ai_TimeHour
  project-wide (0x770ae4 = ai_Time.hour, the clock hour; am.h / amt.h / cm.h / f3gm.c declares and the uses in am.c /
  amt.c / cm.c).
- Header debt left: amt_SendOfficeSwap keeps the K&R `int amt_SendOfficeSwap();` in cm.h (cm.c does not include amt.h);
  cm.h lists amt_SendOfficeMessage's prototype twice (0x4e6680) - pre-existing, not touched.

## match-base 2026-10-25 (base-0 worker, shard 0/6, session 4)

- Shard 0/6, unit amt.c. Check count 2560/4664 unchanged (no new byte match this session: the byte items were
  already byte); grade regressions 0; unit differing bytes 11630 -> 11511.
- amt_GetColumnOfficesOwned (0x4e7040) -> match-deep: the in-loop exit is `if (n >= max) break;` (the original's
  jge targets the shared final epilogue), not `return n` - the break spelling took 103 -> 61 code hunks and made
  ebx/edi shrink-wrap after the col guards like the original. Tried one build each: return 0 in both guard arms
  (identical code; the goto none stays), a separate `i--` statement, the comma init `for (n = 0, i--; ...)` (same
  code), and the out++/n++ order (kept, -1 byte). Left, in the @deep tag: the loop-entry `test ebp,ebp; jl` vs our
  js, the out parameter's home-slot init + double reload, and the rep movs block's allocation. 142 -> 115 differing
  bytes, 41 -> 33 asm lines. The same break on the sibling amt_GetColumnOffices was WORSE (its original keeps the
  in-loop return with its own epilogue: 13 -> 21 asm lines) and was reverted.
- amt_GetColumn7Offices (was amt_Unk4e70f0) byte + readable: the column-7 offices (the records 30 .. 36: deputies
  0x1c / 0x1d, the town mayors 0x1e .. 0x21, 0x22), all of them or only the held ones; the yearly check 0x4e9260
  is its only caller.
- amt_GetOfficeCandidates (was amt_Unk4e7150) -> match-deep: renamed from the application list 0x4ea8e0, the AI
  decision 0x497490 (am.c) and the town hall window 0x5a0590; the sorted per-kind candidate list was read against
  the whole original. Kept leads: `n < 1` -> `n <= 0` (cmp 0/jg), the insertion rate test as continue guards (the
  original's `test ah,0x41; jp`), the table pointer walk (the original's [ebp-8]); the found-flag search form grew
  the function to 432 bytes (reverted). 368 -> 352 differing bytes, 132 -> 127 asm lines; Q5 ROUGH removed, Q1-Q7 pass.
- amt_GetOfficeCandidatesFull (was amt_Unk4e7320) -> match-deep: the same list plus `*full` (a rated office with
  cnt >= 4). Kept leads: `n <= 0` and the continue-form insertion guards (384 -> 359 differing bytes, 165 -> 135
  lines); the pointer walk was WORSE here (135 -> 139 lines) and was reverted.
- amt_GetOfficeHolder (was amt_Unk4e7510) byte + readable: the character who holds the first office of the kind k
  (the AI decision 0x490e00 asks for the kind 0x11 holder); its prototype was missing from amt.h and was added (Q7).
- amt_GetSuperiorOfficeHolder (was amt_Unk4e7550) byte + readable: the holder of the office a deputy kind serves
  (0x23 / 0x25 -> the 0x26 historical-event office, 0x24 -> the 0x27 person's office), the election's reference object.
- amt_CanTakeOffice (was amt_Unk4e75c0) -> match-deep: the promotion-eligibility predicate (same column, the three-level
  cycle), used by the application checks cm.c 0x4dada0 / 0x4dae90 and by 0x4e76b0; 99 code + 6 slots + 3 fill hunks
  remain (the original keeps the b.level dword in esi and 3 in edi, the a / b frame base differs by 0x10).
- amt_Unk4e76b0 (0x4e76b0, IN PROGRESS, still in the base queue): the candidate characters for the office kind k
  (up to n, at most 6 offices looked at). The early-return structure (`if (m == 0) return 0;` after
  amt_GetColumnOffices, the loop inline) and the `n >= 6` bound spelling match the original's per-site return-0
  epilogue copy: 167 -> 116 differing bytes, 183 -> 56 asm lines, 104 -> 92 code hunks (104 at session start;
  the source change is uncommitted work in progress). Next: the out[] store (ours keeps a running pointer, the
  original re-indexes out[cnt-1] from the parameter) and Q1 (name from the 1..27 kind check).
- The ai_Unk4e7150 / ai_Unk4e7320 / ai_Unk4e7510 / ai_Unk4e7550 / ai_Unk4e75c0 / ai_Unk4e76b0 declarations in am.h /
  hmt.h / cm.h and the call sites in am.c / cm.c / unk_575860.c / unk_58bbd0.c are the old spellings resolved by
  their Unk<addr> names; not touched.

## match-base 2026-10-25 (base-0 worker, shard 0/6, session 5)

- Shard 0/6, unit amt.c. Check count 2564 -> 2565/4664 (amt_GetOfficeApplicants); grade regressions 0;
  unit differing bytes 11511 -> 11369 (26 -> 27 matched).
- amt_GetOfficeApplicants (was amt_Unk4e76b0) byte + readable: the last-bytes pass found the two spellings the
  session-4 note left: `out[cnt++] = c;` instead of `cnt++; out[cnt - 1] = c;` (ours built a running pointer plus a
  stack counter, the original re-indexes out with cnt in ebx; 92 -> 1 code hunks) and `if (m >= n) m = n;` instead of
  `if (n <= m)` (the original's cmp eax,esi; jl). Frame 0xac -> 0xa8, no slots left. Named from the role (the office
  holders of kind k's column that amt_CanTakeOffice accepts, i.e. the applicants) and the callers: the AI partner
  choice 0x4a0240, ai_FillNearRing 0x4a3af0 and the vote dialog cm.c 0x4dab50.
- amt_GetEligibleOffices (was amt_Unk4e7770) byte + readable: Q1 only. Named from its role (the open offices of the
  column of c's office rank at a level c may apply for, with the two-person stop); the only caller is the indirect
  page fill 0x5bfd90 (special-amount window), which counts the characters whose appliedOffice (+0x168) is in the
  list. Its prototype was missing from amt.h and was added.
- amt_GetOpenOffice (was amt_Unk4e78a0) byte + readable: Q1 only. The two-person event 0x552920 (he.c) asks it for
  the office both created characters apply for (open, state 3, cnt <= 1).
- amt_GetCreateRanges (was amt_Unk4e78f0) byte + readable (Q1 + Q3): the money values are all multiples of 64 (the
  0x4c4830 sites build their amounts as n * 0x40), so they are written 10000 .. 75000 * AMT_MONEY_UNIT (no byte
  change; the bare 4800000 was Q3). Roles of the ranges: s0/s1 the age (the 4th argument of cm_SendCreateCharacter,
  16 .. 32), i2/i3 the starting money (the 0x4c4830 amount), i4/i5 the level field 0xd, i6/i7 the RANK (the 2nd
  argument of gm_TitleNumber; the job GROUP is the random 1 .. 12, the 1st argument).
- amt_CreateApplicant (was amt_Unk4e7aa0) -> match-deep, reason in its @deep tag: the call's declared types (8th
  parameter unsigned char -> the original's `mov cl,[amt_Flip]`, 4th int -> no truncation) took 103 -> 77 differing
  bytes; what is left is the register allocation (the original needs only esi, ours pushes edi and keeps v in esi)
  and the argument push order. Q1-Q7 pass; the original's overlapping dword reads of the age pair are typed as
  AMTRANGEW (packed, word0 at +0 / word1 at +2).
- amt_InitApplicant (was amt_Unk4e7b60) byte + readable: the parameter is the created character record (kind 0x13 at
  +2, netId +4), not a message; AMTCHARVIEW gained kind at +2 (pad002 split into kind + pad003) and the money goes
  through cm_SendMoveGoodsUnchecked. Q4 via AMTCHARVIEW.netId. Its only caller is the two-person event 0x552920.
- amt_GetKindLevel (was amt_Unk4e7c40) byte + readable: Q1 only (the level of amt_Kinds[k]).
- In progress, still in the base queue: amt_Unk4e7c60 (the vote settling of a record list). The whole original
  (Ghidra 0x4e7c60 + the disassembly) was read against the draft: the statement sequence is equal (the two passes,
  the vote loops with ai_TrustedOfTwo / ai_TrustedOfFour, the 4-candidate scan 0x770b80 .. 0x7d5f80, the best / tie /
  random pick, the amt_SendOfficeMessage sends and the cm_GetSlotState waits); the behaviour gap is the frame (ours
  0x60, the original 0x58 - cnt2 / i / cnt3 spill in ours, the original keeps them in registers) and the register
  roles. Readability open: Q1 (amt_SettleOffices), Q2 ai_Unk770b84 (the character records +4 view; amt.h's
  `ai_Chars[][0x21c]` gives ai_Chars[0] / ai_Chars[768] for the `(char *)ai_Unk770b84 - 4` / 0x7d5f80 pair - check
  the bytes), Q3 0x7d5f80, Q4 8 netId raw reads, Q5 ROUGH. The two vote callees are named now; their amt.c calls
  are still K&R (amt.h cannot name AIPER), so a call-site cast is the way (as cm_SendCreateCharacter).
- Renames (project-wide, every reference): ai_Unk4c4830 -> cm_SendMoveGoodsUnchecked (message 0x10, the unchecked
  variant of cm_SendMoveGoods 0x4c47e0; handler 0x4c7010 "as 0xf, unchecked"; 137 references in 17 files),
  ai_Unk4aab90 -> ai_TrustedOfTwo (0x4aab90: which of b and c p trusts more, 1 b / 0 c / 2 neither),
  ai_Unk4aad30 -> ai_TrustedOfFour (0x4aad30: of the four candidates the id p trusts most, tie random).
- Types / prototypes: AMTCHARVIEW.pad002 -> kind (split); AMTRANGEW added; amt.h's `ai_Unk5043d0()` K&R became
  gm_TitleNumber(group, rank) (the definition in f3gm.c; the sibling call sites pass gm_TitleGroup(...) first, so
  the 1st argument is the group).
- Next: `uv run ledger grade next` = amt_Unk4e7c60 again (the rename + Q4/Q3/Q5 work above), then amt_Unk4e7c60's
  neighbours 0x4e8160 onwards.

## match-base 2026-10-25 (base-0 worker, shard 0/6, session 3)

- Renames: amt_Unk4e7c60 -> amt_ElectOffices, amt_Unk4e8160 -> amt_CheckOfficeColumns (both in amt.h / amt.c);
  ai_Unk770b84 -> ai_CharInts project-wide (80 refs, 7 files: amt.h, cm.h, f3snd.h, amt.c, cm.c, f3init.c, f3snd.c;
  the character records ai_Chars at 0x770b80 + 4 as int[][0x87]). The ai_CharInts rename also cleared Q2 in the
  other byte amt.c functions that read the old name (matrix: one byte function read -> readable).
- amt_ElectOffices (0x4e7c60) -> match-deep. Readability: Q1 (the unit's own comment "lost the election 0x4e7c60"),
  Q4 the 8 `*(int *)(x + 4)` -> `((AMTCHARVIEW *)x)->netId` (byte-neutral), Q5 ROUGH removed, Q7 amt.h's K&R
  ai_TrustedOfTwo / ai_TrustedOfFour / cm_GetSlotState -> real prototypes (`struct AIPER *`, int; lint E 0, X 0 new;
  amt.h only amt.c includes it). Q3 left: 0x7d5f80 is the end of ai_Chars; `(char *)ai_Chars + 768 * 0x21c` gives
  1182/1328 B, 510 lines and `(char *)ai_CharInts + 768 * 0x21c - 4` the same, against the literal's 1137/1312,
  499 - the init's `mov OFFSET; sub 4` makes the second loop's allocation match better, so no-WORSE blocks it
  (same dead end as ls.c's 0x7d6a42). Typed-field spellings c[0x166] / ch[0x168] cost +4 asm lines and stay raw.
  Behaviour: 669 code hunks / 499 lines, allocation only (frame 0x60 vs 0x58).
- amt_CheckOfficeColumns (0x4e8160) -> match-deep, 156 code hunks / 96 differing lines -> 26 / 30. Meaning fixes
  (one edit, one build each, all kept): the health test at +8 is unsigned <= 0 (test/jbe, AICHAR.health; AMTCHARVIEW
  gained `health` 0x008), the second ubyte at +0x1b1 -> +0x1af from the new scan base; the character scan walks
  ai_Chars + 2 (the kind byte) with offsets +0x166 / +0 / +0x1af and the bound ai_Chars + 2 + 768 * 0x21c (here the
  symbol form is byte-neutral!); fl zeroed by six ascending single statements BEFORE the cols stores (a loop gives
  rep stosd, the chained a=b=..=0 stores in reverse order); `fl[i] |= 1;` then `fl[i] |= 2;` instead of `fl[i] = r | 3;`;
  the state-2 branch merged to `if (a != 0 && health > 0) { if (b != 0 && health > 0) fl |= 1; else send(0,0,4); }
  else send(b,0,1);`; the tail as `if (fl[i] & 2) ret = 1; else if ((fl[i] & 1) != 0 && ret == 0) amt_ElectOffices(...);`
  and the redundant `&& n > 0` guard dropped. p = list + p++ replaced by list[j]. (flips the loop's base register,
  -15 lines) and the now unused AMTREC *p removed (byte-neutral). Left: the original's extra `test cl,cl; je` (its
  goto-shaped state-2 source measures 33 lines vs 30), the dl/cl health register and the wait-loop placement.

## match-base 2026-10-25 (base-0 worker, shard 0/6, session 6)

- Shard 0/6, unit amt.c. Check count 2569 -> 2571/4664, grade regressions 0, unit differing bytes 11202 -> 10482 (27 -> 29 matched).
- amt_IsColumnDecided (was amt_Unk4e83b0, 0x4e83b0) behaviour + readable (265 -> 11 differing bytes): the column check of 0x4e8160 for
  one column; the return was missing (the original returns the decided bit, the draft always 0 - the council session 0x4d8e20 runs
  amt_ElectOffices only when it is 0). Matching spellings: the state-2 condition in the original's order (a != 0 && health > 0 &&
  a[0x1b1] == 0 && b != 0 && health > 0), the character scan over ai_Chars + 2 (offsets 0x166 / 0 / 0x1af, bound ai_Chars + 2 +
  768 * 0x21c), the second loop as `j = 0; do { .. j++; } while (j < n);` (the for form repeats the n > 0 guard: 2 extra
  instructions), the tail as `if ((char)f & 2) return 1; return 0;`. Left: the second loop's register swap (original j = esi,
  pointer = edi), order 10 / regs 6 hunks.
- amt_UpdateOffices (was amt_Unk4e85d0, 0x4e85d0) behaviour + readable (194 -> 9 differing bytes): the office/character sync after
  the election. AMTCHARVIEW health / officeKind / netId (Q4); the character scan over ai_Chars + 0x168 (ch[0] = appliedOffice,
  ch - 0x168 the record, ch - 0x164 the netId, bound + 768 * 0x21c: Q3); the wait loop as
  `while (r != -1 && cm_GetSlotState(r) == 0) sv_PumpNet();` (if + while rotates the loop, 29 bytes longer). amt_SendOfficeFree
  takes a char (the caller passes the idx byte without an extension; 0x4e65e0 stays byte). Left: the netId load / v = 0 store
  order and the lea register, order 4 / regs 4.
- amt_HasColumn7Holder (was amt_Unk4e86e0, 0x4e86e0) behaviour + readable (56 -> 9 differing bytes): the list button of the
  special-amount menu 0x5c1130. Fix: `if (owner != -1) { if (gm_FindChar(..) != 0) return 1; } else n--;` (the `== -1` first form
  lays the gm_FindChar block out of line). Left: the xor eax,eax / test order at the loop top and in the return-0 epilogue, order 10.
- amt_SendOfficeNews (was amt_Unk4e8730, 0x4e8730) byte + readable: the office news 0x1c19. p is `unsigned short *` (the original
  zero-extends *p); the number is the original's `(flag ? 0x27b : 0x253) + v` ternary - the `(flag ? 0x28 : 0) + 0x253 + v`
  spelling folds into lea [eax+edx+0x253] and keeps one code hunk. The scan bound ai_CharInts + 768 * 0x21c is byte-neutral here
  (the amt_ElectOffices dead end does not repeat).
- amt_SendOfficeLost (was amt_Unk4e87e0, 0x4e87e0) byte + readable: the same sender with the text 0x1c21 (the lost office, no
  second text, nothing when p is 0); the same three spellings.
- Project-wide rename ai_Unk53dda0 / he_Unk53dda0 -> he_ShowNews (0x53dda0, 264 references in 16 files): the event-text/news sender
  to a player character (notes/units/he.md "show a news line"). It made hmt_Update byte + readable (its last Q2 went), whose stale
  @deep tag was cleared; he.c 0x567c00 passed the function 5 arguments through the old K&R alias, the original pushes text2 = 0, the
  call now passes the 6th argument 0.
- Open work: 0x4e88a0 is next, nothing applied. Read this session against 0x4e88a0..0x4e8c20; the meaning fixes found: the rank
  ranges 1 .. 6 and 0x28 .. 0x2d (the draft's != 0 && < 7 / > 0x27 && < 0x2e give test/je where the original has cmp/jb/ja),
  `total >= 3` (not > 2), the holder block as `v = b.owner; if (b.owner == -1) v = a.owner; cur = 0; if (v == -1) best = 0;
  else { cur = gm_FindChar(v); if (cur == 0) best = 0; else best = gm_GetWorth(..); }` (the draft's comma/|| form), the send
  branches test the pick rank 1 .. 6 (not == 0 / > 6) and lay out the d >= 6 arm b<-pick, a<-0, v = 0x1c first and the
  no-current arm b<-pick / v = 0x1c first. Q1 amt_ElectDeputies; Q4 through an AMTBLDVIEW (owner 0x27 / flags5a 0x5a) and
  AMTCHARVIEW.id (planned amt.h addition, not applied); Q5 ROUGH out. The office scan is i = 36 down to 0 (jge &amt_Table[0])
  with a rep movs copy into a (0x1d) / b (0x1c).

## match-base 2026-10-26 (base-7 worker, shard 7/10)

- Shard 7/10, unit amt.c. Check count 2573 -> 2574/4664; ledger check --against 0 regressions; unit differing bytes
  10482 -> 9906, matched 29 -> 30 of 64.
- **amt_Unk4e88a0 -> amt_ElectDeputies** (readable, then -> match-deep, reason in its @deep tag): applied the
  session-6 reading (the rank ranges `>= 1 && <= 6` / `>= 0x28 && <= 0x2d`, `total >= 3`, the holder block as
  `v = b.owner; if (v == -1) v = a.owner; cur = 0; if (v == -1) .. else { cur = gm_FindChar(v); .. }`, the
  no-current send arm as the rank 1 .. 6 -> b <- pick arm first) and four further meaning/type leads: the
  candidate pointer `ch = ai_Chars[id]` hoisted before the rank compare, AMTCHARVIEW.rank / .secondOffice and
  AMTBLDVIEW.owner / .flags5a instead of the raw offsets (AMTBLDVIEW new in amt.h, pack(1) - an unpacked view
  aligns owner to +0x28 and misses), the init order a.owner = -1, b.owner = -1, j = 0, n = 0, total = 0, and
  `if (s > best) { pick = cp; best = s; }`. 662/896 differing bytes and 303 asm lines -> 417/896 and 231; the
  255 remaining code hunks are the register allocation (original n = edi / best = ebx / -1 = ebx, ours n = ebx /
  best = ebp / -1 = eax; the permutation inverts the vote loop and the prologue from the first push).
  `int rb, rc` (not unsigned char) keeps the frame at the original's 0x78+0x10; the unsigned char locals grew
  the function to 912 bytes. `d = abs(rc - rb)` was WORSE (+15 bytes, reverted); the rank conditions written
  `>= 1 && <= 6` on the int rb were worse than `rb == 0 || rb > 6` (the byte compares need the byte-typed
  expression; it is the typed field form that gets them).
- **amt_Unk4e8c20 -> amt_ElectKind22** BYTE + readable (331/432 differing bytes, 102 lines -> 0): the same
  idioms, plus two structural leads the sibling had already given: `n >= 16` instead of `n > 15`, the vote loop
  without the `if (n > 0)` wrapper (one test like the original), a separate `left` from the vote index `j`
  (j = 0 in the prologue, left = 1 before the scan), and the typed `((AMTCHARVIEW *)x)->id` for gm_GetWorth.
  Named from the role: it elects the kind-0x22 office from the type-7 building owners of the ranks 13 .. 18
  (job group 7). The compare `if (s > best)` and the store order `pick = cp; best = s;` are what the original
  has (see notes/vc6-matching.md); the same spelling was applied to amt_ElectDeputies.
- Open work: **0x4e8dc0** (amt_Unk4e8dc0, the town offices 0x1e .. 0x21, 839/1184 bytes, 409 code hunks) read
  but not changed. Its build branch is the AIMSG template of amt_ExecGebaeudeBauen (am.c 0x4a5320): m.type =
  0xd, m.time = ai_Time (+0x2c / +0x30 / +0x34 = the hour / min / third word), m.to = *(ai_PlayerChar + 4),
  m.fromId = -1, m.arg[0] = m.to, m.arg[1] = the building kind, m.arg[2] = 0, m.flags = 2, the world position
  at +0x80 and the angles at +0x90 (each 3 ints), the name at +0x9c. The original's frame is 0x368 vs our
  0x400: it has no `nm[128]` and its position out is 12 bytes (the d3_PointToWorld output, the compiler put it
  in the dead `a` slot - msg +0x80/+0x84/+0x88 read that slot), so our `int pos[6]` is 12 bytes too big and the
  `a.owner == 0 ? pos : pos` argument should be the position buffer. Q2/Q7 debt: ai_Unk4478e0 = d3_SelectScene,
  ai_Unk4222b0 = err_Message, ai_Unk633444 = d3_CurrentScene, ai_Unk458a20 = d3_PointToWorld (canonical names
  in am.h / anm.h / d3.h / err.h). A project-wide rename of the four was tried and REVERTED: the full-prototype
  merge breaks two other units' call sites - sv.c 0x528180 pushes a 4th, uninitialised register into 0x458a20
  (documented there, a @original-shaped alias) and eleven `ai_Unk4478e0(x)` one-argument calls in f3gm.c / gv.c
  sit against a two-parameter prototype (clang: too few arguments, an E error); unk_58bbd0.c 0x5de652's
  no-argument `ai_Unk4222b0()` call is the same shape. Next session: rename in amt.c/amt.h only (canonical
  prototypes in amt.h, alias declarations left in the other units as their type debt) or merge the aliases
  with the call sites' push evidence. Next `uv run ledger grade next` = amt_Unk4e8dc0.

## match-base 2026-10-26 (main checkout)

- **0x4e8dc0 -> amt_CalcZuenfte (name from the function's own error string at 0x60f11c), escalated.** The
  frame lead above was right and is now fixed: the original is `AMTREC a` at 0x1c, the position buffer
  overlapping a (a is dead on the return path) and `v3` at 0x34 -> frame 0x368, and the first 14 instructions
  now match. Source: `int pos[6]` -> `int pos[3]`, `pos`/`v3` declared inside the `if (it == 0)` build branch,
  `nm[128]` dropped (the gb_ name is built in `buf`), and `lo`/`hi` are `int` (the original stores both as
  dwords at [esp+0x10]/[esp+0x18], as bytes they were byte stores). 251 -> 209 differing instructions; our
  code is 32 B longer (unitdiff MIXED) because the block-local declarations changed the allocation.
  Left (deep): allocation - `hi` enregistered in ebx vs [esp+0x18], `cand[16]` at 0x138 vs 0x40, n/edi vs ebx
  (the kind selection and the memset block).
- Q4 blocked header (for a later session): the 26 raw accesses are the 0xd message build. ai.h's AIMSG has
  type/to/fromId/time/flags/arg at the right offsets but no pos at 0x80 (that is inside arg[10]), no angles at
  0x90, no name at 0x9c; am.c's ai_Unk4a5320 builds the same message with the same raw casts, so the typed view
  is an ai.h + am.c change. Q2/Q7: ai_Unk4478e0 = d3_SelectScene, ai_Unk633444 = d3_CurrentScene,
  ai_Unk4222b0 = err_Message are one-line renames (am.h / d3.h already have the prototypes);
  ai_Unk458a20 = d3_PointToWorld needs the canonical (D3OBJECT *, D3VECTOR *, D3VECTOR *) prototype while the
  call passes an int object pointer and `(char *)j + 0x4c` - the type debt this note already records (the
  project-wide rename was tried and reverted). ai_BuildingName / ai_FindBauplaetze are real prototypes in
  amt.h now (those two Q7 entries are gone). Q1/Q5 done (name, ROUGH removed).

## match-base 2026-10-26 (base session 2, main checkout)

- Check count 2599 -> 2603/4664, grade regressions 0, amt.c 30 -> 34 of 64 matched, differing bytes
  9986 -> 9174. Five functions worked:
- **amt_CheckOffices (was amt_Unk4e9260) byte + readable**: the round-start office check. The last-bytes lead
  was the *type*: `lo` / `hi` must be `int` (the original keeps them in esi / edi with `cmp ecx,esi; jl` /
  `cmp ecx,edi; jle`), not `unsigned char` (byte compares) - with ints the loop pointer and count spill to
  [esp+0x10] / [esp+0x14] exactly like the original. The loop is `for (p = list; n > 0; n--, p++)` with
  `p++` in the update clause (p++ as a body statement rotates the loop: the update block lands before the
  body and entry jumps over it). The fits test is written positively (`if (fits) continue;`), which gives the
  original's `jl` to the loss block and `jle` to the loop bottom. The pump is
  `while (r != -1 && cm_GetSlotState(r) == 0) sv_PumpNet();` - the `if (r != -1) { while (...) }` form makes
  VC6 invert the loop (duplicated test) and cost 90 bytes. Q4 through AMTCHARVIEW (health / secondOffice /
  rank). Called from main.c 0x53b2e0; main.c gained the prototype (the old call was an implicit
  `ai_Unk4e9260`).
- **amt_HasTownObject (was amt_Unk4e9400) byte + readable**: the branches were mirrored (the CalcZuenfte arm
  first, `!= 0` returning 0) and `best`/the `it != 0` tail are `if (it != 0) { if ((it->flags5a & 1) == 0)
  return 1; } return 0;` (the `if (it == 0) return 0; if (flag) return 0; return 1;` form is folded to a
  boolean: mov al / not / and). f3gm.c keeps a K&R `int amt_HasTownObject();`: the typed `(int k)` prototype
  there costs 1 byte in gm_Unk4fa4b0 - type debt.
- **amt_CanUseMayorPower2 / amt_CanUseMayorPower3 (was amt_Unk4e94e0 / amt_Unk4e95a0) byte + readable**: the
  same body as 0x4e94a0 with level >= 2 / >= 3, power flags 0x8000000 / 0x10000000 and money >= 5 / >= 8;
  `cv->secondOffice` keeps the byte compares, `(unsigned char)ai_BTypes[it->type * 0x24d + 0x247] >= 2` gives
  `cmp ...,2; jb`, the `-1` arm first inlines `or eax,-1`, and `>= 5` / `>= 8` give `setge` (not `setg`).
- **amt_ExecutePriceMessage (was amt_Unk4e9660) -> match-deep**: readable (Q1-Q7 pass), behaviour left as
  allocation. Meaning fixes: `best` is `int` (signed `lvl` compare), the district guard is the two unsigned
  compares, the lower price factor is `1.0f - ((rf + 1.0f) * f * 0.3f + 0.2f)` with **double** 0.2 / 1.0
  (the original uses fadd/fsubr QWORD - `(double)0.2` is the widened 0.2, rule 7). Q3/Q4: `amt_PriceRows`
  (0x61a348, 37 rows of 0x15) and `AMT_CHARS_END` (0x7d5f84) in amt.h, `AMTGOODS` (the 0x80-byte goods
  record: amount 0x20 / mean 0x24 / price 0x38), `AMTBLDVIEW.type` / `.id`. Q2: gm_GoodsValue,
  gm_JobBuildingType, gm_Shuffled, sv_MessageBox for the old ai_ aliases; gm_GetSkill and cm_SendGoodsDelta
  renamed project-wide. Left for deep: frame 0xab8 vs 0xad8 (32 bytes of locals), c in ebp vs a slot, the
  zero constant in ebx, the news-loop tail placement (spell reload / width / dup-tail tried, 581 differing
  bytes).
- **amt_ExecuteShareMessage (was amt_Unk4e9a00) behaviour + readable** (only fill hunks left): the loss is
  `(int)(((util_RandomF() + 1.0f) * f * 15.0f + 5.0) * 2.55f)` (5.0 / 2.55 double, 15.0f float) - the draft
  had `f * util_RandomF()`; the shared amount is `(int)((float)m[2] / cnt) + 0x20` (fild/fidiv, not the
  count). The owner compare is `o->owner != (unsigned short)*c` - comparing `unsigned short` with `short`
  widens with movsx, the cast keeps the original's 16-bit `cmp ax,[esi]` and took the function from draft to
  behaviour (235 instructions equal). `AMT_NETID` / `AMT_BLDOWNER`-style fields, `o->id`, and the loop bounds
  through `AMT_CHARS_END` (`(int)q < (int)AMT_CHARS_END + 0x168` - a pointer compare gives `jb`, the int cast
  gives the original's `jl`).
- Renames (project-wide, every use): amt_Unk4e9260 -> amt_CheckOffices, amt_Unk4e9400 -> amt_HasTownObject,
  amt_Unk4e94e0 -> amt_CanUseMayorPower2, amt_Unk4e95a0 -> amt_CanUseMayorPower3, amt_Unk4e9660 ->
  amt_ExecutePriceMessage, amt_Unk4e9a00 -> amt_ExecuteShareMessage, unk_Unk504ec0 + ai_Unk504ec0 ->
  gm_GetSkill (0x504ec0, the skill value 0..1 of a character for a skill index), ai_Unk4c5710 ->
  cm_SendGoodsDelta (0x4c5710, message 0x40: the goods record differences). The ai_ aliases of the amt_
  functions stay (they resolve by address).
- Build note: one full rebuild reported `check: REGRESSED: ai_CondCharShare` (unk_4b39f0.c, an fild/fdivp vs
  fidiv difference) although nothing in that unit changed; rebuilding the same tree gave the match back each
  time. Nothing was reverted for it; a `check` right after a rebuild is the reliable one.

## match-base 2026-10-25 (base-0 worker, shard 0/6, session 6)

- Shard 0/6, unit amt.c. Check count 2603 -> 2608/4664 over the session (amt.c 34 -> 38/64 matched, sv.c 49 ->
  50/137); grade regressions 0; amt.c unit differing bytes 9174 -> 8514. Raised byte + readable this session:
  amt_ChangeDistrictOpinion (0x4ea550), amt_GetDistrictLiking (0x4ea5c0), amt_LoadAemter (0x4ea7a0),
  amt_GetOwnedOfficePay (0x4ea990); escalated: amt_ExecuteMayorPowerMessage (0x4e9ce0), amt_SaveAemter (0x4ea690).
  The amt_ExecuteMayorPowerMessage line: 726 -> 517 asm lines, our size 1968 -> 2048 of 2148.
- amt_Unk4e9ce0 -> amt_ExecuteMayorPowerMessage (byte work handed to match-deep, reason in its @deep tag; readable):
  the office-power request of a mayor (the offices 0x1e .. 0x21) - 0x1e denounces the office of a target
  (m = {1, netId}), 0x1f / 0x21 / 0x20 hit the office owners of a district (m = {4, district}) with the damage
  roll, two-sided / one-sided. Three meaning fixes drove the whole session:
  - the damage roll is the shareholder formula of amt_ExecuteShareMessage: `f = gm_GetSkill((char *)c, 4);
    f = (util_RandomF() + 1.0f) * f * 15.0f; dmg = (int)((f + 5.0) * 2.55f);` - the draft had
    `(int)(gm_GetSkill(c, 4) * util_RandomF())`, which also hid the __ftol and the 5.0 / 2.55 doubles. The fp
    sequence then matches the original byte for byte (two fp hunks left, both stack offsets).
  - the character id the texts print is an `unsigned short` (the original zero-extends: `xor eax,eax; mov ax,[t]`);
    `*(short *)t` signed-extended. AMTCHARVIEW.id is the right view.
  - the netId of c is *not* a local: the original reloads `[c+4]` at every use (`mov ecx,[esi+4]`); with
    `AMT_NETID(c)` in place of a `cid` local the function lost 4 stack bytes (frame 0x8e0 -> 0x8dc, the
    original's) and 678 -> 517 differing lines.
- Last-bytes work kept: the rf arms are written `if (rf > 0.3f) { ... } else { ... }` (the original's
  fall-through blocks are the > 0.3 texts, the `jne` jumps to the <= 0.3 ones; 581 -> 517 lines); the district
  guard is `(unsigned int)m[1] >= 1 && (unsigned int)m[1] <= 6` (the original's `cmp 1/jb` + `cmp 6/ja`);
  the id word of the character is `AMTCHARVIEW.id`, the news bounds `AMT_CHARS_END` (Q3).
- Ideas that changed nothing (declaration/slot order does not follow the source): AMTKIND k before/after the
  AMTREC list[6] and in a nested block, the `ret` conditional as if/else and as a ternary, the `c` parameter as
  short * / char *. VC6 keeps its own aggregate slots: our k at [esp+0x44], the list at [esp+0x5c], the original
  the other way round - slotmap / c2prio work.
- Left (all in the @deep tag): the prologue order (ours calls util_RandomF before the pushes, the original after
  them) and the register roles from instruction 0 (original c=esi, t=ebx, ret=ebp; ours c=ebp, t=esi, ret in
  [esp+0x18]).
- The four byte matches in the order they were found:
  - amt_ChangeDistrictOpinion (0x4ea550): the guard `d >= 1 && d <= 7` (not `d > 0 && d < 8`: the original's
    cmp 1/jl + cmp 7/jg) and AMT_NETID(c) for the two `*(int *)(c + 4)`.
  - amt_GetDistrictLiking (0x4ea5c0): the same guard spelling, the ids through AMTCHARVIEW.id (unsigned short,
    the original zero-extends where *(short *)o signed-extended), the `sum = ...; cnt++;` order (the original's
    inc ebx between the fmul and the fadd) and amt.h's `float gm_Liking();` -> `float gm_Liking(int a, int b,
    int c);` (Q7; am.h / ai.h / f3snd.h already had it).
  - amt_LoadAemter (0x4ea7a0): no `extra` local - `expect = (((version < 0x10051) - 1) & 5) + 0x25;` compiles to
    setl/dec/and/add (the draft kept `extra` in esi and `lea ebp,[esi+0x25]`), and the plain `for (i = 0; i <
    expect; i++)` gives the original's rotated loop with the test-ebp/jbe entry guard (the dead
    `if (extra != 0xffffffdb)` wrapper was the difference).
  - amt_GetOwnedOfficePay (0x4ea990): the decision record is **9 ints** (gv_GetTableRecord copies 9) and the pay
    rate index is `buf[6]`, not `buf[0]` (the draft read buf[0] out of an 8-int buffer): `int buf[9]; int pay;
    pay = (int)amt_Pay[buf[6]];` byte-matched at once (the frame 0x24 -> 0x2c, 4/112 bytes differ at session start).

## match-base 2026-10-30

- amt_ExecuteShareMessage (0x4e9a00) byte + readable: the two `cmp word ptr [..], -1` were `(unsigned short)q[-0x5b] != 0xffff`
  (the record id is unsigned); that also removed the second epilogue. The money call goes through the f3gm thunk
  gm_SendMoneyDelta (0x50cc50, a forwarder to cm_SendMoneyDelta): the original's site refers to the thunk (check M8).
