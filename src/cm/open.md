# cm.c (was unk_4c3180.c; cut.c joined in session 14, its notes are in cut.md)

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the findings of each session, one section per author.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): tag "cm_ExecCommand" (0x4c4190) and "cm_RequestSendCutInfo", "cm_RequestChrMoveToUniverse", "cm_RequestCreateGeb", "cm_ExSellObjekt", "cm_ExSetHE", "cm_LoadBuf"; the game commands sent to / received from the server (net.c): message sizes, send ring, receive list, split long messages, field blocks, ~100 senders, the group checks (0x60bf40) and the handlers (0x60bd98); cut_ half (scene kinds: session / trial 0x4db700, duel, execution, wedding, birth, death, bankruptcy, auction 0x4e1a80, sale 0x4e2860; the party records 0x925520 (0x4e4f40 .. 0x4e5210), the scene random numbers (0x4e52a0 .. 0x4e5360), the debug dumps 0x4e58e0 .. 0x4e5bf0 and the registry 0x4e5770); matched: 0x4d8c70, 0x4d8cb0, 0x4da9e0, 0x4de530, 0x4de5e0, 0x4de610, 0x4dff80, 0x4dfff0, 0x4e0010, 0x4e06d0 and the helpers of session 14 (see git)
- Boundaries: separated (plausible): every function of the range has source (reordered into address order, byte counts unchanged, layout 2026-10-22 session 15) (layout 2026-10-22 session 14: cut.c joined into cm.c, byte counts unchanged; rough drafts: 0x4db700 (the trial scene, ONE function up to 0x4dd95d, Ghidra's 0x4dc7c0 lies inside it, 4 KB read as 3 KB), 0x4e1a80, 0x4e3830, 0x4e4060, 0x4e4cc0, 0x4e5470), One unit because: the strings run monotonic from 0x60cb58 through 0x60ec88, the ints 0x60d360.. serve both halves, the scene registry 0x4e5770 lists the fight as scene kind 3 next to the cutscenes, 0x4d8b30 / 0x4d8c40 are called only from the cut_ tail, and 0x4debe0 went from 182 to 74 B off with cm.c above it. End 0x4e5bf0: the next unit (amt) starts with 0x4e5cd0, its data at 0x60ec90. Start: .data block 0x60bd80 right after f3snd.c's literals. Earlier notes: open: end 0x4d8c70 plausible (layout 2026-10-22 session 13: 0x4d76a0 fight commands, 0x4d88c0 the fight loop, 0x4d8b30 / 0x4d8b50 / 0x4d8c40 drafted; all fight code is drafted up to 0x4d8c70, where cut.c starts: its callers and strings differ, but the initialized ints 0x60d360.. are used on both sides, see layout.md); earlier: (session 12 drafted on to 0x4d7360: the fight end / ransom / loot dialogs, the fight UI, the fighter slot actions; session 11 drafted on to 0x4d3b30: fight scene code "Kampfszenario_*", fight AI, event callbacks; strings of the fight code share the block 0x60cbc0..0x60d20c; session 10: drafted 0x4c3180..0x4cf670, all 0x6a handlers of the table 0x60bd98 and the chat / player-list / cutscene / bomb / floating-number code behind them (0x4cd350..0x4cf370); the .data block continues (strings up to 0x60cbd4+), the code goes on at 0x4cf670 (drafts needed up to the next unit). Start: its .data block starts at 0x60bd80 (0x302, 2 = ai_Unk60bd84, -1 globals 0x60bd88..0x60bd94, the handler table 0x60bd98 (0x6a entries), the check table 0x60bf40 (0x6a), then the cm_ texts 0x60c0e8..) right after f3snd.c's literals (ends 0x60bd7b)
- Range then: 0x4c3180..0x4e5bf0
- Was: unk_4c3180.c (layout 2026-10-22); cut.c, sitz.c, unk_4de4b0.c, unk_4e01e0.c (joined)

## Open work
- cut.c is joined (session 14): the open work of 0x4d8c70..0x4e5bf0 is in notes/units/cut.md; first job: put that half into address order (notes/layout.md).
- The fight code is drafted up to 0x4d8c70 (cut.c starts there, see notes/tus.md). Session 13 drafts: 0x4d76a0 (rough, 4368 vs 4344 B: the 8-slot command loop, cases 1 .. 8 follow Ghidra's switch; the order of the blocks was not tuned), 0x4d88c0 (matched), 0x4d8b30 (matched), 0x4d8b10 (12 B: the original calls 0x4d6930, the draft tail-jumps to both), 0x4d8b50 (the original loads d3_Time into ecx first and 0x6f0da4 after the first multiply; all three expressions are written out), 0x4d8c40 (matched). Rough drafts of session 11 (control flow from the disassembly, args of
  the 0x45f170 / 0x4d2170 out parameters live in parameter slots in the original): 0x4cf670, 0x4cffd0, 0x4d0460, 0x4d0740, 0x4d0fa0 (fight
  setup), 0x4d1c20, 0x4d2c00, 0x4d2f80, 0x4d3390, and 0x4d3550 (its hit / damage locals are never set: the draft folds to 176 of 1072 B).
- Session 12 drafts, matching debt: 0x4d4090 (1 B: [edi + eax] vs [eax + edi] for the id load, index forms tried), 0x4d4050 (one
  lea / mov pair: the original builds the pointer first, locals / casts did not change it), 0x4d6970 (8 B: `rep movs` of the 0x2c
  copy sits before the two loads in the original; memcpy / struct assignment / orders tried), 0x4d3b30 (340 / 480 B, the winner is kept
  in a register `ret` beside the stack `w`), 0x4d6ba0 (k / tgt registers swapped: esi = k, edi = tgt in the original; declaration
  orders and the 0x1d test order tried), 0x4d7190 (frame 4 B small: a `char k` slot at +0xc; the `or byte [x+0x8d],4` form is
  not produced), 0x4d7360 (the original loads g into esi first, one final `return -1`; frame 0x34), 0x4d4540 (frame 0x8b0 vs
  0x8cc, `win` lives on the stack in the original), 0x4d4d70 (5248 vs 4928 B, rough), 0x4d60b0 (736 vs 800 B), 0x4d63e0 (rough).
  Matched this session: 0x4d3ea0, 0x4d3d10, 0x4d4340, 0x4d6930, 0x4d6a50, 0x4d6b70, 0x4d6f60, 0x4d6f90, 0x4d7000, 0x4d70a0, 0x4d7150.
- .data not defined in C: 0x60bd80 (0x302), ai_Unk60bd84 (2), 0x60bd88..0x60bd94 (-1), the tables 0x60bd98 /
  0x60bf40 (0x52a040 is the default check = `return 0`), the literals.
- Near-misses: 0x4c81d0 (3 B, ecx / edx for the field offset; `+=` and the written-out sum give the same), 0x4c4d40
  (12 B, the G17T14 copy order), 0x4c59f0 (15 B), 0x4c63e0 (17 / 464 B), 0x4c5c40 / 0x4c5ca0 (18 B), 0x4c62a0 (23 B),
  0x4c50b0 (29 B, after Ghidra's and offset store order), 0x4c6ec0 (31 B), 0x4c4030 (32 B).
- Rough drafts (the control flow is from the disassembly, not checked): 0x4c70b0 cm_ExSellObjekt (2160 / 2064 B),
  0x4c78c0, 0x4c8240, 0x4c85c0, 0x4c6870, 0x4c4190 cm_ExecCommand, 0x4c3180 (switch: the case order is not the
  original's), 0x4c9dc0 (type 0x35, 2704 B: the "cm_ExChrGotoBuilding" walk, many string copies), 0x4cac40 (0x3a, float
  parts), 0x4cbca0 (0x4c "cm_ExCreateGeb"), 0x4cb4a0, 0x4ce300 / 0x4ce5d0 / 0x4ce830 (bombs / grenades, effect calls with
  float arguments), 0x4cee20 / 0x4cf000 (float math, the byte tables), 0x4cec30 (window calls).
- Near-misses (session 10): 0x4cd010 (1 B), 0x4cf370 (2 B: the init order of the locals e / up / a / b: the original has
  xor esi, ebp, edi; the declaration orders tried give 2 or 4 B), 0x4ccb10 (13 B), 0x4ccbd0 (12 B), 0x4cdf00 (21 B), 0x4cb910 (9 B,
  sete / store order), 0x4cbbd0 (21 B), 0x4cb4a0, 0x4c9830 (35 B: the i = 0 branch layout), 0x4c9450 (the three calls of
  ai_Unk5000c0 are tail-merged in the original), 0x4c9280 (the `ret = 1` block sits out of line in the original).
- Type debt: cm.h's local views of ai / am / rt callees (notes/layout.md, session 9).

## layout 2026-10-22 (session 9)
- G17MSG (0x99 bytes, packed): type, size (0x4c3180 by type), unk03, idx (send-ring slot), seq, next7 (seq of the
  next part of a split message), data[0x81], prev / next (0x91 / 0x95). Its head is net.h's NETPKT; net_Conn.rx is
  ai_Unkf81180, net_Conn.tx a ring slot.
- Send: ai_Unk4c39c0 copies the message into the ring ai_Unka69140[0x8000] (slot = seq & 0x7fff), state
  ai_Unkf31160[slot] (G17STAT: state 0 sent / 1 done / 2 failed, unk01 kind of value, value), appends it to
  ai_Unkf81160; a pending long message (ai_Unka68800, set by 0x4c4480, max 0x3fe bytes) goes after it as parts of
  type 7 (0x4c3720). 0x4c3680 sends (offline: straight into the receive list), 0x4c3fd0 receives.
- Receive: ai_Unka68d24 list of 0x2000 records ai_Unk9367e0 (free list ai_Unka68c00); 0x4c4190 "cm_ExecCommand" runs
  them: groups (type 5 .. 6) are checked first by the check table 0x60bf40 (0x4c43c0), split messages joined into
  ai_Unka68d40 (0x4c38e0), then the handler table 0x60bd98 (m, state). ai_Unka68d1c = "cm_LastRequCount",
  net_Unka687e0 = "cm_LastSyncCount".
- Field blocks (G17FIELDS 0x7c bytes): ai_Unka68c20 (size, count, offset, values; deltas 0x4c3c00 / values 0x4c3d00 /
  0x4c3d90, against the object ai_Unka68d2c) sent as 0x16 / 0x17; ai_Unka68ca0 (k, value) pairs sent as 0x18.
- ids -2 / -3 / -4 in a message are the last created building / character / object (ai_Unk60bd8c / 60bd88 / 60bd90,
  resolved by 0x4c4450). ai_Unk4fea50(&x, &y, &z, id) finds an object as one of three views (x: +0x5d list, +0x27
  owner; y: +0x14 list, kind at 0; z: a character, +0x178 list).
- Handlers seen: 8 / 9 load buffer ("cm_LoadBuf"), 10 building, 0xb / 0xc character (0x507690), 0xd join, 0xe
  remove, 0xf / 0x10 move player goods, 0x11 trade ("cm_ExSellObjekt"), 0x12 production (good + 0x26 amounts, + 0x2e
  ingredient ids, + 0x36 output), 0x13 move object between lists, 0x16 / 0x17 apply field block, 0x18 relation
  floats (0..1000), 0x19 mask / set, 0x1a add float, 0x1b relations 0x7e5360 / 0x889f80 (0x300 x 0x300), 0x1c job /
  message box (AIMSG + texts), 0x1d "cm_ExSetHE".
- Matched on the first draft or after one quick fix: all simple senders, 0x4c3480-style inits not (90 B), the
  checks 0x4c6350 / 0x4c6750 / 0x4c6780 / 0x4c67a0 / 0x4c67c0, the handlers 0x4c6a20..0x4c6ad0, 0x4c6b20, 0x4c6f60,
  0x4c7010, 0x4c7d80, 0x4c7e20, 0x4c8110, 0x4c87a0.

## layout 2026-10-22 (session 10)
- Handlers 0x1e..0x69 of the table 0x60bd98 and their helpers are drafted (names ai_Unk<addr>, the type is in the doc comment).
  Many handlers share the shape `if (s) { s->state = 2; s->unk01 = 0; s->value = 0; } .. if (s) s->state = 1; return 0;`
  (state 2 = failed first, 1 = done, unk01 the kind of the value: 1 character, 3 object, 5 / 6 / 7 / 8 / 9 records); return 1 / 2 / 3 /
  -1 .. -3 are the error codes of the lookups. 0x20 is a "system message" with 0x15 sub-kinds (data[0]); 0x22 "cm_ExAddStraftat"
  (crime records 0x2d bytes at ai_Unk91f700); 0x26..0x28 cutscene records (ai_Unk92cae0[16][0x35c], 0x4e5150); 0x2d..0x34
  and 0x35..0x39 act on the person "sp_<id>" (ch_FindByName; actions 0x2d walk / 0x33 gate / 0x38 mark / 0x39 sit down / 0x3a);
  0x35 is the big "cm_ExChrGotoBuilding" (texts "dummy_gate_ob_%s", "dummy_TUER", "dummy_EINGANG", "dummy_TRANSPORTER"); 0x3a
  "cm_ExGebUpgrade" (model "<dir>gebaeude/*<NAME>[_A..].ogr" tried with level suffixes); 0x4c "cm_ExCreateGeb"; 0x4b chat text
  (appended to ai_Unk75fba0, "$A" separators, cleared after 5 lines); 0x53 / 0x54 four-letter sub-commands ('recl' 0x636c6572,
  'buy ', 'join', 'vael') on the job records 0x12c / 0x12d of a building; 0x55 sub-kinds 1..7 call 0x4d6ba0 .. 0x4d7190;
  0x5f is a jump to 0x4cd870 (player-list message, kinds 0..6; senders 0x4cd610..0x4cd820); 0x64 six four-letter commands; 0x65 .. 0x69.
- Behind the handlers: 0x4cd350..0x4cd600 the player list (8 records of 0x57 bytes at 0x9364e0, ring of 15 chat lines G18LOG at
  0x935da0), 0x4cd9d0..0x4cdb20 the record kind of a character (10 records of 0x5c bytes at 0x60c7c0; list +0x178, kind 5),
  0x4cdb80 hit chance (flag 0x20000000), 0x4cdcf0 factor by record kind, 0x4cde20 model name "ob_<NAME>", 0x4cdf00 / 0x4cdf80
  cutscene records per character, 0x4cdfe0 / 0x4ce040 map cell test / nearest free cell, 0x4ce130 the 32 character-like records
  at 0x930600 (0x21c bytes, id +4), 0x4ce300 / 0x4ce5d0 / 0x4ce830 bombs (0x935c00, 8 bytes) and grenades (0x935a00, 0x10 bytes),
  0x4cedc0 / 0x4cee20 / 0x4cef80 / 0x4cefa0 floating numbers (16 slots G18POP at 0x9304c0), 0x4cf000 per-tick update,
  0x4cf370 scroll arrows. 0x4cc720 (a helper between the handlers) is called by 0x55.
- Jump-table lesson: VC6 lays a switch's blocks out in source order; when the case labels of the original are permuted
  (0x4cc7b0: table 83c, 7f3, 87d, 84e, 860, 81b, 872) the blocks of equal shape hide it from the byte check but `uv run link` shows
  "local reloc .. expected": write the cases in memory order of the blocks (2, 6, 1, 4, 5, 7, 3 there).
- A function consisting of `jmp X` (0x4cce00, 0x4cd5e0) is `return f(a, b);` / `return f(id);` (sibling call), it matched.

## layout 2026-10-22 (session 11)
- 0x4cf670 mouse pick (cursor look by the selected characters, "sp_ESCAPE" / "sp_CONQUER" / "WARE" objects), 0x4cf9c0 / 0x4cfc10 / 0x4cfe70
  the six portrait boxes ai_Unk930400[0x20] (+0 picture, +4 / +8 window objects, +0x10 / +0x18 texts, +0x14 job picture, +0x1c the character), 0x4cffd0 ..
  0x4d0570 the order records (0x2c bytes copied from 0x4cdf00, L[1] = order kind 1 ground / 2 object / 3 escape flag / 4 name / 6 ware / 7 / 8, L[4..6]
  target / x / z, sent with 0x4c5c40), 0x4d0740 the per-tick command handling, 0x4d08c0 / 0x4d0920 the 32 fight records ai_Unk930600[0x21c] (+0 id
  0xffff = free, +0x184 object, +0x188 mark), 0x4d0a30 / 0x4d0a80 / 0x4d0ca0 / 0x4d0f80 scene walks for the flag dummies ("dummy_sp_ESCAPE" ->
  "ub_FLUCHTFLAGGE", "dummy_CONQUER" -> "ub_CONQUER").
- 0x4d0fa0 sets up a fight (scenario "Kampfszenario_<place>.ed3" by flags / building kind, sound sets by the side's type 0 mercenaries / 1 robbers
  / 2 thieves, characters at "dummy_OFFENSE_ENTRY" / "dummy_DEFENSE_ENTRY", carts "ob_KARREN_*_KAMPF.ogr" with "WARE_0n", border cells = 0xd),
  0x4d1a10 ends it, 0x4d1e00 / 0x4d1eb0 intro / outro cutscenes with a random music track. Fight AI: 0x4d2010 / 0x4d2170 / 0x4d22d0 ring search of ground
  cells with a force balance, 0x4d27a0 builds the float table ai_Unk9300a0[6][5][6], 0x4d2c00 picks its row, 0x4d2f80 / 0x4d2e30 give formation types
  (+0x1c4) 0..5 by a random roll, 0x4d3120 turns a type into an order (0x4d0200 / 0x4d02d0 / 0x4d0380 / 0x4d0460 / 0x4d0520).
- Matched this session: 0x4cfe70, 0x4d02d0 (v[4] = a 16-byte vector local), 0x4d0520, 0x4d0a80 (v[4]), 0x4d0ca0, 0x4d0cd0, 0x4d0f80, 0x4d1e00, 0x4d1eb0,
  0x4d2450 (_stricmp, not inlined strcmp), 0x4d2550, 0x4d25a0 (v[4]), 0x4d2650, 0x4d3980, 0x4d3a30, 0x4d3ac0, d3_mir.c d3_Unk454780 (`for (..; j++, row += n)`).
- Near-misses: 0x4d0380 (6 B: the byte at L+0x1d is a local byte, not v[0]), 0x4d2490 (8 B: res in esi / n in edi), 0x4d3ae0 (10 B), 0x4d3120 (15 B),
  0x4d0a30 (the loop tests p before r; a do-while with an early break did not help), 0x4d27a0 (stores match, the two accumulate / clear loops do not),
  0x4d1f10 (two identical loops in the original), 0x4d0570.
- Callback pairs: 0x4d2450 / 0x4d2490, 0x4d2550 / 0x4d25a0, 0x4d2650 / 0x4d2680 (collect objects by name with d3_ForEachObject 0x438d40, then pick the
  closest); 0x4d3300 is the blood-puddle callback of 0x4d3390 (a death).

## layout 2026-10-22 (session 12)
- The fight end: 0x4d63e0 (start: drops dead ids from the two id lists at +0x80 / +0xc0, a cart without goods or no defender ends it; with
  the player present builds the fight scene 0x4d0fa0 .. 0x4d1e00, asks for the ransom 0x4d4540 and shows the result 0x4d4d70;
  remote (0x6f0d8c) it rolls strength sums by 0x4cdcf0 + random 30 and pays the loot by 0x4d3d10 / 0x4d3ea0). Results of the dialog:
  0x6f0cf8 = ransom taken, 0x6f0cfc = ransom value (-1 refused), 0x4d6930 copies them into the record (+0x354 / +0x358).
- Fight record fields seen: +0x14 / +0x34 / +0x38 / +0x10c character ids, +0x7c kind bits (1 carts, 2 character, 4 building), +0x80 /
  +0xc0 the 16 ids of the two sides, +0x100 three cart ids, +0x110 chance flag, +0x354 / +0x358 result. Fighter slot f (4d6ba0 ..): +0 id, +4 active,
  +0x10 target id, +0x14 / +0x18 place, +0x1d hit, +0x20 kill, +0x24 damage, +0x28 target health.
- Texts: 0x1189 .. 0x118f (end, winner), 0x1191 / 0x1193 / 0x1197, 0x119f .. 0x11a7 (capture outcomes), 0x11e0 .. 0x11e7 (speech), 0x112e ..
  0x1146 (ransom dialog).
- The literal "$C" at 0x602568 (missions block) is used by cm.c too and the link check accepts our own copy: short literals are
  pooled across objs (/GF) at least here; the "dummy_TUER" argument for anm.c / rd.c (notes/layout.md) may rest on that.
- Lessons: passing a `*(unsigned short *)p` to an `int` parameter gives `xor eax,eax; mov ax,..` (a `short` prototype drops the xor): use a
  local cast macro with an int view (G19_4C4880U). `(int)(double)v` of an int folds away, `(int)(double)(float)v` keeps the fild / ftol.
  An inner call as argument pops at once; a local keeps the pops merged. Buffers: one `char buf[n]` reused for two sprintf calls when the
  original's slot is the same (pending-pushes arithmetic: slot = esp offset - pending bytes).

## match-base 2026-10-24 (base-4 worker, shard 4/5)
- ai_Unk4c4db0 -> cm_SendExSysMessage (0x4c4db0: sends message 0x20, the extended system message that the cm_ExSysMessage
  handler receives, with the sub type id and 0x7c bytes of the body); 42 references, lint baseline entries renamed.
- cm.h's `int hmt_SetPendingElection();` became the matching `void hmt_SetPendingElection(int v);` (the conflict with
  hmt.h is fixed, not baselined).

## match-base 2026-10-24 (base-4 worker, shard 4/6)

- Reference-only: ai_Unk58fc40 -> lc_GetGebaeudeBauplatz in cm_ExCreateGeb (0x4cbee5's call site) and in cm.h
  (the K&R declaration stays: the caller pushes two arguments, the definition's first parameter is unused).
  No cm.c code changed.

## match-base 2026-10-25

- cm.h: ai_Unka68d28 and ai_Unkf81220 (0xa68d28 / 0xf81220, the load buffer's size and its received
  bytes) are `unsigned` now: the loader 0x520b80 compares them with `jae` (0x520ba1), so the signed view
  was wrong. cm.c's uses (`ai_Unka68d28 = *(int *)&m->data[0]`, `ai_Unkf81220 += 0x80`) compile
  unchanged; unitdiff cm.c 281/477 functions, 60549 differing bytes before and after.

## match-base 2026-10-25 (base-0 worker, shard 0/6)

- 0x447190 call sites renamed ai_Unk447190 -> d3if_Delete; 0x4f2720 (ai_Unk4f2720) and its declaration renamed gv_GetTableRecord; byte unchanged.

## match-base 2026-10-25 (base-0 worker, shard 0/6)

- From the amt.c work, renamed from the message roles (bytes unchanged, all references): 0x4c4540 -> cm_BeginGroup and
  0x4c4580 -> cm_EndGroup (messages 5 / 6, the group brackets whose messages run only when every check in the group
  passes, 0x4c43c0), 0x4c58a0 -> cm_SendApplication (message 0x44, the 7-byte office application AMTAPP; now int, the
  amt caller uses the send slot), 0x4c58e0 -> cm_SendOfficeRecord (message 0x45, the 15-byte office record AMTMSG; now
  int). The two int returns add no code (the call to 0x4c39c0 is the last statement).

## match-base 2026-10-25 (base-4 worker)
- ai_Unk4c3ae0 -> cm_GetSlotState (the state of message slot h, -1 = no slot), ai_Unk6f259c -> ai_GameOver (the leave-game / game-over flag).- Reference-only rename (no code change): 0x4c5fe0 ai_Unk4c5fe0 -> cm_SendTradeMessage (message 0x5c with the 10-byte
  AMTTRADE record; its only caller is amt_SendOfficeSwap); declared in cm.h / amt.h, the amt.c call updated.

## match-base 2026-10-25 (base-5 worker, shard 5/6)

- Reference-only rename (no code change): ai_Unk75e8c0 -> sv_PanelFlag (0x75e8c0, the panel flag) in cm.h / cm.c.

## match-base 2026-10-25 (base-4 worker, shard 4/6)

- 0x4c3f90 ai_Unk4c3f90 -> cm_SendPlayerId, now `int` (it returns the message slot handle of
  cm_SendExSysMessage, -1 when none; byte-neutral: the body has no return statement, eax passes through).
  cm.h updated.

## match-base 2026-10-26 (base-3 worker, shard 3/6)

cm.c includes d3_light.h and calls d3_RelightObjects / d3_RelightObject instead of the ai_Unk453580 / ai_Unk453280 aliases (cm.h lost those two declarations; d3_light.c Q7). No byte change (unitdiff cm.c no WORSE).

## match-base 2026-10-26 (base-0 worker, shard 0/6, session 8)

- ai_Unk935c00 -> cm_Bombs (0x935c00, the 32 (object, time) bomb records; evidence in cm.c 0x4ce290 / 0x4ce300
  and memset 0x100). Renamed in cm.c, cm.h and sv.c (sv_InitTick clears it). No byte change (check 2610
  unchanged, ledger check --against 0 regressions).

## play 2026-10-28 (the tutorial loading bar: the 0x936420 block)

- `ai_Unk4cd580` (0x4cd580, byte) clears 0x24 bytes at `ai_Unk936420` and `memcpy`s 0x24 bytes into it; the
  block is the count plus the eight player states at +4 (`ai_Unk936424`), i.e. **nine ints / 0x24 bytes**. It
  was declared as `unsigned int ai_Unk936420[1]` plus a separate `int ai_Unk936424[8]`, so LINK placed other
  COMMON objects *inside* those 0x24 bytes: `ai_Unka68c04` (the send-group counter) and `ai_Unka68c00` (the
  free receive-record head) landed at +8 and +0xc. The clear therefore emptied the message free list.
- Symptom: the tutorial's `cm_SendExSysMessage(3, clock)` stayed in the send ring (its `ai_Unkf31160` slot
  never left state 0, the ring could not be copied without a free record), so `main_GameLoop`'s
  `while (cm_GetSlotState(w) == 0) sv_PumpNet();` spun forever **with the loading bar at 100 %**.
- Fix: one 0x24-byte object, `unsigned int ai_Unk936420[9]`; `ai_Unk936424` is a file-local view
  `(*(int (*)[8])(ai_Unk936420 + 1))` (the same pattern as ai_Unk7d6a64 / the income rows). Its extern left
  cm.h. Source-only: no function changed bytes.

## play 2026-10-29 (the d2 caption buffer was one byte)

- `ai_Unkf8aae0` (0xf8aae0) was `char ai_Unkf8aae0 = {0}` (1 byte) but it is the d2 caption / status text
  buffer: sv.c sprintf / strcpy's the hover texts into it (`(char *)&ai_Unkf8aae0`) and `d2_DrawTextBox`
  draws it; gfx.h knows the same symbol as `d2_CaptionCopy[]`. The linker put `ch_List` (0x1468ae0, cm.c)
  16 bytes behind it, so the first hover text overwrote ch_List[0..] with ASCII and the next
  `ch_UpdatePositions` dereferenced it (0x7e6d6965 = "eim~") - the crash when the mouse touched a building or
  the window border (the border scroll moves buildings under the cursor).
- It is `char ai_Unkf8aae0[0x80]` now (cm.h's extern too); 0x80 is the gap to the next item in the original,
  d2_InputDragValue at 0xf8ab60. cm.c's `ai_Unkf8aae0 = 0` (the clear at the scene end) is now
  `ai_Unkf8aae0[0] = 0`.
