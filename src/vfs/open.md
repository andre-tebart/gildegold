# vfs.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "vfs_Tfopen", "vfs_init", "vfs:" tags
- Boundaries: separated (proven): tr.c tag / d3_anim.c leaked
- Range then: 0x42eca0..0x431510
- Was: unk_430130.c (round 12)

## Open work
- vfs (unk_430130.c), after round 10: vfs_AddFile (one register), vfs_Unk42fa10 (ebx/ebp swap), vfs_FindDir
  (esi vs ebx), vfs_MemOpen, vfs_Tfopen (only 2 tries; try the single-return style) (notes/units/vfs.md).

## vfs3 (unk_430130.c nonmatching functions) [vfs3]

### Report
Scope: the 10 nonmatching functions of src/game/unk_430130.c. No new functions, no TU boundary changes, no
shared-header additions (include/unk_430130.h unchanged).

Matched (5):
- 0x42f4d0 vfs_Unk42f4d0: block-local copy `VFSDIR *cur = d;` inside the if.
- 0x42fbf0 vfs_Unk42fbf0: `VFSDIR *p = d;` copy, `do { if (p->parent == stop) break; p = p->parent; ... } while (p);`.
- 0x430fa0 vfs_fseek: one `return ret;` at the end, every early `return ret;` turned into else branches.
- 0x430bd0 vfs_fread: same (single `return ret;`, ret lives in memory because of the asm copy).
- 0x430e30 vfs_fwrite: same, with a new `int ret = -1;`.

Nonmatching (5), closest state committed:
- 0x42ef10 vfs_AddFile: 1308 -> 740 bytes (31 asm lines; only a 2-byte register difference for the new
  entry pointer e in the shared tail, everything after it is shifted).
- 0x42fa10 vfs_Unk42fa10: 297 -> 34 bytes (only ebx/ebp swapped between out and the loop dir).
- 0x42f7e0 vfs_FindDir: 245 -> 195 bytes (early load fixed, register choice for dir wrong).
- 0x4311d0 vfs_MemOpen: unchanged at 549 bytes (attempts below).
- 0x430470 vfs_Tfopen: unchanged at 1271 bytes (attempts below).

General VC6 lessons (this TU's author style):
- Single exit: vfs_fseek/fread/fwrite matched only after turning every `return x;` inside branches into
  `ret = x;` + if/else nesting with one `return ret;`. The original's per-path epilogues are VC6 copies of
  that one return. Symptoms: stores to a memory `ret` right before an epilogue, `mov eax,edi` (ret register)
  on paths that look like they return a constant, or an `or eax,-1` at entry.
- Pointer parameters copied into a local first (`VFSDIR *p = d;`) give `mov eax,[esp+4]; test eax; push ebx;
  mov ebx,eax` with the null path returning eax without an `xor`; three functions here use it.
- An array initializer `char buf[N] = "";` at function scope is filled at entry; the original fills it at
  the start of the inner block that uses it (declare it there). Two such arrays fill in declaration order.
- Variables reused across unrelated roles change the stack slots: vfs_AddFile's copy loop uses `lo` (the
  binary search bound) as index, and the second VFS_COPY needs a different count variable than the first.
- `while (hi >= lo)` vs `while (lo <= hi)` flips the compare operands; `if (mid >= n) ...; else if (...)`
  gave the original block order; `date > x` vs `x < date` flips `cmp` operands.
- A store of a constant scheduled after a strlen (`len = strlen(...) + 1; d->nentries = 0; mem_Alloc(len +
  ...)`) uses the zeroed eax from the `repnz scasb`; a store before the strlen uses the zero register.
- A loop duplicated in both arms of an if/else in the original is written twice in the source.

Outside the scope: nothing noticed.

### vfs_Unk42f4d0 0x42f4d0 -- matched
- `d` loaded into ebp before the first strcpy: a block-local copy `VFSDIR *cur = d;` at the top of the
  `if` block (function-level local gave a different layout).

### vfs_Unk42fbf0 0x42fbf0 -- matched
- `VFSDIR *p = d; if (!p) return 0;` gives `mov eax,[esp+4]; test; push ebx; mov ebx,eax`, the null path
  returning eax without `xor`. Loop is `do { if (p->parent == stop) break; p = p->parent; ... } while (p);`.

### vfs_fseek / vfs_fread / vfs_fwrite -- matched
- Single `return ret;` at the end (see report).

### vfs_FindDir 0x42f7e0 -- nonmatching (195/272 bytes; ~14 runs)
- Original loads d into esi at entry (before the `!path` test). `VFSDIR *dir = d;` at the top gives the early
  load, but then dir goes to ebx, the inline strcpy temp to esi and edi is saved separately around the
  vfs_Unk42ecf0 call (one extra push/pop pair). With the parameter d used directly, d gets esi but is loaded
  late (19 asm lines, 245 bytes).
- Tried (no change in registers): local declared first/after/in-if, `dir = d` after the null test, comma in the
  if, register keyword, `p + 1` / `++p` copies, while/for/do loop shapes, inner loop as while/break,
  condition order (swapped = worse), `strlen` in a local, `char *s = path` copy, `if (path)` nesting,
  `if (!path) d = 0; else if (*path)` (pushes then at entry but no early load), single-return forms with a
  `ret` local (117-124 bytes but 57 asm lines, worse layout).

### vfs_Unk42fa10 0x42fa10 -- nonmatching (34/480 bytes; ~10 runs)
- The original duplicates the parent loop in both `if (e)` branches: written twice in the source. The
  `d == stop` case is the out-of-line `else`. `VFSDIR *p = d` local, and a returned local `char *r`
  (`if (!p) r = 0; else { r = out; ... } return r;`) keeps all four pushes at entry.
- Left: out in ebx and the loop dir in ebp; the original has them the other way round. Tried: r declared
  before p, using r instead of out everywhere, separate loop variable q (from p or d), `r = out` at the end,
  `r = (char *)p`, `p == 0`, if/else swapped (worse), early `return 0` (pushes deferred), break-style loops
  (`if (p->parent == stop) break;`, canonicalized to the same code).

### vfs_AddFile 0x42ef10 -- nonmatching (740/1472 bytes, 31 asm lines; ~16 runs)
- Fixed: mine/other `= ""` buffers declared inside the collision block (mine first); `if (d->entries)
  {insert} else {first}`; `while (hi >= lo)`; `if (mid >= n) mid = n; else if (cmp > 0) mid++;`;
  `date > e->unk04` / `date > n->date`; `len = strlen(name) + 1; d->nentries = 0; mem_Alloc(len + 24)`;
  `n = mpool_Get(...); d->files = n;` (and prev->next); copy loop indexed by `lo`; second VFS_COPY count in
  `hi` (any variable other than the first count works).
- Left: the new entry e is in eax in ours, ecx in the original (orig `mov ecx,eax` after the first-entry
  mem_Alloc and `add ecx,eax` for entries + off); the shared strcpy(s, name) then uses eax as its source
  pointer. Tried: e as a fresh variable / dst / old, `entries + mid`, `off + (char *)entries`,
  `d->entries + mid`, e computed before d->entries = entries or before mem_Free (worse), duplicating the tail
  into both branches (worse), `e->name = strcpy(s, name)`, `e->name = s` first, tail store order.

### vfs_MemOpen 0x4311d0 -- nonmatching (549/704 bytes; ~7 runs)
- Differences: original hoists `xor eax,eax` (null return) to the entry; read/z stack slots swapped
  (orig read [esp+0x18] stored as immediate, z [esp+0x14] and detect [esp+0x13] from bl); detect block
  orientation (`cmp ebp,10; ja`), and `je ok; jmp fail` after inflateInit2.
- Tried: all 6 declaration orders of z/read/detect (with char/unsigned char read), int z/read,
  `size > 10 && ...` detect form, `z = (cond)` form, `if (!z) {plain} else {compressed}`, single-return
  variants with `VFSFILE *ret = 0` (517 bytes, ret merges with f in esi), `f = 0` + `return f`. Best byte
  count 516 (`read, detect, z` order) but more asm lines, left at base.

### vfs_Tfopen 0x430470 -- nonmatching (1271/1552 bytes; 2 runs)
- Original: name in edi, f = 0 in ebp stored to a home slot [esp+0x24] at entry (frame 0x368, ours 0x364),
  no zero register (all `test reg,reg`); ours keeps 0 in edi and name in ebp. The not-found path reloads f
  from memory after sprintf and returns it; the null-argument path returns f (ebp) through the final
  epilogue; the MakeDirs failure and text-in-zip error return explicit 0. Read and write success tails are
  tail-merged from `f->flags.binary = binary` on.
- Tried: the null-argument test as `if (name && *name && mode && unk) {...} return f;` (no improvement).
  Likely needs the single-return style throughout; not finished.

## Also about this unit
- notes/units/d3_anim.md, section [d3_anim]

## match-base 2026-10-24 (base-0 worker, session 2)

- Readability-only session; no byte changed (unitdiff 28/33, 2789 bytes, as before). Renames (project-wide):
  vfs_Unk633408 -> vfs_IgnoreCase, vfs_Unk42ecf0 -> vfs_UpperCase, vfs_Unk42ed10 -> vfs_ToSlashes,
  vfs_Unk42ed40 -> vfs_ToBackslashes, vfs_Unk42f4d0 -> vfs_AddFilePath, vfs_Unk42f8f0 -> vfs_FindFile,
  vfs_Unk42fa10 -> vfs_EntryPath, vfs_Unk42fbf0 -> vfs_DirPath, vfs_Unk42fce0 -> vfs_AddDir;
  VFSENTRY.unk04/unk0c/unk10 -> date/num/pos (the same fields VFSNODE already names). The renames also hit
  ch.c/ch.h, bio.c, d3_fl.c/d3_fl.h, d3_interface.c. lint_baseline.txt: the two vfs_DirPath entries renamed.
- vfs_CmpName / vfs_CmpNameI / vfs_CmpEntry / vfs_CmpEntryI take typed VFSENTRY parameters now (bsearch/qsort
  casts at the call sites), Q4-clean and byte-identical. Q6 for vfs_fread, vfs_fgetc, vfs_Init, vfs_Exit.
  20 functions readable this session.
- Escalated (all register/slot allocation, reasons in the @deep tags): vfs_EntryPath (ebp/ebx role swap),
  vfs_AddFile (new-entry pointer eax/ecx + 2 fill bytes), vfs_FindDir (dir ebx/esi, buffer slots),
  vfs_Tfopen (frame 0x368 vs 0x364 - one extra 4-byte local crossing the calls - and the stack shift),
  vfs_MemOpen (detect-block z=0 path merged with the if(z) else in the original; slots). The unit's base queue
  is empty. Tried and reverted on vfs_MemOpen: the `size > 10 &&` orientation of the gzip-magic test (same
  layout).
