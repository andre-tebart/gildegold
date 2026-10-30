# he.c (new, layout 2026-10-23 session 6)

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** first; below it the findings of each session, one section per author.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): event bar, event records, event handlers
- Boundaries: open (layout 2026-10-23, 6th session): new unit, split from main.c at 0x53d430: tag "he_" ("he_AllocNone(): Invalid HE-Type : %i, von Spieler %s" 0x626680, "he_RunNone()", "he_fn_Handlung(): invalid Handlungen_LAST", "he_CreateGfxInfo()", "he_RegisterApEvent()", "he_MessageBoxQuickjump()", "he_CharacterBehavior"; the panel "Panel\Event_Leiste" 0x6266ec). Evidence: .data: its globals 0x625708 .. 0x62571c sit right after main.c's INI strings (0x6256fc "game.INI"), its strings start at 0x626680; code: the event ("Handlung" / HE) records (0x400 of 0x14c bytes at 0x6fab40: type byte, id +4, player +8, due clock +0x52, flags +0x78), the 16 slot event bar, the alloc / run handler tables 0x74df40 / 0x758600 (0xa7 types) and the handlers of the single event types. Evidence added in session 8: the type registration 0x553060 (matched, 67 alloc / run pairs of the types 0x22 .. 0xa5) names the handlers of this file up to 0x56f040 (the forwarder to the free 0x53e1f0) and beyond: the handlers 0x553ae0 .. 0x56... are all in the same event table, so the unit runs at least to 0x573260. Evidence added in session 8: the type registration 0x553060 (matched, 67 alloc / run pairs of the types 0x22 .. 0xa5) names the handlers of this file up to 0x56f040 (the forwarder to the free 0x53e1f0) and beyond, all in the same event table, so the unit runs at least to 0x573260. Open: the end is unknown (the init 0x53d4b0 calls 0x553060 / 0x568310 / 0x56d050 / 0x56f980 / 0x56d220 / 0x56e400 / 0x5733c0 / 0x573260: more modules, probably the same unit up to ~0x575700), Ghidra misses MANY handler starts (about every second function from 0x543ac0 on: found by the `ret` run + aligned start scan, see notes/units/he.md; most drafts are ROUGH)
- Range then: 0x53d430..? (drafted up to 0x568bd2 except the Ghidra-miss starts 0x5690c0 .. in the span of 0x568ae0; layout 2026-10-23 session 10 filled the handler gaps 0x55bf50 / 0x55e180 / 0x55e9d0 / 0x5619a0 / 0x563070 / 0x5652b0 / 0x565620 / 0x565940 ..; 0x548410 and 0x564830 are free-tail labels of 0x5483f0 / 0x5644e0, deliberately without source)
- Was: tag "he_"
- Seam (evidence 2026-10-23): the unit is not the exe's last: .text runs to 0x5eacb4 (last Ghidra start 0x5de450) and 0x575710..0x5de450 (429 KB, 893 functions) is undrafted. he's code references literals only up to 0x62918b ("Game" 0x62916c, "message_filter_mask" 0x629174, "%d" 0x629188, the message-filter feature of the code tail 0x5755c0 / 0x575600) and nothing in 0x62918c..0x62de5c. The next unit's .data starts cleanly at 0x62918c: five 1 ints, a pointer table to 0x578570 / 0x578e40 / 0x579570 / 0x57aa80 / 0x57aeb0 and 18 pointers to the "FT*" state names at 0x629200 .. ("FTBezierEndDelay" .. "FTDisabled", referenced only by the table): the tutorial / campaign module (tag candidate "FT"), literals 0x629304 .. ~0x62a0f4 ("tutorial\main" .. the forms, "TUTORIAL_*", "FTInitChapter*", "CHAPTER_*"). A further block ("contact_*", "Locations\Friedhof", the cannons) starts cleanly at 0x62a264, so the tail holds at least two units, plus the abilities 0x5ba6b0, the click targets 0x5be1d0 / 0x5be720, the duel decision 0x5c6530 and the options / save area 0x5d4xxx. Code seam: he's last function is the setter 0x575710 (288 B, bit setter of 0x6f263c, no literals, body ends 0x575830, fill to 0x575860 - the true end once drafted); 0x575860 (the end-of-game tick: callers sv_Unk52d7a0 at 0x52d9e2 and he's 0x552bc0 at 0x552c07, globals 0x6f265c / 0x6f262c / 0x6310e4 / 0x75e8c0), 0x576020 (caller 0x5d7851) and 0x577110 (no direct call sites) reference neither block; the first function with tutorial literals is 0x577740 ("tutorial\main" at 0x577765, 42 "FTInit*" / "CHAPTER_*" refs in 0x578570). The tutorial range references no he-block literal. Shared literals across objs exist elsewhere in the tail: the options area pushes he's "Game" 0x62916c (48 sites, 0x5d5230 ..), 0x5a5a83 / 0x5a5c08 use he's "Lösegeld" 0x627978 / "dummy_EINGANG_ZELLE" 0x627958 - a cross-block reference is a shared literal, not a boundary violation. Naming: ai_Unk56d050 / 56d220 / 56e400 / 5733c0 / 573260 interleave with he_ functions in the one event table (one obj under the contiguity premise); the ai_Unk553060 extern at the top is he_Unk553060's definition.

## Open work
- **headers (2026-10-30, typing pass 2)**: head.c includes d2_types.h, txt_views.h, he.h, d3.h, d3_quad.h, d3.h, d3_light.h, txt.h, di_input.h, evt.h, data_views.h, unk_5d33b0_types.h, cm_types.h, unk_4b39f0_types.h, time_view.h, ls.h, f3init.h; keeps 6 cast / view macros with @original byte evidence (the address and the matched caller are in each comment): d3t_CreateEmitterK, ai_Unk5a9240K, ai_Unk5a9c30K, gm_NearestBuildingK, sv_SelectPanelObjectK, gm_Unk50a970K.
- layout 2026-10-23 session 15: drafted 0x572fd0 .. 0x575640 (the events 0x94 and 0x9d .. 0xa4). ROUGH: 0x572fd0 (run 0x94),
  0x573760 / 0x573820 / 0x573970 / 0x573ae0 / 0x573c20 / 0x573d30 (the six approaches of 0x9d), 0x573e30, 0x5741f0,
  0x5743c0, 0x574550, 0x5746b0, 0x5748d0 (ROUGH SKELETON, 1802 B: the state flow beyond 0 is a guess), 0x575030, 0x5751e0,
  0x575430, 0x575640. Near misses for the match worker: 0x572fd0 (258/560: the state switch block layout and the duplicated
  text call), 0x573200 (49/80: ev / b in esi / edi swapped), 0x573fb0 (27/176: ev in ebx vs edi), 0x574fe0 (52/80: the loop
  countdown form), 0x5753a0 (13/144: ev / per in esi / edi swapped), 0x575640 (115/176: the cases use different bit-test
  shapes, 0x40 as `(mask & 0x40) >> 6`, 0x80 as `mask < 0`, 0x100 as `mask & 0x100`). Matched this session (19): 0x573260,
  0x573280, 0x5732d0, 0x5733c0, 0x5734b0, 0x573580, 0x5736d0, 0x573f20, 0x574060, 0x5740d0, 0x5740f0, 0x5741d0, 0x574120,
  0x575150, 0x575320, 0x575540, 0x575570, 0x5755c0, 0x575600. Next: 0x575710.
- layout 2026-10-23 session 13: 0x56e5b0 (the run of 0x99) is ROUGH, nonmatching 1850/2752 B: asmdiff shows the
  control flow is right (the 28 prologue instructions are equal, the jump tables 0x56ef80 / 0x56efa0 / 0x56efb0
  covered); the diffs are register allocation (ebx vs edi), unshared epilogue tails and branch polarity — the match
  work is mechanical but large. 0x56efc0 (alloc 0x9a) nonmatching 27/144 (first at +0x4f); 0x56f0b0 (run 0x9b)
  nonmatching 153/208 (first at +0xe; note the latent null-deref the draft keeps faithful: it reads the word of the
  0x505df0(+0xb0) result even when that is 0). Matched this session: 0x56f040, 0x56f050. Next: 0x56f170.
- layout 2026-10-23 session 12: 0x56be30 is a ROUGH SKELETON (272 vs 3959 B): only the preamble (the +0x84 handle check with 0x4c3ae0, the state switch on +0x70 via the table 0x56cda8), the -2 / -1 case and the state 1 case are written; the case bodies of the states 0 .. 7 are comments with the call lists only. Redraft it from `uv run disasm 0x56be30` before matching. All drafts of this session after 0x56be30 are ROUGH (0x56ce40, 0x56d2d0, 0x56d3d0, 0x56d610, 0x56d730, 0x56d990, 0x56daa0, 0x56ddc0); 0x56ce40 and 0x56dd20 are near (35 .. 57 B differ).
- layout 2026-10-23 session 10: drafted 0x565410 .. 0x568be0 (ROUGH / skeleton: 0x566830 (attack of up to eight attackers, 3568 B original, ours 1904: states 4 / 5 summarised), 0x568be0 (income event, 1232 / ~1250 B, the loop body is approximated)). Ghidra-miss handlers found through the registration code 0x568310 (it lists (type, alloc, run) of every handler: the starts missing from names.csv are gaps; checked for 0x553060 and 0x568310: none left) and filled: 0x55bf50, 0x55e180, 0x55e9d0, 0x5619a0, 0x563070, 0x5652b0, 0x565620, 0x565940, 0x5662d0, 0x5679b0, 0x567b80, 0x567d20, 0x567d60, 0x567f00, 0x568220; 0x565db0 and 0x566370 (night watchman) hide behind Ghidra's 0x565d10 span. Matched this session: 0x565500, 0x565b00, 0x565b60, 0x565d10, 0x5662d0, 0x567620, 0x567b80, 0x567d20, 0x567f00, 0x5679b0, 0x55e180, 0x5619a0, 0x568310 (the registration, 1936 B), 0x568aa0. Near misses: 0x565410 (50 / 240 B), 0x563070 (2 / 128 B), 0x568ae0 (13 / 256 B). Next: see the Frontier in notes/layout.md. Layout 2026-10-23 (session 11) drafted 0x5690c0 .. 0x56ba97: matched 0x56a100, 0x56a360, 0x56a710, 0x56b380, 0x56b750; near 0x5690c0 (he_AllocProduktion, 62 / 336 B: the table index `tab + (id << 6) + id + 0x22` is scheduled differently, the original loads 0x91aaf0 before the movzx), 0x56a7b0 (the message record is built in a different order, 218 / 432 B), 0x56b700 (84 / 96 B: the original pushes 0x20 / 0x46 first and keeps the object in eax, ours uses esi); ROUGH drafts: 0x569210 (he_RunProduktion, 3376 vs 3793 B), 0x56a170 / 0x56a3d0 (sibling run handlers), 0x56a960 (trade handler, 2352 vs 2583 B: the two slot loops are simplified), 0x56b3f0, 0x56b570 (he_AllocGebaeudeBauen), 0x56b780 (building construction scene objects).
- Free-tail labels without source (not functions): 0x548410 (the body of 0x5483f0: its `jne 0x548410` jumps there), 0x564830, and 0x4f9764 (a case of a switch in ord.c's range, ebp frame).
- Not drafted: 0x543ac0 (1456 B) and 0x545840 (2224 B), both Ghidra misses (disassemble); 0x544840 (3584 B), 0x546100 (4320 B), 0x5472d0 (3936 B) and everything after 0x5472d0; whatever follows 0x5460f5 (more starts inside the Ghidra function 0x545640 .. 0x546100). The unit end is unknown (see tus.md).
- Not in address order: 0x544330 .. 0x544630, 0x5471e0, 0x545640, 0x545670 are appended at the end of he.c (marker comment); sort them when the gaps are filled.
- Matched: 0x53d430, 0x53d470, 0x53d4b0, 0x53d590, 0x53d5d0, 0x53d600, 0x53d9b0, 0x53dad0, 0x53eba0, 0x53ebe0, 0x53ec10, 0x53ec40, 0x53ed40, 0x53ed70, 0x53ee40, 0x53ef80, 0x53efb0, 0x540fe0, 0x541200, 0x541250, 0x542960, 0x543020, 0x543180, 0x5436a0, 0x545640, 0x5471e0.
- Near misses: 0x53dcb0 (24 B: the original keeps the first slot in ecx and re-reads the global `0x6f262c` before the second call), 0x53f430 (11 B: store order of the 3 message ints), 0x53f500 (12 B: the original has a 0x14 frame: one more local), 0x53e4e0 (30 B: the loop variable `i` lives in memory in the original, `cnt >= 0x20` break), 0x53e840 (41 B: register choice, ebp = 0), 0x541010 (30 B).
- Not matching but equal size: 0x53d6f0, 0x53d800 (the packing of the slots: `q` pointer arithmetic), 0x53d930 / 0x53d9f0 (the 16 bit operand `sub ax,0x21` of the panel position: the original passes a word-sized value: try `short` temporaries), the message senders 0x53dda0 / 0x53dee0 (the original frame is 0x2098 / 0x2098: a 0x1f90 char buffer after a 0xf8 message: ours has the same sizes but another layout; the text copy loops are `strcpy` + `strlen` in the original (inline repnz scasb)).
- ROUGH (written from the pseudocode, the control flow may differ in details): 0x53f6b0 (the advice of the day, the 5 themes are written with macros, original 5600 B, ours 4192), 0x541280 .. 0x5417d0, 0x541b40, 0x542b60, 0x5437c0, 0x544330, 0x544630, 0x545670. The names of the message ids (0x5ec .. 0x60a: message kinds of the 0x53dda0 / 0x53dee0 senders) are not known.
- Types: an event record (`ev`, 0x14c bytes, also ai.h AIJOB): +0 type, +4 id, +8 player (ushort), +0xc player id, +0x10 argument, +0x44 start clock, +0x52 due clock (HETIME 14 B), +0x70 state, +0x74 the slot (0x6faa80 + i * 12: owner, window id, panel), +0x78 flags (2 = posted, 4 = frozen, 8 = has a window, 0x10 run every frame), +0x84 the pending 0x4c4c60 handle, +0x88 the gfx info, +0xac .. +0xc0 arguments (ids of the figures), +0xb0 / +0xc5 own clocks in some types. The message record (0xf8 B, `HM_B` / `HM_I`): +4 type, +8 target player id, +0xc source id, +0x28 clock (HETIME), +0x36 flags (2 / 1 / 9), +0x58 .. arguments; sent with 0x4c4b70.

## layout 2026-10-23 session 6
- Split from main.c at 0x53d430 (tag "he_", see tus.md). The unit is the event system of the game ("Handlung" = action in the assert text, HE = history event, the event bar "Panel\Event_Leiste"): 0x400 event records at 0x6fab40 (0x14c bytes), the alloc / run handler tables 0x74df40 / 0x758600 (0xa7 types, registered by 0x53d600; types 0 and 1 are `he_AllocNone` / `he_RunNone` and 0x44ee40), the 16 slot bar at 0x6faa80, the quick list of records with flag 8 at 0x74db40 (0x100 pointers), the filter / cursor 0x625708 .. 0x625714 / 0x7585e0 of the search 0x53e400 / 0x53e300 (varargs: n, then pairs kind / value; kinds 0 type, 1 owner id, 2 player word, 3 argument +0x10), the gfx info table 0x7588a0 (0x40 entries of 16 bytes), the AP event table 0x758ca0 (0x100 of 0x50 bytes: counter byte, values, name).
- The module init 0x53d4b0 calls the inits of the other event modules 0x553060, 0x568310, 0x56d050, 0x56f980, 0x56d220, 0x56e400, 0x5733c0, 0x573260 (each returns non zero on failure): the unit probably covers all of them.
- The inn meeting ("stammtisch") scripts: command codes are 4 characters as ints ("join" 0x6a6f696e, "cler" 0x636c6572, "exec" 0x65786563, "leav" 0x6c656176, "new " 0x6e657720); he_Unk541b40 handles them, he_Unk542100 is the event that sends "new " / "exec" / "cler", 0x53f430 / 0x53f500 send "leav" / "clear" through 0x4c4540 / 0x4c5d80 / 0x4c4580 (script call with the name "stammtisch supervisor" / "clear dunkle ecke" / "stammtisch clear HE").
- The first mid-function `ret` + nop padding scan (disasm, awk on ret / nop) finds Ghidra's missed starts quickly; they are 0x10-aligned. Matched from the disassembly: 0x543020, 0x543180, 0x5436a0, 0x542960 (all first try once the clock is an AITIME struct assignment and the kind tests are written as in the listing).

## layout 2026-10-23 (session 7)
- Drafted 0x543ac0 .. 0x550070 in address order (about 100 functions, ROUGH: right calls, control flow, globals; bytes later). Matched right away (small ones): 0x5455e0, 0x548110, 0x5483a0, 0x548b70, 0x549080, 0x5482c0, 0x548330, 0x548380, 0x548750, 0x548ef0, 0x549040, 0x549110, 0x54be20, 0x54c4c0, 0x54c510, 0x54c870, 0x54cc20, 0x54d190, 0x54d790, 0x54da20, 0x54de60, 0x54e770, 0x54eb80, 0x54fe80, 0x54b170, 0x54b9b0, 0x54ae20, 0x549f70, 0x54d1e0 (not yet: 9 B off), 0x546100, 0x547170 (names.csv is updated by `uv run build`).
- Event record layout learned (see he.c comments): +0x44 start clock, +0x52 due clock (0x4fe140 (clock, d, h, m) posts it later; 0x4c4c60 (state, ev) schedules the handler, result in +0x84), +0x70 state (-1 / -2 = free requests), +0x74 the window slot, +0x78 flags (2 = own player, 4 = paused), +0xac .. the per-type arguments. Strings give the event names: "Meisterpruefung", "Schutzgeld", "BetriebsPruefung" (audit), "Taschendieb", "Pest", "Patrol", "Dunkle Ecke", "SIM_WIRTSHAUS", "RunSimUnfall", "RunSimKrankheiten", "Alloc_Geliebte", "RndGeselle", "Sensenmann", "SPECIAL\TITEL", "SPECIAL\TALENT_STUFE", the duel ("0x544840").
- Near misses (match worker): 0x548fa0 / 0x548f30 / 0x548230 (about 60 B: the free / post order; `st >= -2` is right), 0x546c40 (217 B: the final loop `while (n != 0)`, register allocation of r / x), 0x547f90 (locals order of the two 0x4fea50 outs), 0x547ee0 (building record index `base + c + c * 588`), 0x54b9b0 matched after the argument order of 0x4c4a60 (flag, 0 = set at the 4th place).
- Frame sizes found for the ROUGH drafts: 0x5461c0 0x924, 0x54b1e0 0x2fc, 0x54dec0 0x3c, 0x54f160 0x418.
- Open work: all drafts of this section are ROUGH (floats via raw constants in 0x54fc00 / 0x54fc70 (0x415ad0 effect calls, argument layout unclear), the message records of 0x114 bytes have only the written fields); the loop in 0x549ce0 / 0x549aa0 is guessed (Ghidra pseudocode only).

## layout 2026-10-23 (session 8)
- Drafted 0x550370 .. 0x5649b0 in address order (about 130 functions; 0x541a50 is a Ghidra miss in the older range and was added). Matched: 0x541a50, 0x550ea0, 0x5516a0, 0x552040, 0x552200, 0x5524e0, 0x552520, 0x5527c0, 0x552820, 0x552880, 0x552b90, 0x552bc0, 0x552c30, 0x553060 (the type registration, 2128 B), 0x553cc0, 0x553d10, 0x553d60, 0x554820, 0x5551c0, 0x5552c0, 0x557160, 0x557b30, 0x557d70, 0x5584d0, 0x558de0, 0x558e70, 0x55bd20, 0x55ce00, 0x55e3a0, 0x55e5c0, 0x561130, 0x563980 (and the other small allocation handlers). Everything else is ROUGH: the larger run handlers (0x556600, 0x557190, 0x557e20, 0x558f10, 0x55a730, 0x55efc0, 0x55fa90, 0x560900, 0x561c20, 0x5628e0, 0x5630f0, 0x5639c0, 0x5649b0 ...) are SKELETONS (right calls and states in order from the call list, the middle parts missing: ours is 40 .. 60 % of the original size); redraft them from the disassembly (`uv run disasm ADDR` = the Ghidra function) before matching.
- Event families seen (alloc handler = the first function, run handler the second): the transport legs 0x555350 / 0x555690 / 0x555b60 / 0x556080 (modes 1 .. 4 of +0xc8, run by 0x556600 "he_RunTransport"), the delivery 0x556dc0 / 0x556ed0, the message boxes 0x552090 / 0x552ca0 (the award ceremony "_VERLEIHUNG_REICHSAMT_*"; literal strings), 0x557190 (the generic event message box, flags in +0xf0), the building events 0x55b3e0 .. 0x55bd80 (+0x5a flags of a building, messages 0x5a / 2 / mask), the purchase / loan 0x55bfd0 / 0x55c540 / 0x55c700, the exam 0x55cc10 / 0x55ce60, the dismissal / promotion 0x55dbe0 / 0x55de30 / 0x55e1d0, the crime events (sabotage 0x558f10, espionage 0x55a030, brawl 0x55a730, burglary 0x561a20 / 0x561c20, building espionage 0x5628e0, attack 0x5639c0, ransom 0x561190 / 0x5614c0), the fire 0x5644e0 .. 0x5649b0. Recurring helpers: 0x501190 (1, kind, id) = look up a building, 0x505df0 = figure by id, 0x4fea50 (&out, 0, 0, id) = object by id, 0x524260 (building, target, type, ev) = group register (release with (0, 0, type, ev)), 0x4c52e0 (figure, target, -1, 0, mode, "animation name") = send, 0x53dee0 / 0x53dda0 = show a news line (text from 0x5176d0 (buf, text id, ..)), 0x4c4b00 relation change, 0x4c4830 payment (to, from, player, amount), 0x4c4a60 (figure, 0x5a / 0x13 / 0x1cc, 2 / 4, mask, 0) = set a flag message.
- The type registration 0x553060 (types 0x22 .. 0xa5, he_Unk53d600 (type, alloc, run)); the handlers it names above 0x553060 are all in this file, 0x56f040 is a forwarder to the free; the module inits called by 0x53d4b0 cover the rest.
- Frame / near misses: 0x5512c0 (10 B: the minute word store of the clock copy), 0x553db0 (2 B), 0x553e10 (18 B), 0x558e10 (13 B: the tail call `jmp 0x53e1f0` is only emitted when the free is the last statement), 0x561470 (the 4fe140 arguments), 0x556dc0 (24 B).
- Open work additions: skeleton functions listed above; 0x564830 is the free tail of 0x5644e0 (no function); the 12 / 1 slot sales tours 0x554320 / 0x554860 are the same shape with different counters.


## layout 2026-10-23 (session 11)
- Handler shapes: an alloc copies the start clock (+0x44) to the due clock (+0x52) and the second clock (+0x60) (14-byte HETIME struct assignment), then `4fe140 (ev + 0x52, a, b, c)` sets the due time; its random part is an INLINE argument `(unsigned short)ai_Unk42ec00(n)` (a separate variable changes the push order). A run handler is `switch (*(int *)(ev + 0x70))` with the states -2 / -1 (hand the characters on with 0x524260 and free with 0x53e1f0), 0 (state = 1) and the work states; the jump table follows the function.
- 0x4fea50 (out1, out2, out3, id) fills whichever of the three out pointers is not 0 (the building record, the second building record, a third); the character records are the 0x21c-byte entries of 0x770b80 .. 0x7d5f80 (+0x17c = the event they work for).
- The news text of a handler is chosen by the building type byte (0x7e3f30 + kind * 0x24d): the table of "_NACHRICHTEN_ROHSTOFFMANGEL_HS_nn" names (he_ProdShortage, unlinked helper in he.c).
- A string with a backslash written through a bash heredoc loses it (`"Misc\EventMessagebox"` became `"Misc\EventMessagebox"`): the link check reports it as a data conflict; fix it by writing the string with the Write/Edit tool instead of a heredoc.

## layout 2026-10-23 (session 12)
- Drafted 0x56bac0 .. 0x56ce40 in address order: 0x56bac0 (the highest stage number in the "_nn" suffixes of the children of the "he_<id>" object: the children via 0x438d40 with the callback 0x56b750, each name parsed right-to-left with strrchr; nonmatching 316/400, the loop index / parse order are the open points), 0x56bc50 (creates the two scene objects of a building event: the model of the type byte *type from the table 0x627e98 - 0x50-byte records, four codes, the model name at +0x10, the record with first dword 0 ends the table and its name is the fallback - then the group "model" (arg4); both placed with 0x43b790 (pos) / 0x43b860 (rot), the first named "he_<id>" through sprintf into the object, the second dropped with 0x412a70 and hooked with 0x50d380 (o, type, 2); returns the second; nonmatching), 0x56be30 (ROUGH SKELETON, see Open work), 0x56cdd0 (the alloc of the type 0x4c: both clocks +0x52 / +0x60 = the start clock +0x44, due 10 through 0x4fe140, state 0, +0xac = -1; matched first try).
- Shapes: the alloc / run pair of the type 0x4c is 0x56cdd0 / 0x56ce40 (the run is the next open function; its states -2 .. 2 come from the table 0x56d038, the player table 0xf82320, the player byte 0x624580, ai_Time hour < 0x16); the pair of the type 0xd is 0x56b570 / 0x56be30. 0x56bc50 has exactly one caller: the (still skeleton) case 4 of 0x56be30 at 0x56c0b7.
- Helper meanings: 0x44c3f0 = d3_LoadGroup44c3f0 (name, f, parent) -> the first object; 0x43b790 = d3_SetObjectPos (pos); 0x43b860 = d3_SetObjectRot; 0x412a70 = d3t_DropObject (obj, maponly, top); 0x50d380 (o, p, 2) names the scene object for a building p and hooks it up.
- REA batch_decompile resolves the Ghidra starts 0x56bac0 .. 0x5706e0 (the Ghidra function 0x56bc50 is really 0x56bc50 + the 0x56be30 run + 0x56cdd0 + 0x56ce40); the exports were not saved to build/ghidra (transcript only): rerun batch_decompile over the batch when the next session needs them.

## layout 2026-10-23 (session 12, continued)
- Drafted 0x56ce40 .. 0x56e400: 0x56ce40 (run of 0x4c: the player byte 0x624580, the 0x80-entry table 0xf82320 of 0x24-byte records (+4 the character's +4, +0x20 the flag), the states 0 = 0x4b4f00 + wait, 1 = 0x4c4db0 (8, &the record's +4) and wait for its handle +0xac, 2 = the handle result), 0x56d2d0 / 0x56d3d0 / 0x56d610 / 0x56d730 / 0x56d990 / 0x56daa0 / 0x56dd20 / 0x56ddc0 (the research events 0x95 .. 0x98: the character +0xac, the record helpers 0x4eaea0 / 0x4eae60 / 0x4eb0b0 / 0x4eabe0 / 0x4eab20, the amount float +0xb4 against the constant 0x5eb788, the night window 0x617830 / 0x617840 by the day index 0x4fe350, the clock regions +0xbc / +0xc0, the building registration 0x4ffe90 + 0x524260 and the release 0x524260 (0, 0, id, 0)), 0x56de10 (the run of 0x98, the random event), 0x56e070 (the scene action of 0x98) and 0x56e400 (the init of 0x99 .. 0x9b, matched).
- The init 0x56e400 registers the handler bodies 0x56e460 (alloc 0x99, 321 B), 0x56e5b0 (run 0x99, 2512 B), 0x56efc0 (alloc 0x9a, 123 B), 0x56f040 (the free forwarder, 5 B: jmp 0x53e1f0) and 0x56f0b0 (run 0x9b, 186 B): they are the NEXT frontier. Drafts past them (0x56f050 .. 0x571640, about 22 functions) were written in this session and reverted before the commit: the strict ledger check reported the skipped handler bodies as gaps.
- Matched this session: 0x56cdd0, 0x56d050, 0x56d220, 0x56e400. ROUGH: 0x56be30 (skeleton, see Open work), everything else after it.
- 0x56dd20 differs in 35 B only with the HETIME struct copy of +0x52 = ai_Time (the field-by-field variant was worse, 113 B).
- The alloc pattern of the research types: state 0, the amount in +0xb4 (a float), the clocks now (both +0x52 and a second region), due 5; the run pattern: night check (retry after 1), 0x56ddc0 (a task in progress) -> retry after 0xf, state 1 hands the result over and frees, state 0 takes the elapsed time off the amount and retries after 5 until it is used up.

## layout 2026-10-23 (session 13)
- Drafted 0x56e5b0 .. 0x56f0b0 in address order: the registered handler bodies of the init 0x56e400 (0x56e460 was
  session 12). 0x56e5b0 (the run of the type 0x99) was decoded from the disassembly (no Ghidra export): the states
  -1 / -2 free path (the building of +0xb8 released with 0x4c4e00, the text 0x628dd4 to the player, free), the paused
  +0x78 & 4 / active handle +0x84 checks, the outer switch on +0x70 (table 0x56ef80: cases 0, 1, 2, 3, 4, 5, 6, 7);
  state 0 waits the notification +0xcc (0x4c3ae0 / 0x4c3b10: the building kind into +0xbc; kind 3 -> state 6, else
  the text 0x628dc8 via 0x4c52e0 -> state 1); states 1 / 3 poll the building done ([0x184] + 0x128, waits 0 / 4
  minutes) then the checks 0x56f2e0 / 0x56f1b0 / 0x56f3f0 pick the texts 0x4004 .. 0x401a (0x5176d0 / 0x53dda0) and
  state 4; state 4 the text 0x628dc8 -> state 5; state 5 waits the building, then the rest time by the kind +0xbc
  (0x21c / 0xb40 / 0x5a0 seconds, at least 5, via 0x4fe140) and frees; state 6 the domicile search 0x56f600 (state 7;
  else the action 0x4a63f0 (per, 0, 0xa7f4c7a0, 1, 0, 100.0f, &ev+0xac) and state 6), state 7 the search 0x56f740.
  0x56efc0 (alloc 0x9a), 0x56f040 (the free forwarder), 0x56f050 (alloc 0x9b) and 0x56f0b0 (run 0x9b: state != 0
  frees; the record of +0xac: the text 0x4035 / 0x4036 (+ the word of the +0xb0 record) to the player).
- The forwarder 0x56f040 matched only as a void tail call `void ai_Unk56f040(int *r) { he_Unk53e1f0(r); }` with the
  older file-scope declaration `int ai_Unk56f040();` (above 0x553060) changed to `void ai_Unk56f040();` — VC6
  rejects the mixed int / void declarations (C2371); the declaration change is byte-neutral (all uses are
  `(void *)ai_Unk56f040` address takings in 0x553060 / 0x56e400). he_Unk56f050 (the alloc of 0x9b: the paused
  +0x78 & 4 return, due = now, 0x4fe140 (+0x30, 0, 0, 0), handle 0x4c4c60 (0, ev)) matched first try.
- Matched this session: 0x56f040, 0x56f050. ROUGH: 0x56e5b0 (see Open work); 0x56efc0 / 0x56f0b0 are small
  near-misses for the match worker.

## layout 2026-10-23 (session 14)
- Drafted 0x56f170 .. 0x572b90 in address order (the events 0x88 .. 0x94 and their init). Matched first try: 0x56f170 (the
  owner search of 0x99), 0x56f1b0 (the meeting message: the float order a - b * 0.5f + 0x42ec20 () >= 1.0 with float
  locals made it match after the first attempt), 0x56f2e0 (same shape with the 0x4fca40 cast), 0x56f360 / 0x56f3b0 (the
  duplicate searches), 0x56f980 (the init of 0x88 .. 0x94, 576 B: the nested he_Unk53d600 chain and the five 0x52b350
  texts; the address comment must sit directly above the definition, not above the declaration block), 0x56fc50 (the
  alloc of 0x88), 0x570130 (the alloc of 0x89), 0x570bf0 / 0x570c70 (the plunder texts: ai_Unk4c4830 takes an unsigned
  char third argument, the text 0x40fe prints the WORD of b, not of a), 0x570ea0, 0x571990 (the alloc of 0x8e), 0x5725b0
  (the run of 0x90: the free path of the original continues after he_Unk53e1f0, do not add a return), 0x5726a0, 0x572860
  (the alloc of 0x8b / 0x92), 0x572b90 (the alloc of 0x93).
- Handler shapes learned for this family: an alloc checks paused (+0x78 & 4), schedules with 0x4fe140 (due = now + n hours)
  and returns; a run switches on +0x70 with the states -2 / -1 freeing and the work state 0; the win / message helpers
  (0x4c4b70 with a 0xf8 record whose type byte sits at +4) recur.
- The night window of the day-based events (0x571000, 0x571640): ai_Unk4fe350 (&ai_Time) is the hour, the minute word is
  *(unsigned short *)&ai_Time + 4, and the two float curves 0x617840 / 0x617830 [hour] are compared with `(float)minute`;
  a state -1 / -2 still works outside the window.
- ROUGH: 0x56fda0 (run of 0x88), 0x5707a0 (run of 0x8a), 0x571000 (run of 0x8c: the message part around the 0x53dee0 calls
  is a skeleton), 0x571640 (run of 0x8d: the value formula (4 - x) * y * 0x5eb91c is a guess), 0x571c30, 0x5720f0,
  0x572310, 0x5726b0, 0x572bf0 (the run of 0x93). The rest are right-shaped near-misses (see notes/layout.md, Frontier, for the counts).
- Near misses for the match worker: 0x56f260 (want / round swapped: ours round in ebx, the original want in ebx; the dec of
  the outer loop sits at the loop top in the original), 0x56fbc0 (91/128: the original spills (float)num to [esp+0xc] and
  hoists the +0x18 load before the branch), 0x5706e0 (the constant 4 is hoisted into eax by the original), 0x571ac0 (80/352:
  the local grp = 0 store is hoisted above the first call, the failure paths share one epilogue).
- Tooling: REA batch_decompile has no procedure for the Ghidra-missed starts (0x56fda0, 0x571000, 0x572bf0 ...); it does
  resolve 0x5720f0 / 0x572310 / 0x5728d0 / 0x5729c0. A `/* 0x... */` comment above a declaration block attaches the address
  to the NEXT definition: keep the declarations above the comment.

## layout 2026-10-23 (session 15)
- Drafted 0x572fd0 .. 0x575640 in address order (the events 0x94 and 0x9d .. 0xa4). The unit now runs to 0x575710 (the next
  Ghidra start, 288 B, not drafted); `uv run ledger next` = 0x575710.
- Shapes: the event records of this family carry the figures at +0xac and +0xb0 (0x505df0), the building at +0x10 and a handle
  at +0x84 (0x4c4c60 (state, ev)); an alloc copies the clocks and schedules, a run switches on the state with -2 / -1 freeing.
- 0xa2 (matched 0x573260 / 0x573280 / 0x5732d0) is the daily Hanseatic League politics: after 10:00 the due clock is the next day
  10:00 (0x4fe140 (.., 0x18, 0, 0), hour +0x56 = 0xa, minute / second +0x58 / +0x5c = 0); the run steps 0 .. 4 through hmt_Update
  (0x4ace40), hmt_Begin (0x4ad0a0), hmt_Election (0x4ad1c0), 0x4ad510 (time of day 6:05, day +1) and 0x4ad580 (time of day 10:00).
- 0x9d is an event with six approaches (the parameter at +0xac, 0 .. 4 and a default): 0x573760 (a name block 0x62901c / 0x629038
  and the texts 0x41dd / 0x41de), 0x573820 (relations 0x4ea550, 0x4e6f80), 0x573970 (a character of the kind 0 whose building word
  is the player's, a message 0x33), 0x573ae0 (the five values +0x80 .. +0x84, 0x4c6020), 0x573c20 (a message 0x9e), 0x573d30 (a
  message 0x9f); all end with the message 0x5fb to per +4 and the name as its text2. The first step is 0x573580 (state 0:
  0x5736d0 (player, figure, approach), retry in 24 h at 6:00 + 15 * random (4) + 5 min), the second 0x573e30 (a payment -n,
  n by the approach, except while per's +0x194 reaches n).
- 0x9e (0x573f20 / 0x573fb0), 0x9f (0x574060 / 0x5740d0), 0xa3 (0x575150 / 0x5751e0), 0xa4 (0x575320 / 0x5753a0): small daily
  events; 0x5740d0 matched as an `if (...) he_Unk53e1f0(ev);` tail call (the jmp form).
- 0xa0 (0x574120 / 0x5743c0) is the trial: 0x5741f0 (the building kind 0x50d060 and floats) runs in state 1; 0xa1
  (0x574550 / 0x5748d0) the meeting of up to four figures (+0x8c ..) in a building (0x574430's ROUGH scene 0x5746b0, the income
  0x575030, the rooms +0xc0 .. +0xce).
- Helper meanings: 0x5092e0 (building, 0x1d, 1) = an amount; 0x50cc50 (id, n) = a payment; 0x4c4830 / 0x4c4880 = payment
  messages; 0x4c4b70 = post a message record (0xf8, type at +4, clock +0x28, flag +0x36); 0x5176d0 = format a text; 0x5de518 =
  __ftol; 0x5a9e90 (index, ev +0xc8) = a meeting partner id; 0x5a9f10 (3, 6) = a record kind; 0x4a5760 (per, n, kinds, out) =
  records for kinds; 0x42ec20 = a random float, 0x42ec00 = a random int.
- 0x5755c0 / 0x575600 (matched) handle the INI: the path copied to 0x6fa0e0, GetPrivateProfileIntA / WritePrivateProfileStringA
  of "Game" / "message_filter_mask" (0x6f263c); 0x575640 answers whether the mask has the bit of the message kind 0x605 .. 0x60d.
- Tooling: a `cat >> file` without a file argument hangs the Bash tool (stdin is not at EOF) — use the Write/Edit tools for file
  changes; the timer keeps running while a command hangs.
- Matched first try: 0x573260, 0x573280 (after the pointer-local fix: write the clock stores through HM_I/word casts, not a HETIME
  pointer local), 0x5732d0, 0x5733c0, 0x5734b0, 0x573f20, 0x574060, 0x5740d0, 0x5740f0, 0x5741d0, 0x574120, 0x575320, 0x575540,
  0x5755c0, 0x575600; 0x5736d0, 0x573580 (after the if-form instead of a switch and the address expression in the second test),
  0x575150, 0x575570 (after the unsigned char cast).

## layout 2026-10-23 (session 16)
- he.c is closed: its last function is 0x575710 (the message filter bit setter, the setter side of 0x575640), body to
  0x57582f, jump table 0x575830 .. 0x575853, nop fill; the unit after it is the new unk_575860.c (see notes/units/unk_575860.md).
- 0x575710 drafted, nonmatching 338/416: the original keeps the mask in the hoisted eax and uses byte ALU (`or al 1`,
  `or ah 1`); our int lvalue reloads and uses 32-bit ops. An unsigned short cast at the lvalue changed nothing (`uv run try`).
- The INI pair 0x5755c0 / 0x575600 and the filter query 0x575640 from session 15 stay as they were (2 of 3 matched).

## match-base 2026-10-25 (base-0 worker, shard 0/6)

- 0x447190 call sites renamed ai_Unk447190 -> d3if_Delete; 0x4f2720 calls renamed ai_Unk4f2720 -> gv_GetTableRecord; byte unchanged.
## match-base 2026-10-25 (base-1 worker, shard 1/6)

- he_Unk53e300 (0x53e300, the he_FindEvent iterator) and its foreign name ai_Unk53e300 merged into
  he_NextEvent project-wide; he.c's redundant K&R declaration dropped (the definition above is the
  prototype). he.h declares char *he_NextEvent(void); ai.h keeps the AIJOB* view (one lint X pair).

## match-base 2026-10-25 (base-4 worker)
- Reference-only renames (no code change; every use updated, build/link/ledger clean): ai_Unk4c3ae0 -> cm_GetSlotState (0x4c3ae0) and ai_Unk4767d0 / ai_Unk476860 -> d2_StartFade / d2_ReleaseFade (0x4767d0 / 0x476860, the old ai_ aliases folded into the d2 names). See notes/units/unk_5d33b0.md for the whole rename list.

## match-base 2026-10-25 (base-0 worker, shard 0/6)
- Reference-only rename: ai_Unk53dda0 / he_Unk53dda0 -> he_ShowNews (0x53dda0, the message-0x11 event text / news line to a player character, this note's "show a news line"; every declaration - he.h and the am.h / amt.h / cm.h / rt.h / f3gm.c views - and all 264 call sites updated). 0x53dee0 (the quick-jump variant, he_Unk53dee0) is not renamed. he_Unk567c00 (0x567c00) called it with 5 arguments through the old alias; the original pushes text2 = 0 (push 0 at 0x567c24, add esp,0x18), the call passes the 6th argument 0 now.


## match-base 2026-10-26 (base worker, main checkout)
- Reference-only rename: he_Unk53dee0 / ai_Unk53dee0 -> he_MessageBoxQuickjump (0x53dee0), evidence: the
  function's own error string "he_MessageBoxQuickjump(): contactname is too long!" (0x626700) and the 0x30
  quick-jump name it copies into the message (+0x68). Every declaration (he.h and the am.h / amt.h / ai.h /
  gv.h / f3gm.c views) and all 100+ call sites updated. The 0x53dda0 alias folded earlier stays as it is.

## match-base 2026-10-26 (base-3 worker, shard 3/6)

he.c includes d3.h + d3_light.h; the ai_Unk453580 K&R prototype is gone and the call uses d3_RelightObjects. No byte change.
