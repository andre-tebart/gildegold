# am unit (am.c; joined from am.c, unk_48e070.c, unk_48f250.c, unk_490830.c, unk_494020.c, unk_494e70.c, unk_4984d0.c, unk_49a0a0.c, unk_4a3af0.c in layout 2026-10-22)

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "am_RegisterMethod(): Methode '%s' ..." (0x5ffde8); AI methods: action registration, decision-table, decide / execute / eval callbacks, the AI cannon shot
- Boundaries: separated (proven): start: 0x5ffd1c = -1 (its first global) right after the AI unit's strings (g1 / g2); end: its .data block ends with "%sgebaeude/*%s_%c.ogr" 0x600478, the next block 0x600490 is unk_4a7140.c's; one .data string block in function order ("building_upgrade", "am_RegisterMethod(): ...", the action names); joined layout 2026-10-22 (session 5) from am.c, unk_48e070.c, unk_48f250.c, unk_490830.c, unk_494020.c, unk_494e70.c, unk_4984d0.c, unk_49a0a0.c, unk_4a3af0.c with unchanged byte counts (0x4a61d0 279 -> 53, 0x4a3af0 2322 -> 2317, 0x493be0 589 -> 576 improved), headers merged into am.h. One exception to address order: 0x495a50 sits at the end of am.c (defined above its callers the far-off draft changes 0x496e40 / 0x499660, matched); move it back once it matches
- Range then: 0x48e070..0x4a6ae0
- Was: unk_490c10.c + unk_495ee0.c (round 21); the eight unk_*.c above (layout 2026-10-22)

## Open work
- **headers (2026-10-30, typing pass 2)**: head.c includes txt_views.h, am.h, data_views.h, cm_types.h, time_view.h, ls.h; keeps 5 cast / view macros with @original byte evidence (the address and the matched caller are in each comment): util_RandomU, unk_Unk4f0e80I, gm_Unk505e60S, cm_SendExSellObjectI, amt_GetKindB; views / casts without byte evidence of their own (weak definitions or drafts' spellings, notes/headers.md debt item 1): d3t_GetObjectBoundsV, ai_GoodsT, ai_BldsT, ai_BTypesT, ai_CharsT, ai_TimeT.
- 0x495a50 is at the end of am.c (out of address order): defined above its callers, its far-off draft (445 B)
  changes 0x496e40 / 0x499660 (matched) and 0x496610 / 0x499c10; move it back between 0x4957d0 and 0x495ce0 once
  it matches (or test then whether the callers still match).
- Type unification inside am.h (the views AIPX / AGCH / AIC4 / G5CH / AIPER, duplicate callee prototypes; notes/
  layout.md **Type debt**), and the near-misses 498c70 2 B, 4900c0 / 4904c0 16, 4945c0 17, 499c10 35, 4979d0
  (notes/units/am.md [g1]..[g4], [hv4]; notes/units/ai.md [ai2]).
- am drafts: the ai_Unk4a63f0 callers 0x48eb50 182 / 0x48ec90 206 / 0x48edc0 208 / 0x48ef10 136 now pass the
  __int64 mask (0x48f010 matched with it); only the AIMSG store order is left (brute-force it again); 0x48ff50
  still to convert. Far off 0x48e120 136, 0x48e370 184,
  0x48f2b0 139, 0x48f600 217, 0x48f990 142, 0x490170 247, 0x490350 322, 0x490690 329, 0x492fd0 258, 0x493420 806,
  0x493890 308, 0x493a40 375, 0x493be0 589, 0x499400, 0x497fa0, 0x497660 749 (notes/units/am.md [g1]..[g3], [hv4]): after the am join.
- unk_49a0a0.c near-misses (layout 2026-10-22): execute callbacks 0x49a5b0 / 0x49aa80 / 0x49ad80 / 0x49b050 /
  0x49b2b0 / 0x49c000 / 0x49bc50 13-14 B each: the original does `xor eax,eax` before `test esi,esi` (one zero for
  the return and the zero-extension of o->id); nested ifs and `&&` tried. 0x49b6f0 129 (other shape). 0x49d7f0 90
  (fcomp mem vs fld / fcompp: d[3] should be a memory operand). 0x49ea90 66 (early-return layout). Drafts far off:
  0x49b7c0 844, 0x49bd90 389, 0x49c8e0 522, 0x49cb60 240, 0x49ce50 368, 0x49d0f0 370, 0x49d2d0 320, 0x49d8d0 751,
  0x49def0 732, 0x49e350 669, 0x49e7a0 557, 0x49ec10 163.
- unk_49a0a0.c / unk_4a3af0.c near-misses (layout 2026-10-22, session 2): 0x4a1170 1 B (`[esi+eax+0x74]` operand
  order of the 0x506430 byte array: `fin[0x74 + r]` / `fin[r + 0x74]` tried; try a struct field), 0x4a13e0 4 B
  (which index register is loaded first in the s[n] sum; swapping the addends changes nothing), 0x49fb80 22 B (eax
  vs edx for s1->unk04 in the 'join' branch), 0x4a1640 36 B, 0x49f7f0 50 B, 0x4a5b50 51 B (same sizes). 0x4a16f0
  (registration, 9104 / 9120 B): where decide == exec (0x52a040) VC6 CSEs the address into eax, the original
  stores it twice: probably two identical stubs folded by the linker (/OPT:ICF); the rest of the diff follows.
  0x4a5320: `_strupr` for 0x5ea986 is a guess (unchecked). Drafts far off: 0x49eec0 1075, 0x49f5c0 338, 0x49f850
  660, 0x4a0a70 819 (1008 B original), 0x4a5140 320, 0x4a5320 424, 0x4a5540 381, 0x4a5760 250, 0x4a58d0 166,
  0x4a5a30 89, 0x4a5d30 346, 0x4a5ef0 671, 0x4a61d0 279, 0x4a63f0 311, 0x4a6570 299. 0x4a67a0 23 B: only the
  order of the ai_Unk6f0698 stores at the end (Ghidra's order tried). 0x4a6ae0 (cannon shot execute) draft 1360 / 1632 B.
- am.c nonmatching (hv3): ai_Unk4960b0 934, ai_Unk496610 915, ai_Unk496b10 561 (need byte in bl, notes/units/am.md);
  hv4 did not reach them.

## g1 -- round 22: 0x48e070..0x48f250 (src/game/unk_48e070.c, include/unk_48e070.h) [g1]

### Report
Scope: 0x48e070..0x48f250. Ghidra listed 9 starts; the range really holds 15 functions: Ghidra missed 6 after
short functions / callbacks (48e120, 48e370, 48e470 inside the "48e070" function, 48eab0, 48f010, 48f1e0).
Nonmatching (7), bytes differing / size:
- 0x48e120 ai_Unk48e120 (136 / 608, 39 asm lines): right layout except (1) the tail `o1->unk08 = need;
  o1->kind = 0x12; call` that the original shares between the two branches (second branch jumps back into it;
  `goto tail` made esi/edi swap), (2) `need = cost - lim` is done in ecx in the 2nd branch, in edi in the 1st.
- 0x48e370 ai_Unk48e370 (184 / 256): original keeps p in esi and never enregisters s2 (loaded to edx at entry,
  reloaded after the call); ours puts s2 in esi, p in edi. Time copy loads also in another order.
- 0x48eb50 / 0x48ec90 / 0x48edc0 / 0x48ef10 (msg senders, 181/320, 206/304, 210/336, 156/256): all four call
  ai_Unk4a63f0(p, 0, 0x7b4c4a0, 0, 0, 100.0f, &v) and the original pushes the constants through registers
  (`xor ecx,ecx` for the three zeros, `mov eax,0x7b4c4a0; push eax`); ours pushes immediates. The same idiom is in
  0x48ff50 (not mine). Not found: locals, inline wrapper, casts, varargs / unprototyped callee, function pointer
  all give `push imm`. Also the order of the AIMSG field stores differs (the scheduler interleaves the ai_Time
  copy; 100 single-move statement orders of 48ef10 and 81 of 48ec90 tried, none closer than 35 asm lines).
- 0x48f010 ai_Unk48f010 (141 / 448): everything equal except the same register-constant idiom in
  ai_Unk4a63f0(p, 6, 0xe07c43b4, 1, 0, f, &v): the original does `mov ebx,0xe07c43b4; mov ebp,1` (pushed ebx / ebp)
  before the util_Random call and pushes them as registers. Locals `k = 0xe07c43b4; one = 1;` are propagated away.

TU boundaries: the AI TU ends at 0x48e070 (notes/units/ai.md): ai_Unk5ffd1c (-1 at 0x5ffd1c) is its first global, then
the string "building_upgrade" (0x5ffd20, used by 0x48e370 with ai_Unk4c4540 / 4c4580), then "UseAlchemistryItems",
"BuildSculpture", "UseCannonTower", "UseSecretRoom" for the functions after 0x48f250 (not mine). The TU continues
past 0x48f250. Suggested name: ai_upgrade.c (module tag; the functions are the AI action callbacks of the
"building_upgrade" decision, kinds 0x40 / 0x41 / 0x42: decide (int a, ch, int b, out1, out2) -> kind or 0, execute
(ch, arg1, arg2)). 0x48e070 itself is the shared "master building" helper (fills ai_Unk6f0470 = number of kind 5/6/7
characters once per game day).

Shared headers: none touched (ai.h read-only; own header include/unk_48e070.h includes ai.h with
`#define ai_Unk501190 ai_Unk501190_twoparams` around it, because the callers pass five arguments). Globals
`ai_Unk5ffd1c` (defined here, `= -1` in .data) and `ai_Unk6f0470` (extern, far COMMON, defined elsewhere / by the
linker) are declared in my header.

Wrong in the Ghidra export (for the next reader): `thunk_FUN_004c5f60` is really 0x50cc50 (ai_Unk50cc50(int id, int v));
`FUN_0042ec00(0x20, &x)` in 48e620 / 48e730 / 48e830 is util_Random(0x20) plus a stray out-pointer: the real call is
`ai_Unk4a58d0(p, 1, &flags, 0, util_Random(0x20) + 25.0f, &id)` (+ 32.0f in 48e830).

General VC6 lessons:
- A decide callback that passes the address of one of its own parameters to a callee gets a local in the dead
  parameter slot: `ai_Unk48e070(p, (int *)&bld)` (bld lands in p's slot).
- Several `return 0` failure exits that the original reaches by `je` to one shared epilogue need the success path
  written as one nested `if (a && b && ...) { ... }` followed by a single `return 0;` (matched 48e120's prologue;
  early-return form gives inline epilogues). A `goto ok;` to a trailing `return 0x40;` merges the success tails.
- `if (x >= a[2]) sel = 2; else if (x >= a[1]) sel = 1; else if (x >= a[0]) sel = 0; else return 0;` matched
  where the nested `<` form did not (block layout and register choice).
- Float compare `fcomp; fnstsw; test ah,5; jp fail` is `if (x < c) {ok}`; `test ah,0x41; jp` is `x <= c`;
  `and eax,0x4100; je` is `x > c`.
- `(int)(f * 0.02f)` sums: when the original has `add eax,edi` (second call first), a temp for the first call and
  `(second + first)` fixes the operand order (48e620).
- `if (!ai_Unk4fea50(&x, ...)) return 0;` is wrong when the original loads x before the stack pop: the return
  value is not tested, only `if (!x)`.

## g2 (round 22): 0x48f250..0x490830 [g2]

### Report
Files: src/game/unk_48f250.c, include/unk_48f250.h (ai.h / am.h untouched).
Real starts (Ghidra had 11; the callbacks reached only through pointers add 5): 0x48f250 48f2b0 48f500 48f600
48f990 48fb70 48feb0 48ff50 4900c0 490170 4902b0 490350 4904c0 490570 490600 490690 (scope ends with the padding
before 0x490830).

- **Nonmatching (9)**, faithful drafts: 0x48f2b0 (139 B, tail merge / slot layout), 0x48f600 (217), 0x48f990 (142),
  0x48ff50 (88), 0x4900c0 (16), 0x4904c0 (16), 0x490170 (247), 0x490350 (322), 0x490690 (329).
- **What the functions are**: 0x48f250 finds the building of a character; 0x48f2b0/0x48f500 decide/execute of
  action 0x43 "BuildSculpture";
  0x48f600/0x48f990 decide/execute of 0x48 "UseAlchemistryItems"; 0x48fb70 registers 0x40..0x43, 0x48 (callbacks
  0x48e120 0x48e370 0x48e470 0x48eab0 0x48f010 0x48f1e0 are before the scope, declared in unk_48f250.h);
  0x48ff50 / 0x490170 / 0x490350 / 0x490570 = decide (p, building, out1, out2) and 0x4900c0 / 0x4902b0 / 0x4904c0 /
  0x490600 = execute (p, building, unused, record; send message 0x88 / 0x90 / 0x89 / 0x8a, return 0x3d) of the
  graveyard methods; 0x490690 = decide of "Graveyard Owner" (0x3d), picks one of the four decides at random;
  0x48feb0 picks a random kind-0x14 character of a building that is not busy with 0x88..0x90.
- **am TU start**: .data of the AI TU ends with the strings of 0x48d810 (0x5ffcd0 "...entladen", ends 0x5ffd18);
  0x5ffd1c is a 4-byte initialised global (-1), read by 0x48e071 / 0x48e0b2, followed by the am strings in
  function order: "building_upgrade" 0x5ffd20 (0x48e3c5, 0x48f555 ...), "UseAlchemistryItems" 0x5ffd34, "BuildSculpture"
  0x5ffd48, "UseCannonTower" 0x5ffd58, "UseSecretRoom" 0x5ffd68, "Build Domizil" 0x5ffd78. unk_489a70.c ends at
  0x48e070, so the am TU starts at 0x48e070 (the first function that touches its .data; earlier data-free
  functions would show nothing). The TU runs on past 0x497490 (hv3).
- **Shared headers**: none edited. The callee prototypes missing from ai.h / am.h are in unk_48f250.h
  (0x4a5140 0x4a5320 0x4a5760 0x4a63f0 0x49eec0 0x509d30 0x509dc0 0x5ba620 0x5cf8a0 0x5044b0). ai.h declares
  ai_Unk4fea50 / ai_Unk501190 with other parameter lists: called through casts (AI_4FEA50_4, AI_501190_5).
  am.h's ai_Unk490690 (void) is renamed away with a #define around the include, as unk_489a70.c does.

### Near-miss notes
- 0x4900c0 / 0x4904c0: only the order of two stores (arg[2] before ids[0] with the loads of unk10 / unk08 hoisted
  above them) differs; all 120 orders of the five stores, `a = b = x` forms and `= msg.ids[0]` forms tried.
- 0x48f2b0: head matches (s1 / s2 as members of one struct gives the original slot order, `t = bl->type + 1`
  and `(unsigned char)ai_BTypes[..].unk247` fix the rest). The original merges the tail `o1->kind = 0x12; call` of
  both paths with the store `o1->unk08 = d` kept before it; ours schedules both stores after the pushes, so the
  tails differ in registers and are not merged.
- 0x48f600: frame size matches with `id` / `flags` scoped to the r < 6 block and `out` to the r == 6 block, one
  `goto fail` exit; left: p lives in edi in the original (ours ebp), -1 in ebp.
- 0x48f990: original does `xor eax,eax` before `test esi,esi` of the first 0x4ffe90 result and the null path
  skips the later xor; flat / ternary / nested variants all compile to ours.
- 0x48ff50 / 0x490350 / 0x490170: the original keeps `id` in the dead slot of parameter b and the random temp /
  `x` in the dead slot of o1 (Ghidra shows `&param_2`), flags in the frame slot; ours puts them elsewhere and
  uses ebx where the original has only edi / esi / ebp. `q = &AIC[id]` (a pointer to the character, as in the
  original's `lea esi,[eax*4+0x770b80]`) is right. 0x490170 also keeps `u1` / `u2` as `mov si,ax` / `mov di,ax`
  and the constant 1 in ebp.
- 0x490690: the original keeps 0x20000000 (flags of the 0x4a63f0 call) in edi from the function entry and
  uses ebx = 0; ours constant-propagates it.

### General VC6 lessons
- A msg fill `memset(&msg, 0, 0xf8); msg.type..; msg.time = ai_Time; ...` is sensitive to statement order: brute force
  all permutations of the 6 stores (720 variants, uv run try) found the match for 0x48f500 in one run.
- A local initialised with an array initializer of constants is compiled as individual zero stores; the original's
  `rep stos` + constants stored between the argument pushes is `memset(a, 0, n); a[i].f = c;`.
- A pointer to an indexed array element (`q = &arr[id]`) used for several fields gives `lea esi,[eax*4+base]`
  where `arr[id].f` repeated gives a reload of the scaled index.
- `if (x < 3)` on a `char` field where the original has `jae`: the field is unsigned (`(unsigned char)` cast).
- `util_Random(n) + 10.0` (double literal) gives `fadd qword`; the original's `fadd dword` needs `10.0f`.

## g3 report (scope 0x490830..0x494020, minus am.c's 0x490c10..0x492e90) [g3]

File: src/game/unk_490830.c + include/unk_490830.h (types AGCH, AGARG, AGDUEL, AGFIN; ai.h only included).
Suggested name: this is the same TU as am.c (address-contiguous: 0x490690.. 0x4908d0 .. 0x490b90 | am.c 0x490c10..0x492e90 |
0x492fd0..0x493ea0 | next function 0x494020). Evidence: am.c's header says the TU starts before 0x490c10, the
callbacks 0x490830 / 0x4908d0 / 0x490b90 are the exec / decide of ai_Unk490c10's registered actions. Merge into am.c
at round close (am.h declares ai_Unk490830 / 4908d0 / 490b90 as `int f(void)` / old signatures: change them then).

0x4908d0 had no Ghidra start: callback only, found with disasm; it lies between 490830 and 490b90.

### Nonmatching (source exists, faithful translation)
- 0x492fd0 (258/560 bytes): structure and every constant right; the original keeps the parameter k in memory
  ([esp+0x54], modified in place) and puts ret/base/mul/idx in ebp/ebx/edi/esi, ours enregisters k in ebx. Tried:
  separate loop counters, pointer loop, volatile k, int k, strncpy as if/else (that one is right, kept).
- 0x493420 (806/912): ints for the rolls, `if (f > 0.8888f)` chain order right; the original keeps the `short`
  Random results in di (`mov di,ax; neg di; sbb edi,edi; add edi,3`) and the roll/`h` in param slots; frame and
  registers differ (original pushes only ebp/esi/edi, ebx locally). The k==0 tail stores an uninitialised local when the
  roll loop falls through (never happens, `mov al,[esp+0x14]`; ours stores w, an uninitialised local too).
- 0x493890 (~308/432): the original keeps b1/b2 (n1/n2 bytes) as char locals spilled to the stack, float
  threshold computed before the RandomF call; ours allocates differently.
- 0x493a40 (375/384): structure right, original spills sum1 to a frame slot ([esp+0x10], `push ecx`) with s1/s2
  in ebp/esi and ph in edi; ours coalesces ph with s1 and puts the spill into the dead parameter slot.
- 0x493be0 (589/720): original keeps r in ebx and spills/reloads it around the Random loop, `o` only reloaded
  from the stack; ours assigns r/o to other registers. Block-scoped temporaries (d, x, flags, neg) already used.

### General VC6 lessons from this scope
- A dead out-parameter variable (`&t`, never read) in its own block scope `{ int t3; f(&t3); ... }` makes
  VC6 give the later variables the original dead parameter slots (4908d0: 1 byte off before, matched after;
  function-scope variables took the wrong slots). Same for a `-1` message value: `{ int neg = -1; ... &neg }`
  inside the branch took the parameter home slot (493ea0, instead of a separate slot / writing to the parameter).
- One final `return 1` after an if/else whose branches both set the same field (and `return 0` guards before it)
  shrink-wraps a late callee-saved push (493200: `push ebp` after the guard); two `return 1` inside the branches
  pushed it at entry.
- A switch whose cases 1 and 2 share a tail but call 0x493420 with different constants: write the calls per case
  (VC6 cross-jumps them, with `push 0/1` before the shared tail); a `z` variable gives different code. Cases 3 and 4
  as separate `return 1` cases (not a fall-through) gave the original registers (4937a0).
- `if (x > 0)` with an unsigned char member gives `test; jbe`: write `> 0`, not `!= 0` (493ea0).
- Pointer compare against the end of a const table: `(int)q >= (int)&tab[9][1]` gives the signed `jl` of the original.
- `x = m; if (10000.0 > x) x = 10000.0;` gives `fld c; fcomp st(1)`; `x / (log10(x) + 1.0) * 0.1 * (k * 0.25f)` needs
  `(x / (..) * 0.1) * (k * 0.25f)` parenthesised to keep the original multiply order (493200).

### Shared headers
None changed (ai.h only read; am.h not included).

### Outside scope
- 0x490690 (decide callback of "Graveyard Owner") and 0x4900c0..0x490600 (sub-functions of 0x490830) have no source;
  names.csv has no rows for them.
- ai.h declares ai_Unk4fea50(a, item, b, id); 0x490830 / 0x490b90 call it (&item, 0, 0, id), done with an (int) cast.
- const tables at 0x5ecf18 (9 pairs) and 0x5ecf60 (5 rows of 6 floats) are in this file as static const.

## hv3 (round 21): heavy 0x490c10, 0x495ee0 [hv3]

### Report
- **Real function starts.** Both Ghidra ranges held many functions (callbacks reached only through pointers):
  - 0x490c10..0x492fd0 (9152 B) = 30 functions: 0x490c10 (registers three actions) + the 29 decision
    callbacks of the decision table at 0x611258 (29 records of 0x24 bytes, callback at +0x20; record 8's
    callback 0x492b80 sits out of order): 0x490e00 490fc0 491200 4912e0 491430 491560 4915f0 491650 4916d0
    491860 491a40 491b20 491bf0 491cb0 491d80 491e80 491fb0 4920c0 4921d0 4922e0 492410 492540 492680
    4927c0 492900 492a40 492b80 492d50 492e90. (0x492cb0 after a `ret` + padding is inside 0x492b80.)
  - 0x495ee0..0x497490 (5552 B) = 20 functions: 0x495ee0 495fa0 495fc0 495fd0 495ff0 496000 496020 496030
    496050 496070 496080 4960a0 4960b0 4965b0 496610 496ab0 496b10 496e40 4972d0 4973f0 (decide / execute /
    evaluate callbacks stored by the registration code at 0x4a1700..; 0x497320 is inside 0x4972d0).
- **Nonmatching (3)**, faithful drafts, the rest is register / slot allocation:
  - ai_Unk4960b0 (0x4960b0): 934/1216 B differ (orig 1280 B). The original keeps the need byte `c` in bl and
    reloads p from its argument slot, walks `p->unk080[i]` with a pointer (ebp) and the int arrays with a
    byte offset (esi += 4); ours keeps p in a register and spills c. int / pointer loop variants: no change.
  - ai_Unk496610 (0x496610): 915/1136 B differ (orig 1184 B). Same `c`-in-bl issue; the original also keeps
    best/second/ka/kb in slots 0x10..0x18 (ours 4 bytes lower), and stores `s2.kind` from al first.
  - ai_Unk496b10 (0x496b10): 561/800 B differ (orig 816 B). Register choice: original a->edi, b->ebp,
    i->ebx and y in ebp (b reloaded from its arg slot afterwards), x in the f slot; ours i->edi, a->ebx.
    Block layout already equal (the fallback block lands after the job loop either way).
- **TU name suggestion**: `am.c` (tag). The .data of both blocks is one string block with the message of
  0x495520 (`am_RegisterMethod(): Methode '%s' hat keinerlei Auswirkungen!!!` at 0x5ffde8, used at
  0x4956ea): 0x5ffd20.. action names (registration functions 0x48e3c5.. use them), "Use Seance" /
  "Handle Grave" / "Graveyard Owner" (0x490c10), "/gamedata/ai/ai_data_2.dfn", "ai_method_addon.ini",
  "LongChange%li".. (0x495635), "_NACHRICHTEN_HS_22" (0x4972d0), then "sabotage".. (0x499b0c). So
  0x490c10.. and 0x495ee0.. are one TU (the AI "methods" module, am_), which starts before 0x490c10 and runs
  on past 0x497490: join unk_490c10.c and unk_495ee0.c (+ headers; unk_495ee0.h includes unk_490c10.h).
- **Shared headers**: none edited. ai.h is included read-only.
- **Outside scope**: 0x52a040 (`xor eax,eax; ret`) is an ICF-folded stub; 0x490c10 stores it twice, so the
  source had two functions (ours: ai_SeanceDecide / ai_SeanceExec, resolved by site). ai.h declares
  ai_Unk53e400 with five arguments; 0x496b10 calls it with three (`push 0x2c; push 0; push 1`), so it is
  probably varargs (unk_495ee0.h calls it through a cast, AI_53E400_3). 0x495520 = am_RegisterMethod.
  0xf83ec0 is the registered-action table (AIACT, 0x94 bytes; unk_47d960.h's ai_Unkf83f50 is its +0x90
  flags). The AI character (0x21c) fields 0xac..0x114 are mood[3..11]; AIPER in unk_490c10.h is our view.

### General VC6 lessons
- Two different function names that the linker folded (ICF) to one address: `act.decide = A; act.exec = B;`
  gives two `mov [..],imm`; one name assigned twice is CSE'd into `mov eax,imm` + two register stores.
- `m = f(g(h(1)->x, ...)->y, byteglob)` pushes the byte global before the inner calls; with temporaries
  (`b = h(1); o = g(b->x, ...); m = f(o->y, byteglob);`) the inner calls come first and all pops merge.
- `x = (float)(p->m - 2.0);` (or `* 1.0f`) stores x before the next call-pop / compare; without the cast the
  store moved after `add esp,4; cmp eax,5` (0x491fb0).
- `n = min(v, 4); n = max(1, n);` gives `cmp esi,1; jge` (like `if (n < 1) n = 1;`); `max(n, 1)` gives `jg`.
- `if (c) *out = 2; else *out = 0; return 1;` keeps two separate `return` values (`mov eax,edx` vs
  `mov eax,1`); two `return 1;` branches were merged into one hoisted `mov eax,edx`.
- `if ((g = f()) < best) { best = g; ... }` gives `fld st(0); fcomp [best]`; `g = f(); if (g < best)`
  gives `fcom [best]`.
- Out structs filled only inside one `if` block (`AIARG s1, s2;` declared in that block) share their slots
  with arrays used in other branches; at function scope the frame was 0x28 / 8 bytes too big.

## g4 report (scope 0x494020..0x494e70, src/game/unk_494020.c, include/unk_494020.h) [g4]

Nonmatching: 4945c0 ai_Unk4945c0 (17/256 bytes): the original keeps the plan pointer in edi and n/result in esi, we get
  the swap, plus the original stores the fraction (fstp) after `push edx`, ours before. Tried ~60 variants (decl
  order, register, split r, x local, float/double fraction, unsigned n, switch spellings, inline fraction,
  extra dead references): none change the allocation. Only real extra uses of q after the switch flip it, and the
  original has none. Unsolved.

TU: the scope continues the am unit (am.c holds 0x490c10..0x492e90 and 0x495ee0..0x4973f0). These 17 functions are
callbacks of a different table (plan / character-pair ratings, AIPLAN at 0x494020..0x494410), no registration code
seen in scope. Suggested file name: keep unk_494020.c, or merge into am.c at 0x494020 between its two parts
(orchestrator decision; join risk: callees defined above callers).

Shared-header additions: none (am.h / ai.h untouched; own header unk_494020.h defines AIC4, AIPLAN, AIGRP, AIREC10,
AIREC2F4 and declares the missing callees 0x4e5360 0x4e52f0 0x4ffc90 0x504e20 0x5085b0 0x50a640 0x4f81c0 0x5ba6b0
plus data 0x5ecfd8 0x7e3f80 0x7e3f84 0x7e4274). ai.h's ai_Unk4f2720(int, AIPOS *) was used for 0x494300's neighbour 0x4946c0.
Note: am.h and ai.h declare ai_Unk4f2720 differently (AIDEC * vs AIPOS *); they cannot be included together.

Ghidra mistakes found: 0x494d40 takes a third argument (n); 0x4947a0 / 0x494830 are `x / (log10(x) + 1.0)` (inline
fldlg2/fyl2x), not log2; 0x494d90 calls util_RandomF (0x42ec20) while 0x494300 calls 0x4e5360 (different generator).
0x4942a0 and 0x4947a0 / 0x494830 / 0x494d40 return int (call __ftol / tail jmp __ftol).

General VC6 lessons
- Clamps: the original tests come from nested min()/max() macros (ai.h's standard `<` / `>` ones) with the
  expression repeated, and the repeated tests stay in the code: `x = min(max(0.0f, x), 1.0f)` (floats),
  `max(min(q->unk04 * 0.25f, ...` inlined, and for ints `max(min(base - d, 2), -2)` with `d` a call result: the
  subtraction is repeated in the macro, so VC6 cannot drop the third (redundant) test. With a named variable
  (`d = base - f(); d = max(min(d, 2), -2);`) it drops it and the block layout changes. For floats the third test
  also stays; `min(max(q->unk04 * 0.25f, 2.0f), 6.0f)` needed the field read inside the macro, not a local float.
- Flag spelling decides `and eax,0x4100; jne` vs `test ah,5; jp`: write `if (a >= b) return ...;` / `if (v > lim)`
  forms (">=" gives `and eax,0x100`), a plain `<` often gives the other. Compare `x * 1.1f > d * 0.9f` not
  `d * 0.9f < x * 1.1f` (operand rank). `if (10000.0 > d) d = 10000.0;` gives `fld const; fcomp st(1)`.
- `(d / x * 0.1f) * 32.0` keeps two fmul; without the parentheses VC6 folds 0.1f * 32.0.
- A float local used only as `sum = acc + v` then compared: write `if (sum >= f) {..break;} if (v > lim) {..break;}`.
- An uninitialised int (`int v;` read on the default path of a switch) loads the dead parameter slot (`mov esi,[esp+0x20]`
  after the loop guard); do not initialise it (`v = send;` moves the load before the guard).
- `f(a) - f(b) + 10` evaluates b first; split into two locals to get a first.
- `sum = f() + g();` keeps the first result in a temp slot (the same slot a later float local `f` reuses), so no
  third float slot: `sum = ai_Unk504e20(..) + util_RandomF(); f = 0.0f;`.

## hv4 (round 22): heavy 0x497660, 0x498c70 [hv4]

### Report
- **Real function starts** (from the AIACT registration code 0x4a1cd6..0x4a2576, eval/decide/exec at
  -0x78/-0x74/-0x70): 0x497660..0x498470 = 7 functions: 0x497660 (decide 11), 0x4979d0 (exec), 0x497af0
  (decide 14), 0x497c80 (exec), 0x497cf0 (decide 15), 0x497ee0 (exec), 0x497fa0 (decide 16).
  0x498c70..0x49a0a0 = 12 functions: 0x498c70 (decide 0x13, 1568 B, one function), 0x499290 (exec),
  0x499400 (decide 0x15), 0x499520 (decide 0x16), 0x499580 (exec), 0x4995d0 (eval), 0x499660 (decide 0x17),
  0x499a90 (exec), 0x499b80 (eval), 0x499c10 (decide 0x18), 0x499f20 (exec), 0x49a020 (decide 0x19).
  Not in scope: 0x497490 / 0x497630 (before), 0x498470, 0x4984d0, 0x498a80, 0x498b80
  (between the two ranges; 0x4984d0 is a decide that 0x498c70 calls directly). am.c ends at 0x49a0a0 for now
  (0x49a0a0 is the next registered exec, probably still am; the strings run on: "sabotage", "pruegel",
  "spionage", "bau_..").
- **Nonmatching (6)**: 0x498c70 (2 B), 0x499c10 (35 B, stack slots), 0x4979d0 (epilogues only), 0x499400,
  0x497fa0, 0x497660. Details below.
- **Shared headers**: none edited. All additions are in include/am.h (mine): AIBTX / AIGOODX / AIBLDX /
  AIFINX views of ai.h's AIBTYPE / AIGOOD / AIBLD / AIFIN, AIPER fields unk00d and flags1c8 and AIJOBX
  fields unk04 / unkac (split out of pads, no rename), globals ai_Unkf84c40/60/d4/f4, macros AI_501190_2/3,
  AI_501030, AI_JOBS1B, AI_JOBNEXT, prototypes for the callees.
- **General VC6 lessons**:
  - Decide callbacks with many failure exits: nested ifs with one final `return 0` give the original's single
    shared `xor eax,eax` epilogue; early `return 0` guards copy the epilogue at each site (0x499660, 0x49a020,
    0x499c10 went from ~900 B / 128 B / 718 B differing to 0 / 0 / 35).
  - An __int64 argument built as a LARGE_INTEGER (LowPart/HighPart) is passed through ecx:eax (and kept in
    registers over two calls); an __int64 constant is pushed as two immediates.
  - `if (f(..) * c < v)` with `(float)(x * -0.5f) + 0.25f`: without the cast VC6 folds to `0.25 - x*0.5` (fsubr).
  - `(float)(v - lim) / v` plus `o1->unk08 = v - lim` (CSE) gives `mov ecx,eax; sub ecx,edx`; a variable
    `d = v - lim` gives `sub eax,ecx; mov ecx,eax`.
  - A named local for `&ai_Chars[bld->owner]` changed the register choice of a whole function (0x497af0).
  - A search loop whose found-code is inside the loop body (`if (match) { ...; return x; }`) matches the
    original's direct exit; `break` + `if (i >= n)` re-tests.
  - `if (b != 1) { if (b != 0) return 0; } else if (...) return 0;` for the b==1-first test order.
  - `!(x >= 1.0f)` gives `and eax,0x100; je`; `x < 1.0f` gives `test ah,5; jp`.
- **Outside scope noticed**: 0x48e3c4.. (another TU) has a copy of 0x499290's "building_upgrade" message code.
  0x4fea50's first parameter is an out pointer too (0x499400 passes &y first, 0x4979d0 passes it second);
  ai.h declares it (int, unsigned short **, int, int). The original 0x498c70 zeroes its t1 twice and t2 never.

### Per function
- ai_Unk497660 (0x497660, 873 B): nonmatching (749 B differ). Pass 2 rewrote it with nested ifs and a `goto
  rate` (one final `return 0`); the epilogues now merge, but register roles differ (orig o1 in edi / o2 in esi,
  ours o2 in ecx) and the loop exits still copy epilogues. Next try: named locals, as in 0x497af0.
- ai_Unk4979d0 (0x4979d0): nonmatching, only the epilogues differ: the original copies `pop; ret` to every
  `return 0` (and returns eax unchanged after a null test), ours jumps to one shared `xor eax,eax` epilogue.
  Tried: the 4fea50 out pointer as its own local / block local / through &s2, if/else tail. All same.
- ai_Unk497af0 (0x497af0): matched in pass 2: the owner pointer in a named local (`c = &ai_Chars[bld->owner];
  v = ai_Unk509ee0(c, ...)`) moves the register choice of the whole function.
- ai_Unk497cf0 (0x497cf0): matched (the search loop returns from inside its body; early returns, `d < 0x32 ? 50 : 30`).
- ai_Unk497fa0 (0x497fa0, 1232 B): nonmatching (~780 B differ, mostly shifted). Kept: `for (i = 0; i <= n - 1; i++)
  ok[i] = 0;` (gives the original `dec; cmp; jl; rep stosd`), `(float)(x * -0.5f) + 0.25f` (without the cast
  VC6 folds it to `0.25 - x*0.5`, fsubr). Left: the two divisions are evaluated in the other order, the
  second copy of the price test is not tail-merged into the first, the 64 bit type mask computes bt late.
- ai_Unk498c70 (0x498c70, 1568 B; Ghidra's range is this one function): nonmatching, 2 bytes: in the else
  branch `h = n % 2 + n / 2` sums into eax, the original into edx (tried split statements, casts, parentheses,
  ternary, reusing i / tot / n). Fixed in pass 2: `(float)(v - lim) / v` with `o1->unk08 = v - lim` (CSE, no
  d variable) at the first two sites, `lim = v - lim` at the third, `(unsigned int)p->unk004 % 8`.
  The original zeroes t1 twice (AI_ZERO(&t1) x2, t2 never): kept as is.
- ai_Unk499290 (0x499290): matched (msg.unk36 = 1 before the msg.time struct copy).
- ai_Unk499400 (0x499400): nonmatching. 0x4a63f0 takes a 64 bit type mask; built as a LARGE_INTEGER
  (LowPart 0xa07d7bb0, HighPart 1) it is passed through ecx:eax as in the original (an __int64 constant is
  pushed as two immediates). Left: the original keeps 1 in ebx (mask high dword, s1/s2.unk08, s2.kind, the
  495a50 argument) and 0.44f on the FPU (fld; fst s1.unk14 ... fstp s2.unk14), ours stores immediates;
  chained assignments do not do it. Looks like an inlined helper filling the AIARGs. The b test needs
  `else if (o1->kind == 0) {} else return 0;` (two tests kept).
- ai_Unk499660 (0x499660): matched in pass 2: nested ifs with one final `return 0` (the early `return 0`
  after the 6f0468 test stays), the 64 bit mask as a LARGE_INTEGER (kept in edi/ebp over both calls).
- ai_Unk49a020: matched: the checks after the b test are nested ifs
  with one final `return 0` (gives the one shared `xor eax,eax` epilogue; early returns copy it per site),
  and the b test is `if (b != 1) { if (b != 0) return 0; } else if (...) return 0;`.
- ai_Unk499c10 (0x499c10): nonmatching, near (35 B): stack slots only (orig cid 0x10, m 0x14, v 0x18; ours
  m 0x10, v 0x14, cid 0x18). Kept: nested ifs with one final `return 0`, the 4a58d0 float argument in a local
  `t`, `c = &ai_Chars[cid]; if (c->unk194 >= 2)`.

## Round 23 solo experiment (2026-10-22): one session, no workers, 30 minutes [solo23]

The orchestrator worked the planned round-23 scopes alone (user experiment): layout first, byte matching
second.

### Report
- 0x49c000..0x49c3c0 sit at the end of unk_49a0a0.c after 0x49b6f0 (0x49b7c0..0x49bd90 go between them).
- Files (am unit, to be joined into am.c by amj later):
  - src/game/unk_494e70.c / .h (g5): 0x494e70 (action table loader, ai_data_2.dfn / ai_method_addon.ini),
    0x495520 (am_RegisterMethod), 0x495770..0x4957a0 (evaluation stack), 0x4957d0 / 0x495a50 (choose the next
    action), 0x495ce0 (execute it), 0x497490, 0x497630, 0x498470. Views G5CH (character) and G5ACT (AIACT with
    the w3 copy block at 0x50).
  - src/game/unk_4984d0.c / .h (g6): 0x4984d0, 0x498a80, 0x498b80, 0x49ee20, 0x49fc90, 0x4a0240, 0x4a0410,
    0x4a3a90 (the ring 0xf83c20, G6RING).
  - src/game/unk_4a3af0.c / .h (g7): 0x4a3af0 (fills the ring), 0x4a47c0, 0x4a4680, 0x4a49a0 / 0x4a4ae0 /
    0x4a4c10 (default eval callbacks), 0x4a4e30, 0x4a4e70.
  - src/game/unk_49a0a0.c / .h (hv5): from 0x49a0a0 (range in notes/tus.md).
- include/ai.h: ai_Unk495ce0 returns int (was void; it returns 1 / 0).

### Findings
- The registration code 0x4a1cd6.. lists these starts in 0x49a0a0..0x49c880: 49a0a0 49a290 49a380 49a390
  49a5b0 49a690 49aa80 49ab60 49ad80 49ae50 49b050 49b120 49b2b0 49b380 49b6f0 49b7c0 49bc50 49bd90 49c000
  49c0d0 49c170 49c190 49c320 49c3c0 49c3e0 49c570 49c700 (0x49c570 / 0x49c700 are "spionage").
- The decide callbacks 0x1c..0x20 share one shape: a G8REC list of goods for 0x4a5760, then "buy" via
  ai_Unk495a50(k, id, 2, ..) or "use" with s.unk10 = action; their execute callbacks (0x49a5b0, 0x49aa80,
  0x49ad80, 0x49b050, 0x49b2b0, 0x49b6f0) scale G5_ACTS[action].w3[0].w from w1[0].w (globals 0xf84e90 ..
  are AIACT fields) and all differ from +0x27 the same way (the original zeroes eax before `test esi`, the
  return value; one fix likely fixes all six).
- 0x4a4e70: 55 B off, the frame is 8 bytes larger in the original (0x480 vs 0x478).
- Duplicate prototypes to unify at the join: ai_Unk4a5760 / 5ba620 / 5cf8a0 (unk_48f250.h has AIPX /
  AIRECX / AIUSE views; mine G5CH / G8REC / G8USE), ai_Unk48fb70 (also in unk_48f250.h), ai_Unk509ee0 and
  ai_Unk509d30 (float returns via casts / int), ai_Unk4a58d0 (b is a float, cast macro G8_4A58D0).

## layout 2026-10-22 (session 1)

- Starts of 0x49c700..0x4a1640 come from the registration code 0x4a16f0..0x4a3a90 (a scan for `c7 45 xx imm32`):
  49c790 49c880 49c8e0 49cb60 49cd30 49cdb0 49cdd0 49ce50 49d030 49d050 49d0f0 49d280 49d2d0 49d460 49d4b0 49d550
  49d5e0 49d600 49d6a0 49d730 49d750 49d7d0 49d7f0 49d8d0 49dc10 49dd90 49def0 49e230 49e350 49e680 49e7a0 49ea90
  49ec10 49ed10 49ee20 49eec0 49f370 49f4f0 49f5c0 49f7f0 49f850 49fb80 49fc90 4a0040 4a0090 4a01f0 4a0890
  4a0940 4a0990 4a0a70 4a0e60 4a1000 4a1170 4a1360 4a13e0 4a1640.
- Callback shapes: decide (int a, G5CH *p, int f, AIARG *o1, AIARG *o2), execute (G5CH *p, AIARG *s1, AIARG *s2),
  eval (float *a, float *b, int i, G5CH *p, int f, o1, o2). 0x49c790 / 0x49c880 are the generic "buy" pair (0x0c).
  Eval callbacks: *a = *b = -1e30f when they refuse, else 0, then ai_Unk4f2720(decision) -> 0x4a4c10 / 0x4a49a0.
  Execute callbacks returning `x ? k : 0` compile to neg / sbb / and.
- 0x49dc10 / 0x49e230 / 0x49e680 / 0x49ea90 (building messages): store order m.type, to, unk0c, unk36, time,
  unk58 matches; the strings name the actions (workers' lodging, town hall, dungeon, well).
- ai_Unk4a63f0's "register constants" are an __int64 mask in a union / LARGE_INTEGER (0x49dd90 matched first
  try, 0x48f010 fixed). Some callers pass 0x28 where the action is 0x29 (0x49d0f0, 0x49d2d0: copy-paste in the
  original).
- Type views: AIBLD fields (type, id, owner, flags5a, unk5c, unk5d) fit; ai_BTypes[k].kind is the building class.

## layout 2026-10-22 (session 2)

- 0x49eec0..0x4a1640: the callbacks of actions 0x33..0x3b: 0x33 AI credit ("AI Kredit", G8OFFER from 0x50a250),
  0x34 build a building of a missing class (0x5043d0 / 0x504290), 0x35 the regulars' table ("stammtisch", tags
  'join' / 'leav' / 'noop' as multi-char constants in AIARG.unk08, G8TABLE to 0x4c5d80), 0x36 / 0x37 / 0x38
  requests (G8REQ to 0x4e9660 / 0x4e9a00 / 0x4e9ce0), 0x38 the office building by the office 0x169 (0x1e..0x21 ->
  kinds 0x17..0x1a; 0x4a05e0 picks a quarter by mutual sympathy), 0x39 office-building upgrade / purchase with a
  credit fallback ("upgr_ai_zunfthaus"), 0x3a / 0x3b children (oldest grown-up child: 0x0a + 4 >= float 0x20).
- 0x4a16f0 registers actions 0x01..0x3b ("DummyUnversehrtheit", ...): generated from the disassembly (blocks end
  at `call 0x495520`; id at [ebp-0x9c], name source address, eval / decide / exec at [ebp-0x78] / -0x74 / -0x70,
  flags at [ebp-0xc]); the same AI_ZERO / strcpy pattern as am.c.
- 0x4a5140..0x4a66f0 (unk_4a3af0.c): build permission (0x4a5140), amt_ExecGebaeudeBauen (0x4a5320: model group
  "%sgebaeude/*%s.ogr", message 0xd), producible good (0x4a5540), inventory records (0x4a5760, void), partner
  finders over the ring 0x4a3a90 (0x4a58d0 / 0x4a5a30) or a random walk over the 768 characters with steps
  0x5ed100 (0x4a5b50 / 0x4a5d30 / 0x4a5ef0 / 0x4a61d0), building finders with the __int64 class mask
  (0x4a63f0 / 0x4a6570, steps 0x5ed140, bit 63 forced), model positions (0x4a66f0, matched with float[4] bounds),
  the AI cannon shot 0x4a67a0 (G9SHOT at 0x6f0698; ballistics table 0x5edfe0 of three cannon types; elevation
  window -15..45 degrees; 0x4ef000 fills a 32-byte record).

## layout 2026-10-22 (session 5)
- Joined into am.c in address order, headers merged into am.h (each part marked with the header it was). Every
  step kept every byte count; three nonmatching functions improved (0x4a61d0 279 -> 53 with unk_4a3af0.c,
  0x4a3af0 2322 -> 2317, 0x493be0 589 -> 576 with unk_4984d0.c): consistent with one unit.
- What clashed and how it was solved (each one byte-checked): ai_Unk4a63f0 (7-int view of 0x48ff50..0x490690 ->
  AI_4A63F0_7), ai_Unk5044b0 (unsigned short id for 0x490690 -> AI_5044B0_US; the int view of am.h is what
  0x498c70 needs, 2 B instead of 807), ai_Unk4c4830 (unsigned char c vs int: rename define for the was-unk_490830.h
  part, G8_4C4830I for 0x49a0a0..), ai_Unk5ba620 (unsigned short vs int id: G8_5BA620 for 0x49a0a0.. and
  0x4a5760, which had no prototype before), ai_Unk4e7c40 (unsigned char k in the was-unk_4a3af0.h part: rename
  define), ai_Unk4a5760 is void, ai_Unk4eded0 returns a pointer, vfs_* from vfs.h. am.h's `(void)` declarations of
  0x490690 / 0x490830 / 0x4908d0 / 0x490b90 are gone (the real prototypes are in am.h now).


## match-base 2026-10-24 (base-4 worker, shard 4/6)

- Reference-only: the 0x58fd40 / 0x58fd90 call pair in amt_ExecGebaeudeBauen (0x4a5320, the AIMSG +0x80 /
  +0x90 fields) now reads ai_ObjectPosition / ai_ObjectAngle; am.h carries the two prototypes (the old
  ai_Unk* names are gone). No am.c code changed.

## match-base 2026-10-25 (base-0 worker, shard 0/6)

- 0x4f2720 calls renamed ai_Unk4f2720 -> gv_GetTableRecord (43 sites; ai.h/am.h still declare the two views, the conflict is unchanged); byte unchanged.
## match-base 2026-10-25 (base-1 worker, shard 1/6)

- ai_Unk494e70 (defined in am.c, 0x494e70) -> ai_LoadActionTable; the ai.c call sites and the am.h /
  ai.h declarations renamed. am.c's AIPER unk170 / unk194 stay (an AI view; AICHAR's fields are bld2 /
  money now).
