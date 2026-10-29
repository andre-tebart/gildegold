# Data sections (since 2026-10-23)

The data of the exe (.rdata, .data with its zero tail) is tracked like the code: the sources are the state, the
ledger computes everything else (`uv run ledger data`, rules D1..D7 in `uv run ledger_data --help`).

## Typed state
- Every data definition and every `extern` declaration carries its exe address: `int x = -1;  /* 0x5ffd1c */`,
  `extern T y;  /* 0x6f0e28 */`, a `/* 0x5ecf60, what it is */` line above a multi-line table,
  `extern a, b;  /* 0xA, 0xB */` one address per declarator, `/* unlinked, why */` for data the exe lacks.
  `uv run gen` parses them (every .c and .h) and writes `build/state/data.csv` (address, name, kind
  def / common / extern, source, size from the object, aliases); `uv run build` regenerates it, the
  gate (`uv run ledger check`) refuses a stale one, a definition without an address, two addresses
  for one name, two definitions at one address.
- Two names at one address (an `extern` in another unit under a different name) are *aliases*: tolerated,
  listed in data.csv, warned about by the gate. Rule 2 of notes/link.md still holds: one name = one address;
  rename towards the defining unit's name when matching touches the code.

## Evidence (computed)
- `uv run link resolve_all`: the original bytes at the relocation sites of the matched functions give the
  address of every symbol they use (`site`); literals, float constants and tables are found by content; a
  comment or an address in the name is only a claim. A symbol with contradicting evidence is not located.
- A plain section of our object (.data / .rdata / .bss) is *placed* when its located symbols agree on one
  base: then every symbol of it has an address (base + offset). Matched (D1) needs independent evidence for
  the base and equal bytes; drafted (D2) otherwise. Tentative definitions (`T x;`, COMMON) are placed one
  by one.
- The CRT's data is placed through LIBC.LIB's matched code (D3), the import tables come from the PE
  directories (D4).

## The data order as boundary evidence (D8, `uv run ledger data-order [ADDR]`)
Literals and float constants are pooled at their first user in link order, so a reference to an address among
unit X's literals (X's extent outside its own globals block) puts the referencing function in X or a later
unit: proven when X's block is matched, plausible when drafted. The gate reports drafted functions that
contradict it. For undrafted code the report lists each function start with the literal addresses it reaches
first; a big forward jump skips a globals block and marks the next unit (the he.c seam at 0x577740 ->
0x629304 shows exactly so). Globals carry no such evidence (any unit references them); the COMMON area none.
The evidence grows with every located block, so the match sessions' data work feeds the layout sessions.

## The layout audit (`uv run ledger data-layout`)
The byte gates cannot see a wrong *shape* when the bytes happen to agree (zero data always does). This report
walks `build/state/data.csv` in address order and prints every definition's declared size and the gap/overlap
to the next one; the alias table prints each alias's declared type size next to the definition size. It flags:
- **overlap**: the declared size runs into the next symbol (an overlap in zero data is invisible to a byte diff).
- **alias-wider**: an alias's declared type is larger than the definition (e.g. a 4-byte `ai_Unk1460ae0` with
  a 0x14-byte `net_Conn` alias: the struct's `closed` field then lands on the next global).
- **blob-touch**: a placeholder byte blob (`char[]`) ends exactly where a named global starts; the sentinel /
  member shape (`d3_Unkfc2e00` + `d3_StockListHead`, `d3_Unkfc2580` + `d3_AnimListHead`). The next global is a
  field of the blob, not a second object at member+offset.

`--suspects` prints only the flagged rows, `--aliases` only the alias table, `--around ADDR [--span N]` a
window, `--no-types` skips clang (overlaps only), `--strict` exits 1 on an overlap or an alias wider than its
definition. `uv run build` and `uv run ledger check` print the cheap counts (no clang) as a one-line `layout:`
summary, so the suspects are not buried in the 105 data warnings; those calls add ~3 ms. The declared sizes are
cached in `build/ledger_data_declared.json`: a warm `data-layout` is ~0.15 s, a cold one (after a source/header
edit, which invalidates the cache) ~2.7 s, and `--no-types` ~0.14 s.

## How to work on it
1. `uv run ledger data-next` (or `data-undrafted [--area=rdata|data|bss|common]`): an undrafted run
   with the externs declared inside it and the addresses the original code points at.
2. Define the symbol in its unit (the extent tells which unit, or which units it may belong to) with the
   original value: `T x = <value>;` for initialized data, `T x = 0;` / `= {0}` for a unit's .bss block,
   plain `T x;` only for the COMMON area (>= the CRT's .bss end), `static` only when the original keeps it in
   the unit's block (notes/link.md rule 8). Keep the address comment; drop the `extern` elsewhere or make it
   an alias to retire later.
3. `uv run build`, then `uv run link` (no new conflicts) and `uv run ledger
   check`. The ledger says whether the block is matched, drafted (why), or out of order (an error: the
   original lays the sections out in code order).
4. Open debt the gate reports as warnings: `not one block` sections (d2.c .data / .bss, d3_tools.c .bss,
   udp.c), `bytes differ` (d3_io_b.c bgi tables, 3 bytes), tentative definitions that belong in a .bss block
   (di_input.c, dd_vesa.c: write `= 0`), member aliases (externs pointing inside dm_L, kb_Down, dm_P).

## The full-rebuild work list: `tools/data_defs.py` (since 2026-10-27)
`uv run link --full` names the globals the exe references and no object defines (build/link/full/unresolved-data.txt,
1524 at the start of the session). `uv run python tools/data_defs.py` defines them from the address each one
already carries:
  - the area from the ledger's borders: COMMON (>= the CRT's .bss end) `T x;`, a unit's .bss `T x = 0;` /
    `= {0}`, `.data`/`.rdata` are reported only (they need the original value);
  - the owner: the unit of the header that declares the symbol (include/cm.h -> cm.c), else the declaring
    `src/**` file, else the unit that names it most; a single-name `extern` in the owner becomes the definition
    in place, anything else gets a line in address order among the unit's definitions of that area;
  - the type of the declaration that shows the most of the object: `extern char ai_GfxDetail;` (a byte view)
    loses to `extern OPTIONBYTES ai_GfxDetail;` (the dword the writer stores). An open array (`T x[]`) is
    sized from the gap to the next declared address only when that gap is a plausible buffer (16 B .. 0x200 B);
    otherwise it is left alone -- VC6 drops a tentative definition of an incomplete array (no symbol at all).
  `--area`, `--unit`, `--limit`, `--write`, `--skip NAME|@FILE`. Re-run it after every `uv run link --full`.
  `config/data_defs_skip.txt` holds the 128 symbols left `extern` for a human, in three groups: member aliases
  (the address lies inside another symbol), definitions that change matched code, and wrong declared sizes.

### What it taught us (2026-10-27)
- **A definition in a TU changes that TU's code.** With `unsigned char d3d_alphatest = 0;` in d3_d3d.c, VC6
  loads it as a dword and masks (`mov ecx,[x]; and ecx,0xff`); the original `d3_SetBlendState` loads a byte
  (`xor ecx,ecx; mov cl,[x]`). So a symbol the original defined in unit U must keep the *extern* form in every
  other unit -- and when defining it in U breaks matched code in U, U is not the owner. That is an ownership
  oracle (`uv run build` prints `REGR`), and it is why 2 matched functions and 2 near matches are in the skip
  list. Big sized arrays (`txt_Keys`, `txt_GroupFirst`) show the same effect: revert txt.c's definitions and
  txt_OpenText drops back from 1498 to 1463 differing lines.
- **The COMMON area is LINK's own order.** A three-object experiment (tentative definitions only, linked a b c:
  the addresses come out zc1 < zc2 < zb1 < zb2 < za1 < za2) shows VC6 allocating the `T x;` records in *reverse
  object order*, and in declaration order within one object. It is therefore not the code order, and a full
  rebuild will not reproduce the original's COMMON addresses from our link order alone -- that needs its own
  pass against the full-link map. `uv run link --full` writes `build/link/full/full.map`.
- **`.bss` extents are contiguous per unit in link order**, so an address tells the owner (D7) -- but the
  declaration site is a poor proxy for it (`d3_LightKeysCulled` is declared in d3_vec.c and sits in an earlier
  unit's block), and tiny globals are packed with byte flags that make a `T x[4]` declaration wrong
  (`d3_MipSkip` is one byte). 28 `not one block` warnings in the `.bss` blocks are that debt, open.
- A written file must be rebuilt from scratch (`rm -rf build/obj`): one CL.EXE run per flags group means a
  group that fails writes no `.flags` stamps for the files that did compile, so those objects keep their old
  stamp and `uv run build` calls them up to date.

### The second pass: `.data`/`.rdata` values and the last 604 (2026-10-27, `data-globals` branch)
- **`tools/data_values.py`** renders the initializer from the original image at the address comment:
  scalars/arrays of ints, floats (`%.9g` + `f`, `%.17g` for doubles), clean C strings for char arrays,
  pointer words turned back into their symbol (`&name`, `name` for an array, the function name for a
  function pointer; an address with no symbol becomes `(void *)0x...`).  A struct is laid out from the
  headers' own field lists; the parser matches braces rather than a regex, so anonymous `union { ... }`
  members count (`D2SEG` 268, `D3CVERT` 80, `D2FORM2` 4124, `D2WIN` 964 -- the old regex truncated them and
  a 4-byte guess made `d2_Forms0[91104]`, 770 MB of `.bss`).
- **Open-array evidence.** `.bss`/COMMON: the gap to the next declared address (the original's own layout)
  floored to whole elements; VC6 drops `T x[];` (no symbol), so the definition must carry a size.  A
  terminator record ends `G15TREE` (`veg_Types`), `G16TRACKS` (`ai_Unk602d70`), `G16LOC`
  (`snd_LocationTracks`).  Six counts the gap cannot settle live in `OPEN_COUNT` in `data_defs.py`
  (`ai_Chars`/`ai_CharInts` 768, `ai_PlayerSlots` 4, `ai_Unkf8f13c` 1, `cm_SaveAcks` 8,
  `gm_PlayerRecords` 688).
- **`D2KEY` is not packed.** `shp.h` had it inside `#pragma pack(push, 1)`, which makes it 3 bytes; the
  original strides 4 (0x5fed74 -> 0x5fed78) and the 24-bit marker colours compare the same three bytes
  either way.  Moving `pack(pop)` above `D2KEY` makes the definition emit `{0, 0xff}` as
  `00 00 ff 00` and moved all five `d2_GrabShape*` closer (`-3 .. -18` asm) at no cost.
- **Literal views are the remaining overlap debt.** 400+ named symbols (`ai_Unk5eb798`, `amt_Pay`,
  `d3_ScrollSpeed`, ...) sit on bytes the original implements as pooled `__real@`/string literals.  A
  definition resolves the full link but lands in the ledger as an overlap with the matched literal; the
  real fix is to use the literal at the call sites.  The full link shows them as `unresolved-data.txt` only
  because our source spells the named extern.
- **The ownership oracle is a regression, by decision.** With the hook off and regressions allowed until
  the full link, the last 125 skip-list symbols were defined too; three matched functions fell
  (`d3_SetBlendState`, `ai_MasterUpdate`, `ahm_AddPlace`).  Moving those three definitions to the owner the
  oracle points at restores them.
- Result: `uv run link --full` data globals **604 -> 0**; `--force` links the whole exe (the 182 "other"
  remain); `.data` 0x3d53c, `.bss` 0x00fbd7f4 against the original's 0xefae84 `.data` section.

### The aliases at the link (2026-10-27, `data-globals` branch)
- **One address, several names.** `data.csv` records every alias of a data symbol (`ch_List` at the
  `ai_Unk1468ae0` address, `d3_ZAxis` / `d3fl_Down` at `ai_Unk5ebb74`, ...).  The sources use each view's
  type, so unifying the *text* would either duplicate bytes or break a type view.  The full link instead
  emits one COFF object with a **weak external** per alias (`storage class 105`, aux
  `IMAGE_WEAK_EXTERN_SEARCH_ALIAS = 3`): LINK resolves `_ch_List` to `_ai_Unk1468ae0` and the object keeps
  its own typed declaration.  `write_alias_object` in `tools/gildegold/link/link.py` generates it from
  `data.csv`; 118 aliases resolved this way.
- **CRT internals.** `uv run link --crt` maps each CRT-region start to its LIBC object/symbol
  (`0x5de652 -> _sprintf`, `0x5de6b0 -> _strstr`, `0x5eaa30 -> __strcmpi`, ...).  `CRT_ALIASES` points the
  stale `ai_Unk5de...` / `crt_*` names at those symbols with the same weak-external mechanism (18 names),
  so the call sites' bytes are untouched.  `__alloca_probe` is the exception: the decompiled duplicate call
  is removed, the compiler emits the probe.
- **Stale function names** are found by `declared_addresses`: a small lexer walks top-level statements
  (brace depth, comments, strings) and reads the address comment after a prototype.  The earlier
  name-only `unk_<addr>` test missed `cfg_Open`, `snd_Play`, `d3_SelectScene`, ...
- **`MSS_auto_cleanup`** (0x5de460) is mss32 *static* code, not an import and not in the generated lib; the
  full link compiles a stub in `build/link/full/stubs.c` until the static mss32 library is available.
