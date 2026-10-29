# unk_4b6480.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the findings of each session, one section per author.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (-): random events / news messages for a character (money, skills, buildings, jobs, goods)
- Boundaries: separated (plausible): one subsystem, all drafted (layout 2026-10-22 session 7): 69 message callbacks (text 0x12e7 + type, sent with 0x53dda0 / 0x53dee0), their table 0x602570 (0x45 entries, .data right after unk_4b39f0.c's "$C"), the dispatcher 0x4bb6c0, 0x4bb720 (posts random message types as AIMSG 0x42) and the LCG 0x4bb7d0 (seed 0x6f0730). Start: the table's first entry, after the missions' save / load. End: 0x4bb810 is the first user of "anm:animals" (anm.c). No module tag; could still be the missions' tail (no evidence either way)
- Range then: 0x4b6480..0x4bb810

## Open work
- Near-misses: 0x4b82c0 35 B (text-argument scheduling), 0x4b8c80 4 B (the two pointer arrays js / os swap stack
  slots; declaration and store order tried), 0x4b9f30 41 B (text id computed into a local `t`, scheduling),
  0x4bb250 15 B (frame layout of t / x / buf).
- Drafts far off: 0x4b6660 (record loop), 0x4b84b0 / 0x4b9cd0 / 0x4ba0c0 (c kept in a stack slot in the original),
  0x4ba630 (direction loop; `p = c` fallback and the zero store into c's slot unexplained), 0x4ba840 (returns 0
  through one shared epilogue: try the nested-if form that matched 0x4ba970), 0x4bacc0 (register roles).
- Type debt: see notes/layout.md (session 7).
- Boundary: 0x4bb720 / 0x4bb7d0 may belong to anm.c (seed 0x6f0730 before anm's .data); see notes/layout.md.

## match-base 2026-10-26 (base-8)
- 0x4b6660 -> `ai_MsgOfficeApplicantPayment` **byte + readable**. Two meaning errors the layout draft had:
  the character scan compares **ai_Chars[j].appliedOffice (0x168)**, not the office held (0x166), and the
  text's office argument is **r[i].kind + 0x253** (record +8), not the +0x14 field (the push count confirms
  it: three pushes are live at the load, [esp+edx*8+0x34] = r[i]+8). The record is amt.h's AMTREC; the unit
  cannot include amt.h (it collides with am.h: ai_Chars / ai_BTypes / ai_Time), so unk_4b6480.h declares its
  0x18-byte view `G14REC {kind, cnt, state, x14}` and `amt_KindColumn` / `amt_GetColumnOffices` with
  `struct AMTREC *` (the amt.h heads of those functions), like unk_4a7140.h does.
- Block layout: the original's shared 0x400 exit sits **before** the found body, reached by the guards'
  `je` and by the loop's fall-through, with the body at 4b6753 only from the two `p != 0` jumps. The nested
  `if (guards) { ... } return 0x400;` form and `if (p == 0) return 0x400;` both put the exit block last
  (test + je after the loop, one extra block). The `goto none` / `goto found` form reproduces the layout
  exactly; `for (i = 0, p = 0; ...)` gives the original's `xor ebp,ebp` / `xor edi,edi` order.
- Renames (project-wide): AICHAR.unk166 -> officeKind, AICHAR.pad168 -> appliedOffice, AICHAR.unk167 ->
  prevOffice, AICHAR.unk169 -> secondOffice (ai.h, the amt.h AMTCHARVIEW names), used by ai.c, hmt.c, hst.c
  too. Only the AICHAR-typed sites were renamed: AIPER / AIC4 / AGCH in am.h keep their own unk167 / unk169
  fields (am.c's uses), so those units' code was untouched.
- Still blocking the Q1-only callbacks: `ai_Unk5ba6b0` (0x5ba6b0, the inventory item / class test, declared in
  f3gm.c as `void *ch, int id`) has no evidenced name yet, so 0x4b6fd0 / 0x4b7430 stay Q2-blocked.
- Byte already, Q1-only, named by the role the Q6 comment records (one name per message index until the
  texts 0x12e7 + type are resolved): 0x4b6480 `ai_MsgRewardPct1to3` (message 0), 0x4b6570
  `ai_MsgRewardPct2to4` (1), 0x4b6830 `ai_MsgPayPct1to3` (3), 0x4b6920 `ai_MsgPayPct1to4` (4), 0x4b6a20
  `ai_MsgRewardPct2to5` (5), 0x4b6b10 `ai_MsgRewardPct1to3Type68` (6 and 8 share it), 0x4b6bf0
  `ai_MsgPayForBuilding` (7, Q4 fixed: the building id is AIBLD.id), 0x4b6d30 `ai_MsgPayPct2to4` (9), 0x4b6e10
  `ai_MsgPayPct125` (10), 0x4b6ef0 `ai_MsgPayPctOver120` (11), 0x4b70f0 `ai_MsgNoOp` (the table's empty
  callback). "reward" / "pay" is the Q6 comments' direction, not yet verified against the message-0x10
  handler: every callback up to 0x4b6ef0 passes (m->id, -1) to cm_SendMoveGoodsUnchecked, so the pair order
  alone does not settle it.
- 0x4b82c0 (next in the queue): single code hunk at instruction 105 is the text-argument load order; the
  menu's add-lea lead is the way in (the original does `add ecx,0x20d; add ecx,edx`, ours one `lea`), not a
  meaning error (the statements match).

## layout 2026-10-22 (session 7)
- G14MSG {int id; unsigned short type} is the callbacks' argument; the dispatcher 0x4bb6c0 (from day 8, not for
  unk1b1) calls ai_Unk602570[type] (0x45 entries; 0x4b9810 `return 0x400` fills unused slots, 0x52a040 lives
  elsewhere). Callbacks return 1 (no character), 0x400 (not applicable) or 0 (sent).
- Template (most callbacks): `k = util_Random(r) + base; v = (int)(ai_Unk50a640(c->id) * k * 0.01);` text
  0x12e7 + type via 0x5176d0, money via 0x4c4830 ((m->id, -1) in the first ones, (-1, m->id) from 0x4b6fd0 on),
  sent with 0x53dda0(c->unk004, c->unk004, buf, flag, 0x603, 0). Two constants differ: 0x4b6e10 * 0.0125,
  0x4b6ef0 * 1/120 (0.008333333333333333); the link gate (uv run link) catches such constants.
- The 0x53dda0 flag is `strlen(buf) > 0x100` in all but the first callback, where it is written
  `strlen(buf) > 0x100 ? 0x100 < strlen(buf) : 0` (a `jbe` around the compare).
- A random count used as an argument after other arguments were loaded (`push 1` after the util_Random call) is a
  local assigned first (`n = util_Random(4) * 2 + 4;`, 0x4b78c0 family); a negative one is `-n` at the call.
- An early `return 0` among `return 0x400` guards (0x4b7200) puts all callee-saved pushes at the entry.
- Shared `0x400` epilogue reached by `jne` (0x4ba970 / 0x4baa80 / 0x4baba0): one nested `if (a && b && ..)` with
  `return 0` inside and `return 0x400` after it.
- 0x4bb6c0: `if (type < 0x45) return table[type](m); return -2;` (the other order puts -2 first).
- 0x4bb720: AIMSG field stores before `msg.time = ai_Time` give the original scheduling.

## match-base 2026-10-26 (base worker, 2nd)

Check 2593 -> 2595/4664; unit 56/66 -> 58/66 matched, 1893 -> 1824 differing bytes. No readability change.
- The text-argument near-miss (Open work: 0x4b82c0 35 B) is **byte**: the 4th txt_Format argument is the
  ternary of the two sums, `(((char *)p)[9] ? 0x230 : 0x20d) + (unsigned char)p->title`, not the flat
  `(cond ? 0x23 : 0) + 0x20d + title`. The flat form is one `lea eax,[ecx+edx*1+0x20d]`; the ternary form
  gives the original's `and ecx,0x23; add ecx,0x20d; add ecx,edx`. A local (`hdr = cond + 0x20d; hdr + title`)
  does not help (still one lea) - the ternary is the lead. Applied to the same idiom in 0x4b84b0 and 0x4b9cd0.
- ai_Unk4b9f30 (0x4b9f30, 41 B) -> **byte** with the same spelling (it keeps the local `t`).
- ai_Unk4b84b0 (0x4b84b0): 54 -> 11 differing with the spelling, then -> match-deep. The one hunk left is the
  `while (util_Random(k) == s)` test: the original pushes k before loading `p = a[s]` and merges the previous
  argument's cleanup into `add esp,0x8`; ours reloads/schedules the other way. No menu rule (`uv run spell`: "no
  menu rule; meaning check first"), no meaning error read - argument-push scheduling.
- ai_Unk4b9cd0 (0x4b9cd0): `char buf[0x1f0]` -> `char buf[0x200]` (the sibling's size): frame 0x310 -> 0x320,
  56 -> 46 differing. Left: the FPU flag test of `util_Random(0x32) <= f` (ours `test ah,0x41; jnp`, original
  `and eax,0x4100; jne`; the two only differ for NaN - the menu's branch lead "swap the if/else branches";
  `f >= util_Random(0x32)` tried, unchanged). The menu's branch lead "swap the if/else branches" works: the
  success block inside `if (util_Random(0x32) > f) { ...; return 0; }` + `return 0x400;` removes the FPU flag
  hunk (46 -> 42). Left: the AIMSG store order (the original hoists `mov edx,[ebp+1]` above the memset and
  stores type/flags after the push) - scheduling.
- ai_Unk4ba840 (0x4ba840) -> **byte**: the Open work's lead holds - the three guards written as one nested
  `if (n != 0) { ... if (o != 0) { ... if (k != 0) { ...; return 1; } } } return 0;` give the original's one
  shared zero epilogue (the early `return 0`s did not; same form as 0x4ba970).
- Rename (evidence: the function's own comment - the chosen job is stamped with the time and told to
  0x4c4c60): 0x4b8c80 ai_Unk4b8c80 -> ai_MsgJobTime (Q1 cleared; Q2 G14_5087F0 / G8_4FEA50 /
  ai_Unk4c5010 / ai_Unk5ba6b0 and the 17 Q4 casts remain).

## match-base 2026-10-26 (base worker, main checkout, second)
Check 2596 -> 2599/4664; the unit 59/66 -> 61/66 matched, 1414 -> 750 differing bytes. Base level 53 -> 48.
- ai_MsgWorkerStop (0x4b9cd0, was ai_Unk4b9cd0) **readable**, -> match-deep: Q4 by b->type / b->id (AIBLD),
  Q1/name from the receive side (0x4c9c10 runs 0x405610 "stops the current action and drops the queued ones" on
  "sp_<p>", so the message stops the worker). Behaviour stays draft: the whole body matches except the AIMSG
  fill's load/store schedule after he_ShowNews; the fromId-local hoist matched one instruction but grew the
  bytes (1414 -> 1447, reverted), `uv run spell`'s width lead does not apply (both sides read `mov ax` and store the
  WORD at time+0xc). Reason in its @deep.
- ai_InventoryHas (0x5ba6b0, was ai_Unk5ba6b0, **byte**): the ROUGH stub's body written from the disassembly -
  gm_Query (inventory +0x178, 1, 0, id), the object must have a 0x62c420 record (0x5ba620) and its +0x21 set;
  the missing `1, 0` of the gm_Query call was the meaning error. Name: the callers' own comments call the ids
  skills (cm.c "the skill 0x16f" / 0x2b5) and the object is found in the character's inventory. unk_58bbd0.h
  gained the G8INVCHAR / G8INVOBJ views. This cleared the Q2 blocker of 0x4b6fd0 / 0x4b7430 too.
- ai_Unk4ba0c0 **behaviour**, -> match-deep (Q1 + Q4): the frame was 0x128 vs 0x124 because `r > gm_Liking(..)`
  spills `r` to a float temp across the call - the original's `gm_Liking(..) < r` loads r after the call
  (fild / fcompp, no slot) and every offset then lines up; the three 0x400 guards share one exit via
  `goto none` + one `return 0x400` at the end (the nested-if form cannot reach the in-loop guards);
  `msg.n = msg.fromId` gives the single b->id load. Left: 10 order hunks (the b->id load / type / flags stores).
- ai_Unk4ba630 -> match-deep: Q4 (b->type, b->id) and Q2 (he_MessageBoxQuickjump) done; the 112 code hunks are
  allocation - the original keeps k in ebp and uses the zeroed ebp for its NULL compares, ours keeps k in a
  slot; `p = 0` (the sibling idiom) compiles identically, `uv run spell`'s reload leads are the original's own spills.
- ai_MsgLoseGood (0x4bacc0, was ai_Unk4bacc0) **byte + readable**: the eligibility test is the good's `type`
  (byte 0), not byte 9 - the disassembly compares [base + id * 0x41] with 9, ours had +9. The name: the 0x11
  handler (cm.c 0x4c70b0) moves the item from data[4] (c) to data[0] (-1), i.e. c loses the good; the text id
  0x9a8 + 2 * good names it.
- ai_Unk4bb250 **byte**, -> match-deep (Q1/Q2/Q4): the local was `int x` but 0x4fe2c0 writes a 10-byte calendar
  record (byte 0 day, byte 1 month, word +2 year, byte 4 hour, byte 5 minute, int +6 seconds) - the frame was
  8 bytes short and the call wrote past it. `unsigned char d[10]` (f3gm.c's d[10] convention) makes the frame
  0x11c and the body identical. Its calls are f3gm.c's gm_ClockToDate / gm_Season under the old ai_Unk names
  (declared locally in 5 TUs; the rename needs the canonical prototypes in f3gm.h - a shared-header decision).
- Renames (project-wide, every use updated): ai_Unk5ba6b0 -> ai_InventoryHas (0x5ba6b0), ai_Unk53dee0 /
  he_Unk53dee0 -> he_MessageBoxQuickjump (0x53dee0, the function's own error string names it), and
  ai_Unk4b9cd0 -> ai_MsgWorkerStop, ai_Unk4bacc0 -> ai_MsgLoseGood.
- AIMSG.dst[0]'s 16-bit text id (0x64) still needs the cast: the textId union arm tried in ai.h regressed
  ai_Unk483a60 (byte -> 5 differing, its slots) exactly as ai.c's note records for a 0x60 arm, so it was
  reverted - match-deep's shared-header decision.
