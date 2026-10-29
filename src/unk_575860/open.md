# unk_575860.c (the unit after he.c)

## Unit
Unnamed unit starting at 0x575860 (layout 2026-10-23, session 16). It follows he.c: he's last function
0x575710 (the message filter bit setter) ends at 0x57582f, its jump table 0x575830 .. 0x575853, nop fill to
0x575860. Boundary evidence: code seam + literals. The code from 0x575860 on reaches neither he's literal block
(last he literal "Game" 0x62916c, "%d" 0x629188) nor the next .data block, which starts cleanly at 0x62918c (the
"FT" state name table); the first function with tutorial literals is 0x577740 ("tutorial\main"). So the unit's
own start is plausible, but its name and extent are open: 0x575860, 0x576020 and 0x577110 have no literals at all
and could belong to the "FT" tutorial module or to a module of their own.

Globals seen so far: 0x5eec60 (the state record pointer: +0 the state 1 .. 0xc, +0x18 the running record,
+0x1c the player, +0x74 the progress word, +0x2c / +0x30 / +0x38 / +0x3c flags), the clock 0x6333cc /
0x6333e0, the gates 0x6f265c / 0x6f2660 / 0x6f262c, the pause pair 0x6310e4 / 0x75e8c0, 0x6f22a0 = 0x6f229c.

## Open work
- Session 17 ROUGH skeletons still to fill from the disassembly (all building, all nonmatching): 0x577e50 (chapter
  form), 0x578180 (the tutorial start), 0x5784a0, 0x578570 / 0x578e40 / 0x579570 / 0x57aa80 / 0x57aeb0 (the
  chapter chains, mechanically generated - the field stores are exact, the memset / scheduling is not),
  0x57b8a0 (the per-frame update: the phase machine and the Bezier part are right, the tail needs a check),
  0x57bc30, 0x57bce0 (the input tail), 0x57bf00, 0x57c560, 0x57c690, 0x57cde0, 0x57ced0 (head only).
- Near-misses from session 17: 0x577b90 (9/208), 0x577d90 (93/128), 0x5780e0 (8/64), 0x578370 (10/208), 0x57b810
  (91/144).
- 0x575860 ROUGH SKELETON: only the entry checks, the first-run / pause block and the state dispatch are written;
  the twelve case bodies (jump table 0x575fe8, states 1 .. 0xc) are comments. Helpers in address order:
  0x476f80, 0x577c60, 0x57b8a0, 0x577d90, 0x5778b0, 0x576040, 0x53dcb0, 0x577a00, 0x577a90, 0x577b60,
  0x4741a0, 0x577e50, 0x475e40, 0x5780e0, 0x577b90, 0x52d7a0, 0x52e330, 0x426f20, 0x427290, 0x4c2760.
- 0x576040 nonmatching (658/768): the structure (the state reads through the macro S, the callback +0x38, the
  transition writes) is written; the diff starts at +0x10 (the argument spill order). Check when the unit is matched.
- The callback / predicate family 0x5764b0 .. 0x577540 is drafted; nonmatching near-misses: 0x5764b0 (40/80),
  0x576500 (115/144), 0x576590 (36/80), 0x576600 / 0x576640 / 0x576680 (35 .. 38/64, first diff +0x8: the original
  compares `[p]` in memory, ours loads p[0] first), 0x5766d0 (199/240), 0x5767d0 (64/96), 0x576830 (11/112),
  0x5768d0 / 0x576920 / 0x576970 (8/80, first +0x2f: the `*(unsigned short *)p[1]` compare), 0x5769c0 (173/208),
  0x576ab0 (69/160), 0x576b50 (8/80), 0x576ba0 / 0x576d40 / 0x576ee0 (277 .. 285/304, first +0x5: the original
  reuses one register for the record kind chain), 0x576cd0 / 0x577020 (22/128), 0x576e70 (23/112), 0x577110
  (93/160), 0x5771b0 (98/128), 0x577230 (30/96), 0x577290 (44/64), 0x5772d0 (67/80), 0x577330 (45/192),
  0x5773f0 (34/112), 0x577450 (57/64), 0x5774d0 (72/112), 0x577540 (83/128). They are one family: the campaign
  step / transition callbacks, all reading the step int[4] and the state record 0x5eec60; matching them later is
  one codegen question, not many.
- 0x577690 (176 B, ROUGH): a save-record builder; the record is a 0x114-byte local, its field offsets are taken
  from the disassembly (the kind bytes +2 of the records 0x770b80 + i * 0x21c: 6 -> +0xc, 7 -> the id list +0x34),
  the type is unknown. Nonmatching 83/176. The next start 0x577740 follows without nop padding (the scan misses
  it; 0x577740 is the first function of the "FT" tutorial literals, "tutorial\main").
- 0x577740 (214 B) matched first try: the tutorial start, the first user of the "FT" literal block ("tutorial\main"
  0x629304, the text 0x624204), so the unit's plausible boundary holds up to here. 0x577820 (48 B) and 0x577850
  (96 B) also matched first try. 0x5778b0 (326 B, ends 0x5779f5) drafted, nonmatching 202/320.
  0x577a90 (203 B) drafted, nonmatching 156/208. 0x577b60 (48 B) matched first try. Next undrafted: 0x577b90.
- Ghidra's starts merge several functions here (0x5764b0 records 1776 B = 16 functions; 0x577330 records 1040 B =
  8); a ret + nop byte scan found them, but it misses a start that follows a `jmp` + nop (no ret): 0x577230 (after
  the tail call of 0x5771b0) and 0x576a80 (after 0x5769c0) were found by reading the disassembly, not by the scan.
  Check every `jmp` + nop run in a new stretch by hand.
- @bytes 0x5770d3..0x577110 (nop fill, declared in the unit header).
- he.c side: he_Unk575710 is drafted but nonmatching (416 B compiled vs 288 B original: the original keeps the
  mask in the hoisted eax and uses byte ALU `or al 1` / `or ah 1`; our int lvalue reloads and uses 32-bit ops).
  A cast to unsigned short at the lvalue did not change the codegen (tried, uv run try).

## layout 2026-10-23
Session 16 (this session, in a worktree: the main checkout was busy with another session). Drafted 0x575710 into
he.c and created this unit with 0x575860 (ROUGH skeleton) and 0x576020 (matched, clears 0x6f2660).
Later in the same session: 0x576030 (matched, sets 0x6f2660), 0x576040 (the state step, 763 B, nonmatching),
0x576340 (the callback caller +0x3c, nonmatching), 0x5763c0 / 0x576410 (the run mask +0x68 / +0x6c, matched),
0x576460 (mode 2 / state 5 query, matched) and 0x576490 (a missed Ghidra start: the step callback predicate on
the kinds 0x19 / 0x1b / 0x29, matched; it is the tail of Ghidra's 80 byte "0x576460"). Then 0x5764b0 ..
0x577540 (47 functions, see Open work): the command / fight / building event callbacks of the campaign state
machine, keyed by the step kind and the building table 0x7e3f30 (0x24d bytes per kind, byte 0x12 / 0x2 / 0xa / 0xd
= the building state), the player word 0x6f25b8 and the fight record 0x6f2310; several are transition callbacks
(return 1 / 3) that drive the state record 0x5eec60 (states 4, 9, 0xa, 0xb and the flag +0x38).

## layout 2026-10-23 (session 17)
Frontier kept going from 0x577b90 to 0x578570 (the tutorial module at the end of the unit). Drafted:
0x577b90 (the second text window S+0x34, the sibling of 0x577a90; nonmatching 9/208: ours loads S->0x18 before
the push of -1 where the original pushes first, and the eax/ecx choice in the `if (S+0x14)` — the original keeps
S in eax there), 0x577c60 (matched first try: clears the second window S+0x34), 0x577cd0 (matched first try: the
progress form "tutorial\progress" (0x629314) into S+0x68 and the slider S+0x6c via d2_WinOpenSlider 0x46a960
(x, 5, 400, 0, 400, 0x75, 0x42, ai_Unk6ed938), x = `*(short *)(0xf8f128 + i * 0x3c4) / 2 - 200`), 0x577d60
(a Ghidra-missed start, matched: releases the progress form S+0x68), 0x577d90 (the progress slider, state 0xa,
nonmatching 93/128: the MIN(TDIFF ? TDIFF : 0, total) shape — the original compiles the `?:` twice and uses
`fild DWORD` (signed) while the compare is unsigned (`jae`); ours CSEs the subexpression and/or converts
unsigned), 0x577e50 (the chapter form updater, 632 B, nonmatching 537/640: the four forms
tutorial\{left,right,top,bottom}_form 0x629368 / 0x629354 / 0x629340 / 0x629328 by the step kind, the step
table r+0x58, 0x10 B per step: +4 kind, +8 text, +0xc a window value; ROUGH but complete), 0x5780e0 (nonmatching
8/64: only the operand load order — ours loads 0x5fe710 before S, swapping the C operands makes it worse (25/64)),
0x578120 / 0x578150 / 0x578170 (matched first try), 0x578180 (the tutorial start, 493 B, nonmatching 361/496:
the 5-entry table pair 0x62918c (ints, 1 = enabled) / 0x6291a0 (callbacks), the chain built by 0x5784a0 and
0x57b750, then the whole state record 0x5eec60 reset; the field stores are right, the loop/codegen differs),
0x578370 (the tutorial stop, nonmatching 10/208: only the scheduling of the `mov ecx, ds:0x5eec60` against the
store `0x6f2658 = 0`; swapping the two C statements makes it 11), 0x578440 (matched first try: the recursive
record free), 0x5784a0 (the main-intro record alloc, nonmatching 145/160), 0x578570 (the chapter 1 chain,
2254 B, ROUGH: eleven 0x74-byte step records, generated mechanically from the disassembly with every constant —
the names FTInitChapter1Intro / Step1 / Step3 .. Step10 / Outro, the texts CHAPTER_1_INTRO, CHAPTER_1_A_MAIN ..
CHAPTER_1_J_MAIN, CHAPTER_1_OUTRO, the ids 0x3f2a .. 0x3f51, +0x3c = 0x576490 (the step predicate), +0x68 the
chain flags, the sound TUTORIAL_CHAPTER_1.sbf into 0x6f2644; nonmatching 1318/1424: our code is shorter than
the original, probably the memset inline and the store scheduling).

The unit is the tutorial module after all: 0x578180 (called from main.c 0x53bb20) initializes the state record
0x5eec60 that the end-of-game tick 0x575860 reads; the "FT" names, the tutorial\* forms and the TUTORIAL_*
texts all belong to it. The name is still unknown (no leaked tag / "xxx:" string), so @named_by stays "-".
Next: 0x578e40 (1839 B, data ptr x1).

Globals found this session: 0x6f2658 (the first record of the chain), 0x6f2640 / 0x6f2644 (sample banks:
TUTORIAL_INTRO_OUTRO.sbf / TUTORIAL_CHAPTER_1.sbf; 0x6f2640 .. 0x6f2658 = six handles freed by 0x578370), the
step record chain (0x74 B each: +0 kind, +4 magic, +0xc text id, +0x10 text key, +0x2c = -1, +0x30, +0x3c the
callback, +0x48, +0x58 / +0x64 tables freed by 0x578440, +0x68 the chain flags, +0x70 the next record). The
state record gains: +0x24 / +0x34 the forms inline_form / aux_form, +0x40 the step, +0x44 the chapter kind,
+0x48 the chapter form, +0x4c the current control, +0x60 / +0x68 / +0x6c = -1.
Later in the same session, the chapter chains (the same record builder shape, all ROUGH from the disassembly and
generated mechanically with every constant): 0x578e40 (chapter 2, 1839 B, 9 records, sound TUTORIAL_CHAPTER_2.sbf
into 0x6f2648), 0x579570 (chapter 3, 5377 B, 18 records incl. the script records FTInitScriptChapter3Step*, sound
TUTORIAL_CHAPTER_3.sbf into 0x6f264c), 0x57aa80 (chapter 4, 1072 B, 5 records, TUTORIAL_CHAPTER_4.sbf into
0x6f2650), 0x57aeb0 (chapter 5, 2207 B, 9 records incl. FTInitAnimChapter5Step2, TUTORIAL_CHAPTER_5.sbf into
0x6f2654), 0x57b750 (the main-outro record, kind 7, magic "RTUO", "FTInitMainOutro" / TUTORIAL_MAIN_OUTRO);
0x57b810 (the cubic Bezier of two floats, u = 1.0f - t, out = u^3 a + 3 u t (u b + t c) + t^3 d; the constants
1.0 0x5eb780 / 3.0 0x5eb790; nonmatching 91/144, the term order differs). The six handle slots 0x6f2640 /
0x6f2644 / 0x6f2648 / 0x6f264c / 0x6f2650 / 0x6f2654 hold the chapter sample banks; 0x6f2658 is the record chain.
The chapter-3 alloc names contain a copy-paste: its first record is tagged "FTInitChapter2Intro" (0x62998c) in
the original too. Next: 0x57b8a0 (910 B, the tutorial per-frame update: r+0x64, r+0x5c, the callback r+0x60 with
the argument [esp+0x18], the -1 / range check, S+0x58 = 2, S+0x5c = the clock, S+0x54 = the value, then 0x472f50).
Then 0x57b8a0 (910 B, the tutorial per-frame update, ROUGH from the disassembly: the step callback r+0x60 called
with the argument, the value rules (-1 -> 2, a valid step index -> phase 2 / S+0x5c = clock / S+0x54), 0x472f50
and 0x475aa0 on the position object S+0x60, then by value: 0 -> 0x46cd80 / 0x46cda0 from the step entry (0x2c B:
+0 / +4 the position, +0x18 / +0x20 / +0x24 / +0x28 the timing and the Bezier points), 1 -> the phases S+0x58 =
2 / 3 / 4 (the Bezier 0x57b810 into 0x46cda0), 2 -> 0x46e710 and the object released), 0x57bc30 (168 B, the info
block 0x75e5a0 .. 0x75e5f4: the bytes 0x4b / 0x18 / 0x10 / 0x33, the dword 0x59a, 0x4fb770 (0x624ee0) + 0x639,
two strings copied from 0x1449e48 / 0x1449e4c, 0x503ee0 (the local, the byte 0x75e5a3)) and 0x57bce0 (541 B,
ROUGH SKELETON: the chapter window 0x62a248 with the five chapter texts 0x3f1b + i (0x6024f8) and the window
table 0xf8f080 (+0x48 = 1, +0x28 = the chapter table 0x62918c[i], +0x1c0 &= -3), the two extra texts 0x3f20 /
0x3f21, the pump 0x5309a0 (0x57bce0, 0x86); the input / selection tail is comments). Next: 0x57bf00.
After 0x57bce0 the frontier went on to 0x57bf00 (the results window, ROUGH SKELETON: the three player records
0x4ec2d0 / 0x4ec320, the byte 0x6f1bc5, then the window / controls / 0x57c560 / the pump 0x5309a0 - the body
after the first loop is comments) and 0x57c560 (the result string writer, ROUGH SKELETON: 0x4eaa50 (arg 1),
0x521040 (the user record 0x770b80 + U16(0x6f25b8) * 0x21c, arg 3, the byte 0x6f1bc5), the text 0x144a2e0 via
0x521150, the 0x5176d0 (buffer, 0x4044, 0x144a7a8[+0x10] / 0x144a80c[+0x14]) string; the tail after 0x4ec490 is
comments). Next: `uv run ledger next`.

## layout 2026-10-23 (session 18)
Frontier went from 0x57dc50 to 0x583700 (31 functions, the tutorial's location / contact windows). The data-order
report put every start into the unit's own literal run (0x62a2a4 -> 0x62a2c4 -> 0x62a350 -> ... -> 0x62a580), so no
new boundary; check went 2399/4019 -> 2403/4050 (four new matches).
- Matched first try: 0x57f5d0 (three 0x458810 vector copies), 0x580a50 (the 0x6f9f60 shot table reset, 100000.0f
  per entry), 0x580fe0 (the 0x56f170 scene check), 0x5830f0 (the entry name line "%s %s %s %s" 0x623594 / "%s %s"
  0x60c7b8; the German / English text pairs at +0x253 / +0x27b).
- Near-misses: 0x57fdb0 (11/32: the pitch as 0..1; a double-constant variant gave 12/32), 0x57e230 (79/80: VC6
  turns our copy loop into `rep movs`, the original keeps the indexed loop and reloads src[0] every iteration -
  a source shape that prevents the memcpy recognition is still open), 0x57dc50 (196/528: our message record sits 4
  bytes higher on the stack, scheduling of the index scaling; the section-end nops), 0x5823b0 (114/448), 0x582560
  (127/176), 0x582600 (123/320).
- ROUGH skeletons to fill from the disassembly: 0x582b40 (0x473650 / 0x5830f0 / 0x583150 item widgets, the detail
  texts 0x62a54c / 0x62a564 / 0x62a544 / 0x62a53c), 0x583240 (the graveyard window's click dispatch and the
  redraw flags; "Locations_Friedhof_Graves" 0x62a580), and the earlier ROUGH ones 0x57e270 (the cannon scene, 2305
  B), 0x57f0e0 (the camera update), 0x580160 (the explosion spawn - only the selection-0 spawn is written).
- 0x57e270 is the cannon scene: the four scene objects 0x446e10 ("KANONE_*" 0x62a310 .. 0x62a350) and the wimpel
  "AO_KANONE_WIMPEL_Aa" 0x62a2fc, the window table scan (0xf8f080, 0x300 entries of 0x2ec, against the list
  0xfb11c0 stride 0xab), the ball table 0x6f9f60 (3 records of 0x38, 3 entries of 0x10: the object, a flag, a
  value) reset by 0x580a50 and read by 0x57f690; 0x57e230 fills the table from 0x570... (called by 0x4eeee0).
- The contact window family (0x5821d0 .. 0x583240) is one shape: 0x52e190, the pump 0x5309a0 (mask 0x67fff), the
  contact ids 0x52e1c0 ("contact_*" strings 0x62a434 .. 0x62a524 with the texts 0x1441f90 .. 0x144a2f8), the
  clicked contact 0x6f22e4, the filter 0x6f231c bits 0x200 / 0x400, and the window list: 0 the window, 2 / 3 the
  screen part, 4 / 5 / 6 the fonts, 0x4d the selected entry, 0x4e the detail window, 0x50 the items of 3 dwords,
  0x51 their widgets. The spy windows 0x581020 / 0x581400 / 0x581950 / 0x581db0 share the message type 0x99
  (0xf8 bytes, the amounts at +0x58, the time at +0x28, the character at +8, the record + 1 at +0xc / +0x5c, the
  value from 0x4a63f0 at +0x58) - a first AIMSG view for this unit.
- Ghidra loses the FPU arguments of 0x5de518 (__ftol): 0x581020 (three amounts), 0x581400 (six), 0x581950 (none),
  0x581db0 (one) and 0x582b40 (0x4103) - the drafts use 0 and say so.
- Type debt: the message records are written by offset (`*(unsigned char *)(msg + 4) = 0x99`); ai.h's AIMSG
  (0xf8) is the type. The entry records of the contact lists (0x4ef090 / 0x4ef170 / 0x4ef6b0 / 0x4ef7c0; +0 the
  id, +4 the sub-entries of 0x44 bytes) are untyped; the "0x50-byte query" of 0x5c1130 is 1 / 0x400 / 6.
- Continuation (same session, 10 more minutes): frontier 0x583700 -> 0x584410. Drafted 0x583700 (the graveyard
  detail panel, ROUGH, the 0x582b40 sibling with the window list at 0 / 2 / 4 / 7 / 0xa / 0xb / 0xc), 0x583b70
  (matched first try: opens "Locations_Friedhof_RentAGrave2" 0x62a59c and calls 0x583700), 0x583c50 (the four
  buttons, the 0x582730 shape; 342/400), 0x583de0 (a confirmation window, 72/288), 0x583ef0 (the buttons by the
  state record, 247/272 - the prologue differs), 0x584010 (the rent confirmation, 507/624) and 0x584260 (the
  RentAGrave window, 132/448). Matched this continuation: 0x583b70. 0x582600 was moved before 0x582730 (it was
  out of address order).

## layout 2026-10-23 (session 19, worktree)
Frontier went from 0x584410 to 0x586ab0 (17 functions, the graveyard contact family of the
"Locations_Friedhof_*" windows; the unit extent is now 0x586ab0). Check 2405/4057 -> 2406/4074 (one new match:
0x5867c0). Data order (ledger data-order 0x584410): the literals climb 0x62a5bc -> 0x62a62c -> 0x62a634 ->
0x62a664 -> 0x62a680 -> 0x62a6dc -> 0x62a70c in function order, one run, no new boundary.
- Matched first try: 0x5867c0 (the "Schlag" action: 0x587c40 / 0x4c4b00 / 0x52b490, +0xc8c = 1). It is called
  with an (unused) argument, so its definition takes a char * and ignores it.
- Drafted nonmatching (calls, control flow and globals complete): 0x584410 (the two-button confirmation,
  229/272: our prologue allocates the frame late and spills btns[1] differently), 0x584520 (the grave action,
  471/656), 0x5847a0 (the rent-a-grave selection, 119/352), 0x584900 (615 B, Ghidra MISSED the start; the
  message type 0x8d), 0x584b70 (501 B, Ghidra MISSED the start; the "Gehilfen erschaffen" window: 0x4f1d50
  limits, 0x4f1170 raise, the script LOCATIONS\FRIEDHOF\Gehilfen_erschaffen_FRIEDHOF.esc "sp_%u"), 0x585380
  (the "Fog" window, two sliders, message 0x90), 0x585750 (the Seance selection -> the out record 1 / 2 / 3),
  0x5859c0 (the Seance2 ritual: 0x585c40 formulas, the .esc script, the two modes), 0x585c40 (the formula
  filler, 64/96), 0x585cb0 (the seance outcome, message 0x8e, the "GEIST_KOMMT" news), 0x585f30 (the seance
  sacrifice: 0x50a640 * 0.25f clamped to 160000 .. 800000), 0x586200 (the seance ghost lottery: the 15 items
  0x2b8 .. 0x2b9 through 0x4a5760), 0x586560 (the selection window -> 0x585750), 0x586610 (the seance driver:
  0x583b70 then 0x586560, dispatch to 0x585cb0 / 0x586200 / 0x585f30), 0x586840 (the contact dispatcher: the
  nine contacts, the key 0x1468410 one-shots 0x1e / 0x2f / 0x22 / 0x23 / 0x19 / 0x16 / 0x11, the clicked-contact
  dispatch, 0x5912c0 each frame).
- ROUGH (the structure is there, the details are summarised): 0x584d70 (1549 B, the "HANDEL\HANDEL_SUCHEN"
  helper window: two or three grave kinds from the building table 0x7e3f30 + kind * 0x24d, the 0x8c event counts,
  the button texts by count, the message type 0x8c) and 0x586200 (the 15-item lottery; the 0x4a5760 record array
  layout was copied from am.c's use).
- Near-misses worth a second look: 0x586560 (88/176: the inner entry loop / the w[0x22] store), 0x5847a0
  (119/352), 0x584410 (229/272, the frame layout), 0x585cb0 (213/640 after the float prototypes were corrected -
  the earlier 440/656 came from declaring 0x4f2660 / 0x42ec20 int).
- Type debt found: 0x573200 takes THREE arguments (a, the compared dword, the compared byte; the disassembly
  reads [esp+0x10] / [esp+0x14]). he.c's draft declares it with two and compares HM_I(r, 0xb0) == a instead of
  == b. See notes/layout.md, Type debt.
- New map of the seance texts: 0x4098 / 0x4097 / 0x4099 (0x586ab0), 0x40a4 / 0x40a3 / 0x40a5 (0x586e40),
  0x40b6 / 0x40b7 / 0x40b9 / 0x40ba / 0x40be / 0x40c8 / 0x40cc (0x585380 / 0x5859c0), 0x40ce / 0x40cf / 0x40d0
  / 0x40d1 / 0x40d2 (0x585750), 0x40d3 / 0x40d7 (the seance outcomes), 0x40ef / 0x40f0 / 0x40f1 / 0x40f2 /
  0x40f3 (the ghost branches), 0x4107 (the entry name), 0x4109 / 0x410b / 0x410c / 0x410d (0x584520), 0x4113 /
  0x4114 / 0x4115 / 0x4116 / 0x4119 / 0x411b (the rent / grave actions), 0x4120 / 0x4121 (0x584900), 0x4128 /
  0x4129 / 0x412a / 0x4131 / 0x4132 / 0x4133 / 0x4135 (the helpers / action windows). The news sender of the
  seance outcomes is 0x4c2760 (4 args), that of the others 0x4c27d0 (5 args, last 0xfa); the pump masks are
  0x677c7 (windows), 0x743c7 (0x5859c0), 0x777c7 (the seance outcomes) and 0x67fff (the contact dispatcher).
- Literals of this session: 0x62a5f8 (the Gehilfen script), 0x62a62c ("sp_%u"), 0x62a634
  ("HANDEL\HANDEL_SUCHEN"), 0x62a64c ("Locations_Friedhof_Fog"), 0x62a664 ("_NACHRICHTEN_FRIEDHOF_14"),
  0x62a680 ("Locations_Friedhof_Seance"), 0x62a69c ("%SEANCE_FORMEL_PART_%i_0_%i"), 0x62a6b8
  ("LOCATIONS_KRYPTA_SEANCE_KRYPTA_e"), 0x62a6dc ("Locations_Friedhof_Seance2"), 0x62a6f8
  ("GEIST_KOMMT_NICHT"), 0x62a70c ("GEIST_KOMMT"), 0x62a718
  ("locations/friedhof/schlag_FRIEDHOF.esc"), the contacts 0x62a740 .. 0x62a7e0 (contact_GRAB_MIETEN ..
  contact_ALTERN_LASSEN); the float constants 0x5eb91c (0.1f), 0x5eb8b0 (0.2f), 0x5eb7b8 (0.4f), 0x5eb79c (0.25f),
  0x5eb780 (1.0f); the data 0x144a498 / 0x144a4f0 / 0x144a514 / 0x144a638 / 0x144a63c / 0x144a40c / 0x144a428 /
  0x144a620 / 0x144a624 (the runtime string pointers), 0x6f2448 (the contact gate), 0x1468410 (the key one-shot),
  0x6f231c (the contact filter, bit 0x200 / bit 7), 0x6f22e4 (the clicked contact), 0x6f22f8 (the grave object),
  0x146738c, 0x587c40 / 0x587c90 / 0x5871c0 / 0x586ab0 / 0x586e40 / 0x587300 / 0x587340 / 0x587370 (the handlers).
- Next: `uv run ledger next` = 0x586ab0 (902 B, Ghidra), the HEIMSUCHUNG handler. Decoded but not
  drafted (the timer): the 0x5c1130 query (1, 0x400, 6, U_I (0x144a414)), the player bar 0x525220, the window,
  the texts 0x4098 / 0x4097, a slider 0x475cb0 (4, 0xc, 4) whose value 0x475980 is paid through 0x520f90, the
  entries 0x4099 / +1, the first selected character (byte +0x188) scanned as in 0x584900, the message type 0x88
  (+8 the player's character word, +0xc rec + 1, +0x28 the clock, +0x36 = 2, +0x38 / +0x60 the found character
  + 4, +0x58 the building record's value (0x4a63f0 fallback), +0x5c the query +4, +0x64 the slider value), the
  0x521150 (U_I (0x144a40c), 0) when no character, 0x521150 (U_I (0x144a428), 0) after the send. Then 0x586e40
  (the same shape, 0x40a3 .. 0x40a5, U_I (0x144a498)), 0x587300 / 0x587340 / 0x587370 / 0x5871c0 / 0x5871e0 /
  0x587c40 / 0x587c90 and 0x5912c0.

## layout 2026-10-23 (session 20, worktree)
Frontier went from 0x586ab0 to 0x589aa0 (30 functions; the unit extent is now 0x589aa0). Check 2406/4074 ->
2413/4103 (seven new byte matches: 0x5871c0, 0x587300, 0x587340, 0x5876a0, 0x587de0, 0x589400, 0x589410).
Data order: no new literals in this stretch except the ones the drafts below name (0x62a8f8 .. 0x62a9f8,
0x62aa14 .. 0x62aa38); the block keeps climbing in function order (`uv run ledger data-order 0x586ab0`),
one .data block, no new boundary.
- Matched first try: 0x5871c0 (the GRAB_MIETEN contact: 0x583240 (0, rec, U_I (0x144a5d0), 0, 0), 0x52d7a0), 0x587300
  (the GRAEBER_PFLEGEN contact: 0x520fe0 gate, 0x583240 with the flag 1 and cb 0x584900), 0x587340 (the
  GEHILFEN_ERSCHAFFEN contact: the same with cb 0x584b70; its 0x52d7a0 is OUTSIDE the if - the original shares
  the epilogue), 0x5876a0 (the seance contact dispatcher: one contact 0x62a83c, 0x586610), 0x587de0
  (memset 0x6f5f60, 0x4000, called from main.c 0x532160), 0x589400 (a 12-byte slot's +4 / +8 = -1) and 0x589410
  (the init of the 12-byte slot table 0x6f5ec0, 0xb slots, the last key 0x57 at 0x6f5f38).
- Drafted nonmatching, structure complete: 0x586ab0 (the HEIMSUCHUNG contact, 902 B: the 0x5c1130 query
  (0x144a414), the window 0x62a5bc, the texts 0x4098 / 0x4097 / 0x4099, the slider 0x475cb0 (4, 0xc, 4) on the
  0x4098 button, the message type 0x88 (+0x64 the slider value, +0x58 the target's level, +0x5c the target +4,
  +0x38 / +0x60 the first selected character +4), 0x521150 (U_I (0x144a428), 0)); near-miss 425/928: the frame
  is 4 bytes larger than the original's 0x158 (one stack slot too many; the original overlaps v and lv at F+0)
  and the q / msg store order still differs. 0x586e40 (the ALTERN_LASSEN contact, the same shape, the query
  0x144a444 (NOT 0x144a498 as the earlier note said), the texts 0x40a4 / 0x40a3 / 0x40a5, type 0x89, +0x36 1,
  +0x64 4, the line 0x40a7 in 0x5176d0 and 0x521150 (the line, 0)); 704/912, register allocation mainly.
- 0x5871e0 (the rent-a-grave handler: 0x4ef7c0 (the character, rec, 0, 0x4ef740, 0xff), without an entry
  0x4f25d0 (the character, &bld, &p) -> 0x5847a0 or 0x521150 (U_I (0x144ada0), 0); with it the record w (+0x1c p,
  +0x20 *p, +0x24 the 0x4ef6b0 flag), the line 0x4107 (0x5eecd0[*p]), 0x583b70 (w, the line), 0x584410 (rec, w),
  0x4758f0 (w[0])). 0x587370 (the VERSICHERUNG contact, type 0x8a, the texts 0x40f6 / 0x40f5 (0x228) / 0x40f7,
  +0x38 / +0x58 the selected character +4). 0x587560 (the trade contacts: 0x62a818 / 0x62a450 / 0x62a808 /
  0x62a7f8 / 0x62a464 -> 0x58bd60 / 0x5af1a0 / 0x5c3200 / 0x5ad150 / 0x5b26b0). 0x587700 (a grave-detail message:
  0x4ffe90 (rec + 0x5d, 1, 0, 0x211), type 0x8f, +0x58 the character +4, +0x5c sel, +0x60 the 0x4ffe90 +2 or -1).
  0x587810 (the grave-detail window: 0x4fe350 (0x770ae0), the 0x8f event gate 0x53e400 (2, 0, 0x8f, 3, rec + 1),
  the line 0x413b / 0x413c, the window 0x62a84c, the texts 0x4138 / 0x4139 (0x228), the buttons by
  0x504ec0 (the character, 1 / 2 / 4) < 1.0, 0x587700 (rec, the character, sel)). 0x587ad0 (the fog / vermin
  contacts 0x62a88c / 0x62a870 -> 0x587810 / 0x585380). 0x587b70 (the building flag mask, called from f3gm.c
  0x50359b: 0x81 / 0x10bd, 0x4461, 0x2200 (a byte of the six at bldg + 0x223 == 0x5044b0 (the character word,
  0x5048e0 (kind))), 0x5000, -1 with 0x6f2448; the building table is the POINTER U_I (0x7e3f30), not its
  address - fixed here, 0x584d70 still has the address form, see Open work).
- ROUGH SKELETONs (the calls, ids and globals are on the disassembly, the widget arithmetic is summarised):
  0x587e00 (the item drag-and-drop: the release via 0x4758f0 / 0x4113f0, the target 0x6f22e8, the widget user
  pointer 0xf8f080 + U_I (0x5fe710) * 0x2ec + 0x2e8 classified into a character (0x770b80 .. 0x7d5f80), the goods
  (0x889f74 + 0xa900), the buildings (0x7e3f30 + 0xca78 = 0x58 * 0x24d), the 0x7e3f34 table (+0x14f218), the item
  table 0x91aaf0 (+0xb9dc, / 0x41) or the raw ranges 0xdf .. 0x427 / 0x427 .. 0x47f; the windows 0x588310 (the
  kinds 0x17 / 0x20 / 0x25 / 0x27, else 0x588950), 0x588160, 0x588bb0, 0x588c60; 0x62a8cc / 0x62a8d0 / 0x62a8d4,
  0xfb1024 + win * 0x2ac, 0x6ed9d4 / 0x6f2670, 0x4741a0). 0x588160 (the goods drag window 0x62a8d8: the 0x1c-byte
  record from the globals 0x62a8f4 .. 0x62a908 / 0x631090, the texts 0x27 / 0x2a / 0x28 / 0x29 and
  0x4731d0 (0, 0, idx + 0x427)). 0x588310 (the item drag window "ToolTip\ToolTip_Objekte_Waffen" 0x62a940 or
  ..._Handelsgut 0x62a91c, the texts 0x1f / 0x20 / 0x21 / 0x22 / 0x23 / 0x25 / "%s$N" 0x62a90c, the kind cases
  0x1b7 .. 0x2b0 and 0x225 .. 0x295, the slot values 0x18 .. 0x50). 0x588950 (a Ghidra MISS: 0x588970 is the body,
  the 0x1d guard falls through: the window "ToolTip\ToolTip_Objekte_Upgrade" 0x62a984, the texts
  idx * 2 + 0x9a8 / + 0x9a9, 0x21 / 0x23 (0x5087f0 (idx, 0x64) * (0x6f23fc * 0.25 + 0.5)), 0x20, 0x24 and the
  0x40 words at bldg + 0x23 & 0x7fff, the bytes +0x1a3 / +0x1e3, 0x62a978 / 0x62a96c / 0x62a960). 0x588bb0 (a
  named record's window 0x62a9b4: strcpy / 0x5ea986 / 0x5de652 (0x62a9cc) / 0x42e770, near-miss 17/176 - the
  frame is 0x180 in the original, ours 0x188). 0x588c60 (the character drag window "ToolTip\ToolTip_Personen"
  0x62a9f8, 0x5be1d0 / 0x5be720, 0x505e30, the texts 0x2b .. 0x41, 0x576040 (7, ch, 0, 0)). 0x5890c0 (the same
  window for the kinds below 0xa, 0x520f20, the entry loop 0..0x4d step 0xf). 0x589450 / 0x5894d0 (the slot
  validation and the key selection: 0x502020 / 0x4fe980 / 0x503530 (the player word, id, e, 0), 0x521960 (id,
  e + 2 or -1, 0x631090), the keys 0x1468410 / 0x14681ea). 0x589570 (the tooltip line of slot idx: a = 0x6f2308 /
  0x6f230c / 0x75ec60 / 0x6f1bc8, b = 0x6f2310, "%s = %G, %s" 0x62aa14 / "%s = %G" 0x62aa20, 0x52e330).
  0x589670 (the item-slot window 0x62aa38, the text 0x205b, the pump 0x657c7, 0x589450 / 0x5894d0 / 0x589570 /
  0x589400, 0x46c600 / 0x46c6c0, 0x46b70, 0x475ff0 / 0x52e860 / 0x52e350).
- Open work: (a) 0x586ab0's frame is 4 bytes too large and its q / msg store order differs - the original's
  source order is memset(q), the -1 fill, then q[0] / q[1] / q[0x10] (already reflected) and the msg stores in
  the order +4, +0x36, +0x64, +0xc, +8, +0x2c, +0x34, +0x28, +0x30, +0x5c, +0x60, +0x38, +0x58; the extra slot
  must be found (v and lv overlap in the original). (b) 0x584d70 uses `(char *)0x7e3f30 + b[0] * 0x24d` - the
  table is the POINTER U_I (0x7e3f30) (0x587b70 fixed); check 0x584d70 and 0x588950 / 0x588160 for the same slip.
  (c) the ROUGH skeletons 0x587e00, 0x588310, 0x588160, 0x588950, 0x588bb0, 0x588c60, 0x5890c0, 0x589670,
  0x589570, 0x589450, 0x5894d0 are far from matching; their details are in their address comments. (d) the
  0x5c1130 query struct of 0x586ab0 / 0x586e40 still carries the -1 fill as a loop (8 dwords).
- Next: `uv run ledger next` = 0x589aa0 (142 B, a call from 0x5190b0), then 0x589b30, 0x589bc0,
  0x589c90, 0x589cf0, 0x589e00, 0x589e50 (184 B, a call from 0x53bb20 = main.c), 0x589f10 (2595 B).

## layout 2026-10-23 (session 20, continuation)
Continued past 0x589670: drafted 0x589aa0 (142 B, the slot table load: 0x430e30 (&n, 4, 1, fh) with n = 0xb,
then each slot's byte +0 / dword +4 / dword +8; 0 on a short read, 1; 23/144, the frame / loop order),
0x589b30 (136 B, the same over 0x51c920 with n = 0 read first; 20/144), 0x589bc0 (198 B, a screenshot: 0x52e330
(0x631090), the pump 0x5309a0 (0, U_I (0x760638)), the name 0x62aa58 with the counter 0x6f2674, 0x4223c0 (the
screen 0xfb9072 * 0xfb9070 * 3, 0x62aa4c), 0x41dec0, 0x478c50, 0x41e050, 0x401b90, 0x422620, 0x52e330
(U_I (0x144adb4))) and 0x589c90 (86 B, the slot keys each frame: U_B (0x1468410) and U_B (0x14681ea), the key
0x58 -> 0x589670, 0x5894d0 and 0x589570 (i) per matching slot; 62/96, the loop detail). The unit extent is now
0x589cf0. Next: 0x589cf0 (264 B, a pump callback; Ghidra merged it into a 311837 B record), then 0x589e00,
0x589e50 (a call from main.c 0x53bb20), 0x589f10 (2595 B).

## layout 2026-10-23 (session 21, worktree)
Drafted the unit's tail 0x589cf0 .. 0x589e50 and closed it at 0x589f10 (the network lobby is a unit of its own,
see notes/units/unk_589f10.md):
- 0x589cf0 (the speed keys, 264 B) matched first try: the game speed 0x6f3608 in steps of 40 (v = (int)(speed / 40
  + 0.5f), the magic multiply + __ftol 0x5de518), the keys 0x4e / 0x4a through 0x4c3f20 / 0x4c3f50 (raise / lower
  0x60bd84) and 0x5d5e90 (the speed apply: 0x5f1290, 0x410610, 0x623814), the mode byte 0x6f23f8 (bit 4 = network,
  bit 0x10), the screenshot key 0x1f (0x589bc0) and 0x5d7430 for the key 0x10; the tail calls are plain
  `return f();` (the original pops esi / adds esp and jumps).
- 0x589e00 (the slot fill, 79 B) matched first try: the table 0x6f5ec0 + i * 0xc gets +4 = the building id
  (the dword at +1 of the record) and +8 = the main object id (0x501b50, else 0x501ce0; +2 of the object; -1
  for the building kind 0x1d). The offset must be a local `off = i * 0xc` (VC6 then emits the lea / shl once);
  two separate `i * 0xc` expressions give two multiplies and 96 B.
- 0x589e50 (the building slots, 184 B) drafted, nonmatching 43/192: the slots 0 the market 0x6f1bc8, 1 the town
  hall (0x501190 (1, 5, 0xf)), the next the building at +0x170 of the first character of the kind 6 (the records
  0x770b80 + i * 0x21c, at most 0x300), then from slot 3 the player's buildings (0x501190 (1, 4, i) /
  0x501030) whose kind record (0x7e3f30 + kind * 0x24d) byte 0 is not 2. ONLY the kind-record address block
  (7 asm lines, first diff +0x81) differs: the original computes esi = base + kind (`add esi,edx`) and
  ecx = 147 * kind (`[esi+ecx*4]`); ours computes 589 * kind into one register and loads the base late
  (`[edx+ecx]`). Variants tried (all identical code): `((unsigned char *)U_I(0x7e3f30))[..]`, a char* cast, an
  int cast of the kind, `+ 0`, a kind local, an offset local, a pointer local, addend swap, constant first,
  do-while, unsigned / char types. For the match session: the cause is outside the block (register weights);
  try c2prio / a candidate in the block.
- Extent: the literals climb to 0x62aa58 ("gamedata/screenshots/gilde%04i.bmp", 0x589bc0); the next block
  starts with the globals 0x62aa7c .. 0x62aa88 and "menu
etlobby" (the lobby).

## match-base 2026-10-25 (base-4 worker)
- Reference-only renames (no code change; every use updated, build/link/ledger clean): ai_Unk4c3ae0 -> cm_GetSlotState (0x4c3ae0) and ai_Unk4767d0 / ai_Unk476860 -> d2_StartFade / d2_ReleaseFade (0x4767d0 / 0x476860, the old ai_ aliases folded into the d2 names). See notes/units/unk_5d33b0.md for the whole rename list.

## match-base 2026-10-26 (base-4 worker, shard 4/10)
- Reference-only renames from the tailor-contact work (no code change, every use updated, build/link/ledger
  clean): gv_FindAccused (0x4f41b0, the unk_Unk4f41b0 / ai_Unk4f41b0 aliases folded), ai_ProductionWindow
  (0x58bd60), ai_StorageWindow (0x5b26b0), ai_MasterLetterWindow (0x5c3200), ai_AmountMenu (0x5c1130).

## play 2026-10-28 (the tutorial start, session 3: the state record and the dialog frame)

- **The state record pointer.** `S` was `*(char **)((unsigned)&ai_Unk5eeab8 + 0x1a8)`: the original's 0x5eec60
  pointer spelled relative to sv.c's 0x5eeab8 const array. The full link puts 0x5eeab8 elsewhere, so +0x1a8
  read a neighbour and every write went to a garbage pointer (crash at `ai_Unk578180+0xba`, `[eax]` with
  eax=0x44228000). 0x5eec60 is a **const pointer to 0x6fa060**, the record inside main.c's 0x6fa020 block
  (0x6fa020+0x40). Now `char *const ai_Unk5eec60 = (char *)full_data_6fa020 + 0x40;` (0x5eec60) and
  `#define S (*(char **)(unsigned)&ai_Unk5eec60)`: a memory load exactly as before, so the unit keeps its
  byte grades and the link relocates the pointer.
- **The chapter table** (main.c) was the five chapter callbacks as the original's raw addresses
  (0x578570/0x578e40/0x579570/0x57aa80/0x57aeb0). In the full link those point into other functions (crash in
  `ai_Unk578180`'s call). It is now `int full_data_6291a0[5] = { (int)&ai_Unk578570, ... }`; the rest of the
  old 356-byte blob (the state-name pointers and strings) is `full_data_6291b4[336]` (nothing reads it yet).
- **ai_Unk57bce0** is a ROUGH skeleton: it now returns int (main.c tests the result), and pops the frame it
  registers (`sv_PopRequest = 1` + a second `sv_RunFrame`) before closing the view and returning 1. Without
  the pop `sv_FrameDepth` stayed one too high; the menu's own pop request was then not consumed and leaked
  into `main_GameLoop`, whose first `sv_RunFrame` returned 0 (the round loop ended after one frame and the
  scene cleanup crashed on the still-linked transport).
- `dbg --build` now loads `full.map` *after* the build (a relink moves every address; the breakpoints used to
  be resolved against the pre-build map).
- The tutorial now runs (90 s / 25 s smoke runs, no crash) to the point of the next open loader work.

## play 2026-10-29 (the tutorial chapter window: the menu that was skipped)

- **ai_Unk57bce0** is no longer a skeleton. The main menu's Tutorial button calls it and it now runs the
  original's window: "tutorial\CHOOSE_tutorial" (0x62a248), the five chapter texts 0x3f1b + i (each object
  +0x48 checkable, +0x28 = 0x62918c[i], +0x1c0 &= -3), the start 0x3f20 and back 0x3f21 texts, then the pump
  0x5309a0 (this function, 0x86): per frame the first checked chapter enables the start (0x475e40), the right
  button / key 1 / d2_ClickedId == 1 set 0x6f22a0, a click on the back pops with 0, a click on the start pops,
  writes the checked state back to 0x62918c and returns 1 when at least one chapter is checked. 0x4758f0 closes
  the window. Before this the function popped its own frame at once and returned 1, which is why the menu was
  skipped and the loading screen came directly.
- The chapter objects were still wrong after the pump was written: `d2_GetWinObj` returned object 0 for all
  seven texts, so nothing was marked and the start button could not be pressed. The cause was in
  **io_PrintText** (its return value and its redraw arguments; see notes/units/io_text.md). With that fixed the
  five entries come back checked (0x62918c is all 1 at start) and "Tutorial starten" starts the tutorial;
  "Zurück zum Hauptmenü" returns to the main menu. Both were clicked through the game's dm_Q queue on a
  debug build and checked by screenshots.
- Grade: ai_Unk57bce0 6 asm / 195 B (ours 544 vs the original 541). The six lines are the two counters of the
  first scan: the original keeps the index (edx) and a second count (esi) although n == i, VC6 folds ours into
  one (tried for/while/do-while, if/else, continue, `i++, n++` in the update clause: every variant the same
  bytes). Meaning is equal.
- **`uv run play|dbg --start tutorial`** still reaches the tutorial without GUI automation: the chapter menu is
  now the screen between the main-menu entry and the loading screen, so ai_Unk57bce0 carries an `#ifdef
  REA_START` hook (`REA_START=tutorial`) that accepts the default selection (0x62918c starts all 1) the way the
  "Tutorial starten" click does. The build gives /DREA_START to this unit as well as main.c (tools/gildegold/
  core/build.py); without the flag the menu waits for a real click and the normal build stays byte-identical.

## play 2026-10-29 (the tutorial intro dialog)

The main-intro dialog "Einführung ins Tutorial" (text 0x3f24 = TUTORIAL_MAIN_INTRO, the `tutorial\main`
form) did not appear. Three separate bugs kept the 0x87 tutorial event from reaching the state machine:
- **main.c** sent the message with `MSG_B(cl, 4)` / `MSG_I(cl, 0xc)` on the `int cl[0x3e]` buffer: the int
  scaling put the type at byte +0x10 instead of +4, so `cm_SendLongMessage` carried type 0 and
  `he_Unk53e040` never created the event. The four stores now cast to `(char *)cl`.
- **he_Unk53e040**'s free-record scan ended at `(unsigned)he_Events + 0x53000` (the first byte, 0x6d) instead
  of `&he_Events + 0x53000`, so it always returned slot 1 and every new event overwrote the previous one - the
  0x87 event survived only a few frames. Now `&he_Events`.
- **ai_Unk575860** and 20 other sites read the state record through `&ai_Unk5eeab8 + 0x1a8`; that address only
  names 0x5eec60 in the original data layout. The full link puts the pointer elsewhere. They now use `S`
  (`*(char **)&ai_Unk5eec60`), which the link relocates.

ai_Unk575860's twelve case bodies were still the ROUGH skeleton. They are written from 0x57595a .. 0x575f80:
state 1 shows the intro and calls he_Unk53dcb0 to activate the event slot, states 2 / 3 answer it, 4 .. 7 run
the first chapter, 9 / 10 handle the second text window and the pending pop, 12 advances to the next record.

A 5 s crash in **sv_Unk530100** was the full link's data shift: the original's absolute 0x75f780 / 0x6f229c no
longer name the frame stack / its depth. The function now uses the symbols unconditionally
(`&ai_Unk75f280 + 0x500`, `sv_FrameDepth`, `ai_Unk623ec4`, `ai_Unk6333cc`). That costs 4 asm / 7 B against the
byte match, so sv_Unk530100 moved from behaviour to draft in `state_baseline/names.csv` (accepted; a symbol
alias at 0x75f780 would be needed to keep the bytes).

Result: the 0x87 event is created, stays in the event table and ai_Unk575860 reaches state 1; the
`tutorial\main` window is created and activated. Drawing it then crashes in `d2_ShpBlit16ClipY` at x=-290 /
y=-225 (clip rect -290,-225 .. 284,225): the form's centre-relative child-window coordinates are being used as
screen coordinates. `d2_DrawWindow` / `d2_DrawWindowBackground` use the local `w->x` / `w->y`; the missing
piece is the view origin (`D2VIEW.viewX` / `viewY`; the original reads them only in `d2_RedrawView` and no
writer is drafted) or the form window's screen x / y in `d2_OpenForm`. Next: locate that writer (or the form
window origin) before the intro can be drawn.

## play 2026-10-29 (the intro dialog drawn: the screen centre, not the view origin)

The d2_ShpBlit16ClipY crash at x=-290 / y=-225 was **not** a missing view origin. The form window's x / y were
what d2_OpenForm / d2_CentreViewWindows wrote; the centre they were computed from was zero. `sv_SetScreenCenter`
(0x530900) had lost its whole float branch in the draft: with mask bit 0x40 (the 3D world) it stored
`(int)0.0f` into both `d2_HalfScreenW` and `d2_HalfScreenH`, where the original stores the **centre of the 3D
viewport**:

    d2_HalfScreenW = (int)(d3_ViewW * 0.5f + d3_ViewX - (mask & 0x100 ? d2_ScreenScale * 6.0f  : 0.0f));
    d2_HalfScreenH = (int)(d3_ViewH * 0.5f + (mask & 0x100 ? d2_ScreenScale * 10.0f : 0.0f) + d3_ViewY);

Read from 0x530908..0x53095c: `fld 0xfe73c8` (d3_ViewW) `* 0x5eb7ac` (0.5f) `+ 0xfe73a8` (d3_ViewX), the
0x100 arm `fsubp` of `0x5fe708` (d2_ScreenScale) `* 0x5ebb1c` (6.0f); the y float `fld 0xfe73c4` (d3_ViewH)
`* 0.5f`, in the 0x100 arm `+ 0x5fe708 * 0x5ebb68` (10.0f), then `+ 0xfc5328` (d3_ViewY); both `ftol`
(0x5de518). The two halves were exactly the missing `__ftol` operands the old comment mentioned. The source uses
the symbols (`d3_ViewW/H/X/Y`, `ai_Unk5fe708` = the float at 0x5fe708 = d2_ScreenScale), and the `(fl & 0x100)`
arms are two branches with `int w, h` locals, which also brings the debt down (31 -> 15 asm, 69 -> 54 B).

The tutorial runs its 3D frames with mask 0x40, so `sv_RunFrame`'s `sv_SetScreenCenter(mask)` zeroed the centre;
the 0x87 event's run handler (he_Unk552bc0 -> ai_Unk577740) then opened `tutorial\main` and
`d2_CentreViewWindows` centred its 580 x 451 main window at `(0 - 580 / 2, 0 - 451 / 2)` = (-290, -225). With the
window origin negative, `d2_DrawWindowBackground` sets the clip rect to the same negative rectangle and
`d2_ShpBlit` sees `x == d2_ClipLeft`, so `d2_ShpBlit16ClipY` never clips horizontally and writes below the back
buffer. With the formula the centre is (451, 396) (view 13's 3D viewport, minus 6 * 1.28 on x, plus
10 * 1.28 on y while the left button flag 0x100 is set), and the window sits at (161, 171): the dialog draws.

**viewX / viewY are not on this path.** The view created by d2_OpenForm is not loaded (`loaded` = 0,
`viewSurf` = 0); the dialog is drawn on the screen by `d2_DrawObjects -> d2_DrawWindow`, which takes `w->x` /
`w->y` as screen coordinates. `d2_RedrawView` (the only reader of `viewX` / `viewY`) is not called. A new search
of the original for a store to +0x188 / +0x18c on a d2_Views pointer found none either, so the d2.h note ("no
writer is drafted") stands; d2_OpenForm, d2_CentreViewWindows and d2_DrawWindow were left untouched.

The "Weiter" button of the dialog is text `%ia[Weiter]` (text id 0x7d), substituted by io_PrintText into
`"$Z%s"`. io_PrintText did not rescan the substituted text, so the `%ia` token was never seen: no redraw created
the button object, `S+0x20` (0x475b80's result) stayed 0 and no click could match it. That and the `$` command
handling are in notes/units/io_text.md ("the '$' commands and the %s rescan"). Verified end to end with the
debug build: `uv run play --start tutorial` shows the dialog (title "Einführung ins Tutorial", red "Weiter"
button) and a click on it closes the intro and starts "Kapitel 1: Steuerung & Panel" (states 2 -> 5 / 6).

## play 2026-10-29 (`--start tutorial` clicks the intro message)

`ai_Unk577a00` (0x577a00) now has an `#ifdef REA_START` arm for `REA_START=tutorial`: it jumps to the answer
(`answered:`) instead of waiting for the click / Return, so `uv run play --start tutorial` clicks the 0x87
intro message through and the tutorial goes on to its chapter records. It jumps *past* the click test rather
than forging `d2_ClickedId` / `d2_PointerObj`: the forged values were read by the rest of the same frame and
acted as a phantom click (it opened the info panel for the tutorial's first step before the state machine got
there). The keyboard arm the original already has (`kb_Last == 0x1c` -> answer) is the same destination, so
the voice stop below it runs too. The normal build is unchanged (the hook is inside the `#ifdef`).

The next records are the chapter-1 steps. Drawing their text crashed in d2_DrawTextLine (`g->adv` with
`g = 0x57209502`): the glyph byte `c` was a signed `char`, so 0x00fc ('ü') sign-extended to 0xfffc in
`d2_GetFrame`'s `offs[(unsigned short)i]` and read 256 KB past the font bank. The original zero-extends the
glyph byte (`movzx dx,dl` at 0x46ee33 and `xor dx,dx; mov dl,al` at 0x46eef3); `c` is `unsigned char` now
(notes/units/d2.md, "the glyph byte"). With that, `uv run play --start tutorial` runs past the intro and reaches
the first chapter record's panel step (`ai_Unk576a80` -> `sv_SelectPanelObject`
from the record's +0x34 callback). `sv_PanelFlag = 1` then gates the state machine and `sv_RunFrame`'s
`ai_FrameWait++` stops the drawing, so the run sits on the panel page: that is the chapter-1 step chain, the
next piece to work on, not the message the debug start was clicking.

## play 2026-10-29/30 (the chapter panel keeps the frame alive)

**The freeze.** At the end of a chapter message the record's +0x34 callback (`ai_Unk576a80`, 0x576a80) calls
`sv_SelectPanelObject(gm_BuildQuery(1, 5, 0xa), -1, ch_EmptyName)`. The market's record query inside it
(`gm_Query(objList, 1, 1, -1)`) is a criterion of kind 1 with the value -1, i.e. a no-op, so `sv_PanelRecord`
stays 0 and `sv_SelectPanelObject` leaves `sv_PanelFlag` at 1 (its "no record, o != cur" way falls through to
the `sv_PopTo(ai_GameFrame)` tail, 0x521b7f). `sv_RunFrame`'s `if (r == 0 || sv_PanelFlag != 0)
ai_FrameWait++` (0x530c1a) then makes `ai_FrameWait` 1 at the draw blocks (and the tail's `--` makes it 0
again) every frame, so neither the 3D update nor the 2D draw block runs, `dd_Flip` is never reached and the
game sits on its last frame (the dark-blue chapter message with the Tutorial page) at 100% cpu. Reproduced
with real clicks too (Tutorial -> only chapter 2 -> the messages), not just `REA_START`.

**The clearer is in `ai_GameFrame`, block 0x591986..0x591a14** (unk_58bbd0.c, just before the frame pump):
with `sv_PanelFlag` set it resolves `sv_PanelObject` - without a `sv_PanelRecord` it focuses the camera on it
(`sv_Unk527840`, 0x527840) and drops the flag; with a record it takes the record word into the object's +0x29
and, by the object's +0x5a bit 0 / `gm_IsHomeType` / `gm_IsType10`, enters the building (`ai_Unk59ee40` /
`ai_Unk59e420`) or only redraws the panel (`sv_Unk527bb0` + `sv_Unk527840`). Our ROUGH `ai_GameFrame` listed
that block in its comment ("the selection 0x75e8c0 / 0x75e8c4 (0x527840, 0x527bb0)") but had no statements.
With it the market panel opens ("Tutorials Marktplatz" in the right panel, verified in a frame dump), the
camera flies to the market and the tutorial goes on; the freeze is gone.

**The blue world was the camera, not the scene** (2026-10-29 follow-up): the very next report - click
"Weiter" in "Einführung ins Tutorial", the next dialog "Kapitel 1: Steuerung und Panel" appears and the city
turns blue - was `sv_Unk528140` passing the stored view's int bit patterns to `d3t_PlaceObject`'s float
parameters, so the camera flew to ~-1.0e9 and every tile was culled to the fog colour. The stored-view casts
and the `ai_CameraStep` int are in notes/units/sv.md ("the tutorial camera flew to 1e9"); the market now
renders and the tutorial continues.

**The chapter-1 chain dropped its first link** (2026-10-29, "the step messages run ahead"): `ai_Unk578570`
wrote `prev = r;` after the FTInitChapter1Intro record, but `r` is the next record and was not assigned yet.
The compiler read the uninitialised slot (`mov edi, _r$[esp+20]` in the listing) and linked Step1 through
that garbage pointer; the intro's +0x70 stayed 0. The state machine therefore left the chapter-1 record
chain after the intro: with `--start tutorial` the run jumped to the chapter-2 market panel within ~20 s,
and a real-click run could show the next step's message before the player did the asked action. `prev = rec;`
is the original's link (4 asm / 9 B closer, `ai_Unk578570` debt 483 asm / 1164 B). Verified with a
manual click/key run: intro -> Weiter -> "Kapitel 1" -> Weiter -> "Die Kamera" -> Weiter -> the hint
"Drückt nun bitte die Taste >N<." (after the 0x429-tick delay) -> N (event 0x16) -> the next record
"Der Stammbaum"; every next step waits for its own click / key event.

## play 2026-10-29 (right-drag: the drag window dispatch and a missing help text)

Two bugs on the same right-drag path (the camera step's "hold the right button" action):

- **`ai_Unk587e00` dispatched the "other record" branch to the goods window.** 0x5880b7 in the original
  calls 0x588bb0 for `sel`; our draft called 0x588160 (`ai_Unk588160`, the goods window, which indexes
  `ai_BTypes + idx * 0x24d`). `sel` is a name/record pointer, so the index was `0xd0355938` and the first
  dereference crashed. The call is `ai_Unk588bb0(sel)` now (same instruction size).
- **The help text id can have no text.** `ai_Unk588bb0` builds `_HILFE_<name>+0`, `txt_FindKey` answers an
  id, and `io_PrintText("$Z$[%s$]", id)` resolves `txt_Ptrs[id]`; if that slot is null (the help text not
  loaded) the `%s` handler calls `strlen(0)`. The guard returns 0 before opening the window now. This makes
  `ai_Unk588bb0` closer (19 asm / 74 B) and avoids the second crash on the same drag.

After both, two right-drags in a row rotate the camera with the hint still up and the game stays alive
(60 s run exits 0).

**Same class, the cannon camera:** `ai_Unk57f0e0` declared `float t[4]` and calls
`d3_MatTranspose(object + 0x18c, t)`, which writes a `D3MATRIX` (0x40 bytes). `t[16]` now; the cannon path is
not reached by the tutorial, but the overflow was the same stack smash as the border-scroll crash
(notes/units/sv.md).

**0x5770e0 is a function, not nop fill.** The chapter-3 record FTInitChapter3Step2 stores +0x3c = 0x5770e0,
which the old header comment called nop fill between 0x5770a0 and 0x577110. The original at 0x5770e0 is a
0x30-byte transition callback (in `ai_Unk576340`'s +0x3c family): kind 0x20 with step p[3] 0x155 answers 1
while the state record's +0x3c is 3, else 0, and every other event falls through to 0x5764b0. The record
stored the *raw original address*, so in the linked exe it called whatever now lives there (in the debug link
the tavern dice window, `read of 0` on a null player record). `ai_Unk5770e0` is defined now and the record
uses `&ai_Unk5770e0`; the header's `@bytes` line is corrected. Same trap as 0x576a80 / 0x577230: a start that
follows a `jmp` or a `ret` + nop run.

**The chapter-3 chain builder was mis-generated** (fixed this session; see the next section). With the panel
fixed the run went on into chapter 3 and crashed at ~41 s in `io_PrintText` (`read of 0x43dc0000`: a float
printed as a text id). The current record was a record the builder called "FTInitAnimChapter3Step8": our
`ai_Unk579570` allocated it as a 0x74-byte chain record (`mem_Alloc(0x74, ...)`), filled +0..+0x20 with floats
(0x441c8000 = 626.0, 0x43dc0000 = 440.0, ..) and linked it through +0x70; the state machine then read +0x14 as
its text id. The original allocates **0x2c bytes** there (`push 0x2c` at 0x57989a) and stores it in `esi+0x64`
of the chapter struct, *not* in the +0x70 chain.

## play 2026-10-29/30 (the chapter 3 / 5 chain builders: the step and animation sub tables)

**What went wrong.** The chapter builders 0x578570 / 0x578e40 / 0x579570 / 0x57aa80 / 0x57aeb0 were generated
from the disassembly by treating *every* `mem_Alloc` in the function as a 0x74-byte chain record. Chapters 1,
2 and 4 only allocate 0x74 records (checked: their functions have no other `push size` before
`call 0x4223c0`), but chapters 3 and 5 allocate the record's **sub tables** as well, and those were turned into
bogus chain records: chapter 3 got 18 records where the original has 10, chapter 5 got 9 where the original has
8. The bogus "records" were the `FTInitScriptChapter3Step*` step tables and the `FTInitAnim*` animation tables,
so the chain contained float blobs whose +0x14 (a float) the state machine printed as a text id.

**The record shape** (already read by the existing drafts; now written by the builders):
- `+0x50` the step count, `+0x58` the step table, `0x10` B per step: `+0` the step delay (0x2c6), `+4` the
  chapter form kind (0..3), `+8` the text id, `+0xc` a window / voice value. `ai_Unk577e50` reads all of them
  and `ai_Unk575860` state 6 reads `[r+0x58][i].+0 + S+0xc` as the step timer.
- `+0x54` the "all steps done" callback (0x577e50 calls it with no argument; chapter 3 record 4 uses
  `ai_Unk577490`, which releases the pending panel id S+0x50).
- `+0x5c` the animation entry count, `+0x60` the animation index callback (called with the update's argument,
  answers the entry index), `+0x64` the animation table, `0x2c` B per entry: `+0`/`+4` the position, `+8` /
  `+0xc` / `+0x10` / `+0x14` / `+0x18` / `+0x1c` the Bezier points, `+0x20` / `+0x24` / `+0x28` the three
  timings. `ai_Unk57b8a0` walks `r+0x64 + x * 0x2c` for `x` in `0 .. r+0x5c - 1`, so a table is exactly
  `count * 0x2c` bytes (0x2c = 1 entry, 0x58 = 2, 0xdc = 5). `ai_Unk578440` already frees `+0x58` / `+0x64`.

**Chapter 3 (0x579570, 10 records, every field from 0x579570..0x57a9af):**
- intro `FTInitChapter2Intro` (the original's copy-paste name) -> `FTInitChapter3Step1` (3 steps, table
  `FTInitScriptChapter3Step1` 0x30) -> `FTInitChapter3Step2` (2 steps 0x20 + anim 0x2c, `FTInitAnimChapter3Step2`,
  `+0x5c` = 1, `+0x34` = `ai_Unk577110`, `+0x3c` = `ai_Unk5770e0`) -> `FTInitChapter3Step4` (3 steps 0x30,
  `+0x54` = `ai_Unk577490`) -> `FTInitChapter3Step6` (3 steps 0x30 + anim 0x58 = 2 entries,
  `+0x5c` = 2, `+0x60` = `ai_Unk577320`) -> `FTInitChapter3Step7` -> `FTInitChapter3Step8` (anim 0xdc =
  5 entries, `+0x5c` = 5, `+0x60` = `ai_Unk5773f0`) -> `FTInitChapter3Step9` -> `FTInitChapter3Step10` (anim
  0x2c, `+0x5c` = 1) -> `FTInitChapter3Outro`. The animation floats are three arms by `d2_ScreenW`
  (0x320 / 0x400 / else); each 0x2c entry is the eight position floats followed by its three timings (the
  0x58 and 0xdc tables are just several such entries back to back).
- The tables are allocated and filled in place, and the records keep the +0x70 chain: the fix attaches the
  step table to `+0x58`, the animation to `+0x64`. `+0x50`/`+0x5c` are set before the alloc (the count is
  known), `+0x54`/`+0x60` are callbacks, not values - a raw original address there would call into whatever the
  link puts at that address (the 0x5770e0 trap).
- The text keys of the step entries and of some record fields sit *inside* multi-string GAPDATA blocks, so
  they are addressed through the block: `0x629dc4` = `full_data_629db0 + 0x14` (`CHAPTER_3_A_DONE`), 0x629cf8 =
  `full_data_629ce0 + 0x18`, 0x629c34 = `full_data_629c20 + 0x14`, 0x629b64 = `full_data_629b4c + 0x18`,
  0x629a5c = `full_data_629a48 + 0x14`, 0x6299dc = `full_data_6299c8 + 0x14`; chapter 5: 0x62a19c =
  `full_data_62a188 + 0x14`, 0x62a14c = `full_data_62a134 + 0x18`, 0x62a0e0 = `full_data_62a0cc + 0x14`,
  0x62a078 = `full_data_62a064 + 0x14`, 0x62a028 = `full_data_62a014 + 0x14`, 0x629fec =
  `full_data_629fd4 + 0x18`. (No new .data: the strings existed, only the symbol names did not.)

**Chapter 5 (0x57aeb0, 8 records):** intro -> Step1 -> Step3 -> Step2 (`+0x34` = `ai_Unk577230`, `+0x3c` =
`ai_Unk577290`, `+0x50` = 0, `+0x54` = 0, anim 0x58 = 2 entries at `+0x64`, `+0x5c` = 2, `+0x60` =
`ai_Unk5772d0`) -> a **second** Step2 record (same name string 0x62a188, no sub tables, `+0x4c` = 0x2d) ->
Step4 -> Step5 -> Step6 (`+0` = 6, `+0x40` = 0x30, `+0x34` = `ai_Unk577690`); the bank
`TUTORIAL_CHAPTER_5.sbf` 0x629fd4 -> 0x6f2654. The chapter-5 Step2 record in the old source was split into
"record + standalone animation record"; the animation is the record's `+0x64` table and the second Step2 is a
record of its own.

**Verified.** `uv run play --start tutorial --seconds 150` exits 0 with no crash (previously crashed at ~41 s in
`io_PrintText`). The market panel still opens ("Tutorials Marktplatz" in the right panel, screenshot), and a
real-click run that selects only chapter 3 (`build/click_tutorial.ps1`: chapter menu 1/2/4/5 off, "Tutorial
starten", the round-change and intro "Weiter") shows "Kapitel 3: Produktion & Handel", then the step message
"Das Warenlager" (the `+0x14` text window, i.e. the record chain and its `+0x50`/`+0x58` step table are being
walked), then waits at the warehouse click step - no crash.
Grades: both builders stay `draft`/`draft`; debt 1032 asm / 4163 B (ours 4896 vs the original 5377) for
0x579570 and 516 asm / 1517 B (ours 1808 vs 2207) for 0x57aeb0.

**Open:** the builders are still field-order / memset-scheduling drafts (the records use byte offsets, the
`d2_ScreenW` arms are spelled out per entry); the next byte work is the same codegen question as the other
chapter builders. Chapter 3 record 4's `+0x54` is `ai_Unk577490` and chapter 5's `+0x60` is `ai_Unk5772d0` -
both are existing symbols, no raw address remains in the builders (`grep -n "0x5[0-9a-f]\{6\}"` in
`src/game/unk_575860.c` is empty).
