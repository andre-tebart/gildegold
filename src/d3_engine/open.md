# d3_engine.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (leaked): D:\develop\Units\d3\d3_engine.c
- Boundaries: separated (proven): leaked / tag neighbours
- Range then: 0x438c30..0x43ea10

## Open work
- 2026-10-26 (base-3): the 78 foreign declarations (`uv run ledger grade` d3_engine.c) are all cross-unit. Two concrete
  blockers found this session: (1) three of d3_Render's callees could be renamed to their evidenced names
  (d3_Unk4439e0 = d3fl_Draw, d3_Unk446990 = d3fl_FarDist, d3_Unk45d500 = d3sky_Draw) but the rename needs one
  prototype per function, and d3_engine.h's void* views differ from d3_fl.h / d3_sky.h - unifying them is the
  header cleanup below, which may flip d3_Render (byte); (2) the old *alias* names still stand at the call sites
  in other shards' units: `ai_Unk43db30` / `ai_Unk43deb0` (= d3_DeflateScene / d3_InflateScene, 0x43db30 /
  0x43deb0) are declared as `int f();` and called in src/game/main.c:3923/3925 and unk_58bbd0.c:7313..8191, so
  those TUs must include d3_engine.h before their foreign declarations of d3_engine functions can go (same for
  the ai_Unk43b790 / ai_Unk43b860 / ai_Unk438d40 / ai_Unk438e60 / ai_Unk43b950 / ai_Unk43d430 spellings in
  f3init.c, he.c, sv.c, cm.h, gv.h, unk_575860.c, unk_58bbd0.c) - the rename is only safe together with the
  K&R -> include replacement, as notes/match.md records for d3q_Create.
- Header cleanup from round 7: d3_engine.h's d3_Unk4439e0 is d3fl_Draw(D3FLMAP *, char) and d3_Unk446990
  is d3fl_FarDist(D3FLMAP *); d3.h's d3_Unk447190 (void) is char d3if_Delete; unk_402140.h's d3_Unk4471b0
  is d3if_Show(char); d3.h / unk_4273d0.h's d3_SelectScene(int) is d3if_SelectScene(int index, char keep).
- Header cleanup from round 8: d3_engine.h's d3_Unk4538a0(int) takes a char (ocmd's d3_LightCalc4538a0);
  d3_engine.h's `unsigned char d3_Unk63345d` is the dword d3_NoTextures (ocmd2); ocmd2's header declares
  d3_tools.c functions (d3t_*) and par_CreateEmitter 0x457b30, which have no shared header yet.
- see notes/units/d3_vec.md, Open work: "Names: 0x453e80 is d3_VecEqual (d3.h) and d3_PtEq (d3_io_t.h); 0x4538a0 is d3_LightCalc453 ..."
- see notes/units/d3_anim.md, Open work: "Type cleanup: D3MATERIAL+0xf4 is the D3ANIMCTL (slots at +0x110, so `tex[3]` at 0x178 is r ..."
- see notes/units/dd_vesa.md, Open work: "Header cleanup: Dd_vesa.c globals/functions are declared both in d3_d3d.h (with D3DCARD) a ..."

## Worker d3_left (round 3, scope 0x432750, 0x433f80, 0x4391d0, 0x439c90, 0x43a7d0, 0x43b080, 0x43bc20) [d3_left]

### Report
- 0x43b080 d3_Unk43b080 is the software draw of the sort buffer.
- Nonmatching (4): 0x439c90 d3_Unk439c90 polygon clipper (12 asm lines), 0x4391d0 d3_Unk4391d0 frustum/clip-plane
  setup (frame and layout right, ~200 asm lines), 0x43a7d0 d3_Unk43a7d0 D3D draw (first draft, ~500 asm lines,
  structure complete), 0x433f80 d3anim_CreateMorphAnim (~210 asm lines, mostly the min/max loop and frame layout).
- Without source (1): 0x432750 (5 KB, d3_anim.c, 1578 asm lines, no Ghidra function).
  It is the per-object animation update (calls 0x4324e0, 0x4315c0, 0x432190, 0x431750, 0x4539b0, 0x458810,
  d3_SetObjectPos/Rot, 0x435740, 0x431ae0, 0x453e20, 0x453e80, d3path_Tangents).
- TU boundaries: none new. 0x4391d0 sits between d3_ProjectMesh and d3_Unk439650; d3_Init (0x43bc20) after
  d3_Unk43bb70.
- Shared headers: none changed. Own headers appended: include/d3_engine.h (D3CLIPINFO, D3CVERT, clip lists,
  D3UV, D3TRIFUNC/d3_TriFuncs, D3INITCFG/d3_Cfg, D3TEXV, D3TLV, D3E_TLV/D3E_TLV2/D3E_COPY_AT macros, prototypes of
  dd_*, d3d_* and other callees); include/d3_anim.h (D3ANIM_COPY3, D3ANIM_BONENAME, D3ANIMMESHX, prototype).
  d3_engine.c also got a local `__inline d3_FtoI2(float f, int *dst)` (parameter order matters, see below).
- d3_CloneObject (0x43d180) flips with every function definition added above it (even an unused __inline
  function changes it): matched with N, 4 bytes off with N+1, 2 bytes off with N+2 definitions. Judge it
  once the file is complete (the orchestrator may need the final function count).
- Outside scope: d3_Planes is really [6] (0xfc5068..0xfc50c8, planes 4/5 are znear/zfar); d3_ClipInfo entries are
  D3CLIPINFO (n, flag byte, 6 planes); 0xfe75e0 is a table of 6 software triangle fillers (all 0x44ee40 at init).

### VC6 lessons
- An inline asm helper's parameter order decides its spill slots: d3_FtoI(int *dst, float f) vs (float f,
  int *dst) moved 70 asm lines in the clipper.
- A D3VECTOR4 local reused for two unrelated groups of differences (xyzw, then rgba) explained both the frame size
  and why the first two components stay on the FPU while the rest are spilled (439c90).
- A 12-byte D3VECTOR (not D3VECTOR4) argument for the rotation fixed the frame of 4391d0.
- Pointer locals (`p = t->v[2]; uv[4] = p->tu; ...`) and 8-byte struct copies (D3UV) reproduce
  "load the pointer once, store twice" where field-by-field code through t->v[i] reloads after each store.
- `!(f & 0x80)` on a signed char compiles to `test; js`, `f >= 0` to `jl`.
- Number of function definitions in the TU (inline ones too) changes register choices in later functions.

### Attempts
#### d3_Unk439c90 (12 asm lines)
Left: (1) the second dot product's term order (orig px*bx, bz*pz, by*py; no permutation or operand order or
parenthesization of terms/reads reproduced it; 1 paren variant gets z first); (2) the `lea eax,&c; mov [slot],eax`
for the first FtoI is scheduled before the fog diff fild in the original. Tried df order/casts, pointer local.
#### d3_Unk4391d0
Original keeps atan2 results a and b on the FPU stack (fld st(0); fst a; fstp global) through sin/cos and the
negations; ours always reloads from memory. Tried chained/parenthesized assignments, s/c locals, double CSE of
sin/cos, statement orders (15 spellings).
#### d3anim_CreateMorphAnim
Body right; the min/max loop forms differ per component (orig: min x/y with fld/fstp copy and `and 0x4100`,
min z integer copy with `test ah,5; jnp`, max x/y fld/fstp with `test ah,5`, max z kept on FPU). Tried if/min/max/
ternary combinations (~60 variants); frame 0x34 vs 0x40 also open.
#### d3_Unk43a7d0
First draft only: register allocation differs from the start (orig keeps 0 in edi and the texture in edi/[ebp-0x18]).
d3_Unk4385a0's first argument is a byte in the original (d3.h declares int).

## Worker d3_engine, round 2 (scope 0x439000..0x43e000) -- REPORT [d3_engine]

All of it is in src/game/d3_engine.c (one TU, address order). 0x439650 d3_Unk439650 is the box frustum clip,
0x43a000 d3_Unk43a000 the LOD surface; d3_CloneObject 0x43d180 flips (see below).

Nonmatching (attempts below): 0x43a160 d3_DrawObjectCB (first draft, structure right, ~not iterated),
0x43a6f0 d3_SetFog, 0x43ce60 d3_InitObject (10 B), 0x43d430 d3_SetObjectAlpha (4 B), 0x43d860 d3_GetBounds,
0x43da00 d3_Unk43da00, 0x43dbc0 d3_Unk43dbc0 (InflateObject).

Still without source in scope: 0x4391d0 (frustum planes from d3_SetViewport), 0x439c90 (polygon clipper,
returns vertex list), 0x43a7d0 (D3D draw of the sort buffer, 2.2 KB), 0x43b080 (software draw of the
sort buffer), 0x43bc20.

TU boundaries: none found; d3_engine.c continues through the whole scope (0x43df60 is a d3_engine.c
callback, 0x43e090 follows). Missed function starts in scope: 0x43da00, 0x43df60.

Shared headers: no changes to d3.h or other shared headers. Only include/d3_engine.h (own TU header) was
extended/retyped (D3MESH layout, D3OBJX full object view, D3OPS, D3CLIP, D3SORTENT, macros D3E_COPY3,
D3E_FILL, D3E_RADIX, inline d3_FtoI). d3_engine.c hides d3.h's int prototype of d3_Unk439870 (third
parameter is a char) with the same #define trick as before. Sort globals use unk_45db40.c's names
(d3_SortBuf/d3_SortPtr/d3_SortCount).

VC6 lessons:
- An uninitialized local returned after `|= 0x40` lands in a dead parameter slot (d3_Unk4399e0 returns
  `mov al,[esp+param]`).
- `box = 0; if (a && b) box = f(); if (!box) {...}` vs `if (a && b && (box = f()))` changes block order.
- A float kept with `fst` for a later use is a local (`d3_HalfW = hw = w * 0.5f; CenterX = hw + x`).
- Struct copies of 12-byte vectors give lea-pointer copies; the original had per-field scalar copies.
- A loop bound re-read in the test (`i < ((unsigned char *)t)[0x208]`) fixed the whole register layout of
  a strcmp search loop.
- Indexed `v[i].field` instead of `v++` moved the strength-reduced IV to the most used field (+0x4c).
- `x |= 0x80` on a signed char vs `*(unsigned char *)&x |= 0x80` differ (immediate vs register constant).
- `min()/max()` macros on globals give the `fld g; fcomp x; test ah,5 / and 0x4100` forms with a
  conditional store only.
- Copy/fill macros with all operands through temps (size, src, dst order); scene index as
  `(unsigned)((char *)p - (char *)base) / sizeof` gives the mul-by-magic unsigned division.
- Register reload order at a join (d3_CloneObject) changed when another function with 12 inline-asm
  labels was added above it, and changed back later: some TU-global state is involved, so near-matches of
  that kind can only be judged with the complete file.

### Details

Missed function starts found in the scope: 0x43da00 (inside Ghidra's 0x43d860), 0x43df60 (inside 0x43deb0).

### Nonmatching

#### d3_SetFog (0x43a6f0)
Original keeps `on` in bl (`mov bl,1` before the call to d3_SetFogRange) and duplicates the four tail
stores into both branches (first branch: end, on, start, color; second: on, start, end, color).
`on = 1; d3_SetFogRange(); ... else on = 0;` gets bl but not the duplicated tail (24 asm lines).
Tried: `on = start < end` + `if (on)`, `if (on = ...)`, ternary, int/unsigned on, store orders, explicit
duplicated tails with return (all 23 store orders: VC6 either merges them or constant-propagates on).

#### d3_Unk43da00 (0x43da00)
Only the D3TEXFLAGS argument: original `mov [f],0 (dword); mov cl,[f+2]; and cl,0xfe; mov [f],0xff;
mov [f+2],cl` - it re-reads the byte after the dword clear. Ours folds the known zero (`mov [f+2],bl`).
Tried: unsigned char / unsigned int bitfields through casts, block-scope `= {0}` initializers (3- and
4-byte structs), `f.b[2] &= 0xfe`. (d3_FreeSurf with `f.b[2] = 0` matches.)

#### d3_InitObject (0x43ce60), 10 bytes
Left: (1) the flag-byte RMWs: original loads 0x211, 0x210, 0x212, 0x213, keeps 0x212 in bl and stores it
before 0x210; ours (statement order 211, ..., 210, 212, 213) stores 210 first and does not use ebx;
(2) the light-entry loop addresses as `[ebx+eax-4]` where the original has `[eax+ebx-4]`.
Tried: all 6 orders of the 210/212/213 statements, 211 moved, unsigned char bitfields through a cast
(VC6 merges the bit assignments into the same RMW as the mask form), `i[e]`, `(e + i)->`, byte-offset loop.

#### d3_GetBounds (0x43d860), 19 asm lines
Body matches (needed dx/dy/dz locals, `r = max(r, d)`, `ok = 1` before the out-parameter stores). Left:
the original keeps the return flag in bl from the entry on (`xor bl,bl` before the obj test, failures
`mov al,bl`) and so reloads the radius pointer; VC6 constant-propagates our `ok` (xor al,al) and keeps the
radius pointer in ebx. Tried: char/int ok, late `ok = 0`, `return 0` at the failures, goto to a shared
`return ok`, `return 1` vs `ok = 1`.

#### d3_SetObjectAlpha (0x43d430), 4 bytes
Only the address operand order of the two `tris[i].tex` accesses per loop: original `[ecx+edx+0x14]`
(IV offset as base), ours `[edx+ecx+0x14]`. Same symptom as d3_InitObject's light loop. Tried: `tris[i]`
via a local or directly, `(tris + i)->`, `i[tris]`, byte offsets `(char *)tris + i * 0x28`, integer
arithmetic `i * 0x28 + (int)tris`, unsigned i, a pointer IV.

#### d3_CloneObject (0x43d180), 4 bytes
Two reloads swapped where the `if (src->material)` block joins: original `mov edx,[&obj->unk210]` before
`mov eax,[obj]`. Needed scalar per-field copies (struct copies give lea pointer forms). Tried: 210/211
statement order, D3OBJBITS bitfields, `nsurf` test spellings, `return obj` inside the if.

#### d3_Unk43dbc0 (0x43dbc0, InflateObject), ~110 asm lines
Surface/alloc part matches (needed `alloc((s->info.ntris = n) * 0x28)`, the `mat < 0` else out of line,
separate per-byte color stores). Left: the triangle loop. The original re-zeroes ecx at the loop top
(entered with a `jmp` past the `xor ecx,ecx`), tests `mat` with `test edx,edx`, and has a free ebx for
byte temps; ours keeps the constant 0 in ebx for the whole loop. Tried: separate variables for the
material index, a temp idx, chained zero stores, `t->flags = 0` moved, indexed `tris[k]`, while loop.
  Update: d3_CloneObject matched after d3_Unk43dbc0 was added and flipped back to these 4-8 bytes when
  d3_RadixSort (4 inline-asm radix passes, 12 asm labels) was added above it. A d3_RadixSort with one
  pass or no asm keeps it matched; blank lines and symbol names have no effect. The reload order at a
  join depends on some TU-global state (asm label/temp count?), so it can only be judged with the
  complete file.

#### d3_DrawObjectCB (0x43a160), first draft
One compile only. Known differences: the out-of-view branch should come last (write
the visible path first / invert the `clip & 0x40` test), MinZ/MaxZ need the original compare forms
(`<=` with and 0x4100; the second `test ah,5; jnp`), the bit-3 copy of the dirty bit must not be masked,
and the obj->unk210 |= 4 path uses `and 0xfb; xor 4` (bitfield = 1).

## Also about this unit
- notes/units/d3_fl.md, section [d3_engine2]

## match-base 2026-10-23
- Reference updates only (a d3_vec.c Readability rename): d3_DrawObjectCB and d3_Render use
  d3_LightKeysCulled / d3_LightKeysVisible / d3_LightGeneration / d3_LightKeyBudgetCulled /
  d3_LightKeyBudgetVisible now (evidence in notes/units/d3_vec.md).

## match-base 2026-10-24 (match worker, reference updates from d3_light.c)

- D3MESH.unk96d -> flags (0x40 = no vertex animation; d3_engine.c 0x439xxx / d3_interface.c / d3_mir.c /
  d3_light.c references updated, byte unchanged) and D3MESH.unk958 -> lightScale (ch.c sets 1.8).

## match-base 2026-10-24 (match worker, reference updates from d3_anim.c)

- d3_Unk4319e0 -> d3anim_MapBones and d3_Unk433b10 -> d3anim_FreePath (one call each); byte unchanged
  (62/75 matched, 3787 B).

## match-base 2026-10-24 (base-3 worker, shard 3/5)

- d3_ForEachObject byte + readable: d3_Unkfc52f0 -> d3_ObjFirst, d3_Unkfc28f4 -> d3_ObjLast (the current
  scene's top-level object list head and last: d3_interface.c copies them to/from D3IF_SCENE.first/.last and
  back, d3_LinkTop appends at the last, the reset sets them to &d3_ObjEnd / &d3_Unkfc5100); D3SCENELISTS
  (the unit's view) .unk084 -> last, .unk0a4/.unk0a8 -> parHead/parTail (they take d3_ParHead/d3_ParTail and
  are walked to &d3_ParEnd). d3_interface.c/d3_interface.h references updated, byte unchanged.
- d3_ProjectMesh (2026-10-25, base-3): the backface test `(v0->sx - v1->sx) * (v0->sy - v2->sy) < ..` has
  its two factors in the other evaluation order in ours (the original subtracts the sx pair first); swapping
  the multiply operands in both products is canonicalised by VC6 (identical bytes, reverted), so the order is
  not a source spelling. The second hunk (438fe5) is an FPU stack-depth difference of 1 (fld/fmul operands at
  st(0..3) vs st(1..2)). Both need the deep toolbox.
- d3_ProjectMesh behaviour + readable: for types 5..7 the flat color is D3OBJECT+0x5c. It is reached through
  the unit-local D3OBJCOLOR view (padding + D3VECTOR color) in d3_engine.c, not a field in d3.h: a field or
  anonymous union in D3OBJECT flips d3_Unk4550e0 by 8 bytes, and so does a plain typedef added to
  d3_engine.h (which recompiles d3_mir.c), see notes/vc6-matching.md. d3_light.h's D3LOBJ names the same
  field color; 0x5c is a position for type 3 / dummy objects (d3t_PlaceAtObject, ecmd 'SetCameraToDummy',
  d3_int_io.c 0x439xxx camera load), which is why D3OBJX.unk05c stays a generic view name for now.
- d3_Unk4391d0 (frustum/clip-plane setup) is work in progress, still draft + draft: the clip-code loop is now
  `for (i = 0; i < 64; i++) { info = (D3CLIPINFO *)d3_ClipInfo[i]; ... }` instead of i++/info++ (asm diff
  253 -> 111 lines; the original keeps the byte offset edi = i*0x68 and compares it with 0x1a00). The Ghidra
  pseudocode of 0x4391d0 gives the rest: s = sin(a); c = cos(a); then plane[0].x = c; plane[1].z = s;
  plane[0].z = s; plane[1].x = -c; and for b: plane[2].y = c; plane[3].z = s; plane[2].z = s;
  plane[3].y = -c; the frustum corner expressions are `(m._11 + m._21) * 0.0f + m._31`-shaped (Ghidra:
  `(local_30 + local_40) * 0.0 + local_20`), not two 0.0f * terms. The c/s variable variant in the x,z,x,z
  store order was tried and is worse (141 asm lines); the store order above was not tried. Readability still
  needs: Q1 a name (the function rebuilds d3_Planes/d3_Frustum/d3_ClipInfo on a viewport/FOV change, e.g.
  d3_SetupFrustum), Q2 D3CLIPINFO.unk04 (set to 0 here; read as `+ clip->unk04` / `!clip->unk04` in the draw
  paths of d3_engine.c, five references).

## match-base session 2026-10-24 (base-3 worker, shard 3/5)

Raised to byte + readable (8): d3_ClipBox (D3CLIP as a union with `raw`, so the byte-wise clear / | / & are
fields), d3_UpdateWorldMatrix (D3OBJECT.posOffset typed field, D3MESH.worldMatrix / cameraMatrix, the call is
d3_MatMul43 from d3_vec.h), d3_ClipObject (was d3_Unk4399e0; D3MESH.zMin / zMax), d3_ClipMesh (D3PVERT.flags is
a union with a D3CLIP `clip` view and `rawFlags`; zeroing through `c->raw` after `c = &v[i].clip` keeps the
original's read-modify-write pattern, `v[i].rawFlags = 0` does not), d3_ClipPoly (was d3_Unk439c90),
d3_DrawSorted (was d3_Unk43b600), d3_UpdateObject (Q6).
Escalated (7): d3_Unk4391d0 (FPU stack keeping of the atan2 results, recorded attempts), d3_DrawObjectCB
(round-3 first draft: register allocation and block placement differ and ours clears obj->state bit 2 where the
original keeps it), d3_SetFogRange (Q2 d3_Unkfc2df0 / d3_Unkfc2df4: only written from d3_SetViewport's last two
int parameters, never varied, no role), d3_SetFog (same Q2; plus the original duplicates the four tail stores
while ours shares them - moving the stores into both branches was worse, 74 -> 89 bytes), d3_DrawSortedD3D
(frame 0x78 vs the original 0x80, two dwords of locals missing, register allocation differs from the first
hunk), d3_DrawSortedSW (Q2 D3CLIPINFO.unk04), d3_UpdateAll (Q2 d3_Unk633448 / d3_Unkfb9220 / d3_Unkfb925c).

Leads: d3_Unk633448 is the "shadow_fake" object (`d3_SpawnObject(4, "shadow_fake")`, objectFlags 0x20); d3s.c
already calls it d3s_Object. A rename in place (declaration stays in d3_engine.h) may work - moving it to
d3s.h and adding `#include "d3s.h"` to d3_engine.c flipped d3_CloneObject and d3_Unk4550e0 and was reverted.
D3CLIPINFO.unk04: 0 for all 64 table entries (0x4391d0), 1 for the mirror clip info with extra planes
(d3_mir.c), used as `+ clip->unk04 == 0` for the fast path and `- clip->unk04` in the winding correction -
flag or plane count is unproven.

## match-base 2026-10-24 (base-3 worker, shard 3/5, second pass)

Raised to byte + readable (11 + 2 side effects): d3_SetObjectPos, d3_SetObjectPosXYZ, d3_SetObject078,
d3_SetObjectRot, d3_SetObjectRotXYZ, d3_SetHardwareMode (d3_Unk43bb70), d3_ApplyGraphicsOptions (d3_Unk43c4e0),
d3_FreeObjectData (d3_Unk43c670), d3_DisposeObject, d3_UnlinkTop, d3_AddChild; d3_ReloadMeshCB and
d3e_RegisterEvent became readable through the d3_FreeObjectData / D3OBJX.events renames. d3_AppendSibling is
readable now (Q4 gone) but behaviour draft, escalated.

Escalated (deep): d3_SetViewport (Q2: d3_Unkfc2df0/fc2df4 are write-only, stored from the last two parameters
and passed back by d3_SetFogRange/d3_SetFog; d3_Unkfe73a4 0xfe73a4 = (int)(65536.0f*(h+y)) and d3_Unkfc2ddc
(set 1.0f) have no reader in the exe - the Ghidra exports 43a610/43a6f0/43b950/43bb70/43bc20/43c240 are the
only references); d3_Init (Q2: d3_Unk44ee40 is the no-evidence no-op triangle filler of d3_io_a.c, the list
heads/sentinels 0xfc2ffc/0xfc4a40/0xfc26e0/0xfe7440 are written here only and the d3_io_t.h aliases are other
struct views, 0x633448 = d3s_Object needs d3s.h and its move flips d3_CloneObject, dd_RBits/GBits/BBits need the
char vs unsigned char view solved; Q4 one raw deref); d3_Shutdown (same d3s_Object / list-walk blockers, Q4
D3_SCENE3C8 = *(void **)(scene + 0x3c8)); d3_SpawnObject (Q2 d3_Unk633438 0x633438: 4 raw uses only - gates the
Oben/Vorne/Seite orthographic cameras, makes the 'r' spawns type 5 and skips the on=0 relight; never written 1
in the exe, no evidenced role); d3_AppendSibling (B: 48 code hunks, no meaning difference - original keeps
first in ecx as the loop cursor and obj in esi with one shared tail store, ours duplicates the tail store for
the empty-list path; a local `last` cursor compiles identically).

Header / view facts: D3MIRROR::obj / info / planes (0xfb9900/0xfb9904/0xfb9908) are now reached as d3_Mirror
fields (d3_mir.h) in d3_engine.c and d3_interface.c. d3_mir.h's d3_Unk4550e0 / d3_Unk4554b0 (D3MIRROR *)
prototypes conflict with d3_engine.h's D3OBJECT * views, so the include goes through #define ..._mirview
around it (d3_mir.c's pattern) and d3_Unk4554b0 is called as (D3OBJECT **)&d3_Mirror - bytes unchanged.
D3LIGHTINFO 0x194 is D3LDATA.time (d3_light.h) -> field `time`. D3OBJX.unk1d0 -> path, .unk1d4 -> events
(D3E_NEVENTS D3EVENTs). D3MESH.unk000 -> freeCallback (called with the object before the mesh is freed; only
cleared in the drafted code). D3_OBJBITS is now (*d3_ObjBits(o)) with an inline accessor, Q4-clean and byte.
Removed externs: d3_Unkfb9900/04/08 (d3_engine.h), d3if_Unkfb9904/08 (d3_interface.h).

VC6: removing the d3_Unk4550e0 / d3_Unk4554b0 prototypes from d3_engine.h flipped d3_Unk4550e0 (8 B) and
d3_LoadScene44be60 (3 B) although neither uses them - see notes/vc6-matching.md.

## match-base 2026-10-24 (base-3 worker, shard 3/6)

Raised to byte + readable (9): d3_SetCamera (Q6), d3_AllocObjectPolys (Q6), d3_FreeSurf (D3SURF.unk18 ->
owner, the D3OBJECT whose mesh owns the surf = D3ANIMCTL.obj at the same offset; the D3TEXFLAGS dword clear
is a local union; D3_OPS()->count / ->Release replace the raw accesses), d3_FreeMesh (Q6), d3_InitSurf
(D3SURFTEX is the D3ANIMSLOT view of the three slots d3anim_StartAnim fills through D3ANIMCTL.slots[3] at
mesh+0x110: frame / nextFrame / subFrame / data / flags / playFlags (0x74 bit 1 = play); D3SURF.tex -> slots;
the int-pointer walk of the loop is kept because a typed slot pointer changes 11 of 80 B), d3_AddObjectDrawData
(D3MESHSUB fields named after D3SSLOT (d3s.h): tex / light / dir; D3MESH.unk0b0 -> frameStamp (== d3_FrameCount
= already transformed, -1 = never)), d3_SetAlpha (Q6), d3_AttachToBone (was d3_Unk43d6c0; stores the bone name in
mesh+0xb4 (inside pad0b4, the field d3anim_MapBones matches children by - not typed out, a padding field can flip
d3_Unk4550e0) and re-poses the parent; ch.c/ch.h references updated), d3_TransformMeshes (was d3_Unk43d840; the
0x40 type mask = type 4, every mesh of the default scene; d3_sm.c declaration + call updated).

Escalated (4, reasons in the @deep tags): d3_InitObject (the state/stateFlags RMW order and the light-loop
address encoding, same instructions), d3_SetObjectAlpha (its 3 raw D3_OPS count accesses are fixed and it is
readable now; only the base/index encoding of the two tris[i].tex accesses per loop is left), d3_CloneObject
(byte; Q2: D3OBJX.unk09c has no evidenced meaning), d3_GetBounds (return flag in bl vs folded al).

Leads from this session: D3OBJX.unk058 = D3LOBJ.flickerEnd, unk05c = D3LOBJ.color / D3IFLIGHT.color, unk078 =
D3OBJECT.posOffset, unk090 = D3LOBJ range/power/atten, unk06c = the pivot (XOBJ.p6c: subtracted before and added
after the parent matrix by d3_PointToWorld). D3MESH+0xb4 = the bone name (see d3_AttachToBone). The unit still
has 29 foreign declarations (`uv run ledger grade d3_engine.c`) for the header canonicalization.

VC6: an anonymous union inside D3SURFTEX (d3_engine.h) flipped d3_Unk4550e0 (d3_mir.c) by 8 B at +0x1be;
plain field renames (D3SURF.unk18 -> owner, D3MESH.unk0b0 -> frameStamp) and the split slot fields are
byte-neutral. See notes/vc6-matching.md.

## match-base 2026-10-24 (base-3 worker, shard 3/6, second pass)

Raised to byte + readable (6): d3_DeflateObject (was d3_Unk43da00; `memset(&f, 0, sizeof f); f.b[2] &= 0xfe;`
reproduces the original's re-read of f.b[2] after the dword clear - the earlier cast / union / bitfield variants
let VC6 fold it, the inlined memset is expanded too late to forward), d3_DeflateScene (was d3_Unk43db30; the "du"
trace, d3_SceneFlags = flags, frees the terrain tiles and deflates every object), d3_InflateScene (was
d3_Unk43deb0; the "iu" trace, the inverse: d3fl_Inflate + d3_InflateObject, d3_SceneFlags = ~flags & 3),
d3_ToggleRLightCB (was d3_Unk43e110; switches the "r" lights type 5 <-> 6), d3_SetRLights (was d3_Unk43e180;
applies it to the scene and relights), d3_EndFrame (Q2 only: d3s_Object / d3s_NumVerts / d3s_NumUVs are the
data.csv aliases of 0x633448 / 0xfb9220 / 0xfb925c). Side effect: d3_UpdateAll became byte + readable through the
same renames, its stale @deep tag was cleared.

Escalated (2, reasons in the @deep tags): d3_InflateObject (first 118 instructions equal, then whole-function
register allocation - the zero register is ebx in ours, ecx/edx/edi in the original, xor/cmp vs test, add vs lea,
one dead `jmp .+2; xor ecx,ecx` at 0x43dd49; no missing statement / constant / callee / field; readability Q2 on
D3PTRI.unk18/unk1c/unk20 and D3PVERT.unk4d plus Q4 `*(unsigned char *)&t->v[m]->flags`), d3_Render (Q2 only: 12
write-only engine stats - 0x633470/474/478/47c/480/484/488/494/498, 0x63344c, 0xfb9928, 0xfc2dc0 - stored from
the named counters and never read, no data.csv alias; d3_Unk6334a5's alias ai_Unk6334a5 is still a placeholder;
the three evidenced callees d3fl_Draw / d3fl_FarDist / d3sky_Draw still have void* views in d3_engine.h).

Header / view facts: d3_engine.h keeps `void d3fl_FreeTiles(struct D3FLMAP *map);` / `void d3fl_Inflate(...)`
(0x441db0 / 0x441ea0, the canonical d3_fl.c functions d3_DeflateScene / d3_InflateScene call) - removing them
from the header flips d3_Unk4550e0 (d3_mir.c, byte) to behaviour, so they stay. They need the new
`struct D3FLMAP;` forward declaration at the top of d3_engine.h: in clang's MS-compat mode a tag first used in a
prototype is a different type from the later `D3FLMAP *` typedef (lint conflicting types). D3OBJX.unk040 ->
offTime (d3_Time when d3_ToggleRLightCB switched the light off; cleared in d3_InitObject, no reader in the
sources so far). cm.h's K&R `int d3_Unk43deb0();` became `char d3_InflateScene(unsigned int flags);` (the X pair
of ch.h/cm.h went away, both lint_baseline lines removed) and its `int d3sm_Build();` a
`void d3sm_Build(void *h, void *t);` prototype (same line in d3_engine.h; d3_sm.h still has no function
declarations).

Started, not finished: d3_Animate (B: 12 code hunks in one block at 0x43e5c9 / 0x43e5da: the same operations,
the original loads d3_Unk6334a8 (0x6334a8) into esi and reuses esi for d3_Unkfe7a88 (= tb_Delay), ours uses edi,
and inc/sub are ordered differently). Moving `fr = d3_Unkfe7a88;` after `dt = now - d3_Unk6334a8;` was tried:
worse (49 equal instructions, push esi moved), reverted - it stays in the queue. Q2 there: d3_Unk432750 (the
undrafted d3_anim.c per-object update), d3_Unk460cb0, and the fps window globals 0x6334a8 / 0x6334ac / 0x6334b0 /
0x6334b4 / 0x63348c / 0x633490 (used only in d3_Animate; d3_Unkfe7a88 was renamed to tb_Delay after tb.c / evtc.c).

## match-base 2026-10-24 (base-0 worker, shard 0/5, reference update)

D3SCENE.unk080 -> first (0x80 is D3SCENELISTS.first, the top-level object list; the d3_Shutdown truth test is the
only use); d3_Unk442280 -> d3fl_ReloadTextures (d3.h prototype); d3_Unkfe79c4 removed (0xfe79c4 is
d3d_UsedTexFormat.fourcc). Byte unchanged, 63/75.

## match-base 2026-10-24 (base-3 worker, shard 3/5)

Raised to byte + readable (3): d3e_EventId, d3e_EventName (Q6 descriptions of the event table
lookups), d3e_RegisterSceneEvent (Q4: D3SCENE.events at 0x3c8 as a typed field in an anonymous
union replacing the pad; D3_SCENE3C8 now maps to it; byte count unchanged). d3_Animate: readable
(d3_Unk432750 -> d3anim_StepObject, d3_Unk460cb0 -> ts_UpdateScroll, the two fps windows
0x6334a8/ac/48c -> d3_Fps60Time/Frames/d3_Fps60 and 0x6334b0/b4/490 -> d3_Fps10Time/Frames/d3_Fps10),
then escalated for its one code hunk. Four statement orders of the fps block tried
(fr,n,dt / n,fr,dt / fr,dt,n / dt,n,fr) plus a no-fr spelling; the original simply reuses the a8
temp register for fr, ours loads tb_Delay early and uses edi - register allocation only, byte work.
Unit's base queue is now empty.

## match-base 2026-10-26 (base-3 worker, shard 3/6, session 5)

The unit's base queue is empty; this session did readability renames (no byte changed, check 2603/4664 before
and after, unitdiff d3_engine.c/d3_mir.c/d3_int_io.c no WORSE).
- D3CLIPINFO.unk04 -> mirror (D3MIRPLANES.unk04 -> mirror, same layout): 0 for all 64 clip infos (d3_Init),
  set to 1 only by d3_MirCreatePlanes (d3_mir.c 0x454be0) for the mirror-outline planes, read only as the
  "always clip" flag + the winding-test term in both draw paths. That cleared d3_DrawSortedSW (byte + readable,
  @deep tag removed) and took the field out of d3_DrawSortedD3D's Q2 list.
- d3_Unkfc2ffc -> d3_StockListHead, d3_Unkfc4a40 -> d3_StockListEnd, d3_Unkfc26e0 -> d3_AnimListHead,
  d3_Unkfe7440 -> d3_AnimListEnd (role from the d3_engine.h comments: the mesh driver / stock list walked with
  ->Release and the animation list walked with d3anim_Release; the d3_io_t.h views at the sentinel addresses
  are d3_StockHead / d3_AnimHead, D3IOOBJ / D3IOANIMDATA, and are kept as their own views). Cleared
  d3_Shutdown (byte + readable, tag removed); d3_Init still lists the other write-only list pointers/flags and
  d3io_NoopCallback.
- Reference updates: src/game/d3_int_io.c (d3io_FindStock), src/game/d3_mir.c, include/d3_engine.h,
  include/d3_mir.h. (A test include of d3_light.h in this TU still flips d3_CloneObject - the d3_light.c Q7
  sweep stays blocked, see notes/units/d3_light.md.)  LATER SAME SESSION: not blocked after all - replacing
  the local `d3_LightInvalidate` declaration with `#include "d3_light.h"` keeps d3_CloneObject byte; the flip
  came from the duplicate declaration, not from the header content. The unit now includes d3_light.h and uses
  d3_LightInvalidate / d3_LightUpdate / d3_SetVertexColors / d3_LightRemoveRecords from it; d3_engine.h lost
  d3_LightRemoveRecords, d3_SetVertexColors and d3_LightUpdate (d3_light.c Q7, 17 -> 2 foreign declarations).
- d3e_SaveEvents' prototype moved from d3_interface.c (where it was the only declaration) to this unit's
  header, after D3EVENT; d3_interface.c already includes d3_engine.h, so 1 of the unit's foreign declarations
  is gone (77 -> 76) with no byte change. The other foreign copies (d3.h, cm.h, ch.h, d3_fl.h, ...) need their
  callers to include d3_engine.h and stay open - d3_SetFog cannot move: d3_interface.c calls it with a
  D3FOGCOL struct while the definition takes unsigned long (a caller-side view, not a canonical prototype).
