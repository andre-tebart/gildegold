# sblib.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Worker sblib (round 2) -- final report [sblib]

Scope: 0x4214a0..0x4223c0, 0x426000..0x4273d0, 0x418ce0..0x41be00.

### Files
- unk_4214a0.c (error handler): err_Write 0x4214a0, err_WriteSymbol 0x421d50, err_GetString 0x421ed0,
  err_IsCodeProtect 0x421f00, err_CheckCall 0x421ff0, err_Init 0x4220b0, err_Exit 0x422250, err_Message 0x4222b0,
  err_Log 0x422330.
- unk_425bc0.c (sample library, appended): sb_GetSample 0x426030, sb_FindEntry 0x4260c0, sb_SetRestart 0x426170,
  sb_GetVariation 0x426190, sb_SetPath 0x426240, sb_FindBank 0x426290, sb_FindBankFile 0x4262c0,
  sb_FindSampleOf 0x4262f0, sb_FindOldestEntry 0x426350, sb_GetBankOf 0x426450, sb_FreeMemory 0x4264a0,
  sb_AllocSample 0x426540, sb_LastBank 0x426630, sb_LoadSampleBank 0x426950, sb_UnloadAll 0x426db0
  (was snd_Unk426db0), sb_IsPlaying 0x426f20, sb_IsVariation 0x426f60, sb_PlaySample 0x426fa0, sb_Resume 0x427010,
  sb_PlayVariation 0x427050, sb_Stop 0x427290, sb_SetPan 0x4272f0, sb_SetVolume 0x427330,
  sb_SetMasterVolume 0x427370, sb_SetLoops 0x427390, sb_IsInit 0x4273c0.
- unk_418ce0.c: d3_Unk418ce0 0x418ce0 (char -> key code table, D3_ZERO_AT on the parameter).
- dd_vesa.c: dd_Restore 0x41bdd0 (first function of the TU, placed before dd_Exit).

### Nonmatching (in scope)
- sb_LoadEntry 0x426660 (35 asm lines): structure, slots and registers match after rewriting it with a single
  `return ret` and `else ret = -1` branches; left: in the original the header field stores (`d->len`,
  `d->data` / `v->len`, `v->items`) come after pushing fread's arguments and the length is copied reg-to-reg.
  Tried: decl order (fixed the zero registers), assignments inside the fread arguments (3 forms, no effect),
  a local `n` for the length (worse).
- sb_Play 0x427110 (20 asm lines): `int ret = 0` in ebp and `goto end` for every failure gets everything but block
  placement: the original puts the shared `ret = -1` block right after the snd_SetSampleData test, ours at the
  end. Tried ~18 variants: early returns, `return ret = -1`, fail label inline / in an else, fully nested
  if/else with sentinels.
- sb_UnloadSampleBank 0x426ba0 (48 asm lines): single exit fixed most; left: `prev = NULL` hoisted before the
  driver test in the original, bank search loop shape, slot order of ret/i. Tried prev init at the declaration,
  `if (prev)` order, 3 loop forms, volatile ret.
- sb_Update 0x425d20 (round-1 draft, not worked on).
- d3d_Error 0x418e90 (12 KB, one function, not several): `switch (d3d_hr)` with 110 cases, each
  `strcpy(buf, "DDERR_X"); strcat(buf, ": "); strcat(buf, "text"); break;` into an unused `char buf[1024]`.
  The source is generated from the exe (scratch scripts parsed the case bodies and emulated the dispatch; all 110
  value -> body mappings check against the DX6.1 header macro values; body order = original order). The bodies
  match; the compare tree / jump-table clustering does not. The original tree has a node for 0x887602e4
  (D3DERR_MATERIAL_CREATE_FAILED) jumping to the epilogue, and its root sits at index 92 of the 110 used values:
  probably about 75 more cases (the unused D3DERR_* codes 0x887602bc..0x88760826) whose bodies end up empty.
  VC6 prunes `case X: break;`, `return;`, `(void)0;` and dead-store bodies before building the tree, so those
  labels could not be reproduced yet. Next idea: bodies only removed after switch lowering.
  Resolved 2026-10-23: the guess was right (all 75 D3DERR_* codes); a folded `if (0) { }` body keeps
  the labels in the tree and tables and emits no code. d3d_Error now matches (see notes/units/dd_vesa.md).

### In scope, no source yet
- 0x4215c0 err_ExceptionFilter (unhandled exception filter, ~0x790 bytes, not a Ghidra function: no pseudocode,
  batch_decompile refuses it; use uv run disasm 0x4215c0 0x790). Prototype is in unk_4214a0.c
  (`LONG WINAPI err_ExceptionFilter(EXCEPTION_POINTERS *)`). It uses err_WriteSymbol, err_CheckCall,
  err_IsCodeProtect and the exception-name strings at 0x5f6f14.. ("_crashdump.log"). No names.csv row yet.

### TU boundaries
- unk_418ce0.c: 0x418ce0..0x41bdd0 (after d3_snd.c, before Dd_vesa.c). Not proven that both functions share a TU.
- dd_vesa.c starts at 0x41bdd0 with dd_Restore (it goes at the start of the file, not the end).
- unk_425bc0.c: the sample library ends at 0x4273d0 (sb_IsInit is its last function).
- unk_4214a0.c: 0x4214a0..0x4223c0, including the exception filter at 0x4215c0.

### Shared-header additions
None. The d3d_Error, d3_Unk418ce0 and dd_Restore prototypes already existed. The definition of d3_Unk418ce0 takes
`unsigned char *`, dd_vesa.h declares `void *`; unk_418ce0.c does not include dd_vesa.h. Inside unk_425bc0.c:
SBSAMPLE.info is now `SBENTRY *`, start is `int`, fields var/pan are named, new SBDATA/SBVAR/SBENTRY/SBBANK;
sb_Unk1460a24/sb_Unk1460a60 became sb_Dummy.info / sb_DummyEntry.

### General VC6 lessons
- Callee-saved pushes come after an early `if (!x) return y;` (the early path pops nothing) when the rest of the
  function is written with a single `return` at the end. VC6 still duplicates the epilogue into several
  returns in the output. Fixed sb_GetSample, sb_PlaySample, sb_PlayVariation, sb_FindOldestEntry and
  sb_LoadSampleBank.
- `xor esi,esi; mov eax,esi` on a failure path = `else s = NULL;` followed by a shared `return s;`.
- `or bl,0xff` + `cmp [..],bl` is a (signed) `char` compared with -1. With unsigned char and 0xff the constant
  goes through `mov bl,0xff`.
- Initialized local char arrays (`char x[] = "??? "`) declared in an inner block are initialized where the block
  starts. Declaration order sets the copy order. A global byte read early into a register is a local copy.
- Strength-reduced loops over a global array compared with `jl` are index loops. Pointer loops give `jb`.
- `abs(0x7f) / 2` is not constant-folded.
- `while (1) { if (c) break; ... }` keeps the test at the top. `while (!c)` gets rotated.
- An empty `case X: break;` (also `return;`, `(void)0;`, a dead store) is dropped before VC6 builds the switch
  tree.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "sblib successfully initialzied", "sb_Init"
- Boundaries: separated (plausible): own literal block 0x5f8140..0x5f8310; the outliers 0x5f3ce8 / 0x5f72e8 of tustrings are false hits (call displacements; layout 2026-10-22); start next to path.c (guess)
- Range then: 0x425bc0..0x4273c0
- Was: unk_425bc0.c (round 12)

## Open work
- match 2026-10-23 (8th): no new attempt on sb_Play / sb_UnloadSampleBank (re-read only).
- match 2026-10-23 (7th), no gain: sb_UnloadSampleBank (8 B, slots): `int ret;` without initializer with `ret = 0` at the end of the then-branch / at the top, `} else { ret = -1; }`: same 8 B (32 variants).
- match 2026-10-23 (5th): sb_Play with every failure as `return -1` (no goto) puts an epilogue before every return (worse: the original has exactly two epilogues, the failure one right after the SetSampleData call with all failures jumping into it, the success one at the end shared with the `ret != 0` exit); sb_UnloadSampleBank without the `path` local (bank->path / b->path at both sprintf calls, 128 variants): same 8 B.
- match 2026-10-23 (4th): sb_UnloadSampleBank still 8 B (slots). Also tried, no change: i / k / path declared at block scope (inner `int k`, `char *path = ...`), and ret used as a live-looking value (`sb_Stop(s, ret)`, `k = ret`, `i = ret`, `prev = (SBBANK *)ret`, `data = (void *)ret`; 371 sampled): VC6 folds ret == 0 everywhere.
- match 2026-10-23 (3rd): sb_UnloadSampleBank slots: ours path -4 / ret -8 / i -12, the original ret -12 / i -8 / path -4 (uv run listing variable map; c2prio: i -21 eax, path -41, ret -96, all spilled). No change (8 B) from: dead early stores (`ret = 0;` once or twice at the top, `int ret; ret = 0;`, `i = 0;` / `path = 0;` / `ret = 0;` before either if), code-free ret uses on the early return (`return ret = 1`, `{ ret = 1; return ret; }`, `return ret + 1`, `return ++ret`): all folded before slot assignment.
- match 2026-10-23 (2nd): sb_Play: fail label INSIDE the SetSampleData `if` body with every earlier failure `goto fail` (body: `ret = -1; return ret;` / `goto end` / `return ret = -1` / `return -1`, and the if/else form with the rest in the else): 293 B unchanged or worse (55 lines). sb_UnloadSampleBank: `register` on ret / i / k: no change.
- match 2026-10-22 (2nd pass): sb_LoadEntry matched: `fread((char *)(d + 1), d->len, 1, f)` / `fread((SBDATA *)(v + 1), v->len, 1, f)` (the fread arguments recomputed from d / v after the member stores, not d->data / hdr.len) - the stores then sink behind the argument pushes like the original. sb_UnloadSampleBank 401 -> 341 B: `SBBANK *b, *prev = NULL;` as the initializer (xor eax,eax hoisted). Left: ret slot at [esp+4] in the original ([esp+8] ours; declaration orders do not change it), `if (prev != NULL) prev->next = ...; else sb_Banks = ...` gives the original's branch layout but counts more differing instructions (94 vs 40) because the loop head still differs: the original loads `[edx+ebp+0x38]` into ecx before `lea eax,[edx+ebp]` (`dp = e->data` local, `b->entries[i].data` test first: no gain). sb_Play: else-form / goto fail: no change (the shared `or ebp,-1` epilogue stays at the end). sb_Update 361 -> 5/784 B (3 asm lines): no `el` temp in the first fade block (`now - s->fadestart` twice), and the stop branch as `fl = s->flags | 8; s->flags = fl; s->fadestart = 0; s->flags = fl & 0xfc;` (a temp keeps both flag stores; `|= 8; ... &= ~3` merges into one store). Left: the original's second op is `and al,0xfc` (byte op, as from bitfields) after the first store `mov [esi+0x14],eax`; ours drops the first store and uses `and eax,0xfc`; bitfield union views (merge), `unsigned char` temps, `volatile` read (gives the second store but still `and eax`) tried.
- match 2026-10-23: sb_Update MATCHED: the original's two flag stores are `*(volatile unsigned int *)&s->flags = fl;` (first store) then `s->flags = fl & ~3;` (`~3` gives `and al,-4`, `0xfc` gives `and eax,0xfc`); every non-volatile spelling (temps, casts, order) merges the stores (the 10-22 note that a temp keeps them was wrong).
- match 2026-10-23: sb_UnloadSampleBank 341/40 -> 8 B / 16 asm lines (stack slots only). What fixed the body: `if (prev) prev->next = ...; else sb_Banks = ...`; the found-body inside the search loop (`for (b = sb_Banks; b; b = b->next) { if (b == bank) { ...; break; } prev = b; }`, no `if (b)` after it); `if (b->entries[i].data)` as the first test with `e = &b->entries[i]` before it. Left: ret is at [esp+4] in the original, ours [esp+8], and i swapped with it ([esp+0x14] vs [esp+0x10]); an extra `ret = 0;` after the first sprintf gives the right slots (then everything matches but that one extra store), so ret needs more weight than i: tried code-free uses (`ret |= 0`, `ret = ret + 0`, `ret = ret ? ret : 0`, ... all folded), `return ret` inside the found arm (worse), declaration orders (700 permutations), i in an inner block / unsigned / `++i`, `e->`-spellings of the entries[i] uses (all worse).
- match 2026-10-23: sb_Play still 293 B / 20 asm lines: VC6 puts the shared `ret = -1` block (with `or ebp,-1`) at the very end whatever the source form: goto-fail inside the last if, `if / else` with the tail in the else, label before `end:`, all four failure sites x {`ret = -1; return ret`, `return ret = -1`, `ret = -1; goto end`} x {load failure `goto end` / `return ret`} (162 variants) - none gives the original's inline F with a duplicated epilogue; `return ret` forms move ret out of ebp.
- match 2026-10-22: sb_Play: the original has the shared `or ebp,-1` failure epilogue right after the snd_SetSampleData test (jne to the
  continuation) and the `end:` return at the very end; goto / `return -1` / `ret = -1; return ret` at each of the four failure sites (15
  combinations) all gave 314+ B.

## match-base 2026-10-24 (base-0 worker, session 2)

- sb_Init, sb_Exit: Q6 only, now readable. sb_Update: Q4 only - the volatile flag store is
  SBSAMPLE.volatileFlags, a union member over the flags dword (the same bytes as the raw volatile cast, no raw
  access); the two-store sequence is unchanged (byte).
- Q6 for the rest of the API: sb_FindEntry, sb_GetVariation, sb_SetPath, sb_FindBank, sb_FindBankFile,
  sb_FindSampleOf, sb_FindOldestEntry, sb_GetBankOf, sb_FreeMemory, sb_AllocSample, sb_LastBank,
  sb_LoadSampleBank, sb_UnloadSampleBank, sb_UnloadAll, sb_IsPlaying, sb_IsVariation, sb_PlayVariation,
  sb_Stop, sb_SetVolume, sb_SetMasterVolume. 30/32 byte; sb_UnloadSampleBank is behaviour (slots 24).
- sb_Play -> match-deep: the failure epilogue placement (inline after snd_SetSampleData in the original, the
  shared end block in ours); the 162 source variants of the earlier rounds are in this note. Q6 added.
- Still open for the unit grade: the canonical header. d3_snd.c declares sb_GetSample / sb_SetRestart /
  sb_Resume / sb_SetLoops (its own views; sb_SetRestart takes char* there) and calls 0x426fa0 via the alias
  snd_Unk426fa0; f3init.c and he.c call 0x426fa0 / 0x427290 / 0x426030 through the K&R aliases ai_Unk426fa0 /
  ai_Unk427290 / ai_Unk426030, f3init's call passing 5 args where sb_PlaySample has 7 (@original evidence
  needed). The canonical fix is an include/sblib.h plus the callers including it.
