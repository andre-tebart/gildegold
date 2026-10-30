# d3_interface.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (leaked): D:\develop\Units\d3\d3_interface.c
- Boundaries: separated (plausible): leaked; layout 2026-10-22: d3_obj.c joined (.data: d3o_LightSet / d3o_FogSet = -1 at 0x5fbcfc head the block, then the d3_interface literals 0x5fbd08..0x5fbe80, then "Unknown Object-Type in wr_object()!" 0x5fbe8c: one block); ends before d3_int_io.c (codegen, round 9)
- Range then: 0x446e10..0x44aa10
- Was: d3_obj.c (unk_448ac0.c, round 12) joined layout 2026-10-22

## Open work
- **headers (2026-10-30, typing pass 2)**: head.c includes d3.h, d3_vec.h, d3_engine.h, d3_mir.h, d3_d3d.h, d3_fl.h, d3_interface.h, bio.h, d3_light.h, data_views.h, f3init.h; keeps 1 cast / view macros with @original byte evidence (the address and the matched caller are in each comment): dd_ClearB; views / casts without byte evidence of their own (weak definitions or drafts' spellings, notes/headers.md debt item 1): d3_PointToRefD, d3_WorldMatrixD.
- see notes/units/ch.md, Open work: "Round 7 leftovers: ch_InitWalk (15 bytes, stack-slot order), ch_ActWalk / ch_ActWalkCells ..."
- see notes/units/d3_engine.md, Open work: "Header cleanup from round 7: d3_engine.h's d3_Unk4439e0 is d3fl_Draw(D3FLMAP *, char) and ..."
- Round 9 header cleanup: unk_40d680.c's d3_Unk449de0 /
  d3_Unk44a170 are char d3_SetLightSet(unsigned, float, float, char) / char d3_SetFogSet(unsigned, unsigned,
  float); d3.h's void d3_SetParent returns char; unk_448ac0.c declares d3_SetFog(float, float, D3FOGCOL) vs
  d3_engine.c's unsigned long; d3.h's tex_Load takes unsigned
  char last two arguments (D3IO_TEXLOAD cast); d3_anim.h clashes with d3.h on d3_Unk4319e0.
  Leftovers: di_UpdateButtons, di_GetMouse (notes/units/di_input.md), d3_YawTo, d3_CornerCB,
  d3_ScreenToUV (below, d3_obj.md section), d3io_ReadFloor (notes/units/d3_int_io.md).

## Worker d3if, round 7 (scope 0x446990..0x447e60) -- REPORT [d3if]

14 functions in the scope (12 Ghidra starts + 2 missed callbacks, 0x447260 d3if_ShowCB and
0x447800 d3if_PrecacheCB).
Nonmatching (2): 0x446990 d3fl_FarDist (35/1072 bytes: three scheduling spots), 0x4478e0 d3if_SelectScene
(50/1408 bytes, only the store/load schedule of the global-copy block).

TU boundary: d3_fl ends at 0x446dc0 (d3fl_SetShift, on a D3FLMAP). d3_interface.c starts at 0x446e10: its
strings are the two SelectTextureSet messages (0x5fbd08, 0x5fbd4c), "MegaCam" (0x5fbd8c) and
"D:\develop\Units\d3\d3_interface.c" (0x5fbd94, __FILE__ of the EvictManagedTextures check at line 682 = 0x2aa
in d3if_SelectScene); the 1.0f constant at 0x5fbd04 before them is the last d3_fl constant. The first string of
the next TU, "d3d_Devi..." (0x5fbda0?), follows the path, so 0x447e60 (next function, 0x447e60 starts with
`mov ecx,[esp+4]; push ebx`) is probably still d3_interface.c.
The fixed scope also contains the d3_fl tail 0x446990 (map far distance) and 0x446dc0 (SetShift), which are in
src/game/unk_446990.c (own file so the d3fl4 worker's unk_43eab0.c is not touched; the orchestrator can join it
to unk_43eab0.c / its header at merge time; it only uses D3FLMAP from include/unk_43eab0.h).

Files: src/game/unk_446990.c + include/unk_446990.h (d3_fl tail), src/game/d3_interface.c +
include/d3_interface.h. Shared headers: no changes. Names (our own, because the existing declarations have other
types): d3_Unk447190 (d3.h, `void`) = d3if_Delete (returns char), d3_Unk4471b0 (unk_402140.h, `int show`) =
d3if_Show (char), d3_SelectScene (d3.h / unk_4273d0.h, `int unk`) = d3if_SelectScene (int index, char keep),
d3_Unk446990 (d3_engine.h, `void *p`) = d3fl_FarDist (D3FLMAP *), d3_CreateObject446e10 kept (first parameter is
the parent object, passed as int). Cleanup for the orchestrator: those declarations should be renamed/retyped.
Also from this scope: d3_Unk446180 (d3fl_Prerender) takes a D3FLMAP *, d3if_Prerender(char) = 0x447860.

What the functions are:
- d3if_SelectScene / InitScene / Reset: the engine's global state (object list ends, particle list ends, map,
  sky, ambient colour, fog colour/range, "levels") is saved into / loaded from d3_Scenes[index]
  (D3IFSCENE in include/d3_interface.h, all offsets used by these functions); InitScene gives an empty scene a
  "MegaCam" camera object; Reset empties the current scene.
- d3if_Show(obj, show): hides (saves type in +0x21c, type = 1) / shows; '!'-prefixed names lose the '!' on show.
- d3if_SelectTextureSet(obj, surf, set, reload): reloads the textures of one D3SURF from the name table of
  its ops (`ops+0x204`, 0x40 bytes per name, count x nsets); the sprintf messages are formatted into buffers
  that are never used.

### VC6 lessons
- asmdiff shows a bogus `-`/`+` pair for an instruction with two relocations (`mov ds:A, offset B`); `uv run build`
  diff / check says matched. Trust check.
- Index loops over a global array of structs (`for (i = 0; i < 6; i++) arr[i].a = ...`) give the countdown
  `dec ecx; jne` with the pointer based at the second member, and a pointer loop (`for (p = arr; p < arr + 7; p++)`)
  gives `jb` with a different base; the original used the index form (d3if_Reset, d3if_InitScene).
- A float global assigned `0` / `200.0f` is stored with an immediate (`mov [g], 0`); keeping it `float` (not
  unsigned long) fixed the immediate-vs-register zero (d3if_Reset).
- `d3_Scenes[i].field` through `((T *)d3_Scenes)[i].field` folds `base + field` into the displacement
  (`[eax+0xfd7c20]`); a pointer local `s = &scenes[i]` adds the base first. Repeating the whole expression
  `D3IF_SCENE(d3_CurrentScene).x` at every use reproduces the reloads of the current index after each asm block.
- Two-reloc risk: `D3_COPY_TO(offset g, src, n)` (dst named, size/src temps) vs `D3_COPY(dst, offset g, n)` (src
  named, size/dst temps) vs `D3_COPY3` (all three temps) all occur in one function (d3if_SelectScene); the
  temps' stack slots tell which one.
- A block that is out of line in the original is the `else` of the test written the other way round
  (`if (tex) {...} else {fail}` in SelectTextureSet); a repeated `sprintf` message reached from two tests
  (`ntris <= 0` and `replaced == 0`) is ONE sprintf after `for (...) {...} if (!replaced)`: VC6 jump-threads the
  loop guard to it.
- One final `return 1` with an early `return 0` makes VC6 push the callee-saved registers after the early
  return (shrink wrapping), as in d3if_SelectScene; `if (keep) return 1;` in two places gave the pushes first.
- A byte flag tested and set in a loop (`!(t->state & 0x80)` ... `t->state |= 0x80`) puts 0x80 into a register
  (bl) only for `unsigned char`; the existing `signed char state` gave `test dl,dl; js`.
- `d3_Busy < 1` with a zero kept in ebx is `d3_Busy <= 0` (`cmp [mem],ebx; jg`).
- Strcpy-like byte loops `while ((*q++ = *p++));` give the one-pointer + difference form (`[edx+eax]`).
- Bug-prone: a function declared `void` in a shared header but returning `mov al,1` in the original
  (d3_Unk447190, d3_SelectScene): `#define old new_name` around the include (as unk_402140.c does) is not needed
  when the new function simply has another name.

### Nonmatching details

#### d3fl_FarDist (0x446990, 35/1072 bytes)
Four planes through the map corners in camera space (pl[0..3] = D3VECTOR4; normals are the camera matrix
columns times (0,0,-1), (1,0,0), (-1,0,0), (0,0,1), written out as `m11*0 - m31*1 + m21*0` ...), then the
smallest distance along the 5 view rays at 0xfc2960 (d3fl_Rays) that crosses a plane beyond zmin.
What fixed it (frame 0x40 -> exact 0x6c, all but three spots): u and v are `D3VECTOR4` locals (16 bytes: v at
esp+0x14, u at 0x24, pl at 0x34, so the frame is exactly three vectors + 4 planes); `t4 = m21*0` etc. are named
float locals (they land in the dead parameter slots; unnamed `m31*0 + m21*0` is factored to `(m31+m21)*0`);
t4/t5/t6 are assigned interleaved with pl[0].x/y/z; `one = -1.0f` used as `+ m31 * one` (VC6 emits `fmul 1.0;
fsubr`; a `+1.0f` variable is folded away); the corner is computed as `v.x = f*scale + orgx; ...; v.x = f*b0 + v.x;
...; v.x -= pos->x` (three statement groups; the grouped `f*b0 + (f*scale + orgx)` gives another schedule); float
literals in the tests (`t < -1e-07f || t > 1e-07f`, `t > 0.0f`, `t > zmin`), `best == 1e10` as a double compare,
`if (best == 1e10) return fabs(zmin); return best;`; outer loop `for (r = d3fl_Rays, i = 0; i < 5; i++, r++)`
(a pointer + counter gives the countdown `dec edi; jne` and the second saved register edi, an index loop over the
global array gives `cmp edx, end`); pl[3] as `(m11*0 + t4) + m31`.
Left (three spots): (1) `push edi` four instructions earlier in the original, (2) `mov esi,[ecx]; sub esi,4;
mov [esp+0x10],esi` (the int f) two FPU instructions later, (3) the operand order of the dot product in the
loop: the original loads `pl.x` and `pl.z` (ecx) before `r->x`/`r->z` (edx) but `r->y` before `pl.y`; no source
order of the three products (all 16 tried), a pointer `p` into pl, or an `r` initialized at the top changed it.
A one-statement-move search (+-3 positions, 3 rounds) over the pre-loop statements found nothing better.

#### d3if_SelectScene (0x4478e0, 50/1408 bytes)
Everything matches except the order of ~12 loads/stores in the block that copies the new scene's fields into the
globals. The original stores in this order: par1, sky, last, 0x63349c, map, AmbB, 0x5fb5c4, first, AmbG, par2,
0x5fb5c0, AmbR, Dir[0], d3_Camera = 0, Dir[1], Dir[2], Gray (fstp last), with fld of Amb B/G/R right after
their stores; the scheduler of ours moves the Amb stores / the Gray chain around. A statement-move search
(single moves, beam of 5-8, 5 rounds in all, 17 statements) got it from 101 bytes to 50 bytes (current order in
the source); the ambient globals as a struct / volatile / the Gray expression in any operand order or
position did not help. What fixed the rest: `if (index != cur) {...}` with ONE `return 1` at the end (the
pushes of ebx move after the early `return 0`), `if (d3_Unk633df8) Unk45e620 else create`, `d3_CurrentScene =
index` before `d3_DefaultScene = ...`, D3_COPY_TO for both the fog and the levels copy, `d3_Busy <= 0`.

## Worker d3if2, round 8 (scope 0x447e60..0x448910) -- REPORT [d3if2]

9 functions in the scope (8 Ghidra starts + 1 missed callback, 0x448540 d3if_PickCB).
Nonmatching (1): 0x4486e0 d3if_ScreenBox (5 asm lines: one `fmul st,st(1)` + later `fstp st(0)` where the
original has `fmulp st(1),st`).

TU: all of it is d3_interface.c (src/game/d3_interface.c, appended in address order). DrawSprite's two D3DCHECKs
use __FILE__ "D:\develop\Units\d3\d3_interface.c" at lines 973 (SetTexture) and 1008 (DrawPrimitive); their
strings (0x5fbdb8, 0x5fbe34) follow the path string. After them come "Seite", "Vorne", "Oben" and then
"Unknown Object-Type in wr_object()!" (another TU); the first three are probably used by 0x448910
(d3_Unk448910, called by d3if_Reset, outside my scope), so d3_interface.c likely continues there.

Files: src/game/d3_interface.c, include/d3_interface.h (own header, appended: D3IFMOVE, D3IFSPRITE, D3IFTEXFLAGS,
D3IFHIT, D3IFPICK, D3IF_OBJ068, d3if_Unkfe798c, prototypes of d3_GetBounds 0x43d860 and a char version of
d3_Unk4387b0). Shared headers: no changes. d3_interface.c renames d3.h's int-parameter d3_Unk4387b0 while
including it (`#define d3_Unk4387b0 d3_Unk4387b0_int`, as d3_d3d.c does) and now includes <math.h>.

What the functions are:
- d3if_ClearScene(index): the current scene via d3if_Reset; another scene by switching to it with keep=1,
  disposing objects / particle systems / map / sky / unk3c8, and switching back.
- d3if_MoveObject(obj, from, to) + d3if_MoveCB: moves a top-level object tree from one D3IFSCENE to the end of
  another (syncing the default scene's list globals before and after), re-scenes every object in the tree and
  deletes particle systems whose parent is one of them.
- d3if_LoadSprite(name, glow) / d3if_FreeSprite / d3if_DrawSprite(t, x, y, angle, sx, sy, alpha): screen sprites,
  drawn as a rotated D3DTLVERTEX triangle fan ("TurboBuffer") over a full-screen viewport.
- d3if_Pick(x, y, flags, mask) + d3if_PickCB: object under a screen point (projected bounding spheres, nearest
  per object type, lowest type wins).
- d3if_ScreenBox(obj, rect): screen rectangle of an object (type 0: square of size obj+0x68; meshes: projected
  bounding box clipped to d3_Viewport).

### VC6 lessons
- `char ret = 0; if (in range) { ...; ret = 1; } return ret;` gives the `xor al,al` before the first compare
  (d3if_ClearScene); early `return 0` / nested ifs did not.
- `to->last->next = obj; obj->prev = to->last;` reloads `to->last`; the original reads it once into a local
  (`o = to->last; o->next = obj; obj->prev = o;`), which also fixed the register rotation of a later block.
- `sqrt(d) < r` (float r) is `fld r; fcompp; and eax,0x4100`; `(float)sqrt(d) < r` is `fcomp r; test ah,5; jp`.
- A param `sx *= 0.5f` used once is forwarded into its use (no early `fmul`). The original's early
  `fld sx; fmul 0.5` with the value killed by `fmulp` at its use came from writing the product expression
  repeatedly (`corner[0][0] = size * sx; corner[1][0] = -(size * sx); ...`, CSE'd), not from hw/hh locals
  (d3if_DrawSprite). An aggregate initializer `float corner[4][4] = {{size * sx, ...}}` matches too.
- Named float locals kept on the FPU are not popped at their last use (`fmul st,st(1)`, later `fstp st(0)` at
  the block end); CSE temps and params are (`fmulp`).
- `((float *)obj)[0x68 / 4]` loads after `d3_ScaleX` in `d3_ScaleX * x * f`; a cast macro with extra outer
  parentheses `(*(float *)((char *)o + 0x68))` gets loaded first.
- Indexing `b[i].x` in an 8-vertex loop puts the pointer base at the middle field (&b->y, as in the original);
  `b++` puts it at &b->z.

### Nonmatching details

#### d3if_ScreenBox (0x4486e0, 5 asm lines)
Everything matches except the y projection in the mesh loop: the original multiplies by the reciprocal with
`fmulp` (reciprocal dead), ours with `fmul st,st(1)` and pops it after the ftol call. Looks like the original's
reciprocal is a CSE temp, but `(1.0f / b[i].z)` written twice is recomputed in ours (the r->left/right stores
kill it). Tried (no change): f as a block-local / separate / double / register variable, x/s/y reused for it,
loop shapes (index, b++, do/while, while, unsigned i), min/max vs ifs, operand orders, a comma-expression body,
inline helpers for the projection or the whole body, assigning the y result back to f, dropping the type-0
branch or the clip block (still `fmul`). Worse: `z = b[i].z` local with `1.0f / z` twice (CSE survives but the
product order flips), projections inside the min/max macros.

## d3_obj.md (joined into d3_interface.c, layout 2026-10-22)
## Worker d3if3, round 9 (scope 0x448910..0x449960) -- REPORT [d3if3]

11 functions in the scope (9 Ghidra starts + 2 missed callbacks, 0x448c20 and
0x4495f0, and 0x449710 which Ghidra did start but nobody had named).

Nonmatching (3): 0x448ac0 d3_YawTo (53/352 bytes), 0x448c20 d3_CornerCB (167/560), 0x448e60 d3_ScreenToUV
(first version, frame 0xc4 vs 0x108, 1472/1744).

TU boundary: 0x448910 and 0x448a50 are still d3_interface.c ("MegaCam"/"Oben"/"Vorne"/"Seite" strings, 0x448a50
is the viewport clear + d3_Frame). Nothing from 0x448ac0 on uses a string of d3_interface.c and none of them
references the wr_object strings; I put 0x448ac0..0x449960 in src/game/unk_448ac0.c (own TU guess: the d3 file
that follows d3_interface.c, probably the scene IO TU with "wr_object"/"rd_object"; the orchestrator can join it
with d3o's unk_449960.c). The scope end 0x449960 is not a visible TU boundary.

Files: src/game/d3_interface.c (appended 2 functions), include/d3_interface.h (appended d3_Frame prototype
(0x43e680, defined in d3_engine.c, undeclared in headers) and d3if_ClearView), src/game/unk_448ac0.c +
include/unk_448ac0.h (new: D3FINDCTX, D3CORNERCTX/PT/VERT/TRI, prototypes of 0x454040, 0x454070, 0x454690,
d3_ZAxis re-declared). No shared header (d3.h, d3_engine.h, ...) was changed. d3_anim.h cannot be included
together with d3.h/d3_engine.h (d3_Unk4319e0 clash), so d3_Unk454690 is declared in my header.

What the functions are:
- d3_Unk448910: creates the cameras of a scene: "MegaCam" (0, 0, -256); when d3_Unk633438 is set also the
  orthographic "Oben" (0, 256, 0, rot -pi/2), "Vorne" (0, 0, -256) and "Seite" (256, 0, 0, rot +pi/2) objects
  (their unk211 bit 1 is cleared), stored in d3if_Unk6338b8/bc/c0.
- d3if_ClearView: when d3_Unk633451 and d3_Busy <= 0: hardware mode clears d3_Viewport via d3_Unk435fb0,
  otherwise dd_ClearC with the fog colour unless dd_Cleared; then d3_Frame(0x40, 1, 1).
- d3_FindObject449690(obj, flags, c, name) / 449770 (case-insensitive): look an object up by name (a leading '!'
  is ignored) and/or by address `c` in the default scene (flags | 0x200); the siblings after `obj` are cut off
  during the walk (obj->next saved and zeroed). The callbacks share a context {name, obj, found}.
- d3_Unk4497f0(obj, pos, rot, outpos, outrot) / d3_Unk449920: converts a position / rotation matrix given in
  world space into the coordinate system of obj's parent (inverse of the parent matrix) and (449920) applies
  it with d3_SetObjectPos/Rot.
- d3_YawTo(obj, p): signed horizontal angle between obj's facing and the direction to p (acos of the dot product
  of the two normalized xz directions, sign from a perpendicular obtained with sin/cos(pi/2) at run time).
- d3_CornerCB + d3_ScreenToUV(sx, sy, obj, tex, &u, &v): the texture coordinates under a screen point. The
  callback collects, over all polygons textured with `tex`, the vertex nearest to each screen corner (min x+y,
  min y-x, min x-y, max x+y) with its uv and screen position (D3CORNERCTX, 0x88 bytes). The caller
  normalizes the four uv, picks three corners by uv and intersects the view ray (centre/scale globals) with the
  plane of the triangle, barycentric inside test, second triangle (4th corner) when r + s > 1.

### VC6 lessons
- A callback whose branches share the `p == s` tail: the original tests `if (name) { ...two arms... } else if
  (p == s) found = s;` (name arm first); the `!name` arm first or a goto chain gave different block layouts
  (d3_FindCB449690/449710, both matched with the same shape).
- Two functions that differ only in `strcmp` (inlined to the 2-byte unrolled compare loop) vs `_strcmpi` (call)
  are the same source with the compare swapped.
- A local that is the 3rd argument and is read when `obj` is NULL (`mov edi,[esp+0x20]`) is just `D3OBJECT *next`
  that is not initialized on that path.
- `d = min(1.0f, max(-1.0f, d))` compiled to the original's triple test; the original's `mov edx,[d]; mov [d],edx`
  came from a second float local sharing d's slot (`e = min(max(-1.0f, d), 1.0f)`).
- A signed byte flag tested for sign (`test al,al; jns`) came from `t->flag & 0x80` on a signed char field, not
  `flag < 0` (which gives cmp/jge).
- `if (A) { ... } else if (B) {...}` with `(float)expr > 0.0f` compares against a DWORD constant only when the whole
  sum is float.

### Nonmatching details (tries used: YawTo ~20, CornerCB ~10, ScreenToUV 3)
- d3_YawTo (0x448ac0): everything up to the final compare matches (clamp, acos). Left: the rotated-vector term.
  The original loads `fld st(1); fmul [a.z]` (sin*az first, sin/cos kept in FPU regs), stores the float t
  (`fstp [esp+0x20]`), computes cos*az - sin*ax, keeps it with `fmul st,st(1)` and pops at the end; ours loads the
  memory operand first (`fld [a.x]; fmul st,st(1)`) and evaluates cos before sin. Tried: sin/cos as named
  double/float locals in both orders, t and u as float/double locals, (float) casts, term orders, inline sums.
- d3_CornerCB (0x448c20): 167/560 bytes. Matches except: x/y stack slots swapped ([esp+0x20]=x in the original,
  ours y), the sum compares load old values as `mov edx,[y]; mov ecx,[x]; add edx,ecx` (ours `add ecx,[mem]`) and
  the 4th corner block reloads globals/locals from memory (ours CSEs). Tried: flipping the compares, declaring
  y before x, computing y first, a local copy of c->tex, `fl = flag24` local.
- d3_ScreenToUV (0x448e60): first version straight from the Ghidra pseudocode (loop and branch structure
  right, constants right). The original has a 0x44 bytes larger frame (every float temp in its own stack slot, the
  three best values at [esp+0x10..0x18], min[2] at 0x4c, ia/ib/ic as ints in memory at 0x54..0x5c); it keeps mn[1]
  on the FPU across the first loop and copies mn[0] with fld/fstp (min-style conditional expression); the plane
  dot has the `(nz + ny + nx) * 0.0` factored term. Not worked on beyond one min() experiment.

## Worker d3o (round 9): 0x449960..0x44abb0 [d3o]

### Report
- 10 functions in 0x449960..0x44abb0.
  Ghidra knew 8; it missed 0x449d80 (d3_ScaleLightCB) and 0x449da0 (d3_SetLightKeyCB), the two
  d3_ForEachObject callbacks of d3_SetLightSet. 0x44a748..0x44a76f is d3_WriteObject's switch table.
- Files: src/game/unk_449960.c, include/unk_449960.h (orchestrator: join with d3if3's file, which
  holds 0x448910..0x449960; my file calls its d3_Unk4497f0 / d3_Unk449920, prototyped locally).

| addr | name | notes |
|------|------|-------|
| 0x449960 | d3_SetParent | d3.h declares it `void`; it returns char (renamed via #define around the d3.h include) |
| 0x449b20 | d3_SetLightKey(obj, key, t) | blends a light's 7 day-time keys (D3LIGHTINFO entries) |
| 0x449d80 | d3_ScaleLightCB | missed by Ghidra |
| 0x449da0 | d3_SetLightKeyCB | missed by Ghidra |
| 0x449de0 | d3_SetLightSet(set, t, scale, calc) | = unk_40d680.c's d3_Unk449de0 (SetLightSet) |
| 0x44a170 | d3_SetFogSet(a, b, t) | = unk_40d680.c's d3_Unk44a170 (SetFogSet) |
| 0x44a2c0 | d3_WriteObject(f, obj) | "wr_object", recursive |
| 0x44a770 | d3_WriteMap(f, D3FLMAP *) | terrain map |
| 0x44a9e0 | d3_SceneVersion(version) | version -> 0/4/6/7 |
| 0x44aa10 | d3_SaveScene(name, f) | scene file version 0x3a6c00bb, "wb" |

- TU: d3_SaveScene ends at 0x44abad, right before d3o2's 0x44abb0. No __FILE__ string in the range; the
  only literals are "wb" (0x5f8324, shared) and "Unknown Object-Type in wr_object()!" (0x5fbe8c).
- Shared headers: none changed. My header defines views of existing structures under new names:
  D3LIGHTKEY / D3LIGHTKEYS (= d3_engine.h's D3LIGHTENTRY / D3LIGHTINFO with fields named; phase 0x1a4 is a
  float), D3FOGCOL / D3FOGSET / D3LIGHTSET (= d3_interface.h's D3IFTRIPLE / D3IFLEVEL: colour b,g,r,a +
  start, end floats), globals d3o_Fog (= d3if_Fog 0xfba960), d3o_Sets (= d3if_Levels 0xfba9c0), d3o_Amb
  (0x5fcc78), d3o_Dir (0x633b1c), d3o_LightSet 0x5fbcfc, d3o_FogSet 0x5fbd00, d3o_FogScale 0x5fbd04,
  d3o_SkyCol 0xfb91c0 (sky colour per light set, written to sky+0xaa4..0xaa6).
  The .c file declares d3_SetFog(float, float, D3FOGCOL) (d3_engine.c defines it with unsigned long) and
  renames unk_43eab0.h's vfs_Root / vfs_Unk42fbf0 declarations (they clash with unk_430130.h).
- Prototype clashes for the join: unk_40d680.c declares d3_Unk449de0(int, float, float, int) and
  d3_Unk44a170(int, int, float); real: char d3_SetLightSet(unsigned, float, float, char), char
  d3_SetFogSet(unsigned, unsigned, float).

### VC6 lessons
- A 4-byte colour passed as `*(unsigned long *)&col` is pushed after the FPU work for the other arguments;
  the original pushes it first: the parameter is a by-value 4-byte struct (`d3_SetFog(float, float, D3FOGCOL)`).
- Lerps where all three differences are computed first and the third spilled to the frame (frame 0x10
  bigger): a D3VECTOR4 difference local `d.x = b.x - a.x; ...; out.x = d.x * t + a.x;`.
- `l->k[key].pos[i]` written out in a loop (not through `c = &l->k[key]` pointer locals that the code above
  uses) gave the original loop: one byte-offset IV for the local arrays, pointers based at the middle field.
  Pointer locals made VC6 use pointer differences and a countdown.
- `(unsigned)f` then `f - key`: `float t = *time;` keeps f on the FPU (`fld st(0)` before _ftol).
- Product order inside `(int)(a * u + b * t)` (byte fields) only reacted to tie-breaks: a dead `u = 0;` at
  the top, fog-set pointer locals `a`/`b`, or parenthesizing one product. Source operand order: no effect.
- `isdigit(p[1])` on plain char: movsx; `unsigned char > 0`: `test; jbe`, `!= 0`: `test; je`.
- Switch block order follows case order in the source (1/4 falling into 2/3, then 0, then 5-8).

### Outside the scope
- Nothing else noticed.

## match-base 2026-10-24 (base-0 worker, session 2)

- d3_CreateObject446e10 -> d3_CreateObject (the engine's object factory; Q1 fixed). D3SURF.unk14 ->
  texHandles (the array of the surf's texture handles, ops->count of them, read as D3TEXHANDLE ** here and
  D3FLTEX ** in d3if_SelectTextureSet) and d3_Unk44cbb0 -> d3_AttachMesh (attaches the named stock's LOD
  surfaces to the object; d3_AttachMesh is byte + readable now, d3_CornerCB became readable as a side effect).
- d3_CreateObject -> match-deep: the only failing rule is Q2 on d3_Unk44e080, the int alias of d3io_LoadObject
  (include/d3_io_t.h). Including d3_io_t.h in d3_interface.c fails: d3_fl.h's view macros already bind
  vfs_Root/vfs_Tfopen to other types (include/vfs.h:94/133 redefinition). The same clash hits d3_engine.c /
  d3_int_io.c (d3_io_t.h vs their D3IOKEY and vfs views), so the alias stays in d3_engine.h. Header cleanup
  of d3_fl.h / d3_io_t.h / vfs.h (or per-unit view macros) needed first.

## match-base 2026-10-24 (base-0 worker, session 3)

- d3if_SelectTextureSet -> byte + readable. New field names (all from the same-object views in d3_io_t.h /
  d3_io_fl.c / d3_mir.c): D3MESH.mirrorPoints / mirrorPolys / mirrorSource (0x960/0x964/0x968, the
  "d3_mir:MirroredPoints" / "d3_mir:MirroredPolys" allocation tags and the surf they were made from),
  D3SURF.texSet (0x195, the current texture set; D3IOSURF.f195), D3OPS.nsets (0x1e4 = D3BGF.nsets, the .TXS
  set count) and D3OPS.texnames (0x204, D3BGF.texnames, nsets x count names of 0x40, set-major),
  D3SRCPOLY.slot (0x28 = D3IOPOLY.slot, the surf texture slot). D3SURF's old `int unk194` became two bytes
  (`unk194` stays: nonzero forces transform/draw, d3_engine.c draw + d3s.c 0x194).
- d3if_Show / d3if_ShowCB / d3if_ShowTree -> readable. D3OBJECT.type0 (0x21c, "the object's real type",
  the same field D3OBJX.type0 names) replaces the D3IF_OBJ21C raw macro; d3_Anim's raw cast on it too.
  d3_Unk4538a0 -> d3_LightInvalidate (0x4538a0, d3_light.c); the three declarations (d3_interface.c, ecmd.h,
  d3_engine.c) were unified on the definition's unsigned char, so the three lint baseline pairs are gone.
- d3if_Reload -> readable; d3_CreateObject -> byte + readable (@deep cleared). tex_ReloadAll (0x461090,
  ts_texture.c: void tex_ReloadAll(char, void (*)(int, int, int))) replaces the d3_Unk461090 alias in
  d3_engine.h (the alias keeps int,int because its callers pass ints; the real prototype is in ts_texture.c
  only, ts_texture.h has no prototypes). d3io_LoadObject (0x44e080) is now declared locally in this file as
  `struct D3IOOBJ *d3io_LoadObject(char *, char *);` - the tag form is the same type as d3_io_t.h's typedef,
  so lint sees no conflict, and d3_io_t.h still cannot be included here (d3_fl.h vfs views).
- d3if_Reset (0x4473f0): still below readable, but 11 of its 16 identifiers are named now. Done:
  d3_Unkfc52f0 -> d3_TopFirst (0xfc52f0, the default scene's first top-level object), d3_Unkfc28f4 ->
  d3_TopLast (0xfc28f4, the last), d3_Unkfc5100 -> d3_TopHead (the head sentinel D3OBJECT, d3_InitObject'd
  and pointed to by d3_TopLast), d3_Unk6338c4 -> d3if_MegaCam, d3if_Unk6338b8/bc/c0 ->
  d3if_CamOben/CamVorne/CamSeite (the "Oben"/"Vorne"/"Seite" cameras of d3if_CreateCameras, was
  d3_Unk448910), d3_Unk633dd4 -> d3_Sky (0x633dd4 = D3SKY *d3_Sky in d3_sky.c; the d3_engine.h alias got the
  struct D3SKY * type), d3_Unk63349c -> d3_SceneFlags (the flags d3_Unk43db30 stores, printed as "df", ch.c
  decides from them what to rebuild) and D3IFSCENE.unk3c8 -> events (0x3c8, the D3EVENT table that
  d3_RegisterSceneEvent allocates, "d3e:RegisterSceneEvent(eventtable)"; the D3_SCENE3C8 macro still names it
  in d3_engine.c).
- Open for d3if_Reset (read before naming): d3_Unk43ff90 (0x43ff90 IS d3fl_FreeMap in d3_fl.c, but its
  definition takes D3FLMAP * while d3_engine.h's alias declares void *, so a plain rename is a conflicting
  types error; the fix is a d3_fl.h prototype + an include of d3_fl.h in d3_engine.c, or a local declaration);
  d3if_Unk5fb5c0 / d3if_Unk5fb5c4 (0x5fb5c0 / 0x5fb5c4, no reader besides the D3IFSCENE.unk3cc/unk3d0
  save/restore in d3if_SelectScene, values 4 and 0xf, no evidence yet); D3SCENE.unk3d4 (1 after d3_LoadScene
  loads a scene file) and unk3d5 (ch.c refuses to walk in a scene with it, d3t_FindFreeScene skips scenes
  with either) - the setters of 3d5 are not in the decompiled code, so no evidenced name.
- Remaining base work in this unit after the session: `uv run ledger grade next`.

## match-base 2026-10-24 (base-0 worker, shard 0/5, session 2)

- Named project-wide: D3SCENE.unk3d4 -> loaded (set to 1 by d3_LoadScene after the file is read, 0x44be60;
  d3t_FindFreeScene skips a slot with it) and unk3d5 -> dirty (f3init.c: a new universe is only reserved and
  "marked dirty (0xfd7f75)" with = 1 at 0x50d7f0's callers (line 2533/2554) and reloaded and cleared by
  0x50d7f0 / 0x50dc30; ch.c refuses walks and moves into a dirty scene, ch_GetHMap skips the scene rebuild).
  The D3IFSCENE mirrors 0x3d4 / 0x3d5 got the same names.
- D3IFSCENE.unk3d8 -> sceneFlags (= d3_SceneFlags 0x63349c, InitScene/SelectScene copy it), unk3dc -> hardware
  (= d3_Hardware 0x633450), unkb0 -> hmap (D3SCENE.hmap 0xb0; ch.c / f3init.c pass it to d3sm_DestroyRaw and
  ch_GetSceneHMap), unkb4 -> mapSize (0x80 with terrain / 0x40 without, read from the scene file; the size of
  d3sm_Create and the coordinate bound in ch_InsertAction).
- d3_Unk43ff90 -> d3fl_FreeMap (0x43ff90 is defined in d3_fl.c; the d3_engine.h alias was `void(void *)`).
  The prototype is declared locally in d3_interface.c: the same line in include/d3_fl.h recompiles d3_tools.c
  and flips d3t_UpdateSunRays (fild + fsubp vs fisub); d3_engine.c has its own local prototype too.
- d3if_ClearScene: byte + readable.
- Escalated to match-deep: d3if_Reset, d3if_InitScene (Q2 0x5fb5c0 / 0x5fb5c4 = D3IFSCENE.unk3cc / unk3d0:
  write-only - only the store pair in d3if_Reset and the save/restore pair in d3if_SelectScene reference them
  in the whole .text, there is no pointer to them in .data, the .data initials are 4 and 0xf, no writer of
  D3SCENE.dirty outside these either, so no evidenced meaning), d3if_SelectScene (the same pair; the other 12
  identifiers d3_Unk45ce00/45e620, d3_Unk633df8, d3_Unkfb9900, d3if_Unkfb9904/08, d3if_Unk6339db/dc,
  d3if_Unk633dcc, d3if_Unkfba944, d3if_Unkfc29bc/40 still need reading; behaviour is reached, the 22 code hunks
  are moved instructions).
- Raised byte + readable: d3if_MoveObject (d3_Unk43dbc0 -> d3_InflateObject: builds the drawable points and
  triangles from the loaded mesh, the allocation tags read "d3:InflateObject(allpolys/allpoints/mat)"; the
  K&R `int ai_Unk43dbc0();` in cm.h was an unused foreign declaration and is gone), d3if_LoadSprite (Q2
  d3if_Unkfe798c -> d3d_UsedCard.info.caps: 0xfe798c is 0xfe7680 + 0x30c and the original reads it with
  `test BYTE PTR ds:0xfe798c,1`; the extern alias was dropped. Q4: D3IFTEXFLAGS is now a union whose `raw`
  member is the D3TEXFLAGS that tex_Load takes by value), d3if_DrawSprite (Q3 0xffffff -> #define D3IF_RGB_MASK),
  d3if_ScreenBox (behaviour + readable; the only Q2 was D3MESH.unk048).
- D3MESH.unk008 -> worldMatrix (0x439870 writes it with d3_WorldMatrix) and unk048 -> cameraMatrix (named
  cameraSpaceMatrix on the base-0 branch and cameraMatrix on the base-3 branch; the merge kept cameraMatrix -
  the object matrix relative to the camera, built by d3_MatMul43(m, camera->matrix, out); GetBox's third
  argument is cast to D3MATRIX * in d3_GetBoxVerts). Both are the 0x40-byte D3MATRIX; the old comments said
  "bounding box, object/camera space" and were wrong. Types stay char[0x40] (only names changed, no codegen
  risk). Reference sites: d3_engine.c, d3_mir.c, d3_interface.c. Side effect: d3_GetBounds became readable.

## match-base session 2026-10-24 (base-3 worker, shard 3/5)
Reference updates only (renames in a shared header): d3_Unk453ee0 -> d3_MatMul43 (now from d3_vec.h), D3MESH
unk048 -> cameraMatrix (same field as the base-0 note above; merge kept this name). No code change in this
unit; the full ledger check stayed at 2501 matched functions.

## match-base session 2026-10-24 (base-0 worker, shard 0/5, session 3)
Unit base queue empty. **Raised to byte + readable:** d3if_ClearView (Q2 cleared by d3_Unk435fb0 ->
d3d_Clear, the 0x435fb0 function in d3_d3d.c, and d3_Unk633451 -> d3_EngineReady, set at the end of d3_Init
and cleared in d3_Shutdown (the base-3 name; the base-0 spelling d3_Initialized was dropped in the merge)); d3_WorldToParent (0x4497f0, was d3_Unk4497f0: converts a world-space
position/rotation into the parent's space) and d3_SetWorldPosRot (0x449920, was d3_Unk449920: sets the local
pos/rot from a world pos/rot), which also raised d3_SetParent; d3_SetLightKey and d3_ScaleLightCB (the
D3IFLIGHT view over D3OBJECT 0x5c / 0x90: color / range / power / atten, the names of d3_light.h's D3LOBJ;
D3LIGHTKEY.unk28/unk2c -> range/atten); d3_WriteMap (D3IFBITS / D3IFREGION / D3IFMAP views replace the five
float bit-pattern casts; d3fl_Unk631090 -> d3fl_EmptyName, D3FLREGION.unk0c/unk10 -> scrollSpeedU/V,
D3FLMAP.unkc4 -> hscale); d3_SaveScene (D3IFSCENE mapSize / events instead of the raw 0xb4 read and the
D3_SCENE3C8 macro, D3IFBITS for the float globals and the fog colour).
**Escalated:** d3if_CreateCameras (Q2 d3_Unk633438: all four image references read, only store is 0 in d3_Init,
no pointer in the image, .data initial 0; GildeGold_TL.exe has the same flag set 0 only; the three roles - ortho
view cameras, 'r' lights as type 5, on==0 invalidation - give no evidenced name), d3_CornerCB (three hunks that
are the same operation in another register/order spelling; swapping the pt[0] sum terms and the local
declaration order produced identical code), d3_ScreenToUV (after the float-vs-double fix below the remainder is
the frame 0xc4 vs 0x108 and FPU scheduling), d3_WriteObject (Q2 unk09c / unk214 / unk19c / unk1a0 / unk30 /
unk34 have no evidenced use beyond the scene file; 9 Q4 serialization casts).
**Finding:** d3_ScreenToUV's constants are mistyped, not wrong: the original uses DWORD constants
(0x5eb780 = 1.0f, 0x5eb788 = 0.0f, 0x5eb948 = 0.33333334f) where the source had double literals, and keeps a
double 1.0 only for `r + s <= 1.0` and the outv `1.0 - (...)` (0x5eb8a8 QWORD); 15 QWORD constant uses became
float, unitdiff 1472 -> 1451 bytes.

## match-base 2026-10-25 (base-0 worker, shard 0/6)

- The `#define d3_Unk447190 d3_Unk447190_void` workaround around d3.h is gone: d3.h now declares the
  canonical `char d3if_Delete(D3OBJECT *obj);`; byte unchanged.

## match-base 2026-10-26 (base-3 worker, shard 3/6)

d3_interface.c includes d3_light.h; its local d3_LightInvalidate declaration and d3e_SaveEvents prototype are gone (d3e_SaveEvents now in d3_engine.h, d3_LightInvalidate in d3_light.h). No byte change.
