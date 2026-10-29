# cut.c (sitz.c, unk_4de4b0.c, unk_4e01e0.c until layout 2026-10-22; joined into cm.c in session 14, these notes are about its second half 0x4d8c70..0x4e5bf0)

Unit notes: status, range and boundary evidence are in notes/tus.md. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the findings of each session, one section per author.

## Open work
- Every function of the half has source now (session 14). Rough / far-off drafts to rewrite from the disassembly: 0x4db700 (the trial scene, 0x4db700..0x4dd95d is ONE function
  of 8.8 KB, the draft compiles to 3.3 KB: the vote / witness loops, the torture instrument pick (13 names at 0x60dad0..0x60db84) and the arguments are guesses), 0x4e1a80 (auction, 1840 of
  2616 B: the bid loop), 0x4e3830 (scene party runner, 1024 of 1968 B: the net run), 0x4e4060 (the scene runner, 1248 of 2064 B: the viewport arithmetic with 0x43b950, the
  replay-mode decisions), 0x4e4cc0 (party update, 656 of 640 B: needs the table view `rec[i].field` with the record starting at 0x925520 and the timers at +0x18), 0x4e5470 (message box,
  688 of 704 B), 0x4e24c0 / 0x4e26d0 (the offer dialog / the AI answer, 528 / 400 B), 0x4daf80 (sketch).
- Near misses of session 14: 0x4e2e60 (scene loader: the original pushes 0x40 for 0x43f8a0 before the branch on 0x6f35ca and pops it at once; ours merges the pops), 0x4e48b0 (the
  original copies the first string with the byte loop and the second with `repnz scas` + `rep movs`; strcpy gives two byte loops), 0x4e4b50 (a separate pointer per branch), 0x4e51b0
  (`xor edx,edx` goes after `mov ecx,ebx` in the original), 0x4e5210 (ebp instead of ebx for the id), 0x4e58e0 (the literal branch sits inline in the original: if/else order and
  ternary give the same code).
- Matched in session 14: 0x4e2860, 0x4e2910, 0x4e4870, 0x4e4a50, 0x4e4f40 .. 0x4e5180 (party records), 0x4e52a0 .. 0x4e53e0 (seed, random, progress), 0x4e5730, 0x4e5770 (registry),
  0x4e5980 .. 0x4e5bf0 (debug dumps).
- The scene kind table (0x92c500, 5 ints per kind, filled by 0x4e5770 through 0x4e5730): +0 run function (called with the party), +4 / +8 callbacks of the kind 6 members / the others
  (r, m), +0xc flag byte (1: the members answer one after the other), +0x10 check. Kinds: 0 picture show, 1 council votes, 2 trial, 3 fight, 4 duel, 5 execution, 6 wedding, 7 birth, 8 death,
  9 bankruptcy, 10 auction, 11 sale.
- Nonmatching helpers (layout 2026-10-22): 0x4e2bd0 (32 / 48 B: `if (replay) b = 0; return d2_Unk475590(a, b)` compiles to a second tail call; the original shares one `jmp`), 0x4e2e00
  (33 / 96 B: the original keeps -1 in esi for the compares and `return`; do / while + goto variants tried), 0x4e3420 (35 / 112 B: the original loads `a` into ecx before `add esi,ecx`),
  0x4e3490 (66 / 224 B, the compare shape: `fcompp` + `test ah,0x41` + `jp`), 0x4e3750 (55 / 64 B, for and do-while tried), 0x4e32f0 (10 / 160 B: the byte store `((char *)&v)[2] = 0` sits after the
  first float push in the original), 0x4e30c0 (rough), 0x4e36a0 (rough), 0x4dae90 (the original reuses the parameter slot as the out value: `&r` of the parameter itself, 221 / 256 B).
- Shape-only drafts (far off, rewrite from the disassembly): 0x4d8e20 (the session, 2320 of 6.9 KB: the two vote loops over the candidates / voters are
  missing, only the setup, the head show and the end are there; the locals are 6 name tables ("Sitzung_*.ed3", "sitzungen_flug_*.esc") and the group id
  array {6,5,4,3,2,1}), 0x4ded10 cut_MainDuell (1224 / 1936 B; three rounds of 0x4de900, result texts 0x111c .. 0x1123), 0x4e0770 (wedding script; the
  macro G25_STEP / G25_VOICE hides the pattern: text, voice wait with 0x426f20, pause 0x3e8, 0x4e3490 with the cleanup 0x4e06d0 aborts), 0x4e0f70 (birth),
  0x4e1250 (death), 0x4e1700 (bankruptcy), 0x4e01e0 (execution), 0x4de640 / 0x4de900 (duel shot / intro rounds), 0x4debe0, 0x4dfa20, 0x4dfcb0, 0x4dfdd0, 0x4e0040, 0x4e0580.
- Near-misses: 0x4de4b0 (44 B: the original keeps a dead `fstp` of the random angle into the parameter slot; volatile made it worse), 0x4e0e90 (2 B: buffer offset
  esp+0x14 vs 0x1c, the original has one local fewer), 0x4e1220 (the original tests `and eax,0x4100` for `life <= 0`, the draft `test ah,0x41`),
  0x4dff80 matched with `(unsigned int)ai_Unk4e52f0(10) > 7`.
- Type debt: cut.h declares all callees K&R (notes/layout.md session 13).

## layout 2026-10-22 (session 13)
- Boundary: see notes/tus.md (start 0x4d8c70, end unknown). The module is the scripted cutscenes: each scene opens "cd2\*.mp3" music, a .ed3 world, a
  .sbf bank, "cutscenes\<name>\enter_*.esc", speaks texts through 0x515960 (text id; a view from 0x4741f0), waits with 0x4e3490(ms, cleanup, state) which
  returns nonzero when the player skips, and ends with the exit scene + 0x4e30c0 / 0x4e3640 and 0x6ed9d4 = 0. 0x60d360 = 0x702c6 is set at each scene start
  (the frame wait argument of 0x5309a0).
- Layouts: the party record (0x114 bytes, same as 0x6f0d04): +0xc owner, +0x30 count (byte), +0x34.. ids (4 each), +0x7c / +0x80 kind / argument,
  +0x94 the answer (duel dialogs), +0x14 text. A character is 0x21c bytes at ai_Unk770b80 (+2 kind: 6 / 7 = persons of players, +4 id, +9 sex,
  +0x166 flag, +0x1b1 imprisoned, +0x1cc flags), ai_Unk770b84[id][0] the owner id.
- Duel (0x4ded10): m + 0x7c the scene (0..5 -> "duell.ed3"), the rounds ask 0x4de900 per side: the kind +0x94 of the record 0x4e50e0 decides (2 insult, 3 a roll,
  4 the pistol shot via the object callback 0x4de640); globals 0x6f0da8[0..5] (damage dealt to A / B, flags of won rounds), 0x6f0d94 (a fighter down),
  0x92bca4 / 0x92bca0 the two character copies. 0x4de5e0 / 0x4de610: a table of numbers (0x92bcc0) used as a repeatable random source.
- Matched on the first draft: 0x4d8c70 (the y argument is `(short)` of the whole expression, via a short-parameter cast), 0x4d8cb0 (the inner call result goes to a
  local first, the pops merge; the arguments are (x, y, ..) = (stack offset, 0x20 / 0x3e / 0x2f)), 0x4da9e0, 0x4de530 (`< 0.2` double), 0x4de5e0, 0x4de610
  (the counter is stored after the second modulo), 0x4dfff0, 0x4e0010, 0x4e06d0.

## layout 2026-10-22 (blocked-TU pass)
- Boundary evidence is in notes/layout.md (Open boundaries: cm.c / cut.c, cut.c end). The scene registry 0x4e57xx lists the scene kinds: 2 session {0x4db700 main, 0x4dd960 questions, 0x4de340 AI answers, 0x4daf80
  start check}, 3 the fight {0x4d63e0, 0x4d8b10, 0x4d60b0}, 4 duel {0x4ded10 ..}, 5 execution {0x4e01e0 ..}.
- 0x4dd960 asks the player the court / torture questions ("cutscenes\\prozess" yes / no boxes, "cutscenes\\folterwahl" three choices, texts 0x14bc .. 0x1532); the bodies sit in the order of the jump table
  entries 0, 1, 2, 3, 6, 4, 5. A switch's jump table order is the source order of the case bodies: when the link tool reports "local reloc ... expected", the case bodies are in the wrong order (0x4de340: 6, 4, 5).
- The helpers: `ai_Unk6f0d8c` is the replay mode (the helpers do nothing, return 0 / 1); 0x4e32d0 / 0x4e32b0 / 0x4e3270 / 0x4e3290 / 0x4e3250 forward to the sound / object calls
  (0x4c27d0 / 0x4c2760 / 0x4c2830 / 0x426ba0 / 0x4028a0) with the same arguments (one `jmp`); 0x4e3420 / 0x4e3490 / 0x4e3570 wait n ticks (14 ms per tick, `d3_Time`) in a frame-wait loop
  (0x5309a0 with 0x8000 added to 0x60d360; 0x4e3060 uses 0x80 and 0x4e2d40 the constant 0x79706); 0x92cae0 is the scene party table (16 slots of 0x35c B), 0x1468ae0 the 0x300 objects.

## layout 2026-10-22 (session 14)
- Boundary: cm.c and cut.c are one unit (joined; evidence in notes/tus.md). Order of the second half in the file: not yet address order (reorder = first job of the next session).
- Records: the party records at 0x925520 (96 x 0x114, G28REC: +0 id (-1 free), +8 kind, +0x14 the thing, +0x26 flags, +0x30 count / used, +0x31 default 8, +0x34 ids); the lookups
  0x4e4f40 (smallest pending id), 0x4e5050 (kind + thing), 0x4e50a0 (id), 0x4e5120 (free), 0x4e5150 (store), 0x4e5180 (clear) match as index loops `tab[i].field`. The member slots at
  0x92cae0 (16 x 0x35c) keep their id at +0x10 (a pointer-based compare of 0x92caf0 against the slot base gives a link conflict: use `*(int *)(slot + 0x10)`).
- The seed of the scene random numbers is 0x92c4e0 (a linear congruence 0x41c64e6d, 0x3039; the number is the high word (+2) modulo 0x7fff, unsigned); 0x4e52f0 (n), 0x4e5360 (float, times
  1/32767), logged with "cut_rnd", "cut_rndfloat", "cut_randseed", "cut_getrandseed". 0x4e4a50 is "cut_CheckMaster", 0x4e4b50 "cut_CutsceneReady".
- 0x4db700 and 0x4e4060 take ONE argument (the party), the trial's `a, b, c` are the first three ids of the party. The window names of 0x4e4060: "cutscenes/prozess_main" (kind 2),
  "announcement_cinemascope" (own view flag 0x20), "announcement".
