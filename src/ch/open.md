# ch.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "ch_*" error strings
- Boundaries: separated (plausible): "ch_*" tag; neighbours udp.c / evtc.c coherent groups with their own string blocks
- Range then: 0x402140..0x40b530
- Was: unk_402140.c (round 12)

## Open work
- **headers (2026-10-30, typing pass 2)**: head.c includes d3.h, d3_anim.h, d3_sm.h, path.h, ch.h, data_views.h; keeps 2 cast / view macros with @original byte evidence (the address and the matched caller are in each comment): d3anim_StartAnimCh, util_RandomU; views / casts without byte evidence of their own (weak definitions or drafts' spellings, notes/headers.md debt item 1): d3t_CamMoveI.
- Round 7 leftovers: ch_InitWalk (15 bytes, stack-slot order), ch_ActWalk / ch_ActWalkCells (VC6 drops a
  redundant slot test; notes/units/ch.md); ch_PathToTarget 0x408490 shares their path patterns (try ch7's
  one-return and CH_FLAGW tricks). d3fl_Draw 0x4439e0 (10 KB draft, frame 0x14c vs 0x150), d3fl_Prerender,
  map_Pick, map_PickScreen (notes/units/d3_fl.md). d3fl_FarDist (35 B), d3if_SelectScene (50 B) (notes/units/d3_interface.md). Round 8: d3if_ScreenBox
  0x4486e0 (fmul vs fmulp, notes/units/d3_interface.md).
- see notes/units/tr.md, Open work: "util_Random: ai.h, ch.h and tr.h disagree on its prototype (hv3). ..."
- see notes/units/d3_anim.md, Open work: "Nonmatching functions (177 after round 17): `uv run ledger check --verbose | grep FAIL`; per-functi ..."
- see notes/units/d3_engine.md, Open work: "Header cleanup from round 7: d3_engine.h's d3_Unk4439e0 is d3fl_Draw(D3FLMAP *, char) and ..."
- see notes/units/tr.md, Open work: "unk_402140.h declares `unsigned short util_Random(int)`; it is `int util_Random(unsigned s ..."

## Worker ch (round 4): 0x4028a0..0x404360 [ch]

### Report
All 20 functions of the scope are in src/game/unk_402140.c (address order, after
ch_UpdatePositions). A header change also matched the two round-3 leftovers
ch_UpdateFades and ch_SetVisible.

TU: unk_402140.c continues through the whole scope (it calls 0x404360, 0x404810, 0x404a80 of ch2's range);
nothing in the scope starts a new TU. No Ghidra-missed functions: the scope is contiguous ret/nop padding.

Names: 0x4033c0 ch_MoveToScene (ch_MoveCharacter2Universe), 0x403620 ch_Unk403620 (drags the transport).

#### Nonmatching (3, details below)
- 0x402ba0 ch_Unk402ba0 (ch_Updatelowpoly): block order of one if/else and a re-test at the end.
- 0x403ab0 ch_AnimDone: 3 instructions (zero-extension of the returned bit).
- 0x403dc0 ch_CheckAniMorph: one re-test the original keeps, and stack slot order.

#### Header changes (only include/unk_402140.h, the TU's own header; no shared header touched)
- CHARACTER fully laid out (0x244 bytes): id, kind, name, unk02c/030, slot (0x70), morph (0x7c), next (0x80),
  nextloops (0x84), unk085 (char), body (0x130), fullname (0x170), unk1a0, unk228 callback, lowname (0x230).
  **flags (0x8c) are now `unsigned short` bitfields over 0x8c..0x8d** (was unsigned char + pad); this is what
  matched ch_UpdateFades/ch_SetVisible, and ch_CreateMesh's `or byte [esi+0x8d],0x10` (= flags.unk12).
  ch2's code (0x404360..) should use the same struct; merging the header needs care.
- New: CHTRANSPORT (what unk124 points to, CH_TRANSPORT()), CHANIMFLAGS, CHMOTION/ch_Motions (0x14696e0,
  0x4c-byte records, name at +0x25), D3OBJBITS / D3OBJ_BITS(), D3OBJ_UNK048/078/200, D3MATERIAL_UNK958,
  D3OBJ_ANIM() (D3OBJECT.unk1cc is a D3ANIMCTL), externs ch_Unk5f122c/5f1294/5f1298/5f129c/5f12a0 (.data of
  this TU, not defined yet), ch_LowPoly (0x631064), and prototypes for d3_Unk447fd0, vfs_Unk42ed10,
  d3anim_Unk4315c0, d3_GetHeightAt (D3SM), d3_Unk4463b0, d3_Unk448ac0, lp_Unk52a040, err_Message,
  d3t_ReleaseObjectTextures, ch_Unk404360, ch_Unk404810.
- ch_Unk403620's second parameter is now `char now` (was int; ch_SetPos still matches).
- d3.h's prototypes of d3_Unk44e4a0 (void, int flag) and d3_Unk435550 (D3ANIMFLAGS) are wrong for this TU:
  unk_402140.c hides them with `#define x x_void` around `#include "d3.h"` and unk_402140.h declares
  `D3ANIMDATA *d3_Unk44e4a0(char *, char *, char)` and `int d3_Unk435550(void *, char *, CHANIMFLAGS)`.
  unk_402140.c now also includes d3_anim.h and unk_45db40.h before its own header.

#### General VC6 lessons
- A 12-byte flags struct passed by value whose middle dword never touches memory (`mov esi,eax; and al,..;
  or al,..` then copied from esi into the argument) is a struct of separate fields (int-sized bitfield word
  at +4); VC6 enregisters that member. `unsigned char b[12]` or unions with arrays keep it in memory.
  The store order of the other fields was brute-forced (CHANIMFLAGS, 3 functions).
- `*(unsigned short *)p |= 0x1000` compiles to `or byte [p+1],0x10`; an `unsigned char` `|= 0x10` gives
  load/or/store. So a byte op on the high byte of a flag word means a 16-bit (bitfield) type.
- `k = (float)(70.0f / (float)sqrt(x))` vs `k = 70.0f / (float)sqrt(x)`: same instructions, but the
  extra cast/parentheses moved two fstp's past the argument pushes (matched ch_Unk403620).
- `!(a - b < -5.0)` gives `test ah,5; jnp`; `a - b >= -5.0` gives `and eax,0x100; jne`.
- `d3anim_Release((T *)f(name), 1)` pops f's arguments immediately; through a local (`a = f(name);
  g(a, 1);`) all pops are deferred into one `add esp,0x20` (twice in this TU).
- A varargs loop with the count in a stack slot decremented to zero (`dec eax; mov [cnt],eax; jne`) and the
  arg pointer re-stored each iteration came from `for (i = 0; i < n; i++) anim = va_arg(ap, char *);`;
  `while (n-- > 0)` keeps the parameter as counter.
- Searches `for (; ch_List[i] != ch && i < CH_MAX; i++);` followed by `if (i < CH_MAX) {success}` (success
  inside the if) gave the original's layout; `if (i >= CH_MAX) return 0;` puts the failure inline.
- Initialized `D3VECTOR4 rot = {0, 0, 0, 0}` gives four `mov [esp+x],0` at entry, before the null check.
- `strcpy(dst, "")` is inlined as `mov dl,[literal]; mov [dst],dl` (the pooled "" at 0x631090).

#### Outside the scope
- 0x404360 is `ch_Unk404360(ch, n, ...)` (varargs like ch_PreloadAnims, called with (ch, 1, "gehen")),
  0x404810 is called by ch_Delete before the body is released.
- 0x52a040 (lp_Unk52a040) creates the low-poly body (CHARACTER.unk22c), called by ch_SetLowPoly.
- .data 0x5f1220..0x5f12ac belongs to this TU (two ints -1, 1, 1, "normal_s"[0x60], 1.0f, -1, -1, -1, 2, 1,
  0x100, 0x100) and is only declared extern so far.
- D3OBJECT.unk1cc is the D3ANIMCTL (slots[0].data at +0x84, slots[1].data at +0x100), not a D3OBJINFO.

### Nonmatching attempts
#### ch_AnimDone 0x403ab0
Code matches except the return: original `xor ecx,ecx; mov cl,[eax+0x6d]; mov eax,ecx; shr eax,5; and eax,1`,
ours `mov al,[eax+0x6d]; shr eax,5; and eax,1`. Tried (no change): `(unsigned)` / `(int)` / `(unsigned long)`
casts, `(b & 0x20) >> 5`, a local, byte and int bitfield overlays, a ternary for the whole return.
Fixed so far: int return type, `if (!ch->slot && !ch->next) return 1; if (!s) return 0;` (re-reads the slot).

#### ch_CheckAniMorph 0x403dc0 (about 15 variants so far)
Fixed: index `i = unk150 != -1 ? unk150 : 0` computed before the sprintf (gives the register allocation),
d3_Unk433f40 result through a local (deferred pops), `loops = ch->nextloops` local, signed char unk085 (shares
the -1 kept in ebx), memset + field stores for the second flags, `if (ch->slot) {...} else sprintf`.
Left: (1) the original re-tests `!ch->morph && !ch->slot` before the second block (morph kept in edx), VC6
removes our re-test (tried: if/else-if, separate ifs, swapped order, a local copy; only a volatile read keeps it).
(2) stack slots: original released/n at 0x10, obj 0x14, flags 0x18; ours obj 0x10, flags 0x14, released 0x20
(tried: initialization places, declaration in block, obj init order).

#### ch_Unk402ba0 0x402ba0 (ch_Updatelowpoly, about 20 variants)
Code matches block by block; left: the block order of the camera-distance if/else. The original has the far
block (hide body, show low-poly, sync pos/rot) inline and the near block (show body, hide low-poly) after it;
VC6 gives us the near block first whatever we write: then/else swapped, `>=`/`!(<)` / `<` compares, the
distance in an `__inline` helper, goto forms. Also the slot checks at the end: the original re-tests
`s`/`slots[0].data` (Ghidra shows `if (s) {if (!d) {...} if (s) goto L;} if (d) {...} if (!s) return; L:`);
if/else-if chains and separate ifs both give our shorter form.
Fixed: `(lp != 1 || !cam || !(dist < 1500.0f))` (float compare, jp/jnp), the scene branch as
`if ((a == X && b == Y) || !b) show; else hide;`.

## Worker ch2 (round 4): 0x404360..0x405810 [ch2]

### Report
All 25 functions of the scope are at the end of src/game/unk_402140.c, after a
`---- 0x404360..0x405810 (worker ch2) ----` marker. The TU does not end inside the
scope: 0x405810 and later (0x405ad0 is called with "sitzend/aufstehen") are still character code.

Names: 0x404e60 ch_Unk404e60 (save ground types), 0x404ed0 ch_Unk404ed0 (restore them), 0x404fc0 ch_Unk404fc0
(set ground in a diamond).

#### Nonmatching (4, attempts below)
- 0x4047a0 ch_RemoveAction: 4 asm lines, only `mov ecx,0x65` / `xor eax,eax` order before rep stosd.
- 0x404a80 ch_Unk404a80: 5 asm lines, placement of the `xor eax,eax` for the two `return 0`s.
- 0x404360 ch_LoadLowAnims, 0x4044a0 ch_AttachAni: the D3ANIMFLAGS initialisation (see below).

#### Header changes (include/unk_402140.h)
- Changed: `ch_Unk404a80` returns `int` (was declared void; it returns the ground type). Callers ignore it.
- Appended a block marked `0x404360..0x405810 (worker ch2)`: includes d3_anim.h and unk_45db40.h; CHACTION
  (0x194-byte queue entry, 2000 in ch_Queue at 0x631050) and CHACTDEF (0x4c bytes, 64 in ch_ActDefs 0x1469700);
  CH_* accessor macros for CHARACTER fields the struct does not name (0x2c, 0x70 current D3ANIMSLOT, 0x80,
  0x85, 0x8c flags as byte/word, 0x128 queue head, 0x130/0x230 model names); externs for the ground buffers
  (0x146aa00, 0x14aaa20), dirty rectangle (0x5f12a8/ac, 0x63107c/80), 0x5f12a0, 0x5f12a4 ch_Enabled,
  0x631060, 0x631084/88 ch_Saved0/1, 0x5f1224 ch_LastUpdate, d3t_CamMoving 0x6310e4 / 0x6310e8; prototypes
  for d3_GetHeightAt/d3sm_GetVertex (D3SM), map_Pick (0x4463b0, float *y here, char * in d3_snd.c),
  d3anim_Unk4315c0/435810/435870, d3t_PlaceObjectV, vfs_Unk42ed10, util_Random (0x42ec00, returns
  unsigned short), and the ch worker's range: ch_Unk403950, 403ab0, 403b00, 403dc0 (my guesses at their
  signatures, check against ch's), plus 0x4072a0, 0x40b270, 0x40b2f0, 0x405ad0 beyond the scope.
  `d3_LoadAnimFile` is a cast macro of d3_Unk44e4a0 (d3.h declares it void, it returns int).
- src/game/unk_402140.c now also includes <stdarg.h> (top of file).
- CHARACTER+0x128 (`CHANIM *anim`) is really the CHACTION queue head (type at +9, '-' = walk action).

#### General VC6 lessons
- Redundant null tests in the original (`test eax,eax` repeated on a value already tested) come from re-reading
  the field (`if (CH_SLOT(ch) && ...)` each time) instead of a local: VC6 threads jumps over tests of a local but
  not over a CSE'd memory load (ch_ActAnim, ch_UpdateAction).
- Two identical blocks were tail-merged by our build but not by the original; `if (!x) return -1; return y;`
  instead of `if (x) return y; return -1;` at the end kept them apart (ch_ActAnim).
- A direct `or/and BYTE PTR [reg+0x8c],imm` RMW right after a call: write the flags as `unsigned short`
  (`*(unsigned short *)&ch->flags |= 4`); VC6 narrows it to the byte op. As `unsigned char` (byte or
  bitfield) it loads the byte early and stores later. This probably also fixes ch_SetVisible / ch_UpdateFades
  (0x402480 / 0x4022e0, ch worker's scope, not touched). The word-wide tests in ch_Update (`mov cx,[..+0x8c];
  test cl,0x32; test ch,ch; js`) confirm the flags are 16 bits.
- `max(1, x)` vs `max(x, 1)` and `min(size - 2, x)` vs `min(x, size - 2)` choose jge/jg and operand order;
  clamps `if (h->size - 2 < nx)` likewise. A loop counter initialized at its declaration (`int i = 0;`) is
  zeroed at function entry (ch_Unk404fc0).
- Store order of struct members follows source order (ch_InsertAction: `a->func = ...; a->ch = ch;`), and
  comparing `a->args[k]` directly instead of copying them into locals kept the original register choice.
- `if (f(..)) g(h, nx, nz, out); else g(h, x, z, out);` is the source of two pushes in each branch around a
  shared call (ch_RandomNear).
- `near` is a macro in windows.h (empty); a local named near breaks the build.

#### Outside the scope
- The character TU continues past 0x405810 (0x405ad0 "sitzend/aufstehen" helper, 0x40b270/0x40b2f0 queue
  helpers, 0x4072a0 path search taking (D3SM *, from, to, x, z, *x, *z, 600, 0)).
- ch_Ground0/1 are 0x40020 bytes apart (256*256 ints + 0x20).

### Nonmatching attempts

#### ch_RemoveAction 0x4047a0 (4 asm lines)
Only the order of `mov ecx,0x65` / `xor eax,eax` before both `rep stosd` differs (original sets the count
first). prev/next locals fixed everything else (`prev = a->prev; if (prev) prev->next = a->next; next = a->next;`).
Tried for the clear: memset with sizeof/0x194/0x65*4, ZeroMemory, int loops (index, pointer, counting down,
inline and in an __inline helper; all become the same rep stosd), struct copy from a static zero (movs),
D3_ZERO (asm). None changes the order.

#### ch_Unk404a80 0x404a80 (5 asm lines)
D3VECTOR4 pos fixed the frame (0x18). Left: the original loads `xor eax,eax` before `test esi,esi` (h) and has a
second `xor eax,eax` for the d3_GetHeightAt failure; ours shares one. Tried: early `return 0` before/after
`obj = ch->obj`, obj assigned inside the if, initialized declarations, `return 0` inside the outer block,
`else return 0`, `else type = 0; return type;`, an `int ret = 0` local.

#### ch_AttachAni 0x4044a0 (79 asm lines, mostly the D3ANIMFLAGS init)
The original's init: dword0 = 0, dword2 = 0, dword0 = 0 again, b[0] = 0 (bl), b[8] = 0|2, b[1] =
`and al,1; shl al,3; or al,0x10`, dword1 kept in ebx and `or ebx,3` (a dword op). VC6 factors every spelling of
`0x10 | (loop & 1) << 3` we tried into `((loop & 1) | 2) << 3` (cast, +, local, |= split). A bitfield overlay
(unsigned char or unsigned int bitfields) is much worse (no constant tracking). The if/else on the two inlined
strcmps (`if (strcmp(a) && strcmp(b)) load(0); else load(1);`) gives the original block layout, but the strcmp
result registers are swapped (orig edx then eax) and our function is 12 bytes longer.

#### ch_LoadLowAnims 0x404360 (45 asm lines; kept: r = call; memset; *(int *)&f = 0; byte ops; ((int *)&f)[1] &= ~3)
Same flag init (b[1] = 8, dword1 &= ~3 in esi, b[8] |= 2, b[0] = 0 from a zero kept in ebx). In the original the
init is scheduled after the d3_Unk433f40 call, before testing its result; VC6 hoists our stores above the call
whatever the source position (r = call; init; if (r || ...)). The original's va_list lives in memory (frame
0x214 vs ours 0x210) and the function keeps 0 in ebx (`cmp eax,ebx` for n > 0).

## Worker ch3 (round 5): 0x405810..0x406e40 [ch3]

### Report
The scope's 22 functions are at the end of src/game/unk_402140.c after a
`---- 0x405810..0x406e40 (worker ch3) ----` marker. Ghidra knew 13
starts; 8 more are action callbacks that are only referenced as function pointers (0x405870, 0x405a20,
0x405b30, 0x405d40, 0x405fb0, 0x406210, 0x4062d0, 0x4067f0; the last three are registered at 0x40b0dd..
0x40b10a, ch4's range). The TU does not end in the scope (0x406e40.. is ch4's).

0x406c60 ch_SetHandObject: 1 left hand 0x68, 2 right hand 0x64, 3 head 0x6c.

#### Nonmatching (1)
- 0x405fb0 ch_ActDroppingObject: 32 asm lines, only which register holds a->ch (see below).

#### Header changes (include/unk_402140.h)
- ch2's prototype `void ch_Unk405ad0(CHARACTER *, char *)` replaced by `int ch_StandUp(...)` (it returns 1/0);
  the call in ch_StopActions renamed accordingly.
- Appended block `0x405810..0x406e40 (worker ch3)`: CHACTOBJ (CHACTION.u as name/name2/unk170..178, via
  CH_ACTOBJ(a)), CH_SLOT2 (CHARACTER+0x74, the sitting animation), CHACT_UNK18C, CHACT_HASPOS/CHACT_POS
  (CHACTION+0x17c/0x180), D3MATERIAL_NAME104, CH_UNK23A, CH_UNK038, CH_HAND; CHOBJLIST, CHPATH/CHPATHPT,
  CHWALKDEF; prototypes of all scope functions plus d3anim_SetFrame (0x435950), d3_Unk446f30, d3_Unk43d6c0.
- No changes to other shared headers.

#### General VC6 lessons
- A builder whose null check is `test eax,eax; jne; ret` and which keeps eax live to the end (strcpy through
  dl, not al) returns the pointer: `if (!a) return 0; ...; return a;` (ch_QueueAnimSpeed, ch_SitDown).
- `if (!x && c) f(0, 0); else if (c) f(v, 1);` (semantically the same as testing x, then c in each branch)
  gives the original's "test x; cmp c twice" with no retest of x: VC6 threads the second, identical compare.
  Writing `else if (x && c)` keeps a retest of x (ch_ActMove2Universe).
- A value returned 0 from a register that is later reused for another variable (`xor edi,edi` ... `mov
  eax,edi` on failure, edi = obj later) is a variable assigned 0 *as a statement* at that point and a single
  `return type;` at the end with nested ifs; VC6 splits it into separate webs (ch_FollowGround). The
  declaration initializer `int type = 0;` zeroes it at entry in another register.
- Byte copies whose second load is zero-extended (`xor ecx,ecx; mov cl,[p+1]`) and both loaded before the
  stores: the values went through int locals (`x = p[i].x; z = p[i].z; out[n].x = x; out[n].z = z;`);
  that also moved `step` into the dead parameter slot as in the original (ch_ThinPath).
- Order of zero-initialized local vectors (`D3VECTOR4 pos = {0,...}; D3VECTOR rot = {0,...};`) follows the
  declaration order (ch_SetHandObject). `memset(&list, 0, sizeof list)` gives a single `rep stosd`;
  `= {0}` stores the first dword separately.
- The original source has a missing comma in a string table: `"dummy_AUSGANG" "dummy_LAUFEN1"` is one
  27-byte initializer, so names[14] stays empty (ch_FindDummyCB).
- Swapping if/else so the short block is the `else` again fixed the block order (ch_ActTakingObject).

#### Outside the scope
- 0x14696e0 (ch_Motions in the header, a CHMOTION array) is passed to ch_ThinPath as a CHPATH
  ({int n; int; CHPATHPT *pts}) by 0x408322/0x408637/0x408967 - the "motions" type at that address is
  probably wrong.
- ch_DeclareAction registrations for 0x4067f0, 0x406210, 0x4062d0 sit at 0x40b0dd..0x40b10a.

### Nonmatching attempts

#### ch_ActDroppingObject 0x405fb0 (32 asm lines)
Everything matches except the ch_SetHandObject call in the frame check (`if (CH_SLOT(ch) && frame >=
args[0])`): the original reloads `a->ch` into ecx, ours pushes the local ch (edi), and the register names of
the last block follow from that. In the matched ch_ActTakingObject the same reload comes from the join after
ch_SetNextAnim; here the count==0 path always returns, so there is no join. Tried (none changes it):
`if (!unk080) {...} return;` in the first block, an else around the rest, `a->args[0] <= frame`, ch assigned
as a statement, `((CHACTION *)a)->ch`, raw offset `*(CHARACTER **)((char *)a + 0x14)`, `a->args[-7]`, an
__inline helper for the call, `else (void)0`, a slot local, CH_UNK080(a->ch), a re-test of a->count after a
shared unk080 test (fixes the reload but adds the test, loses the duplicated unk080 test), a flag
`first = 1` tested before / after the unk080 test, a local copy of count, no ch local at all (worse).

## Worker ch4 (round 5): 0x406e40..0x408690 [ch4]

### Report
12 functions in the scope (10 Ghidra starts plus two it missed: 0x406e60 and the 5-byte
thunk 0x4076b0), at the end of src/game/unk_402140.c under `---- 0x406e40..0x408690 (worker ch4) ----`.
The TU does not end in the scope: 0x408690 (next function) is still a character
action (releases the morph, d3_ReleaseTexture/0x433f40/0x434b70).

#### Nonmatching (5, attempts below)
- 0x406e60 ch_ActTurn (turn action, "bewegung/dreh_90_links/rechts"): D3ANIMFLAGS init and key scheduling.
- 0x4072a0 ch_FindFreeCell (was ch_Unk4072a0): code identical, only stack slot assignment (96 lines).
- 0x407790 ch_GetWaypoints: 10 asm lines, x/z stack slots swapped.
- 0x4079b0 ch_GetNewPath (its mem_Alloc tag is "ch_GetNewPath"): flow matches, slots and some scheduling.
- 0x408490 ch_PathToTarget (tag "ch_t:waypoints"): 34 asm lines, x/z stack slots swapped (both pairs).

#### Header changes (include/unk_402140.h only; no shared header touched)
- CHARACTER: `pad08e[0x96]` split into `unk08e` (0x8e), `contacts[16]` (0x90, `struct CHCONTACT {ch; flags}`),
  `pad110[0x14]`; `pad1a4[0x84]` split into `float unk1a4` + `pad1a8[0x80]`. Size unchanged.
- CHACTION: `pad18[0xc]` is now `D3VECTOR rot` (turn actions); union `u` gained `turn.started`; `u.walk` gained
  cur (0xf8), unk0fc, target (0x110), end (0x124), tpos (0x134), dest (0x144), unk154, unk160 (layout unchanged).
- ch2's `ch_Unk4072a0` prototype renamed to `ch_FindFreeCell` (caller ch_RandomNear updated) plus
  `#define ch_Unk4072a0 ch_FindFreeCell` so ch3's code still compiles; drop the alias after the join.
- New in the ch4 block: CHMAPNODE, CHWAYPT, CHKEYFLAGS (+ `d3_Unk433d70K` cast macro), CH_ANIM078, CH_LASTFRAME,
  CH_UNK240, CHCONTACT typedef, externs ch_PathMap (0x63108c, D3SM *), ch_Path (0x14696e0, PATHOUT; note it is the
  same address as ch_Motions above, so one of the two is mis-based), ch_PathBuf (0x14686e0), ch_PathOffset
  (0x631068), and a guessed prototype `int ch_Unk406bc0(CHARACTER *, PATHOUT *, unsigned char *, D3SM *)` for ch3's
  0x406bc0 (path compaction). The orchestrator must reconcile that name/signature with ch3's definition.
- src/game/unk_402140.c now also includes unk_424f30.h (path_Find, PATHOUT).

#### General VC6 lessons
- `f(a, c ? K1 : K2, r)` compiles to setcc/sbb arithmetic. The original's "push K1 / push K2 in the two arms, one
  shared call" is `if (c) f(a, K1, r); else f(a, K2, r);` (VC6 tail-merges the calls); an argument shared by both
  arms must be computed into a variable first, or it is computed in each arm.
- `(float)(d * 0.01f) * 0.5f` keeps two `fmul DWORD`; `d * 0.01f * 0.5f` (any parenthesization) folds to one QWORD.
- A sum of two distances keeps each root separate only with reused `dx/dy/dz` locals
  (`dist = sqrt(..); dx = ..; dist += sqrt(..)`); one expression interleaves both and spills.
- A real struct member array (`ch->contacts[i]`) lets VC6 fold the offset into each lea; a cast macro
  `((T *)((char *)ch + 0x90))` is CSE'd into one register.
- In three functions our two address-taken `int x, z` locals got each other's stack slots. Declaration order,
  names, block scope and use order did not change it; a dead `z = 0;` at the top or storing z first did.
- Ghidra misses functions after short ones; check `nop` padding in uv run disasm output for unlisted starts.

#### Outside the scope
- 0x408690 continues the character TU. 0x406bc0 (ch3's scope) is called as (ch, &ch_Path, buf, h) and returns the
  new path length; 0x404ed0 is called with the height map after path changes.

### Nonmatching attempts

#### ch_FindFreeCell 0x4072a0 (96 asm lines, all stack slots)
Code and frame size match. The distance sum needed explicit `dx/dy/dz` locals reused for both roots
(`dist = sqrt(..); dx = ..; dist += sqrt(..)`): with one expression VC6 interleaves both roots and spills.
`max(1, z - r)` (not `max(z - r, 1)`) gives the `jl` form. Left: the three D3VECTOR4 locals are laid out
dst, v, src (orig src, dst, v) and the dead parameter slots used for loop temps differ. Tried: declaration
order, renaming (no effect), struct copies of from/to, 12-byte src, scalar sx/sy/sz (frame 0x34), dead init
of src, swapped branch order of the two search loops.

#### ch_GetWaypoints 0x407790 (10 asm lines)
The original keeps `out` in the frame slot and walks the parameter itself (`start = out; ... out++`).
`n = i + (ch->unk124 ? 2 : 1)` gives `setne; inc; mov; add`. Left: x and z (address-taken, live in the dead
ch/out parameter slots) are swapped. A dead `z = 0;` at the top, or storing `out->z = z` before `out->x = x`,
swaps them as in the original (but adds code). Tried: declaration order, inner block, `int xz[2]`, reusing i/n,
parenthesized uses, `(void)z;`, `if (0) z = 0;`, all 6 orders of the three stores.

#### ch_PathToTarget 0x408490 (34 asm lines, stack slots only)
Same x/z slot swap as ch_GetWaypoints, for both pairs (x1/z1, x2/z2). Tried: declaration orders,
`z1 = z1;`, dead `z1 = 0;` (breaks more), clamp operand order, store order after FindGround/FindFreeCell,
`int p1[2]` arrays.

#### ch_ActTurn 0x406e60
Flow and frame match; the original builds the D3ANIMKEY/flags block twice (both branches of `if (CH_SLOT)`)
with the flags `b1 |= 0x14; b1 |= 8` as one load/or/or/store from memory after a dword zero store, and
dword 1 of the flags never stored (enregistered field). Ours either folds the byte to the constant 0x1c,
hoists the identical flag init above the branch, or (memset) stores dword 1. The frame access needs
`fr = &frames[n - 1]`; the original loads turn.z, turn.y, turn.x and unk00 before any store.

#### ch_GetNewPath 0x4079b0 (2768 vs 2736 bytes)
Matched shapes: the `k == last ? 0xd : 1` and `oa->unk154 != -1 ? 0 : 0xd` arguments are if/else calls; the
min(9, steps) for the second goes through `r`; `(float)(d * 0.01f) * 0.5f`; the rest buffer through a pointer
`q`; CHARACTER.contacts as a real member. Reusing cx/cz/i/tz for sz/fx/fz/ret (the original's slot sharing)
brought the frame from 0x9c0 to 0x9bc (orig 0x9b8). Left: the slot permutation of the scalar locals (gx/gz
swapped again), the memset/loop pointer base of the contacts (orig `lea ecx,[ebp+0x94]` separately), the
`steps * m` operand (orig loads steps into a register), `k = unk124 ? cur - 2 : cur` (orig lea/jmp form; the
if/else spelling compiles identically to ours). Tried also: `d * 0.01f / 2`, two statements, inline helper,
`min(steps, 3)`, swapped contacts store order, `m * steps`.

## Worker ch7 (round 7): 0x408690..0x40b580, unk_402140.c (walking along a path) [ch7]

### Report
- Nonmatching: 0x4097d0 ch_ActWalk (action 0x2d; one redundant compare missing),
  0x409bd0 ch_ActWalkCells (action 0x3a, 3.3 KB; register allocation in the cell-skip loop + same compare).
- ch_InitWalk 0x408690 down from 751 to 15 bytes (stack slot order of the target
  cell vs y).
- TU boundaries: Ghidra's 0x4093d0 (5.4 KB) is three functions: 0x4093d0, 0x4097d0, 0x409bd0 (starts after
  nop padding). Nothing else missed in 0x408690..0x40b580.
- Headers: only unk_402140.h (the TU's own header), ch5 block: `extern float ch_Unk5f1290` (0x5f1290, 1.0),
  prototypes ch_ActWalk / ch_ActWalkCells (both `void`, they never set eax); ch6's `int ch_Unk4097d0` /
  `int ch_Unk409bd0` declarations were removed and ch_Open casts the new names to CHACTFUNC. No shared header
  changes.
- General VC6 lessons:
  - `unsigned char` test `cmp BYTE PTR [x],1; jae` is `x < 1`; `!x`, `x == 0` and `x <= 0` give `test`.
  - A local that is returned on one path and holds the call result on another may live in two registers:
    `a = 0; ...; if (...) { a = f(); if (a) {...} } return a;` (one return at the end) gives the original's
    `xor edi,edi ... mov eax,edi` plus the result kept in eax; an early `return a` inside the if does not.
  - A frame bigger than all used locals: look for an array the callee fills (`CHARACTER *others[32]` for
    ch_FindNear, of which only others[0] is read). Unused locals are dropped, so a dummy array never works.
  - `t = c ? x : 1.0f - x` keeps x on the FPU across the branch (`jne; fsubr`); `t = x; if (!c) t = 1 - t;`
    stores and reloads t.
  - `!(t > 0.0f)` is `and eax,0x4100; je`, `!(t < 1.0f)` is `test ah,5; jnp` (compare the forms in
    vc6-matching.md for `<=` / `>=`).
  - Two functions tail-merged to one call: write the call at the end of each branch (ch_WalkRotate's
    left/right turn), the first copy survives.

### Matched
- 0x408e60 ch_NextWaypoint: `unsigned char` test `cmp BYTE PTR [x],1; jae` is `x < 1` (`!x`, `== 0`, `<= 0`
  give `test`); `test al,al; jbe` is `x > 0`. `fsqrt; fstp st(3); fstp st(0); fstp st(0); fstp [dist]` needs
  the root in a float local first (`f = (float)sqrt(...); a->u.walk.dist = f;`). `speed * min(2.0f, f)`
  compiles to `fadd st,st(1)` / `fld st(0)` (the `* 2.0` becomes an add).
- 0x4093d0 ch_WalkRotate: `o->unk084.y` loaded onto the FPU right after the call and popped on the snap
  path is a `double rot` local; `test ah,0x41; jp` + `and eax,0x100; je` is `!(ang <= 0.04 && ang >= -0.04)`;
  `fabs(ang)` kept on the FPU across the else-if chain is CSE of repeated `fabs(ang)`; the rotation call is
  written at the end of both the left and the right branch (tail-merged into the first copy);
  `(float)sqrt(...) > dist * 1.4` pops the dead squares before the compare.

### Nonmatching
- 0x4097d0 ch_ActWalk (action 0x2d): one difference left. The original tests the slot a third time:
  `started? ; slot == 0 -> start ; f6d_20 -> start ; slot != 0 -> skip ; start` (`cmp eax,ebx; jne` after the
  bit test). Our `!started && (!slot || f6d_20 || !slot)` drops the third test. Tried (all drop it or add a
  `started` retest): `== 0` / `== NULL` / `ch->slot` / `CH_XSLOT` spellings, `a->ch` for one term (reloads),
  ternary `slot ? f6d_20 : 1`, `!(slot && !f6d_20 && slot)`, nested ifs, `else if (!slot)` with the call
  duplicated (nested: dropped; flat `else if (!started && !slot)`: extra started test), goto into the call,
  call folded into the condition, a static __inline helper for `!slot || f6d_20`, `f6d_20 == 1`/`!= 0`,
  a raw byte test. The same pattern sits in ch_ActWalkCells (0x409d91).
- 0x409bd0 ch_ActWalkCells (action 0x3a, 3.3 KB): inline copies of ch_InitWalk / ch_StartWalkAnim /
  ch_WalkSound / ch_NextWaypoint / ch_WalkRotate (with other constants and a sign bug: the right turn's
  first branch subtracts). Left: the redundant slot test above, the frame is 4 bytes larger (n gets a stack
  slot) and the cell-skip loop's bound `min(0x100, n + 32)` is hoisted in ours (the original recomputes it
  each iteration from n in edx, loop counter i in edi). Writing the loop with a separate start `n` and
  counter `i` (`n = cur + 1; for (i = n; i < min(0x100, n + 32); ...)`) gives the original loop exactly,
  but then the constant 0 keeps ebx through the step-sound block and `ground` lands in edi (original:
  ground in ebx, spilled to the slot shared with args[1]); the `i = cur + 1; for (n = i; ...)` form (kept)
  gets ground right but hoists the bound. Tried: block-local n/pts/pt, no pts variable, a pt for the
  GetVertex call, while/break loop forms, `ground` initialized / unsigned / declared first, reusing x/z/n
  for ground, an __inline step-sound helper (worse).
  Matched pieces worth knowing: `f * ch->unk1a0 * ch->unk1a4 * K` needs `(f * ch->unk1a0) * ...` to keep f
  first; the cell pointer `pt = &pts[n]` + `pt++` gives the original's `lea eax,[ebx+edx*2]` walk.
- 0x408690 ch_InitWalk (15 bytes): fixed by reversing the `unk160 & 8` branch (target copy first),
  `CH_FLAGW(a->ch) &= ~8` (word bitfield op narrowed to `and BYTE PTR`), and the target cell as `int t[2]`.
  Left: original slots (low to high) z, x, t[0], t[1], y, name; ours z, x, y, t. With scalars tx/tz the two
  int pairs swap instead (24 bytes). Tried: declaration orders, `tz` assigned first, y as float[1], 4-byte
  struct, D3VECTOR (right order, frame 8 bytes too big), x/z as an array, both as arrays, t as a struct,
  dead `x = z = 0` (much worse).

### Matched in this round (round-6 drafts)
- ch_WalkToObject: one `return a;` at the end (see Report).
- ch_Close: `void **p = &ch_Queue[i].u.walk.path;` inside the if, then `mem_Free(*p); *p = 0;`.
- ch_AddActionCB: stores `func, type, ch, count` (as in ch_AddAction), then `next, unk10, func2, unk08`.
- ch_ActFade: `CHARACTER *others[32]` passed to ch_FindNear; `if (ch->unk02c != -1) SetVisible(ch, 0); else
  SetVisible(ch, args[0]);`; `!(t > 0.0f)` / `!(t < 1.0f)` clamp; ternary for the direction.

## Also about this unit
- notes/units/bmp.md, section [start401]
- Type debt: include/ch.h declares sb_GetSample with a void* view (sblib.c / sndc.h: SBSAMPLE*, f3snd.h: int);
  one lint_baseline line added (`sb_GetSample include/ch.h`) at the decomp merge 2026-10-24 so the ch.c / chc.c
  callers keep their void* locals until the unit's sound types are settled.

## match-base 2026-10-24 (match worker, reference updates from d3_anim.c)

- d3anim_Unk4315c0 -> d3anim_ApplyRootMotion (5 call sites) and d3_Unk433b10 -> d3anim_FreePath (4 sites);
  nothing else, byte unchanged (96/112 matched, 7760 B).

## match-base session 2026-10-24 (base-3 worker, shard 3/5)
Reference updates only (renames in a shared header): D3OBJ_UNK078(ch->obj) -> ch->obj->posOffset (the macro is gone). No code change in this unit; the full ledger check
stayed at 2501 matched functions.

## match-base 2026-10-25 (base-0 worker, shard 0/6)

- 0x447190 call sites renamed d3_Unk447190 -> d3if_Delete (the canonical name); byte unchanged. ch_SetHandObject became readable (only failed Q2 on that placeholder).

## match-base 2026-10-25 (base-5 worker, shard 5/6)

- Reference-only rename: ch_Unk404a80 -> ch_PlaceOnGround (0x404a80, puts the character on the ground;
  returns the ground type) and its parameter unk -> force; the Q6 description moved to ch.h. Codegen unchanged.

## match-base 2026-10-26 (base-9 worker, shard 9/10)

Three functions of this unit worked, all handed to match-deep; namespace work lifted five more functions to
readable. New names (evidence in the @deep reasons and the headers): `CHARACTER.lowPolyBody` (0x22c),
`CHARACTER.flags.lowPolyAnimFailed` (0x02 bit of 0x8c), `CHARACTER.lastActionFrame` (0x85; `CH_FRAME` is a
typed access now), `D3ANIMFRAME.time` (0x00, the frame start time; ch.c 0x2332 copies it to D3ANIMKEY.time),
`ch_CurrentGroup` / `ch_CurrentScriptGroup` (0x5f1294 / 0x5f1298; sv.c and f3init.c write the first from the
building record, ch_Reset clears both), `ch_MorphTimeout` (0x5f129c, -1) / `ch_MotionTimeout` (0x5f12a0, 2),
`d3if_Show` (was d3_Unk4471b0, canonical name from names.csv) and `d3_LoadAnimStream` (was d3_Unk44e4a0; d3.h
keeps its @original void prototype under the old name, the ch.c #define trick around d3.h is unchanged).
Readable went up for ch_SetLowPoly, ch_SetNextAnim, ch_PreloadAnims, ch_FindNear300, ch_StopAnim (and
d3anim_CreateMorphAnim in d3_anim.c).

- ch_UpdateLowPoly (0x402ba0, was ch_Unk402ba0): 820 -> 778 differing bytes / 180 -> 84 differing instructions.
  The gain is the spell menu's dup-tail "write the statements in both arms of the if/else": the shared
  `if (lowPolyBody->type != 1) d3if_Show(lowPolyBody, 0)` tail; the kept form pulls that one statement after
  the camera if/else (identical code). Left (match-deep): the far/near block order - the original has the far
  block inline and `jnp` to the near block at 402db8, ours the near block inline and `jp` to the far block;
  the last-bytes pass tried both-arms, pull-after, positive-&& and `!(&&)` polarities (all give our order) -
  and the s/data re-tests at the anim tail (menu reload; the local `s` stays in ecx in the original too, so
  re-reading `ch->slot` / `D3OBJ_ANIM(ch->lowPolyBody)` at the use does not produce them). Q4 left: D3OBJECT
  0x48 / 0x200 have no evidenced name (0x48 = item type | 0x3000000 in the item-sprite spawners f3gm.c
  0x4fbb80 / ls.c 0x51e900, 0x3000000 in ch_CreateMesh; 0x200 = the per-object int d3_WriteObject writes and
  rd_object reads, the item pointer in the sprite spawners).
- ch_AnimDone (0x403ab0): close (8 differing instructions), only the zero-extension of the last return differs
  (original `xor ecx,ecx; mov cl,[eax+0x6d]; mov eax,ecx`, ours `mov al`). Eight width spellings tried this
  session (unsigned int / unsigned short locals and casts, /32, `(unsigned)… % 2`, an 8-bit bitfield view,
  `(b[1] & 0x20) ? 1 : 0`) plus the earlier session's list in the @deep reason - VC6 trims all of them. Q6
  description added (readable).
- ch_CheckAniMorph (0x403dc0): far; the statements match Ghidra 403dc0 hunk by hunk. Left (match-deep): the
  original's redundant re-test `if (!ch->morph && !ch->slot)` at 40408b, six reloads (spell menu) and the
  slots/order/fill hunks. Q2 left: `CHARACTER.flags.unk1` (0x02) - one reader (ch.c 0x848), no writer in any
  drafted source, no evidence for its role.

## match-base 2026-10-26 (base session, main checkout)

Worked the base queue of this unit; six functions escalated to match-deep, one left in progress. Build check
2599/4664 before and after; unitdiff 96/112 matched, 7718 differing bytes, no WORSE; ledger check --against 0
regressions.

- ch_LoadLowAnims (0x404360) -> deep: only the 12-byte D3ANIMFLAGS init scheduling left (original places it
  between d3_FindAnim and its result test, ours hoists it above the call; eax vs ecx as the zero register, so
  test vs cmp). Source position (before/after/split), memset vs explicit dword stores, and the spell dup-tail
  entries were tried. Readability: the dword view is now a local union `{ D3ANIMFLAGS anim; int words[3]; }`
  (Q4 gone).
- ch_AttachAni (0x4044a0) -> deep: one code hunk, the flags init. Key finding: the b[1] value must be written
  with a *multiplication*: `f.anim.b[1] = 0x10 | (loop & 1) * 8` gives the original's `and al,1; shl al,3;
  or al,0x10`; every `<< 3` spelling (and `+ 0x10`) makes VC6 factor it to `((loop & 1) | 2) << 3`, which is
  shorter code but not the original's. That variant re-schedules the whole block (byte diff 198 -> 257), so it
  was reverted. A duplicate `f.words[0] = 0` (the original stores dword 0 twice) also schedules differently.
- ch_PlaceOnGround (0x404a80) -> deep: close (5 differing instructions). Original materialises the h==0 return
  with an extra `xor eax,eax` before the h test and the GetHeightAt return with a second one before the pops;
  VC6 merges our two `return 0` sites into one shared xor after pop edi/pop esi. Guard forms, else forms and
  the spell push-pop entries were tried.
- ch_ActDroppingObject (0x405fb0) -> deep: the original reloads `a->ch` for the ch_SetHandObject call in the
  frame check, ours forwards the local ch; `ch = a->ch;` and alias forms are CSE'd away. Readability: 
  `CHARACTER.unk064` renamed `rightHand` (0x64, hand 2 in ch_SetHandObject), CH_SLOT/CH_UNK080 replaced by
  `ch->slot`/`ch->next`. Q2 left: `CH_ACTOBJ.unk174` (per-action int payload: gate room id vs D3ANIMDATA*).
- ch_ActTurn (0x406e60) -> deep: far (125 differing instructions), the two flags/key blocks. The 406f5f
  branch (`jp` vs `and eax,0x100; je`) spells worse both ways (135/137 asm lines) than the kept nested form.
  Readability: `CHARACTER.turnAnim` named (0x78, the CH_ANIM078 macro is gone), ch->slot typed.
- ch_FindFreeCell (0x4072a0) -> deep: 342/342 instructions, only the stack-slot permutation of the three
  D3VECTOR4 locals (ours dst,v,src; original src,dst,v) and the loop temps in the dead parameter slots.
- ch_GetWaypoints (0x407790) -> deep: close (10 differing). x/z slots swapped at both ch_PosToCell calls plus
  the `n = i + (ch->transport ? 2 : 1)` add/mov order. Renames: `u.walk.unk0f0` -> `pathLen` (cell count, the
  path is 2 bytes per cell), `u.walk.unk154` -> `retryAt` (ch_Time of the last failed path search + 200/225
  ms wait, -1 none).
- ch_GetNewPath (0x4079b0) still in the base queue: its callees are now named (`ch_SaveGround` 0x404e60,
  `ch_RestoreGround` 0x404ed0, `ch_SetGroundDiamond` 0x404fc0) and the `ch_Unk406bc0` cast macro is gone (the
  calls go to ch_ThinPath directly). Readability gaps left: `CHARACTER.unk08e`, `u.walk.unk0fc`,
  `u.walk.unk160`, `CHARACTER.unk1a4`, and the CH_ACTIONS / CH_UNK240 macros (CH_ACTIONS has the typed
  `ch->actions` union member; CH_UNK240 reads 0x240, inside lowname, so it needs a named field).

## match-base 2026-10-26 (main checkout)

Four functions worked, all to match-deep; ledger check 2599/4664 before and after; ch.c unitdiff 96/112
matched, 7718 -> 7716 differing bytes, no WORSE; ledger check --against 0 regressions.

- **ch_GetNewPath (0x4079b0) -> deep.** Far draft, 2291/2768 bytes, 382 differing instructions of 842; every
  hunk read - same statements, one stack slot more (0x9bc vs 0x9b8), scalars permuted (tx/tz 0x1c/0x20 vs
  0x20/0x24, gx/gz 0x2c/0x30 vs 0x30/0x2c, sx/cx 0x34/0x38 vs 0x20/0x24), oa edi vs esi; the contacts loop base
  (ours shares the memset's edx, original `lea ecx,[ebp+0x94]` + `[ecx-4]`) and the rest-buffer loop are the
  same statements. Last-bytes pass: 20 spell reload entries (all re-read fields), dup-tail 407a28/407b3d/408420
  (the k==last?0xd:1 and retryAt!=-1?0xd:1 calls, already two-armed). Tried: dead locals fx/fz/sz/ret removed
  (byte-identical, kept); `rest[nrest*2]` instead of `q++` (2253 bytes but 432 asm lines - reverted: the
  original does walk the rest buffer by pointer, the path by index); copy-back and if/else spellings
  (byte-identical). Readability: `CHARACTER.unk08e` -> `contactSteps`, `.unk1a4` -> `speedScale`,
  `u.walk.unk160` -> `u.walk.flags` with `CH_WALK_TARGET/DEST/RESTART/STOPANIM/ARRIVED` (bits 8/0x10/0x20/
  0x40/0x80, all ch.c sites), `lowname[0x14]` split -> `unsigned char pathType` at 0x240 (the path_Find unit
  type, read by all three path_Find calls), CH_UNK240 macro gone, CH_ACTIONS is the typed `ch->actions`.
- **ch_PathToTarget (0x408490) -> deep.** Gain kept: writing z1's clamp before x1's (independent clamps) made
  the store schedule and the clamp slots match - 34 -> 28 differing instructions, 16 -> 14 bytes; only the
  slot of the two address-taken pairs is left (original x1 in a's dead parameter slot at [esp+0x30], z1 at
  [esp+0x14]; ours mirrored). Declaration order and the x1=ox/z1=oz store order do not move it (the ch4
  pattern of notes/vc6-matching.md). Q7 fixed: `d3sm_FindGround` prototype added to d3_sm.h.
- **ch_InitWalk (0x408690) -> deep.** 15/1056 bytes, 32 differing instructions, all one slot-order issue: the
  original has the target cell below y (z,x,t[0],t[1],y,name; t at frame+0x8, y at +0x10), ours y below t.
  Tried: `int t[2]` declared after y (byte-identical - the VC6 map does not move), name declared first
  (byte-identical), scalars tx/tz (24 bytes / 50 lines, reverted). VC6 puts the address-taken array above the
  float whatever the declaration order.
- **ch_ActWalk (0x4097d0) -> deep.** 8 differing instructions of 323: exactly one redundant `ch->slot` test
  missing (4099c4..4099d3: the third `cmp eax,ebx; jne` after the f6d_20 bit test) plus the 409bcc fill.
  spell: no menu rule at 4099cf. All round-7 spellings already recorded above; this session re-read the flow
  and confirmed the following block's redundant `started` re-test (4099fc) is reproduced by our build.

Unit-wide readability (byte-neutral, full ledger check 2599 unchanged): CH_SLOT/CH_FLAGB/CH_FLAGB2/CH_FLAGW are
gone from ch.c - `ch->slot`, `ch->flags.bytes.lo`, `ch->flags.all`; CH_XSLOT now casts `ch->slot`; CHSLOT.unk60
-> `playSpeed` (0x60, the playback speed the walk code sets from ground type x speedScale).

Q gaps left (evidence, for the next session): `u.walk.unk0fc` (0xfc) is written -1 by all four path builders
(0x408387, 0x408652, 0x408984, 0x409d1e) and read nowhere - the whole ch TU disassembly has only those four
[reg+0xfc] references and chc.c never touches 0xfc. `ch_Unk631058` (0x631058) is read by 10 ch.c sites and
never written in any drafted source. `CHARACTER.unk1a0` only ever read (a factor of CHSLOT.playSpeed).
`ch_Unk5f1290` (0x5f1290 = 1.0f) read only in ch_ActWalk. `D3OBJ_UNK200` = D3OBJECT.pad200[0] (0x200): the
evidence for its role is above (the per-object int d3_WriteObject saves, the item pointer the sprite spawners
store) - a typed d3.h field is the next step, d3.h was not touched. `CH_UNK080` (0x80) is `D3ANIMDATA *next` in
the struct but ch_TakingObject prints it with `%s`, so its type needs a decision. `CH_UNK110` (0x110) has one
byte store 0 (0x4088ec) and no reader.

Follow-up in the same session (after ch_ActWalk): **ch_ActWalkCells (0x409bd0) -> deep** - 2130/3376 bytes,
138 differing instructions of 971 (ours 969, the same two-instruction missing redundant `ch->slot` test as
ch_ActWalk, see its @deep for the spelling list), everything else allocation (the frame 4 bytes larger, the
cell-skip bound hoisted; the noted right-turn sign bug - the 2.0943952f branch subtracting - is already
reproduced). Its readability has no gap beyond the unit-wide ones above.

## match-base 2026-10-26 (base-3 worker, shard 3/6)

ch.c now includes d3_light.h (calls d3_RelightObject from d3.h until that declaration is removed; the include is inert for the unit, no byte change).
