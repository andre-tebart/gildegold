# Data, link and layout notes

For the gapdata, layout and base passes: where data lies, what one object is, how the linker orders things, what
a unit boundary looks like. Not needed to match a function's code (that is `notes/vc6-quick.md` and
`notes/vc6-matching.md`).

- **A byte of a global `union { int word; unsigned char byte[4]; }` loaded as the dword** (`mov ecx, dword [U];
  xor eax,eax; mov al, ch`, a later `cmp byte [g], cl` reusing it) where the original does `xor eax,eax; mov al,
  byte ptr [U+1]`: the bytes were separate globals. Char-pointer views, casts and bitfields of the union give the
  same dword load; only a distinct object fixes it (probe: `extern unsigned char probe;` inside the function in a
  `dcp try`). An aligned unsigned char global can still load as `mov ecx, dword; and ecx, 0xff` (ai_SaveOptions'
  `_itoa(b, ..)`), so a `.word & 0xff` spelling elsewhere is no evidence for the union. ai_ApplyGfxOptions byte
  (the ai_GfxFloor split). New externs in menu.h moved sv_t's tutorial chapters off byte: declare in the users.
- One name = one address; `/* 0xADDR */` per declaration; reach struct members through the type.
- Original string / float text and bits; `__FILE__` via the file's first `#line N "D:\\develop\\..."`.
- .bss statics / globals need `= 0` to keep declaration order; globals in the original's common block (past the
  last LIBC .bss symbol, gildegold 0x6f3300) are tentative `T x;` - link.exe allocates commons after every .bss
  contribution, the libraries' included, and dcp's D5 accepts a tentative definition there.
- A unit's .rdata is its named constants, then the literals it is the first to use (COMDATs, folded across units):
  the bytes right after a unit's last constant are its literals. bio, first in link order, holds the shared ones
  (255.0f after ai_FloatOne). A constant the code loads from memory is another unit's `const` read through an
  extern (ai_CardBack in unk_4a7140), not a header `static const volatile`.
- Never `&X + N` past X: a constant of another object is its literal (`80.0f`), a field is reached through its
  pointer (`d2_LastWindowPtr->w`), several globals one memset clears are one object (EVTGLOBALS, int[4]), flag
  bytes after a byte are objects of their own, an int at an odd address is a packed struct's field.
- A memset over several globals rewritten as single stores costs the byte match: declare the block as one object.
- **A global the original reloads after an inline `strcpy` into another global** (ours keeps the value in eax and
  the copy loop takes other registers; then a whole callee-saved cascade, constants 0 / 1 not in registers): the
  two globals are one object; VC6 forgets every field of the object the copied bytes land in. Probe it with
  `strcpy((char *)&rec + off, src)` in `dcp try` (byte at once for sv_SelectPanelObject, 86 ins -> 0), then declare
  the record (sv_QuickJump: object, record, two unread ints, contactName).
- **Pooled literals**: all three `1.0f`s of d3s_AddShadow were the shared ai_FloatOne, but the M10 named only the
  first two until those were written as the object; check the whole function after each M10 fix.
- **`shl eax,2` shared by two loads `[eax+A-4]` / `[eax+A]`** where ours CSEs the address `lea r,[e*4+A]` and
  reads `[e*4+A-4]`: a negative displacement off the symbol. The object before T (at A-4) and T were one record
  in the original: declare it (`SVCLIMATE { int seasonClimate; SVWEATHER weather; } sv_Climate`, the old names as
  field macros in the header) and the plain `T.a[h - 1]` matches (every spelling on T's own base = control).
  Do not read the neighbour through a cast of the other global: our link pads separate objects (the 0x188-byte
  record went to +8), dcp session wrap flags it as M.layout. sv_WeatherFrame 11 ins -> byte; the header change
  also took sv_t's ai_TutorialInitChapter3 to byte (tie-break).
- The `1.0f` of an M10 `open:` line naming ai_FloatOne is worth trying first: reading the datum changes the
  scheduling around it (ai_ScatterShot 18 -> 8 ins with the copy moved).
- **Shared float object vs literal moves unrelated tie-breaks**: `ai_FloatZero` / `ai_FloatOne` against `0.0f` /
  `1.0f` at one site flipped the order of two register reloads at a loop latch 1400 bytes away (d3sndw_Update,
  8 combinations: 3 keep it, 5 flip it). Try every combination of the function's pooled constants before
  spending variants on the hunk itself; `dcp try` rows with the same `ins` can still be different hunks.
- `1.0f / x` against `ai_FloatOne / x`: dcp's M10 says the original reads ai_FloatOne (0x5eb780), but that is the
  address of the pooled literal __real@3f800000 - the literal gives byte, the extern const gives other code
  (d3anim_PoseBones). Write the literal unless the function needs the object (a read the literal would fold).
- **A short global the original loads as a word where ours loads a dword and widens the arithmetic** (`mov ecx,
  dword [g]; sub ecx,eax` vs `mov cx, word [g]; sub cx,ax`): VC6 widens only when it knows the global is 4-aligned
  (its unit defines it). Probe with `extern short probe;` in a dcp try variant. A tentative `short g;` (COMMON)
  gives the original's code. Settled: when the global lies in the common block (past LIBC's last .bss symbol)
  the original's definition WAS tentative - write `short g;` (D5 accepts it there). d2_ResizeWindow,
  gm_BuildKindObjects byte. Below the common block the original defined it in some unit's .bss: look for the
  owner whose code does not touch it (an extern in the user unit).
- **The colour keys were 3-byte objects.** d2_TransparentColour (0x6f0454) and the marker keys (0x5fed74 /
  0x5fed78): no code of the original touches key+3 (refs to +0..+2 only). With the 4-byte D2KEY (r, gb, pad) VC6
  treats the object as a dword container and reads r + g with one `mov eax, dword [key]` (`mov cl, ah`) and b as a
  byte; with `{ unsigned char r; unsigned short gb; }` under pack 1 (3 bytes) it loads gb as the original's `mov ax,
  word [key+1]` and r as a byte. The earlier reading ("VC6 splits a word at an odd offset of its symbol") was the
  container effect, not the offset. d2_GrabShape16NoRuns, d2_GrabShape24NoRuns, d2_GrabShape16Runs byte; the key
  loads of d2_GrabShape8Runs / 24Runs match (their other hunks remain). Owner moved f3gm -> shp (between shp's
  0x6f0450 and d2's 0x6f0458). The type is D2COLKEY in shp.h: changing D2KEY in d2_types.h (the 3-byte body, a
  forward typedef, or removing it) moved sv_t's ai_TutorialInitChapter2 / 3 off byte (tie-break on that header's
  type set), so the old 4-byte D2KEY stays there unused with a comment saying why; f3gm declares the key as an
  extern of the incomplete `struct D2COLKEY` (it only takes its address).
- **Where the commons are.** A VC6 link of `int a = 0; int c;` in two objects plus LIBC (scratch experiment, map
  file): .bss is a.obj's, b.obj's, then LIBC's contributions (crt0dat, nlsdata2, ..), and only then every common
  symbol as `<common>` (the program's first, in encounter order, then the CRT's __crtheap, __acmdln, ..). In
  GildeGold LIBC's last .bss symbol ends at 0x6f3300: everything above (0x6f3300..0x14ebe84) is common data, i.e.
  tentative definitions in the original (the d2_ / d3_ / ts_ names interleaved there follow the first object's
  header order). Below it the units' `= 0` definitions are right.
- **The short globals were commons.** d2_ScreenH / d2_ScreenW (0xfb9070 / 0xfb9072) and gm_KindObjCount
  (0x76fdc4) lie in the common block; the original's code (word registers, `mov cx, word [g]; sub cx, ax`) is what
  VC6 makes when it does not know the global's place, i.e. a tentative definition. Written `short d2_ScreenH;` /
  `unsigned short gm_KindObjCount;`: d2_ResizeWindow and gm_BuildKindObjects byte, nothing else moved. dcp's D5
  (decompanion fedde79) now fires only below the common block (the end of the last crt.data symbol in
  uninitialised data) and names the block's start in its message.
- Not done: the other ~2000 `= 0` definitions above 0x6f3300 are commons in the original too; converting them is
  codegen-neutral except where a function's code depends on knowing the placement (the short-global widening
  above). Convert one when a stuck function shows that symptom.
- Gm_BuildKindObjects (stuck): `extern unsigned short probe;` in place of gm_KindObjCount is byte at once: VC6
  keeps a short global in a word register through a call-free loop only when the unit does not define it (the
  d2_ScreenH question again; settled: a common, tentative definition - byte).
- **A short global loaded as a dword (`mov ecx, dword [d2_ScreenH]; .. sub ecx,eax; mov [m],cx`) where the original
  keeps `mov cx, word [..]; sub cx,ax`**: VC6 widens a 16-bit load-op-store chain to 32 bits only when it knows
  the global is 4-aligned, i.e. when the unit defines it (`short d2_ScreenH = 0;`). Probe: `extern short probeH;`
  inside the function in a dcp try variant gives the original's word load, and so does a tentative definition
  (`short d2_ScreenH;`, COMMON: byte, nothing else moved) - but dcp's D5 gate refuses a tentative definition
  at an uninitialised-data address ("write it `= 0`"), so it was reverted. Moving the `= 0` definitions below
  the function changes nothing (C2 sees the whole unit). Settled (gapdata): the globals lie in the
  original's common block, so their definition was tentative; D5 now accepts that.
- **Same-GUID symbols of a library**: strmiids.lib defines CLSID_FilterGraph and CLSID_FilgraphManager with
  one value; an M8 naming the second means the original wrote that name (mov_Open). Same for the `""`
  window title: the original passes ch_EmptyName (M9), as other units do.
- A non-const global holding 0.0f (`ai_FloatZero`, M10 "the original refers to the datum") instead of the
  literal also moved the schedule elsewhere in d3sndw_Update (a latch's reload order): write the object
  when M10 names it, even where the value is the same.
- **A global 2D array whose row stride is not element_size * count needs a block struct.** `init_StockBlocks` is
  0x2a90 bytes per player (0x10 header ints then 0x55 records of 0x80), so `G12STOCK [][0x55]` (stride 0x2a80)
  reads the wrong player's records and compiles to a different address chain. Declare
  `typedef struct { G12STOCK rec[0x55]; char pad[0x10]; } G12STOCKBLOCK;` - the records first keep the symbol
  2 at 0x7d6a70, the tail pad gives the 0x2a90 stride, and the address chain then matches the original
  (`0x7d6a70 + player * 0x2a90 + i * 0x80`): 330 -> 275 differing instructions.
- **Q7 with a shared K&R declaration**: when the other units call the callee with an int address literal
  (`gm_MinutesBetween(rec, 0x770ae0)` in he.c / sv.c / gv.c) the shared header cannot carry a prototype (each
  TU would be a lint cross-TU conflict), and a prototype in one .c is one too. The exemption tag must sit on the
  *call line itself*: `x = f(a, b);  /* @original: ... */` clears Q7, a comment block above the line does not.
- **An invalid escape drops the backslash silently and shortens the literal**: `"misc\Messagebox"` written
  with one backslash is `"miscMessagebox"`, and `"\game.INI"` is the 9-byte `"game.INI"` (VC6 copies dword +
  dword + byte) where `"\\game.INI"` copies 10 bytes (dword + dword + word). The function still matches
  (relocations are masked) but the string data at 0x60ea68 differs and `link --nonmatching` reports the
  `??_C@...` symbol as a data conflict; check a strcpy hunk at the inlined copy before suspecting the source
  structure. Write `\\` in the C source.
- **A stale alias rename must carry the canonical signature into every TU**: renaming ai_Unk4fe140 ->
  gm_AddTime (424 refs) left unk_5d33b0.c's `int gm_AddTime();` (K&R, int return) against the prototype -
  lint's cross-TU X conflict (every conflict is an error, so ledger check failed at the commit). Fill in the real
  prototype (with the file-scope `struct GCLOCK;`) instead of parking it.
- **`@deep` reasons are parsed by Q5**: a reason containing the word ROUGH makes the function fail the
  draft-marker rule (readability fell readable -> draft). Word the escalation reasons around the markers.
- **A hardcoded guard is not an array end**: d2_OpenForm's view scan compares to the literal 0xfb91e4
  (0x474376), which is not d2_Views + 0x30 (0xfb6060). A #define with the address and the instruction
  in its comment keeps the immediate and passes Q3 (same lesson as the 0x50d490 end marker).
- **A symbol in a loop bound is a relocation where the original has an immediate**: `(int)arr + 0x40 *
  0xf8` in a `while (p < ...)` made the linker see two disagreeing sites for the array symbol and the
  masked field differ; a `#define` with the numeric end keeps the immediate and passes Q3 (0x50d490).
- Q2's placeholder regex flags `unkNN` *field* names used in a body, so an unknown field cannot be
  typed without a meaning; keep one raw access and escalate the field-name gap.
- Two stores of the same value to two different globals can be emitted in either order; VC6's choice is not
  controllable from the C (every source order, chained assignment, types, names and declaration order gave the same
  order here). The masked byte compare masks exactly those addresses, so such a function reports "matched" while
  `dcp link`'s site inference sees the two relocations swapped ("matched sites disagree" for both symbols). A struct
  view of the two adjacent words (G16TRACKHANDLES { current; copy; }) is the fix: the stores then relocate to one
  base with +4 / +0 addends and both sites agree on the base whatever the order.
- The d2 object table 0xf8f080 is a POINTER global (d2_Objs), not an array: `U_I(0xf8f080 + off + id * 0x2ec)`
  compiles to an absolute store with a `mov eax, ds:0xf8f080` that the original does not have; write
  `U_I(U_I(0xf8f080) + off + id * 0x2ec)` (0x5d6f40 matched with it; the same holds for every control-record
  access of the front-end windows). Caching it in a local pointer (`char *obj = (char *)U_I(0xf8f080);`) gives one
  load for many accesses, which is what a window with several fields does.
- A pointer built from a symbol plus a bias is not folded: `char *p =
  (char *)externdata + 2;` emits `mov reg,addr; add reg,2` where a symbol declared at the biased address loads
  with one `mov reg,addr`. ls.c's gm_PlayerRecordBytes (0x7d5f82, the +2 byte view of gm_PlayerRecords) was
  needed to keep 0x51a1a0 and 0x51db00 at their original shape.
- Q6 counts an address comment above a prototype *in any file*: vfs_fwrite (0x430e30) was readable
  through ls.c's foreign `/* 0x430e30, the file read / write ... */`; removing that declaration (the
  right call, ls.c now includes vfs.h) dropped it to draft until include/vfs.h described the
  prototype. When a foreign declaration goes, move its description to the defining unit's header.
- Renaming a function that is declared differently in several files (the X lines of dcp lint) must
  make every declaration agree in the same commit, or the pre-commit ledger gate fails with "declared
  differently here and elsewhere" (every X conflict is an error; nothing is parked).
- A function whose compiled section has NO trailing alignment nops (the exe shows the linker's 0xcc right after its
  last byte, while every /O2 function carries 0x90 nops inside its own section) was compiled with optimization off or for size:
  `#pragma optimize("", off)` (or `("s", on)`, same bytes) around the function reproduces it. Found on the two big inline-asm
  blitters d2_Unk47d210 / d2_Unk47d710; the 0xcc there is therefore not object-boundary evidence.
- Before inventing a name for a placeholder callee, check the OTHER units' headers for a named declaration of the same address: d3_Unk44c9d0 was already `d3io_LodName` in d3_int_io.h and d3_Unk5fbf78 already `d3io_Lod`; replacing the stale aliases with the owner's names fixed Q2 without a guess (and removed duplicate externs).
- Two callbacks with one address stored in one record without a register CSE (`mov [x],imm` twice): two
  identical functions folded by the linker (/OPT:ICF), not one symbol (0x4a16f0, unresolved).
- Linker-folded functions (ICF, e.g. a `xor eax,eax; ret` stub at 0x52a040): two names folded to one address give
  two `mov [..],imm` stores; one name assigned twice gives one `mov eax,imm` + register stores.
- TU evidence from .data: each obj has one plain .data block of its initialised globals, then its string
  literals as COMDATs in function order; /O2 implies /GF, so identical strings are pooled across TUs. A new
  non-string block marks a new obj; a string sitting in an earlier obj than its only user means a removed
  function of that obj used it. `__LINE__` constants date a file (0x43f needs ~1100 lines of the same file
  before it). A callee defined above its caller is a two-way probe even for nonmatching callers (a big drop in
  differing bytes = same TU); "no change" proves nothing.
- Counters reloaded after a `rep movs` record copy: counter and array are members of one global struct.
- Before declaring a "new" global, grep its address in include/: 0x6333cc was d3.h's `volatile d3_Time`, and the
  volatile fixed the reload order.
- Two globals in a commutative expression can be swapped with no byte change; one `/* 0xADDR */` comment per
  declaration, giving the address of the symbol's first byte.
- String literals and float constants must have the original text and bits (D3DCHECK text, `__FILE__` via
  `#line N "D:\develop\..."`); statics / globals in a TU's .bss keep declaration order only with `= 0`.
- `__LINE__` is pushed as an immediate and is not masked: pin it with `#line N` before the statement.
  Line numbers recovered so far tell us where code sat in the original file (d3_tools.c: 2343, 2348).
