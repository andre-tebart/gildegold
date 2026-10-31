# txt.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Open work
- txt_OpenText 0x42cbc0 (match-base 2026-10-31 opus3, deep, size-explained 1729 vs 1774): i is memory-homed in the original (no lea of its slot anywhere, yet it is stored at every definition, reloaded after the byte store txt_Ptrs[i][k] = 0, and the init loops store their final values 0x80 / 0x4800). Forcing i into memory (experiment: (int)&i used once at the end, not committed) gives 1773 vs 1774, equal switch tables and the same 12 epilogues: the extra "Could not open TextFile" tail is the zero register (ours push ebx / mov [txt_Buffer],ebx cross-jumps into the HeaderFile tail). Still unexplained: what made i aliased in the original source (q = (char *)&i, *&i, an __inline helper taking &i fold away; an out-of-line helper adds a call; memset(&i,0,0) emits xor ecx/eax); the dead memset(txt_Random,0) survives only when the memset and the 0xff loop name different symbols (ai_Unk114fe60 vs txt_Random: one object, two names - rejected); the empty if (txt_Count == 0x71b) cmp/jne +0 (i = i, end = i, col = col, txt_GroupFirst[ngroups] = txt_Count tried; the last keeps a second store).

## Worker "sfx" (round 4): 0x42c050..0x42e2a0 [sfx]

### Report
- Scope 0x42c050..0x42e2a0: 22 Ghidra functions (0x42c3a0 is an mss32 import thunk,
  `AIL_get_timer_highest_delay`, not game code). 0x42cbc0 is one 5.7 KB function (txt_OpenText) with a
  switch jump table and its index table at 0x42e204..0x42e29b; 0x42e070 is a block inside it, not a function.
- Files:
  - unk_42b720.c: 0x42c050 sfx_SetStreamVolume, 0x42c090 sfx_SetStreamLoopCount, 0x42c0d0 sfx_SetStreamPos,
    0x42c110 sfx_StreamStatus, 0x42c140 sfx_GetStreamPos, 0x42c180 sfx_GetStreamLength, 0x42c1c0
    sfx_FreeDriverSlot, 0x42c1e0 sfx_FindDriver, 0x42c200 sfx_FreeSampleSlot, 0x42c240 sfx_FreeStreamSlot,
    0x42c280 sfx_FindSample, 0x42c2c0 sfx_FindStream, 0x42c300 sfx_SampleDriver, 0x42c340 sfx_StreamDriver,
    0x42c380 sfx_NumDrivers.
  - unk_42c3b0.c (text definition files, txt_*): 0x42c3b0 txt_Field, 0x42c430 txt_SkipSpaces,
    0x42c480 txt_TrimRight, 0x42c4b0 txt_ParseLine, 0x42c9c0 txt_LoadIncludes.
- Nonmatching (2): 0x42c950 txt_ParseRandomText (25 bytes, one reload), 0x42cbc0 txt_OpenText (same block
  layout, register allocation differs throughout). Attempts below.
- TU boundaries: unk_42b720.c ends at 0x42c398 (sfx_NumDrivers); 0x42c3a0 import thunk; unk_42c3b0.c starts
  at 0x42c3b0 and continues past the scope end 0x42e2a0 (txt_LoadIncludes calls 0x42e530, outside the
  scope). Leaked names: strings "txt_ParseRandomText" (0x42c950's error message)
  and "txt_OpenText"/"txt_openText" (0x42cbc0's MessageBox captions).
- Shared headers: none changed. include/unk_42b720.h (own TU header) got the five stream imports
  (AIL_set_stream_volume/_loop_count/_ms_position, AIL_stream_status, AIL_stream_ms_position) and the six
  stream prototypes. New include/unk_42c3b0.h with the txt_* globals and a TXTFILE struct (0x74 bytes).
- msx.h's snd_StreamVolume..snd_StreamLength (0x42c050..0x42c180) are these sfx_* functions; they take
  HSTREAM and int / int * and return int (0 or -1).
- VC6 lessons:
  - An uninitialized local assigned on one path only is read from a parameter's stack slot on the other
    path (`char *res;` in txt_Field loaded as a dword from the `char type` slot). It looks like a 4th
    argument, but the callers pop 0xc.
  - Register choice between a local and a CSE temp (`n - 1`) flipped by splitting `if (a || b) x = s;` into
    `if (a) x = s; if (b) x = s;` -- same instructions, different reference weights.
  - Repeating the cleanup at each failure site (instead of `goto fail`) matched again: VC6 tail-merges the
    copies, and the eax/ecx/edx rotation in each copy follows source order (txt_LoadIncludes).
  - `strcpy(suffix, q + 1)` after `if (strlen(q + 1))` gives the `lea edx,[esi+1]` CSE; incrementing q
    first does not.
  - Inline `memcpy(dst, src, strlen(src) + 1)` gives `repnz scasb; not ecx; rep movsd/movsb`; strcpy to a
    computed destination gave the byte loop.
  - `if (x == C) ;` and any body that folds away (self-assignments, dead stores, empty `__asm {}`, an empty
    inline function, a same-value store in both branches) drops or changes the compare; the original's
    `cmp eax,0x71b; ...; jne <next instruction>` in txt_OpenText remains unexplained. 2026-10-26: also
    tried a one-case `switch (txt_Count) { case 0x71b: break; }`, a `goto` to a label on the next statement
    and `while (txt_Count == 0x71b) break;` - all three compile identically to the empty `if` (no compare,
    unitdiff 1463 asm lines before and after), so the dead compare is not reachable with these shapes either.
- Outside scope: sfx_OpenDigOutput (0x42b7c0) is still nonmatching (round 3).

### Attempts on nonmatching functions
- txt_Field (0x42c3b0, matched): the uninitialized `res` makes VC6 load it from the `type` parameter slot
  (`mov edi,[esp+0x1c]` in the else path). Early `return p` inside the loop is a `goto` into the
  `type == 4` return block. Register order of res vs `n - 1` was fixed by the two separate ifs.
- txt_ParseRandomText (0x42c950): the original reloads p from its stack slot in the two store-p branches
  (ours reuses esi). Tried: `*(int *)&p`, a `char **` table, a local copy (`s = p` in the call and as a
  declaration), an int parameter with a char * copy, merging the two stores (22 bytes), goto into a shared
  store, inline helpers for strncmp/p[2], `n < 10`, nested ifs, if/else for the range test, `&p[0]`, an
  unsigned char read, int return.
- txt_OpenText (0x42cbc0): all blocks, calls and the switch are in the original order (shape diff ~425 of
  ~1460 lines once registers and stack offsets are normalized). The original keeps the char position `i` in
  memory ([esp+0x10], cached in edx), lineno in ebx and 0 in ebp; ours enregisters i per loop (separate
  webs: the loops after the line loop get ebx/edi and strength reduction, where the original indexes with
  `shl edx,0xc` each time) and keeps lineno in memory. The original also stores the final values of the two
  init loops (`mov [esp+0x10],0x80`, `0x4800`), so VC6 treated i as one live range there. Tried: i
  int/unsigned, col int/unsigned, lineno int/unsigned/short, i/col as `int ic[2]`, a separate `pos` for the
  line loop, for/while/do shapes of the char loop and both increment orders, moving `lineno++`, memcpy for
  the buffer copy (kept), empty-if bodies for `if (txt_Count == 0x71b)`.
  `sprintf(path, "%s\\resources\\textbin_%s\\", "german")` really passes only one argument (original bug).

## Worker "txt" (round 5): 0x42e2a0..0x42ee60 [txt]

### Report
- Scope 0x42e2a0..0x42ee60: 28 functions (24 Ghidra starts, plus 0x42e7b0,
  0x42ee10, 0x42ee50 missed by Ghidra, plus 0x42ec90, which Ghidra lists as a thunk: it is the tail call
  `util_Seed(s) { srand(s); }`).
- Files:
  - unk_42c3b0.c (txt): 0x42e2a0 txt_FreeFiles, 0x42e2e0 txt_FindFile, 0x42e320 txt_FreeFile,
    0x42e360 txt_ReloadTextFile, 0x42e530 txt_LoadTextFile (both leaked via their mem_Alloc tags
    "txt_txt:txt_ReloadTextFile:PointerTable" / "txt_txt:txt_loadTextFile:..."; the old txt_Unk42e530),
    0x42e770 txt_FindKey.
  - unk_42e7b0.c (time base, tb_*): 0x42e7b0 tb_TimerProc (timeSetEvent callback, `_tb_TimerProc@20`),
    0x42e8c0 tb_Init, 0x42e940 tb_Exit, 0x42e970 tb_OpenFunction (leaked: "tb_OpenFunction: Too many
    TimeBaseProcs!"), 0x42e9d0 tb_CloseFunction, 0x42ea10 tb_SetFunctionOff, 0x42ea50 tb_IsFunctionOff.
  - unk_42ea90.c: 0x42ea90 tr_startup (leaked), 0x42ebc0 tr_shutdown, 0x42ec00 util_Random,
    0x42ec20 util_RandomF, 0x42ec40 util_RandomSq, 0x42ec70 util_RandomF2, 0x42ec80 util_Randomize,
    0x42ec90 util_Seed.
  - unk_430130.c (vfs), at its top: 0x42eca0 vfs_strcmp, 0x42ecf0 vfs_Unk42ecf0 (strupr unless
    vfs_Unk633408), 0x42ed10 vfs_Unk42ed10 ('\' -> '/'), 0x42ed40 vfs_Unk42ed40 ('/' -> '\'),
    0x42ed70 vfs_IGetSubDir (leaked: "vfs:IGetSubDir"; was vfs_Unk42ed70), 0x42ee10 vfs_CmpName,
    0x42ee50 vfs_CmpNameI (strcmp/stricmp comparators against `*(char **)elem`, for qsort/bsearch).
- TU boundaries:
  - unk_42c3b0.c (txt) ends at 0x42e7a5 (txt_FindKey); its .data ends with "Could not open textfile:%s"
    at 0x5f8f88.
  - unk_42e7b0.c (time base) 0x42e7b0..0x42ea81; .data = the one string at 0x5f8fa4.
  - unk_42ea90.c 0x42ea90..0x42ec95; .data = tr_Key1/tr_Key2 pointers at 0x5f8fd0 with their strings,
    then "tr_startup" (0x5f8ff8). The six random helpers (0x42ec00..0x42ec95) have no data, so they might
    be a TU of their own; kept with tr_* for now. tr_startup uses inline-asm zero/copy macros (TR_ZERO,
    TR_COPY, as D3_ZERO / VFS_COPY), so maybe the d3 tool header.
  - unk_430130.c (vfs) starts at 0x42eca0: gz_magic (its first data) sits at 0x5f9004 right after
    "tr_startup", and 0x42eca0/0x42ecf0 test vfs_Unk633408.
  - Not done (outside scope): 0x42ee60..0x430130 (vfs continues: 0x42ee60, 0x42ef10, 0x42f4d0, 0x42f620,
    0x42f7e0, 0x42f8f0, 0x42fa10, 0x42fbf0, 0x42fca0, 0x42fce0); append them to unk_430130.c between
    vfs_CmpNameI and vfs_AddZip.
- Header changes:
  - include/unk_42c3b0.h (own): txt_Unk42e530 renamed txt_LoadTextFile (caller in txt_LoadIncludes
    updated); new txt_* prototypes; vfs_Tfopen/fclose/fread/fseek and vfs_Root declared with `void *`
    handles (to avoid pulling zlib/unzip into the txt TU).
  - New include/unk_42e7b0.h (TBFUNC, tb_* globals; d3_Time 0x6333cc declared volatile as in d3.h) and
    include/unk_42ea90.h (TRREC, tr_*, util_Random*, TR_ZERO/TR_COPY).
  - include/unk_430130.h (the vfs TU's own header): VFSDIR pad000 -> name, pad110 -> parent, new
    `char unk118` (sizeof 0x11c, the size vfs_IGetSubDir allocates); vfs_Unk42ecf0 returns char *;
    vfs_Unk42ed70 replaced by `VFSDIR *vfs_IGetSubDir(char *, VFSDIR *)` (vfs_AddZip casts); added
    vfs_strcmp, vfs_CmpName, vfs_CmpNameI. vfs_Unk42ed10 unchanged (also in unk_402140.h).
  - No shared header touched.
- Outside scope, noticed:
  - unk_402140.h declares `unsigned short util_Random(int n)` (0x42ec00); the real one is
    `int util_Random(unsigned short n)` (rand() % n). Left as is (ch code matches with it).
  - d3_engine.h's d3_Unkfe7a88 (0xfe7a88) is tb_Delay (UINT); d3_Time 0x6333cc is advanced by tb_TimerProc.
- VC6 lessons:
  - `x = f(a, c ? 0 : 1)` gives `sete`; the original's `test; je; push 0; jmp; push 1` into one call is
    `if (c) x = f(a, 0); else x = f(a, 1);` (the two calls are tail-merged up to the differing push).
  - Global counters incremented as `mov eax,[g]; inc eax; mov [g],eax` (not `inc [g]`) are volatile.
  - `return -1.0f * x * x` / `s * x * x` per branch moves the constant to the end (`fmul st,st; fmul c`);
    `s = x < 0 ? -1.0f : 1.0f; return s * x * x;` keeps `fld c; fmul st,st(1); fmulp` in both
    (tail-duplicated) returns.
  - A loop that counts the found slot too (`inc ecx` before the test, `cmp ecx,0x20; jle` after, then
    `lea eax,[ecx+ecx*2-3]`): `while (t[i++].p && i < 32); if (i > 32) error; i--; t[i]...`. Indexing
    with `t[i - 1]` folds the -1 into the displacement instead.
  - `for (i = 0; i < n; i++) { if (*p == c) *p = d; p++; }` gives the countdown `inc edx; dec ecx; jne`;
    the indexed `p[i]` form keeps `cmp eax,ecx; jl`.
  - txt_LoadTextFile: an initialized `int i = 0` that is set to 0 again in the first loop (`for (i = 0;`)
    chose the original register assignment; with `for (; i < n; i++)` relying on the initializer, all three
    registers rotated.
  - A function ending in a call with its own arguments becomes a `jmp` (util_Seed, vfs_strcmp's stricmp,
    vfs_CmpNameI rewriting only the second stack argument).

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "txt_ParseRandomText", "txt_OpenText"
- Boundaries: separated (proven): sfx.c / tb.c tags
- Range then: 0x42c3b0..0x42e770
- Was: unk_42c3b0.c (round 12)


## match-base 2026-10-23 (match worker)

- txt_SkipSpaces and txt_TrimRight: descriptions only (both were byte).
- txt_ParseRandomText: described, then escalated for behaviour. The four stores and the branch structure are
  identical; the original reloads `p` from its home slot (`[esp+0xc]` / `[esp+8]`) in the two store-p paths
  where ours keeps it in esi - register allocation, and the note above records the earlier attempts
  (a local copy, a char ** cast, merged stores, inline strncmp).
- txt_OpenText: `txt_Unk6333bc` renamed `txt_GroupCount` (0x6333bc is written once, `txt_GroupCount =
  ngroups` at the end of txt_OpenText, and read nowhere in the exe; `ngroups` is the #define group counter),
  so the function is readable; behaviour escalated (line/column live ranges, see above).
- txt.c is behaviour draft (the two escalations) and readability readable.

## match-base 2026-10-25 (base-3 worker, shard 3/6, session 4)

Q7 canonicalisation (10 foreign declarations -> 0): every caller includes txt.h now.
- include/d2.h: the `txt_FindKey` line dropped (d2.c already includes txt.h).
- src/game/f3init.c, ls.c, io_text.c, main.c, he.c, unk_575860.c, hst.c: `#include "txt.h"`, the local
  txt_* declarations dropped. hst.c's G12TXTFILE view (a copy of TXTFILE) went away with it (txt.h's
  TXTFILE, same 0x74-byte layout); the "txt.h conflicts with am.h's vfs views" note was stale.
- io_text.c's `ai_Unk42c3b0` is txt_Field (0x42c3b0) now - 22 call sites, declaration from txt.h.
- main.c's `ai_Unk42e2a0` is txt_FreeFiles (0x42e2a0).
No byte changed (unitdiff --against on txt.c and the touched units; check 2569/4664). txt.c: 13 functions,
readability readable (the 2 behaviour drafts stay @deep).

## match-base 2026-10-26 (base-3 worker, shard 3/10)

txt_ParseRandomText: the `uv run spell` dup-tail prescription "swap the branch polarity" (the store-p arm first:
`if (strncmp(p, "{r", 2)) { txt_Random[txt_Count] = (int)p; } else { ... }`) was the one menu entry of
the three not in the tried list; it gives 52 B / 24 lines, WORSE (25 B / 18 lines before), reverted. The
@deep reason records it. txt_OpenText unchanged (1463 asm lines, the i / lineno live-range finding above);
its dead `cmp eax,0x71b` got three more shapes (one-case switch, goto to the next label, while-break), all
identical to the empty `if` (see the 0x71b attempts above); txt.c stays 11/13 byte, readability readable.

## match-base 2026-10-31 (opus3)
- txt_OpenText: escalated again with the evidence above. Two names for 0x114fe60 exist (io_text.c defines
  `int ai_Unk114fe60[18440]`, txt.h declares `int txt_Random[0x4800]` with no definition): typing debt, one object
  should get one definition (owner io_text by the data layout).
