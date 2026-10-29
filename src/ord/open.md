# ord.c (new, layout 2026-10-22 session 17)

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** first; below it the findings of each session, one section per author.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (guess): order handlers, grid statistics of the characters
- Boundaries: open (layout 2026-10-22 session 17): the order handlers of the characters. Evidence: the table 0x611768 (0x3a pointers, 0x4f4e20 .. 0x4f8140, dispatcher 0x4f81c0) sits right after gv's strings (0x611700), then the strings of the privilege dialog 0x4f8200 (0x611850 "$Z%s$N", "privillegien\prv_*") and the big model-name table 0x6118ac .. 0x61774c (used by 0x4f8890 / 0x4f8ae0 and later units); codegen: 0x4f5d50 / 0x4f5f00 match only without 0x4f2870 (gv.c) defined above them. Handlers 0x4f4e20 .. 0x4f8140 drafted (most matched), then the grid statistics (0x4f82a0 clear, 0x4f8300 cell, 0x4f8360 / 8460 / 8590 / 8920 grid sums, 0x4f8700 pick target, 0x4f8890, 0x4f88c0 history, 0x4f9170 / 9190 / 9710 record message, 0x4f9c00 / 9ce0 / 9d40 / 9e00 ratings). End: 0x4f9e30 uses the strings 0x61774c "misc\statistics_taxes" = the first user of the next string block (statistics / head icons / Character_Names.dat), so 0x4f9e00 is the last function of ord.c (0x4f9e30 is the next unit, f3gm.c). All drafted (layout 2026-10-23, ROUGH: 0x4f8ae0, 0x4f8ed0, 0x4f9020, 0x4f9220, 0x4f9850; 0x4f9764 is the memcpy tail of 0x4f9710, not a function). Open: incomplete, name is a guess
- Range then: 0x4f4e20..0x4f9e2f
- Was: guess

## Open work
- Drafted in layout 2026-10-23 (ROUGH, compile, none matches yet): 0x4f8ae0 (the statistics: out[10] floats), 0x4f8ed0 (smooths and sends them), 0x4f9020 (loads them from a saved record), 0x4f9220 (the event generator, record 0x91b480), 0x4f9850 (texts from the grid columns). 0x4f9764 is the memcpy tail of 0x4f9710 (Ghidra split it), not a function. The float expressions follow the pseudocode (floats vs doubles unchecked); the grid columns are 0x91ab00 + 0xf0 .. 0x138.
- Near misses: the level / job handlers with `(c[9] ? 10 : 0) + 0x114 + level` (0x4f4f20, 0x4f5200, 0x4f53c0, 0x4f54a0, 0x4f5580, 0x4f5660, 0x4f5730, 0x4f58e0, 0x4f59b0, 0x4f77d0, 0x4f78b0, 0x4f7990) and `+ 0x253 + job` (0x4f6180, 0x4f64e0, 0x4f65c0, 0x4f7000): 28 / 93 bytes, all one cause (the original's `add; add` instead of `lea`, see notes/vc6-matching.md); 0x4f4e20 (register allocation m = ebx, c = edi, r = esi, `r` assigned late; the shared `return 1` of the first condition), 0x4f5800 (the scan loop `if (id != -1) { if (job != 0x12) break; if (job > 0x15)..}` is folded by our compiler), 0x4f5a80 (only `push ebx` placement: the original pushes ebx after the two early returns), 0x4f8140 (the constants 6 / 2 are kept in registers in the original), 0x4f8360 / 0x4f8460 / 0x4f8590 / 0x4f8920 (grid code, drafts only), 0x4f82a0 (our compiler drops the first memset of 0x91b480: 16 B), 0x4f8700 (4 B: eax / edx swap), 0x4f9190 / 0x4f9710 (the original copies the 0x24-byte record with the generic unaligned block copy, `lea ecx,[edi+edi*2]; and ecx,3 ..`, ebp frame; memcpy / struct assignment / pack(1) did not give it), 0x4f9c00 (87 B), 0x4f9d40 (12 iterations over 10 weights).
- Names: ord.c is a guess; gv.h is shared with gv.c (split ord.h later).

## layout 2026-10-22 session 17
- The handlers have the shape `int f(char *c, int *m)`: m[0] the kind (1 / 3 ask, 2 / 4 answer), a player (c[2] == 6) gets the text through `ai_Unk5176d0(m + 5, ai_Unk5f8318, id)` (kind 1) or a dialog `ai_Unk4f8200(buf, mode, 0x657c7)` (kind 3, mode 0 / 1 / 2 selects privillegien\prv_small / prv_big / prv_very_big); results 1 = not handled, 2 = handled, 4 = level too high, 9 = flag 0x200, 0xa for answers. Families: levels (c[0xd]), jobs (c[0x166] / c[0x169], with the answer branch through m[0x87], the message's partner character), flags of +0x1cc, and the record ones (0x4f5c30, 0x4f5d50, 0x4f5f00, 0x4f7a60, 0x4f7b80, 0x4f7e40, 0x4f7fc0, 0x4f6b40: clear the text at m + 0x14, `r` = callee result | 2 / 0xa).
- The table 0x611768 (0x3a handlers by order number, dispatcher 0x4f81c0 returns 0x40 for a bad number or no character, 1 for a character kind +2 >= 10).
- The grid: 8 x 8 cells of 0x18 bytes at 0x91ae60 (+0 / +2 shorts, +4 byte count, +8 int average, +0xc / +0x10 / +0x14 floats), cell index y + x * 8 from 0x4f8300 (scene cell size = *(0xfd7c50 + 0x20) / 8); the 3 x 3 neighbourhood loops of 0x4f8460 / 0x4f8590 / 0x4f8920 clamp with `setl; dec; and`. 0x4f82a0 clears the tables, 0x4f88c0 shifts a 15-record history (0x91ab00, 0x14 bytes each).
- Message of 0x4c5e80: tag at +0x24: 'init' 0x696e6974, 'set ' 0x73657420, 'dsbl' 0x6473626c, 'enbl' 0x656e626c, handled by 0x4f9710 (record 0x91b480, 9 ints).
- Matched this session: 0x4f4ff0, 0x4f5070 / 0x4f50f0 (earlier), 0x4f5190 .. 0x4f5350, 0x4f5c30, 0x4f5d50, 0x4f5f00, 0x4f60b0, 0x4f6270 .. 0x4f6410, 0x4f66b0 .. 0x4f6b40, 0x4f6c60 .. 0x4f75e0, 0x4f76e0, 0x4f7a60 .. 0x4f8070 (flag family), 0x4f7e40, 0x4f7fc0, 0x4f81c0, 0x4f8200, 0x4f8300, 0x4f86a0, 0x4f8890, 0x4f88c0, 0x4f9170, 0x4f97a0, 0x4f9ce0, 0x4f9e00.

## layout 2026-10-23 session 4
- 0x4f8ae0: counts the characters of type 0 / 1 with their two objects (+0x16c / +0x170) per grid cell (+0x14 of 0x91ae60), sums the building records through the kind table 0x617468 (4 shorts per kind: +0 value, +2 / +6 flags, +4 count), then out[0] = 0x4f9ec0 (), out[1] / out[3] = 1 - objects / characters, out[2] = (1 - (0x4f9d40 * 0.7 + 0x4f9ce0 * 0.3)) * the grid sum, out[9] the mean rating. 0x4f8ed0 keeps 0x6f1b9c / 0x6f1ba0 as a smoothed rating (0.95 and 0.5 factors) and the rate 0x91aaf4 = (cur - base) * 0.0007336182.
- 0x4f9220: record 0x91b480 [0] enabled, [1] counter, [2] time, [3] .. [6] float accumulator (base / scale / current / limit), [7] phase 0 .. 3 (message / object 0x59 / 0x4e / 0x50), [8] stage; when the accumulator passes the limit a 16-entry phase table (by stage) is drawn from and the message is sent with a time 0x4fe140 later; the record goes out with the tag 'set ' (0x73657420) through 0x4c5e80 (handler 0x4f9710).

## match-base 2026-10-25 (base-0 worker, shard 0/6)

- 0x4f2720 calls renamed ai_Unk4f2720 -> gv_GetTableRecord; byte unchanged.

## match-base 2026-10-26 (base-6 worker, shard 6/10)

- 0x4f4e20 (renamed ord_ApplyForOffice, order table entry 0): the original's shape is `r = 2` inside the
  guard + `if (m[0] == 3) {...} else if (m[0] == 1 && c[2] == 6) {...}` with ONE final `return r` (the
  0x4f4ff0 idiom; a `return r` inside the if makes VC6 fold the constant and drop the register). 240 -> 191
  differing bytes; left @deep: m = ebx / c = edi / r = esi in the original (ours m = edi, c = ebx), its
  return-1 block at the end of the function, and its loop return `mov eax,esi` (ours constant-folds `mov
  eax,2`). Tried and reverted: early-return guard `if (!(m[0] == 3 || m[0] == 1)) return 1;` (gives two
  `je`, not `je; jne`), explicit `else return 1`, `goto fail` label at the end, `int partner` / `char kind`
  temps, `int *msg = m` (copy-propagated back), declaration-order permutations, unsigned r, unsigned char
  pointers.
- 0x4f4f20 (renamed ord_LevelOrder): the `(c[9] ? 10 : 0) + 0x114 + level` lea problem is solved by the
  complete-constant ternary `(c[9] != 0 ? 0x11e : 0x114) + level` (notes/vc6-matching.md has the rule; this
  is the first confirmed byte-level hit in ord.c): `and eax,0xa; add eax,0x114; add eax,ecx`. The `unsigned
  char lv` local is NOT in the original: read `c[0xd]` twice, VC6 keeps the byte in cl and the frame stays
  0x100 (with lv the frame is 0x104 and lv is spilled to [esp+4] because cl is reused for the constant 6 of
  `c[2] == 6`). Best shape: `if (c[0xd] >= 3) r = 4; else { r = 2; if (kind 3 && player) {...} else if (kind
  1 && player) {...} } return r;` with the level test BEFORE the if/else-if chain and one final return.
  137 -> 97 differing bytes.  The SAME spelling is owed to the sibling level handlers 0x4f5200, 0x4f53c0,
  0x4f54a0, 0x4f5580, 0x4f5660, 0x4f5730, 0x4f58e0, 0x4f59b0, 0x4f77d0, 0x4f78b0, 0x4f7990 and to the job
  handlers with `+ 0x253 + job` (0x4f6180, 0x4f64e0, 0x4f65c0, 0x4f7000): use `(c[9] ? 0x27b : 0x253) + job`.
  Left @deep: the original keeps m in esi and r in edi (ours the same rank, names swapped), its r = 4 block
  at 0x4f4fad between the kind-3 and kind-1 text blocks (ours after the if/else).
- Readability, unit-wide: gv.h has ORDCHAR (the 0x21c-byte character records as the handlers read them:
  kind +2, netId +4, health +8, partner +0x5c; `ORD_CHARS` view macro, sizeof is 0x21c so `ORD_CHARS[i]`
  keeps the original's `lea` chain) and ORDEVENT (ownerId +0xac, targetId +0xb0). Renames: ai_Unk5c6d00 ->
  ai_OfficeApplication (the office application window of the ask kind), ai_Unk5f8318 -> txt_PercentS (the
  "%s" literal at 0x5f8318 that txt_Format dispatches on: >= 0x4800 is a string pointer, below is a text
  id), ai_Unk4f8200 -> ord_PrivilegeDialog, 0x4f4e20 -> ord_ApplyForOffice, 0x4f4f20 -> ord_LevelOrder.
- The complete-constant ternary applied unit-wide (2026-10-26, after the 0x4f4f20 hit): 11 level-handler lines
  `(c[9] != 0 ? 0x11e : 0x114) + level` and 17 job-handler lines `(c[9] != 0 ? 0x27b|0x286|0x28c|0x297|0x298|
  0x29d|0x29e|0x29f|0x2a0 : 0x253|0x25e|0x264|0x26f|0x270|0x275|0x276|0x277|0x278)` now match BYTE: 15
  functions (0x4f5200, 0x4f53c0, 0x4f54a0, 0x4f5580, 0x4f5660, 0x4f5730, 0x4f58e0, 0x4f59b0, 0x4f68d0,
  0x4f69a0, 0x4f6a70, 0x4f77d0, 0x4f78b0, 0x4f7990 and the jobs 0x4f6180 / 0x4f64e0 / 0x4f65c0 / 0x4f7000
  among them). ord.c 48/84 -> 63/84 matched, 5938 -> 4948 differing bytes; check 2573 -> 2588.
- 0x4f5800 (@deep): the partner is c + 0x5c (not m[0x17], 0x4f5857 reads [ebx+0x5c] with ebx = c), the scan
  breaks on `job <= 0x15` (0x4f5845 cmp cl,0x15; jbe exit, both tests go to the exit block), and the id is the
  unsigned short field vs 0xffff. Left: m = edi / c = ebx in the original (ours m = esi, c = edi) and its loop
  counter owning edx (so the id compare keeps the immediate where ours hoists 0xffff into edx).

## match-base 2026-10-26 (base-1 worker, shard 1/6)

- Order table 0x611768 read from the exe (58 entries, 0x4f4e20 .. 0x4f8140; the next dword at 0x611850 is the
  "$Z%s$N" literal): the handler name now says which test it makes, the table index is in the description.
  Renamed (readable, byte unchanged): 0x4f5200 ord_Level3Order, 0x4f53c0 ord_Level4Order, 0x4f54a0
  ord_Level4Order2, 0x4f5580 ord_Level4To5Order, 0x4f5660 ord_Level5UpOrder, 0x4f5730 ord_Level5UpOrder2,
  0x4f58e0 ord_Level6UpOrder, 0x4f59b0 ord_Level6UpOrder2, 0x4f77d0 ord_Level7Order, 0x4f78b0 ord_Level8Order,
  0x4f7990 ord_Level9UpOrder (these test c[0xd] = level), 0x4f60b0 ord_Job11Order, 0x4f6180 ord_JobMultiOrder
  (0xd / 0xf / 0x15 / 0x1b), 0x4f6270 ord_Job28Order, 0x4f6340 ord_Job29Order, 0x4f6410 ord_Job34Order,
  0x4f64e0 ord_JobRangeOrder (0x1e .. 0x21), 0x4f65c0 ord_JobMultiOrder2, 0x4f68d0 ord_Job17Order, 0x4f69a0
  ord_Job17Order2, 0x4f7000 ord_JobMultiOrder3, 0x4f6a70 ord_JobMultiOrder4, 0x4f6c60 ord_Job10Order, 0x4f6d30
  ord_Job15Order (they test +0x166 / +0x169 = job / job2), 0x4f7ca0 ord_Flag800Order, 0x4f7d70 ord_Flag800Order2,
  0x4f7ef0 ord_Flag200Order, 0x4f8070 ord_Flag400Order (privilege flags of +0x1cc).
- ORDCHAR (gv.h) got the fields the handlers read: textVariant +9 (nonzero picks the alternate text block, +10
  for the level texts / +0x28 for the job texts), level +0xd, job +0x166, job2 +0x169, flags1c8 +0x1c8,
  flags1cc +0x1cc; ORD_CHAR(p) is the single-record view. The frame of 0x4f6a70 / 0x4f6c60 / 0x4f6d30 is
  unchanged (ORD_CHAR(c)->id = the old `*(unsigned short *)c`).
- 0x4f5a80 ord_WageReportOrder: the wage report order (table entry 17); ORDWAGES (gv.h) is the record of
  gm_CharacterWages (0x4fa700, renamed from ai_Unk4fa700 / unk_Unk4fa700 - the two names of one function were
  folded) and the Q4 casts are gone. Escalated @deep: the body is 149/149 instructions equal, frame 0x410, the
  register roles equal (ebx = base, esi = c, edi = buf); the ONLY difference is the callee-saved save
  placement - the original pushes esi at entry, ebx at 0x4f5ada (after the two return-1 blocks) and edi inside
  the kind-3 block, ours pushes ebx in the prologue. Tried and reverted: declarations of o / buf / base inside
  the kind-3 block, the spell push-pop / order hints, early-return guards. Needs the register-allocation
  toolbox (vargen / push-pop block).
- Also readable: 0x4f9170 ord_GetMessageRecord (ord_MessageRecord = the 9-int record 0x91b480),
  0x4f97a0 ord_GetRatingByte (ord_RatingBytes = 0x91b460), 0x4f8890 ord_GetKindRecord (ord_KindTable =
  0x617468, ORDKIND / ORD_KINDS view), 0x4f9ce0 ord_StatisticsRating.
- OWED by the next base session: the remaining 20 handlers fail Q2 + Q7 on one ai_Unk5c**** callee each. Q7
  needs the callee's real 2-argument signature: every caller pushes (c, m) while the unk_58bbd0.c drafts
  declare one `int param_1` (e.g. 0x5c9c50 already takes two, 0x5cd400 takes one). Fix the definition and the
  gv.h prototype together, then the handler only needs its name + the ORDCHAR flags. 0x4f8140 and 0x4f5800 are
  still behaviour drafts (0x4f5800 has @deep leads in the session note above).

### match-base 2026-10-26 (base-1 worker, shard 1/6) - the rest of the pass

- The whole ai_Unk5c**** family the order handlers call is the "Privilegien" of the professions (the strings
  "privillegien_medicus" etc. in unk_58bbd0.c name them). Named by that role: ai_MedicusPrivilege,
  ai_SpyTargetHandler, ai_BlackmailBuildingListener, ai_BlackmailContact, ai_BlackmailContactVariant,
  ai_EmbezzlementPrivilege, ai_MisappropriationWindow, ai_EmbezzlementContract, ai_CounterEspionage,
  ai_CounterEspionageWindow, ai_EspionageWindow, ai_EspionagePayment, ai_EspionageOrderHandler,
  ai_EspionagePayment2, ai_EspionageRelation, ai_EspionageAmountWindow, ai_SpyTargetFlagHandler,
  ai_EspionageOverviewHandler, ai_InterrogationWindow, ai_DismissalHandler, ai_InterrogationWindow1,
  ai_InterrogationWindow2, ai_EspionageResult, ai_LawPrivilegeWindow, ai_WorkerDismissalWindow.
- Q7 for their callers: every caller pushes (c, m), so gv.h now declares each of them
  `int name(char *c, int *m);` and the unk_58bbd0.c draft definition line carries a `@original` tag (its
  int / one-parameter view is what the body's own byte offsets need; the call sites are the evidence). This
  is the recipe for the rest of the family.
- Handlers raised to byte + readable this way (names, the ORDCHAR flags1c8 / flags1cc / job / job2 / level /
  textVariant fields and the ORDMSG text / answerFlag / partner fields replace every cast): 0x4f4ff0
  ord_EspionageResultOrder, 0x4f50f0 ord_SpyTargetOrder, 0x4f5190 ord_BlackmailBuildingOrder, 0x4f52e0
  ord_BlackmailContactVariantOrder, 0x4f5350 ord_BlackmailContactOrder, 0x4f5c30 ord_EmbezzlementOrder,
  0x4f5d50 ord_LawOrder, 0x4f5f00 ord_LawOrder2, 0x4f66b0 ord_EmbezzlementContractOrder, 0x4f67c0
  ord_CounterEspionageOrder, 0x4f6b40 ord_MisappropriationOrder, 0x4f6e00 ord_CounterEspionageWindowOrder,
  0x4f6f10 ord_EspionageWindowOrder, 0x4f70f0 ord_EspionagePaymentOrder, 0x4f7200 ord_EspionageOrder,
  0x4f7300 ord_EspionagePaymentOrder2, 0x4f7400 ord_EspionageRelationOrder, 0x4f74f0 ord_EspionageAmountOrder,
  0x4f75e0 ord_SpyTargetFlagOrder, 0x4f76e0 ord_EspionageOverviewOrder, 0x4f7a60 ord_InterrogationOrder,
  0x4f7b80 ord_DismissalOrder, 0x4f7e40 ord_InterrogationOrder2, 0x4f7fc0 ord_InterrogationOrder3.
  0x4f8140 ord_DismissWorkerOrder readable, escalated @deep (allocation: the original keeps m[0] in ecx, the
  constant 2 in eax and pushes ebx+esi; our r = 2 shapes all fold or pick edi - tried in the @deep reason).
- 0x4f81c0 ord_DispatchOrder: the table 0x611768 is now `int (*ord_OrderHandlers[0x3a])(char *c, int *m)`
  (typed function-pointer array: Q7 clean, the call is `ord_OrderHandlers[id](c, m)`).
- ORDMSG note: `answerFlag` (0x214) must stay an `int` - the original stores a dword
  (`mov DWORD PTR [esi+0x214],1`); as a byte it cost 0x4f5d50 / 0x4f5f00 their byte match.
- 0x4fa700 gm_CharacterWages: gv.h's prototype takes `unsigned short id` with the f3gm.c definition tagged
  @original - a 32-bit id adds a `xor ecx,ecx; mov cx,[esi]` at the 0x4f5a80 call site.
- OWED: 0x4f5070 (needs role names for the int +0x24 / +0x2c fields), 0x4f8300 / 0x4f86a0 (the scene global
  0xfd7c50 has two names: unk_Ptrfd7c50 here, ai_Unkfd7c50 in cm.c; 0x45f170 needs a unit-local @original
  prototype view), 0x4f88c0 / 0x4f9e00 (0x91ab00 / 0x91b4c0 views + ai_Unk6f1b9c as a float), 0x4f82a0 /
  0x4f8ae0 / 0x4f9220 / 0x4f9850 / 0x4f9c00 (behaviour drafts).

### match-base 2026-10-26 (base-1 worker, shard 1/6) - statistics tables

- 0x4f82a0 ord_ResetStatistics: **byte + readable** (was 21/28 instructions). The original's first clear
  (9 dwords at 0x91b480) was missing from our build: `memset(ord_MessageRecord, 0, 0x24)` followed by
  `*(int *)ord_MessageRecord = -1` makes VC6 drop the WHOLE memset (not just the overlapping dword; a
  dead-store kill of the inlined memset - volatile, a local pointer and a type view do not help). The store
  now goes through a second name for the same address, `ord_MessageRecordEnabled` (gv.h, a data.csv alias;
  declared as a distinct extern so the compiler cannot see the aliasing), and the function is byte-identical.
  Evidence for the name: 0x4f9710 sets it 1 / 0 on the 'enbl' / 'dsbl' message and 0x4f9220 runs its event
  only at 1. The only caller is main_Unk537de0 (0x537e3f, "start of a game"), hence the name.
- Renames (byte-neutral, project-wide): 0x4f82a0 -> ord_ResetStatistics (was unk_Unk4f82a0 in ord.c,
  ai_Unk4f82a0 in main.c); ai_Unk6f1b9c (0x6f1b9c) -> ai_RatingBase (the base the rating change rate
  0x91aaf4 is measured from, 0x4f8ed0: rate = (ai_Rating - ai_RatingBase) * 0.0007336182; ai_Rating
  0x6f1ba0 is the smoothed one); unk_Tab91b500 -> gm_CharacterStatistics (folded with f3gm.c's alias
  ai_Unk91b500: "the character statistics (0x68 bytes)", 0x4fd7e0 / 0x4fdfd0); unk_Tab91ae60 ->
  ord_GridCells (the 8 x 8 grid of 0x18-byte cells); unk_Tab91ab00 -> ord_StatisticsHistory (15 records of
  0x14 bytes, shifted by 0x4f88c0); unk_Tab91b4c0 -> ord_StatisticsRecord (the 10 floats 0x4f8ed0
  recomputes and 0x4f9020 loads).

- 0x4f8360 ord_CountGridObjects: **readable**, escalated @deep (allocation: the count-clear loop's induction
  variable; the reason lists every source shape tried). Meaning read against uv run disasm 0x4f8360 0x100:
  the nested 8 x 8 count clear, the object loop (gm_BuildQuery(1, 6) / gm_BuildNext, the +0x61 object's +0x4c
  position, the value gm_EquipmentValue, o[0x5c] the building level) and the average loop all match; the
  frame 0x10c and the ebx / esi / edi pushes are the original's. Readability: the grid is typed
  (ORDCELL in gv.h, 0x91ae60 = ai.h's ai_Grid; AIGRID itself must not be touched: adding the same fields
  there regressed ai_StartBuildingWork's slots, notes/vc6-matching.md) and the building record got the
  ORDBLD view (gv.h, packed, the fields ord.c reads: level 0x5c, object 0x61).
- Renames: unk_Unk4f8300 / ai_Unk4f8300 (both names of 0x4f8300, ai.c and ai.h) -> ord_GetGridCell, its
  prototype now `char *p` in gv.h and ai.h (the callers pass a position pointer; that also clears the lint X
  conflict between the two headers). unk_Unk4f8360 -> ord_CountGridObjects.

- 0x4f8460 ord_AddGridAmount and 0x4f8590 ord_SubGridAmount: **readable**, escalated @deep (both: allocation /
  register-save placement; the reasons list the read and the leads). Read against uv run disasm: the
  gv_GetTableRecord (kind & 0xff), gm_FindById(&b, 0, 0, who), amount / y = util_Random(8), the object guard,
  ord_GetGridCell(object + 0x4c, &x, &y), the +2 add / sub, the xhi / yhi clamp (mov [esp+0x3c],8 + cmp/jg), the
  x - 1 / y - 1 setl-dec-and clamp and the 3 x 3 loop (index y + x * 8) all match. The original defers the
  callee-saved saves into the if block (ebx / esi / edi) and the outer loop (ebp) and spills amount to a stack
  slot until ebx frees up; ours saves edi in the prologue and keeps amount in edi (ours amount=edi / y=ebx, the
  original amount=ebx / y=edi).
- Readability, unit-wide: gv.h got ORDBLD (packed, the building record of gm_BuildQuery as ord.c reads it:
  level 0x5c, object 0x61, ai.h's AIBLD) and gm_FindById's prototype (ai.h's 4-argument view). ORDCELL (gv.h) is
  the 0x91ae60 grid as ord.c reads it (ai.h's AIGRID plus the +8 average and the +0xc / +0x10 / +0x14 float
  sums - AIGRID itself was NOT touched, see notes/vc6-matching.md).
- 0x4f8460 / 0x4f8590 renames: unk_Unk4f8460 -> ord_AddGridAmount, unk_Unk4f8590 -> ord_SubGridAmount.

- 0x4f8920 ord_RebuildGridSums: **readable**, escalated @deep (far draft: 111 code / 16 fpu hunks, allocation +
  the same nested 8 x 8 clear shape as 0x4f8360; the reason lists the full reading). Read against uv run disasm
  0x4f8920 0x260: the graveyard walk (0x91f700, stride 0x2d, skip +0x25 == 0 / id -1), the 0x2d copy,
  gm_BuildQuery(1, 1, r[0x21]), the AIBLD / ai_BTypes kind guards (0xb..0xd, 0x10), ord_GetGridCell,
  gv_GetTableRecord, w = (short)r[0x22] / rec[2] * rec[1] (fild / fidiv / fimul - the float spelling compiles
  to the same code as the old double one), the +0xc += w * 0.5f (0x5eb7ac = 0.5f) and the 3 x 3 loop
  (+0x10 += w, +0xc += w * 0.5f) whose clamps are the setl / dec / and form. Measured and reverted: the bound
  written `gv_Graveyard + 0x5a00` instead of the end symbol (worse by 36 bytes - the original compares against
  the absolute 0x925100).
- Renames: unk_Tab91f700 -> gv_Graveyard (0x91f700, gv.c's own wording "the graveyard game state / the 0x2d-byte
  records at 0x91f700"; 48 uses in gv.c are byte-neutral), unk_Tab925100 -> gv_GraveyardEnd (0x925100, gv.h's
  "their end"), unk_Unk4f8920 -> ord_RebuildGridSums. ORDCELL's floats got role names: unk0c -> halfWeightSum,
  unk10 -> weightSum, unk14 -> buildingCount.

- 0x4f8ae0 ord_ComputeStatistics: **Q1/Q2 done**, escalated @deep (far draft, 449 code hunks; ROUGH stays -
  the body was not verified hunk by hunk against the disassembly, so the float / double spellings of out[5] /
  out[9] are the next session's first lead; Q4's 14 raw accesses need the typed character / AIBLD / AIBTYPE
  views, which the unverified reading and the gv.h / ai.h conflict block).
- Renames: unk_Unk4f8ae0 -> ord_ComputeStatistics, ai_Unk4f9ec0 -> gm_IncomeRating (folded with f3gm.c's
  definition), unk_Unk4f9d40 -> ord_WeightedRecordSum, unk_Ptr889f74 -> ai_Blds (folded with ai.h's name for
  0x889f74; gv.h's char * view is the same pointer).

- 0x4f8ed0 ord_UpdateStatistics: **Q1/Q2 done**, escalated @deep (far draft, 119 code hunks; ROUGH stays - the
  original fuses the rating smoothing into the FPU stack with a QWORD 0.95 and rep-movs the record before the
  first-value test, and the float / double spellings were not settled; next session's first lead). Readability
  gained: 0x91aaf4 got `ord_RatingRate` (float), 0x6f1ba0 `ai_Rating` (ai.h's name, declared in gv.h), the
  float bits of 0x6f1b9c a second view `ai_RatingBaseF`; the char-pointer arithmetic on ord_StatisticsHistory
  is gone (Q4 23 -> 14, byte diff 4932 -> 4894). Names: ai_Unk4c5800 -> cm_SendMessage41 (cm.c: "message 0x41"),
  unk_Tab91b470 -> ord_StatisticsTime.

- 0x4f9020 **ord_LoadStatistics** (was unk_Unk4f9020): **readable, behaviour still draft** (69 asm / 160 B, from
  126 / 234 at the start). Rewrote the restore: typed ORDSAVE / ORDSTATS / ORDSTIME views, the 14-byte clock
  copy (packed; the unpacked struct copied 16 bytes), the character walk over ORDCHARKIND (stats at +0x80, not
  +0x7e), and the average division. 0x4f9850 -> **ord_ReportStatistics**. Remaining 4 code hunks are allocation
  only: frame 0x18 vs 0x1c (7 vs 8 slots), `push ebx` at entry vs after `fld p[6]`, the walk's base is health +8
  here vs kind +2 in the original, and the epilogue order. Tried and kept: clock struct copy, packed ORDSTIME,
  `q = p + 11` base, ORDCHARKIND, declaration order. Tried and reverted: separate `health` pointer. Next:
  match-deep (slotmap: the original zeroes a slot at 0x8; vargen / push order). **Keep the new types in ord.c**:
  the same declarations in gv.h perturbed ord_CountGridObjects (8 -> 14 asm) and gm_LoadTables (62 -> 65) via the
  full recompile; isolated, both returned to their best.

## match-base 2026-10-27 (solo, main)

- **0x4f9190 ord_InitMessageRecord: byte + readable.** The 0x24-byte copy of the record is the engine's generic
  unaligned block copy with the source named directly (`lea ecx,[edi+edi*2]; and ecx,3 ..`), i.e. the D3_COPY
  shape; d3.h needs `<d3d.h>` (DDPIXELFORMAT) so ord.c gets a unit-local `ORD_COPY` macro instead of the include.
  The tag store `'init'` goes first (VC6's schedule). Renames: ai_Unk4c5e80 -> cm_SendMessage56 (cm.c's message
  0x56), unk_Unk4f9190 -> ord_InitMessageRecord. Typed view ORDMSGREC / ORDINITMSG (record + tag at +0x24), so Q4
  is clean; `ord_MessageRecord` is now `[0x24]` in gv.h (only ord.c uses it; the clang asm-operand lint needs the
  complete type).
- **0x4f9220 ord_UpdateMessageEvent -> match-deep** (was the ROUGH layout draft, 535 asm). Meaning read against the
  disassembly and fixed: `gm_AddTime` is the four-argument call `(stamp, Random(4) hours - 7 more outside
  7 .. 0x16, Random(2) * 0x1e minutes, 0)` (the draft's `t` with a 2-argument cast was wrong; the original pushes
  secs 0, mins, hours and cleans 0x10), the stage 2 / 3 table loops are two constant loops (the `?:` folded to
  `sete` arithmetic), `r[8] = r[7]` after a phase message, and the tail is `ORD_COPY3` (both operands through
  temps; forces the ebp frame). Left: the phase-3 block order and the merged branch tails are still `code` hunks
  in diffclass, the register/slot allocation (constant 0 in edi vs esi, `r` at ebp-0x24 vs ebp-0x64) and Q4 (the
  0xf8 long-message view, the ai_PlayerChar+4 view). The local best reference held the wrong 2-argument spelling
  (364 asm / 898 B); `uv run ledger grade accept` reset it to the correct source's current measurement (433 asm /
  1032 B), so later sessions no longer read the wrong draft as the target (the first correct measurement was
  373 asm / 989 B). Keep this source.
- **0x4f9710 ord_HandleMessageRecord: byte + readable.** The record copy is the all-direct block copy shape
  (`ORD_COPY_AT(offset ord_MessageRecord, m, 0x24)`), the tag switch compares unsigned, and only `break` +
  `default: return 0;` + a shared `return 1` gives the original's immediate stores and duplicated `mov eax,1`
  tails (a `return 1` per case CSEs the constant into eax).
- **0x4f97c0 ord_BuildingCapacityRatio: byte + readable.** The last test is `if (b != 0) return (float)a / b;
  return 0.0f;` (the `b == 0` first spelling emits the inverted jump and swaps the two FPU returns).
- Header hazard this session: adding `extern unsigned short ai_TimeHour;` to gv.h flipped **d2_DrawObjects**
  (93 -> 97 asm); the extern moved into ord.c (as am.h / cm.h / amt.h declare it) and d2_DrawObjects returned to
  its best. gv.h's `ord_MessageRecord[0x24]` did not move anything else.

### match-base 2026-10-27 (solo, main, second session)

- **0x4f9850 ord_ReportStatistics -> match-deep** (271 asm / 605 B -> 170 asm / 578 B, readable). Meaning read
  hunk by hunk against `tools/disasm.py 0x4f9850` and fixed: the gate is the *year* (`ai_TimeYear >= 8 &&
  ai_TimeYear % 4 == 0`, the original's `and eax,0x80000003 / jns / dec / or / inc` is a signed `% 4`, not `& 3`);
  the four averages are the last four history rounds 12 .. 15 (a = `noSecond`, b = `noFirst`, c = `combined`,
  d = `score`); the phrase tests are `<= 0.15` (the original's `test ah,0x41; jnp` = less-or-equal, so 0x1c11 ..
  0x1c14 are the *low* / present texts and our draft had the polarity inverted), and the income rating test is
  `v >= 0.0` (a QWORD zero - `v >= 0` with the int literal compiles to a DWORD 0.0f compare and a different
  code shape); the three min tests are `<=` (a tie takes the later column: 0 .. 3, not 0) and the tier reads are
  `ids[k]`, `ids[k + 2]`, `ids[k + 4]` (the original really indexes two / four entries past the 4-int table, i.e.
  into the first buffer - kept, it is what the bytes do); d has two separate `if`s (0x1c0d / 0x1c0f) each with
  its own copy of the news loop, and the three loops are `for (i = 0; i < 768; i++)` over `ORD_CHARS[i]` (the
  known trip count is why the original has no pre-loop test). Typed ORDSTATROUND (the history as 16 rounds) and
  `ORDCHAR.kind` / `.netId` clear Q4, `ai_TimeYear` clears Q3, the description was rewritten (Q5/Q6). Left: the
  original keeps the income rating in st0 (`fcom` without pop, never stored) and the running minimum m in
  [esp+0x10] (`and eax,0x4100; jne` + the reverse compare per `<=` test), ours is the other way round (m in st0,
  rating homed with `fst [esp+0x20]`); and the original's two 0x200 buffers are tier text at [esp+0x38] /
  sentence at [esp+0x238], ours the reverse under every declaration order and name pair tried. Next: slotmap /
  c2prio on m and the rating, then the buffer order.
- **0x4f9c00 ord_AdvanceRatingBase: byte + readable** (was `unk_Unk4f9c00`, cm.c called it `ai_Unk4f9c00`; both
  renamed, cm.h now declares it `void ord_AdvanceRatingBase(void)`). Two meaning fixes closed all 15 asm: the
  clamp is a *lower* clamp (`if (ai_RatingBaseF < lim) ai_RatingBaseF = lim;` - the original leaves lim in st0
  after `fcomp st(1)` and `fstp`s it, so the draft's `>` was inverted), and the second stamp copy is the whole
  int at +4 (`mov [0x91b474],ecx` with ecx from the hour test), not a zero-extended short. Typed ORDSTIME (fields
  renamed year / hourWord / minuteWord / stamp) and a second view `ord_GameClock` at 0x770ae0 clear Q4 with no
  byte change: `(unsigned short)hourWord` keeps the original's 32-bit load + 16-bit compare. Q7: the call line
  itself carries `@original` (gv.h's shared K&R declaration cannot be replaced - he.c / sv.c / gv.c pass the
  clock as an int address literal, and a prototype in ord.c is a lint cross-TU conflict; a comment above the line
  does *not* exempt it, only the same line does).
- **0x4f9d40 ord_WeightedRecordSum -> match-deep** (40 asm / 172 B -> 19 asm / 45 B). The weight table is one
  aggregate initializer (`float w[12] = {ten weights}`): the original's two zero stores go through eax because
  they are the initializer's implicit tail, and `sum` must be declared / initialized *before* the table so
  `sum = 0` stays an immediate `mov [esp+0xc],0` instead of being folded into that zero register. Left: one hunk,
  the loop's register choice (ours i in esi and &w in edi, the original i in edi and &w in esi) plus the
  `fild` / `fmul [esi]` / `add esi,4` / `inc edi` order.
