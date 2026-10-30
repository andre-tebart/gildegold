# ai.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "ai_" module prefix in its messages (ai_CalcMeisterDiebe, ai_CalcMeisterWache, ai_CalcAngriff, ai_MeisterTrade). Round 21 (ai): TU = 0x47d9e0..0x48e070 from .data 0x5fef04.. and the string block 0x5fef0c..0x5ffd18 in function order; unk_47d960 / 47ec20 / 4817a0 / 4854f0 / 48a4d0 joined; round 22 (ai2): unk_483fc0 / 4864b0 / 487fb0 / 489a70 joined, the whole TU 0x47d9e0..0x48e070 is in ai.c
- Boundaries: separated (proven): 0x47d9e0..0x48e070 from .data 0x5fef04.. and strings 0x5fef0c..0x5ffd18 in function order (round 21)
- Range then: 0x47d9e0..0x48d810
- Was: ai.c, unk_47d960.c, unk_47ec20.c, unk_4817a0.c, unk_4854f0.c, unk_48a4d0.c (round 21), unk_483fc0.c, unk_4864b0.c, unk_487fb0.c, unk_489a70.c (round 22)

## Open work
- **headers (2026-10-30, typing pass 2)**: head.c includes net.h, ai.h, data_views.h, d3_types.h, am_types.h; keeps 5 cast / view macros with @original byte evidence (the address and the matched caller are in each comment): util_RandomU, he_MessageBoxQuickjumpI, gm_Unk505e60S, cm_SendRemoveGoodsI, cm_SendExSellObjectI; views / casts without byte evidence of their own (weak definitions or drafts' spellings, notes/headers.md debt item 1): ai_FallbackBuildingT, ai_PlayerBuildingT, ai_GoodsT, ai_BldsT, ai_BTypesT, ai_CharsT, ai_TimeT.
- AI drafts (ai.c, whole TU joined in round 22): near-misses ai_Unk48a380 9 B (register roles), ai_Unk483d30 13,
  ai_Unk489870 56, ai_Unk487180 350 (size exact), ai_Unk487410 730 (block layout) (notes/units/ai.md); far off
  ai_Unk4801f0 2339/3184, ai_CalcMeisterDiebe 3261/4144, ai_Unk47ddf0 1031/1328 (f1), 47edb0 1618 / 47f700 1867 /
  ai_CalcMeisterWache (f2), ai_CalcAngriff 0x4817a0 / ai_CalcMeisterRaeuber (f3), ai_MeisterTrade 0x48a4d0 (hv2),
  ai_MeisterTradeAllgemein 1414/5008 (stack slots), ai_Unk4854f0 (0x24-flag block layout); round 21: 483fc0 1855 /
  484a60 1839 (47edb0 variants, edi/esi), 486190 554, 4864b0 2133, 487780 699, 487fb0 ~1011, 4889f0 1753,
  ai_MeisterCollectTrans 0x489cb0 1167 (zero register), ai_MeisterTradeFernEinkauf 0x48d010 1460 / ...2 0x48d810 949
  (notes/units/ai.md, sections by worker).
- see notes/units/tr.md, Open work: "util_Random: ai.h, ch.h and tr.h disagree on its prototype (hv3). ..."
- see notes/units/tr.md, Open work: "unk_402140.h declares `unsigned short util_Random(int)`; it is `int util_Random(unsigned s ..."

## f1: 0x47d960..0x47ec20 (src/game/unk_47d960.c, include/unk_47d960.h) [f1]

### Report (round 20, worker f1)
Nonmatching: 47ddf0 ai_Unk47ddf0 (1328 of 1344 bytes, 1031 bytes differ). Logic is complete (partner search over
all 768 characters, then the messages 0x210 / 0x214..0x218). What is left is register allocation / stack
layout: the original keeps `best` (the index) and `bestf` on the stack for the whole loop and gives the registers to
i (edi), the kind pointer (ebp), c (esi) and `id` (bx, partial register, ebx starts as -1 via `or ebx,-1`); ours
puts `best` in edi and spills `id`. The original also reuses the dead parameter slot for the row offset i*0x300 and
for the byte `a`. Tried: struct {float;int;} for best/bestf, declaration orders, address-taken best, do-while vs for,
abs(), unsigned short prototype of 0x505e60 (this one fixed the `mov bx` / `and eax,0xffff; cmp edi,eax` pair).
Ideas not tried: give `best` more stack-only uses, reuse `a` for the min/max temporaries.

TU boundaries: 0x47d960 is the last function of the D2 shape / blit code (only caller d2_Unk47ce60, a 16 bit
"darken the window rect" helper; it is inline asm that expects esi / edx from the caller, no C calling convention)
and belongs with unk_47ab30.c / d2.c. The AI unit starts at 0x47d9e0 (character update, the 13 needs "mood", the
per-frame loop over the 768 characters, the dispatcher 0x47e990 that calls the masters' evaluations by building
kind: 0x4801f0 (ai.c), 0x480e70, 0x481ef0 (ai_CalcMeisterDiebe, ai.c), 0x482f00, 0x4836a0, 0x483a60, 0x483fc0,
0x484a60, 0x47edb0, 0x47f700). TU name suggestion: `ai.c` for 0x47d9e0.. (join with the existing ai.c, which starts at
0x4801f0; this file would come first, the function 0x47d960 stays with the d2 shape code). The scope ends at 0x47ec20:
the AI unit continues (47ec20 / 47ed60 / 47edb0 / 47f700 ... belong to other scopes).

Shared headers: none changed. ai.h is included and used read-only; our own types are in unk_47d960.h:
AIC (full character, 0x21c bytes, with the fields this file touches; AICHAR in ai.h is a subset), AITM (game time:
int d0, ushort hour @4, int min @6, int xy @0xa, 14 bytes; ai.h's AITIME at 0x770ae0 does not match the real
layout, the code reads the dword at 0x770ae6), AIW (view of ai_Watch at 0xf895a0 with the character pointers).
ai.h prototypes that are wrong for this code (worked around with a function pointer cast, not changed):
ai_Unk53e400 is called with 3 arguments (1, 0, 0xb), ai_Unk501d80 takes an int (xor/mov zero extension at the call).

General VC6 lessons
- Clamps: windows.h `min(max(0.0, expr), 1000.0)` gives exactly "x < 0 || x < 1000 ? (x < 0 ? 0 : x) : 1000" with the
  expression evaluated once (`fld st(0)` copies) and double constants; a float local plus ifs does not.
  Use `a >= b` (not `b <= a`) to get the `and eax,0x100; jne` compare form.
- `abs()` from stdlib.h is inlined (cdq / xor / sub) under /O2 (/Oi).
- `memset(&s.f, 0, 16)` gives four stores through a copied pointer register (esi) that a plain pointer loop does not.
- `unsigned char` global tested as `!(g & 0x80)` gives `test cl,cl; js`; `signed char >= 0` gives `jl`.
- Looking up `ai_Chars[idx].id != 0xffff` first and `c = &ai_Chars[idx]` afterwards changes the register that holds
  idx*27 (match of 47e560); the same value through `c->id` does not.
- Variable reuse: the original reused one int for the ftol result and the later delta (`whole`, 47e560): the frame
  shrank by 4 bytes only when it was a single variable.
- A prototype parameter of `unsigned short` makes the caller load `mov bx,[mem]` without extending (garbage upper half
  stays in ebx); the comparison with an int then does `mov eax,ebx; and eax,0xffff`.
- A callee declared with an `unsigned char` parameter gets no `xor eax,eax` before `mov al,[mem]`; declared `int` it does.
- Out-of-line vs inline `return`/branch order follows source: `if (k != 0x1f) f(); else g();` chains without `return`
  hoist the shared `push esi`; separate `return;` statements do not (47e990).
- `__asm` bodies compile verbatim; the prologue pushes of the registers the asm names (ebx esi edi) come from the
  compiler (47d960 matched on the first try).

Outside the scope: 0x47d960 uses d2_Unk5fed84 / d2_Unkf8a634 (declared in unk_47d960.h; unk_47ab30.h declares the
first as well, identical type).

### Open
- 47ddf0 register allocation (see above).

## ai2 -- round 22: AI TU join 2, prototype / view unification, near-misses [ai2]

### Report
Scope: join unk_483fc0.c / unk_4864b0.c / unk_487fb0.c / unk_489a70.c into src/game/ai.c, unify the views and
prototypes into include/ai.h, near-misses. No fresh functions.

Join (step 1): src/game/ai.c now holds the whole AI TU 0x47d9e0..0x48e070 in address order (0x483fc0 / 0x484a60
before 0x4854f0, 0x486190 after it, then 0x4864b0..0x489870, 0x489a70..0x48a380, 0x48a4d0 / 0x48bc80, 0x48d010 /
0x48d810). The join itself changed no byte (compiled bytes + relocation targets of all 55 functions compared before /
after). git rm of the four .c files, their four headers and the compatibility headers
unk_4854f0.h / unk_48a4d0.h (nothing includes them any more).

Names (log strings): 0x489a70 ai_MeisterBewaffnen, 0x489cb0 ai_MeisterCollectTrans, 0x48d010
ai_MeisterTradeFernEinkauf, 0x48d810 ai_MeisterTradeFernEinkauf2 (same strings, sells entry 0 first).

Header (include/ai.h, step 2):
- Views folded into the ai.h types (same offsets / types, no byte change): AIEN -> AIENTRY (new fields bld 0x14,
  store 0x18, unk28 / unk2c / unk30 floats, unk50 replace the pads), AIS / AISLX -> AISLOT (alias bld = unk08, store
  0x0c, alias unk28 = amount, unk38; price = unk1c, key = unk20, need = unk34), AIENX -> AIENTRY (key = unk34, need =
  unk4c), AICHX -> AICHAR (sk81..sk84 in a union with name[], unk178 at 0x178; money = budget), AIBLDX -> AIBLD (unk25
  in a union with name[], unk5c), AIJOBX -> AIJOB (unkb0 / unkb4 / unkb8; idac = u.w.lo), both AIBTYPEX -> AIBTYPE
  (nslots 0x22, slot[0x100] 0x23, k230 = unk230, k231, k232, min0 / min1 0x240, cap247 = unsigned alias of unk247),
  AIGOODX -> AIGOOD (unk22 in a union with name[]), AIOBJT -> AIOBJ (id -> unk02, next -> unk06, bld -> unk1c),
  AIWATCHW -> AIWATCH, AIINFOW -> AIINFOP, AIMSG unka0 (was AIMSG_A0), AITIME min (was AI_TIME6). The macros
  F5_*, AIC / AI_CHX / IC, AI_SLX / AI_ENX, AI_CH_178, AIBTYPE_247 and the round-21 compatibility typedefs / macros
  (AIENT, AISLT, AIGD, AIOBJX, AIITEM, AIJOBW, AITMSG, AI_ENT, AI_SLT, AI_GD, ai_Money, AI_PROD, AI_GNAME,
  ai_Unk4817a0 / 48a4d0 / 48bc80) are gone (no user left). Own types moved in: AISTORE (fields unk04 / unk08 / unk10 /
  unk14), AIFIN, AIREC, AIMAPENT; macro AI_KIND; AITIMEX dropped (unused).
- Prototypes: ai_Unk487410 / 489300 / 489480 return int, 487180 / 487410 take AIENTRY *, 487ea0 / 489480 / 489870
  added; ai_Unk501190(int n, ...) (variadic: CalcMeisterWache pushes (1, 6), 0x4864b0 pushes (1, 0, 0x47));
  ai_Unk524260 takes an int id (every caller in the exe zero-extends: `and eax,0xffff; push eax` or `xor ecx,ecx; mov
  cx,..`; with int, 0x47edb0's call site is now byte-identical to the original); ai_Unkf8a0a0[10] / ai_Unkf8a0e0[16];
  ai_Unk7e3f34 rows are AIOBJ; callees from the f-headers added in address order (4bc670, 4cd9d0, 4f15e0, 4ffc40,
  500c20, 501b50, 506430, 5086b0, 508d70, 509b10, 50ab20, 50cd40, 5176d0, 5190a0, 53dee0, 5bb2d0), ai_Unk7e3f3c /
  ai_Unk7e4b20 externs.
- Effect of the int id of 524260 on the other callers (nonmatching, register noise around the now-correct call
  pattern): 47edb0 1614 -> 1618, 47f700 1890 -> 1867, 4801f0 2356 -> 2339, 483fc0 1849 -> 1855, 484a60 1836 -> 1839.

am.h (hv4's file, not edited) must drop / change:
- line 146 `AIBLD *ai_Unk501190(int a, int b, int c);` -> drop (ai.h: `(int n, ...)`; gives C4031 / C4028 now).
- duplicates of ai.h, same text, can go: ai_Unk6f1bc5 (98), ai_Unk4ffe90 (145), ai_Unk50c550, ai_Unk4c47e0,
  ai_Unk4c49a0, ai_Unk4c4b00, ai_Unk5176d0 (236).
- older conflicts (warnings before this round too): ai_Unk4f2720 (141, AIDEC * vs AIPOS *), ai_Unk4c3ba0 (223,
  AIPER * / int vs void * / unsigned int), ai_Unk4c3c00 (224, int field vs short field).
- AIJOBX (am.h's own, 0xb0 / 0xb4 / 0xb8 / 0xbc) could become AIJOB now (ai.h has unkb0 / unkb4 / unkb8; unkbc
  would need adding). ai.h no longer defines any AIJOBX, so there is no clash.

Near-misses (pass 2):
- 0x489660 ai_Unk489660: **matched** (13 -> 0). `n--;` duplicated into both arms after the call instead of once after
  the if/else: VC6 tail-merges it but schedules it before the flags store.
- 0x483a60 ai_Unk483a60: **matched** (38 / 33 -> 0). `if (NSlots) { if (NEntries) { if (cmp) d010 else d810 } else
  d010 } else if (NEntries) d810`: the d010 call written in both arms makes VC6 hoist the common (ch, obj) pushes
  above the NEntries test and then tail-merge the two calls (the `||` form shares one call block, so no hoist).
- 0x48a380 ai_Unk48a380: 36 -> 9. `id = car->unk02; fill = car->unk12;` read first, then stores dst[0], car, unk84,
  n = 100 - fill (brute-forced 230 orders / temps). Left: register roles only (ours id eax / fill ecx / n edx, orig
  ecx / edx / eax); declaration order, names, block scope, `n` temp, `-k + 100` change nothing.
- 0x489870 ai_Unk489870: 57 -> 56 (`sk81 < sk82` operand order). Left: ch / n register roles (ebp / ebx swapped) and
  the original pushes &msg before the `sk82 >= sk84` test (store of 4 after the push). Tried: duplicated calls in
  both inner arms (both orders, with the outer if swapped), ?: for arg[1], a value local, no-alias / decl order.
- 0x487410 ai_Unk487410: 730 B unchanged, asm diff 359 -> 143 lines (`unk40 > unk36` operand order). Left: block
  layout (orig puts the "special item" else-block inline and the main block after the ret) and the swapped stack
  slots of j / need offset. Tried: `||` chain with swapped arms, negated test, continue form, inline helper
  functions (static __inline returning the test), for / while / do-while k loop: all byte-identical.
- 0x483d30 ai_Unk483d30: 13 unchanged. Orig loads *p right after the dword store of msg.time+8 (before the word
  store and unk36). Tried: orders time / arg1 / unk36, *p in a local at 7 positions, volatile / unsigned reads.
- 0x487180 ai_Unk487180: 350 unchanged. The original keeps `found` in eax around the j loop (xor eax,eax gives both
  zero stores; the j >= N exit reuses the `test eax,eax` of the recursion result; the found exit is threaded past the
  test). Tried: while / for / while(1) loop forms (all far worse), `return 0` in both arms, all zero-init spellings
  (identical).

VC6 lessons (for vc6-matching.md):
- Duplicating a statement / call into both arms of an if/else is not neutral: VC6 tail-merges the copies, but the
  scheduling (489660: `n--` before the flags store) or common-prefix hoisting (483a60: argument pushes hoisted above
  the branch, the original's `push ebp; push esi` before `test eax,eax`) follows the duplicated form. Pushes before a
  test that both arms need = the call was written in each arm. Hoisting needs both successors to start with the
  same instructions (same registers) and to have one predecessor each, so `A || B ? f : g` (shared call block) never
  hoists.
- A store order the original cannot get from any statement permutation (48a380: loads above the first store) can
  come from reading the fields into locals first.
- Stack-slot tie-breaks also move with identifier names: renaming four functions (ai_Unk489cb0 ->
  ai_MeisterCollectTrans ...) moved 0x483a60 from 38 to 33 bytes with no other change.
- `if (a < b)` vs `if (b > a)` picks the cmp operand order (and the jcc); fixing it can shrink an asm diff a lot even
  when the byte count stays (487410: 359 -> 143 lines).
- VC6 gives identical code for for / while / do-while k loops, if/else with swapped arms, negated chains and
  `static __inline` predicate helpers: block layout of 487410 is not decided by any of these.

TU boundaries: nothing new (AI TU 0x47d9e0..0x48e070 as found in round 21). Outside the scope: notes/tus.md row for
ai.c needs the round-22 update (all four files joined).

## f2 (round 20): 0x47ec20..0x4817a0 [f2]

### Report
- **Nonmatching (3, drafts translated from the Ghidra export)**:
  - ai_Unk47edb0 (0x47edb0, 2304 B): ~1610 B differ. Logic is the same as 0x4801f0's first half; remaining diff is
    register allocation (ours ch=ebp / found=ebx, original ch=ebx / found=ebp; entry pointer and index registers
    swapped) and the perm loops: the original keeps `ch` in a stack slot inside them (more live values:
    it addresses ai_Goods as base + item*0x40 + item) and loads b from [esp+0x14] (ours 0x10).
  - ai_Unk47f700 (0x47f700, 2640 B): same as 47edb0 plus the 0x20 / 0x200 flag filters; ~1890 B differ.
  - ai_CalcMeisterWache (0x480e70, 2320 B): ~1630 B differ. The original keeps ch on the stack in the patrol blocks
    (ours: register) and idle count in ebp / ch in edi; frame 0x50c vs ours 0x508 (the original has a `name`
    pointer local across the check block, which is later tested `if (name) return;`, apparently a source bug).
- **TU**: ai.c. 0x480e70 (ai_CalcMeisterWache) sits between ai_Unk4801f0 and ai_Unk4817a0 in ai.c; 0x47ec20..0x47f700
  precede it. Suggest: join unk_47ec20.c into ai.c in address order (0x47ec20.. before 0x4801f0, 0x480e70 after it).
  Module tag "ai_" (messages "ai_CalcMeisterWache(): ...", "Produktion von %s starten").
- **Headers**: include/unk_47ec20.h (includes ai.h, read-only). No shared header changed. Wanted in ai.h at the join:
  `int ai_Unk4817a0(AICHAR *, int)` (ai.h says void; 0x480e70 tests its result, I declared `ai_Unk4817a0r` with the
  address comment), AISLOT fields 0x08 / 0x14 / 0x24 / 0x2c, AIGRID byte at +4, AIWATCH layout past 0x20
  (cnt[4] at 0x20, entry copies at 0x40 and 0x1a0, slot copies [16] at 0x300; see AIWATCHX), the AIINFO tail
  (the shuffled index array follows AIINFO directly: AIINFOP).
- **Outside scope**: none found. 0x47ed60 (ai_Unk47ed60) is the "request items" helper called from ai.c.

### General VC6 lessons
- A named local for an intermediate (`m = a * b; if (c() - (m >> 2) >= 5)`) takes a callee-saved register and
  spills the loop counter; the original had the expression inline (temp spilled to the stack). Inline it.
- A clamp `x = f(); a->m = x; if (x >= lim) x = lim; a->m = x;` (local, two stores) gave the original
  `cmp eax,edi / mov [m],eax / jl / mov eax,edi / mov [m],eax`; storing straight into the member gave a different shape.
- `util_Random(n)` result kept in an `int` (not unsigned short) gives `mov si,ax; and esi,0xffff`.
- `unsigned short < 7` is `cmp cx,6; jbe` in the original: write `<= 6`. `(uint + ushort) % 3` is `xor edx,edx; div`.
- `(char)x != 0` on an unsigned char field compiles to `test cl,cl; jbe` when the source says `x > 0`.
- Comparison operand order decides `cmp a,b / jl` vs `cmp b,a / jg`: `limit <= x` and `x >= limit` differ.
- `if (!f(a)) { if (g(a)) {...} } else {...}` puts the else block after the second call (the out-of-line success block).

## hv1 (round 19): heavy 0x4801f0, 0x481ef0 + d2 near-miss tail [hv1]

### Report
- **Nonmatching (5)**:
  - ai_Unk4801f0 (0x4801f0): draft, ~2360/3200 B differ (ours 3184 B).
  - ai_CalcMeisterDiebe (0x481ef0): draft, ~3250/4112 B differ (ours 4144 B).
  - d2_Unk46aa00 (4/4720), d2_Unk46d040 (10/480), d2_Unk46bf60 (17/624): unchanged.
- **New files**: src/game/unk_4801f0.c + include/unk_4801f0.h (the AI types: AICHAR 0x21c at 0x770b80,
  AIBLD 0xa9, AIENTRY / AISLOT, AIMSG 0xf8, AIWATCH at 0xf895a0, ...), src/game/unk_481ef0.c +
  include/unk_481ef0.h (includes unk_4801f0.h).
- **TU name suggestion**: `ai.c` (tag). 0x481ef0's messages are "ai_CalcMeisterDiebe(): ..."; the same string block
  (0x5fef0b..0x5ff800) holds ai_CalcMeisterWache, ai_CalcAngriff, ai_CalcMeisterRäuber, ai_MeisterCollectTrans and
  the production messages ("Produktion von %s starten"). 0x4801f0 (production/plant evaluation, "PFLANZBAR") shares
  the types, globals and callees with 0x481ef0, so both are very likely one TU: join unk_4801f0.c and
  unk_481ef0.c (and the headers) into it.
- **0x481ef0 is one function** (4112 B; nop padding only after its last `ret`); Ghidra merged nothing.
- **Shared headers**: none edited (d2.h untouched).
- **Outside scope**: tr.h declares `int util_Random(unsigned short)`, ch.h `unsigned short util_Random(int)`. The AI
  callers use the result as an unsigned short (`and eax,0xffff` after the call), so unk_4801f0.h declares the ch.h
  form; the TU join should settle on one prototype. The AI callees (0x47ed60, 0x4817a0, 0x4854f0, 0x486190,
  0x487180..0x48a4d0, 0x4bc5a0.., 0x4ffe90 (varargs object list search), 0x53e400 / 0x53e300 (job list)) have no
  source yet; their names are ai_Unk<addr> placeholders in unk_4801f0.h / unk_481ef0.h.

### General VC6 lessons
- Three stores to members of one struct followed by a member test: the source order (`min = a; max = b; value = c;`)
  only shows once the tested member is read into a local (`fl = o->u.s.flags; if (fl & 0x10)`); without the
  local every store order gave the same bytes (d2_Unk475cb0).
- `x += t` compiles to `add [mem],reg` when the member is written through a different access form than its earlier
  loads (`d2_Wins[win].unk250 += t` after `D2WO(o).unk250` reads): VC6 does not CSE across the two forms
  (d2_Unk472760).
- `else if (A && B) {...}` vs `else { if (A) { if (B) {...} } }` decides which of two duplicated tails each failing
  test jumps to (only branch displacements differ) (d2_Unk472760).
- Dead parameter slots for locals: a K&R `int` parameter copied into an `unsigned char` local, plus a separate
  variable per branch (`h`, `h2`), gave the original slot assignment (d2_Unk475730).
- A local whose address is taken (`f(&i, &j)`) stays in its stack slot everywhere: as a loop counter it is
  stored each iteration and the loop is not strength-reduced. Use another variable for unrelated loops.
- A loop over a global array that compares the strength-reduced pointer with a signed `jl` is an index loop
  (`for (i = 0; i < 768; i++) a[i]...`); a pointer loop gives `jb`.
- Counters reloaded from memory after each record copy (`rep movs`) into a global array: the counter and the
  array are members of one global struct (aliasing), here ai_Watch at 0xf895a0.
- A `switch` with cases 0..3 gives a jump table; the original's compare chain (1, 0, 3, then `<= 1` return) is an
  if/else chain.
- Neither textual operand order, single parentheses around a member, casts, nor declaration order of the frame
  pointers changes the `w->x` / `w->w` load order (d2_Unk46aa00, d2_Unk46bf60): VC6 canonicalizes the sum.

### Per-function notes
- ai_Unk4801f0 (0x4801f0, 3200 B): draft. Register allocation (ch in ebx vs ebp, a in ebp vs ebx) and stack
  slots; the last loop's type 3/4/0x28 ai_Unk4889f0 calls are tail-merged differently (orig: 3 and 4 share one
  call + check, 0x28 its own call and check, the rep movs shared). 1 `uv run try` run (else-branch / while forms of the
  `o` search: worse).
- ai_CalcMeisterDiebe (0x481ef0, 4112 B): draft. Block layout of the r == 1 (kidnap) branch (orig puts its grid
  loop tail after the r == 3 branch), n / the constant 8 in ebp / edi swapped, stack slots. 1 `uv run try` run
  (float compare forms: `!(max > v)` and `!(util_RandomF() < 0.95)` kept).
- d2_Unk46aa00 (4/4720): `w->x + w->w + 4` loads x / w in the other order. 2 runs (all 11 term orders and
  parenthesizations, int / short casts, pointer forms): no change.
- d2_Unk46d040 (10/480): x / y in esi / edi swapped. 2 runs (`register` on x / y / both, local copies assigned in
  both orders, early or late): no change (copies are propagated away).
- d2_Unk46bf60 (17/624): `len = w->w - r->w - l->w` and `w->w - r->w + w->x` evaluated in canonical, not textual
  order. 3 runs (parentheses 24 combos, term orders 16, l / r / m / w declaration orders 17): no change.
- d2_Unk475cb0: matched in run 3 (see lessons). d2_Unk472760: matched in run 2. d2_Unk475730: matched in run 2.

## f3 (round 20): 0x4817a0..0x483fc0, AI masters part 2 [f3]

Files: src/game/unk_4817a0.c, include/unk_4817a0.h (includes ai.h read-only).

### Report
Nonmatching (closest first):
- 0x483d30 ai_Unk483d30: 13 bytes, one load of `*p` is scheduled before the 16-bit store of msg.time.unkc in the
  original and after it in ours (permutations of the five msg statements and a `pv = *p` local tried).
- 0x483a60 ai_Unk483a60: 33 bytes; the original pushes (ch, obj) before the NEntries test of the NSlots != 0 arm
  (shared by the d010 / d810 calls). `if/else`, `goto`, `?:` on the function designators all emit the pushes inside
  the arms.
- 0x482f00 ai_CalcMeisterRaeuber, 0x4817a0 ai_Unk4817a0: far off (stack slots of the float temps, `w` statement
  split, return-0 layout). 482f00 uses `volatile int z` to reproduce the stack slot / read of a zero local.

TU: both files continue ai.c ("ai_" messages: ai_CalcAngriff at 0x4817a0, ai_CalcMeisterR\xe4uber at 0x482f00,
ai_CalcMeisterWache text inside 0x482f00). Suggested: join into ai.c at round close (0x4801f0, 0x481ef0 are the
neighbours; 0x4817a0 sits between them, so the real file is one TU with 0x481ef0 after 0x4817a0). The scope ends at
0x483fb3 (0x483ee0 ends there, nop padding to 0x483fc0). 0x4864b0 and later belong to the next scope.

Names: 0x4817a0 is `ai_CalcAngriff` (string "ai_CalcAngriff(): Meister %s ..."), ai.h calls it ai_Unk4817a0 and
declares it `void`, but the original returns 0 / 1 in eax: unk_4817a0.c defines it as `int` through a
`#define ai_Unk4817a0 ai_Unk4817a0_void` around the include of ai.h. Please change the prototype in ai.h to
`int ai_Unk4817a0(AICHAR *ch, int n)` (callers ignore the value) and rename. 0x482f00 = ai_CalcMeisterRaeuber
(string "ai_CalcMeisterR\xe4uber"). 0x4836a0 is the "Bankmeister" money balance; the other names stay ours.

Shared-header additions: none (ai.h untouched; hidden fields reached through macros in unk_4817a0.h).

Outside scope: ai.c's AI_FILL_IDS macro has its backslash continuations flattened to blanks (still compiles).
Diebe code in ai.c uses `A <= B` + `continue` for the best-candidate compare; the original of the same pattern
here (0x4817a0 / 0x482f00) is `test ah,5; jp` with the *new* candidate's product evaluated first.

### VC6 lessons
- A compare that comes out as `cmp a,b` with swapped operands / the wrong jcc: write the condition negated
  (`!(v < m * 1.5)`, `p.x - 2 > v`, `v > m * 2`, `m - sum <= 0`); 0x4836a0 went from 125 to 0 bytes off with
  exactly this. `uv run try` with a list of equivalent spellings solves these in one run.
- A callee taking `char` / `unsigned char` receives `mov cl,[mem]; push ecx` (no zero extension); declare the
  parameter as a char type (4f22a0, 508660, 4c47e0 third argument) and `short` for the `mov dx,..; add edx,0x65` push.
- Two locals that look like one slot in the original (a value in the dead parameter slot read twice): use one
  variable (v for the new position and for the bank balance).
- Loop `ok = 1; while (i < N) { ...; if found ok = 0; test ok at the bottom }` = `for (i = 0; ok && i < N; it++, i++)`
  (not `i < N && ok`, not a `break`).
- Comparing a float product against another: `!(A > B)` with A the first call evaluated vs `A <= B` differ in
  which side is evaluated (spilled) first.
- `a = (8.0f - r) * w * 0.5; a = Random() * a * 0.01;` is sunk behind the Random call by the optimizer; the
  original keeps the first product before the call (unsolved).

## ai -- round 21: AI TU join, ai.h unification, near-misses [ai]

### Report
Scope: join the AI files into src/game/ai.c, unify the AI headers into include/ai.h, near-misses. No fresh functions.

Nonmatching (13), bytes / asmdiff lines before -> after this round:
- 0x483d30 ai_Unk483d30: 13 / 2 (unchanged). The original loads `*p` before the 16-bit store that ends the
  msg.time copy; ours after. Tried: all orders of the msg statements, memcpy / pack(1) / field-wise time copy,
  `p[0]`, volatile read, int store for arg[2]. Not found.
- 0x483a60 ai_Unk483a60: 33 / 8 -> 38 / 18. +5 B is a stack-slot tie-break (obj / ok swap) that flips with the
  header / TU contents, not with any function above it (bisected: deleting functions does not bring it back; the
  old file plus `#include <windows.h>` gives the same 38). The real difference stays the (ch, obj) pushes the
  original hoists above the NEntries test; if/else, swapped tests, `?:` and function-pointer forms tried.
- 0x48bc80 ai_MeisterTradeAllgemein: 3352 / 270 -> 1414 / 259: the stock-sale loop reads `ai_Entries[i].unk3c`
  directly instead of a local `v` (strength-reduction base = &e->unk3c as in the original). Left: stack-slot
  packing (the original shares f with a pointer temp and v with `sold`; ours shares f, v and sold).
- 0x48a4d0 ai_MeisterTrade: same loop change, 1369 -> 1316 lines (4884 -> 4894 B, size 6048 -> 6032).
- 0x481ef0 ai_CalcMeisterDiebe: f3's hint applied (`if (!(new * w > best * bestw)) continue; bestw = w; best =
  bb;`): the compare now comes out as the original's `fcomp; test ah,5; jp` with the new product first. Lines
  1387 -> 1379, bytes 3253 -> 3261 (the function's block layout differs: the original places the r == 0
  (burgle) branch before the rest of the r == 1 (kidnap) branch, so most bytes are shifted).
- 0x4854f0: 172 lines unchanged: the 0x24-flag block of the kinds 14/8/32 section is laid out after the 0x204
  block in ours, inline in the original; `else if`, negated test, `!=`/`&&` form all compile identically.
- Far off, unchanged: 47ddf0 (420 lines), 47edb0, 47f700, 4801f0, 480e70, 4817a0 (ai_CalcAngriff, +5 B tie-break
  noise from the join, same as 483a60), 482f00.

TU boundaries:
- AI TU = 0x47d9e0 .. 0x48e070 (exclusive). Evidence: its .data starts at 0x5fef04 (1.0f, 0.5f: ai_Unk5fef04 /
  5fef08, written by 0x47e560; 0x5fef00 = 50 belongs to the asm drawers 0x47cf00..0x47d710 before it), then its
  string literals 0x5fef0c..0x5ffd18 in function order (0x47e360, 0x47e560, 0x47e990, 0x47edb0, 0x4801f0,
  0x480e70, 0x4817a0, 0x481ef0, 0x482f00, 0x4836a0, 0x487fb0, 0x4884b0, 0x488740, 0x489a70, 0x489cb0, 0x48a4d0,
  0x48bc80, 0x48d010/0x48d810) with no other data block in between. At 0x5ffd1c a plain global (-1, read by
  0x48e070, "building_upgrade") starts the next obj: 0x48d810 is the last AI function.
- 0x47d960 (asm darkening blit, now in unk_47d600.c, not mine): the padding before it is 0xcc (the end of the asm
  drawers' obj, notes/units/shp.md) and after it nop, as inside C objs. So it is not in unk_47d600.c's obj; it is
  either the first function of the AI obj or an obj of its own (it has no data). Left where it is.
- Joined into ai.c in address order (unk_47d960.c f1, unk_47ec20.c f2, unk_4817a0.c f3, unk_4854f0.c,
  unk_48a4d0.c hv2 + the old ai.c); no matched function changed. The functions of the other scopes
  (0x483fc0, 0x484a60, 0x486190.., 0x4864b0, 0x487fb0, 0x489a70, ...) belong to the same TU.

Header (include/ai.h, the only AI header now):
- One layout per table: AICHAR (f1's AIC merged: mood, sel, budget 0x1b8, prod 0x1bc, flags1cc, partner ...),
  AIOBJ (0x43 bytes, the object record: id/type, unk02, count, list, unk1c / cnt; the workshop level bytes at
  0x1c / 0x1d are now lvl1c / lvl1d), AIENTRY (AIENT merged), AISLOT (AISLOTX / AISLT merged), AIGOOD (name, in[]),
  AIBTYPE (unk223, unk230, unk247), AIJOB (u.id / u.w, in / out / amount, unkf1), AIMSG (AITMSG's trade fields
  in a union with arg[], amount[], unk89, unk9d), AITIME (AITM's min / xy in a union with unk6 / unk8 / unkc),
  AIWATCH (AIW / AIWATCHX merged, slots[16]), AIGRID (used), AIPOS, AIRECIPE. AIINFOP stays a separate type
  (an AIINFO local with perm[32] behind it; merging would grow 0x4801f0's frame).
- Kept for other files: typedefs AIENT, AISLT, AIGD, AIOBJX, AIITEM, AIJOBW, AITMSG; macros AI_ENT, AI_SLT,
  AI_GD, ai_Money, AI_PROD, AI_GNAME; #define ai_Unk4817a0 / ai_Unk48a4d0 / ai_Unk48bc80 to the new names.
  Field names that two files spelled differently are anonymous-union aliases (need1c / unk1c, count / unk0e ...).
  Changed meaning: AIOBJ.unk1c is the int at 0x1c now (the bytes are lvl1c / lvl1d); AIOBJX's int id at 0x02 is
  unk02 (id is the ushort at 0).
- Globals: ai_Unk6f1bc8 AIBLD *, ai_Unka68d2c char *, ai_Unkf8a0c8 / ai_Unk6f0468 AICHAR *.
- Prototypes: ai_CalcAngriff (0x4817a0) and ai_MeisterTrade / ai_MeisterTradeAllgemein (0x48a4d0 / 0x48bc80)
  return int; ai_Unk53e400(int n, ...) is variadic like ai_Unk4ffe90 (n key/value pairs: (1, 0, 0xb) and
  (2, 0, k, 3, id)); ai_Unk501d80 takes an int (callers do `and eax,0xff; push eax`); ai_Unk4c3c00 takes a short
  field (0x4836a0 needs it); ai_Unk4c3ba0 takes void *; ai_Unk50aaf0 / 50ab50 take void *.
- include/unk_4854f0.h and include/unk_48a4d0.h are now compatibility headers that only include ai.h (the
  round 21 workers' new files include them).
- ai.c no longer includes windows.h (only net.h needed it, for SOCKET: typedef'd in ai.c). With windows.h,
  0x480e70 lost 24 diff lines (694 -> 718) through a tie-break; without, it is back at 694.

VC6 lessons:
- Stack-slot / register tie-breaks of nonmatching functions depend on the header contents of the TU (adding
  windows.h to an unchanged file moved 0x483a60 from 33 to 38 bytes), not only on the functions above. A join
  can shift near-misses by a few bytes without any TU meaning; bisecting with deleted functions tells the two
  apart (non-monotonic = noise).
- A loop that reads a field once into a local and once more as the field strength-reduces to a different base
  than one that uses the field everywhere: `if (e->unk3c > ..) { e->unk44 = max(5, e->unk3c / 4); .. }` moved
  0x48bc80 from 3352 to 1414 bytes.
- VC6 anonymous structs / unions in C (MS extension) compile under /W1 without warnings and give the same code:
  usable to keep two names for one field in a shared header.
- `if (A) {..} else if (B) {..}`, `if (A) {..; continue;} if (B)`, and the negated forms of a six-term `||`
  chain compile to the same blocks: the then-block placement is not decided by the source branch order there.

Outside the scope: 0x47d960's obj (above). The AI TU continues past 0x48bc80 to 0x48d810.

### Per-function notes
- 483d30: see report. Orig: `mov dx,[ai_Time+0xc]; mov [msg.time+8],eax; mov eax,[ebp]; mov [msg.time+0xc],dx;
  mov byte [msg.unk36],1; mov [msg.arg1],eax`.
- 483a60: orig pushes ebp/esi (obj, ch) before `test NEntries`, the d810 call of the NSlots == 0 arm jumps into
  the shared `call d810`.
- 48bc80: slots orig f 0x14, temp 0x1c, v/sold 0x24, car 0x28, s 0x2c; ours f/v/sold 0x18, temp 0x20, s 0x24.

## f4 (round 21): 0x483fc0..0x4864b0 [f4]

### Report
- **Nonmatching**:
  - ai_Unk483fc0 (0x483fc0, 2624 B): 1849 B differ. Variant of ai_Unk47edb0 with the 0x400 entry flag (no 0x20 / 0x200
    filters), the extra calls 487ea0 / 489480 / 489870 / 483ee0 / 483d30 and the `job == 0 || job->kind == 4` filter on
    the idle people. Same register-allocation problem as 47edb0 (the original reads e->unk40 through the entry
    pointer; ours folds it to the absolute ai_Entries address and flips the compare operands).
  - ai_Unk484a60 (0x484a60, 2576 B): 1836 B differ. Same as 483fc0 without the 4 extra calls (calls 487b40 instead of 487ea0).
  - ai_Unk486190 (0x486190, 736 B): 554 B differ. Pure register allocation: original has ai_NSlots in ecx and a zero in
    esi, ours count in esi and zero in edx; 3 `uv run try` variants (declaration order, global count in the search loop) did not change it.
    The original keeps two pointers into the goods lists (ebp / edi), see Ghidra.
- **TU**: ai module ("ai_" strings); the scope is one range of ai.c's TU (0x4854f0 has its own file unk_4854f0.c). Suggest joining
  into ai.c at the AI join with 0x4854f0 in address order.
- **Shared headers**: none changed. unk_483fc0.h includes unk_4854f0.h (AIENT / AISLT / AIGD / AIJOBW / AIRECIPE) and adds AIWATCHW,
  AIINFOW; ai_Unkf8a0a0 / ai_Unkf8a0e0 are declared again (also in unk_47d960.h, same types).
- Naming hint: 0x483fc0 / 0x484a60 are two master-kind variants of the production evaluation (like 47edb0 / 47f700); 0x486190
  fills ai_Slots with the goods of the two fixed lists at 0xf8a0e0 / 0xf8a0a0 (the second loop reads the item for
  ai_Unk5089c0 from 0xf8a0e0[k], a quirk of the original, kept) and then adds the running jobs' (kind 0xb) in / out goods.

## hv2 (round 20): heavy functions 0x4854f0, 0x48a4d0, 0x48bc80 [hv2]

### Report
- Nonmatching (all have full drafts): ai_Unk4854f0 0x4854f0 (unk_4854f0.c, 172 asm diff lines, 3248/3232 B),
  ai_Unk48bc80 0x48bc80 = ai_MeisterTradeAllgemein (unk_48bc80.c, 270 lines, size exact 5008 B, rest is mostly
  stack slots), ai_Unk48a4d0 0x48a4d0 = ai_MeisterTrade (unk_48a4d0.c, 1369 lines, 6048/6064 B).
- No merged functions: disasm shows one prologue/epilogue each.
- TU: all three belong to ai.c ("ai_" tag; strings ai_MeisterTrade(): / ai_MeisterTradeAllgemein(): sit next to
  ai_CalcMeisterDiebe's; they use ai_Entries / ai_Slots that ai_Unk4801f0 reads; 0x48a4d0 tail-calls 0x48bc80).
  Suggested: join into ai.c in address order (0x4854f0 < 0x48a4d0 < 0x48bc80); names ai_MeisterTrade (0x48a4d0)
  and ai_MeisterTradeAllgemein (0x48bc80) are from the original messages.
- Headers: new include/unk_4854f0.h (full AIENT / AISLT / AIGD layouts reached through casts of the ai.h
  globals, AIJOBW, AIRECIPE, AIITEM, prototypes) and include/unk_48a4d0.h (AIOBJX, AITMSG, min/max, ai_Money,
  data 0x6f1bc5 / 0x6f1bc8 / 0x6f046c / 0xa68d2c / 0xf86a24 / 0xf8a0c8, prototypes). No shared header changed.
  ai.h needs at the join: ai_Unk48a4d0 returns int (unk_48a4d0.c hides the void prototype with a #define), and
  the AIENTRY / AISLOT / AIGOOD / AICHAR pads should become the fields of unk_4854f0.h (AICHAR+0x1b8 = money).
- VC6 lessons: (1) a 4-int -1 fill done by the original as `or reg,-1; mov ebx,edx; mov [ebx..+0xc]` is an
  inlined memset(p, 0xff, 16); (2) slot searches with `jge` on the strength-reduced pointer are int-index loops,
  a pointer loop gives `jae`; (3) `j = 0` stored before the `jne` of a found flag = the next search loop is a
  plain `for (j = 0; !found && ...)` without an `if (!found)` around it; (4) the field VC6 strength-reduces to
  (the base of `[esi+off]`) follows the source references: using the struct field instead of a local copy
  (`AI_SLT[j].item` in every compare, `e->unk44` as the sprintf argument) moved the base to the original's.
- Outside the scope: nothing new.

### Per-function notes
- ai_Unk4854f0 (0x4854f0, unk_4854f0.c): nonmatching, ~2385/3312 bytes; job loop and everything after it match
  but for stack offsets (frame 0x148 vs 0x138). Off: section 2 (kinds 14/8/32 extra entries: branch layout of the
  two item lists, 11 strength-reduced entry pointers), first loop (`ai_NEntries++` scheduled after slot[1], -1
  not kept in a register), slot pointer = slot base in sections 2/3.
- ai_Unk48a4d0 (0x48a4d0, unk_48a4d0.c) = ai_MeisterTrade (tag in its messages): nonmatching, 4889/6032 bytes
  (orig 6064), mnemonic-sequence ratio 0.83: frame 0x754 vs 0x74c; first slot loop uses &slot.amount as base in
  the original; the two debug sprintf calls of the "Verkauf" branches are tail-merged in the original (mine has two
  calls); float temp f kept on the FP stack (fcom + spill as double) where the original stores it as a float (fst)
  and reloads it for the `f <= rand * 1.25f + 0.25` test; block order of the two ai_Unk4c3ba0 loops and of the
  stock-selling loops differs. ai.h declares it void, it returns int (1 / ai_Unk48bc80's result): the .c hides
  the ai.h prototype with a #define.
- ai_Unk48bc80 (0x48bc80, unk_48bc80.c) = ai_MeisterTradeAllgemein (tag): nonmatching, 3911/4992 bytes (orig
  5008), 266 asm diff lines: almost all stack-slot offsets (+4) plus the entry-loop base pointers (original
  strength-reduces to &e->unk3c, mine to &e->item / &e->unk44) and the slot search of the stock loop (pointer +
  counter in memory in the original).
- Lesson: the slot searches `for (m = 0; !s && m < 128; m++)` are index loops: the original compares the
  strength-reduced pointer with `jge` (signed, from the int index); a pointer loop gives `jae`.

## f5 (round 21): 0x4864b0..0x487fb0, AI code (entry / slot tables) [f5]

### Report
- Nonmatching (4), with the cause:
  - ai_Unk487180 0x487180 (350/656 B, 34 asm lines): structure and all expressions match; left: `found = 0; j = 0;` is
    stored from `xor eax,eax` in the original (imm stores in ours, 3 variants tried: chained assignment, separate,
    for / while(1) forms), N is loaded into edx not eax, the strength-reduced updates are `off += 2; j++` (ours
    `j++; off += 2`), and the loop-end pair is `je top; jmp exit` (ours `jne exit; jmp top`).
  - ai_Unk487410 0x487410 (730/928 B, size exact): every block matches, the block layout does not. Original:
    `[chain of ==, last test jne E][L][k tail][ret][E: entry!=-1 recursion][E: loop]`; ours puts E inline and L after
    it. if/else-if/else, `||` vs `&&` chain, continue forms, do-while for the k loop all give identical bytes
    (VC6 normalises them), so the layout comes from something else (unsolved).
  - ai_Unk487780 0x487780 (699/912 B): the original keeps the char-loop pointer (`&ai_Chars[i].bld`) in memory
    ([esp+0x10], reloaded at each use) and `found` in edx with a redundant `test edx,edx` in front of each slot
    search; ours keeps the pointer in esi and drops the test. u14 clamp: original stores `u14 - (u24 + u28)`
    and then `& ((x < 0) - 1)` as two stores; VC6 drops the first store in ours.
  - ai_Unk4864b0 0x4864b0 (2133/2960 B, size exact): far-off, whole function is register allocation (original keeps
    `bld` in esi, frame 0x120 vs our 0x124), call argument load order and scheduling of the float spills; the
    logic of all loops / sorts matches.
- TU: all seven belong with unk_4854f0.c (same tables ai_Entries / ai_Slots; ai_Unk487040 is called from
  ai_Unk4854f0) and ai.c (ai_Unk487780/487ea0 are declared in ai.h). Suggested: join unk_4854f0.c + unk_4864b0.c into
  ai.c-family file in address order (0x4854f0 < 0x4864b0).
- Shared headers: none changed. include/unk_4864b0.h (new) includes unk_48a4d0.h and adds AIEN / AIS (full rows of
  ai_Entries / ai_Slots), AIGOODX, AIBTYPEX, AISTORE (ai_Unk7e4b20 table), externs ai_Unk7e3f3c, ai_Unk7e4b20,
  ai_Unkf8a0a0[16], ai_Unkf8a0e0[16] (unk_47d960.h has them as int[5] / int[6]: pick one at the join),
  prototypes ai_Unk501190 (3 arguments; unk_47ec20.h has 2), ai_Unk501030, ai_Unk4ffc40, ai_Unk50ab20, ai_Unk500c20,
  ai_Unk4f15e0(AIBLD *) (takes the building, tail call in ai_Unk487ea0), ai_Unk50cd40 (float, 4 args), ai_Unk5086b0.
  ai.h needs at the join: ai_Unk487410 returns int (the .c hides the void prototype with a #define), ai_Unk487180
  takes the full AIEN row (hidden the same way), ai_Unk487ea0 / ai_Unk4864b0(AICHAR *, AIOBJ *store, int f) missing.
- VC6 lessons:
  1. A byte parameter pushed with `xor ecx,ecx; mov cl,[m]; push ecx` is a callee taking `int` (ai.h says
     `unsigned char`: a plain call gives `mov al,[m]; push eax`); `((int (*)(int))ai_Unk501d80)(b->type)` (macro
     AI_KIND) reproduces it.
  2. `(float)a / (unsigned)b`-style `fidiv` from a stack copy with a dead zero store next to it = the divisor is
     `unsigned int` (ai_Unk487040: goods field 0x22).
  3. `x <= -1e10` on a float field compiles `test ah,0x41; jp`; the original `and eax,0x4100; je` is
     `!(x > -1e10)` (double literal).
  4. min/max macros (`a < b ? a : b` / `a > b ? a : b`) fix the operand order of `cmp; jl/jg` clamps: original
     clamps were `max(v, lo)`, `min(hi, v)`, `min(cnt, v)`; `x >> 1` (not `/ 2`) gives a bare `sar`.
  5. `flags |= 0x40; store; later test flags & 2` reuses the register (`or ecx,0x40` / `test cl,2`), while
     `store flags | 0x40` gives `mov eax,ecx; or al,0x40`: keep one local `fl |= 0x40; ch->f = fl;`.
  6. A pointer local for an argument expression (`oc = &ai_Chars[owner]; f(oc, t, ...)`) changed the register
     choice of an unrelated local (ai_Unk487b40 matched only with it).
  7. Character loops over a 768-row table with `for (i = 0; i < 768; i++) ai_Chars[i].f` are strength-reduced to
     a signed `jl` loop on `&row.bld`, the base is the field referenced most often (unk165 when it is read twice).
  8. `util_Random(n) >= k` / `k <= util_Random(n)`: compare operand order follows the source order.
- Outside the scope: nothing new.

## f6 report (scope 0x487fb0..0x489a70, src/game/unk_487fb0.c + include/unk_487fb0.h) [f6]

TU name suggestion: ai_slots.c (or ai_master2.c). Module tag "ai_"; AI master purchases (renovation/upgrade,
slot and capacity buys), the order dispatcher ai_Unk4889f0 (AIINFO), thief/spy helper tests (0x489300..0x489870).
Scope end: 0x489a70 (ai_Unk489a70 belongs to the next TU, untouched).

Nonmatching (4):
- 489660: 13 bytes; after the call, VC6 interleaves `n--` with the flags load (ours: ecx/eax interleaved; original
  sequential load/dec/store, then flags). 9 variants tried, none helps.
- 489870: 57 bytes; register roles swapped (original ch=ebx, n=ebp; ours opposite) plus sk82/sk81 load order.
  The two-counter variant puts ch in ebx but splits n over edx/ebp.
- 487fb0: i spilled to the stack; original keeps i (ebp), bld (edi), rv (ebx) in registers. Block-scoping i
  around the two loops gets the frame right (0x1fc) and halves the asm diff; the top `xor ebp,ebp` still differs.
- 4889f0: far off (loops now in the original shape; zero-register / allocation differences, message order not
  brute-forced).

Header additions: none in shared headers. unk_487fb0.h hides ai.h's declarations of ai_Unk489300 (void; it
returns int) and ai_Unk524260 (ushort id; callers here widen to int) with `#define name name_decl` around the
include, then redeclares them. Another worker editing ai.h should make 489300 int.

VC6 lessons:
- `for (i..) arr[i].f` on a global array: write `AIC(i).field` directly, no pointer local (base member/induction
  matched at once). `while (1) { if (i >= N) break; ...; i++; if (found) break; }` is the unrotated shape.
- Brute-forcing statement order of a message-field block (`uv run try` with generated permutations, 720-5040 variants,
  1-2 minutes) found matches for 488740 and 4884b0; order of independent statements decides scheduling AND
  register roles (4884b0: `y = mb - d` before `x = ma - a` made d spill like the original).
- `switch (r) { case 0: body; break; case 1: body; break; }` with duplicated bodies gives the `sub eax,0; je;
  dec eax; jne` chain (VC6 merges the bodies); `case 0: case 1:` gives a range test.
- `volatile` on a byte-wide struct read made `lea eax,[..]; mov al,[eax]` (489660).
- Passing a ushort field to a callee prototyped with int adds `and eax,0xffff` and changed register roles (4888c0).
- `x*c*c` on floats folds to one multiply; split into two statements. `a > f` with the constant/product first
  gives the `fld c; fcomp mem; and eax,0x4100` compare.
- Named intermediates (bld, rv, lim) compete for callee-saved registers and can spill a loop counter; reducing
  the live range of the counter (block scope) fixes it.
- Two AIMSG locals in one function stay separate (frame +0xf8) when both are used.

## f7 (round 21): 0x489a70..0x48e070, src/game/unk_489a70.c + include/unk_489a70.h [f7]

### Report

The nonmatching drafts are faithful translations (logic checked against the asm).

| addr | name | status | note |
|------|------|--------|------|
| 0x489a70 | ai_Unk489a70 (masters hand a weapon out of a slot to a character of the building) | **matched** (576 B) | index do-while, see lessons |
| 0x489cb0 | ai_Unk489cb0 (= ai_MeisterCollectTrans, strings) | nonmatching, 1167/1696 B | zero register: VC keeps `xor ebp,ebp` live over the whole table loop and `n` lands in memory; orig has n in edi, no zero register (about 485 asm lines) |
| 0x48a380 | ai_Unk48a380 (unload order of the goods carts) | nonmatching, 36/336 B | near miss: orig computes `100 - car->unk12` before the stores of dst[0] / unk84 but stores msg.n last; tried all 24 orders of car / dst[0] / unk84 / n and a temp: none gives it |
| 0x48d010 | ai_Unk48d010 (= ai_MeisterTradeFernEinkauf, strings) | nonmatching, 1460/2064 B | frame 8 B too big (two extra stack scalars: `amt` / `d` get own homes, orig keeps them in registers and only uses the shared fild temp), slot loop picks the wrong strength-reduction bias |
| 0x48d810 | ai_Unk48d810 (variant of the above that sells entry 0 first) | nonmatching, 69 asm lines (about 950/2144 B) | frame and the first 235 instructions equal; left: register choice in the entry-0 block (orig keeps `b` in edi and `amt` in esi, mine `market` in esi and `amt` in edi), zero register of the exp[] stores (orig edx, mine edi), `amt` stored to its home after the call, `cmp BYTE PTR [ecx],0x47` (mine loads al first) |

No TU boundary found: the scope is one run of AI code (all functions use ai_ tags, ai_Chars / ai_Slots / ai_Entries);
it belongs to the AI unit (ai.c join). The scope end 0x48e070 is the nop padding after 0x48d810 (ends 0x48e063);
the next function is outside.

Suggested names (log strings): 0x489cb0 ai_MeisterCollectTrans, 0x48d010 and 0x48d810 ai_MeisterTradeFernEinkauf
(the two variants; 0x48d010 has an int3 guard `nB > 3`, 0x48d810 sells entry 0 first), 0x48a380 something like
ai_CartUnloadOrder (no string), 0x489a70 ai_MeisterBewaffnen ("Meister %s bewaffnet %s mit %s").

Shared headers: none touched (ai.h / unk_48a4d0.h read-only, unk_4854f0.h untouched). include/unk_489a70.h includes
unk_48a4d0.h and adds: AIOBJT (0x43-byte rows behind 0x7e3f34), AISLX / AIENX (slot / entry rows with the sorted
buy list fields: price 0x1c, key 0x20, need 0x34 / bld 0x14, key 0x34, need 0x4c), macros for padding fields
(AI_CH_178, AI_TIME6, AIBTYPE_247, AIMSG_A0, AI_SLX, AI_ENX), globals ai_Unk7e3f34, ai_Unkf8a0a0 / e0,
ai_Unkf8a0cc, ai_Unkf89540 and prototypes ai_Unk47e8b0, 4cd9d0, 4c4540, 4c4580, 4c47e0, 508660, 5bb2d0.
For the ai.c join (orchestrator): these clash with unk_4817a0.h (same names: AIITEM, ai_Unka68d2c int vs char *,
ai_Unk7e3f34 AIITEM * vs AIOBJT *, ai_Unkf89540 AIOBJ * vs AIOBJX *, ai_Unkf8a0cc, ai_Unk4c47e0 ... equal) and
with ai.h (ai_Unk489a70 / ai_Unk489cb0 declared void with other parameter types; my file renames them with
`#define` around the include, like unk_48a4d0.c). ai_Unk48d010 / ai_Unk48d810 return int (declared void in
unk_4817a0.h). Fields found: AICHAR 0x178 is an object list (arg 1 of ai_Unk508660 / ai_Unk4ffe90), 0x1b8 money,
AIBLD 0x65 is an index 1..3 (type 'G' = bank), the 0x7e3f34 rows are 0x43 bytes (id +2, next +6, building id +0x1c).

### Data worth knowing

- 0xf8a0e0 / 0xf8a0a0 hold the goods ids of two weapon lists (see unk_47d960.c); ai_Unk489a70 even overwrites
  t[k] with the id of the object it found (the original does that, kept).
- 48d010 / 48d810 keep nA / nB (sold / bought counts) across carts (only reset in 48d010: nB); original quirk, kept.
- ai_Unk48d010 compares `msg.dst[3]` (still -1 for a fresh message) with the building ids in the entries loop;
  48d810 does the same in the entries loop and compares dst[0] in the slots loop. Kept as in the asm.

### General VC6 lessons (for notes/vc6-matching.md)

- A `while (!found)` loop whose first test is an in-body `if (j >= n) break;` (rotated guard `test ecx,ecx; jne` before
  it, `test ecx,ecx; je top` at the bottom) is written exactly so: `j = 0; while (!found) { if (j >= n) break; ...; j++; }`.
  The `if (t[-1] != 0) { j = 0; while (!found) ...` form keeps `xor edx,edx` before the `test`.
- A 768-element loop that compares the biased induction register with a constant (`cmp esi,0x7d6104`) is
  `i = 0; do { c = &ai_Chars[i]; ... i++; } while (i < 768);` (pointer / `(int)c < const` forms compare an unbiased
  copy: lea + cmp).
- Two views of one global array (`(AISLT *)ai_Slots` and `(AISLX *)ai_Slots`) are two induction variables: use one view
  for every access in a loop, or the strength-reduction bias changes.
- A pointer induction variable (`sl = AI_SLX; ...; sl++`) with constant start gives the bias on the *max-offset
  field* (here `need` at 0x34: `esi = 0xf86a74`, key at [esi-0x14]) and `cmp DWORD PTR [esi],0` for a field that is
  reloaded after a call; the array-index form picked another field and loaded the value into eax. For a variable
  start (`sl = &AI_SLX[i]`) the bias stayed 0.
- `float <= 0` as the loop-exit test: `if (!(x <= 0)) break;` gives `test ah,0x41; jp`, `if (x <= 0) return 0;` gives
  `jnp` (the original uses both); `x > 0` gives different code (size grew by 16 B).
- `char buf[1028]` costs 8 bytes of frame more than `char buf[1024]` (alignment): an exact Ghidra size
  (`local_408 [1028]`) overstates the buffer by the saved-ebp slot, take 1024.
- `msg.unk36 = 1;` stored before `msg.time = ai_Time;` gives the original's order (unk36 byte store between the dword
  copies) in the cart-message blocks; after the copy gives byte-store-last.
- Zero register trap: many `= 0` stores (n = 0, n136 = 0, own = NULL, `push 0` and `job != NULL` tests) make VC keep
  `xor ebp,ebp` live for the whole loop and push the counter into memory; the original gets its zero from the
  counter's own register (`xor edi,edi` = n). Not solved (0x489cb0).

## match-base 2026-10-24 (base-1 worker, shard 1/5)

- ai_Unk47dd70 -> ai_WorksForMaster (the character kinds 2..5 that work for a master; callers in
  am.c / f3gm.c / f3init.c / unk_58bbd0.c), byte + readable. ai_Unk770b80 -> ai_Chars project-wide
  and ai_Unk7e3f30 -> ai_BTypes / ai_Unk91aaf0 -> ai_Goods (ai.h's own names) also cleared Q2 in
  ai.c and am.c.

## match-base 2026-10-25 (base-0 worker, shard 0/6)

- 0x4f2720 calls renamed ai_Unk4f2720 -> gv_GetTableRecord; byte unchanged.
## match-base 2026-10-25 (base-1 worker, shard 1/6)

- Six ai.c functions raised to byte + readable (all were already byte; only the readability was missing):
  ai_Init (0x47d9e0, the AI module's init), ai_UpdateNeeds (0x47da80) / ai_GrowNeeds (0x47dab0, the 13
  mood needs), ai_WorkLimit (0x47dd00) / ai_WorkLoad (0x47dd30, the per-frame AI work budget) and
  ai_IsLocalMaster (0x47dd90). Check count unchanged (2523/4664 matched); project base-level 1091 -> 1097.
- Project-wide renames: ai_Unk47d9e0 -> ai_Init, ai_Unk494e70 -> ai_LoadActionTable, ai_Unk4fe1e0 ->
  gm_SetTime (the f3gm.c name; ai.h keeps the AITIME view), ai_Unkf89580 -> ai_UpdateStart, ai_Unkf89584
  -> ai_LastUpdateTime, ai_Unkf89594 -> ai_CountCarry, ai_Unkf8a0e0 -> ai_WeaponList, ai_Unkf8a0a0 ->
  ai_WeaponList2, ai_Unk47da80 -> ai_UpdateNeeds, ai_Unk47dab0 -> ai_GrowNeeds, ai_Unk47dd00 ->
  ai_WorkLimit, ai_Unk6f1ba0 -> ai_Rating, ai_Unk47dd30 -> ai_WorkLoad, ai_Unk53e300 / he_Unk53e300 ->
  he_NextEvent, ai_Unk62489c -> ai_NumPlayers, ai_Unk6f046c -> ai_TradeMsgCount, ai_Unk47dd90 ->
  ai_IsLocalMaster, ai_Unk1460af4 -> ai_NetPlayer, ai_Unk6f2590 -> ai_CurrentPlayer, ai_Unk6f23fc ->
  ai_Difficulty.
- AICHAR field evidence: 0x194 -> money (amt.c: the price is taken off the character's money), 0x170 ->
  bld2 (the am.h views already called it bld2; f3gm.c 0x4fdcb0: its home / building), 0x1e4 ->
  mood3Rate (its only reader scales mood[3][0]; message 0x1a syncs it - a role name, the game meaning
  is unknown), AIJOB 0xf1 -> flags (he.c 0x56a7b0 / 0x56a960: the trade event's flag byte, nonzero
  defers the message posting, the bits are unknown).
- Q4 on `bi = *(int *)&best;` (0x47dab0): `union { float f; int i; } best;` with best.f / best.i is
  byte-identical (VC6 writes the 0 to best's slot and reads it back); a plain `bi = 0;` is folded and
  loses the spill.
- Q3 on the 0x47da80 bound: `(int)c < (int)(ai_Chars + 768)` keeps the original's signed `jl`
  (`c < ai_Chars + 768` gives the unsigned `jb`).
- Open: 0x47ddf0 ai_Unk47ddf0 is still draft (582 code hunks; the round-20 note: logic complete,
  allocation only). Readability leads collected: ai_Unk4c3c00 -> cm_AddFieldDelta (cm.c 0x4c3c00
  "adds n values of size bytes at field as differences"), ai_Unk505df0 -> gm_FindChar (f3gm.h already
  names 0x505df0; ai.h keeps the AICHAR* view; ~600 references), ai_Unk505e60 -> gm_CharLevel (f3gm.c
  0x505e60 "the level of the character id"), ai_Unk50c550 -> gm_Liking (f3gm.c 0x50c550 "how much the
  person a likes the person b"), ai_Unk5a8ee0 / 5a8f90 / 5a9050 = the three pair-message variants
  (texts 0x132f / 0x1339 / 0x1343; a role name beyond "pair message" is not evidenced). AICHAR job
  bytes: 0x166 -> rank (amt.c "his rank"), 0x167 -> nextRank ("the next one"), 0x169 -> rank2 ("the
  second rank"; the four bytes 0x166 .. 0x169 index the name table 0x143ab0c per cm.c). 0x214 .. 0x218
  (the partner state sent as message fields 0x214 / 0x215 / 0x216 / 0x218): no evidenced names, only
  the merge counters of 0x47ddf0 itself describe them. Q3: 0x7d5f82 -> the ai_Chars loop bound.

## match-base session 2026-10-25 (base-1 worker, shard 1/6)

- ai_Unk47ddf0 (0x47ddf0) escalated to match-deep: 420 / 455 instructions differ although the logic is
  complete (every hunk read against uv run disasm 0x47ddf0 0x570: no missing / extra statement, constant or
  callee). The original keeps bestf at [esp+0x14], best at [esp+0x18], f at [esp+0x1c] (frame 0x14) and
  reuses the dead parameter slot [esp+0x28] for the ai_RelationMemory row offset i*0x300; ours keeps best in
  edi, f at [esp+0x20] (frame 0x1c), and the original's return-0 tails are out of line (47de32, 47df5b,
  47e2b0, 47e307) where ours are inline. `uv run spell`: 3 x dup-tail, 1 x reload (the netId load did not move);
  the Demorgan polarity spelling `if (!c->unk166 && !c->unk169 && c->kind != 5)` is byte-identical (reverted).
- Readability renames (evidence): ai_Unk505df0 -> gm_FindChar (0x505df0, 602 refs; the name already existed in
  f3gm.h / f3gm.c, the ai.h / ahm.h / am.h / amt.h / cm.h / gv.h declarations stay as duplicates),
  ai_Unk4c3c00 -> cm_AddFieldDelta (0x4c3c00, 174 refs). Not done: ai_Unk505e60 -> gm_CharLevel and
  ai_Unk50c550 -> gm_Liking - their declarations disagree across headers (ai.h `unsigned short` vs gv.h
  `unsigned`; amt.h / cm.h / gv.h K&R), so the declaration cleanup has to come first (ai.h's ai_Unk505e60
  line is tagged @original with the am.c 16-bit push evidence).
- Q2 / Q3 leftovers of 0x47ddf0: AICHAR 0x214 .. 0x218 have no evidenced role anywhere (0x218 has no reader);
  the three pair-message callees 0x5a8ee0 / 0x5a8f90 / 0x5a9050 differ only by their text ids (0x132f / 0x1339
  / 0x1343); 0x7d5f82 = ai_Chars + 768 * 0x21c + 2 (the loop bound literal of 0x47ddf0 and 0x47e360).
- ai_Unk47e360 (0x47e360, byte) escalated to match-deep for its readability: Q1 no evidenced name (a local
  master's per-frame building scan that announces a master hire - its sprintf "stellt Meister fuer %s ein" -
  then runs the partner logic); Q2 AICHAR.unk008 (32 truthy / > 0 tests, no reader names it) and AICHAR.unk1b1
  (2 uses: the post-partner action refresh here, the message sender 0x4bb6c0's skip); Q3 the 0x7d5f82
  ai_Chars + 768 * 0x21c + 2 loop bound (an extern needs an evidenced name for the one-past-the-end kind
  field). ai_Unk501d80 (0x501d80, no source, no Ghidra export) read by disassembly: ai_BTypes[type].kind 1..0x20
  -> the category 1..9 (jump table 0x501e14, default 0), a building-type category in an undrafted unit.
- Renames of this session, evidence in the function comments: ai_Unk47ddf0 -> ai_FindPartner, ai_Unk4957d0 ->
  ai_ChooseAction ("chooses the next action"), ai_Unk495ce0 -> ai_ExecuteAction ("executes the current action"),
  ai_Unk4a3af0 -> ai_FillNearRing ("fills the ring 0xf83c20 with the characters near"), ai_Unk4c55d0 ->
  cm_SendMessage3d ("message 0x3d"), ai_Unk5eed70 -> ai_BuildingTypeMasks (ai.h "bit masks of building types"),
  ai_Unk6f0468 -> ai_Partner (am.c: the partner of the current update, set around the partner logic),
  ai_Unkf83f50 -> ai_ActionFlags (the flags at +0x90 of each 0x94-byte G5ACT record, indexed by the action id).
  AICHAR.unk132 -> act and unk218 -> mask (the am.h G5CH view already names 0x132 "the current action" and
  0x218 "the action groups it may use"; same type, no union needed), AITIME.unk4 -> hour (ai.h's own comment).
- ai_UpdateCharacters (0x47e560, byte; was ai_Unk47e560) -> match-deep for Q2 AICHAR.unk024 only: the value the
  per-frame loop decays ("whole = c->unk024; whole = -whole - (int)(whole * -0.89f)") and syncs as message field
  0x24, and am.c 0x494020's "sum = a->unk024 + a->unk01c" against an n * 70..190 limit - an accumulating score
  with no string, text id or caller name. Named around it: ai_Unk47e360 -> ai_MasterUpdate (a local master's
  per-frame building scan and hire, then the partner logic), ai_Unk47e7f0 -> ai_CanAct, ai_Unk48a380 ->
  ai_UnloadCarts, ai_Unk4fe280 -> gm_MinutesBetween (the f3gm.c name), ai_Unk5fef04 / 5fef08 ->
  ai_DifficultyValue / ai_DifficultyScale.
- ai_Unk47e990 (0x47e990, byte, the master evaluation dispatcher) -> match-deep for its readability: the seven
  master-evaluation callees are undefined names - 0x47edb0 (default "evaluates the production of a master's
  workshop"), 0x47f700 (kinds 0xe / 8 / 0x20), 0x4801f0 (category 2), 0x483fc0 (kind 0x1d), 0x484a60 (kind
  0x1f), 0x483a60 (kind 9, no role comment), 0x4836a0 (the bank master's money balance); the five production
  variants differ only by the AIENTRY flags their bodies test, and no source holds the kind-to-profession table
  that would name them; 0x501d80 (no source). Globals named on the way (ai.h documents all three):
  0x6f2308 -> ai_PlayerBuilding, 0x6f230c -> ai_FallbackBuilding (the f3snd.c 2308-then-230c fallback),
  0xf8a0c8 -> ai_LoggedMaster ("the master whose decisions are logged").

## match-base session 2026-10-25 (base-1 worker)

- ai_SendRemoveEmptyGoods (0x47ed60, byte + readable; was ai_Unk47ed60): sends message 0x14 (cm_SendRemoveGoods)
  for every workshop entry with no stock (AIENTRY.amount 0) that is not in production (flags bit 8), so the
  clients drop the empty good from the stall's store (handler 0x4c7d80). Named on the way: ai_Unk4c4960 ->
  cm_SendRemoveGoods (message 0x14), AIOBJ.unk02 -> objId (0x500190 gives a new one; messages and 0x4fea50 find
  the instance by it), AIENTRY.unk3c -> amount (gm_ListAmount / gm_Unk50ab20).
- ai_EvaluateProduction (0x47edb0, readable) -> match-deep: far draft 1608/2272 bytes. The human-player block's
  if/else-if order (the f(1) branch first, then f(0)) is the original's (Ghidra + disassembly) and bought 10
  bytes (1618 -> 1608); the rest is register allocation (the original keeps ch in ebx and reloads it, ours in
  ebp), match-deep's slotmap / listing / c2prio. Naming: ai_Unk4854f0 -> ai_UpdateProduction (builds the entry /
  slot tables and their pending / sort state), ai_Unk487b40 -> ai_UpdateStaff (the hour 7 / 12 / 18 job messages
  and hiring), ai_Unk4884b0 -> ai_BuySlots, ai_Unk488740 -> ai_BuyCapacity, ai_Unk487fb0 -> ai_BuyUpgrade,
  ai_Unk487180 -> ai_HasMaterials, ai_Unk487410 -> ai_StartProduction, ai_Unk4888c0 -> ai_FreeJobCharacters,
  ai_Unk4889f0 -> ai_SendJobOrders, ai_Unk5a8870 -> ai_DrawSettingGood (0x5a8870 draws a good by the weights of
  a building's setting), sv_Unk524260 -> sv_MoveToGroup (the sv.c definition and the ai_ alias, 106 call sites),
  ai_Unk50ab80 / gm_Unk50ab80 -> gm_FreeCapacity (the f3gm.c name; the ai.h / cm.h / rt.h aliases follow).
  Fields: AIENTRY.unk40 -> capacity (what ai_Unk50ab80 returns), AIGOOD.unk36 -> batch (he.c 0x5690c0 hands it
  out; the store needs room for it and the market that much plus the stock), AIBLD.unk5d -> objList (with
  am.h's AIBLDX view), AICHAR.unk008 -> health (100 at creation in ahm.c, > 0 = alive), unk165 -> title (the
  f3gm.c title rank, 0x4fda70 / 0x504840), unk184 -> model (f3gm.c 0x4fbd10 stores the building at +0x2c),
  AITASK.unk2c -> bldId.
- ai_EvaluateProductionFlags (0x47f700, readable) -> match-deep: far draft 1867/2608. Read region by region
  against Ghidra 47f700; the done label passes b (0x47fddc: [esp+0x14]) to both 0x47ed60 and 0x47ec20 (Ghidra's
  iVar3 there is scratch reuse). The same if/else-if rewrite that helped 47edb0 is WORSE here (1867 -> 1877) and
  was reverted - do not copy a branch form across the production variants without measuring.
- ai_Unk4801f0 (0x4801f0, draft + draft) is still in the base queue: Q1 + Q2. Open Q2: ai_Unk511e60 =
  unk_Unk511e60 in f3init.c, whose functions all carry the unk_ placeholder prefix (the unit has no name yet -
  a unit-name decision, not a function rename), and AIMSG.unk14 (only ever written 0, or -1 in hst.c; no
  reader gives it a role - AIMSG.unk08 is "to" (G8MSG names +8 to, every write is the owner's netId), unk0c
  the source building id, unk10 the count of ids[] (msg.ids[msg.unk10]++), unk36 the message flags (he.md) -
  those four could be renamed next). Read against Ghidra 4801f0: the three queries (0xff / 0x2a / 0xfe), the
  slot flags-4 loop, the ai_UpdateStaff / ai_BuySlots(10) / ai_BuyCapacity(0x19) / ai_BuyUpgrade order, the
  message-9 build (the Ghidra uVar7 test is the C object o, not the drawn id), the kind-6 block, the two
  entry loops, the check block, the kind-11 plant code and the job tail match; the far difference is
  register allocation. Named on the way: ai_Unk4ffcc0 -> gm_QueryNext, ai_Unk4bc5a0 -> veg_FindType,
  ai_Unk4bc710 -> veg_Fits, ai_Unk4c5050 -> cm_SetPlant (message 0x2c: takes / frees a plant place of a
  building, handler 0x4c9520); AIBLD.unk71 -> plants (the 64 G15PLANT records of the building's vegetation
  map), AIPLANT.unk00 / unk42 -> id / size (a partial G15TREE, rd.h); the AIOBJ union views count (0x0e) /
  list (0x14) are used in ai.c instead of their unk aliases.
- ai.c declarations can carry ai_ alias names for functions defined in other units under their canonical name
  (gm_QueryNext, veg_FindType, veg_Fits, cm_SetPlant): there is no link step, so renaming the alias to the
  canonical name is safe and is the readability fix (no second name is invented).
- Raised on the way (byte + readable, were byte + draft): ai_FreeJobCharacters (0x4888c0, Q1 only) and
  ai_BuyCapacity (0x488740, its last Q2 placeholder was AIMSG.unk08). The AIMSG fields are named now:
  unk08 -> to (the receiver's player netId; am.h's G8MSG already names +8 "to"), unk0c -> fromId (the
  sending building id), unk10 -> nIds (the count of ids[], msg.ids[msg.nIds]++), unk36 -> flags (1 / 2 / 9,
  he.md's message record); am.h's AIXMSG view follows. AIMSG.unk14 stays: only ever written 0 (hst.c: -1),
  no reader names it.

## match-base session 2026-10-25 (base-1 worker)

ai.c: base queue 34/55 -> 26/55 below behaviour + readable; check 2560/4664 before and after.
Worked in queue order: ai_EvaluateProductionCategory2 (was ai_Unk4801f0, named by role - 0x47e990
dispatches here on ai_Unk501d80(type) == 2) -> match-deep; ai_CalcMeisterWache -> match-deep;
ai_CalcAngriff -> match-deep; ai_CalcMeisterDiebe -> match-deep; ai_CalcMeisterRaeuber -> match-deep;
ai_CalcMeisterBank (was ai_Unk4836a0, byte) Q-only handover; ai_Unk483a60 Q-only handover with a new Q6
description. Every @deep reason carries its per-function read and the Q2 evidence gaps.

- The umland message's string copy is VC6's inlined strcpy. Replacing ai_CalcMeisterRaeuber's indexed
  copy loop (`j = src[i]; ((char *)msg.arg)[i] = j; i++;`) by `strcpy((char *)msg.arg, src)` dropped it
  from 321 code / 26 slots hunks to 207 / 12 (asm diff 211 -> 139 lines). The pointer-loop spelling in
  ai_CalcMeisterWache already compiled as the inline copy (byte-neutral there).
- The stale ai_ aliases of functions this unit calls are the Q2 bulk; renaming them to the canonical name
  of the defining unit is safe (no link step) and was done project-wide: gm_BuildQuery (0x501190),
  gm_BuildNext (0x501030), gm_FindBuildingById (0x502020), gm_Opinion (0x50c520),
  gm_GetGebaeudeHauptraum (0x501ce0, from the callers' error text; f3gm.c's own gm_Unk501ce0 included),
  gm_FindById (0x4fea50), gm_IsHomeType (0x502520), gm_BuildingRank (0x5090b0), gm_OwnerWorth (0x509c80),
  gm_SumSlotAmounts (0x508660); ai_Unk511e60 -> init_BuildingSceneUniverse (the f3init.c unit style,
  its definition was unk_Unk511e60).
- Q2 evidence gaps that stay (no name in any source after reading callers, callees, data):
  ai_Unk4f8300 / ord.c's unk_Unk4f8300 (unit unnamed, @named_by guess - no prefix can be chosen here),
  ai_Unk501d80 (no source, no Ghidra export, defining unit undrafted), ai_Unk553cc0 (he.c predicate, no
  role name), ai_Unk4864b0 (trade decision builder, no comment yet), ai_Unkf89540 / ai_Unkf8a0cc,
  ai_Unka68d2c, AIPOS.unk18, AIBLD.unk39 / unk65, AIMSG.unk14 / unk89, AIOBJ.unk06 / unk1c (documented as
  `bld` in round 22, but the field rename is blocked by the other unk1c fields) / unk20 / unk2a / unk36 /
  unk37, AIJOB.unkac (a union id: building id here, netId in am.c), AISLOT.unk34 / AIENTRY.unk4c,
  AIBTYPE.unk247.

- Late in the 2026-10-25 base-1 session: ai_Unk483d30 -> ai_StartBuildingWork (behaviour, Q-only
  escalation: Q2 AIBLD.unk65, Q4 msg.arg[2]); ai_Unk4ef6b0 -> gv_EntryIndex. The 0x4f22a0 rename was
  tried and reverted: the caller needs an int * result, gv.c's draft returns int (headers disagree,
  lint), so the AI-side name stays ai_Unk4f22a0 until gv.c's prototype is settled.

## match-base session 2026-10-25 (base-1, shard 1/6, second run)

Worked in address order; 0x483ee0 ai_EndWorkerPose is byte + readable now, the other seven are escalated to
match-deep (reason in the @deep tag). The leads that remain, so they are not repeated:
- 0x483ee0 ai_EndWorkerPose (was ai_Unk483ee0): the kind-0x1d (graveyard) master's work-hour step - at 7 / 12 /
  18 o'clock, once per hour, the first character of his building with the worker title 0x1c, at work (+0x1b4
  bit 0) and with its model in the building gets its role pose extra ended with 25% probability (ai_EndCharExtra,
  was ai_Unk587c40: evt_Stop of the model's +0x28 extra, then the at-work flag toggled by f's bit 0).
- 0x483fc0 / 0x484a60: the original spells the entry accesses through the table - ai_Entries[i].field with the
  field offset folded into the absolute address ([esi+0xf88a80] = capacity) - and materializes &ai_Entries[i]
  only for the callee arguments; our source keeps the pointer local for the fields. Measured on 0x484a60's first
  prod loop: asm 612 -> 559 lines, code hunks 765 -> 715, differing bytes 1839 -> 1861, unitdiff WORSE, reverted.
  The rest of both bodies needs the same rewrite plus the allocation.
- 0x4854f0 ai_UpdateProduction: the entry-init block is VC6's per-field hoisted pointer form (one stack slot per
  AIENTRY field, each bumped by 0x58 per iteration). The memset(slot, 0xff, 16) written as four slot[i] = -1
  stores measured 2361 -> 2334 bytes but 172 -> 244 asm lines (WORSE, reverted).
- 0x486190 ai_FillSlotTable: pure allocation (ai_NSlots in ecx, the zero in esi, the two goods-list pointers in
  ebp / edi). Q2 left: AISLOT.unk10 / unk18 / unk30-now-rank / unk38, and AIGOOD.unk22.
- 0x4864b0 ai_BuildTradeOffers: frame 0x124 vs 0x120 = one local too many (merge candidates: i / j / k / m /
  off / found; k and m only in the job loop, off only in the ai_Blds walk). Q4: the AISLOT swap through the
  AIENTRY scratch etmp and *(int *)((char *)bld + 0x45) (a percent factor) have no evidenced types.
  Q2 left: AIENTRY.unk38 / unk44 / unk4c / unk50, AISLOT.unk10 / unk18 / unk38, AIBLD.unk65.
- 0x487040 ai_CalcEntryCost: byte; readability fails only AIGOOD.unk22 (the divisor of the margin formula, its
  only reader in the sources).
- 0x487180 ai_HasMaterials: 43 code hunks, all after instruction 88 = the offer-row pointer and the loop
  counter's stack slots.
Shared-header work of this session (byte-neutral, project-wide): AIENTRY.cost / materialCost / objectValue /
price / margin / rank, AISLOT.price / rank / buyAmount, AISTORE.goodsObj, the AISLOT unk28 alias merged into
amount; callee / global names gm_FindObjectInst, gm_StallGoodsList, gm_ListAmountById, gm_FindGoods,
gm_ObjectValue, gm_CollectOffers, gm_Offers, gm_NOffers (the f3gm.c definitions are canonical, the ai_ / ai_Unk
aliases in ai.h are gone).

## match-base session 2026-10-25 (base-1 worker, shard 1/6, session 3)

Shard 1/6, unit ai.c. Check count 2567/4664 unchanged; ledger check --against 0 regressions; unitdiff ai.c 37601 ->
37602 differing bytes in 26 functions (ai_AddEquipmentNeeds gained 4 differing *instructions*, which the unit
count measures; the byte total moved by the fill alignment).
- ai_StartProduction (0x487410) readable, escalated for the block layout. Every block read against
  uv run disasm 0x487410 0x370 and build/ghidra/487410.c: the statements, constants, callees, field offsets
  and the item-list order all match. The original keeps the special-item block inline after the 15 je
  (0x4874cb..0x48751b) and puts the entry!=-1 block (0x48754a) and the offer-scan block (0x4875e7) OUT of line
  after the loop epilogue/ret; ours emits the entry block inline and the special block at $L4532 before the loop
  tail. Tried one edit one build each, ALL byte-identical (183 asm diff lines, 129 code / 68 slots hunks
  unchanged): (1) positive `== ||` chain with the special block as the then-branch and the entry/offer blocks as
  else-if, (2) nested else (`if (special) {} else { if (entry != -1) .. else if (..) }`), (3) special block +
  `continue` then a separate if / else if, (4) swapping the k / j declaration order. What is left: the block
  ordering and the stack slot choice (k 0x18 vs 0x10, j 0x10 vs 0x18, the offer-row pointer 0x14 vs 0x1c).
  Readability: AIENTRY.unk38 -> pending (the running jobs add their in / out amounts to it exactly as they add
  them to AISLOT.pending, 0x4854f0 / 0x4858xx; the room test is (pending + amount) * 4 < capacity), Q6
  description.
- ai_AddEquipmentNeeds (0x487780) readable, escalated. Meaning-class lead that MATCHED (kept): the renovation
  budget compare `(float)make * price + (float)sum <= (float)limit` is the original's negated form
  `if (!((float)make * price + (float)sum > (float)limit)) break;` -> the `and eax,0x4100 / jne` idiom instead of
  `test ah,0x41 / jnp` (147 -> 143 differing instructions, code 176 -> 168). Measured and NOT it: the original's
  two stores of make (0x487949 / 0x487951) - the read-back spelling
  `make = make & (((int)make < 0) - 1)` after `make = u` is folded by VC6 into one store (byte-identical), and
  the original's add-then-sub association (add ecx,ebp / sub eax,ecx) stays sub, sub in ours; `int u` instead of
  `unsigned int u`, byte-identical. What is left: the char loop's base pointer lives in the FRAME in the
  original ([esp+0x10] = &ai_Chars[i].bld, reloaded at each use; ours keeps it in esi and spills once) and the
  original re-tests found (test edx,edx) before each slot search; plus the store scheduling (order 52) and the
  char pointer's register allocation. Readability: Q4 raw access replaced by the typed AICHAR.inventory (see the
  rename list).
- Rename of this session (byte-neutral, verified against HEAD for both other units): AICHAR.unk178 ->
  inventory, the character's goods inventory (a G8 object list of item ids, next at +0x3f; gm_Query finds a
  prototype kind in it, gm_SumSlotAmounts sums a need slot over it, am.c 0x4a5760); am.h's AIPX mirror follows.
  Refs: ai.c 4, am.c 8, unk_4b6480.c 1.
- ai_BuyUpgrade (0x487fb0) readable, escalated to match-deep, 99 -> 48 differing instructions. The fix that
  matched: the two slot loops were `p = &bt->slot[0]; do { ...; i++; p++; } while (i < bt->nslots);` - VC6
  spilled BOTH induction variables, the frame came out 0x200 vs the original's 0x1fc (i at -0x1f4 AND ret at
  -0x200 in memory; the original has ret at -0x1fc, f at -0x1f8, first at -0x1f4 and the loop counter i in ebp,
  loop tail 0x488289 inc ebp / mov dl,[ebx+0x22]). Written as `for (i = 0; i < bt->nslots; i++)` with
  bt->slot[i] the frame matched and the function went 99 -> 48 differing asm lines (code hunks 151 -> 52, every
  slots hunk gone). Measured and NOT it, one build each: all-c->bld (81 code hunks, worse), all-bld (377,
  worse), int n (identical), dropping the redundant `AICHAR *c = (AICHAR *)ch` alias (identical - the cleaner
  ch form is kept). What is left is the register pair of the building pointer (original edi, ours ebp) and its
  knock-on hunks (0x487fc0 zero register, 0x488060 the lim registers, 0x4881c3 test vs cmp against the zero
  register, 0x488217 the re-zero, 0x48824c / 0x4882e3 the ai_Goods[n] address form), the store order 0x488101
  and a 3-instruction dup-tail 0x4884aa. The float block's two 0.005 factors are real (Ghidra 487fb0:
  f = (float)x * 0.005 * 0.005). Readability done: Q2 ai_Unk506430 -> gm_PlayerRecord, ai_Unk508d70 ->
  gm_BuildingLevel (returns the building's level byte +0x5c), ai_Unk509b10 -> gm_EquipmentValue, AIBLD.unk25 ->
  masterId (am.c 0x497cf0 looks the AI's buildings up by it, the upgrade message goes to that character),
  AIBLD.unk5c -> level (0..100, renovation threshold, 100 - level in the renovation message), AIFIN.unk40 /
  unk44 -> upgradeSpent / renovationSpent; Q4 *(unsigned short *)&msg2.arg[0] = ret -> the new AIMSG.itemId
  union view (0x58).

## match-base session 2026-10-25 (base-1 worker, shard 1/6, session 4)

- ai_SendJobOrders (0x4889f0) -> match-deep. Read branch by branch against uv run asmdiff and
  build/ghidra/4889f0.c; the statements, constants, callees and field offsets equal the original. One
  source-shape lead MATCHED and was kept: the 64-record vegetation-map scan is the original's
  `do { if (k >= 64) { if (!found) return; break; } ... } while (!found);` (the null return INSIDE the loop, not
  after it) - 562 -> 519 code hunks, 356 -> 335 asm lines, differing bytes 1753 -> 1803 (the tool's WORSE is
  asm lines, so no WORSE). What is left: the map pointer's register (original ecx / ebp, ours eax), the AIMSG
  store order, the [esi+0x1c] reloads.
- Q4 that stays raw: the message's 0x58 count byte and 0x5a item word (`*(unsigned char *)&msg.arg[0]`,
  `*((unsigned short *)&msg.arg[0] + 1)`) need an anonymous struct in the AIMSG.arg union, and ANY such member
  flips ai_Unk483a60 to 5 differing bytes (the [esp+0x10]/[esp+0x18] slot swap) - re-measured this session with
  an `order` view (count / item), reverted. Shared-header decision for match-deep.
- Q2 evidence gaps: AIMAPENT.unk10 (the vegetation record +0x10: written -1 by veg_SetPlant, only compared
  == -1 in the map scan, no other reader), AIMSG.unk14 (written 0 / -1 only, no reader), AIJOB.unkb8 (a
  per-kind payload word: the plant id in the harvest job, counters / ids / news ids in he.c).
- Renames (byte-neutral, full ledger check clean): ai_Unk4bc670 -> veg_FindPlantId (rd.c's definition; the AI
  header declaration follows rd.h; ai.c casts its AIMAPENT view), ai_Unk501b50 -> gm_BuildingMainObject
  (f3gm.c's definition, the prototype now in include/f3gm.h), AIMAPENT is now the AI's full view of the
  G15PLANT records (id / unk04 / x / z / type / size / state / unk10 / obj, same 0x18 layout; ai.c cannot
  include rd.h because rd.h's ai_MapRefs clashes with ai.h's), AIMAPENT.unk0d -> state, AIREC (the old 0x0a
  view) removed, msg.itemId = info->e->item replaces four raw word stores.
- ai_Unk489870 -> ai_SendFreeSpies (0x489870) readable, escalated: the espionage kind 0x8f band (he.c 0x571ac0),
  Q6 rewritten. Behaviour left: the prologue register pair (original ebp = the parameter, ebx = n; ours the
  reverse) and the [ch+0x16c] reloads; `uv run spell` push-pop 48987c, reload 4898b5 / 489924 / 4899a5. Measured and
  NOT it (byte-identical): dropping the `AICHAR *c` alias, swapping the i / n declaration order.
- ai_MeisterCollectTrans (0x489cb0) escalated (was ai_... named already). Q6 rewritten, Q4 cleared (q is
  AIOBJ*, q->unk06; the two job->unkac uses are job->u.id). Q2 gaps: AIOBJ.unk06 (a link id: the owner chain
  here, a building id in am.c 0x4254, a good / object id in ai.c 0x481ef0), AIOBJ.unk1c (a building id here,
  a count / level in its union elsewhere), AIMSG.unka0 (written -1 only). Behaviour: 593 code hunks, allocation
  / scheduling only (zero register ebp vs edi, the n / n136 slot rotation, cmp cx,bp vs test cx,cx).
- Project-wide rename this session: ai_Unk4fe140 -> gm_AddTime (0x4fe140, the f3gm.c definition was already
  gm_AddTime; the ai.h / cm.h / gv.h declarations were the stale alias) - 424 references in 15 files.
- ai_MeisterTrade (0x48a4d0) escalated. Meaning fix KEPT: the goods-object field-delta block is `if (o->unk1c != 0)
  { v = 0; cm_AddFieldDelta(4, 1, o - cm_FieldObject + 0x1c, &v); ... }` (the original tests and writes 0, not 4
  - 0x48aa34) - 4894 -> 4851 differing bytes. Rest: allocation (frame 0x74c vs 0x754, ebp/edi). Q2 gaps:
  ai_Unkf86a24 (gate global, no writer, no name), AIOBJ.unk1c, AIMSG.u.s.unk84, AIMSG.unk9d. Q6 rewritten.
  Field renames this session (byte-neutral, full check clean): AIENTRY.unk44 -> sellAmount (the units to sell,
  set from amount and logged "Verkauf von %li Einheiten"), AISLOT.unk18 -> sellAmount (the SELL_FMT amount,
  counted down), AIOBJ.unk12 -> fill (the cart load in percent, msg.n = 100 - fill), AISLOT.unk08 -> the existing
  bld alias. Names: ai_Unk5098a0 / gm_Unk5098a0 -> gm_BuildingGoodsList, ai_Unk5a88e0 -> ai_SettingFactor,
  ai_Unk4fe140 -> gm_AddTime.
- ai_MeisterTradeAllgemein (0x48bc80) escalated: Q6 rewritten, the same four Q2 gaps as ai_MeisterTrade
  (ai_Unkf86a24, AIOBJ.unk1c, AIMSG.u.s.unk84, AIMSG.unk9d); behaviour = allocation only (`uv run spell` slots
  48beb1 / 48c08c / 48c28b / 48c2af, reloads 48bf16 / 48bf5e / 48c211, iv-shift 48bd87).
- ai_MeisterTradeFernEinkauf (0x48d010) escalated: behaviour = the frame (0x69c vs 0x694, one local too many) and
  the slot rotation it causes (`uv run spell` "code (no menu rule)" hunks are shifted slots); Q2 gaps ai_Unkf86a24,
  ai_Unkf89540, ai_Unkf8a0cc, AISLOT.unk20, AIENTRY.unk4c, AIMSG.u.s.unk84, AIMSG.unk9d; Q4 *(int *)(bl + 1)
  (AIBLD.id) and *(int *)(bl + 0x65) (AIBLD.unk65, no role).

## match-base session 2026-10-26 (base-1 worker, shard 1/10)

- ai_MeisterTradeFernEinkauf2 (0x48d810) -> match-deep: behaviour is allocation / scheduling only (5 code
  hunks: b in edi vs our market in esi, the exp[] / i zero block, eax / ecx roles, cmp BYTE PTR [ecx],0x47 vs
  our mov al,[ecx] with an extra amt home store, 3 nop fill). Tried one edit one build each: AIBLD typed view
  (bl->id / bl->type / bl->unk65) byte-identical and kept (Q4); buf[1028] frame 0x69c, WORSE, reverted; d for
  amt byte-identical, reverted.
- Renames with evidence (byte-neutral, full check clean, 0 regressions): ai_Unkf89540 -> ai_Carts (the kind-0x1d
  cart objects ai_Unk483a60 fills, "Von Auto ... entladen"), ai_Unkf8a0cc -> ai_NumCarts (its count, 0 .. 16),
  ai_Unkf86a24 -> ai_TradeMsgOff (read-only in every source, never written; nonzero skips the trade message and
  its count), AISLOT.unk20 -> saving (the buy sort key = the base price minus the price here; <= 0 ends the
  trade), AIENTRY.unk4c -> buyAmount (min of the entry's amount and its space).
- AIBLD.unk65 stays an evidence gap, but the two roles are now known: a player slot 0 .. 4 for shops / banks
  (gm_GoodsPrice's p; the goods table 0x7d6a70 has exactly 5 columns up to ai_PlayerSlots 0x7e3f40; this
  function indexes exp[] per player with it; ai_CalcMeisterBank's clamp >= 4 is the town slot) and a char*
  work record for a kind-0x1d building (gv_EntryIndex's r; am.c 0x4a3f.. uses AG_4EF6B0(bld->unk65, r)).
  An anonymous union playerSlot / work in ai.h compiled byte-identical here but flipped ai_Unk483a60 (byte) to
  5 differing bytes (the known header tie-break) - reverted, a shared-header decision for match-deep.
