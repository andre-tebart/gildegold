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
  0.2857 ours puts in cm (0x5edc58: the +8 of amt_TabA .. ord's literals), the 512.0f ours puts in f3gmt
  (0x5ee8e8, first surviving user 0x4ffba4; the original's is 0x5ebb54: the +8 -> +16 at ls_LanguageNames).
- **A main.c GAPDATA block of 4 bytes in .rdata's range with no source user** (only `extern GAPDATA` lines) is a
  pooled literal its users already write: delete it (it sat in .data, +200k off).
- **A header comment edit is a tie-break too**: changing only comments in d3_vec.h / am.h moved sv_t's
  ai_TutorialInitChapter3 / f3gm's gm_FormatAmount off their grade; keep shared headers' text unless needed.
- **datameasure's placed count is not monotonic**: symbols at delta 0 behind two cancelling errors drop out when
  one error is fixed (7 -> 2 at agi_SepSpace); count the steps instead.
- **The .rdata size gap is mostly the library**: of .rdata's -952 at s3's start only -128 was game data (lc's
  unwritten kind masks, gm_SpecialItemGroups in .data); the rest is LIBC objects the original links and ours does
  not (the syserr strings 0x5ef274 .. 0x5ef5a0, SEH scope tables, five more dxguid GUIDs). Compare the game part at
  the first library symbol (c_dfDIKeyboard: ours vs the original's address), not the section size.
- **A named const that is a string literal of the original**: main_DefaultHost "127.0.0.1" sat among main's .data
  strings, ours had it as const char[] in .rdata: write the literal at the user.

### .bss, common block and tentative definitions

- **.bss statics / globals**: need `= 0` to keep declaration order; globals in the original's common block (past
  the last LIBC .bss symbol, gildegold 0x6f3300) are tentative `T x;`, and dcp's D5 accepts a tentative definition
  there.
- **Where the commons are**: a VC6 link puts .bss as each obj's own .bss in link order, then LIBC's, then every
  common symbol as `<common>` (rule: next bullet). In GildeGold LIBC's last .bss symbol ends at 0x6f3300: everything
  above (0x6f3300..0x14ebe84) is common data, tentative definitions in the original. Below it the units' `= 0` /
  static definitions are right.
- **LINK 6.00's common rule** (s7, cl/link tests in a scratch dir, map files): (1) a common belongs to the FIRST
  object in link order that has a tentative definition of it (an extern or a mere reference does not count);
  (2) one group per object, the groups in REVERSE link order of the command-line objects (menu's group first at
  0x6f3300, udp's last at 0x14eaae0), LIBC members' commons after all of them; (3) inside a group the order is
  LINK's name order, not definition order, size or alignment (two-letter names sort by 4*c1 + c2, longer names
  scramble like a hash; definition order only breaks ties): with our invented names the in-group order cannot
  match, the group order and extents can; (4) each common is aligned to the next power of two of its size, at most
  32 (3 -> 4, 12 -> 16, 17..inf -> 32; 609 of our commons follow it exactly); (5) /OPT:REF drops a common no kept
  code references (a unit's own .bss is never dropped).
- **Consequences**: a common-range datum off its common alignment is part of a bigger object (ai_BldCarts at
  0x6f40a4: one 40-byte LCBLDCARTS ai_BldCartList at 0x6f40a0, ai_UpdateBldCarts byte; s8:
  menu's MENUOPTIONS 0x6f35c0, sv's SVQUICKJUMP 0x75e8c0 / SVBAND 0x760440 / SVCLIMATE 0x760480, d3_engine's
  D3CLIPSTATE 0xfc5060, each starting with the small datum before the misaligned one; none is left). A readerless gap
  block in the common range is the alignment pad before the next common: delete it (26 deleted). A datum inside
  another unit's group span is that unit's (move the tentative definition; moves changed no grade); at a boundary
  between two groups pick the neighbour among its users.
- **Codegen of a tentative definition**: `unsigned char x;` loads as a byte (like an extern); a tentative char[4] or
  4-byte struct, and every `= 0` definition, loads as `mov ecx, dword; and ecx, 0xff`. menu's ai_SaveOptions /
  ApplyOptions / OptionsGfxScreen / OptionsGameScreen need the dword at 0x6f35c4 / 0x6f35c8: those bytes were two
  4-byte objects (kept `= 0` until declared so). `__declspec(align(1))` (GAPDATA) turns a tentative definition into
  a .bss definition (no COMM): write a common gap block as plain `unsigned char x[N];`.
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
- **Done (s7, s8)**: every game datum above 0x6f3300 is a tentative definition in its group's unit, d3s_EdgeX / EdgeY
  too since decompanion 8cbfa28 (the original reads them at [esi*4+0xfc50d0] / [esi*4+0xfc50e0], two displacements,
  any order works; our link puts EdgeY 0x10 before EdgeX, and M.layout no longer judges a pair of two commons).
- **Measure the common area per group, not by steps**: every symbol of it is a step (name order); compare each
  group's start (s7: from f3gm on -13.0 .. -14.1 KB, before it +68.7 KB = gm_QueryStack in f3gmt's .bss - 13 KB).

- **All-zero initialisers go to .bss** (`int x = 0;`, `= {0}`, a GAPDATA of zeros: our map has them past .data's
  end, as VC6 does). A zero datum inside the original's initialised .data is a pad (the 8-alignment before a
  >= 64-byte datum, a string's NUL / pad) or a member of an initialised object (sv_ViewRectLeft / Top 0 between
  Right / Bottom 694 / 600: one RECT-like object). Zero gap blocks before such an object are pads: delete them.

### Unit boundaries

- **TU evidence from .data**: each obj has one plain .data block of its initialised globals, then its string
  literals as COMDATs in function order; /O2 implies /GF, so identical strings are pooled across TUs. A new
  non-string block marks a new obj; a string sitting in an earlier obj than its only user means a removed function
  of that obj used it. `__LINE__` constants date a file (0x43f needs ~1100 lines of the same file before it).
- **A callee defined above its caller is a two-way probe**, even for nonmatching callers: a big drop in differing
  bytes = same TU; "no change" proves nothing.
  The split itself is the probe: f3gm split at 0x4fe570 dropped gm_BirthDate / gm_UpdateTechTree /
  gm_TitlePeriodServed (the clock functions 0x4fe140 .. no longer above them), at 0x4fe140 nothing fell.
- **A literal or a named const: the codegen decides**: VC6 /O2 shares a load across branches with a literal
  operand but not with a const object (ai_CannonAim: `dt * 1.6f` byte, `dt * ai_CannonZoomSpeed` draft); a value
  amid named .rdata that a byte function needs as a literal ends the named block there: the object's literals
  start (in function order, each function's in reverse order of first use), the next named datum is a new object
  (sv_t / svcannon / svgrave at 0x5eec68 / 0x5eecb0).
- **A short global loaded as a dword** (`mov eax, dword [g]; and eax, 0xffff`) after a split: the user's unit
  defines it; ai_CityStatsPanel fell to draft until ai_StatsChartFlags moved with it (lcamt).
- **Data of other units inside a block go by place**: rt_MapSurface / ai_MapScrollEnabled (lcmap), ai_NextPartyId
  (f3gmt), ai_DragWindow (svgrave), ord_KindTable (f3gm) were defined by their readers' units; the block's object
  owns them.
- **dcp unit split (data)**: it leaves data definitions in place but moves their comment continuation lines into
  the declaration copies (orphan `... */` lines: C2143 at the copy, and the definition's comment then swallows the
  next definitions: unresolved externals). Remove the orphans and restore the continuations from git. Its
  prototype copies can precede the typedef they use (CANNONSCENE): move them below it.
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

## Checking a unit boundary (verification of the s1-s3 splits)
- Data evidence (named datum after literals, plain data after a string run) proves an object boundary exists; the
  exact function is only bounded by the last user of the old object's data and the first user of the new one's.
- `tools/c2dag/bprobe.py TREE UNIT P..` emulates a split at each P (earlier functions become externals for the
  later ones) and lists the functions whose code changes: a byte function that falls bounds the split from that
  side (grave <= 0x4ef4f0, gs <= 0x4f2720, f3gmt exactly 0x4fe140, svgrave exactly 0x582730). "No change" proves nothing.
- Opposite needs on one callee (one caller byte only if it is known, another only if unknown) prove a boundary
  between them even where no data exists (lcamt: ai_FindUsableGood).
- Pitfalls: a "plain" datum may be a string literal of the original's run (named in our source); a no-reader datum
  can be a literal of a removed function only if it is a float/double; an object without data is invisible to the
  data method.
- Also found so (s4): a run of short strings ("%s", "rt", "wb" ..) closing a unit's strings whose first users are
  all later units: that unit's removed functions (sblib before evt_Path, -24; "gb_" in txt's run): an accepted
  open step. A named int whose bytes spell a string at a string run (ai_Unk5f8b10 = 6251111 = "gb_") is that
  literal: delete it when the users already write the literal.
- A unit's first plain .data datum after the previous unit's strings may have no reader at all (dd_vesa's 5x7
  font, 637 bytes before dd_LastDriver; d3_tools' four corner vectors before d3t_SunFadeStart): declare it with the
  original's bytes; its place in the block is the owner. Not a split by itself (the block's other data decide).
- **A >= 64-byte object is 8-aligned in .data too**: D3VECTOR4[4] at 0x5f341c linked at +4; four D3VECTOR4 objects
  matched. A 64-byte datum at an address not 0 mod 8 is several objects.
- **The common block is grouped per object, in reverse link order** (menu, lcdom, lc, lobby, svgrave's tooltip
  object, svcannon / sv_t, he ..); inside a group the order is LINK's name order (see the common rule). A common
  between two groups can belong to either neighbour.
- **Splitting a header's prototypes into a new unit's header**: replace them in the old header with `#include
  "new.h"` at the same place; removing them (and including new.h in the callers) moved two functions of unrelated
  includers by the declaration set (reg.h from di_input.h).
- **datameasure --sizes** (s5): per obj of our link order the shift over its data and its string literals (placed
  by their bytes in the original). A jump inside one obj ("own") is data of another place; a jump between objs is
  data the original has there. The +6.7 KB of initialised .data were 341 main.c gap blocks holding string literals
  the code also writes (duplicates, 9.9 KB). A gap block whose bytes are only strings already in our map is a
  duplicate: delete it.
- **Plain data after a run of strings = a new object** found so (s5): poker / aip (ai_LeisureActions after poker's
  NPC model strings), he / he_b (he_BuildingModels after 0x568310's "Heilen"), d3_engine / d3_event (d3e_Events
  after "_0"). bprobe found no function in each; the first function was bounded only by the last string user before
  and the first reader after.
- **Blocks hold other units' data by place** (s5): he's block 0x625708.. (he_Find*, ai.c's ai_AmbushSpots, cm.c's
  cm_EventNames), main's 0x624560.. (init_AiTown, cm_Run*, mission's ai_MissionMode / ai_NumPlayers /
  ai_MissionPartner, sv_ViewRect*): moved to the block's unit; no grade moved.
- **A pushed address dcp refs misses**: `dcp refs 0x6248b8` showed no reader, `grep_orig.py 0x6248b8` seven
  functions pushing the char[128] "gamedata/cities" our code wrote as a literal (main_CitiesDir). Before calling
  a datum readerless, grep the original's code for the address.
- **dcp unit split with data definitions after the split point** (d3_engine: its data sit between and after the
  moved functions) removes multi-line comment lines and leaves orphans: 14 errors, 75 "falls". Write such a
  split by hand (old file minus the functions, the tool's head for the new one): no grade moved.
- **tr_Key1 -16** is the same accepted cause as evt_Path -24: sblib's short strings ("wb", "%s\n") that our link
  places with txt, their first surviving user.
- **bprobe.py caches the merged compile** (build/c2dag/unitprobe/bp_base_<tree>_<unit>.pkl), keyed by the unit's
  text since s5's start: an edited unit is recompiled.

### s6 additions
- **A named char array at a unit's string run is that unit's literal** (he / he_b: 10 `char x[N] = "..."` in main.c,
  N the length padded to 4): write the literal at the user, the run then lines up by first use. Turning the arrays
  into literals let M9 compare the text: main.c's "Rohstoff suchen" was "Rohstoffsuchen" in the original.
- **A .data table no code refers to, whose entries point at a unit's functions** (d3_io_a's AGI keyword tables
  0x5fc2c8..0x5fc4cc, 912 bytes): the reader that walked it was removed by /OPT:REF; .data keeps the table and the
  table keeps the callbacks (their only reference). Declare it with the original's bytes (D6 checks each pointer).
  Its initialiser strings come in reverse order of the source (VC6 prepends) and before the functions' literals
  when the table is defined above the functions. An entry pointing into another unit's function is an ICF fold
  of an identical body (the AGI materials' "*DONE*" = bgi_NextMat): reference the kept one, do not invent a copy.
- **A boundary inside a unit pair can be moved by hand** (d3_int_io / d3_io_t at 0x44c510): move the functions
  to the next file's top and edit both boundary lines in decl/text.dcl; the moved functions may need the
  receiving file's missing includes. A .bss gap larger than the 8-alignment of the next datum (d3_LodMissing ..
  d3io_StockPath) reads as an object boundary and can pick the first function where bprobe finds nothing.
- **Including a new header moves tie-breaks by itself**: an empty `#include "d3_event.h"` (guard only) in
  d3_engine.h took d3_mir's d3_MirDrawObject byte -> shape (byte again with 15 unused typedefs): the file / guard
  macro, not the declarations. Expect it on every header split; the build's sweep settles it (commit its row).
- **A block of plain data right before a unit's literals is that unit's**, whoever defines or writes it (the
  light budgets f3init wrote and the ambient colour d3_interface wrote are d3_light's 0x5fcc6c block; d3o_FogScale
  is d3_interface's). Moving such a definition changed no grade; moving its extern between shared headers
  (f3init.h -> d3_interface.h) changed none either.
- **Zero members of an initialised object in .data** (sv_ViewRect {0, 0, 694, 600}): accesses by absolute field
  address cannot tell four ints from a struct; the section can (VC6 puts a zero scalar in .bss). One RECT with the
  old names as field macros in the header; local `extern int` copies of the names must go (the macro breaks them).
- **The common block (s6 count)**: 5 game commons then; s7 made the rest tentative (609 commons now).
- **ai_TutorialInitChapter3 (sv_t)**: every build after an he.h / sv_t change re-sweeps it and stops at shape
  (the baseline goal); `dcp try` with 6 unused typedefs before it still gives byte: restore its tiebreak row.

### s7 additions
- **dcp prints sizes >= 1 MB in hex** (`size=0x00132000`): datameasure skipped those data (cm_RecvPool, txt_Keys ..,
  6.9 MB not counted); its parser takes hex now.
- **A unit-.bss gap no instruction reads** (d2: 10836 bytes after d2_DragActive, 0x6ed9f8..0x6f044c; grep_orig finds
  no reference, no dword of the image points into it): data of removed functions (a unit's .bss survives /OPT:REF, a
  common does not). s8 declared it as d2_UnreadBss[10836] (one unread block of its owner, no invented readers).
- **dcp and commons**: M.layout does not judge a pair of two commons one register expression indexes (decompanion
  8cbfa28: LINK's name order, not ours); a common cl padded (gm_QueryStack: 0x14030 COMM size for 0x14020 bytes)
  takes its declared size as its extent, the pad a D1 warning (toolchain effect); `dcp rename` compiles an object
  with commons again (dbc55e7). ai_RelationMemory's D1 / D8 went away once it was defined in its group's unit (f3gm).

## Decision: commons are placed per group (user, s7)
The order inside an object's common group follows LINK's hash of the original names, which the exe does not hold.
Until everything else matches, a common group counts as placed when its start and extent equal the original's
(tools/exe/datameasure.py "common groups"); the in-group order is not worked. Choosing names that sort into the
original's order (reverse-engineering LINK's name order) is kept for the very end.

### s8 additions
- **cl's COMM size depends on the symbol table, i.e. the names**: VC6 cl wrote COMM _gm_QueryStack 0x14030 for an
  int[20488] (0x14020) when the name hashed next to a big common (ai_Relations); a 2-common test reproduces it (int
  q[20488] + char r[0x90000]: q 0x14030; names swapped: the other one bumped, by (64 - size % 64) / 2 or 0x20). The
  original's layout shows its exact size, so the bump is our name's: pick a true name cl does not pad
  (gm_QueryChildStack). Check a big common's COMM value in the obj (a symbol-table dump) when D1 / C.overlap names
  it with a few bytes over. `dcp rename` patches names into the existing objects: `dcp build --force` recompiles.
- **Below the common area the blocks go by place** (s8 moved 37 data, no grade moved): a datum between two data of
  one obj's .bss block is defined by that obj (d2 block 0x6ed918.. holds gfx / io_text / sv / f3init data; shp's
  0x6f044c.. holds d2_ShpW / ClipLeft; ch / evt / txt / d3_engine / d3_fl / d3_interface / d3_light / d3s / f3gm /
  f3gmt blocks). Keep a char where a defining unit's dword load would be wrong (ai_OwnPlayer in ord).
- **A common group is placed only when every byte below it is**: start = 0x6f3300 needs the units' .bss and the
  .data before it exact. s8 left -2.4 KB below the common area: the .bss start (util_Line) -528 = game .data -356 +
  LIBC .data, then the units' unread holes (qnt -1 KB, d3if_MegaCam's 275, 4-byte holes). Fix those, then extents.
- **datameasure --groups** prints every common group (start and extent deltas); an extent off by a few dozen bytes
  is the in-group name order padding (menu +16 with three commons), not a size error.


### s9 additions
- **A table indexed around 0 reveals its base**: qnt_InitTables writes `[i*4 + 0x631b34]` for i = -255..255 and
  stores 0x631b34 into a pointer: the table spans 0x631738..0x631f34 (511 ints); 8 more bytes before it at the
  8-aligned 0x631730 made it DL1's CLOSEST_INFO c_info (index/r/g/b, distance, squares[511]): QCLOSEST qnt_Closest
  (+1 KB). Our int[256] at the middle address had the loop write below the array.
- **An initialised path buffer**: a string followed by zeros up to the next datum is one char[N] (d2_FormsDir
  "forms\\" is char[256]: 0x5fe608..0x5fe708; main_OpenScreen sprintfs into it). Ours was char[8]: -248.
- **LIBC data with no reader, pulled by a removed function**: syserr.obj's _sys_errlist / _sys_nerr (.data
  0x630740..0x6307f8) and its 43 strings (.rdata 0x5ef274..0x5ef5a0) sit in the original with no reference. A
  call from code /OPT:REF later removes still pulls the member (and its data stays). zlib's gzerror calls
  zstrerror(errno), which is strerror only under HAVE_STRERROR: defining it in gzio.c pulls strerror -> syserr and
  closes .rdata (-809 -> +16) and .data's LIBC part. Check a LIBC gap against members the source's removed code
  could reference before calling it unfixable.
- **Unread .bss holes** (`tools/exe/holes.py LO HI`): a gap between two declared data that is not the
  8-alignment before a >= 64-byte datum (or before the first datum of a unit whose section holds one) and that no
  instruction operand or image dword points into is data of removed functions: declare an unread int block after
  the datum it follows (s9: ~30 blocks in ch .. f3gmt, `<prefix>_UnreadBss`). A gap at a unit boundary goes to the
  unit before it unless alignment says otherwise (d3_interface's 352 bytes start 8-aligned at 0x633758 after
  d3_fl's last byte: a d3_fl block would be 8-aligned to 0x633758 too and overrun).
- **The pooled "" literal in .bss**: twenty units push 0x631090 for "" (ours ch_EmptyName); VC6 places the
  all-zero "" COMDAT in .bss. Ours still has ecmd's ocmd_* "" literal right after ch's block, which fills the
  4 bytes 0x631094 the original has there: do not declare an unread block for them.
- **Unread .data tables between a string run and the next named datum** are the next object's first datum: the
  58 shorts at 0x6116f0 before ord_OrderHandlers (58 pointers) are the handlers' texts (ord_OrderTexts, -120).
- **LIBC member order (open)**: the original's LIBC .text puts strupr / stricmp / strnicmp / mkdir / getcwd / itoa
  at its end (0x5ea986..): the game called the old names (strupr, stricmp ..), which LIBC does not define, so they
  resolve only through OLDNAMES.LIB, and the members are pulled in a later pass. Ours calls _strupr (23 files),
  _stricmp, _strnicmp, .. and pulls them early (bio.c's `strupr` alone moved strupr.obj out of second place).
  fpinit / _file / rand also come in another order (the original: strrchr, fpinit, ftol, fprintf, _file, rand ..;
  ours fprintf, _file before fpinit): the LIBC .data then nets +20 against the original. Not done in s9.
- **Common start reached by cancelling errors (s9 end)**: menu .. main start at the original's address with game
  .data -4 (ai_ShotDrag), LIBC .data +20, the mixed .bss block 0x6f1bf8..0x6f2f98 -68, LIBC .bss +52. Each part
  still needs its own fix.

### s10 additions
- **LINK's library order is the unresolved list's order** (checked member by member, `tools/exe/extorder.py`): the
  list grows in link order, each object's undefined externals in its COFF symbol-table order, and cl writes those
  per function in definition order, inside a function in REVERSE order of first use (`f1 { fprintf(stderr, ..);
  return rand(); }` -> _rand, _fprintf, __iob). A pulled member's own externals go to the end of the list. LIBC's
  .text, .data and .bss each follow that member order.
- **Old CRT names (s10, done)**: a member the original links after every other one (strupr, stricmp, strnicmp,
  mkdir, getcwd at 0x5ea986..) is reached only through OLDNAMES.LIB in LINK's later pass: no object referred to the
  underscore name. The game called `strupr`, `stricmp` / `strcmpi`, `strnicmp`, `mkdir`, `getcwd`, `itoa`
  (xtoa.obj then lands where a LIBC member pulls it, after crtmbox, as in the original). Respelled in 41 files:
  .rdata -> exact (+16 -> 0), .data's end exact, no grade moved. In extorder's output an old name shows with one
  underscore (_strupr), the CRT's own with two (__strupr).
- **LIBC members pulled earlier than any surviving user (open, not reproducible)**: fpinit / ftol before udp's
  fprintf, rand before ch's sprintf, atof and crt0dat right after chc's strncpy (before d3t's chkstk), fwrite
  after sb's fread, fgets before strncmp, toupper before findfile. No surviving function of the objects before
  those points calls them (dcp refs: the first ftol call is ch's 0x402360, the only exit call WinMain's, no early
  code touches crt0dat's .bss): the externals of functions /OPT:REF removed (their symbols stay in the object).
  Effect: LIBC .bss order (ours _file, fpinit, time, strtok, crt0dat ..; the original fpinit, _file, crt0dat,
  time, strtok ..) and its size: ours ends +48 at __fmode, the commons start +64 (32-aligned) after s10's moves.
- **A defined char keeps byte loads where it is only tested / stored**: sv_RunFrame (byte) reads sv_MenuOpen /
  sv_InMissionComplete with `mov al, byte` also when sv.c defines them `= 0`; ai_OwnPlayer defined in f3gmt keeps
  f3gmt's `mov dl / cl / al, byte` loads. Only the widening use loads the dword (sv_UpdateBook's `mov eax, dword
  [sv_BookPhase]`). So a datum's dword load in a unit proves the unit defines it; a byte load proves nothing.
- **The mixed .bss block 0x6f1bf8..0x6f2f98 (done)** is the units after f3gmt in link order, each contiguous:
  f3gmt's tail (ai_PlayerChar), f3init 0x6f1bfc.., ls_SaveSlot, sv 0x6f1c40..0x6f23f8, main 0x6f23f8..0x6f2618
  (he_* data after it: main's or he's), sv_t, svcannon, svgrave, lobby, lcmap (from ai_BauplatzCount 0x6f2688: its
  block is 8-aligned by ai_NewsText), lcdom, lcamt, menu (ai_GfxResolution only). ~45 definitions moved
  (tools/exe/movedef.py), unread holes declared, no grade moved; every game .bss datum below LIBC's sits at its
  original address once .data's end is right.
- **An 8-byte datum is 8-aligned by cl in .bss / .data** even as a union of two DWORDs; a DWORD[2] or a
  pack(4) struct is not. msx_Last at 0x6325b4 (LARGE_INTEGER, used as one by msx_Thread, byte) is therefore not
  an 8-byte LARGE_INTEGER in the original's declaration: open (4 bytes, absorbed by the pad before msx_Tracks).
- **Literals of a removed function in an earlier unit's string run**: main's run holds "%ia[%s]",
  "dummy_Kartentisch_RECHTS_UNTEN", "sonstiges\STADTPUNKT_ANM_K.baf", "sp_STADTPUNKT_K" (0x625150..0x6251d8)
  right after main_ChooseCity's strings; their surviving users are lcamt / lc (ai_CityInfoWindow,
  ai_CreateCityPoint). /GF puts a pooled literal into the first object that uses it, so a main.c function
  /OPT:REF removed used them. The source keeps the literals at the true users: main's block is short by them and
  .data's end is -32 (s10 end). Earlier sessions had filled such places with GAPDATA copies of other units'
  literals (full_data_625150, full_data_62a9f8): those were deleted (the users write the literals).
- **Named char arrays that are literals of the string run**: tools/exe/strdiff.py lists a unit's strings against
  the original's; a string the original has inside the run that ours has as `char x[N] = ".."` in the plain block
  is the literal at its users (s10: main_HomeSweetHome / BeamedHome / RoundTotalFmt / MasterExamLog, lcmap's four
  news keys, lcamt's form names, lcdom's "panel\useobj"; push imm32 or the same inline copy, no grade moved).
  Still named: lobby_ChooseProfessionForm (0x6250b8, main's run, used by lobby only: same removed-function case),
  ai_CharNameFmt / ai_FmtLabel (0x62a564 / 0x62a56c, svgrave's run; svgrave keeps them as objects for its code).
- **A readerless table between a unit's last literal and the next unit's first datum** is the next unit's first
  datum: menu_UnreadData[240] 0x62d6c8 (after lcamt's "special\uni", before ai_ScreenModes); ai_DomizilCount
  0x62cb58 directly after ai_DomizilTable is lcdom's, with 88 readerless bytes after it.
- **Open for s11 (.data)**: svgrave's .data looks like two objects (ai_GraveChatterAt 0x62a538 + strings, then a
  second plain block ai_DragWindow 0x62a8cc..): a unit boundary inside svgrave (the s4 question); menu's
  ai_PokerUi 0x62dda0 sits after menu's strings (another object's plain block?), the menu option form strings and
  "Gamedata/saves/GILDE_SAVEGAME_%i.SAV" stand earlier in the original's run; the .data steps before main
  (evt -24, tr -16, d3_event -64 .. rd -72, the cm / f3gm / f3gmt / sv named arrays) are untouched.

### s11 additions
- **One obj's .data is its plain block, then its literals, even with data defined between functions** (cl test,
  scratch: `int a1 = 1; f() {"string_of_f"} int b1 = 2; g() {"string_of_g"} int c1 = 3;` -> a1 b1 c1, then the two
  strings). So a plain datum after a string run is a new object, wherever the source defines it.
- **Finding the objects of a unit**: `tools/exe/blockusers.py UNIT LO:HI:LABEL ..` labels each plain block + run and
  prints per function the blocks its original code reads; the users change at the boundary. Found so (s11):
  anm / rd at 0x4bc5a0 ("dummy_TUER" closes anm's run before rd's veg_Types: rd_DoorPos .. rd_RandomNamedObject
  are anm's), cmdump (ai_CharKindNames 0x60eae0, ai_DumpPerson* 0x4e58e0..), svtip (ai_DragWindow 0x62a8cc,
  0x587e00..), pokerwin (ai_PokerUi 0x62dda0, 0x5d9de0..). Each with bprobe: no change.
- **The common block confirms a split**: the first rows 0x6f3300 / 0x6f3500 / 0x6f35c0 were ai_EffectState (shot's
  ai_InitWind), ai_PokerState (pokerwin), ai_Options (menu): three objects in reverse link order. Defined all in
  menu.c, the group's extent was +16; per object all three are exact.
- **cm is four objects by its .data, but the code disagrees (open)**: cm_SpeedTick .. cm_Checks + strings up to
  cm_LobbyAddChat's "%s %s" (0x60c7b8); ai_WeaponRecordData / cm_DefenseFactor .. cm_FightButton 0x60c7c0 + the
  fight strings (first user ai_FindWeaponRecord 0x4cd9d0); cm_DialogFrameFlags .. cm_DialogLength 0x60d360 + the
  scene strings (first user cm_DrawFightTime 0x4d8b50); ai_CharKindNames (done: cmdump). bprobe at 0x4cd9d0 makes
  cm_FightWeaponLoss byte -> 31 ins, cm_LayBombAction / cm_ThrowBombAction byte -> 1: the fight code needs to know
  cm_AddFieldValues (0x4c3d00, A's start): `dcp try` with only that callee made an unknown external: draft 63 ins
  (slots: VC6 sees that v does not escape). The fight object of the original therefore defined (or included) a
  copy of cm_AddFieldValues that /OPT:ICF folded into A's (a header-defined static or a shared include): not
  settled, no split made. The scene part (0x4d8b50..) has no byte fall at its boundary (RunBirth / RunAuction /
  ExecCutscene distances grow).
- **Named char arrays inside a string run are literals** (continued from s10): cm_WalkToDummyWhy (strcpy of the
  literal: the same mov ecx,5 / rep movsd), cm_TextNewline "$A" (10 users across units: the first user's object
  holds the pooled literal), ai_CharNameFmt "%1N3" / ai_FmtLabel "%s:" (svgrave's run), lobby_FmtName ..
  FmtChatShout (lobby's run, first-use order), lobby_ChooseProfessionForm (main's run: a removed main function, the
  literal now with its surviving user lobby, +24 accepted). Single chars at a string run's pad
  (ai_Unk60dd24 = '0' ..) are the pooled "0" / "1" / "2": delete them when the users write the literals.
- **A string of an earlier unit's run whose only surviving user is later is a removed function's literal**
  (s11 more): d3d_D3D->lpVtbl->EvictManagedTextures text in d3_engine's run (user d3_interface; d3_engine -48 /
  d3_interface +48 cancel), "FULLSCREEN" / "DIRECTWINDOW" in d3_io_t's run (user WinMain), "!bk_" / "bk_" in rd's
  run (users f3init / lc). Accepted, like s10's.
- **A LARGE_INTEGER at 4 mod 8**: cl test: `#pragma pack(4)` before `#include <windows.h>` declares winnt.h's
  LARGE_INTEGER 4-aligned (the CRT headers push their own packing; struct { __int64 } and double stay 8-aligned).
  msx.c sets it: msx_Last 0x6325b4 placed, every msx function byte.
- **A common group's extent off by less than the largest alignment pad** can be the in-group name order alone
  (lcdom -24: its commons' pads 12 / 28 / 24 depend on the order). Not a size error until the names are settled.
- **Removing a shared header's extern moves tie-breaks** (as before): cm.h's two externs took ls_ReadSaveHeader byte
  -> shape (proven byte under a declaration set), sv_t.c's took ai_TutorialInitChapter2 byte -> shape, lobby.h's
  took main_RoundReport shape -> byte.
- **State at s11's end**: steps 61 -> 48 (no .bss step below the common area); .data end -48 (only the accepted
  removed-function literals); the common area starts at +0 (s10: +32): shot, pokerwin, menu, lc, lobby, svtip,
  svgrave placed by start and extent. Open: cm's fight / scene objects (above), lcdom's group extent (name
  order), f3gm (gm_TagTmp +22108, gm_BoyHead), f3gmt (gm_BQType .. gm_RankLogFormat -608), sv (sv_FramesPerTick ..
  sv_BarToggle), d2_PointerSprite / d2_HoverX, amt_TabA +8, the early accepted steps.

### s12 additions
- **LINK 6.00.8447 /OPT:ICF keeps the LAST copy in link order and folds only external COMDATs** (cl/link test,
  scratch: a.obj / b.obj each with an identical function between two distinct ones; the surviving body sits at
  b.obj's place under both names, with the objects swapped at a.obj's; a `static` copy is never folded, even with
  identical bytes and relocations). So a function at an earlier object's address can't be the folded twin of a
  later object's copy, and a static twin would stay in the exe as a second body.
- **The cm conflict was our C, not the boundary**: the fight code needed cm_AddFieldValues (0x4c3d00) defined in its
  unit only because its sent values were function-scope locals other statements share. With the callee unknown
  (`dcp try`: a local prototype of `cm_AddFieldValuesX` + `#define cm_AddFieldValues cm_AddFieldValuesX` inside the
  function), VC6 treats the address as escaping and orders the loads after the store / drops the register. A value
  sent by address to a message builder (cm_AddFieldValues / AddFieldValue / AddFieldDelta) is the sending block's own
  local (`{ int vv = -1; cm_BeginFieldBlock(..); cm_AddFieldValues(.., &vv); .. }`), or one variable for one meaning
  (cm_FightWeaponLoss: `char v = 1`, `v = -neg` on the loss path; the original's one slot holds both). Written so the
  functions are byte with the callee known AND unknown, and the split is free. The probe for any "the callee must be
  known" fall: make that one callee an unknown external in a `dcp try`, then look for the plain local that makes the
  unknown form byte. (cm_ExecCutscene shape -> byte, cm_RunAuction 62 -> 36 ins on the way.)
- **cm is three objects** (A 0x4c3180.., fight 0x4cd9d0.., scene 0x4d8b50..; cmdump since s11): .data blocks, the .bss
  blocks (0x6f0cd0 / 0x6f0ce0 / 0x6f0d68) and the common groups (scene 0x925510.., fight 0x930380.., cm 0x935da0..)
  agree. A common read only by an earlier object can lie in a later object's group (cm_OrderTicks, read by
  cmfight's cm_RunCutOrders, in the scene group): that object defined it tentatively, the reader only declared it.
- **The pair test of the common area** (`tools/exe/commongaps.py`): between two neighbouring commons of the original
  the gap must equal LINK's alignment pad of the second (next power of two of its size, max 32), whatever the name
  order. A larger gap with no reader is the first object's own space (an unreferenced common is dropped by
  /OPT:REF, so it can't be a datum of its own): kb_Map / kb_Down / kb_Pressed [512], main_NetSaveName 257..288 bytes,
  D3DZBUFCTX 65..80 (its twin D3DTEXFMTCTX is 0x50). A pad the next one's size can't explain means the next one is
  bigger: cm_LobbyStateFlags is a 17..32-byte object (open / changed / closed: cm_LobbyState hands out its address);
  snd_TimeCopy + the seven rain loops 8 apart are one 72-byte object (SNDRAIN). Check the codegen before taking a
  merge: cm_LobbyChat + cm_LobbyChatNext as one ring object took cm_LobbyAddChat byte -> draft (the original keeps
  the index in a register across the line stores: two objects); SNDRAIN moved nothing.
- **The +32 after huge commons is cl's COMM bump** (ai_Evidence, d3_Noise, txt_Keys / Random / Names / Ptrs,
  ch_Ground0): it depends on the original names (s8), commongaps lists it as unexplained; leave it for the naming end.
- **A common another object owns can't be written yet** (decompanion bug-reports/common-owned-by-another-unit.md):
  d3s_EdgeX / EdgeY lie in d3_engine's group (d3_engine tentatively defined them too, first in link order), but
  defining them in d3_engine.c fails dcp link's M.layout (the pair exemption needs the reader's unit to define them)
  and defining them in both units fails L.src (two definitions). Left in d3s.c.
- **Named char arrays of a later unit inside an earlier unit's run** (gm_TagTmp "tmp" in cm's run, f3gmt's
  "f3_gm:lp..INST / PROT" tags and the rank log format in f3gmt's run): literals at the users, as s10 / s11.
  A replacement through Python's re.sub turns `\366` into the raw byte: write escapes with str.replace or check
  the line afterwards.
- **Unread .data inside a plain block** (holes.py --section data, plus an image dword scan): f3gmt 0x61a65c (16
  bytes), sv 0x623858 / 0x62389c, gfx 0x5febd8, and sv's first datum 0x623780 (int[3] after ls's last string):
  declared with the original's bytes. With them .data's end is exact (the s11 "-48 accepted" was these, not only
  removed-function literals), and every datum below LIBC's .bss sits at its address (765).
- **State at s12's end**: steps 47 -> 36; .rdata and .data ends exact; the common area now starts at +64: LIBC's .bss
  (+48 at __fmode, s10's open member order: fpinit / _file / crt0dat pulled by removed functions' externals), which
  the old .data -48 had cancelled. So no group is "placed" until that is fixed; the extents are the measure:
  commongaps finds no unexplained pair except cl's bumps; groups outside the name-order range: amt (the bump), txt /
  ch (bumps), d3s (EdgeX / EdgeY above).
