# Campaign: full exe without the original

Goal: build GildeGold.exe from sources with the real LINK.EXE (no original bytes), get it startable,
then byte-identical. This file is the running state of that campaign.

Branch: `main`. The `data-globals` feature branch (the data-globals + full-link work, 27 commits) was
fast-forwarded into `main` on 2026-10-28 at `1da5bbad`, so `main` is the branch now. The pre-commit hook is
**back ON** (`git config core.hooksPath tools/hooks`): `uv run link` is clean (0 unresolved externals), so the
gate runs again on every commit. It was off for the two data-globals sessions while the byte-matching
regressions were allowed.

## Where we are (2026-10-28, "the hybrid build is removed" session)

**The hybrid exe is gone; the full link is the only link.** `uv run link` links every object with LINK.EXE
into `build/link/GildeGold_full.exe` and exits 1 on any unresolved external; the pre-commit hook runs it.
Deleted: `tools/run_hybrid.ps1`, `gildegold/link/hybrid_bisect.py`, the `--nonmatching` / `--only` / `--skip`
/ `--out` / `--crt` / `--lnk` modes, the `Linker`/`emit`/`.hybrid` placement code, and the hybrid gate. The
data ledger's address evidence is now the read-only `gildegold/ledger/evidence.py` (same rules, same figures:
rdata 45.0% / data 46.6% / bss 0.2% matched, 69.8% overall). The first full-link gate run found two stale
cross-unit names the hybrid resolution had masked (`ai_Unk5d5c40`, `unk_Unk513050`); `rename_stale.py`
renamed them, `uv run build` is 0 REGR, `uv run link` 0 unresolved, `uv run ledger check` 0 errors / 105
warnings, `uv run lint` E 0 / X 0, and `uv run framedump` stays for the `REA_FRAME_DUMP` dumps.

## Where we were (2026-10-28, "the section kinds and the .bss border" session)

**The gate is green and the hook is on again.** `uv run link --nonmatching` went from **12 conflicts on 12
symbols and 36 functions not placed to 0 / 0**; `uv run ledger check` 0 errors, 105 warnings; `uv run build`
green (0 REGR); `uv run lint` E 0 / X 0; `uv run link --full` 92 objects, 0 unresolved; the full exe still
starts and stays alive (timed run: alive after 25 s). Progress 27.1% byte-identical / 2661 matched (unchanged:
this session is data-model work), data 69.8% matched.

### The .bss border was 195 KB too low (a generated stand-in was the evidence)

`find_borders` took the lowest *located* .bss item as the border, and main.c's drafted `full_data_600c00` (a
`tools/full_addrs_gaps.py` stand-in) sat at 0x600c00. So the .data area was reported as `0x5f1000..0x600c00`
(64512 B, a flattering "87.0% matched") and the *rest of the initialized .data* — f3snd.c's 36 KB of tables,
amt.c's rates, the big literal runs of f3gm/unk_58bbd0/sv — was counted as the .bss zero tail ("drafted
89.7%"). Two independent measurements pin the true border: the last nonzero byte of the original's raw .data is
0x630f4b (raw end 0x631000) and the first .bss item with independent evidence is bio.c's `util_Line` at
0x630f50 (a `site` hit from matched code).

The rule now needs **independent evidence** (`rank == "matched"`, no `full_*` stand-in), which is what the D7
rule already says about stand-ins. The per-area summary is honest now:

```
rdata   0x5eb000..0x5f0c7a   23674 B   matched 45.0%  (was 31.8%)
data    0x5f1000..0x630f50  261968 B   matched 46.6%  (was "87%" over 64 KB)
bss     0x630f50..0x6f3300  795568 B   matched  0.2%  (the zero tail: 98.1% of it is claimed zeros)
```

The overall "data 69.8% matched" is byte-weighted over all four areas and does not move, as expected: the same
bytes with the same ranks in a different bucket.

### 41 definitions move to .rdata (the section kind the original has) + 99 `extern const`

The previous session moved three constants (`d3_YAxis`, `ai_Unk5edc58`, `ai_Unk5ebba4`) into .rdata because
their original addresses are there. A scan of every data definition against the original's *area* (`rdata` /
`data` / `bss` / `common`, the ledger's own model) found 41 more: bio.c `bio_CrcTable`; cm.c 8
(`ai_Unk5eb9b8`, `ai_Unk5ebb74`, `ai_Unk5ecebc`, `ai_Unk5edc10`, `ai_Unk5edc28`, `ai_Unk5edc50`,
`ai_Unk5edc60`, `ai_Unk5edc68`); am.c 7; hmt.c 6; gv.c 7; amt.c 3 (`amt_TabA`/`TabB`/`Pay`); d3s.c
`d3s_Dist`/`d3s_Palette`; d3_fl.c `d3fl_Suffix`; d3_sm.c `d3_TriEdge`; vfs.c `vfs_Ext`; ts_texture.c
`d3_ScrollPool`; f3gm.c `ai_Unk5ee160`; ls.c `ls_LanguageNames`; sv.c `ai_Unk5eeab8`. Each definition got
`const`, and **every declaration of it too** (99 `extern const` in 8 headers and 8 units): VC6 refuses
`extern T x;` followed by `const T x = ...` in one TU (C2373), and a non-const declaration of a const object
is a lie in the model.

Grade-neutral (0 REGR) and it fixed **8 of the 12 link conflicts**: the neighbours that inflated the extent
of `amt_Flip`, `ai_Unk60bd94`, `ai_Unk600350`, `ai_Unk5ebc1c`, `ai_Unk623ac0`, `unk_Tab611258`,
`full_data_624584` and `txt_PercentS` were these writable definitions sitting in the same `.data` run. Side
effect measured: `hmt_Unk4ad7a0` 8 asm / 17 B closer (`hmt_Records` const changed its access).

The *cost* is the two order rules below: a const block in .rdata is a plain section item, and the pooled
literal stream is laid out around it.

### The extent of a data item never crosses the next item of the original

Three separate rules had to learn the same fact ("our pad is not our data"): a small definition is followed in
*our* object by the compiler's alignment pad, while the original has the next item's bytes there.

- **`link.py` `check_data`**: the compared span is `min(next symbol of the object, next address the link
  located)`. That removed the four pad conflicts (`txt_PercentS`, `init_SoundVolume`, `ai_Unk6238a4`,
  `ai_CityNameFormat`) and, because a conflicting symbol is kept out of the address map, it also unblocked the
  27 functions `txt_PercentS` was blocking.
- **`ledger_data.py`, a section with several address runs** ("not one block"): the item now ends at
  `min(next symbol of the section, next located address of the section, next address the link located)`.
  `unk_Tab5ee074`/`unk_Tab5ee084` (gv.c .rdata) had overlapped by 4 bytes of pad before.
- **`ledger_data.py` `literal_owner` (D8)**: an address inside *any* unit's plain (non-pooled) section item is
  a global, and "a global may be referenced from any unit" — so it is no evidence about the link order. Before,
  a plain block that failed "not one block" did not place its unit and was invisible, so the extent of the
  previous unit covered it and 25 D8 errors appeared once the const blocks existed.

### D8 in .rdata is a warning, like D7 (the same measurement)

The remaining 13 D8 hits are all addresses in .rdata's pooled-constant stream (0x5ebbc0..0x5ebc08) referenced
from units earlier than the block that precedes them. .rdata is not object-ordered — the campaign measured that
for D7 (ai.c's masks at 0x5eed70 come after am.c's `ag_Pairs` at 0x5ecf18 although am.c's code comes later).
Same rule, same conclusion: an order violation in .rdata is a **warning**; .data keeps the error. With
`d3_TriEdge`/`d3s_Dist`/... now in .rdata there are also 2 D7 .rdata warnings (d3_sm.c before d3_vec.c, cm.c
before ai.c) of the same accepted class.

### The literal at a global's address, four more times

Each of these was a definition whose bytes are a pooled literal of the original; the model is "the literal is
the storage", so the global went and the code passes the literal (the `evt_Path` pattern, in the other
direction):

- **`txt_PercentS`** (gv.c) was a fabricated 24-byte `char[]` covering the five pooled literals "%s", "rt",
  "%s\n", "wb", "." at 0x5f8318..0x5f8330 (the other four belong to txt.c/evt.c). The original's symbol is the
  3-byte "%s", shared by **106 call sites in 4 units** (`txt_Format(x, "%s", ...)`, 5 of them as
  `(char *)(unsigned)&txt_PercentS`); the externs (11) and the definition are gone.
- **`ai_CityNameFormat`** (unk_58bbd0.c): the 4 bytes `"%1G"` at 0x61168c; its one use passes the literal now.
  The 4 bytes at 0x611690 are *another unit's* "Gebäude" literal, which the old 8-byte extent swallowed.
- **`ai_Unk5ed098`** (cm.c): the original's 500.0f at 0x5ed098 is the *merged* `__real@43fa0000` COMDAT
  (`uv run link` shows am.c/hmt.c/ahm.c resolving that literal to 0x5ed098 by site). One constant, four users:
  the global (read by he.c through `*(float *)(unsigned)&ai_Unk5ed098`) became the literal `500.0f`.
- **`unk_Tab611258`** (gv.c): 1080 bytes of initializer whose last 32 are the pooled literal
  `"Hm, das ist doch nicht verboten"` (0x61166c..0x61168c); the table is 1044 bytes and the literal is its
  neighbour, not its tail.
- **`d2_NoBackgroundRestore`** (+1): the original's 4 bytes at 0x5fe71c are `01 02 00 00`; the flag is a 1-byte
  `char` (value 1, the only value the code tests) and nothing in the exe references 0x5fe71d..0x5fe720, so the
  2 is a second 1-byte flag: `char d2_Unk5fe71d = 2;` names it and the compiler pad is gone.

### Checks (this session)

```
uv run enc              0 tracked files not UTF-8 / not LF
uv run build            green, 0 REGR   (debt: hmt_Unk4ad7a0 8 asm/17 B closer; ord_CountGridObjects +6 asm:
                        removing gv.h's extern changes the TU's declaration count, the VC6 schedule quirk)
uv run lint             E 0, X 0 (257 prototypes not checked)
uv run link --full      92 objects, 4615 /INCLUDE, 0 unresolved externals
uv run link             1 conflict on 1 symbol (ai_Unk6238a4), 0 bytes differing
uv run link --nonmatching   0 conflicts on 0 symbols, 0 functions not placed      <- the hook's gate
uv run ledger check     0 errors, 105 warnings   (64 not one block, 16 function order, 13 D8 .rdata,
                        2 crt, 2 D7 .rdata, the 5 known classes, 1 nothing located)
uv run ledger check --exact  0 errors (baseline promoted)
build/scratch/runexe.py build/link/game/GildeGold_full.exe ALIVE after 25 s
progress                27.1% byte / 2661 matched; data 69.8%
```

### What is left

- **`ai_Unk6238a4`**, the one conflict `uv run link` still reports (matched functions only): our 4-byte
  `int = -1` at 0x6238a4 is followed in the original by the "Sky_" literal at 0x6238a8, but that literal is
  only resolved in `--nonmatching` mode (it is read from a drafted function's site), so the bound there is the
  section's next symbol (8 bytes). Same class as the four fixed above, needs either the literal resolved in
  matched mode or the defining site's literal turned into a symbol.
- **The 105 warnings**: 64 "not one block" sections (the per-unit declaration order still differs from the
  address order, and many blocks are incomplete: they hold other units' names or our literals), 16 units with
  functions out of address order, 13 D8 + 2 D7 .rdata (accepted by the measurement above), 205 statics in the
  COMMON area, 37 externs that point inside a defined symbol, 2 crt items, 2 symbols with conflicting evidence
  (the float 1.75f and `full_rdata_5eb7e8` content-match into the CRT's `87disp` data), 1 section with nothing
  located, 115 aliases.
- **The 2 false content matches** (`ai_Unk5ebc1c`, `full_rdata_5eb7e8` → 0x62e470/0x62e46c, the CRT's 87disp
  constants): `find_constant` should prefer the commented site when the byte pattern also occurs in the CRT's
  data. They only cost the two symbols their `located` status (warning).

## Previous: 2026-10-28, "the data debt is paid" session (14 -> 0)

**0 ledger errors (was 14), 95 warnings.** `uv run build` green (0 REGR), `uv run lint` E 0 / X 0,
`uv run link --full` 0 unresolved, `uv run link` 5 conflicts on 5 symbols (unchanged), and the full exe
still starts and stays alive (timed run: alive after 25 s). Progress 27.1% byte-identical / 2661 matched
(unchanged: the whole session is data-model work), data 69.8% matched.

The five error classes of the previous session are all paid, none of them by the per-unit layout sort:

### `d3_Scenes` is the container of five more names (4)

`ai_MapRefs` (+0xac), `ai_Unkfd7c50` (+0xb0, with the aliases `unk_Ptrfd7c50` / `init_SceneHeightMaps`),
`init_SceneDirty` (+0x3d5) and `ai_Unkfd7f78` (+0x3d8) are `data_views` macros of `d3_Scenes`.  The
blocker was the per-unit type; the answer is the **pointer-valued view**:

```
#define ai_MapRefs          (*(void **)((char *)&d3_Scenes + 0xac))
#define ai_Unkfd7c50        (*(char (*)[805])((char *)&d3_Scenes + 0xb0))
#define unk_Ptrfd7c50       (*(char **)((char *)&d3_Scenes + 0xb0))
#define init_SceneHeightMaps (*(int (*)[202])((char *)&d3_Scenes + 0xb0))
#define init_SceneDirty      (*(char (*)[3])((char *)&d3_Scenes + 0x3d5))
#define ai_Unkfd7f78        (*(unsigned char *)((char *)&d3_Scenes + 0x3d8))
```

rd.c reads the *value* (`rd_Road.map = (G15MAP *)ai_MapRefs`, `ai_MapRefs->w`), ai.c wants the record
array (`((AIMAPREF *)&ai_MapRefs)[bld].map`) and sv.c the flat dword stride (`((int *)&ai_MapRefs)
[scene * 0xf8]`); a macro of the record type made rd.c's bare uses load the *address*.  ls.c's
`ai_Unkfd7c50 = d3sm_Create(...)` became `*(int *)ai_Unkfd7c50 = ...`.  rd_BuildRoad got 12 asm closer
(the `&ai_MapRefs` operand shape).

### `ai_Unk91f700` is the container of f3gm's goods table (1)

The cm.c crime-record block is 0x5a00 bytes (0x91f700..0x925100); `ai_Unk91f716` (+0x16, 0x59ea) is a
view.  This is the pair e43d2754 had to revert: as a view of the old **18-byte** definition the storage did
not exist past +0x16 and the first save crashed.  With the container resized to 0x5a00 (its next symbol in
our object is 0x925510, so the extent is exactly 0x5a00) the view is safe.

### `ch_Motions` is 64 records (1)

0x4c * 64 = 0x1300, 0x14696e0..0x146a9e0 (`ch_Path` is its alias at +0).  `ai_Unk1469700` (int) and
`ch_ActDefs` (CHACTDEF[64]) are views at +0x20: the original's code proves it (`ds:0x1469705` = record 0's
name at +0x25, `ds:0x1469700` = its +0x20 dword, which ls.c indexes as `[kind * 0x13]`).  The old
`ai_Unk1469700[1216]` was a *separate* 4864-byte definition in ls.c; the full exe had two copies of the
storage.

### `d3_Cfg`'s fields are views, not a field spelling (2, accepted -> paid)

`d3_SortMax` (+0x18, .maxpolys) and `d3_Unkfe7400` (+0x20, .maxhicol) are macros over `d3_Cfg`.  The
previous session's "the field spelling costs d3_MirDrawObject / d3sm_GetGround's byte" does not apply to
the macro form: 0 REGR, and three drafts got closer (d3fl_DrawWater -4, d3fl_Draw -9, d3sm_ShadowCB -14
asm).

### The two odd-address pairs: one 1-aligned container each (2)

The extent overlap was "a 1-byte flag whose next symbol is 4/8-aligned because our section's offset parity
differs from the original's".  A VC6 object of 8 bytes or more is 8-aligned, so a separate table cannot sit
at +1 or +2 — but a **single 1-aligned container can**:

- f3gm.c: `ALIGN1 char ai_Unk61a686[550] = { 255, 0, <the 548 table bytes> }` (0x61a686..0x61a8ac) with
  `ai_Unk61a688` a `char[548]` view at +2; the four uses of the flag became `ai_Unk61a686[0]`.  gm_Unk503ee0
  got 12 asm closer.
- sv.c: `ALIGN1 char ai_Unk623ba9[277]` with the **original bytes** (the old `int ai_Unk623baa[69] = {0}`
  was all zeros, the original's table is not) and `ai_Unk623baa` an `int[69]` view at +1.  The container is
  the last definition of sv.c's .bss section, so its extent is exactly 277.
- `ALIGN1` is `__declspec(align(1))` in a macro (gen cannot parse a `(` in a declaration), and the *extern*
  must carry it too (`extern ALIGN1 char ai_Unk623ba9[277];` — an object takes its alignment from its first
  declaration).

### The section kinds: three constants move to .rdata (2 D7)

`d3_YAxis` (d3_vec.c), `ai_Unk5edc58` (cm.c, the 1.1 double) and `ai_Unk5ebba4` (cm.c, d3_Origin) are
`const` now: the original keeps them in .rdata, and our .data definitions there were D7 noise.  All three
are read-only and the change is byte-neutral (no REGR, no asm movement).

### `d3_DefaultScene` belongs to d3_engine.c (2 hybrid-link conflicts)

The last open item of the previous session's d3_engine .data block: the pointer at 0x5fb5b0 was f3gm.c's
`ai_Unk5fb5b0`.  It is now `D3SCENE *d3_DefaultScene = d3_Scenes;` in d3_engine.c right before
`d3_DefaultD3DConfig`, and `d3if_Unk5fb5c4` moved next to it (it is at 0x5fb5c4, the config's next
symbol, not in d3_interface.c's object).  This removed the two artificial size conflicts
(`ai_Unk5fb5b0`'s row was 8 = pointer + the config's first dword; the config's row was 20 = 16 + our
padding), i.e. the hybrid link is back to its 5 known conflicts.

### The ledger rules, corrected by measurement (2 D7 + the data-area border)

- **D7 does not place a unit by a generated gap.**  `full_data_*` / `full_rdata_*` are the address-literal
  layer's stand-ins, written into main.c on purpose; their definition site is not ownership evidence.
  Excluding them removes main.c's false D7 error (and is why `ledger_data`'s main.c .data block is no
  longer a 1000-symbol catch-all).
- **.rdata is not object-ordered.**  Measured: ai.c's `ai_BuildingTypeMasks` (0x5eed70) sits after am.c's
  `ag_Pairs`/`ag_Chance` (0x5ecf18) although ai.c's code comes first, and ai_MasterUpdate 0x47e3fb reads
  `ds:0x5eed78`; the read-only area interleaves the pooled-literal stream (`__real@` / `??_C@` COMDATs).
  An order violation there is a **warning**, not an error (.data and .bss keep the error).
- **The .bss border** is the first located .bss block *inside .data*; a zero-initialised definition whose
  original address is in .rdata (a misplaced section kind) must not make the whole .data area "bss".  The
  data summary had been `0x5f1000..0x5ebba4` (**-21596 B**) and the .data undrafted bytes were counted as
  .bss; it now reads `0x5f1000..0x600c00`, 64512 B, 87.0% matched.

### `link.py`: an alias must resolve to a name an object *defines*

Two view conversions left `_ai_Unkfd7c50` / `_ch_ActDefs` weak externals pointing at names no object
defines.  A data.csv row's *name* column can be the stale placeholder while the real definition is in the
alias column (e.g. `0x6333fc, d3_Unk6333fc, ... vfs_Root`), so the alias object now picks the canonical
name from the names an object actually emits (COFF `_name` spelling) and skips a row with no defined name.

### `tools/data_views.py` additions

`VIEW_TYPE` (the shape the *code* needs when the defining declaration is a different one: ai_MapRefs,
unk_Ptrfd7c50, init_SceneHeightMaps, ch_ActDefs, ai_Unk61a688, ai_Unk623baa), `VIEW_ADDR` (a view that
data.csv only lists as an alias, so it has no row), views with no definition at all, and the
`#include "data_views.h"` no longer lands inside a function (d3_mir.c's `#undef nedge` was the last
directive before the first use).

### Checks (this session)

```
uv run build            green, 0 REGR
uv run lint             E 0, X 0 (257 prototypes not checked)
uv run link --full      92 objects, 4615 /INCLUDE, 0 unresolved externals
uv run link             5 conflicts on 5 symbols (the same 5 as before the session)
uv run ledger check     0 errors, 95 warnings
build/scratch/runexe.py build/link/game/GildeGold_full.exe ALIVE after 25 s
progress                27.1% byte / 2661 matched; data 69.8%
uv run link --nonmatching   12 conflicts on 12 symbols, 36 functions not placed  (hook stays off)
```

### What is left

- **The pre-commit hook stays off** (`git config core.hooksPath tools/hooks` is unset): `ledger check` is
  green, but the hook also runs `uv run link --nonmatching`, which still reports 12 conflicts on 12 symbols
  and 36 functions not placed (the data-oracle / gap-ref symbols; unchanged by this session). *Paid in the
  next session (0 conflicts, 0 not placed, hook on).*
- **The 5 hybrid-link conflicts** (d2_NoBackgroundRestore, txt_PercentS, init_SoundVolume, ai_Unk6238a4,
  ai_CityNameFormat): per-TU literals and oversized definitions, as before.
- **95 warnings**: the `not one block` sections (the per-unit declaration order still differs from the
  address order in about 28 sections).  The layout sort is no longer needed for the gate; it is the
  remaining cleanup for the .data/.bss blocks and for the misaligned pad extents.
- The one D7 .rdata warning (am vs ai) is accepted by decision, with the measurement in the rule.

## Previous: 2026-10-28, "the literal is the global" session (20 -> 14)


**14 ledger errors left (10 overlaps + 4 D7), down from 20.** `uv run build` green (0 REGR),
`uv run lint` E 0 / X 0, `uv run link --full` 0 unresolved, `uv run link` 5 conflicts on 5 symbols
(was 6), and the full exe starts and stays alive (timed run: alive after 20 s). Progress 27.1%
byte-identical / 2661 matched (`ai_Unk4e0580` came back as a side effect), data 69.8% matched.

### `evt_Path` (1): the literal at a global's address is the global

`f3init.c`, `sv.c` (2x) and `he.c` each spelled the scripts directory as a string literal
`"x:\\engine\\gfx\\scripts\\"`. The original's operand at those four call sites is `0x5f8330` =
`evt_Path` (the writable array `main_Init` overwrites with `"scripts/"` at 0x5320af; the *same* storage,
which is how the relative path reaches the event loader). Our literal pooled its own copy, and the ledger
content-matched it *onto* `evt_Path` -> one "overlap" per unit that got there first. The fix is the call
sites: pass `evt_Path`. (evt.c's .data is now "not one block" — 0x5f8330 and evt_NextId 0x5f8434 — which
is a warning, not an error: the block is evt.c's whole literal region.)

### `d3_DefaultD3DConfig` (4): the block's 4th dword is another unit's write-only global

The config is 12 bytes (`d3d_SetConfig` reads `[0]`, `[4]`, `[8]`) followed at +0xc by `d3if_Unk5fb5c0`
(0x5fb5c0 = 4) and `d3if_Unk5fb5c4` (0x5fb5c4 = 0xf), the write-only scene pair `d3if_Reset` /
`d3if_SelectScene` save and restore. Our 12-byte definition padded to 16 (d3e_Events is 8-aligned), so the
ledger's 16-byte extent at the config's *site* disagreed with the original's bytes (`+0xc`: ours 0, orig 4)
-> the symbol went into `bad_keys`, the block fell back to d3e_Events' base 0x5fb7f0, and the config's
phantom 16 bytes at 0x5fb7f0 covered the `"%i"` / `"_S"` / `"_1"` / `"_0"` literals (4 overlaps).

Fix: declare the 16 bytes (`{94,1,0,0,3,0,0,0,2,0,0,0,4,0,0,0}`, the last dword is the scene pair) and make
`d3if_Unk5fb5c0` a view of it (+0xc, the `data_views.py` pad class). The byte check now agrees, the symbol
is located at its site, and the section is "not one block" (config 0x5fb5b4, d3e_Events 0x5fb800,
d3e_Resolve 0x5fb920) so the ledger places each by its own site instead of one wrong base.

**The whole stretch is d3_engine.c's .data** (the unit header already said so): 0x5fb5b0 is
`d3_DefaultScene` (a pointer to 0xfd7ba0), 0x5fb5b4 the config, 0x5fb5c0/4 the pair, 0x5fb5c8 is
`"D:\develop\Units\d3\d3_engine.c"` (the assert `__FILE__`) and the unit's other strings follow to
0x5fb7f3. One block only if the literals were part of the section - our build emits each literal as a
COMDAT, so the block can never be one piece. The *owner* of the pointer is `d3_DefaultScene`, which our
`f3gm.c` defines as `ai_Unk5fb5b0` (a duplicate, 102 uses of that name; data.csv has the alias row). Moving
that definition to d3_engine.c and renaming the uses is the next step for this block.

### `d3_ScrollSpeed` (1): a real 4-byte overlap in the original

The original's `d3sm_Pick` does `fcomp QWORD PTR ds:0x5ebc20`: an 8-byte double (a `float 0.0001f`
promoted, d3_sm.c's pool) whose *upper* dword, 0x3f1a36e2, is `d3_ScrollSpeed[0]` (0.6024f) at 0x5ebc24.
That is a genuine overlap of two constants in the original - no C declaration can express it as two
objects. Model: one `char d3_ScrollPool[64]` at 0x5ebc20 with the original bytes, and three views
(`include/data_views.h` cluster `d3_ScrollPool`):

```
d3_SnapEpsilon  (double)        +0x0   <- d3sm_Pick's comparison (was the literal 0.0001f)
d3_ScrollSpeed  (float[15])     +0x4   <- ts_UpdateScroll
d3_AnimDiv      (unsigned int[12]) +0x40
```

The block's extent (to the next symbol) is exactly the 0x40 bytes, so no overlap; d3sm_Pick compares
against the view, so its literal leaves d3_sm.c's pool. Bonus: `d3fl_UpdateWater`'s asm distance improved
by 4 (`debt: 12 asm (-4, CLOSER)`) - the view's operand shape is closer to the original's.

### `tools/data_views.py` fixes (the tool damaged the tree twice this session)

- **`--only` must not rewrite the other clusters' header/include state.** `names` (the set behind
  `NAME_RX`) was the *selection*, so every other unit looked like "used a view before, no longer does":
  all 32 includes were moved and the header lost its cluster comments. Run the full tool (no `--only`)
  for the header; the edit set is still only what changed.
- **a leftover include keeps its position** (`elif had: lines.insert(0, INC)` put it above the unit's own
  headers, which suppresses their `#ifndef DATA_VIEWS_H` declarations - the 9736610d rule). It now goes
  back where it was.
- `off == 0` is allowed (a container's first member, e.g. `d3_SnapEpsilon` at +0).
- `unsigned int` is sized like `int` (`elem_size` strips `unsigned`/`signed`), so `d3_AnimDiv[12]` works.

### What the two odd-address pairs taught (both attempts measured, both reverted)

`ai_Unk61a686` (1 B at 0x61a686) + `ai_Unk61a688` (the table at 0x61a688) and `ai_Unk623ba9` (1 B at
0x623ba9) + `ai_Unk623baa` (276 B at 0x623baa) are extent overlaps, not definition overlaps: the ledger's
extent is "to the next symbol of our object", and **VC6 8-aligns any object of 8 bytes or more**. A 1-byte
flag followed by a big array therefore pads differently in our section than in the original unless the
section's *internal offsets mod 8* agree with the original's addresses.

- Moving `ai_Unk61a688` right after the flag in f3gm.c: no help (0x2ab2 -> 0x2ab8, still +6; the original
  has +2 because its flag sits at 6 mod 8, ours at 2 mod 8).
- Making `ai_Unk623baa` a `char[276]` (1-aligned, so it *can* follow the odd flag) and swapping the two
  declarations: the char block still went to +8 (>= 8 bytes -> 8-aligned) and pulled `ai_Unkfb906c`'s extent
  over `d2_ScreenH` / `d2_ScreenW` - the same two new overlaps the earlier view attempt produced. Reverted.

The real fix is the **per-unit data-layout sort** (declaration order = address order, with the right sizes),
not a per-symbol trick. That is also what the 4 D7 errors and the remaining pad extents need.

### What is left (14)

- **`d3_Scenes` x4** (0xfd7ba0..0xfe73a0 = 64 x 0x3e0 - the struct and the count are already right):
  `ai_MapRefs` (+0xac, `AIMAPREF[]` / `G15MAP *` / `int[1]` per unit), `ai_Unkfd7c50` (+0xb0,
  `char[805]` in cm.c, an assigned `int` in ls.c), `init_SceneDirty` (+0x3d5, the `.dirty` field, indexed
  with an explicit 0x3e0 stride), `ai_Unkfd7f78` (+0x3d8). All four are views of scene 0's fields with a
  0x3e0 stride; the blocker is the per-unit type and the `ai_Unkfd7c50 = ...` store in ls.c (it becomes
  `*(int *)((char *)&ai_Unkfd7c50)`).
- **The two odd-address pairs** (2): see above; needs the layout sort.
- **`ai_Unk91f700`/`ai_Unk91f716`** (1): 0x91f700 is 0x16 bytes in the original (0x91f716 = the next
  block), our 24-byte extent overruns it; the merged single definition (0x5a00 bytes) is the way that keeps
  the storage (the view alone crashed the save, e43d2754).
- **`ch_Motions`/`ai_Unk1469700`** (1): `CHMOTION` is 0x4c B and `ai_Unk1469700[1216]` (4864 B = 64
  records) starts at ch_Motions + 0x20 and ends exactly at `ch_Ground0`; `ls.c` indexes it as
  `ai_Unk1469700[kind * 0x13]`, a view of the record's +0x20 dword. So `ch_Motions` is 64 records, not 8.
- **`d3_Cfg` x2**: still the accepted pair (the field form costs d3_MirDrawObject / d3sm_GetGround's byte).
- **4 D7** (`am.c` .rdata 0x5ecf18, `d3_vec.c` .data 0x5ebb94, `cm.c` .data 0x5edc58, `main.c` .data
  0x5ee01c). Measured this session: `ag_Pairs` at 0x5ecf18 is *matched* by site and `ai.c`'s .rdata at
  0x5eed70 is matched too, so am.c's .rdata really is before ai.c's although its code comes after - the D7
  rule's "object order = code order" does not hold for .rdata here (COMDAT/const ordering). The other three
  are our *constant definitions* in .data whose addresses are in the original's .rdata (`double
  ai_Unk5edc58 = 1.1`, `D3VECTOR d3_YAxis`, the `full_rdata_*` gaps): the section kind is wrong, the
  "definition should be the literal" class.

### Checks (this session)

```
uv run build     green, 0 REGR   (data.csv regenerated; d3fl_UpdateWater 12 asm (-4, CLOSER))
uv run lint      E 0 errors, X 0 conflicts (257 prototypes not checked)
uv run link --full   92 objects, 4615 /INCLUDE, 0 unresolved externals
uv run link           5 conflicts on 5 symbols (was 6: full_data_600c01 and ai_Unk5f1294 are gone,
                      d2_NoBackgroundRestore surfaced: +0x1 at 0x5fe71c, ours 01 orig 01 02)
build/scratch/runexe.py build/link/game/GildeGold_full.exe ALIVE after 20 s (copy the fresh exe first)
uv run ledger check   14 errors, 93 warnings
uv run python -m gildegold.ledger.progress   27.1% byte / 2661 matched, data 69.8%
```

## Previous: 2026-10-28 "data views" session (78 -> 20)

**20 ledger errors left (16 overlaps + 4 D7), down from 78.** The full exe starts again and stays alive
(timed run: alive after 15 s, main window up); `uv run build` green (0 REGR), `uv run lint` E 0 / X 0,
`uv run link --full` 0 unresolved, `uv run link` the same 6 conflicts on 6 symbols.

### `tools/data_views.py`: the mechanical view conversion

`tools/data_views.py` (`--write`, `--only CLUSTER...`) turns a name that sits inside another object's storage
into a macro in `include/data_views.h`, removes its definition and every declaration, and adds the include to
the units that use it. A `CLUSTERS` table names the container and its views; the offset comes from `data.csv`,
the type from the declaration it replaces (an earlier run's macro is parsed back, so re-runs are idempotent).
**56 views in 29 clusters are converted**: `ai_Chars` (5), `ai_PlayerSlots` (9), `gm_PlayerRecords` (1),
`d2_Wins` (3), `d2_Radios` (1), `d2_Views` (1), `ai_Unkf83ec0` (5), `ai_Unkf31160` (2), `ai_Unk925520`,
`ai_Unk92cae0`, `d3d_UsedCard`, `d3_ParEnd`, `d3_Mirror`, `ai_GfxFloor`, `cm_SaveAcks`, `ai_Unk6f2618`,
`d3_StockHead` (3), `d3_AnimHead` (2) and the eleven **pad class** pairs (below).

Four things had to be measured, and each is now encoded in the tool:

1. **`(char *)&BASE`, not `(char *)BASE`**, and the container must be declared as an *array* in a unit that
   declares it as a scalar. `extern unsigned char ai_Chars;` made `(char *)ai_Chars` a byte *load* (all eight
   REGR functions of the first run); `(char *)&ai_Chars` fixes the address, but VC6 still emits
   `mov edx,[eax*4+disp]` instead of the original `shl eax,2` + `[eax+disp]` (measured in
   `build/scratch/t/t5.c`). The array form `extern char ai_Chars[];` gives the original encoding, so 180 such
   declarations were rewritten (he.c, unk_58bbd0.c, unk_575860.c, main.c, sv.c, ls.c, f3gm.c, f3init.c,
   amt.c, cm.c, gv.c - grade-neutral).
2. **VC6's instruction schedule depends on the number of external declarations in the TU.** Removing the five
   view declarations from `d3_engine.h` flipped `d3_MirDrawObject` by 8 bytes (+0x1be); adding five *dummy*
   `extern int` back restores it, four do not (`build/scratch/t/t.c`... `hz5.h`). So a **header keeps its
   declaration of a view**, wrapped in `#ifndef DATA_VIEWS_H`, and the `#include "data_views.h"` goes *after*
   the unit's own headers (a unit that does not include it therefore sees exactly the old declaration set).
   d3_par.c's `#define d3_ParTail d3_ParTail_engine` / `#undef` pair around `d3_engine.h` also needs the
   include to come later.
3. **A view must fit inside its container.** `ai_Unk91f716` (23018 B) overlaps the 24-byte `ai_Unk91f700`
   block by 2 bytes, but its storage does not live inside it: as a macro the bytes past +0x16 did not exist
   and the full exe crashed at 3.5 s in `vfs_fwrite -> gzwrite -> deflate` when `ls_SaveGame` ran
   (`build/crashwatch.py`: 0xc0000005, first at `_read_buf`). It is a definition again (1 overlap back).
   **Views that overrun their base are the runtime risk of this whole approach**: `ai_CharInts` (+4),
   `ai_Unk770d8c` (+0xe4), `ai_Unk7e4274` (+0x40) and `init_SlotCurrency` (+0x54) point past their base,
   which in the original is `gm_PlayerRecords` / `gm_Offers` and in our link is whatever LINK placed there.
4. **The alias object** (`uv run link --full`) must not point a weak external at a name no object defines:
   `d3_StockList` / `d3_AnimList` became macros, so `d3_Unkfc4c40` / `d3_Unkfe75a4` (their aliases) are views
   too, and the two declarations keep their place in `d3_engine.h` with the address as prose (gen then does
   not pair them).

### The pad class (11 pairs)

A byte our object pads to the next symbol's alignment is another unit's zero-initialised flag in the original:
`d3_MipSkip` +1..+3 (`d3d_ZEnabled` / `d3d_balpha` / `d3d_glow`), `ai_GameState` +2 (`sv_MenuOpen`),
`d3_AnimPath` +4, `d3d_Config` +1 (`d3_ShadowMode`), `dd_GBits` +1 (`dd_GLoss`), `dd_BBits` +3,
`d2_ClipTop` +4 (`d2_ShpLightTable` - the two `__asm` lines now read `dword ptr [d2_ClipTop+4]`, and
unk_47d600.c's does too), `ai_Unk6339db` +1, `ai_Unk6f0454` +1, `ai_Unkfa5f04` +4 (`d2_PointerX`). Both sides are zero storage, so the view is behaviour-identical - the `d3_Unk63345d` /
`ai_Unk6f0455` precedent, and the 11 overlaps went away.

### Sizes (three definitions were claiming a neighbour)

`d2_Wins` is `[0x60]`, not `[0x61]`: d2.c's own walk ends at `&d2_Wins[0x60].active` (its note at 0x468a90
says so) and 96*0x3c4 ends exactly at `d2_Radios`. `full_data_62aad0[16]` (the 0x62aae0 literal follows),
`ch_EmptyName[1]` (the 1-byte `""` literal of `d3fl_EmptyName`), `ch_Ground0[65536]` (the 0x40020 gap holds
`ch_Ground1`).

### What is left (20)

- **6 literal-pooling overlaps**: `d3_ScrollSpeed` (inside a pooled `__real@`), `evt_Path` (same address as
  an f3init.c literal), `d3_DefaultD3DConfig` x4 (three float literals inside it). The definition must
  become the literal / a view of it.
- **`ai_Unk61a686`/`ai_Unk61a688`** and **`ai_Unk623ba9`/`ai_Unk623baa`**: 1-byte/4-byte extent vs a 4-aligned
  array at +1 or +2 (the compiler cannot place an int array at an odd address; the view works - it was tried
  for `ai_Unk623baa` and moved `ai_Unkfb906c`'s extent over `d2_ScreenH/W`, so it needs its own analysis).
- **`ai_Unk91f700`/`ai_Unk91f716`**: 2-byte overlap of two real blocks (above).
- **`d3_Scenes` x4**: not converted - the four names inside have different types per unit (`ai_MapRefs` is
  `AIMAPREF[]` in ai.c, `G15MAP *` in rd.c, `int[1]` in sv.c; `ai_Unkfd7c50` is an int/pointer variable, not a
  byte block). **The real find: `sizeof(D3SCENE)` is 0x3e0, not 0x400, and there are 64 scenes**
  (`0xf800 / 0x3e0`); the code scales scenes by 0x3e0. That is a struct-size fix of its own (it changes
  `sizeof(D3SCENE)`, so it needs a clean build + REGR check, and then the four names are its fields).
- **`d3_Cfg` x2** (`d3_SortMax` / `d3_Unkfe7400` are `.maxpolys` / `.maxhicol`): accepted, the field form costs
  `d3_MirDrawObject` / `d3sm_GetGround`'s byte grade.
- **`ch_Motions`/`ai_Unk1469700`**: an int[1216] block that starts inside `CHMOTION[8]` and runs over
  `ch_Queue`; needs the same "does it fit" analysis.
- **4 D7**: `am.c` .rdata at 0x5ecf18 (`ag_Pairs`, a static const table referenced from am.c but sitting
  before ai.c's first .rdata), `d3_vec.c` .data, `cm.c` .data, `main.c` .data - one located symbol far
  outside its unit's block each.

### Session log (what landed)

```
49339777  data views: ai_Chars cluster (5 views), the shared header tool
3c8f7091  the player slots, the player records, the d2 window/radio/view tables (15 views)
c71bd8b6  the action/ref/record tables and the d3 list sentinels (21 views)
9736610d  put the include after the unit's own headers, keep header declarations guarded
a51ef786  the pad class (11 pairs of zero-initialised flags in one block)
6f539823  trim the three oversized definitions (gap, "" literal, ch_Ground0)
e43d2754  ai_Unk91f716 is not a view (it crashed the full exe's first save)
```

Scratch left in `build/`: `view_plan.py` (the clusters with type/offset), `scratch/t/t*.c`
(the VC6 encoding experiments), `scratch/runexe.py` (start, wait, kill - a GUI game needs this).

## Previous: end of the 2026-10-28 "pay the data debt" second pass (78 errors)

**78 ledger errors left (74 overlaps + 4 D7), down from 160.** The exe still starts and shows its window
(" Europa 1400 - Gold Edition - 2.06"); `uv run build` is green on a **clean** rebuild (0 REGR), `uv run lint`
is E 0 / X 0, `uv run link --full` is 0 unresolved, `uv run link` is the same 6 conflicts on 6 symbols.

### The finding: a symbol inside another symbol is a view, not a definition

Almost every remaining overlap was one of three shapes, and the first two are the same fact seen from two
sides: the original's COMMON area is packed, so a name whose address lies *inside* a larger object's storage
is a **view** of that storage (`#define X (*(T (*)[n])((char *)BASE + off))`, the `include/txt_views.h`
pattern), not a second definition. Two independent proofs were used:

- **The addresses fit exactly.** `d2_HitRects` at 0xf8a680 + 0x400 = the next global (a `[256]` table whose
  declared 0x1000 overran 19 globals; the original is `[0x40]`, and `d2_AddHitRect`'s `i > 0x40` guard is the
  original's own one-past overrun). `ai_Chars[768][0x21c]` ends exactly at `gm_PlayerRecords` (0x7d5f80), and
  everything named in between (`ai_CharInts` at +4, `ai_Unk770b89` at +9, ...) is a view of a record field.
  `init_StockBlocks[4]` (the save writer writes four block headers; `[8]` had claimed 22 symbols up to
  `ai_PlayerSlots`), and the four `ai_Unk7d6a9*` names are views into block 0 at +0x20/+0x24/+0x30/+0x38
  (`ai_Unk7d6aa0 + p * 0x2a90` is how f3gm.c walks them).
- **The struct fits exactly.** `d3_Cfg` is the 0x60-byte `D3INITCFG` and ends at 0xfe7440, so `d3_SortMax`
  (0xfe73f8) and `d3_Unkfe7400` (0xfe7400) *are* `.maxpolys` / `.maxhicol` — but the field spelling was
  tried and reverted: it changes the load schedule of `d3_MirDrawObject` and `d3sm_GetGround` and costs
  their byte grade, so the two separate definitions stay and their 2 overlaps are accepted (the field
  macro form is commented in `d3_engine.h`). `d3d_UsedTexFormat`
  (0x50-byte `D3DTEXFMTCTX`, 0xfe79c0) holds `d3_TexFormat8` (.pf, +8), `d3_TexFormat16Bpp` (.bpp2, +0x28),
  `d3_TexFormat16` (.pf2, +0x2c), `d3_Unkfe7a0d` (.same, +0x4d) and `d3d_TexRMask/GMask/BMask`
  (.pf2.dwRBitMask +0x3c / +0x40 / +0x44) — 10 overlaps from one struct.

Counted out of the debt this session: `d2_AnimView` (21; a view of `d2_Anims + 8`, x at +5, not the "x at
0xd" the old note guessed — `d2_MoveObj` 0x46cfac writes 0xf8a52d with a 0x11 stride), `d2_HitRects` (19),
`init_StockBlocks` (20), the `d3_TexFormat*` block (8), `ai_Unk61ac98` (10; the 352-byte initializer was the
unit's string pool, the symbol is the 6-byte `"%1G$A"` literal), the three pooled floats
`ai_Unk5eb798`/`ai_Unk5eb7ac`/`ai_Unk5eb8b0` (6; the `double` was two adjacent 4-byte literals, and the
`*(float *)(&x + 4)` sites are just `0.25f`), and the singletons below.

### Owner moves (the block layout tells which unit defines a byte)

`d3_NoTextures` sat in ecmd.c but its address (0x63345d, an *unaligned dword* right after `d3_D3DActive` at
0x63345c) is in d3_engine.c's .bss block: ecmd.c now only sets it. The same move took `ai_Unk6334a5` (after
`d3_FogOn`, f3init/cm use it), `ai_Unk633751` (after `d3fl_DetailShift`) and `ai_Unk6f0455` (after
`ai_Unk6f0454`) into the unit that owns the block. The D7 error "d3_snd.c's .bss ... ecmd.c's" went away with
the first move. When VC6's size alignment prevents the original's odd offset (a 2-byte object is 2-aligned, so
`ai_Unk6f0455[2]` lands at +2, the original packs it at +1), the second name becomes a view of the first byte
(`d3_Unk63345d`, `ai_Unk6f0455`). **`__declspec(align(1))` on an initialized `.bss` int is silently
ignored** (measured on `d3_NoTextures`: still at +4), so the view is the way, not alignment.

**Declaration order is section order.** `d3_snd.c`'s `.data` listed `d3sndw_Names` before `d3sndw_Range`;
sorted by address it is `d3snd_Max`, `d3sndw_Range`, `d3sndw_Vol`, `d3sndw_Names`, whose sizes give exactly
the original's address deltas — the section is one block now.

### What is left (78) — the same classes, smaller

- **The big COMMON view clusters** (~45): `ai_Chars`/`ai_CharInts` (+4/+9/+0x16c/+0x188/+0x20c) and
  `gm_PlayerRecordBytes` (+2 of `gm_PlayerRecords`), `ai_PlayerSlots` (`ai_Unk7e3f80` +0x40 ... ,
  `ai_Unk7e4274` +0x334, `init_SlotCurrency` +0x348), `d2_Wins` (`[0x60]`, `ai_Unk8f128` +8, `ai_Unkf8f13c`
  +0x1c, `ai_Unk8f39c` +0x27c) and `d2_Radios`/`d2_RadioSel` (+8), `d2_Views` (+0x10), `ai_Unk83ec0`,
  `d3_Scenes`, `d3_StockHead`/`d3_AnimHead`, `ch_Motions`/`ai_Unk1469700`. They are all the
  `#define name (*(T (*)[n])((char *)BASE + off))` conversion, but the names are used in 5-13 units and
  declared `extern unsigned char NAME;` in per-function blocks (193 externs for `ai_Chars` alone), so the
  conversion needs one shared header and the extern blocks stripped — a mechanical tool
  (`tools/data_views.py`, report/dry-run first, like `mk_txt_views.py`) is the next step.
- **The misplaced `.bss` -> COMMON pass** (the "210 statics sit in the COMMON area" warning): `d3_AnimPath`,
  `d3_Unkfc2580`, `d3_Frustum`, `d3_TexFormat8Bpp`'s old storage, ... are `= 0` in our source but the original
  has them tentative (`T x;`) in the COMMON area. Converting them removes the last huge `not one block`
  sections (d3_engine .bss, dd_vesa .bss, d2 .bss, ch .bss) and the pad overlaps that come from a misplaced
  symbol being counted into a unit's block.
- **The pad class** (d3_MipSkip +1..+7 are the d3_d3d flags; d2_ClipTop +4 is d2_ShpLightTable;
  dd_GBits/BBits; ai_GameState +2 is sv_MenuOpen; ch_Motions +0x20 is ai_Unk1469700): the extent is "the next
  symbol of our own section", so the fix is the per-unit block layout (order + missing fillers), not a size.
  `build/block_order.py` prints, per `not one block` section, which adjacent pair has an address delta the
  declaration cannot produce (missing symbol vs wrong size).
- **The 4 D7 errors**: `am.c` .rdata (`ag_Pairs` 0x5ecf18 is before ai.c's 0x5eed70 although am.c comes
  after), `d3_vec.c` .data, `cm.c` .data, `main.c` .data. Each is one located symbol far outside its unit's
  block (an owner guess or a wrong address), not a unit-order problem.

### Gotcha (cost 20 minutes this session)

After a compile group fails, the objects that did compile keep no `.flags` stamp, and a later
`uv run build` compares stale objects: it printed phantom `REGR` lines for `d3_MirDrawObject` /
`d3sm_GetGround` and then `ord_BuildingCapacityRatio`, all of which vanished on `rm -rf build/obj`. Always
clean-build after a failed group before trusting REGR.

Scratch analysis tools left in `build/` (not committed): `debt_report.py` (overlap containers with unit,
section kind and data.csv size), `debt_layout.py NAME...` (the section/symbol neighbourhood behind an item),
`common_layout.py` / `common_runs.py` (the COMMON area per unit, declared size vs the address gap),
`block_order.py` (per `not one block` section: is it only order, or do the sizes disagree, and where).

## Previous: end of the 2026-10-28 "pay the data debt" session (160 errors)

**The data debt went from 1330 to 160 errors and the full exe still starts** (window " Europa 1400 - Gold
Edition - 2.06"). The three tools below are the debt payers and are re-runnable; every step is grade-neutral
(the hybrid link's bytes do not move) and the full link stays clean.

```
uv run link --full      92 objects, 4615 /INCLUDE, 0 unresolved externals
uv run build            green (VC6), 0 errors
uv run lint             E 0 errors, X 0 cross-TU conflicts (258 prototypes not checked)
uv run link              6 conflicts on 6 symbols, 0 not placed
uv run ledger check     160 errors, 94 warnings    (155 overlaps + 5 D7; no grade losses)
```

**The exe starts** from `build/link/game/GildeGold_full.exe` after `cp build/link/GildeGold_full.exe` there
(the full link writes to `build/link/`, not to the game directory): the process stays alive, the main window
is up and titled.

### What paid the debt

1. **The gap block is packed (`tools/gap_fix.py --resize`).** VC6 aligns a data object of 8 bytes or more to 8
   and every object to 4, so the generated `full_<sec>_<addr>` block had padding between its definitions, and
   the ledger (which reads an item's extent as "up to the next symbol of the object") counted the pad as part
   of the item in front of it.  Two things were needed:
   - `GAPDATA` (`#define GAPDATA __declspec(align(1)) unsigned char`) on every gap, so the block packs
     contiguously; the sizes are the exact distance to the next known address (no `max(4, ...)` clamp -- a
     one-byte gap at an odd address must stay one byte).
   - the same alignment on the *extern* declarations: **VC6 takes an object's alignment from its first
     declaration**, so `extern unsigned char full_x[];` before the definition forbids `align(1)` on it.  The
     tool rewrites all 1326 externs to `extern GAPDATA ...`.
   `gen` cannot parse a `(` in a declaration (it takes the statement for a prototype), hence the macro.
2. **Gaps that repeat a pooled literal became the literal (`tools/gap_fix.py --write`).**  The ledger pairs a
   gap with the literal item that owns the same bytes; the reference sites (`(unsigned)&full_x`, `(unsigned)
   full_x`, `&full_x`, plain `full_x`) become `(unsigned)"text"` (octal escapes for the umlauts; no source
   file carries a non-ASCII byte) or a float/double literal read from the original bytes.  54 definitions
   dropped, 16 `.rdata` gaps inlined as float constants; a gap that ends inside another item is trimmed to
   that item's start (27).
3. **The oversized named definitions are trimmed (`tools/data_trim.py`).**  The data passes sized open arrays
   and tables from the address gap, so a definition claimed the next unit's block or a pooled literal:
   `d2_Wins[0x61]` -> `[0x60]`, `ai_Unk625820[256][0x20]` -> `[115][0x20]`, `ai_WeaponList[16]` -> `[10]`,
   `amt_Pay`, `d3_AnimDir`, `ai_Unk5ee160`, `ai_Unk60e7c0`, `ch_PathBuf[0x1000]` -> `[1024]`, 23 in all.
   The tool finds the declaration statement (a brace/comment/string aware scanner), rewrites the first
   dimension, cuts the initializer list (or the string) to the new count, and relaxes the header's
   `extern T x[N]` to `extern T x[]` (a fixed size in a header conflicts with the definition: C2369).
   Guard rails: the span must parse as a single declaration of that name, the initializer list must be a
   whole number of elements (a boundary inside an element is rounded down), and the statement is rebuilt as
   `text[:start] + new + text[semicolon:]` (an early version dropped the prefix and truncated five files).

### What is left of the data debt (160 errors, 155 overlaps + 5 D7)

- **The pad/field-view class (~60).**  Two or three symbols of *another* unit sit inside an item's extent.
  Two causes, both visible in the class list:
  - our object's data layout has padding where the original's had another unit's symbol
    (`d3_MipSkip` +1 is `d3d_ZEnabled`; `d3_MipSkip` is already `unsigned char`, the pad after it is the
    next ts_texture.c symbol's 8-alignment).  The ledger reads the extent as "next symbol of our object",
    so it includes the pad.  Packing every definition with `align(1)` was tried (`tools/data_pack.py`) and
    reverted: it makes the whole section share one (wrong) base, because the original has padding too
    (175 -> 183 errors and `gm_BuildKindObjects` lost bytes).  The real fix is the per-symbol size/type and
    the declaration order inside each unit.
  - a symbol genuinely sits at a field offset (`init_StockBlocks[8]` -> the block count is 5 (0xD4D0); the
    `ai_Unk7d6a90` views are fields of block 0, `d2_AnimView` is `d2_Anims` seen as `D2SND` with `x` at
    0xd not 5 -- its 0x11-byte stride is identical, so the fix is the struct offset, then the base agrees).
- **`ai_CityNameFormat`** paid: its 224-byte initializer was this unit's neighbouring string pool, so the
  definition is the four bytes `"%1G"` again (16 overlaps gone).  The same shape is left in
  `d3_DefaultD3DConfig`, `evt_Path`, `ai_Unk5eb7ac` (the original pools one copy per object; our one
  definition is placed at one of several sites).
- **The 5 D7 order errors** (`am.c` .rdata, `d3_vec.c` .data, `cm.c` .data, `main.c` .data, `d3_snd.c`
  .bss): the definitions are in target order but our object's *declaration* order puts another unit's data
  in between; sorting each unit's file-scope definitions by address is the mechanical fix.
- `uv run link`'s 6 conflicts are the same per-TU-literal / oversized-definition symbols
  (`txt_PercentS`, `ai_CityNameFormat`, `full_data_600c01`, `ai_Unk6238a4`, `init_SoundVolume`,
  `ai_Unk5f1294`).

## Previous: end of the 2026-10-28 "restore the grades" session

**105 of the 106 accepted grade regressions were restored; the one left (`ai_MasterUpdate`) is accepted; the
full link and the hybrid link are still clean.** The gate was red only on the remaining data debt, with a
written reason for the accepted grade loss (below).

### How the 102 came back

1. **The signed loop bound (the bulk, ~40 functions).** `build/bounds_fix.py` rewrote 256 walks as
   `(int)p < (unsigned)START + delta`; the unsigned RHS turned the comparison into `jb`, the original has
   `jl`. The faithful form is `(int)p < (int)((unsigned)&START + total)` (delta folded, `&` added so a
   scalar start is not read as a value). Fixed site by site in the functions whose byte grade had fallen;
   `(unsigned)&X + k` still yields `jb` and must not be changed blanket-wise.
2. **A definition in the defining TU changes that TU's code (the `and 0xffff` class).** `ai_PlayerId`,
   `d3d_alphatest`, `ai_OwnPlayer` were *initialised* definitions (`= 0`) in a unit whose code reads them:
   VC6 then loads a dword and masks (`mov eax,[x]; and eax,0xffff` / `and eax,0xff`) where the original's
   unit saw only an `extern` and zero-extends (`xor eax,eax; mov ax,[x]`). Making the definition
   *tentative* (`T x;`, COMMON) restores the original load and the byte match. None of the three is a
   weak-external alias target, so COMMON is safe for the full link; `ai_PlayerId`'s 225 `U_W(&x)` sites in
   unk_58bbd0.c needed exactly this.
3. **The readability losses** were the two unnamed `gm_Unk50c550` / `gm_Unk5085f0` calls; renamed
   project-wide to `gm_Liking` / `gm_PlayerFactor` (148 references), and `full_data_624588` to
   `main_GfxPath` (the "GfxPath" INI key main_Init passes as the vfs root). 11 readability grades back.
4. **Declaration-level byte losses** (all restored to byte + readable):
   - `d3_SetFogSet`: this unit declares `d3_SetFog(float, float, D3FOGCOL)` and passes the colour struct
     (`@original` on the header line); the `*(unsigned long *)&col` raw access is gone.
   - `d3_LoadScene`: `d3_int_io.c` declares the dword form `d3_SetFog(unsigned long, unsigned long,
     unsigned long)` and passes the raw dword bits (no `int -> float` conversion).
   - `d3if_SelectScene`: the definition returns `char` and takes a `char keep` (`@original`); the headers,
     `ls.c`, `f3init.c`, `main.c` and `unk_58bbd0.c` carry the same declaration again.
5. **`he_Unk53eba0` / `he_RemoveGfxInfos` / `he_Unk53ec10` / `main_Unk537c70` / `ai_Unk5a84d0` / the
   `ai_Unk51f080` family**: the same bound fix at their own sites (`&he_RunHandlers + 0x6a0`, the
   `ai_Unk61a348` walk, `if ((int)slot > (int)&ai_MissionSeed)`, `(int)&ai_Unk61a348 + 0x309`).

### The 4 grade losses restored this session + the one accepted

The four that came back (2026-10-28 "restore the grades" session; each is the *address-literal -> symbol*
transformation changing VC6's codegen, so the fix is the literal form the original used):

- **`ls_ReadPlayerRecords`** (byte): the loop bound is the image literal `0x7d6a42` again (`(int)p < 0x7d6a42`).
  The symbol-relative `(unsigned)&ai_Unk7d6a40 + 0x2` turns the compare unsigned (`jb`); the literal keeps
  `jl`. Q3 flags the literal (the documented blocker: no symbol form keeps the signed compare).
- **`sv_Unk530100`** (behaviour): back to `SV_U(0x623ec4) <= SV_U(0x6333cc) - 0x23a`; the relocatable
  `&X` base compiles `add ecx, -0x23a` where the literal compiles `sub ecx, 0x23a`. Encoding only.
- **`ai_Unk490c10`** (byte): `act.decide = ai_SeanceDecide; act.exec = ai_SeanceExec;` again - two ICF-folded
  `return 0` stub names (both 0x52a040) so VC6 stores two immediates instead of sharing one register load.
  The two per-site names are declared in am.h and resolved by `FUNC_ALIASES` in link.py (weak externals), so
  the full link stays 0 unresolved.
- **`ai_Unk580a50`** (byte): back to the literal bases `0x6f9f60` / `0x6f9f68` / `0x6f9f6c` / `0x6f9f70` /
  `0x6f9f74`; VC6 folds `base + i*0x38 + j*0x10` into one immediate with negative offsets.

The one accepted (still draft, deep matching work, not a data fix):

- **`ai_MasterUpdate`** (byte -> draft, 68 asm): a register-role difference (ebp/ebx swapped over the whole
  body) plus a missing `mov ebp, ds:X`; the campaign's earlier "ai_ActionFlags owner" hypothesis does not
  hold (the tentative definition is already COMMON and changing it changes nothing). Reverting its loop bound
  to the literal `0x7d5f82` changes only -2 asm and makes `ai_CalcMeisterWache` worse, so the bound is not
  the cause. Accepted with `uv run ledger grade accept`; the baseline records it at draft.

## Previous: end of the 2026-10-27 "make it run" session

**The full exe starts and reaches the main menu.** `build/link/game/GildeGold_full.exe` opens the window
(" Europa 1400 - Gold Edition - 2.06"), runs `main_Init` / `main_Unk531c50` / `main_Unk532160`, loads the
gamedata tables, builds the menu (all nine buttons with their real texts, the background scene) and stays
alive and responding. The title is no longer empty (the version buffer is the txt_Ptrs block now).

What this session fixed (each was a data-model assumption the full rebuild broke, invisible in the hybrid
link):

1. **Array-bound literals turned into detached gap symbols.** `ai_Unk5a8390` (the first crash) walked a
   settings list to `full_data_91f6e0`, a gap placed far away, and wrote -1 over the data section. The bounds
   of `ai_Unk9300a0`, `ai_Unk925520`, `he_QuickList`, `gv_Effects`, `ai_Unk91b6e0` and the 0x6fa260 table
   are now relative to their array.
2. **`d3_TopFirst` / `d3_TopLast` are views, not storage**: 0xfc52f0 is `d3_TopHead.next` and 0xfc28f4 is
   `d3_ObjEnd.prev` (the header now has the two macros). As separate globals `d3_LinkTop` / `d3_UnlinkTop`
   updated only the sentinels and `d3if_Reset` disposed the same object twice.
3. **The text pointer table.** The original's 0x143a1c0 is one `char *txt_Ptrs[0x4800]` block; the 374 data
   symbols in [0x143a1c0, 0x144c1c0) (d2_Strings, ai_TextLoadGame, ai_BuildingNames, ai_DifficultyTexts,
   ai_MessageFilterTexts, every `full_data_*` gap, ...) are views of its slots. As separate definitions the
   full rebuild let `txt_LoadTextFile` fill one block and the menu read another (NULL → `d2_TextWidth(NULL)`).
   `include/txt_views.h` (generated by `build/mk_txt_views.py`) is one macro per view, `txt_Ptrs + offset`;
   the definitions and extern declarations were removed. The references still resolve to the original
   addresses, so the hybrid link stays byte-identical (0 REGR).
4. **Inter-symbol loop bounds.** 256 more walks were bounded by *another global's address*
   (`p < &gm_PlayerRecords`, `q < &ai_Unk925114`, `s < &ai_PlayerPrototypeCount`, ...). In the original the
   globals are contiguous; here each lives in its owner object. `build/bounds_fix.py` rewrote them to
   `(unsigned)START + delta` with delta from `data.csv`, so the walks keep the original extent.
   `unk_Unk503c60` was writing -1 through `ai_Unk919f80` this way (the gamedata price table then indexed
   `ai_BTypes` with 0xff and faulted in `gm_ItemPrice`).
5. **`main_Init`'s vfs root** was the *contents* of the path buffer as a string literal
   (`vfs_Init("\\project\\gfx\\", 0)`), not the buffer the options loader fills. The vfs therefore had no
   packed resources: every `textbin_german\*.res` open failed, `txt_LoadIncludes` returned 0 and no text
   pointer was ever set. It now passes `&full_data_624588` (the GfxPath buffer), like the original 0x531ba0.

Debugging tools left in `build/` (scratch, not committed): `crashwatch.py` (WaitForDebugEvent loop, stack
dumps, `CRASHWATCH_DUMP=addr:size,%reg:size`), `watchwrite.py` (a page-protection write watcher; WOW64
hardware watchpoints are silently ignored, guard-page faults are not).

```
uv run link --full      92 objects, 4615 /INCLUDE, 0 unresolved externals
uv run build            green (VC6), 0 errors, 0 REGR
uv run lint             E 0 errors, X 0 cross-TU conflicts (258 prototypes not checked),
                        C 4243 calls through K&R declarations, 1470 implicit
uv run ledger check     1437 errors, 94 warnings   (data debt: D7 x4 + the gap overlaps)
progress                26.6% byte-identical, 2614 functions matched, 2050 drafted; data 69.8% matched
uv run link --nonmatching  48 conflicts on 48 symbols, 78 functions not placed
```

The 16 non-`full_*` conflicts (txt_PercentS, unk_Tab611258, ai_Unk61a678, ...) are the same data-oracle
byte losses as before. The other 32 are **gap definitions whose initializer is shorter than the checked
span** (`full_data_6249e8` holds "grou" where the original has "groups/", `full_data_624584` ...):
`tools/full_addrs_gaps.py` sized them to the next known address *at the time the source was generated*, and
the data.csv row/byte check now sees the rest of the original string. Regenerate the gaps with the current
data.csv (next step) or size them to the next address *end* as the Debt section says.

## Historical: end of the address-literal session (2026-10-27, earlier)

**The whole source layer now references data by symbol, not by the original's address.** `tools/full_addrs.py`
replaced every hardcoded original address in the code of the 21 units that use them:

```
exact data.csv symbol        U_I(0x6f2588)          ->  U_I((unsigned)&main_Window)
inside a symbol (view)       U_I(0x62d7bc)          ->  U_I(((unsigned)&ai_ScreenModes + 0x4) + i * 8)
function address             (WNDPROC)0x531780      ->  (WNDPROC)(unsigned)&main_Unk531780
a literal in no symbol       (char *)0x60da10       ->  (char *)(unsigned)&full_data_60da10
```

A reference is a relocation, so the **hybrid link still produces the original's bytes** (the ledger masks the
relocated dword and `uv run link` checks that it resolves to the original address): 0 REGR, only 3 functions
moved (all to their byte size) after the two multi-line bodies were rebuilt. 7214 literal occurrences in code
(1766 distinct data addresses, 75 function addresses) became references.

The address gaps (addresses the sources never named, 1041 of them) are new tentative definitions in
`src/game/main.c` (`full_data_<addr>` / `full_rdata_<addr>`, initialised from the original's bytes, sized to the
next known address) generated by `tools/full_addrs_gaps.py`, with an `extern` declaration at the top of every
function that uses one.

**How far the full exe gets now.** `build/link/GildeGold_full.exe` loads and runs through:

1. `main_Unk53ccf0` (game.ini, options, command line) and `main_Unk5324f0` (RegisterClass "Die Gilde",
   CreateWindowEx, ShowWindow, SetFocus) -- the window is created;
2. `main_Init` (err/mem/tr/directories/vfs/table init);
3. `main_Unk531c50`: `ai_LoadOptions`, `dd_CheckModes`, the screen-mode pick, the window fit, `d3_Init`
   (D3D device, `d3_FindTextureFormats`, `d3_SetupFrustum`, `d3_SetViewport`, `d3_UpdateAll`), `di_Init`,
   `tex_SetPalette`, the path setup, `d2_Open`, `sv_PumpMessages`;
4. the game loop's first iterations and the gamedata table load (`gm_LoadTables`).

It then crashes in `ai_Unk5a8390` (`src/game/unk_58bbd0.c`, the production window's first helper) -- the next
data-model bug, not an address-literal one.  The window title is still empty because our `crt_Sprintf(title,
" %s", buf)` gets the right format string now, but `main_Unk531a00` writes the version into a 4-byte gap
symbol (`full_data_75e760`) instead of a real 0x30-byte buffer.

The exe carries the same compatibility shim as the installed game / `hybrid.exe`:
`build/link/game/GildeGold_full.exe` -> `DWM8And16BitMitigation HIGHDPIAWARE` in HKCU
`AppCompatFlags\Layers`. The PCA dialog seen during `uv run link --full` is about **RC.EXE** (the VC6
resource compiler), not the game; RC still runs and writes the 1204-byte `.res`.

The gate is **red on purpose** (the hook is off) while the accepted regressions are paid:

```
uv run ledger check     1458 errors, 94 warnings
                        1041 of the errors are data overlaps of the generated full_<sec>_<addr> gap
                        definitions with named views the address gap did not know (see "Debt" below)
                          4 D7 (block order: owner guesses)
                          grade: none (0 REGR; the transformation is grade-neutral)
uv run link --nonmatching   16 conflicts on 16 data symbols, 26 functions not placed
```


- **Stale cross-unit names: 849 -> 0** (done).  **Intra-TU E errors: 22 -> 0** (done).
  **Cross-TU X conflicts: 291 -> 0** (done; 456 -> 0 symbol/file pairs).
- The two rename grade regressions are gone (`ai_Unk49ed10` byte-matched, `ai_Unk498c70` behaviour).

### How the 182 "other" closed (this session)

- **40 stale function names.** `link --full` now reads the address comment of a top-level *prototype* (a
  small brace/comment/string lexer in `declared_addresses`), so `cfg_Open -> reg_OpenKey`,
  `snd_Play -> sb_Play`, `d3_SelectScene -> d3if_SelectScene`, ... went to `rename_stale.py`.
- **118 data aliases.** `data.csv`'s alias column is turned into a COFF object of weak externals
  (storage class 105, `IMAGE_WEAK_EXTERN_SEARCH_ALIAS`) by `write_alias_object`; LINK resolves each typed
  view to the one definition, so the sources keep their names and no duplicate data is emitted.
- **18 CRT refs.** `uv run link --crt` names each one (`_fopen`, `_strstr`, `_sprintf`, `__ftol`,
  `__strupr`, `__strcmpi`, `__itoa`, `_fread`, `_fwrite`, `_fseek`, `_ftell`, `_fgets`, `_atoi`,
  `_strchr`, `_fclose`, `_exit`); `CRT_ALIASES` adds them to the same weak-external object. The cdecl
  `AIL_get_timer_highest_delay` maps to the stdcall import `@0` the same way.
- **`__alloca_probe`** (`ai_Unk5de9c0`): the decompiled duplicate call was removed (the compiler emits the
  probe itself); this had been the note's "duplicated __chkstk".
- **`MSS_auto_cleanup`** (mss32 static, not an import): a stub in `build/link/full/stubs.c` returning 0
  until the static mss32 lib is available. **`AIL_get_timer_highest_delay`** is a real mss32 import.
- **`ai_Unk7662c0`** (sv.c): the declaration now carries `/* 0x7662c0 */`; the data tool defined it as a
  COMMON dword.

### Entry + resources

- `build/link/full/entry.c`: `int WINAPI WinMain(...) { return main_Unk53ccf0(...); }`. The PE entry is
  `WinMainCRTStartup`; `/ENTRY:main_Unk53ccf0` pulls crt0's `_main` (unresolved), so the wrapper keeps the
  CRT startup and calls the game's WinMain. The original's PE entry is 0x53ccf0 directly.
- `tools/make_icon.py` extracts RT_GROUP_ICON 101 + its two RT_ICON images into a .ico; RC.EXE compiles
  `icon.rc`; the .res is linked.

### The address-literal session (2026-10-27, second session)

**What it did.** Replaced the original's absolute addresses in the *code* with symbol references, so the
full rebuild's own data layout is what the program reads.

- `tools/full_addrs.py --write [FILE...]` rewrites each `0x<image address>` literal that sits in code (comments
  and string literals are masked) into an expression:
  - `data.csv` symbol (exact): `(unsigned)&NAME`;
  - inside a *defined* symbol's extent (the views the sources never named): `((unsigned)&NAME + off)`;
  - a `names.csv` function: `(unsigned)&FUNC`;
  - nothing at all (a gap): `(unsigned)&full_<sec>_<addr>`, collected for step 2.
  `data.csv` rows whose symbol no object defines (`kind=extern`, stale after a rebuild) are skipped — only
  *defined* symbols count, otherwise the tool re-invents symbols that no longer exist.
- the gap symbols are defined by `tools/full_addrs_gaps.py`: one `unsigned char full_<sec>_<addr>[N]` per gap
  in `src/game/main.c`, initialised with the original's bytes (zero gaps as `= {0}`), `N` = the distance to
  the next known data address. It reads `build/link/full/unresolved-other.txt`, so the loop is
  `link --full` -> `full_addrs_gaps.py` -> `build` -> `link --full`.
- every name a unit did not declare got an `extern unsigned char NAME;` (or a typed/prototype line) in a
  block at the top of the unit.  `tools/full_addrs_fixdecl.py` repairs the block from the compiler's C2040 /
  C2371 / C2374 / C2373 (drop the line, the header owns the type) and C2065 (hoist/forward-declare).

**The linker is the oracle for `REGR`.** The transformation is grade-neutral: 0 REGR, 0 lint E/X.  A
function's compiled bytes only change where the replacement forced a different *type* (e.g. an `int` view of
a `char` variable), which is exactly what the ledger's byte check flags — none are left.

**Four real bugs fixed on the way (each invisible in the hybrid build):**

- **a weak external cannot alias a COMMON symbol.** LINK resolves it to `IMAGE_BASE + 4` (verified with a
  three-object experiment: a `real_y` alias resolves, a `common_x` alias gives `0x400004`).  Every alias
  target data.csv records as `common` (57) is now a real `.bss` definition (`= {0}`).  This alone took the
  exe past `ai_LoadOptions` (it used to write the options through `0x400004`).
- **a definition smaller than the view it carries.** `d3_Planes` was `[4]` but `d3_SetupFrustum` writes
  `[5]` (the original's 4th/5th planes sit in the 32 bytes before the next symbol): the spill killed
  `d3_TopFirst` and `d3_UpdateAll` then walked `0xbf800000`.  Now `[6]`.
- **`d3d_UsedTexFormat`** (a 0x50-byte `D3DTEXFMTCTX`) was defined as the 4-byte `d3_TexFormat8Bpp`; the
  texture-format enumeration overwrote `vfs_Root` with "DXT1".  The symbol is now the 0x50-byte block, its
  first dword read as `.bpp`, and it is a real definition (alias target).
- **`d3t_SunRays`** was `static` and is reached by address from main.c: made external.

**Crashes are now data-model bugs, not address bugs.** The debugger helper used here is a 60-line ctypes
`WaitForDebugEvent` loop (scratch); `procdump`/`cdb` are not installed.  The WER dumps in
`%LOCALAPPDATA%\CrashDumps` parse, but the 32-bit context in them is a WOW64 one (716 bytes, not 2747).
Register values from the exception record (code `c0000005`, `ExceptionInformation[1]` = the fault address)
plus `build/link/full/full.map` were enough to locate every crash.

### Debt (the accepted regressions)

> Update (2026-10-27, grade-restoration session): the three data-oracle byte losses, the four
> declaration byte losses and the ten readability losses below are **paid** - 102 of the 106 grade falls
> are restored, see "Where we are" at the top. What is left of the grade debt is five functions
> (`ls_ReadPlayerRecords`, `sv_Unk530100`, `ai_Unk490c10`, `ai_Unk580a50`, `ai_MasterUpdate`), each with
> the reason written up at the top of this file.

- **1041 data-overlap ledger errors from the generated gap symbols.** A gap is sized to the next known
  address *start*, but several named views (`ai_Unk144ad24`, `dd_GLoss`, `ch_Motions`, ...) extend into the
  gap, so `full_<sec>_<addr>` claims bytes a named symbol already owns.  Fix: size a gap by the next address
  *end*, not start, and mark a gap whose span is inside a defined symbol's extent as an inner view instead of
  a gap.  Until then `uv run ledger check` stays red; the exe is unaffected (the overlap only claims bytes).
- ~~**3 data-oracle byte losses**~~ **paid**: `d3_SetBlendState`, `ahm_AddPlace` came back by making
  `d3d_alphatest` / `ai_OwnPlayer` *tentative* definitions (COMMON) in their units - an initialised
  definition in the reading TU is what flips the load to `mov eax,[x]; and 0xff`. `ai_MasterUpdate` is
  **not** the `ai_ActionFlags` ownership case (that is already COMMON and irrelvant); it is a register-role
  difference, still open.
- ~~**4 byte losses from the rename / d3_SetFog / d3if_SelectScene declarations**~~ **paid**:
  `d3_SetFogSet` and `d3_LoadScene` use a unit-local `d3_SetFog` declaration matching the original call
  (`D3FOGCOL` by value / three dwords), `d3if_SelectScene` is `char (int, char)` again, and `ai_Unk490c10`
  is a code-sharing difference (two ICF-folded symbols in the original), still open.
- ~~**10 readability losses**~~ **paid**: `gm_Unk50c550` / `gm_Unk5085f0` are `gm_Liking` /
  `gm_PlayerFactor` now (148 references), and `full_data_624588` is `main_GfxPath`.
- **403 overlaps**: 400+ named symbols (`ai_Unk5eb798`, `amt_Pay`, `d3_ScrollSpeed`, ...) are *views of the
  original's pooled float/double literals* (`__real@...`). Defining them resolves the full link but claims
  bytes the original implements as literals. The source-level fix is to use the literal (or the literal's
  own symbol) at the call sites; that is matching work, not a data-definition bug.
- **4 D7 order errors**: the owner guess put a definition in a unit whose code order contradicts the data
  address (am.c .rdata, d3_vec.c .data, cm.c .data, d3_snd.c .bss).
- `uv run link`'s 13 conflicts are the same literal-view / data-oracle symbols (16 before this session;
  three went away with the paid items above).
- The `d2_GrabShape*` five are closer (the D2KEY unpack fix) but still drafts; `veg_FindType` became
  byte+readable (its `@deep` tag was cleared).


### Done so far

1. **mss32.lib** (the missing Miles import library). Generated from `orig/GildeGold.exe`'s import
   directory: `tools/make_mss32.py` + `config/mss32.def`. 31 imports, verified to resolve with the exact
   decorated names (`_AIL_startup@0` …). `LIB /DEF` alone strips the leading underscore, so the script
   patches each import member's name-type NOPREFIX -> IMPORT_NAME.
2. **`uv run link --full`** (`tools/gildegold/link/link.py`). Links all 92 objects in address order +
   LIBC + import libs + mss32, `/INCLUDE`s every function, and classifies every LNK2001 into
   `build/link/full/unresolved-{data,renames,other}.txt`. `--force` adds `/FORCE:UNRESOLVED`, which makes
   LINK write `build/link/full/full.map` + the exe although symbols are missing (measurement only: the exe
   is not startable). The map is the scoreboard for the data work: compare each symbol's VA with its
   `data.csv` address; within one section a constant delta means the order is right and only the base moved
   (our `.text` is still shorter than the original's), different deltas mean the block order or a size is off.
3. **The 849 renames** (committed). `tools/rename_stale.py` renamed `ai_Unk<addr>` / `d3_Unk<addr>` / …
   forward-declaration placeholders to their canonical names.
4. **The declaration layer** (`tools/align_decls.py`, `tools/decl_conflicts.py`,
   `tools/annotate_original.py`, `tools/fix_fwd_comment.py`). Three passes in order:
   - `--returns-only --safe`: 235 declarations whose return type was a wrong `int`.  Only
     `void` / pointer returns at first (they cannot move a call site's bytes); 233 edits.
   - `--abi-safe` (default): 59 + 83 declarations whose prototype has the same call shape as the
     canonical one — same parameter count, same pointer depth (typedef chains resolved, so
     `DWORD`/`int`, `LPDIRECTDRAWSURFACE4`/`void *` are one class), same scalar width, tags the file
     can name.  142 edits, bytes unchanged.
   - `--all` for the rest: the old-style `T f()` declarations whose call sites all pass the full
     argument list.
5. **The data globals, first pass** (`tools/data_defs.py`, 2026-10-27). Turned the work list into
   definitions: COMMON `T x;`, a unit's .bss `T x = 0;` / `= {0}`. 472 COMMON + 448 .bss written across 57
   units; `.data`/`.rdata` were reported only (they need the original value). 128 symbols stayed `extern` on
   purpose in `config/data_defs_skip.txt`.
6. **The data globals, second pass** (this session, `tools/data_values.py` + `tools/data_defs.py`).
   `data_values.py` renders a `.data`/`.rdata` initializer from the original bytes: scalars, arrays, strings,
   pointer words turned back into the symbol they name (`&name` / `name` / the function), and structs laid
   out from the headers' own field lists (a balanced-brace parser, so anonymous unions count). The gaps in
   `.bss`/COMMON size an open array from the original's next declared address; a terminator record sizes the
   `G15TREE` / `G16TRACKS` / `G16LOC` tables; `OPEN_COUNT` records the six counts the gap cannot settle
   (`ai_Chars` 768, `ai_CharInts` 768, `ai_PlayerSlots` 4, `ai_Unkf8f13c` 1, `cm_SaveAcks` 8,
   `gm_PlayerRecords` 688). 604 -> 0: 601 definitions + the two `static`s (`d3t_CamMoving`, `d3t_CamMove`)
   made external. The skip list is no longer passed (its ownership entries are the 3 regressions above).
7. **The 182 "other" -> 0** (this session). The full-link driver now classifies stale *prototype* names
   (a small lexer for the address comment of a top-level declaration) and data aliases from `data.csv`;
   `rename_stale.py` applied 40 function renames. A generated COFF object of weak externals (storage class
   105, `SEARCH_ALIAS`) resolves the 118 data aliases and 18 CRT internals (`uv run link --crt` names them)
   to the one definition; `AIL_get_timer_highest_delay` maps to its stdcall import. The duplicate
   `__alloca_probe` call was removed; `MSS_auto_cleanup` is a stub in the full-link directory;
   `ai_Unk7662c0` got its address and a COMMON definition.
8. **Entry + resources** (this session). The full link uses the game's `main_Unk53ccf0` through a WinMain
   wrapper (the CRT startup stays; `/ENTRY` pulls crt0's `_main`), and `tools/make_icon.py` + RC.EXE add the
   original's `101 ICON`. The exe links with 0 unresolved and loads.
9. **The address literals -> symbols** (2026-10-27, second session). 7214 occurrences in the code of 21 units
   (1766 distinct data addresses, 75 function addresses) are symbol references now; 1041 address gaps are new
   `full_<sec>_<addr>` definitions in main.c.  `uv run build` green, **0 REGR**, lint E 0 / X 0; the full
   link still 0 unresolved.  The exe now runs the window, main_Init, the whole `main_Unk531c50` screen open
   (dd_CheckModes, d3_Init, tex_SetPalette, d2_Open) and the gamedata table load; it dies in the production
   window's data model (`ai_Unk5a8390`).  Data matched 36.0% -> 69.8%.

### What the declaration work taught us (both are now encoded in the aligner)

- A struct/union tag first seen in a **parameter list has prototype scope**: two `struct D3SURF *`
  parameters in two declarations are two different types.  Every file that spells `struct TAG` needs a
  file-scope `struct TAG;` first (the aligner adds one just below the unit header comment).
- Old VC6 emits `and eax,0xff` / `movsx` at a call site from the **declaration** the caller saw, not
  from the callee's definition.  Two files can therefore carry two different types for one function and
  both be byte-correct.  Those are `@original` cases, never alignment.
- **An old-style `T f()` is compatible with a canonical prototype only when every parameter is
  default-promotion-stable.**  `char` / `short` / `float` parameters make the two incompatible, and a
  strict prototype can also reject the calls that were compiled against the non-prototype declaration
  (`cm.c`'s `ch_FindFreeCell` passes 8 of 9 arguments).  Every such declaration carries
  `@original <call-site evidence>`.

### What the data work taught us (2026-10-27, details in notes/data.md)

- **A definition in a TU changes that TU's code.** With `unsigned char d3d_alphatest = 0;` in d3_d3d.c,
  VC6 loads it as `mov ecx,[x]; and ecx,0xff`; the original `d3_SetBlendState` loads a byte
  (`xor ecx,ecx; mov cl,[x]`). So defining a symbol in unit U when the original defined it elsewhere
  *loses* matched code — which makes `uv run build`'s `REGR` line a reliable **ownership oracle**: if the
  definition breaks code in U, U is not the owner. It cost 2 matched functions and 2 near matches to
  learn (ai_OwnPlayer, d3d_alphatest, the txt.c tables); they are in the skip list and the bytes are back.
- **The COMMON area is LINK's own order**: reverse *object* order (three-object experiment: zc1 < zc2 <
  zb1 < zb2 < za1 < za2 for objects a b c), declaration order within one object. So the original's COMMON
  addresses cannot come out of our link order by accident, and the owner of a COMMON symbol cannot be
  read off the address. Our 472 COMMON definitions link but do not land at the original addresses (the
  map deltas are all different) — that is a separate pass.
- **`.bss` blocks are contiguous per unit in code order**, so an address does tell the owner, but the
  declaration site is a poor proxy (`d3_LightKeysCulled` is declared in d3_vec.c and sits in an earlier
  unit's block) and byte flags packed next to dwords make a `T x[4]` declaration one byte too wide
  (`d3_MipSkip`). 28 `not one block` warnings mark that debt.
- **VC6 drops a tentative definition of an incomplete array** (`T x[];` emits no symbol at all), so an
  open array must be given a size — but only from evidence (the gap to the next declared address when it
  is a plausible buffer); a guessed size changes the compiler's view of the bound and moves code.
- A written file must be rebuilt from scratch (`rm -rf build/obj`): one CL.EXE run per flags group means a
  failing group writes no `.flags` stamps for the files that did compile, so those objects keep an old
  stamp and `uv run build` calls them up to date (this cost an hour of chasing phantom regressions).

## Next steps (in order)

### 1. Make the full exe run -- DONE (main menu reached; see "Where we are" above)

The menu is up. What is left of this step:

- **Click through the menu.** The nine buttons, options screens, "new game" and the scenes load data through
  more of the code paths fixed here (text views, character walks). Every crash is again one of the four
  classes: a walk bounded by another symbol, a view of one block defined twice, a definition smaller than
  the view, or a struct field whose offset the header models differently. `build/crashwatch.py` +
  `build/watchwrite.py` locate them; `build/bounds_fix.py` finds the next walk.
- **Regenerate the gaps** (`tools/full_addrs_gaps.py` after `uv run link --full`) so the 32 gap-byte
  conflicts disappear; the tool must size a gap to the next address *end* (Debt below).
- **The version buffer** (old note): now inside `txt_Ptrs`; the title shows correctly. Keep an eye on
  `main_Unk531a00`'s other neighbours (`ai_OptionsFile`'s 4-byte gaps in the old note).
- **Function-address literals with no `names.csv` row** are still literals: `0x402c20`, `0x5770e0`,
  `0x4080a0`, `0x576490`.
- Run it from `build/link/game` (the Mss32/ddraw DLLs and data are there); that path carries the
  `DWM8And16BitMitigation HIGHDPIAWARE` shim.

The old debugging notes for the classes reached this session:

- **`ai_Unk5a8390` (`unk_58bbd0.c`, production window, crash at +0x5).** Run the exe under the ctypes
  debugger, read the fault address, and decide which of the three classes it is:
  1. a **definition too small for the view the code writes** (the `d3_Planes` / `d3d_UsedTexFormat` class):
     enlarge the definition to the type the *view* declares, keep it a real definition if it is an alias
     target;
  2. an **alias target that is COMMON** (the `0x400004` class): make it `= {0}`;
  3. a **struct field offset** that our header models differently from the original (the `gm_FreeList` /
     `gm_LoadTables` class, where an object pointer read from a record is a float): read the record with
     `uv run disasm`, compare the field offsets with the header, fix the header.
- **The version buffer.** `main_Unk531a00` writes "Europa 1400 - Gold Edition - 2.06" into the gap symbol
  `full_data_75e760` (4 bytes).  The original's buffer is 0x30-odd bytes inside `ls_SavePath`'s 640-byte
  block; either size the gap from the write (main_Unk531a00 writes ~0x30 bytes) or name it properly
  (`main_Version`) and give it that size.  Same for `ai_OptionsFile`'s neighbours — the window title is
  empty because of this, and `GetPrivateProfileString` buffers written into 4-byte gaps corrupt their
  neighbours.
- **Gap sizes.** `tools/full_addrs_gaps.py` sizes a gap to the next known address *start*; make it the next
  *end* (and turn a gap whose span is inside a defined symbol into an inner view).  That also removes the
  1041 overlap errors.
- **Function-address literals with no `names.csv` row** are still literals: `0x402c20` (he.c, the CRT
  region?), `0x5770e0`, `0x4080a0`, `0x576490`.  Name or leave them; they are few.
- Run it from `build/link/game` (the Mss32/ddraw DLLs and data are there); that path carries the
  `DWM8And16BitMitigation HIGHDPIAWARE` shim.

### 2. Restore the accepted grade regressions -- DONE (2026-10-28 session)

Four of the five were matched back to their baseline grade (see "Where we are" at the top for how each one
came back: the literal form the address-literal pass replaced), and `ai_MasterUpdate` (68 asm, a
register-role difference) was accepted with `uv run ledger grade accept` - the baseline now records it at
draft. `uv run ledger promote` was run.

The hook is **still off**: `uv run ledger check` now exits 0, but the hook also runs
`uv run link --nonmatching`, which still reports 12 conflicts on 12 symbols and 36 functions not placed
(the data-oracle / gap-ref symbols).  Turn it on (`git config core.hooksPath tools/hooks`) once that link
mode is clean too.  The one grade loss left, `ai_MasterUpdate`, still needs the deep matching work (its
`@deep` reason is written up in the note above).

### 3. Pay the data debt -- 1330 -> 160 -> 78 -> 20 -> 14 -> **0** errors

The fifth pass (2026-10-28, "the data debt is paid", the top section) closed the 14: `d3_Scenes` is the
container of five more names (the pointer-valued `ai_MapRefs` view), `ai_Unk91f700` is the container of
f3gm's goods table, `ch_Motions` is 64 records with `ai_Unk1469700` / `ch_ActDefs` as views at +0x20,
`d3_Cfg`'s two fields are macros (the field *spelling* was the costly form, the macro is byte-neutral), the
two odd-address pairs are one 1-aligned container each, three read-only constants moved to .rdata, and the
`d3_DefaultScene` owner moved to d3_engine.c.  Two D7 errors went with the rule corrections (gap symbols
do not place a unit; .rdata is not object-ordered) and one with the .bss border fix in `ledger_data.py`.
The text below is the first pass' list, kept for the class descriptions.

- ~~**The definition-inside-another's-storage class**~~ **paid for the named cases**: `d2_AnimView`,
  `d2_HitRects`, `init_StockBlocks`' fields, `d3_Cfg`'s `d3_SortMax`/`d3_Unkfe7400`, the
  `d3d_UsedTexFormat` block, `d3_NoTextures`/`ai_Unk6f0455` (views over the neighbouring byte). The
  remaining clusters are the same shape; the blocker is the 193-199 extern declarations per name, not the
  model.


- ~~**The generated gap definitions**~~ **paid**: exact sizes + `align(1)` + the literal/view resolution
  (`tools/gap_fix.py`).
- ~~**The literal-view overlaps**~~ **mostly paid** (`amt_Pay`, `ai_Unk5ee160`, `d3_ScrollSpeed`,
  `ai_Unk5ebbb4`, `d2_Wins`, ... trimmed; the gaps over literals are literals now).  Left: the per-TU
  literal globals (`ai_CityNameFormat`, `evt_Path`, `d3_DefaultD3DConfig`).
- ~~**`align(1)` on every data definition**~~ **tried and reverted** (`tools/data_pack.py` existed for a day):
  packing every definition makes an item's extent its declared size, but it also makes the original's layout
  look packed -- the original has padding too -- so a unit's section then shares one (wrong) base and the
  ledger gained overlaps (175 -> 183) and `gm_BuildKindObjects` lost bytes.  The pad class needs the real
  per-symbol size/type (`d3_MipSkip` is already `unsigned char`; the pad is the next symbol's 8-alignment).
- **The field views**: `init_StockBlocks[8]` is `[5]` and its nested `ai_Unk7d6a90`-style symbols are its
  fields; `d2_AnimView` is `d2_Anims` as `D2SND` (same 0x11 stride, `x` at 0xd).
- **COMMON order**: the tentative definitions do not land at the original addresses (LINK orders COMMON in
  reverse object order); the pass needs `full.map` against `data.csv` per section.
- Scoreboard: `uv run link`, then compare `build/link/full/full.map`'s publics with `data.csv` per
  section (constant delta = right order, other deltas = wrong order/size).

## Key commands

```
uv run link                           # link every object (the exe); writes full.map + unresolved-*.txt
uv run link --force                   # the same + /FORCE:UNRESOLVED (only needed while symbols are missing)
uv run play                           # debug build (REA_FIX_16BPP + REA_FRAME_DUMP), link, and play the exe;
                                      #   a crash writes build/crash/<stamp>/ (crash.txt, crash.dmp, frame PNGs)
uv run play --no-build --seconds 30   # smoke-run the last exe; --no-debug, --intro, --args for the variants
uv run dbg --watch net_Conn.closed --on 'bt,read' --read net_Conn
                                      # the debugger: symbolic --bp / --watch / --trace / --read, --on ACTIONS;
                                      #   a crash writes the same build/crash/<stamp>/ as play
uv run dbg --struct NETCONN           # the layout clang computes (no process); --struct --check for every
                                      #   `/* N bytes */` header note
uv run play --snapshot net_Conn,ai_Unkf31160[2]
                                      # crash.txt gets those globals through their layout (--dump state|normal
                                      #   for a smaller minidump than the default full memory)
uv run shot --now                     # dump the frame the running game is showing (REA_FRAME_DUMP) + PNG;
                                      #   play's launch-armed dumps are the first, black frames
uv run play --start tutorial          # take a main-menu entry without a click (REA_START debug build; with
                                      #   dbg: --start tutorial --bp ai_Unk57bce0 to stop in that path)
uv run python tools/make_mss32.py     # regenerate mss32.lib
uv run python tools/make_icon.py      # extract the 101 ICON from orig into build/link/full (RC.EXE compiles it)
uv run python tools/rename_stale.py   # rename stale names (reads unresolved-renames.txt)

# the address-literal layer (this session): the original's absolute addresses -> symbols
uv run python tools/full_addrs.py                # dry run: what would be rewritten, which names need a decl
uv run python tools/full_addrs.py --write [SRC]  # rewrite (default: all src/game/*.c); `--gap-owner main.c`
uv run python tools/full_addrs_gaps.py           # define the full_<sec>_<addr> gaps from unresolved-other.txt
uv run python tools/full_addrs_fixdecl.py        # repair the per-unit declaration block from build.log
                                                 #   (drop a line the header already declares; hoist/fwd-declare)
# the loop: link --full -> full_addrs_gaps.py -> build -> link --full

# the data-debt payers (this session):
uv run python tools/data_views.py [--write] [--only CLUSTER...]
                                                 #   (run it WITHOUT --only when a header change is
                                                 #    involved: --only rewrites the whole header's
                                                 #    cluster groups and moves the other units' includes)
                                                 # a name inside another object's storage -> a macro in
                                                 #   include/data_views.h (the CLUSTERS table names the pairs;
                                                 #   a header keeps its declaration under #ifndef DATA_VIEWS_H,
                                                 #   the include goes after the unit's own headers)
                                                 #   VIEW_TYPE names the shape the *code* needs when the
                                                 #   definition is another shape (a pointer-valued view like
                                                 #   ai_MapRefs, a 1-aligned container's table at +1/+2);
                                                 #   VIEW_ADDR for a view data.csv lists only as an alias
                                                 #   (`ALIGN1` = `__declspec(align(1))` in a macro: gen cannot
                                                 #   parse a `(`, and the extern must carry it too)
uv run python build/view_plan.py                 # scratch: the clusters with type, offset and size
uv run python build/scratch/runexe.py EXE CWD SECS   # start the game, wait, kill (GUI: never leave it running)
uv run python tools/gap_fix.py --resize          # pack the gap block: exact sizes + align(1) (+ the externs)
uv run python tools/gap_fix.py --externs         # `extern GAPDATA ...` (VC6 takes alignment from the 1st decl)
uv run python tools/gap_fix.py [--write]         # resolve the gap overlaps: literal / float / view / trim
uv run python tools/data_trim.py [--write]       # trim an oversized definition to the next item's start
                                                 #   (fixes the dimension, cuts the initializer, relaxes the
                                                 #   header's extern to an open array)
# order: gap_fix --resize -> gap_fix --externs -> build -> gen -> gap_fix --write -> build -> gen
#        -> data_trim --write -> build -> gen -> ledger check

uv run python tools/data_defs.py      # the data-global workhorse (dry run; --write applies)
                                      #   --area common|bss|data|rdata  --unit SRC  --limit N
                                      #   --skip NAME|@config/data_defs_skip.txt
                                      #   .data/.rdata values come from tools/data_values.py (the original
                                      #   bytes at the address comment); the skip list records the old
                                      #   ownership/member-alias/wrong-size debt, not needed for the link
                                      #   run it after every `uv run link --full`, then
                                      #   rm -rf build/obj && uv run build   (see the stamp gotcha)

uv run python tools/decl_conflicts.py # the X-conflict report: canonical + every differing declaration
uv run python tools/align_decls.py    # dry run of the alignment; --write applies it
                                      #   --returns-only --safe | --abi-safe (default) | --all
                                      #   --symbols A B ... | --skip A B ...
uv run python tools/annotate_original.py FILE SYMBOL "why"   # @original on a declaration line
uv run python tools/fix_fwd_comment.py --write FILE ...      # repair a displaced unit header

uv run lint                           # clang E/X/C counts
uv run ledger check                   # the gate (pre-commit)
uv run ledger data-next               # first undrafted data run
```

## Notes / gotchas

- **VC6's data alignment rule** (measured, `build/scratch/align`): a data object of 8 bytes or more is 8-aligned,
  every object is at least 4-aligned, and `__declspec(align(1))` overrides it -- *except* that **an object
  takes its alignment from its first declaration**, so an `extern T x[];` before the definition defeats the
  definition's `align(1)`.  That is why `tools/gap_fix.py --externs` exists.
- **The ledger reads an item's extent as "up to the next symbol of our object"**, not as the declared size, so
  padding between our symbols is counted as part of the item in front of it.  Packing a block with
  `align(1)` makes the extent the declared size again; doing that for every data definition is the next
  step for the pad class of overlaps.
- **A generated block of definitions must be packed**: the gaps came out with padding between them, which
  alone accounted for ~1300 of the overlap errors even where the sizes were right.
- `gen` takes a declaration containing `(` for a prototype (`if "(" in head and fp is None: continue`), so a
  `__declspec(align(1))` prefix hides the definition from `data.csv`.  Put it in a macro (`GAPDATA`).
- `uv run python tools/*.py --write` must be idempotent and safe on a half-written tree: `data_trim.py` once
  dropped everything *before* the rewritten declaration (it returned `head_new + tail` instead of
  `text[:start] + head_new + tail`) and truncated five files.  Check `wc -l` after a bulk rewrite.

- The lint gate is the *byte-matching* discipline; the VC6 build being green is what the *link* needs.
- `@original <why>` must be on the line clang points at.  For a declaration that is the whole line; for a
  call it is the call line.  `S.code` (gen) has comments blanked — read `S.text` when looking for tags.
- `tools/fix_fwd_comment.py` exists because a forward declaration prepended at offset 0 pushes the unit
  header comment (`@named_by` / `@boundary` / `@bytes`) down, and the ledger then stops seeing a
  `@bytes` declaration (48 bytes of unk_575860.c became undrafted once).
- `RC.EXE` (VC6, `Common/MSDev98/Bin`) trips the Windows Program Compatibility Assistant: a
  "Diese App kann nicht ausgeführt werden ..." dialog appears on `uv run link --full`. RC still runs
  (rc=0, 1204-byte `.res`); the dialog is noise. The game exe itself keeps the `DWM8And16BitMitigation
  HIGHDPIAWARE` shim at `build/link/game/GildeGold_full.exe` (HKCU `AppCompatFlags\Layers`), like the
  installed game and the `hybrid.exe` entries.
- `uv run link --full` now succeeds and writes `full.map` without `--force`; `--force` is only for the
  earlier stages (and for measuring when a symbol is missing).
- The full link adds two generated inputs: `build/link/full/entry.obj` (WinMain -> `main_Unk53ccf0`),
  `stubs.obj` (`MSS_auto_cleanup`), `aliases.obj` (weak externals: the data aliases + `CRT_ALIASES`) and
  `icon.res`. Re-run `uv run link --full` after changing `data.csv` so the alias object is current.
- After `git checkout -- src/` the state (`data.csv`) is stale: run `uv run build` *and*
  `uv run link --full` before `data_defs.py`, or it works from an old work list.
- **A COFF weak external cannot alias a COMMON symbol** (LINK stores `IMAGE_BASE + 4`): every alias target
  must be a real definition (`T x = {0};`).  This is why 57 `common` rows in `data.csv` became `.bss`
  definitions; a new alias whose target is `kind=common` will break the same way.
- **A definition smaller than the view the code writes** is invisible in the hybrid link (the original's
  neighbours are contiguous) and corrupts the full rebuild: `d3_Planes[4]` -> `[6]`,
  `d3_TexFormat8Bpp` (4 B) -> the 0x50-byte `d3d_UsedTexFormat` block.  When a function writes past its
  array in the sources, the type is wrong even though the bytes match.
- **`static` at a literal-addressed symbol** cannot be reached by name from another unit: `d3t_SunRays`,
  `d3t_CamMoving`, `d3t_CamMove`.
- **`__asm` blocks and bit-mask literals**: `test edx, 0x800000` in `dd_CpuHasMMX` is *not* an address;
  `tools/full_addrs.py` must not rewrite a literal inside `__asm { }` (it did once, and the assembler
  rejected `((unsigned)&ai_Relations + 0x1aca0)`).  The `0x400000` image base appears as a plain constant
  in several places too — the tool reports it as `unknown` and leaves it.
- **A block inserted "after the last #include" can land inside `#ifdef REA_FRAME_DUMP`** (d2.c, dd_vesa.c):
  the compiler then ignores every declaration in it.  Put it after the `#endif`.
- **A declaration block above a function's doc comment** hides the `/* 0x... */` address line from `gen`
  ("no address comment above its definition").  Insert it *above the comment block*, not between comment
  and signature; `tools/fix_fwd_comment.py` repairs the unit header variant.
- **A 1-aligned container models a packed block** (`__declspec(align(1)) char X[n]`, the `ALIGN1` macro):
  VC6 8-aligns any object of 8 bytes or more, so a separate table can never sit 1 or 2 bytes after a flag —
  one container with the table as a view can.  The **extern declaration must carry the alignment too**
  (`extern ALIGN1 char X[277];`): an object takes its alignment from its first declaration.
- The ledger's D7 places a unit by its own plain data blocks only: the generated `full_<sec>_<addr>` gap
  stand-ins (owned by main.c by construction), pooled literals and .rdata order violations do not count
  (`.rdata` is not object-ordered — measured on ai.c's masks vs am.c's ag_Pairs).  The .bss border is the
  first located .bss block inside .data, so a zero-initialised definition at an .rdata address cannot make
  the whole .data area "bss".
- **An alias object's canonical name must be a name an object defines** (`link.py` picks it from the COFF
  symbols and skips a row with no defined name): a data.csv row's name column can be the stale placeholder
  while the real definition is in the alias column, and a weak external at a view's extern stays unresolved.
- Byte budget: 27.1% byte-identical (2661 functions matched), data 69.8% matched
  (`uv run python -m gildegold.ledger.progress`). The accepted regressions are listed under Debt above.
