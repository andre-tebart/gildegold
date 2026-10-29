# Link tooling

## Current state (2026-10-28): the full link only

`uv run link` links every object with the real LINK.EXE into `build/link/GildeGold_full.exe` and exits 1 on
any unresolved external; the pre-commit hook runs it after `uv run ledger check`. The hybrid exe is gone: no
writing our bytes into a copy of the original, no `--nonmatching` / `--only` / `--skip` / `--out` / `--crt` /
`--lnk`, no extra `.hybrid` section, no `tools/run_hybrid.ps1` and no `hybrid_bisect`. What remains in the
link module is the COFF/PE reader, the full link, the alias-object writer and the LIBC.LIB data placement.
The read-only address evidence the data ledger uses moved to `tools/gildegold/ledger/evidence.py` (original
relocation sites of matched functions, literal/table content, imports, CRT symbols, comments). The
`REA_FIX_16BPP` / `REA_FRAME_DUMP` compile flags stay as full-build debug flags; `uv run framedump` reads the
dumps. `build/link/mss32.lib` (the Miles import library, generated from the original's import directory) is
created on demand by the link and seeded into worker worktrees. `uv run play` compiles with the debug flags,
links, and runs the exe under a crash reporter (`build/crash/<stamp>/crash.txt` + `crash.dmp` + frame PNGs);
it rebuilds the normal objects after the link, so the grades and the pre-commit gate stay clean.

## `uv run dbg`: the debugging side of the full build

`uv run play` says *that* it crashed; `uv run dbg` says *why*, without a new throwaway script per question.
It runs the last link (like `play --no-build`; `--build` compiles and links first) under the same debugger
`play` uses, and every target is named by symbol and resolved through `build/link/full/full.map` at start. A
raw address is **refused**: the map moves with every link (`vfs_fread` moved 0x430bd0 -> 0x430ae0).

- `--bp SYM[+0x10]` an INT3 at a code symbol. A hit restores the byte, single-steps and reinserts it with the
  other threads suspended (so no thread can run through the restored byte), and WOW64's `0x4000001F` /
  `0x4000001E` breakpoint and single-step codes, second-chance INT3s and the `Wow64Get/SetThreadContext`
  flags are handled once, here.
- `--watch SYM.FIELD` a hardware write watchpoint (DR0..DR3) whose offset and width come from the header
  layout, installed on every thread at `CREATE_THREAD_DEBUG_EVENT`. A hit reports the writer's **symbol and
  instruction** (capstone walks back from the faulting EIP) plus the stack. An unaligned 2/4-byte field is
  widened to the aligned dwords around it and says so: x86 silently ignores an unaligned watch (the
  0x6ede4f 4-byte watch that never fired), and the report names the covered range.
- `--trace SYM[:EXPR]` logs count, caller, arguments and, with EXPR, a value and its total
  (`--trace 'vfs_fread:a1*a2'` = size*n and the bytes read so far).
- `--read ITEM` dumps a global through its layout: `net_Conn`, `net_Conn.rx->type`,
  `ai_Unkf31160[2].idx`, at the crash, at exit and at any stop whose `--on` names `read`.
- `--on TARGET:ACTIONS` is the per-question part: `regs`, `stack=N`, `sym(EXPR)`, `sum(EXPR)`,
  `mem(EXPR,N)`, `str`, `arg(N)`, `caller`, `bt`, `depth`, `read`, with EXPR over `a0..a7`, `ret`, the
  registers, `count`/`total` and u8/u16/u32.
- `--struct NAME` prints the layout clang computes for one record (or global) without starting a process;
  `--struct --check` compares every `/* N bytes */` header note with it (all 243 agree today). The layout is
  computed **per unit**: `ch.h` is not self-contained (it needs D3OBJECT/D3ANIM/D3SCENE from other headers)
  and two units may declare one name differently (`struct D3IOKEY` is 0x38 in d3_int_io.c, 0x58 in
  d3_io_t.c), so a header-only parse would be wrong. `build/cache/dbg_types/` caches one clang run per unit,
  keyed like the lint cache.

A second-chance exception writes the same `build/crash/<stamp>/` report as `play`, with the stop log and the
`--read` items appended to `crash.txt`.

## Crash reports: the dump is the state, the stack is the chain

`play` (and `dbg`) write `build/crash/<stamp>/`; four things in it changed to make a crash answerable offline:

- **full memory**: `crash.dmp` is `MiniDumpWithFullMemory` (~23 MB for this exe, 0.1 s), not the old
  stacks-only `MiniDumpNormal` (17 KB). The globals, the pools and the character table are in the dump, so a
  post-mortem reader can answer "was the slot value set?" without another run. `--dump state` is the middle
  ground (data segments + private read/write pages, ~16 MB), `--dump normal` the old behaviour.
- **`--snapshot`**: `play --snapshot net_Conn,ai_Unkf31160[2],net_Conn.rx->type` writes those globals into
  `crash.txt` through their clang layout (the same `Read` paths `dbg --read` uses), next to the registers and
  the stack.
- **the faulting instruction**: `access: write of 0x00000040 (not in any module)` is now followed by the
  instruction and its effective address, `mov byte ptr [eax + eax*8 + 0x...], dl  [eax+eax*8+...] =
  0x... (eax=0x...)`, so a null base plus a field offset reads as such; a target that is in the image is
  still named (`gv_Graveyard+0x...`).
- **a probable call chain**: the ebp walk stops at one frame with /Oy, so `crash.txt` also lists the stack
  words whose bytes below them are a call instruction ending exactly there (`E8 rel32`, `FF /2`), in stack
  order and with the callee named: `0x00468989  d2_FlushDirty+0x49  <- call d2_BlitPart`. `dbg`'s `bt`
  action uses the same chain. The old "stack scan" stays, but only for words near a code symbol and labelled
  unvalidated.

`--start NAME` skips the main menu: `main.c` has a `REA_START` hook (debug build, `uv run play|dbg --start
tutorial`) that takes one menu entry exactly as a click does - the same button id, the same action code, no
GUI automation. Entries: `new`, `load`, `tutorial`, `network`, `gameoptions`, `gfxoptions`, `sfxoptions`,
`credits`, `quit`, and the debug-data `ini` / `netsave` / `checktext`. The hook is compiled out of the normal
build (`REA_START` is only in `play`/`dbg --build`), and `--start` refuses an exe without it. `--start
tutorial` also advances the tutorial's round-change page (`main_Unk5384e0`'s "Weiter" scroll) the way a click
does, so a run reaches the in-game path without GUI automation.

That page's "Weiter" then exposed a chain of bugs on the way into the game: `main_Unk5384e0`'s `ln` local was
0x100 bytes, but `gm_Unk50af00` memsets a whole 0x21c-byte record into it and zeroed the frame's saved ebp /
return address; `sv_Unk522f10` called `d2_ShpBlit(0, 0, 0)` for the grabbed view shape (the fourth argument
was stack garbage); its money line `txt_Format("%I~ %w", h)` had lost the owner argument, and `%w` is the
player-slot city-name look-up (`txt_Field(cityNames[player.city], 1, 8)`), not a string argument (both text
engines). Two full-link data collisions followed: `ai_Time` was declared as 3 bytes in anm.c, so cm.c's
message 3 (`ai_Time[6]` / `[10]`) overwrote `anm_Animals`; and `ch_ActDefs` (the 0x1300-byte view at
`ch_Motions + 0x20`) reaches 0x20 past `ch_Motions`, so `ch_Open`'s memset zeroed `ch_Queue`. With `ai_Time`
14 bytes and a 0x20-byte `ch_MotionsTail` symbol, `uv run play --start tutorial --seconds 120` exits 0
(0 errors in `ledger check`; the ai_Time / ai_TimeHour alias is the one new data-layout suspect).

The frame dumps are armed at launch, so a crash after a minute still has the first (black) frames; the
report now says how old they are. `uv run shot --now` arms the trigger of a running game instead, waits for
the frame and converts it (and the crash report picks those up): `uv run shot --now -n 2`, `uv run shot` (the
newest dump), or `uv run shot RAW...` (convert given dumps).

```
uv run dbg --bp ls_ReadPlayerRecords+0x10 --on 'regs,stack=16,sym(ret)'
uv run dbg --watch net_Conn.closed --on 'bt,read' --read net_Conn
uv run dbg --trace 'vfs_fread:a1*a2' --max 50
uv run dbg --struct NETCONN
```

Everything below is the history of the hybrid link (rounds 12/13), kept as the record of the conventions it
established. Read "the link" there as "the hybrid link", and the rule numbers as rules the sources still
follow.

## Report (round 12)
- **Hybrid exe works.** `uv run link` writes our objects into a copy of GildeGold.exe at their original
  addresses (relocations applied, everything without source left as original bytes). With the matched functions
  only, the output is byte-identical to the original: 971 of the 1003 functions are written, and 32 keep the original
  bytes because their linked bytes differ (see "Wrong symbols" below). `--nonmatching` adds our nonmatching code:
  1066 functions are written in place, 12 that are longer than their slot go to an extra `.hybrid` section
  behind a `jmp`, and 34 are skipped (unresolved or conflicting symbols). That exe starts, plays the intro
  videos through our `mov_Open`, and reaches the main menu (`tools/run_hybrid.ps1`, screenshot checked).
- **New gate.** `uv run link` exits 1 on a symbol conflict (until 2026-10-25 only on one not in the baseline
  `config/symbols/link_baseline.txt`, 109 known in this round; the baseline reached zero and is gone, `--nonmatching`
  also fails on a function it cannot place, and the pre-commit hook runs it). It catches what `uv run build`'s
  masking cannot see: a reference to the wrong global, function, literal or float constant; a wrong jump-table
  order; wrong string text; wrong `/* 0xADDR */` comments.
- **CRT = SP5 LIBC.LIB.** 261 code sections of 157 LIBC.LIB objects are found byte-identical (relocations masked) and
  cover 0xc053 of the 0xc814 bytes from 0x5de4a0 to the end of .text (`uv run link --crt`).
- **Real LINK.EXE probe.** zlib+unzip linked by LINK 6 (`--lnk`, /OPT:REF, original object order) puts all 85
  functions at the original offsets, except that everything after the empty static `tr_static_init` sits 0x10 later.
  The original has no copy of it in trees.obj: its `_tr_init` calls the shared empty function FUN_004b70f0.
- **Convention demos** (they change no code, and `uv run build` still exits 0): `src/game/d3_sky.c` gives
  `#line 38` the original path, so `__FILE__` now matches. In `src/game/d3_snd.c` the statics are now
  `= 0` / `= {0}`, so its .bss is one block at 0x631130..0x6311f8, laid out like the original.
- No function scope, no names.csv rows, no shared-header changes. Commits: 3118faf and the notes/tool commit.

## Tools
- `uv run link` (after uv run build) writes `build/link/GildeGold.exe` and these reports:
  - `build/link/report.txt`: the conflicts.
  - `build/link/symbols.csv`: every resolved symbol, with how it was resolved.
  - `build/link/data_ranges.txt`: where each object's data definitions sit in the original, and whether each
    object's .data/.bss is one block laid out like the original.

  Options:
  - `--nonmatching`, `--only A,B` and `--skip A,B` choose what is placed.
    A function too big for its original extent goes to `.hybrid` with a `jmp` at its address; calls to it
    from other code resolve to that original address (as in the original), only references inside it use the copy.
    Data checks read original bytes past a section's raw size (virtual .bss) as 0.
  - `--update-baseline` accepts the current conflicts.
  - `--crt` runs the CRT probe.
  - `--lnk GLOB[,GLOB]` (with `--lnk-flags`) runs the real-LINK layout probe.
- `tools/run_hybrid.ps1` (Windows PowerShell, repo root): mirrors the GOG install into `build/link/game`, using
  junctions for the folders and copies of the small files. The install itself is never written. The mirrored
  game.ini is always set to window mode (`Bildmodus=DIRECTWINDOW`; the exe compares the upper-cased value with "DIRECTWINDOW" at 0x53cec1, anything else is fullscreen) with all sound off (msx / sfx / weather 0, all
  volumes 0); the script refuses to start otherwise (user rule). game.ini holds
  absolute paths into the install. The script runs the exe there, takes a screenshot every 4 s
  (`build/link/shot-N.png`) and reports whether the exe stayed alive. Smart App Control is off on this PC (user, 2026-10-21); while it was on, it blocked some freshly linked exes and not
  others (unsigned files are judged one by one).

Symbol resolution, strongest evidence first:
1. names.csv address (`placed`).
2. Import table through the SDK import libs (`iat`). WS2_32 is imported by ordinal, and the libs give the
   ordinals.
3. The original bytes at a relocation of a *matched* function (`site`): DIR32 gives S = value - addend, REL32
   gives S = value + P + 4 - addend.
4. String literals and `__real@` found once by content (`content`); GUIDs by their bytes in dxguid/strmiids
   (`lib`).
5. Fallbacks, only used for nonmatching code:
   - Ghidra FID names (`fid`).
   - The address in the name, e.g. `d3_Unk6334a4` or `FUN_xxxxxx` (`name`).
   - A unique `/* 0xADDR */` declaration comment (`comment`).
   - A fresh copy in `.hybrid` for unplaced literals (`new`).

These checks run against independent evidence:
- `site` vs `placed`.
- Two matched sites disagreeing on one symbol.
- `site` vs the address in the name.
- `site` vs the declaration comment.
- Our initialized data and literals vs the original bytes at the resolved address. Pointers inside data
  are checked too.
- Local relocations, e.g. jump tables, vs the original.

## Link inputs (for the full rebuild)
- **PE**: image base 0x400000, entry 0x5e0955 (LIBC `WinMainCRTStartup`, wincrt0.obj). No relocations
  (/FIXED). Section alignment 0x1000, file alignment 0x1000, subsystem Windows GUI 4.0, linker 6.00, no
  debug directory, checksum 0. Four sections, the default VC6 LINK layout:
  - `.text` 0x401000, 0x1e9cb4 bytes.
  - `.rdata` 0x5eb000, 0x5c7a bytes.
  - `.data` 0x5f1000: raw size 0x40000, virtual size 0xefae84.
  - `.rsrc` 0x14ec000, 0x508 bytes.
- **.text**:
  - 0x401000..0x5dd3df: game objects in link order. zlib/unzip are objects 0x4616d0..0x4687a0, in alphabetical
    order. The original pads between functions with 0x90.
  - 0x5dd3e0..0x5de44f: dinput.lib data (library, placed after every object file): the DIOBJECTDATAFORMAT
    arrays of c_dfDIKeyboard (256 x {&GUID_Key 0x5eee70, i, 0x8000000c | i << 8, 0}) and c_dfDIMouse (X / Y / Z
    axes, 4 buttons); byte-equal to dinput.lib members with the GUID relocations masked.
  - 0x5de450..0x5de491: `jmp [IAT]` thunks from dinput.lib/ddraw.lib (code calls DirectInputCreateA,
    DirectDrawCreate and DirectDrawEnumerateExA directly).
  - From 0x5de4a0: LIBC.LIB objects in the order LINK pulls them in. The order starts strrchr, fpinit, ftol,
    fprintf, _file, errmode, rand, sprintf, strstr, 87ctran, strncpy, atof, crt0dat, chkstk, ...; the full
    list comes from `uv run link --crt`.
  - Gaps the probe did not explain: 0x5df701 (0x344 bytes), 0x5e5b0f (0x336) and 0x5e7dc1 (0x86).
  - There is also one MSS32 `jmp [IAT]` thunk inside game code at 0x42c3a0 (`_AIL_get_timer_highest_delay@0`).
- **Identical COMDAT folding is on** (/OPT:ICF, or LINK 6's default with /OPT:REF):
  - FUN_004b70f0 (`ret`) is called from 4 places, including zlib's `_tr_init`, which calls its empty static
    `tr_static_init`.
  - Experiment (lt/e*.c): LINK 6 /OPT:ICF keeps the copy of the *first* object in link order, but it did not fold
    zlib's *static* `tr_static_init` into an identical external function linked first.
  - So how the original ended up with FUN_004b70f0 is open: maybe zlib was built with `local` defined empty, or
    the link order differs from the placement order.
  - Rule for sources: several C functions can share one address. Never give two names.csv rows the same address.
- **.rdata** 0x5eb000:
  - IAT first: 0x5eb000..0x5eb378, DLLs in ASCII order ADVAPI32, DDRAW, DINPUT, GDI32, KERNEL32, USER32, WINMM,
    WS2_32, mss32, ole32.
  - Then the game's float constants (`__real@`, in order of first reference) and the game objects' .rdata (zlib
    0x5ebc90..0x5ecde8).
  - Then library data: GUIDs IID_* at 0x5eeec0.. and CRT .rdata from about 0x5eef50.
  - Import directory at 0x5ef980. Its order is KERNEL32, USER32, GDI32, ADVAPI32, ole32, WINMM, WS2_32, DINPUT,
    mss32, DDRAW. That is the VC6 default project lib list `kernel32 user32 gdi32 ... advapi32 ... ole32 ...`
    (unused libs drop out), followed by `winmm ws2_32 dinput mss32 ddraw`.
  - Then the hint/name tables, up to 0x5f0c7a.
- **Imports**: KERNEL32, USER32, GDI32, ADVAPI32 and ole32 come from VC98/Lib. WINMM and WS2_32 also come from
  VC98/Lib; WS2_32 is imported by ordinal. DINPUT and DDRAW come from the DX6.1 SDK. **mss32.lib (Miles) is
  missing** from the toolchain; we need it or a .def for `LIB /DEF` (31 imports, read from the
  original exe's import directory).
- **.data** 0x5f1000:
  - The objects' initialized .data, in link order: game (unk_401d50 0x5f10f8 ...), zlib .data
    0x5fd1b8..0x5fe598, more game data, then CRT .data 0x62de88..about 0x630c88. The game's string literals are
    here (VC6 /O2 = /Gf: writable pooled literal COMDATs).
  - Then every object's .bss in link order, starting with unk_401000's `util_Line` at 0x630f50 (d3_tools
    0x6310e4, d3_snd 0x631130, ...); CRT .bss is at 0x6f2f9c..0x6f32fc.
  - Then the COMMON area, about 0x6f3300..0x14ebe84 (~14 MB), holding the tentative definitions `T x;`. LINK
    orders it in a hash-like order (experiment lt/t5.c), so source cannot steer it.
- **Resources**: one RT_GROUP_ICON 101 (32x32 and 16x16, 4 bit), language 1031. No version resource. A
  rebuild needs `101 ICON` in a .rc file; take the icon from orig/ at build time, since we commit no assets.
- **Relocations**: none in the exe. Inside our objects, only DIR32 and REL32 occur in code and data.

## Conventions the probe forces (worker-rule changes for the orchestrator)
1. **Run `uv run link` after `uv run build`**, and commit only when both exit 0. A conflict on a new symbol
   means a "matched" function references the wrong thing. The pre-commit hook runs `uv run link --nonmatching`:
   no conflict, no unplaced function (since 2026-10-25, no baseline).
2. **Globals: one name = one address.**
   - Declare each global once (extern in the owning TU's header), with `/* 0xADDR */` holding the address of the
     *symbol's first byte*, not of the member used.
   - A member access goes through the struct type, never through a second symbol at member + offset.
   - Address-in-name symbols (`d3_Unk6334a4`) must sit exactly at that address. Since `site` resolution
     checks both, a wrong comment fails the gate.
3. **Commutative operands can hide swaps.** `a + b` and `b + a` compile to the same bytes with the masked
   fields exchanged, and so do two calls whose targets are swapped. Only the link gate sees it. Several matched
   functions have exactly this: ch_Saved0/1, ch_Unk5f1294/8, di_Mouse/di_Keyboard, d3_Unk633480/63347c,
   d3_Unk1463570/1464fbd, d3_Unkfc2dc0/fe75b4.
4. **Switch jump tables are masked by `uv run build`.** `uv run link` compares them. evt_Statement's table is rotated:
   entry 0 goes to the last case block.
5. **String literals must have the original text.** The original's bytes are the reference, and the report
   prints ours next to the original.
   - The `D3DCHECK(call)` text is the stringized call, so it gives the original spelling of the call
     (e.g. `&d3d3` without a `(void **)` cast, `Snow->Texture->surftext...`).
   - Literals that differ (27, listed in report.txt): msx "Starting Track %s !", net "Send:"/"%02f",
     "shadow_fake", "textbin_%s\\%s.res", "Software\\Ahead Entertainment\\", and the WSA messages in net.c.
6. **`__FILE__`**: give the first `#line` of the file the original path with doubled backslashes:
   `#line 38 "D:\\develop\\Units\\d3\\d3_sky.c"`. Later bare `#line N` lines keep that file name. Paths are in
   config/symbols/tu_anchors.csv; note the case: `Dd_vesa.c`, `units\Msx`, `units\net`. Demo: d3_sky.c.
7. **Float constants must have the original bits.** `__real@` content is checked.
   - A double constant that is a widened float (`00000040e17a843f` = (double)0.01f, d3_tools) is written as a
     float, e.g. `(double)0.01f` or through a float variable.
   - Off-by-one-ulp constants (`57.29578f`-style values, 0.2f in unk_402140) need the exact literal.
8. **Statics and zero-initialized globals keep declaration order only with an initializer.**
   - `static T x;` goes to the object's .bss in a hash-like order.
   - `static T x = 0;` and `T x = 0;` / `= {0}` go to .bss in **declaration order** (aligned), and that is how the
     original's per-TU .bss blocks look.
   - Plain `T x;` (non-static, no initializer) is COMMON and lands in LINK's far area.
   - So: a global that sits in the TU's .bss block (0x630f50..0x6f2f9c) is defined in its TU with `= 0`, in
     address order. A global in the far area (>= 0x6f3300, e.g. ch_Fades 0x14eaa20, udp_Bcast 0x14eaae0) is a
     tentative definition `T x;`, and it must not be `static` (unk_401d50's `static struct sockaddr_in udp_Bcast`
     is wrong, which is why its .bss is not one block).
   - Demo: d3_snd.c, verified with data_ranges.txt ("block at 0x631130..0x6311f8").
9. **const data goes to .rdata**, and other initialized data to the object's .data in declaration order.
   - A literal the original keeps in .rdata, like `" "` and `"\""` at 0x5ebb58 that the agi strtok calls use, is
     not a plain /Gf literal. It is probably a `const char[]` or a /GF-compiled TU; open.
   - The original also keeps one literal text at several addresses (`" "` in .rdata and .data), so `uv run link`
     resolves literals and `__real@` per object.
   - VC6 puts the empty literal `""` in a .bss COMDAT.
10. **Initialized data defined in our sources is checked** against the original: `dd_LastDriver = -1`,
    `d3e_Resolve = d3e_DefaultResolve`, zlib tables. Define data in its TU only with the original values.
    Otherwise declare it `extern` with its address comment. Since 2026-10-23 every definition and extern needs
    its address comment (uv run gen -> build/state/data.csv), and the data ledger
    (`uv run ledger data`, notes/data.md) says which bytes of .rdata / .data are matched, drafted or
    undrafted, and which unit's block an undrafted run belongs to.

## Wrong symbols in "matched" functions (the round-12 baseline; all resolved, the baseline is gone since 2026-10-25)
Kept as history; build/link/report.txt had the details.
- **Swapped pairs**, see rule 3.
  - ch_Saved0/1 (ch_Update, ch_Reset).
  - ch_Unk5f1294/5f1298.
  - dm_L/dm_P/dm_S.
  - di_Keyboard/di_Mouse.
  - di_AcquireMouse/di_AcquireKeyboard: names.csv or the call sites are swapped.
  - evt_Include/Else/Return/Mode1/Mode2: names.csv names vs evt_Statement/evt_Compile calls.
- **strchr vs strrchr**: agi_Bitmap0 calls 0x5de4a0 (`strrchr`) where the source says strchr; txt_OpenText and
  vfs_FindDir are affected too.
- **Comment and address mismatches in the d3_engine/d3_d3d headers**: d3_ZFar, d3_ViewY, d3_ScaleY, d3_CenterY,
  d3_RadixDst, d3_HalfH, d3_Cfg (3 members via one symbol), d3d_UsedCard (3 addresses), d3_MaxZ, d3d_ViewY,
  d3d_Far, d3_Unkfe75b0, d3_Unkfb9220 and others. The 41 name/comment conflicts are in report.txt.
- The 32 matched functions whose linked bytes differ: ch_Update, ch_ActMove2Universe, ch_ActFade, ch_Reset,
  di_* (10), mem_Alloc, evt_TypeOf, evt_GetToken, evt_Declare, evt_Statement, txt_ParseLine, txt_LoadIncludes,
  vfs_Unk42f4d0, vfs_MakeDirs, d3d_Init, d3d_CreateDevice, d3d_SetConfig, d3d_SetWBuffer, d3d_CreateTexture,
  d3_Unk4385a0, d3_Unk4387b0, d3_Unk43b080, d3_Init.

## Open
- (done 2026-10-25) The baseline is resolved and removed; the hybrid places every function and the link is a
  pre-commit gate.
- How the ICF survivor ended up at FUN_004b70f0, and the three unexplained CRT gaps.
- mss32.lib for a real link.
- A full LINK.EXE rebuild needs every TU. `--lnk` already shows that LINK reproduces a TU group's layout from
  our objects.

## Link baseline fixes (worker "lfix", round 13)

### Report
- **Baseline 110 -> 9 entries**, all caused by one thing outside my scope: `bgi_Bitmap0/1/2` in
  `src/game/d3_io_b.c` (worker d3io4) call `strchr` where the original calls `strrchr` (0x5de4a0), at lines
  127/140/153 (`dot = strchr(buf, '.')`). That makes `_strchr` conflicting, and the 8 matched functions that
  really call strchr (mem_Alloc, evt_TypeOf, evt_GetToken, evt_Declare, txt_ParseLine, txt_LoadIncludes,
  vfs_Unk42f4d0, vfs_MakeDirs) keep the original bytes. Checked locally: with those three calls changed to
  `strrchr`, `uv run link` reports 0 conflicts. The orchestrator / d3io4 should make that change and empty the
  baseline.
- `uv run build`: 1024/1173 matched before and after, exit 0 throughout. Linked (matched only): 1016 functions written
  in place (was 992), 0 bytes differ from the original.
- No function scope, no names.csv rows added (two swapped, see below).

### Fixes
- evt_Statement: case blocks in the original order `1, 2, 4, 5, 6, 8, 7, 3` (the jump table was rotated);
  the evt_Include/Else/Return/Mode1/Mode2 conflicts were only the block positions. names.csv was right.
- agi_Bitmap0/1/2 (d3_io_a.c): `strrchr`.
- di_input.c: names.csv rows 0x410710/0x410740 swapped (0x410710 is di_AcquireMouse, as header and source
  already said). di_SetMousePos stores dm_P first and copies to dm_S, dm_L. di_PollMouse left-button press:
  `if (dm_L.rhold == 0) dm_S.lclick = 1;` (was lclick / lextra). reg_OpenKey literal `"Software\\Ahead ..."`.
  di_LClickTime is in .bss (0x6310dc): `= 0`, not `= -1`.
- ch_Reset: `ch_Unk5f1298 = ch_Unk5f1294 = -1; ch_Saved1 = ch_Saved0 = 0;` (store order).
- Floats: `57.295776f` (0x42652ee0, ch/chc), `0.19999999f` (0x3e4ccccc, ch_WalkRotate only; ch_ActWalkCells
  keeps 0.2f), `(double)0.01f` (d3t_UpdateSunRays).
- `__FILE__`: `#line 1 "D:\\develop\\..."` as the first line of d3_tools, dd_vesa, msx, net, d3_d3d, d3_interface,
  ts_texture (paths from tu_anchors.csv). With `#line 1` on physical line 1 all later line numbers stay the same.
- Literal texts from the original: msx "... !", net WSA messages (WSAECONNRESET is much longer), "Send:",
  "%u. Recv ... %02f", txt `textbin_%s\\%s.res`, d3_engine "shadow_fake".
- D3DCHECK texts: d3t_DrawSnow uses `Snow->Texture->surftext` (local renamed `Snow`, D3TSNOW field `Texture`);
  d3_sky `l->t->surftext`; dd_vesa QueryInterface without `(void **)` casts. **Shared header change:**
  `D3TEXHANDLE` (d3.h) +0x64 is now an anonymous union `d3dtex` / `surftext` (layout unchanged, old name works).
- d3_Init: `d3_Cfg.viewW + d3_Cfg.viewX` / `viewH + viewY` (4 sums), `d3_Unk1463570 + d3_Unk1463580 +
  d3_Unk1464fbd`. Here the source order of a commutative sum of globals *does* decide the load order.
- d3_Render: `d3_Unk633480 = d3_Unkfc2dc0;` before `d3_Unk63347c = d3_Unkfe75b4;` (VC6 emits the stores
  in the opposite order of the source).
- **Shared header change:** D3DCARDINFO (d3_d3d.h): 0x18/0x1c are trans_destblend/trans_srcblend, 0x20/0x24
  glow_destblend/glow_srcblend (names from the original D3DCHECK texts). This fixed d3d_Init, d3d_CreateDevice,
  d3d_SetConfig, d3d_SetWBuffer, d3d_CreateTexture, d3_Unk4385a0, d3_Unk4387b0, d3_Unk43b080, d3_Init.
- Headers d3_engine.h, d3_d3d.h, d3_fl.h, d3_interface.h, dd_vesa.h, ecmd.h, sndc.h, ch.h, di_input.c: every
  `int a, b; /* 0x.., 0x.. */` split into one declaration per comment (addresses from build/link/symbols.csv).
- d3_sm.c: `d3_Unkfe75fc/fe7604/fc5348/fc2944` were second names of d3_CenterX/Y and d3_ScaleX/Y; now uses those.

### `--nonmatching` only
- Fixed: di_LClickTime, d3_Unk633490 / d3_Unk6334b4 / d3if_Unkfc5340 (comments), d3_Unkfc2944 / d3_Unkfe7604.
- Left: 6 "linked bytes differ" for matched callers of functions moved to `.hybrid` (d3_AddChild ->
  d3_AppendSibling, d3_Render -> d3_DrawObjectCB, d3_Unk454be0 -> d3_Unk454890, sb_LoadSampleBank /
  sb_PlayVariation -> sb_LoadEntry, tex_Animate -> tex_GetFrame). Not a source error: inside one object,
  `Linker.target()` resolves a call to the .hybrid copy instead of the original address (where emit puts a `jmp`).
  Proposed uv run link fix (I was not allowed to edit tools/): remember each section's original address in
  `emit` and, in `target`, return it for a reference to offset 0 of another placed section. With
  `--skip` of the 14 moved functions the run has no new conflicts.
- Also noticed: `Image.off()` does not stop at a section's raw size, so for .bss addresses check_data compares
  with bytes of the next section (that is where "orig 01000400" for di_LClickTime came from).
- Skipped functions for unresolved symbols (mpool_Get, _itoa, d3_PtEq, d3_TriNormal, ...) are not conflicts.

### Lessons
- In the Git Bash heredoc, `\\` in Python source reached the file as `\`; write scripts with backslashes as files.
- d3d_TexRMask/GMask/BMask (0xfe79fc..) are members of d3d_UsedTexFormat (0xfe79c0 + 0x3c..) reached through
  second symbols (rule 8b); not a link conflict today, left for the d3_fl owner.

## Open work
- mss32.lib is missing from the toolchain for a real link (notes/link.md). Hybrid runs (2026-10-21, after the
  round-12 close): Smart App Control is off (user), so freshly linked exes start; in window mode the menu is drawn
  twice side by side with a green tint by the original exe too (window-mode quirk on this desktop, not our code).

## `uv run dbg --build` and the map (2026-10-28)

`dbg` resolved every `--bp` / `--watch` / `--read` through `build/link/full/full.map` *before* running the
`--build` step, so one invocation compiled, relinked (which moves every address) and then put INT3s at the old
addresses: in the tutorial run a breakpoint landed 0x20 bytes into the wrong function and the process crashed
with a wild `eip`. The build now runs before the map is loaded (`tools/gildegold/game/dbg.py` `main`).
