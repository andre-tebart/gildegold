# Data, link and layout notes

For the gapdata, layout and base passes: where data lies, what one object is, how the linker orders things, what
a unit boundary looks like. Not needed to match a function's code (that is `notes/vc6-quick.md` and
`notes/vc6-matching.md`).

### One object or several

- **Never `&X + N` past X**: a constant of another object is its literal (`80.0f`), a field is reached through
  its pointer (`d2_LastWindowPtr->w`), several globals one memset clears are one object (EVTGLOBALS, int[4]), flag
  bytes after a byte are objects of their own, an int at an odd address is a packed struct's field. A memset over
  several globals rewritten as single stores costs the byte match: declare the block as one object.
- **A global the original reloads after an inline `strcpy` into another global** (ours keeps the value in eax, the
  copy loop takes other registers, then a callee-saved cascade, constants 0 / 1 not in registers): the two globals
  are one object; VC6 forgets every field of the object the copied bytes land in. Probe with
  `strcpy((char *)&rec + off, src)` in `dcp try`, then declare the record (sv_SelectPanelObject byte, 86 ins -> 0;
  sv_QuickJump: object, record, two unread ints, contactName).
- **Counters reloaded after a `rep movs` record copy**: counter and array are members of one global struct.
- **`shl eax,2` shared by two loads `[eax+A-4]` / `[eax+A]`** where ours CSEs `lea r,[e*4+A]` and reads
  `[e*4+A-4]`: a negative displacement off the symbol; the object before T (at A-4) and T were one record. Declare
  it (`SVCLIMATE { int seasonClimate; SVWEATHER weather; } sv_Climate`, the old names as field macros in the
  header) and the plain `T.a[h - 1]` matches; every spelling on T's own base: no lever. Do not read the neighbour
  through a cast of the other global: our link pads separate objects (the 0x188-byte record went to +8), dcp
  session wrap flags it as M.layout. (sv_WeatherFrame 11 ins -> byte; the header change also took sv_t's
  ai_TutorialInitChapter3 to byte)
- **A byte of a global `union { int word; unsigned char byte[4]; }` loaded as the dword** (`mov ecx, dword [U];
  xor eax,eax; mov al, ch`, a later `cmp byte [g], cl` reusing it) where the original does `xor eax,eax; mov al,
  byte ptr [U+1]`: the bytes were separate globals. Char-pointer views, casts, bitfields of the union: no lever;
  only a distinct object fixes it (probe: `extern unsigned char probe;` inside the function in a `dcp try`). An
  aligned unsigned char global can still load as `mov ecx, dword; and ecx, 0xff` (ai_SaveOptions' `_itoa(b, ..)`),
  so a `.word & 0xff` spelling elsewhere is no evidence for the union. (ai_ApplyGfxOptions byte, the ai_GfxFloor
  split)
- **Colour keys read as r + g with one `mov eax, dword [key]` (`mov cl, ah`) and b as a byte, where the original
  has `mov ax, word [key+1]` and r as a byte**: the keys were 3-byte objects (no code touches key+3, refs to +0..+2
  only). The 4-byte D2KEY (r, gb, pad) makes VC6 treat the object as a dword container; declare
  `{ unsigned char r; unsigned short gb; }` under pack 1. It is the container effect, not a word at an odd offset.
  Type D2COLKEY in shp.h, owner shp (between shp's 0x6f0450 and d2's 0x6f0458, moved from f3gm); f3gm declares the
  key as an extern of the incomplete `struct D2COLKEY` (it only takes its address). Changing D2KEY in d2_types.h
  (the 3-byte body, a forward typedef, or removing it) moved sv_t's ai_TutorialInitChapter2 / 3 off byte, so the
  old 4-byte D2KEY stays there unused with a comment saying why. (d2_TransparentColour 0x6f0454, marker keys
  0x5fed74 / 0x5fed78; d2_GrabShape16NoRuns, d2_GrabShape24NoRuns, d2_GrabShape16Runs byte; key loads of
  d2_GrabShape8Runs / 24Runs match, other hunks remain)
- **A global 2D array whose row stride is not element_size * count**: declare a block struct. `init_StockBlocks`
  is 0x2a90 bytes per player (0x10 header ints then 0x55 records of 0x80), so `G12STOCK [][0x55]` (stride 0x2a80)
  reads the wrong player's records. Declare `typedef struct { G12STOCK rec[0x55]; char pad[0x10]; } G12STOCKBLOCK;`:
  records first keep the symbol at 0x7d6a70, the tail pad gives the stride, and the chain matches
  (`0x7d6a70 + player * 0x2a90 + i * 0x80`): 330 -> 275 differing instructions.
- **A `mov eax, ds:0xf8f080` the original does not have**: the d2 object table 0xf8f080 is a POINTER global
  (d2_Objs), not an array: write `U_I(U_I(0xf8f080) + off + id * 0x2ec)`, not `U_I(0xf8f080 + off + id * 0x2ec)`
  (0x5d6f40 matched; same for every control-record access of the front-end windows). Caching it in a local
  (`char *obj = (char *)U_I(0xf8f080);`) gives one load for many accesses, as a window with several fields does.
- **`mov reg,addr; add reg,2` where the original has one `mov reg,addr`**: a pointer built from a symbol plus a bias
  is not folded (`char *p = (char *)externdata + 2;`); declare a symbol at the biased address (ls.c's
  gm_PlayerRecordBytes 0x7d5f82, the +2 byte view of gm_PlayerRecords, keeps 0x51a1a0 and 0x51db00 at shape).
- **A hardcoded guard or loop bound is an immediate, not an array end**: d2_OpenForm's view scan compares to the
  literal 0xfb91e4 (0x474376), not d2_Views + 0x30 (0xfb6060); `(int)arr + 0x40 * 0xf8` in a `while (p < ...)` made
  the linker see two disagreeing sites for the array symbol and the masked field differ (0x50d490). A `#define`
  with the numeric address (the instruction in its comment) keeps the immediate and passes Q3.

### Literals, floats and .rdata order

- **Strings and floats**: original text and bits (D3DCHECK text); `__FILE__` via the file's first
  `#line N "D:\\develop\\..."`.
- **Bytes right after a unit's last constant**: its literals. A unit's .rdata is its named constants, then the
  `__real@` COMDATs it is the first in link order to use (the linker keeps the first copy). ch, the first unit with
  float code, holds the shared pool 0x5eb780..0x5eb8dc (1.0f, 255.0f, 0.0f, ...); every unit's `1.0f` reads 0x5eb780
  (the only 4-byte 1.0f outside the tables). Order inside a unit: per function in source order, the literals it is
  the first to use, in reverse order of first use in its code (VC6 prepends; a cl test reproduces it). Simulating
  that over ch's original code predicts the pool except 3 literals that sit earlier (1.0f 0x5eb780, 3.0f 0x5eb790,
  0.4f 0x5eb7b8): functions of ch the linker removed (/OPT:REF) used them first. Ours places ch's pool in that
  order without them. A constant the code loads from memory is another unit's `const` read through an extern
  (ai_CardBack in unk_4a7140), not a header `static const volatile`.
- **A strcpy hunk at the inlined copy, or `link --nonmatching` reporting a `??_C@...` symbol as a data
  conflict**: an invalid escape drops the backslash and shortens the literal. `"misc\Messagebox"` is
  `"miscMessagebox"`; `"\game.INI"` is the 9-byte `"game.INI"` (dword + dword + byte) where `"\\game.INI"` copies
  10 bytes (dword + dword + word). The function still matches (relocations masked), the string data at 0x60ea68
  differs. Write `\\` in the C source.
- **`__LINE__` pushed as an immediate (not masked)**: pin it with `#line N` before the statement. Recovered line
  numbers tell where code sat in the original file (d3_tools.c: 2343, 2348).
- **Pooled float constants are literals**: 0x5eb780.. (ch's pool) and the other units' `__real@` runs are VC6's
  literals, not objects: write `1.0f`, `0.0f`, `0.5`, `1.0`, `100.0f` .. A stand-in object at a pool address
  (the former ai_FloatOne / ai_FloatZero / cm_DoubleHalf / cm_DoubleOne / ai_FloatHundred ..) makes M10 report
  every literal reading it. Turning all 264 uses back into literals: 4664 functions, one fell for good
  (d3sndw_Update), two moved by the declaration set only (ls_ReadSaveHeader, main_RoundReport, G.tiebreak), one rose
  (he_RunNightWatch). A literal the code keeps where a constant would fold (x * 1.0f) is the text's shape, not an
  object: he_AllocBurn's `f = 1.0f` joined with the distance is a float local (fld dword [1.0f]), not ai_FloatOne.
- **Object vs literal at an equal instruction**: an object operand and the literal differ in C2 beyond the bytes:
  d3sndw_Update's `vol * 0.0f` swaps two reloads at the probe loop latch (4 ins; insensitive to the declaration
  set, 32 samples) where any float in memory (extern, static, static const) gives byte. Kept as a commented static
  local; the original's fmul still reads the pooled 0.0f, so its text differs elsewhere (open).

- **Named constants come first in an object** (cl test: a `const` defined after every function still sits at
  offset 0 of the obj's one .rdata section; the `__real@` COMDATs follow in function order). So a named datum
  AFTER literals in the original starts a new object: a unit boundary. Found so: d3_light ends with
  d3_ShadeInit (its 2/pi, 1/1024 before d3_ZAxis), shot (0x5dbc10) splits from menu, grave (0x4ef3c0) from gv
  (ai_CannonBalls then gv's literals then gv_SaleShares), am's literals 0x5ecec0.. then ag_Pairs (amr split off
  at 0x492fd0), amr's literals then ai_CharWalkSteps (amt_ai at 0x4a3a90), grave's literals then gv_RankWorth with
  the .data block gv_Decisions (gs at 0x4f2720). A step at a named datum is the place to look: walk the literals
  before it (their first users' order shows which object they belong to).
- **A const object of 64 bytes or more is 8-aligned** (cl tests: int[15] align 4, int[16] / int[4][4] / a 64-byte
  struct align 8, also inside the section: an int[1] before an int[16] puts it at +8) and makes its obj's
  constant section 8-aligned. A >= 64-byte named datum at an address not 0 mod 8 in the original is several objects
  (gv_ListEntries[4][4] -> four int[4], read with a constant row at four sites).
- **A named int nobody reads between named constants** (0x5ed1ac = 11 after the 11 ai_RingStrides, 0x5edbb0 = 48
  after hmt's literals): a const of the unit (a count) whose readers use the immediate or were removed; declare it
  (ai_RingStrideCount, ai_MissionTypeCount: names / owner guesses, said so), never pad.
- **An index the original clamps to N on an array of N entries** reads the next object (gv_RankWorth[10] reads
  ord's -20.0's low dword): declare the real size, the overrun is the original's; a padding entry moves the
  next unit's data.
- **Named constants keep definition order and pack** (char[2] + char[2] at +0, +2): two 2-byte strings 4 apart in
  .rdata are char[4] (agi_SepSpace / agi_SepQuote); 12-byte vectors 16 apart are D3VECTOR4 (the d3 axes).
- **Values in .rdata in an order no first-use gives** (the first users' addresses jump up and down, or an
  earlier unit reads one) are named constants, not literals; only the sites that read that address use the
  object, the same value elsewhere is a pooled literal (shot's ai_ReachScale 0.1 vs the 0.1f of ai_InitWind).
  An object operand can change C2's CSE: ai_SphereVolume needs `ai_SphereFactor * (size * size * size)`.
- **Literals of functions /OPT:REF removed**: a literal sits in a unit whose code never reads it (d3_tools'
  255.0, 0.002, 0.59f, 4.0f, 0.25, 0.2857 at 0x5eb978..; 100f / 256f / 0.0015625 / 512 at 0x5ebb40..): a removed
  function of that unit used it first. Our link places it with its first surviving user instead; these steps
  stay open (no dead function is invented). Accepted so far: vfs_Ext -32, d3fl_Suffix -24, agi_SepSpace -32,
  d3_ZAxis -24 (d3_tools 0x5eb978..; 6.0f 0x5ebb1c in d3_fl's literals, first user am 0x490ed7; 100f / 256f /
  0.0015625 / 512 around -pi/2 0x5ebb40..), the 100.0f / 0.002 ours puts in d2 / ai (0x5ece08 / 0x5ece30), the
  0.2857 ours puts in cm (0x5edc58: the +8 of amt_TabA .. ord's literals).
- **A main.c GAPDATA block of 4 bytes in .rdata's range with no source user** (only `extern GAPDATA` lines) is a
  pooled literal its users already write: delete it (it sat in .data, +200k off).
- **A header comment edit is a tie-break too**: changing only comments in d3_vec.h / am.h moved sv_t's
  ai_TutorialInitChapter3 / f3gm's gm_FormatAmount off their grade; keep shared headers' text unless needed.
- **datameasure's placed count is not monotonic**: symbols at delta 0 behind two cancelling errors drop out when
  one error is fixed (7 -> 2 at agi_SepSpace); count the steps instead.

### .bss, common block and tentative definitions

- **.bss statics / globals**: need `= 0` to keep declaration order; globals in the original's common block (past
  the last LIBC .bss symbol, gildegold 0x6f3300) are tentative `T x;`, and dcp's D5 accepts a tentative definition
  there.
- **Where the commons are**: a VC6 link (`int a = 0; int c;` in two objects plus LIBC, map file) puts .bss as
  a.obj's, b.obj's, then LIBC's contributions (crt0dat, nlsdata2, ..), then every common symbol as `<common>` (the
  program's first, in encounter order, then the CRT's __crtheap, __acmdln, ..). In GildeGold LIBC's last .bss
  symbol ends at 0x6f3300: everything above (0x6f3300..0x14ebe84) is common data, tentative definitions in the
  original (the d2_ / d3_ / ts_ names interleaved there follow the first object's header order). Below it the
  units' `= 0` definitions are right.
- **A short global loaded as a dword and widened** (`mov ecx, dword [d2_ScreenH]; .. sub ecx,eax; mov [m],cx`)
  where the original keeps `mov cx, word [..]; sub cx,ax`: VC6 widens only when it knows the global is 4-aligned
  (its unit defines it); it keeps a short global in a word register through a call-free loop only when the unit
  does not define it. Probe: `extern short probe;` (or `extern unsigned short probe;`) inside the function in a dcp
  try variant. In the common block the original's definition was tentative: write `short d2_ScreenH;` /
  `unsigned short gm_KindObjCount;` (byte, nothing else moved). Moving the `= 0` definitions below the function:
  no lever (C2 sees the whole unit). Below the common block the original defined it in some unit's .bss: look for
  the owner whose code does not touch it (an extern in the user unit). dcp's D5 (decompanion fedde79) fires only
  below the common block and names the block's start. (d2_ScreenH / d2_ScreenW 0xfb9070 / 0xfb9072,
  gm_KindObjCount 0x76fdc4; d2_ResizeWindow, gm_BuildKindObjects byte)
- **Not done**: the other ~2000 `= 0` definitions above 0x6f3300 are commons in the original too; converting them is
  codegen-neutral except where code depends on knowing the placement (the short-global widening). Convert one when
  a stuck function shows that symptom.

### Unit boundaries

- **TU evidence from .data**: each obj has one plain .data block of its initialised globals, then its string
  literals as COMDATs in function order; /O2 implies /GF, so identical strings are pooled across TUs. A new
  non-string block marks a new obj; a string sitting in an earlier obj than its only user means a removed function
  of that obj used it. `__LINE__` constants date a file (0x43f needs ~1100 lines of the same file before it).
- **A callee defined above its caller is a two-way probe**, even for nonmatching callers: a big drop in differing
  bytes = same TU; "no change" proves nothing.
- **A function with NO trailing alignment nops** (the linker's 0xcc right after its last byte, while every /O2
  function carries 0x90 nops inside its own section): compiled with optimization off or for size;
  `#pragma optimize("", off)` (or `("s", on)`, same bytes) around it reproduces it. The 0xcc there is not
  object-boundary evidence. (d2_Unk47d210 / d2_Unk47d710, the inline-asm blitters)

### Headers and views that move code elsewhere

- **A header's type or extern set is a tie-break for other units**: new externs in menu.h moved sv_t's tutorial
  chapters off byte (declare them in the users); changing D2KEY in d2_types.h moved ai_TutorialInitChapter2 / 3
  off byte; the SVCLIMATE header change took ai_TutorialInitChapter3 to byte.
- **A reload order fixed by `volatile`**: before declaring a "new" global, grep its address in include/ (0x6333cc
  was d3.h's `volatile d3_Time`, and the volatile fixed the reload order).

### Ledger and naming rules

- **One name = one address**; one `/* 0xADDR */` comment per declaration, giving the address of the symbol's first
  byte; reach struct members through the type. Two globals in a commutative expression can be swapped with no
  byte change.
- **Before inventing a name for a placeholder callee**, check the other units' headers for a named declaration of
  the same address: d3_Unk44c9d0 was `d3io_LodName` in d3_int_io.h, d3_Unk5fbf78 `d3io_Lod`; the owner's names
  fixed Q2 without a guess (and removed duplicate externs).
- **M8 naming the second of two same-GUID library symbols**: the original wrote that name (strmiids.lib defines
  CLSID_FilterGraph and CLSID_FilgraphManager with one value; mov_Open). Same for the `""` window title: the
  original passes ch_EmptyName (M9), as other units do.
- **A rename must make every declaration agree in the same commit**, or lint's cross-TU X conflict fails the
  pre-commit ledger gate ("declared differently here and elsewhere"; every X conflict is an error, nothing is
  parked). Renaming ai_Unk4fe140 -> gm_AddTime (424 refs) left unk_5d33b0.c's `int gm_AddTime();` (K&R, int
  return): fill in the real prototype (with the file-scope `struct GCLOCK;`).
- **Q7 with a shared K&R declaration**: when other units call the callee with an int address literal
  (`gm_MinutesBetween(rec, 0x770ae0)` in he.c / sv.c / gv.c) neither the shared header nor one .c can carry a
  prototype (cross-TU conflict). The exemption tag must sit on the call line itself:
  `x = f(a, b);  /* @original: ... */` clears Q7, a comment block above the line does not.
- **Q6 counts an address comment above a prototype in any file**: vfs_fwrite (0x430e30) was readable through
  ls.c's foreign `/* 0x430e30, the file read / write ... */`; removing it (ls.c now includes vfs.h) dropped it to
  draft until include/vfs.h described the prototype. When a foreign declaration goes, move its description to the
  defining unit's header.
- **Q2's placeholder regex flags `unkNN` field names used in a body**: an unknown field cannot be typed without a
  meaning; keep one raw access and escalate the field-name gap.
- **`@deep` reasons are parsed by Q5**: a reason containing the word ROUGH fails the draft-marker rule (readable
  -> draft). Word escalation reasons around the markers.

### Link checks

- **Link order (exemeasure's `order:` pairs)**: functions land in the exe in their definition order within the
  obj, except `static` functions: VC6 emits those after their first user, so a static callback defined above its
  creator ends up behind it. The original's callbacks (d3t_*CB, d3_FindObject*CB, ocmd_ParticleCB) sit before
  their creators: they were not static. Write callbacks extern (the codegen is the same). A pair of two extern
  functions is a definition order that differs from the address order in our source. Moved into address order
  (no grade changed): d3_engine, d2, d3_fl, d3_light, hmt, lobby, main, ord, sv_t, lc, sv, gv, he, f3gm (2026-11; he and
  f3gm by moving each local prototype / macro block in front of its first user after the moves: a call before its
  prototype is an implicit `int f()` and a C2371 or a silent prototype loss; check with cl /W3 for C4013). Left:
  am (ai_ChooseSubAction up: 9 byte functions fall, its callers need it below), menu (the option screens / save windows are defined below
  their callers on purpose, see menu.h), and the ICF folds (sv_ReturnZero, ai_AlwaysTrue, ai_MsgNoOp /
  agi_Kind: one body for several names, not an order).

- **"matched sites disagree" for both symbols of two stores of one value to two globals**: VC6's store order is
  not controllable from the C (source order, chained assignment, types, names, declaration order: no lever); the
  masked compare reports "matched" while `dcp link` sees the relocations swapped. Fix: a struct view of the two
  adjacent words (G16TRACKHANDLES { current; copy; }), the stores then relocate to one base with +4 / +0 addends.
- **Two `mov [..],imm` stores of one address into one record without a register CSE**: two identical functions
  folded by the linker (/OPT:ICF), not one symbol; one name assigned twice gives one `mov eax,imm` + register
  stores. (0x4a16f0, unresolved; `xor eax,eax; ret` stub at 0x52a040)
