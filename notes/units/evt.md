# evt.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "evt_LoadScript", "evt_RunScript"
- Boundaries: separated (proven): sblib.c / sfx.c tags; own string block 0x5f8484..
- Range then: 0x4273d0..0x42b710
- Was: unk_4273d0.c (round 12)

## Open work
- match 2026-10-23 (7th): evt_Eval 672 B / 186 lines: cause of the esi / edi swap found with c2prio: `val` (prio 151, esi) is allocated before the first evt_Cur web (prio 59, edi); the original has evt_Cur in esi (first insn `mov esi,[evt_Cur]`) and the zero / val in edi. Splitting `val` into two variables (a second `val2` for all assignment sites and uses except the `*=` / `|=` lines, found by a hill climb over the 16 sites) gives evt_Cur = esi and 242 B (338 instructions, was 342; lines unchanged) but is not meaningful C (not kept). Declaration orders of the locals (2880 perms sampled 400): no change. Next: find what natural source lowers val's weight (the original probably has a separate variable for the builtin / array results).
- match 2026-10-23 (7th): evt_DeclareLocal 210 B / 95 lines -> 206 B / 51 lines: the checks first, the copies after: `if (type != IDENT) { if (type == VAR) { if (local) { dup error; Stop; return 0; } } else { unknown error; Stop; return 0; } }` then `if (type == IDENT) strcpy(name, t.u.s); else strcpy(name, var->name);` (this gives the original's block order: error blocks, then the two copies, `je`/`jne` directions equal). Left: the original keeps count in ebx and a zero in edi (`xor edi,edi` at entry, `mov eax,edi` in the error return, which also has ONE shared evt_Error call after both message pushes and `add esp,0xc` before the evt_Stop push; ours has two calls and a deferred `add esp,0x10`). Tried: zero local `z` (all declaration orders, returns of z: folded), ternary message in one evt_Error call in four structures (199 B but 103 lines: the ident copy moves in front), a `goto stop` shared Stop (no change). evt_If: statement-structure variants of the `evt_SkipStatement` tail (else-block, inverted if, `pc -= 1`, comma return, `ok` / `c` locals): no change (32 B, the original hoists `mov eax,[evt_Cur]; mov ecx,[eax+0x118]` above the `jne` of the SkipStatement test).
- match 2026-10-23 (5th): evt_If: spellings of `level--` / `pc--` (16 variants) canonicalised; the difference is the original scheduling its `mov eax,[evt_Cur]` before the deferred `add esp,4` after the call (same effect as d3_Unk458e70).
- match 2026-10-23 (4th): evt_FindLogic (8 B, n / end esi <-> edi): c2prio gives end -24 (esi) and n -25 (edi), the original wants n above end; `n` as a second for-increment (`p++, n++` / `n++, p++`), `return p` / `n + s` / `&s[n]`, `n++` before the test with `s + n - 1`, `n += 1`: no change (8 B) or far worse. evt_Step / evt_If: not touched beyond reading (-1 kept in edi in evt_Step = the same const-in-register web as d3_snd / di_input / mov_Find: the constant 0 / -1 is hoisted into a callee-saved register by the original and never by ours; needs more or hotter references to the constant than the C has, nothing tried yet raises it without changing the code).
- match 2026-10-22 (2nd pass): evt_FindById matched with the early return `if (id == -1) return 0;` before the loop (the nested `if (id != -1) for (...)` put the xor after the test). evt_FindLogic with the same early-return shape (`if (*s == '&' || ...) return 0;`): no change. No gain: evt_Step (-1 kept in edi in the original and stored by `mov [evt_LastId],edi`, `cmp eax,edi`): `evt_LastId = scene`, `scene = evt_LastId = -1`, `sc->unk104 == scene`: constant-propagated, same code. evt_FindLogic: while / n-first / `s[n]` / `&s[n]` forms: no change. evt_DeclareLocal (210 B): the original merges both error exits (`push msg; ... jmp common` with the message chosen first, one evt_Error/evt_Stop/return 0 using the zero register edi; count = 1 in ebx, reused as the constant 1 for the later `type == 1` test) and puts the IDENT strcpy after the error block; `t.type == VAR ? msg1 : msg2` merges, a `!= IDENT` outer if, a switch: none gives that layout (199-206 B).
- see notes/units/d3_anim.md, Open work: "Nonmatching functions (177 after round 17): `uv run ledger check --verbose | grep FAIL`; per-functi ..."

## Worker "script" (round 2): 0x4273d0..0x42abc0, event script interpreter [script]

### Report
- Scope 0x4273d0..0x42abc0: 40 functions in src/game/unk_4273d0.c (+ include/unk_4273d0.h).
- Nonmatching: evt_DeclareLocal 0x4277c0, evt_FindLogic 0x427e00 (esi/edi swap only),
  evt_FindById 0x4284a0 (one xor placement), evt_If 0x4289f0 (tail scheduling only), evt_Eval 0x429f30
  (register swap + one strcpy shape). Attempts below.
- TU: no file name leaked (only the scripts directory "x:\engine\gfx\scripts\"), so it stays unk_4273d0.c.
  It starts at 0x4273d0 right after the sample library and continues past 0x42abc0: FUN_0042ada0
  (evt_Unk42ada0), FUN_0042b410 (evt_Unload), FUN_0042b540 (evt_FindBuiltin) are evt_ functions, and a
  function starts at about 0x42abc0.
- Shared headers: none changed. My own header declares mem_Alloc/mem_Free/err_Message/vfs_Tfopen/
  vfs_fclose/vfs_fread (vfs ones with void * instead of VFSFILE *) and d3_CurrentScene, vfs_Root.
- VC6 lessons:
  - A loop-invariant test in the loop condition (`for (i = 0; f && i < 8; i++)`) gives an unrotated loop
    (test at the top, `jmp` back) with i zeroed before the invariant test (evt_FindVar).
  - `for (k = start, len = strlen(s); !found && k < len; k++)`: putting the bound in the for initializer
    (not a statement before) changed the loop to reload k at the top (evt_GetToken).
  - A value returned from a callee-saved register on the failure path came from
    `for (...) if (free) { v = ...; break; } if (!v) error; return v;` (evt_AddLocal).
  - An `__asm int 3` else-branch leaving a local uninitialized gives that local a stack home slot (evt_Declare).
  - Builtins get pointers to their argument slots; a char argument through a `char c` local copied as int
    (`args[i] = *(int *)&c`) un-merged two identical if-branches (evt_CallBuiltin).
  - va_arg instead of walking `&n` matched evt_RunScriptArgs (register push order).
  - A global char array (not a string literal) is why `strlen(path)` is computed at run time (evt_LoadScript).
  - Switch case source order decides block order (evt_EnterFunction 2,1,7,6; evt_Eval comparison returns).
  - `if (a) {big} else {small}` vs. inverted: the out-of-line block is the else (evt_Compile, evt_While,
    evt_Error, evt_GetToken digit branch).
  - A float stored as `fstp [t+4]` was a float union member, not `(int)(float)` as Ghidra showed.
  - `n = 1; evt_Error(...)` vs. the other order moves the constant load around the call.
- Outside scope: none touched.

TU: src/game/unk_4273d0.c + include/unk_4273d0.h (no file name leaked; function names from the
"evt_LoadScript", "evt_RunScript", "evt_EnterFunction" strings, the rest are ours). The TU continues past
0x42abc0 (callees FUN_0042ada0, FUN_0042b410, FUN_0042b540 are evt_ functions too; a function starts at
about 0x42abc0).

### Attempts on nonmatching functions
- evt_DeclareLocal (0x4277c0): the original keeps 0 in edi from the entry (`xor edi,edi` right after
  `mov ebx,1` for count) and returns it on both error paths (`mov eax,edi`); ours folds it to `xor eax,eax`
  and lays the blocks out differently. Tried: `int r = 0` returned by the errors, `EVTVAR *v = 0` with
  `v = evt_AddLocal(); return v`, an ok flag tested once, if/else nesting in both directions, a goto to a
  shared error tail, message chosen in a variable. Best was the nested `if (t.type != EVT_IDENT)` form (69 asm
  lines), kept the plain form.
- evt_FindLogic (0x427e00): only n/end swapped between esi/edi. Tried declaration order, `p = s` in the
  initializer or before, `&s[n]`, `++n`, an `e = end` copy, the first test in the loop condition.
- evt_FindById (0x4284a0): the original sets eax (i) to 0 before the `id != -1` test and has id in esi;
  ours sets eax after it. `int i = 0` moves the xor but puts id in edx. Tried while/do/for forms, the test
  in the loop condition (as in evt_FindVar, worse here), an early return, break + `if (i < 128)`.
- evt_If (0x4289f0): only the tail differs: after evt_SkipBlock the original loads evt_Cur->pc and
  decrements before the pops, and the evt_SkipStatement success path jumps into that block (cross jump).
  Tried `--pc`, `pc -= 1`, `pc = pc - 1`, the decrement in both branches, an explicit pos local.
- evt_Eval (0x429f30): layout and data flow match; left: esi/edi swapped (evt_Cur/token type vs val),
  the strcpy of an array element into evt_StrResult is a two-pointer loop in the original (relative
  addressing in ours), and op arithmetic order (`imul edx,edi`). Tried: volatile result (wrong: it stops
  the shared `mov [result],eax` store), init orders, `result = result * val`, explicit copy loops,
  `strcpy(.., (char *)(val = ...))`.

## Worker "evt" (round 3): 0x42abc0..0x42c050 [evt]

### Report
- Scope 0x42abc0..0x42c050: 38 Ghidra functions plus 2 that Ghidra missed (0x42b6c0, 0x42b700).
- unk_4273d0.c (evt TU continued): evt_ResumeCall 0x42abc0, evt_StepAll 0x42aef0,
  evt_Shutdown 0x42af30, evt_UnloadAll 0x42b010, evt_Init 0x42b050, evt_Unload 0x42b410,
  evt_FindBuiltin 0x42b540, evt_ImportCommand 0x42b5b0, evt_StartScript 0x42b6c0 (no Ghidra function),
  evt_GetStartScript 0x42b700 (no Ghidra function; returns &evt_StartScript), evt_ResetUnk5f8430 0x42b710.
- unk_42b720.c + include/unk_42b720.h (Miles Sound System output; only leaked name is the
  mem_Alloc tag "sfx_OpenDigOutput", so functions are sfx_*): 0x42b720 sfx_Startup, 0x42b750 sfx_Shutdown,
  0x42b780 sfx_SetMasterVolume, 0x42b970 sfx_CloseDigOutput, 0x42b9f0 sfx_CloseAll, 0x42ba30 sfx_Release,
  0x42ba70 sfx_ReleaseAll, 0x42bab0 sfx_Reacquire, 0x42bb00 sfx_ReacquireAll, 0x42bb40 sfx_SetNumSamples,
  0x42bb80 sfx_AllocSample, 0x42bbf0 sfx_SetSampleFile, 0x42bc40 sfx_ReleaseSample, 0x42bca0
  sfx_ReleaseAllSamples, 0x42bd00..0x42be40 sfx_Init/Start/Resume/EndSample, sfx_SetSampleVolume/Pan/
  LoopCount, 0x42be80 sfx_SampleStatus, 0x42beb0 sfx_OpenStream, 0x42bf20 sfx_CloseStream, 0x42bf80
  sfx_CloseAllStreams, 0x42bfe0 sfx_StartStream, 0x42c010 sfx_PauseStream.
- Nonmatching: evt_Step 0x42ada0 (register caching of evt_Depth / -1 constant, 14 asm lines),
  sfx_OpenDigOutput 0x42b7c0 (ebx/edi swap only). Attempts below.
- TU boundaries: unk_4273d0.c (evt) ends at 0x42b71a (evt_ResetUnk5f8430). unk_42b720.c starts at 0x42b720
  and continues past the scope end 0x42c050 (stream volume/loop/pos at 0x42c050.., then the slot helpers
  0x42c1c0..0x42c380 that are already declared in unk_42b720.h).
- Shared headers: none changed. include/unk_4273d0.h (my TU's own header) got the new prototypes and
  globals (evt_Stack, evt_Builtins, evt_Console, evt_CurCall, evt_Depth, evt_LastId) and a d3_SelectScene
  prototype; evt_Unk42ada0 renamed evt_Step (one caller in evt_Stop), evt_Unload now returns int.
  include/unk_42b720.h declares the mss32 imports itself (`__declspec(dllimport) __stdcall`),
  MSS_auto_cleanup (0x5de460, mss32.lib static code, called before AIL_startup), mem_Alloc/mem_Free and an
  SFX_ZERO copy of d3.h's D3_ZERO (to avoid pulling in d3.h/d3d.h).
- For the orchestrator: msx.h declares these same functions as snd_* with partly wrong types
  (snd_OpenDriver = sfx_OpenDigOutput 0x42b7c0, snd_CloseDriver = sfx_CloseDigOutput, snd_SetDriverVolume =
  sfx_SetMasterVolume, snd_OpenStream = sfx_OpenStream, snd_CloseStream, snd_StopAll = sfx_CloseAllStreams
  0x42bf80, snd_StreamRewind = sfx_StartStream 0x42bfe0, snd_StreamPause = sfx_PauseStream 0x42c010,
  snd_StreamVolume.. 0x42c050..0x42c180 next scope). Header cleanup: point msx.h at unk_42b720.h names.
- VC6 lessons:
  - Three global zero stores done late from `xor eax,eax` (instead of a zero register shared with an
    earlier `mov [x],0` immediate) were one `memset(&evt_Globals, 0, 12)` -- the three globals
    (evt_Globals/evt_NGlobals/evt_MaxGlobals) are probably one struct. Separate `= 0` stores make VC6 keep
    0 in a register from the top of the function, which shifted the whole strcpy schedule (209 asm lines).
  - Long runs of inlined strcpy of short literals: the scheduler interleaving follows source order;
    moving one strcpy (keyword "if") by one or two positions was enough (try every single-line move:
    81 variants in one `uv run try` run).
  - A pointer walk over varargs that reads `mov edi,[ecx+4]; add ecx,4` with the base `lea` computed
    in the loop preheader after the `nargs > 0` test is `va_arg` (evt_ImportCommand), not `*++p`.
  - `if (i >= 256) return 0; return &a[i];` vs `if (i < 256) return &a[i]; return 0;` decides which
    return is the fall-through (evt_FindBuiltin).
  - `int ret = 0; if (...) {...} else ret = -1; return ret;` (with every nested failure `else ret = -1`)
    was the shape of all 25 small MSS wrappers; early `return -1` gives `xor eax,eax`/no callee-saved
    zero. The zero in the ret register is reused for other zero arguments/stores (`push edi` for a
    0 argument to AIL_set_named_sample_file / AIL_open_stream).
  - `if (cond) { x = &arr[i]; use x... }`: a pointer local to a global array element is folded back into
    `[i*4+arr]` addressing; writing `arr[i]` everywhere gave the original's `lea esi,[ebx*4+arr]` CSE
    (the inline-asm zero macro clobbers ebx, so VC6 precomputes the addresses itself).
  - A store to a global followed by a test of the same global (`evt_Cur = &tab[i]; if (evt_Cur->f & 1)`)
    schedules the store before the test; the original's store inside the if was `if (tab[i].f & 1)
    { evt_Cur = &tab[i]; ... }`.
- Outside scope: none touched.

### Attempts on nonmatching functions
- evt_Step (0x42ada0): the original keeps evt_Depth cached in edx (reloaded right after every call) and
  -1 in edi (stored to evt_LastId and used for `unk104 == -1`); ours uses immediates and loads evt_Depth
  only where needed. The pop at the end is duplicated in the original and cross-jumped (first copy jumps
  into the second after `dec edx; mov evt_Cur,ecx`). Best so far: a `goto pop` into the shared tail
  (14 asm lines). Tried: chained `evt_LastId = scene = -1`, `scene = evt_LastId = -1`, `evt_LastId = scene`,
  comparing `unk104 == scene`, a late `scene = -1`, `else scene = -1`, the inverted outer condition.
- sfx_OpenDigOutput (0x42b7c0): only i (slot index) and the (bits >> 3) product temp swap ebx/edi.
  Matched the rest by writing `sfx_Drivers[i]->...` everywhere (no `SFXDRIVER **d` pointer local; with
  it, VC6 folds the pointer into `[i*4+X]` addressing). Tried: operand orders of the products, `/ 8`,
  int casts, a `b = bits >> 3` local, `(i = slot()) >= 0` in the condition, `int bits`, a copy of i for
  the zero macros.
- (match-base 2026-10-24) EVTSCRIPT.unk104 renamed `group` and evt_Unk5f8430 renamed evt_NoGroup (evidence:
  chc.h CHC_GROUP is the same field and names it group; the global is -1, the "no group" sentinel). Name
  only: no position changed (uv run ledger check --against: 0 regressions).

## match-base 2026-10-24 (base-3 worker, shard 3/5)

- The base queue of the unit is empty: 47 functions byte + readable, evt_If behaviour + readable, and
  evt_DeclareLocal / evt_FindLogic / evt_Eval / evt_Step escalated to match-deep (each `@deep` reason in its
  address comment names the hunks read): DeclareLocal count=1 in ebx and the error 0 in edi plus the merged
  error tail; FindLogic a pure n/end esi<->edi swap; Eval evt_Cur/val esi<->edi and the inline string-copy
  loop; Step the -1 in edi, evt_Depth cached in edx and the duplicated pop tail.
- Q6: every function is described in include/evt.h (purpose, return value, side effects); the description
  carries text after the address there, so the bare /* 0xADDR */ at the definitions stays valid.
- Q2/Q4: 0x6332e0 -> evt_ReturnValue; evt_ResetUnk5f8430 -> evt_ResetNoGroup; evt_StartScript's unused
  parameters; EVTVAR.data is the EVTVARDATA union (p/i/b/s/f, same 4 bytes); EVTVAR.typeWord /
  EVTFUNC.nameWord / EVTBUILTIN.nameWord carry the 4-byte scratch clears of evt_Compile/evt_Init;
  EVTBUILTIN.fn is a union of fn0..fn7 prototypes (evt_ImportCommand still stores the K&R pointer);
  evt_CallBuiltin's byte argument goes through a char/int union; EVTSCRIPT.unkc94 -> character (CHARACTER *),
  CHARACTER.unk028/unk02c/unk030 -> scriptId/group/scriptGroup; Unk40b710 -> evtc_Wait (evt.c includes
  evtc.h). evt.c now includes the ch.h chain as ch.c does (windows.h, d3d.h, d3.h, d3_anim.h, d3_sm.h,
  path.h, ch.h) - ch.h needs d3.h and the d3_Unk44e4a0/d3anim_StartAnim define dance first.
- Still open for the unit's readability (header canonical): he.c, sv.c, unk_575860.c, unk_58bbd0.c and cm.h
  declare the evt functions under the old ai_Unk* names (ai_Unk4284a0 = evt_FindById, ai_Unk4287b0 = evt_Load,
  ai_Unk429ab0 = evt_RunScriptArgs, ai_Unk429e90 = evt_Stop, ai_Unk429c40 = evt_RunCommand, ai_Unk4299a0 =
  evt_RunScript, ai_Unk428530 = evt_LoadScript); replacing those declarations by #include "evt.h" and renaming
  the call sites project-wide was not done. `uv run ledger grade evt.c` lists them.
- (match-base 2026-10-24, base-4 worker) evt.h's stray `int Unk40b710()` (evtc_Wait) is gone; evt.c includes
  evtc.h and compares `evt_Cur->waitfn` against evtc_Wait. No code changed (evt.c 47/52 before and after).

## match-base 2026-10-25 (base-3 worker, shard 3/6, session 4)

Q7 canonicalisation (41 foreign declarations -> 0), all by name, no byte changed (unitdiff --against on
evt.c and every touched unit; check 2569/4664):
- the placeholder call sites renamed to evt.h's names: ai_Unk4284a0 -> evt_FindById, ai_Unk428530 ->
  evt_LoadScript, ai_Unk4287b0 -> evt_Load, ai_Unk4299a0 -> evt_RunScript, ai_Unk429ab0 ->
  evt_RunScriptArgs, ai_Unk429c40 -> evt_RunCommand, ai_Unk429e90 -> evt_Stop, ai_Unk42af30 ->
  evt_Shutdown, with the script handles cast (EVTSCRIPT *) / the int globals cast (int).
- files: f3init.c, he.c, main.c, sv.c, cm.c, unk_575860.c, unk_58bbd0.c include evt.h; their local evt_*
  declarations are gone; include/cm.h lost its 8 evt lines (cm.c is the only caller).
evt.c: 52 functions, readability readable (the 4 behaviour drafts stay @deep).
