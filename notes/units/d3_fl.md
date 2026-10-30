# d3_fl.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "d3_fl:TextureCache" allocation tags
- Boundaries: separated (proven): d3_engine.c / d3_interface.c leaked
- Range then: 0x43eab0..0x446dc0
- Was: unk_43eab0.c (round 12)

## Open work
- **headers (2026-10-30, typing pass 2)**: head.c includes d3.h, d3_engine.h, d3_fl.h, d3_light.h, d3_d3d.h, data_views.h; no @original cast or view; views / casts without byte evidence of their own (weak definitions or drafts' spellings, notes/headers.md debt item 1): tex_GetFrameF, tex_FindF, d3_DirToWorldD.
- d3d_TexRMask/GMask/BMask (0xfe79fc..) are members of d3d_UsedTexFormat reached through second symbols
  (one-name-one-address rule; d3_fl owner). (lfix's two uv run link bugs were fixed after the round-13 close,
  with the user's go: `--nonmatching` has 0 conflicts too.)
- see notes/units/d3_anim.md, Open work: "Nonmatching functions (177 after round 17): `uv run ledger check --verbose | grep FAIL`; per-functi ..."
- see notes/units/ch.md, Open work: "Round 7 leftovers: ch_InitWalk (15 bytes, stack-slot order), ch_ActWalk / ch_ActWalkCells ..."
- see notes/units/d3_engine.md, Open work: "Header cleanup from round 7: d3_engine.h's d3_Unk4439e0 is d3fl_Draw(D3FLMAP *, char) and ..."

## Worker d3_engine2, round 3 (scope 0x43e090..0x43fed0) -- REPORT [d3_engine2]

Files:
- d3_engine.c: 0x43e090 d3_ReloadMeshes, 0x43e110 d3_Unk43e110 (callback Ghidra missed), 0x43e180 d3_Unk43e180,
  0x43e1b0 d3_EndFrame, 0x43e200 d3_Render, 0x43e680 d3_Frame, 0x43e6b0 d3e_DefaultResolve, 0x43e6c0 d3e_EventId,
  0x43e710 d3e_EventName, 0x43e740 d3e_RegisterEvent, 0x43e820 d3e_RegisterSceneEvent, 0x43e900 d3e_SaveEvents,
  0x43e970 d3e_LoadEvents, 0x43ea10 d3e_LoadSceneEvents
- unk_43eab0.c (own TU, "d3_fl" terrain texture cache): 0x43ed70 d3fl_BlendMMX, 0x43eef0 d3fl_Render,
  0x43f080 d3fl_MakeWeights, 0x43f180 d3fl_InitCache, 0x43f270 d3fl_FreeCache, 0x43f910 d3fl_Unk43f910,
  0x43f930 d3fl_SetMips, 0x43f990 d3fl_Flush, 0x43faa0 d3fl_Alloc, 0x43fb00 d3fl_Find, 0x43fc10 d3fl_GetTexture,
  0x43fe00 d3fl_Init

Nonmatching (4): 0x43e4d0 d3_Animate (7 bytes), 0x43f8a0 d3fl_SetDetail (register swap), 0x43eab0 d3fl_BlendC
(first branch matches, blend loop register allocation), 0x43f300 d3fl_MakeJitter (first draft). Details below.

TU boundary: d3_engine.c ends at 0x43eab0. Its .data ends with the d3e event table (0x5fb800), the d3e_Resolve
pointer (0x5fb920 = 0x43e6b0) and the "d3e:Register(Scene)Event" tags; 0x5fb970 holds d3fl_Blend = 0x43eab0,
followed by "d3_fl:TextureCache" and "*fltx4/2/1": a new TU starts at 0x43eab0 (no path string, tag prefix
"d3_fl:" suggests d3_fl.c). unk_43eab0.c continues past the scope end.
Missed function start in scope: 0x43e110 (d3_ForEachObject callback used by 0x43e180).

Shared headers: no changes to d3.h or other shared headers. include/d3_engine.h: appended a "round 3, worker
d3_engine2" block (globals of d3_Render/d3_Animate, the d3e event types D3EVENT/D3EVNAME/D3EVHANDLER, file_*
helpers 0x4011a0..0x401240, dd_ClearC = dd_Clear 0x41e100 declared with byte arguments, d3d_ConfigBits =
d3d_Config as a byte). New include/unk_43eab0.h. Note: d3_engine.h already declares d3_Unk43f930(int) and
d3_Unk43f8a0(int); the real functions are d3fl_SetMips(char) and d3fl_SetDetail(unsigned int).

VC6 lessons:
- `dd_Clear(&vp, GetBValue(c), GetGValue(c), GetRValue(c))` (windows.h macros) gives the original
  `mov eax,c; mov dl,[c+2]; mov cl,al; mov al,ah`; `(unsigned char)(c >> 8)` gives `shr eax,8`.
- A local array reused for unrelated values: d3fl_Render keeps the three weight tables and later the four source
  pointers in one `unsigned char *s[4]`; d3fl_MakeWeights' 0x30 frame with gaps was `int w[4], d[4]` (constant
  indices enregistered, the arrays keep their slots).
- A row pointer kept in memory and an index loop `[edi+eax-1]` (no pointer IV) came from a 2D array
  `tiles[j][i]` (`unsigned char tiles[5][5]`), not from a `t += 5` row pointer.
- Clamp `cmp 16; jb out-of-line; cmp 64; jbe; mov 64` = `min(64, max(16, n))`; other spellings reorder blocks.
- Locals assigned before an inline-asm block: `mask = mapsize - 1; n = size + 1;` computed at function entry.
  Their position relative to a store (`c->size = size; mask = mapsize - 1;`) moves the `lea` around the store.
- An initialized char array in an inner block (`if (c = alloc()) { char names[5][7] = {...}; ...}`) is filled
  after the call, as noted before; "" vs "*" showed up as a byte vs word copy of the literal.
- Struct member store order: try all permutations (5 stores, 120 variants with a generator script) instead of
  trusting Ghidra's order.
- `emms` in a function without an ebp frame: `_m_empty()` declared by hand (`void _m_empty(void);
  #pragma intrinsic(_m_empty)`); the toolchain has no mmintrin.h but C2 knows the intrinsic. `__asm emms` would
  force an ebp frame.
- An all-asm function with `align 16` inside the __asm block matched directly (VC6 pads with `lea ebx,[ebx+0]`
  / `lea esp,[esp+0]`), MMX mnemonics included.
- `(wv >> 8) & 0xff` of a value in memory is loaded as `mov r,[wv+1]; and r,0xff`; in a register VC6 uses
  `mov bl,ah` -- seen in d3fl_BlendC, where the original spills wv.

### Nonmatching details

#### d3_Animate (0x43e4d0), 7 bytes
The fps counter needs a local for d3_Unkfe7a88 (`fr`) assigned *before* `n = ++count; dt = now - last;`
(otherwise fr does not get esi). Left: the original loads fr after `sub ecx,esi` (and reuses esi from the
`last` temp); ours loads it before. Tried: all 6 orders of the fr/n/dt statements (fr after dt changes the whole
allocation), `dt > 0` vs `dt` (the original tests `test; jbe`, i.e. `> 0` on unsigned), fr int/unsigned/ulong,
`fr * dt`, dt inline in the condition, `dt = now; dt -= last`, direct global increments instead of n.
`flags = mask | 0x181` as a named local fixed the main register layout.

#### d3fl_SetDetail (0x43f8a0)
Original keeps n in eax and the min(n, 64) loop variable in edx, computes `n >> 7` in place and stores 6 - s via
dl; ours swaps eax/edx. Tried: ternary/if/min() for the 64 clamp, byte/int shift counter (int is right: `inc
ecx`), for/while loop, copying n first, `n >>= 7`, `n / 128`, store order and min() spellings of the
`d3fl_Unk633751 = min(2, n >> 7)` part, early return guard, reusing m for `1 << s`.

#### d3fl_BlendC (0x43eab0)
First branch (all four sources equal) matches except the frame size (orig 0x30, ours 0x24). The blend loop has
the right operations but a different register allocation: the original keeps s0..s3, the dest pointer, both
counters, the weight pointer and the weight dword in memory (12 slots) and therefore extracts bytes with
`[wv+1]; and 0xff` and `sar; and 0xff`; ours keeps more in registers. Tried: int weights, inline weight
expressions, mask spellings, byte-offset table reads, unsigned char casts, signed wv. 6 tries; worth a fresh look
(maybe the source used an array `unsigned char *s[4]` like d3fl_Render, or a different loop shape).

#### d3fl_MakeJitter (0x43f300), first draft
Writes 256 texture triangles (D3FLUV t[256][3] at d3fl_Jitter 0xfc0d60): four fixed ones in the square f..1-f
(f = 1 / d3fl_Unk633748), then 63 x 4 copies with random offset, scale `rand / RAND_MAX / 3 + 0.9` and rotation
`rand * 2pi / RAND_MAX` (d3_Unk4545c0 builds the matrix from {0, 0, angle}). The vertices are (x, y, 0) * M * s +
o with x, y in {0, 1}; the original keeps `0 * m._31` etc. as separate (CSE'd) products. Open: the original's
0x84 frame; `g = 1 - f - f` is stored into four different slots (rot.x, rot.y, the oy slot and one more) and
each `g + f` is computed from its own copy, so the fixed triangles were probably built through the same
struct/vector variables as the random ones (maybe with s = g, o = f); the output pointer is biased by 0x80.

## Worker d3fl, round 4 (scope 0x43fed0..0x441f50) -- REPORT [d3fl]

In unk_43eab0.c:
- 0x43fed0 d3fl_Exit, 0x43fee0 d3fl_TileLod, 0x43ff90 d3fl_FreeMap, 0x440140 d3fl_Invalidate (was d3_Unk440140),
  0x440170 d3fl_Fill (flood fill), 0x440280 d3fl_WaterPoint, 0x4402f0 d3fl_Ramp, 0x441270 d3fl_SameType,
  0x441660 d3fl_LightCB (d3_ForEachObject callback, no Ghidra function), 0x441c50 d3fl_InitTile,
  0x441db0 d3fl_FreeTiles, 0x441ea0 d3fl_Inflate

Nonmatching (3): 0x440360 d3fl_PrepareWater (3.8 KB, full draft, same calls in the same order, mostly slot and
register allocation), 0x441300 d3fl_MakeDivide, 0x441860 d3fl_MakeLight (same size). Details below.

TU boundary: the whole scope is the d3_fl TU (unk_43eab0.c, probably d3_fl.c). Its strings run from 0x5fb970
("d3_fl:TextureCache") over "PrepareFloorWater: ...", "d3_fl:Water(...)", "d3_fl:Tile*", "d3fl:Inflate(...)",
"d3_fl:Light/Divide*", floor texture / height map / water map loading messages ("boden/", "_w.bmp") up to the
SelectTextureSet messages at 0x5fbd07..0x5fbd8c; "D:\develop\Units\d3\d3_interface.c" follows at 0x5fbd94. So
the TU continues past 0x441f50 (map loading, SelectTextureSet) toward d3_interface.c; map_Pick 0x4463b0 and
map_Region 0x446910 (named in d3_snd.c) are d3_fl functions too.

Types (include/unk_43eab0.h, own header): D3FLMAP (the terrain map = d3_Unk633738 = D3SCENE.unk0ac: size, n,
mask, height/types/water/light maps, divide[3], src[3], tiles[8][8] at 0xe0, regions at 0x19e0, typenames[8][0x40]
at 0x1a64, watertype 0x1c6c, nregions 0x1c6d, flag bits at 0x1c70/0x1c71), D3FLTILE (0x64), D3FLPOINT (0x50),
D3FLPOLY (0x28), D3FLWREG (0x14, water rows), D3FLREGION (0x158, water regions; d3_snd.c's D3MAPREGION.water at
0x154), D3FLLIGHTCTX (in the .c). D3FLTEX got the flags field at 0x6c. Own copies of helpers: d3fl_FtoI /
d3fl_FtoI2 (= d3_FtoI of d3_engine.h), D3FL_FILL (= D3E_FILL with size declared first), D3FL_COPY_TO
(= D3_COPY_TO of d3_d3d.h). map_Pick declared here with a float * out (d3_snd.c declares char *; it is the
ground height).
Shared headers: no changes. Note for header cleanup: d3_engine.h's d3_Unk43ff90 / d3_Unk43fed0 / d3_Unk441db0 /
d3_Unk441ea0 are d3fl_FreeMap / d3fl_Exit / d3fl_FreeTiles / d3fl_Inflate (all void *map).

VC6 lessons:
- `max(0, y - n)` gives `sub; sets dl; dec edx; and` (no test); `max(y - n, 0)` gives `test; setle`. For
  `min(a, b)` vs `min(b, a)` the compare operand order and jge/jl flip; try both per statement (d3fl_LightCB).
- `max(1, v)` (constant first) gives `cmp v,1; jae`; `max(v, 1)` gives `jbe`.
- A `return 1` inside the loop branch of an if/else made VC6 defer the callee-saved pushes; with one shared
  `return 1` at the end all four pushes were at entry, as in the original (d3fl_LightCB).
- Statement order of two independent loads (`map = ctx->map; r = obj->unk090.x;`) is visible in the schedule.
- `for (; r->y >= 0; r++)` behind an `if (t && t->wregions)` guard (not `&& r->y >= 0` in the guard) avoids a
  duplicated loop-entry test; a found element returned via `p = ...; break;` + one `return p` (d3fl_WaterPoint).
- `(k + a) + b` parenthesized keeps a float local k on the FPU and adds the members in source order; without the
  parentheses VC6 loads a member first (d3fl_TileLod).
- A recursive call in tail position (flood fill) is turned into a loop, as noted before; matched directly.
- The SameType byte compare needs both operands of the same signedness (`unsigned char t = 0xff` with an
  `unsigned char *types` or both char); mixed signedness adds movsx/movzx.
- `if (x < 0xff)` on an unsigned char compiles to `cmp; jae`, `== 0xff` to `jne`: the original spells the
  water-type test both ways in one function.

### Nonmatching details

#### d3fl_PrepareWater (0x440360, 3856 bytes incl. the switch table)
Complete draft: water type search (strupr + strstr "WASSER"), water mask with 8-neighbour marking, water height
map (copy or AND), shore ramps per row (d3fl_Ramp), divide[0] diagonal switch (codes 1,7,8,9,14 set bit 7;
2,4,6,11,13 clear), CalcRegions (fill 0xff, 0xfe for quads with four wet corners, d3fl_Fill per region), region
array realloc + init (tex "EF_WASS_06A_2T_W_AN0" via tex_Find / tex_Load + tex_Reload, tex_AddRef per extra
region), per-tile strips / points / polygons, removal of strips with flag 0xff (sprintf + memmove), cleanup.
Same 36 calls in the same order; ours is 16 bytes longer. Fixed: tile marking through `ty = y / n; tx = x / n;
map->tiles[ty][tx]` (a t pointer folds 0xe0 into the lea); `max(1, ...)`. Open: the original retests
`v == 0xff` on the (v == 0xff, in) path of the ramp loop (ours threads it; `v != 0xff && in`, `in && v != 0xff`,
`< 0xff`, nested+goto tried), the switch code is built in a byte register in the original (`add bl,bl`, then
`and ebx,0xff`), ours in ebx; local slot assignment differs throughout. 4 tries.

#### d3fl_MakeDivide (0x441300)
Semantics right (level 0 loop, levels 1/2 with step {1,2,4}, d3fl_PrepareWater, tile height range). Open:
- `((abs(..) <= abs(..)) * 2 | d3fl_SameType(..)) << 6`: the original computes `setle; add al,al`, keeps the
  byte across the call, ORs and shifts by 6; ours always distributes the shift (`shl bl,7`, `shl al,6`).
  Tried: byte/int temp, `d |= ...; d <<= 6`, `* 64`, `+`, `? 2 : 0`, `(x << 1 | f) << 6`, casts: no change.
- register choice (map in ebp in the original, edi in ours) and slot order.
- tile height loop: map->n re-read (not a local n) is right; `mn = 0xff` goes through the dead map
  parameter slot in the original (`mov byte [esp+0x48],0xff; mov bl,[esp+0x48]`). 4 tries.

#### d3fl_MakeLight (0x441860)
Same size as the original. Light part: slot offsets / FPU order (`map->unkb8 * map->unkc4` loads b8 first in
the original), the `lea edx,&iv` of the inlined d3fl_FtoI is scheduled before the ftol call in the original.
Divide part: `k = (map->size * y + x) * s` with `map->light[k + ...]` and an unsigned level counter are right.
The divide-edge decision (a, b, c, e = bit 7 of the four corners): set 0x80 for 0001 0110 0111 1000 1110,
clear for 0010 0100 1001 1011 1101, rest unchanged. Kept: else-if groups {0111,1000} set, {1101,1011,0100,
0010,1001} clear, {0001,1110,0110} set. The original tree tests !a first; all its clear paths share one block
while the set block exists three times (first group, {1110, 0001}, 0110), which suggests five groups
(S, C, S, C, S) with clear-block tail merging; tried 288 + 144 group/order permutations (gen_ml5/6 in
build/variants), none closer than the kept one. 8 tries.

## Worker map, round 5 (scope 0x441f50..0x4439e0) -- REPORT [map]

Ghidra missed none in the scope (no ret + new start inside the scope).

In unk_43eab0.c:
- 0x441f50 d3fl_InitMap (divide/light buffers, D3_ZERO + d3fl_InitTile per tile, MakeDivide, MakeLight)
- 0x442080 d3fl_LoadTexture (tile type texture + "_high_1"/"_high_2" mip sources; needs a separate index local
  `k` and one shared `return -1` after `if (k < 8) {... if (tex_FindFile) {...; return k;} sprintf}`)
- 0x442210 d3fl_ReplaceTexture, 0x442280 d3fl_ReloadTextures (= d3_Unk442280 of d3.h; d3fl_Unk43f910 now calls it
  by this name)

Nonmatching (5): 0x442390 d3fl_LoadMap (64 asm lines), 0x442ac0 d3fl_TileBox (24 lines, scheduling only),
0x442ed0 d3fl_UpdateWater (16 lines), 0x4430c0 d3fl_DrawWater (full draft, ~390 lines), 0x4437f0
d3fl_UpdateTiles (75 lines). Details below.

TU boundary: the whole scope is d3_fl (unk_43eab0.c) and it continues past 0x4439e0 (0x4439e0 is called from
d3_engine.c as d3_Unk4439e0(d3_Unk633738, flag); the SelectTextureSet strings after "d3_fl:Floor" belong to the
functions after the scope).

unk_43eab0.c now includes d3_engine.h (after d3.h): no other function changed, and it
gives D3PVERT, D3CLIPINFO, d3_ClipInfo, the fog/projection globals and the sort buffer to d3fl_DrawWater.

Types (own header include/unk_43eab0.h only; shared headers untouched):
- D3FLDESC (new, input of d3fl_LoadMap: name, height/water/types/lightofs buffers, texname, scale, hscale, nregions,
  size, regions, typenames[8][0x40]); D3FL_COPY (copy macro with size, src, dst temps).
- D3FLMAP: lightofs (0x20, was unk20), transform floats orgx..unkc8 at 0x90..0xcc (a 4x4 matrix: origin and the three
  axes), name/texname at 0x19e4/0x1a24, size 0x1c78 (pad1c72).
- D3FLTILE: center (0x00), zmin/zmax (0x50), clip (0x62). D3FLPOLY: unk1c/unk20 are floats (texture offsets).
- D3FLREGION: unk0c/unk10/unk138/unk148/unk14c are floats; speed[4] (0x18), amp[4] (0x28), wave[4][4][4] (0x38)
  (d3fl_PrepareWater's f18..f34 renamed accordingly). D3FLTEX: stamp (0x54), frames/frame/speed (0x70), size 0x80.
- Globals: d3fl_Suffix (0x5ebaa8), d3fl_Unk631090, d3fl_Origin/AxisX/AxisY/AxisZ (0xfbbd30, 0xfc0d50, 0xfbbd00,
  0xfbb8e0), d3fl_Box[8] (0x6334b8), d3fl_LightCol/Ambient/SunCol (0xfbb860, 0xfbbd20, 0xfbb880); prototypes for
  tex_FindFile, bmp_GetInfo, bmp_Load, vfs_Unk42fbf0, vfs_Root, tex_GetFrame (returns D3FLTEX *), d3_AnimDiv,
  d3_Unk439650, d3_ClipMesh (byte mask, D3FLPOINT *, int, D3FLPOLY *, int).
- d3fl_TileLod's third parameter is a float (the mean tile distance), not an int.

VC6 lessons:
- A function called without a prototype still compiles (implicit int) and silently changes the caller's code:
  d3_Unk439650 went from 452 to 388 differing bytes in d3fl_UpdateTiles once declared. grep the build log for C4013.
- Two loops over the same 8x8 grid: the original uses a different column counter per loop (shares the row
  counter); with one variable both loops got the same register.
- Map transform writes through a `D3MATRIX *` cast of &map->orgx (instead of named fields) reproduce the original's
  reloads of map->size after the stores.
- A frame 12-16 bytes larger with three rounded float temps kept in memory (fstp/fld) was a D3VECTOR4 local
  (struct members are not kept on the FPU stack); `volatile float` gives the stores but reuses dead parameter slots.
- `for (i = 0; i < n; i++)` over an unsigned n gives the `test; jbe` guard; `> 0` on unsigned char fields gives
  `test; jbe` too.
- `uv run try`: a replacement with an empty new text (`===` directly followed by `>>>`) is mis-parsed and swallows the
  next block; put a comment as the new text.

Outside the scope: d3_engine.h declares d3_Unk4439e0(void *, char) for the function right after the scope; the
round-4 header-cleanup note (d3_engine.h's d3_Unk43ff90 etc. are d3fl_* functions) still applies.

### Nonmatching details

#### d3fl_TileBox (0x442ac0)
Same instructions except two FPU scheduling spots (the hmax byte load and the final pops are scheduled earlier
in the original). Corner = `p = X*x + O; q = Z*z + p; box = Y*h + q` per component (D3FL_CORNER macro) with
D3VECTOR4 p, q (16-byte vectors fixed the frame). Tried: separate h2 / early h2, hmax directly in the macro,
(float) casts, flat tile index, tile pointer, float-only / D3VECTOR4 box types, box pointer local, __inline
corner function (much worse). 4 runs.

#### d3fl_UpdateTiles (0x4437f0)
D3VECTOR4 c (frame size), map->tiles[y][x] indexing (no tile pointer: the original strength-reduces to
&tile.zmin), separate column counters for the two loops (shares y). Open: the original keeps the first loop's x
in memory and never uses ebx (ours: x in ebx); box-sum pointer reuses the box register. Tried: box++ loops,
unsigned i, declaration order, n/sum init order, split sum statement, n++ position, volatile x (worse), swapped
if/else. 5 runs.

#### d3fl_LoadMap (0x442390)
Full draft. The transform at 0x90..0xcc written through a D3MATRIX pointer (fixes the reloads of map->size
after the stores). Open: the original reloads map->size twice back to back (`mask` / `unk08`, `-size` and
`size / 8`) where ours CSEs one load; the float copies scale/hscale are fld/fstp in the original, mov in ours.
Tried: comma (VSET) statements, a second map pointer alias, casts, split |=. 3 runs.

#### d3fl_UpdateWater (0x442ed0)
Index loop over unsigned n (`jbe` guard), `frames > 0 && speed > 0`, D3VECTOR4 v for the three rounded wave
values (frame size 0x14), double d for the fourth. Open: the original takes copies of the (double)i CSE
(`fld st(1); faddp`) and consumes d in place; ours keeps d as a register variable. Tried: f double local,
volatile floats, inner-block declarations, explicit (double)i, inline 4th expression, long double / float d.
5 runs.

#### d3fl_DrawWater (0x4430c0)
Complete draft from the Ghidra output and the asm: sun = normalized d3fl_AxisY, k = 0.5 - |sun . d3fl_Down|; per
water row the start point (AxisZ*y + Origin, then AxisX*x0, x0 as a float local), height h = k * f14 + flags.b[0];
per point the wave offset, alpha clamp to 0..255 via __ftol, light-map colors through d3fl_FtoI with min(iv, 255)
(bit 7 adds d3fl_SunCol), d3_ClipMesh, projection with / without fog (the d3_ProjectMesh loops of d3_engine.c),
counters, and the polygon queue into d3_SortPtr. Open: register allocation from the start (the original keeps the
tile in esi and walks the row list with a pointer at &r->x0), the inner loop's slots. No `uv run try` runs.

## Worker d3fl4, round 7 (scope 0x4439e0..0x446990) -- REPORT [d3fl4]

Nonmatching (4), all in unk_43eab0.c (asm lines = uv run asmdiff count):
- 0x4439e0 d3fl_Draw (10 KB): full draft, 2773 vs 2817 instructions, 2872 differing asm lines (was 4303 for the
  first draft); every landmark (11 calls, 21 inline copies, one struct copy) lines up.
- 0x446180 d3fl_Prerender: 76 asm lines (was 121).
- 0x4463b0 map_Pick: 84 asm lines (was ~300), first 131 instructions identical.
- 0x446750 map_PickScreen: 84 asm lines (70 before, but now map sits in ecx and the first 12 instructions are
  identical; the rest is a register swap).

Missed starts: none. 0x4439e0..0x44617f is one function (ebp frame, one `ret`, no padding inside); Ghidra was right.

TU boundary: the whole scope is d3_fl (unk_43eab0.c); map_Region (0x446910) is the last function before d3if's scope.

Types and declarations (own header include/unk_43eab0.h only):
- D3FLPOLY: `flip` at 0x26 (bit 0: the other diagonal), pad27.
- New D3FLSPLIT (12 bytes, D3FLTILE.split: point, map x, y), D3FLTRIUV (6 floats, one triangle's UVs, D3FLTILE.bp).
- D3FLTILE use in d3fl_Draw: unk10 = point count, unk20 = triangle count, unk38 = split count, unk40 = UV triples
  used (bp), unk5e = new level, lod = old level.
- Globals d3_Unk5fcc78 (ambient color, D3VECTOR) and d3_Unk633b1c (point light color, D3VECTOR); prototype
  d3fl_Draw(D3FLMAP *, char). d3_engine.h still declares the same function as d3_Unk4439e0(void *, char) (calls
  match through relocations, so nothing breaks; rename at header cleanup).
Shared headers: no changes.

VC6 lessons:
- In the edge passes, `D3FL_POS(p, lx, ly)` with the unsigned coordinates passed straight into a macro that uses
  them three times (conversion CSE'd on the FPU stack) gave the original's order; a float local converted first
  (`fx = (float)lx;`) spills (`fst tmp`) and computes the other axis first (1000 fewer differing lines).
- `if (c) {A} else {B}`: the original's fall-through block is the `if` body; when the flip test's fall-through used
  poly[1], the source tested `poly->flip & 1` (not `!(... & 1)`). Swapping the branches fixed hundreds of lines.
- Identical tails of both branches must be written in both branches when the original has them twice (no tail
  merge across inline-asm copies with different labels).
- `min(0xff, iv)` gives `cmp; jle`, `min(iv, 0xff)` gives `jl`.
- A frame 8 bytes too small with x/z floats 8 apart was a D3VECTOR4 (map_Pick); `*py = ...` without a local y lets
  VC6 forward the stored value (the original reloads *px but not *py).
- A masked copy of a coordinate in another register (`and esi,ebp`) is a new variable (`ix = x & ~(s - 1)`).
- `ret = k << 16 | j` vs `k * 0x10000 | j` changes register allocation of the whole function.
- `if (!t->unk5e) s = map->lod ? map->lod : 1; else s = t->unk5e;` (if/else with a tile pointer) gives the
  `test al,al` on the byte and the threaded `mov ebp,1`; `if (!(s = t->unk5e))` zero-extends first.

### Nonmatching details

#### d3fl_Draw (0x4439e0, 10144 bytes, one function)
The floor draw called from d3_engine.c (d3_Unk4439e0(map, flag)):
- light colors (map->nolight: ambient/sun 0, light 1; else d3_Unk5fcc78 / d3_Unk633b1c / map->ambient), camera
  transform of the map origin and axes into d3fl_Origin/AxisX/AxisZ/AxisY;
- if map->dirty: d3fl_UpdateTiles; per tile vertices (height + light map, D3FL_LIGHT macro, 5 copies in the exe)
  and, when a level changed, the triangle pairs (diagonal from divide[s >> 1] bit 7, jitter UV set from d3_Noise if
  bit 6, texture from d3fl_GetTexture); then a second pass adding split vertices on edges next to finer tiles
  (right, left, top, bottom; D3FLSPLIT list at tile+0x30, extra triangles appended, private UV triples at tile+0x3c,
  counted in tile+0x40), or moving the split vertices when nothing changed; then d3_ClipMesh, projection (with fog
  when d3_FogOn) and the triangle flag update; dirty = 0;
- water: d3fl_UpdateWater(d3_Time, nregions, regions);
- per visible tile: d3_MinZ/MaxZ, counters, sort entries for visible triangles (rendering the cache texture on first
  use, as d3fl_Prerender), d3fl_DrawWater.
Fixed: top edge duplicates its tail in both branches (6 copies in the exe); left/top edges test `poly->flip & 1`
first; edge positions via D3FL_POS(p, int x, int y); `min(0xff, iv)`.
Open: frame 0x14c vs 0x150, map in esi vs edi (the original keeps map in edi through the header), slot assignment
everywhere, FPU order of the vector setup in the first loop (`fld st(1); fmul [AxisX.y]` vs ours
`fld [AxisX.y]; fmul st,st(2)`), the projection loop's x/y/z reloads (the original loads z, y, x before d3_Dist2).
Tried and worse: a separate temp vector q in D3FL_POS (O added before the Z part), q instead of a for the AxisY*h
products in the edge passes; no effect: types/height load order, lod compare operand order, `fs` local vs int
multiply, cast of d3_Camera. ~8 compile runs.

#### d3fl_Prerender (0x446180)
Fixed: lod fallback as above, `if (d3_Unk633450 && d3_Camera) {... return count;} return 0;` (count = 0 stored as an
immediate before the test), `!(tex->state & 0x80)` (js), coordinates `mx + i * s`, `my + j * s` from the loop
counters (strength-reduced like the original). Open: n in esi (original: n in memory, tx in esi), slots shifted by
one. Tried: int n, int tx/ty, unsigned size, chained nx/ny, my in the outer loop, statement and declaration order,
swapped 7-tests, tile field vs pointer, ternary forms. 5 try runs.

#### map_Pick (0x4463b0)
Fixed: `D3VECTOR4 d` for the map-space offset (frame 0x18), `*px = x = ...; *py = ...;` without a y local, a separate
`ix = x & ~(s - 1)`. Open: the u+v>1 branch spills map ([esp+0x38]) and loads both height bytes before the filds;
scheduling in the other branches. Tried: x1/y1 order and inlining per branch (no effect or worse), separate
assignments, nested ifs, compare on locals. 5 try runs.

#### map_PickScreen (0x446750)
Same instructions as the original except registers: the original keeps ret in esi (with a home slot) and loads map
into ecx only; ours keeps tx in ebp and ret in memory. Tried: ret initializer at declaration / before r2, nested if,
tile pointer, register keyword, operand order of k. 3 try runs.

## Also about this unit
- notes/units/d3_interface.md, section [d3if]

## match-base session 2026-10-24 (base-3 worker, shard 3/5)
Reference updates only (renames in a shared header): d3_Unkfe75b0 -> d3_DrawnVerts. No code change in this unit; the full ledger check
stayed at 2501 matched functions.

## match-base session 2026-10-24 (base-0 worker, shard 0/5, session 2)
**Raised to byte + readable:** d3fl_InitCache, d3fl_FreeCache, d3fl_Invalidate, d3fl_SetMips, d3fl_Init,
d3fl_Exit (Q6 descriptions), d3fl_ReloadMap (0x43f910, was d3fl_Unk43f910: reloads the textures of a map if
there is one; the name is the role, named from the two d3fl_SetMips call sites). Side effects of the renames:
d3fl_Flush, d3fl_LoadTexture, d3fl_ReplaceTexture and ch.c's ch_FollowGround became readable too.
**Renames (project-wide):** D3SCENE.unk0ac -> map (the scene's terrain map; D3IFSCENE already had map at 0xac;
reference sites in ch.c, d3_tools.c, d3_int_io.c), d3fl_Unk633748 -> d3fl_DetailScale (1 << the trailing zeros
of the detail level, 0 when off), d3fl_Unk63373d -> d3fl_MapShift (copied into D3FLMAP.shift), d3fl_Unk631090 ->
d3fl_EmptyName (the shared empty string, ch.h's ch_EmptyName at 0x631090), D3FLREGION.unk0c/unk10 ->
scrollSpeedU/scrollSpeedV (the fmod(t * speed + offset, 1) texture scroll), D3FLMAP.unkc4 -> hscale.
**Escalated:** d3fl_BlendC (frame 0x24 vs 0x30 and the register roles of s0..s3 / the weight bytes; the
operations and the 208/214 equal instructions agree, it is another register/slot spelling), d3fl_MakeJitter (the
original materializes f, g and the intermediate u/v in stack slots - frame 0x84 vs 0x60 - and spills them
around every call; the values, the 12 fixed u/v entries and the 0x60 loop stride were verified against the
disassembly).

## match-base session 2026-10-24 (base-0 worker, shard 0/6)

**Raised to byte + readable:** d3fl_TileLod (lod field renames below; the lod delay logic reads t->lod,
which is only ever -1 from d3fl_MakeDivide / 0 from the zeroing, so the deferral branch is dead as far as
the sources show), d3fl_LightCB (D3LOBJ light view: obj->power / range, see d3_light.h).
**Escalated:** d3fl_SetDetail (code hunk: the original keeps n in eax and the clamp in edx, ours the other
way round; c2prio m 38-40 vs n 11-15, the clamp ternary/if variants do not move it), d3fl_Alloc and
d3fl_GetTexture (Q2 only: D3FLTEX/TSTEXTURE unk58, slot 0x58 is only ever zeroed - Alloc eviction,
GetTexture creation, both Draw re-render sites, ts_texture init, d3_tools DC_ unload - and never read;
GetTexture's Q4 *(int *)&flags is fixed with a local int/D3TEXFLAGS union view), d3fl_PrepareWater
(code 1285/slots 200/regs 6, round-4 reading: ramp v == 0xff retest, byte-register divide switch),
d3fl_MakeDivide (code/regs: map in ebp vs edi and the (abs<=abs)*2|SameType<<6 shift distribution),
d3fl_MakeLight (code/fpu/slots: FPU operand order, frame 0x60 vs 0x68, divide-edge groups after 288+144
permutations).
**Renames (project-wide / all references):** d3fl_Unk633750 -> d3fl_DetailShift, d3fl_Unk633751 ->
d3fl_DetailSizeShift, d3fl_Unk633740 -> d3fl_LightMargin, D3FLMAP.unk1c64 -> lodDist2 (20.0f),
D3FLMAP.unk1c68 -> lodDist4 (40.0f), D3FLMAP.unkb8 -> negScale, D3FLTILE.unk48 -> lodDistBias (never
written: always 0; added to both lod thresholds), D3FLTILE.unk4c -> centerDist, D3FLTILE.unk58 ->
lodDelay, D3FLTILE.unk61 -> nextLod.
**Headers:** d3_fl.c now includes d3_light.h (with the d3_RelightObject -> d3_RelightObject_void rename
around d3.h, as d3_light.c does); its d3fl_MakeLight declaration is struct D3FLMAP * (d3_light.c passes the
void * d3_Terrain, which C converts implicitly), so the d3fl_MakeLight (symbol, d3_fl.h/d3_light.h) lint
pairs are gone (lint_baseline.txt lost those two pairs and the two stale ai_Unk430bd0 pairs).
**Not done:** an anonymous union of D3VECTOR views at D3OBJECT/D3OBJX 0x90 (lightParams / angles / vec)
flips d3_CloneObject from byte (0x43d180, same effect as the D3PTRI/D3TEXTURE unions in d3_light.md) - it
was reverted; LightCB and MakeLight read the light fields through D3_L() instead. Q2 unk05c (D3LOBJ color,
evidenced) is left for the same reason: renaming or union-izing the D3OBJECT field is a shared-header
change that must keep d3_CloneObject byte. Next base item: d3fl_InitTile (Q2 D3FLTILE/D3FLPOLY unk
fields, Q3 0xffffff).

## match-base 2026-10-24 (base-0 worker, shard 0/6)

The unit's base queue is empty (9 -> 0). Raised to byte + readable: d3fl_InitTile (D3FLPOINT as the D3PVERT views
color/specular/src/flags, D3FLPOLY as the D3PTRI views clip/tex/zOfs/uOfs/vOfs/flags, D3FLTILE.npoints/npolys/
lodLevel, D3FL_WHITE for the white vertex colour) and d3fl_UpdateWater / d3fl_FreeTiles / d3fl_FarDist (through the
field renames: D3FLMAP.idxMask, D3FLREGION.wavePhase/uOfs/vOfs, the transform rows xAxisY/xAxisZ/zAxisX/zAxisY/
yAxisX/yAxisZ). Escalated to match-deep with the reason in each @deep tag: LoadMap (6 code hunks, same operations:
value reuse/lea vs dec, alloc/copy scheduling, integer division vs FPU mult, mov vs fld/fstp copies), DrawWater
(518 of 580 hunk instructions in another register/slot allocation; one meaning fix kept: the npolys min is signed),
UpdateTiles (local layout and tile pointer walk; the unsigned guards are now spelled > 0 for the original's jbe),
map_Pick, map_PickScreen, Prerender (register/slot allocation), Draw (full draft: map in esi vs edi over 2872
instructions, frame 0x14c vs 0x150). Unit readability still needs the canonical header: 11 foreign declarations
(d3_engine.c, d3_interface.c, d3_snd.c, f3init.c, ls.c, unk_58bbd0.c, ch.h).

## match-base 2026-10-26 (base-3 worker, shard 3/6)

The `#define d3_RelightObject d3_RelightObject_void` hack around #include "d3.h" is gone (d3.h no longer uses it in the d3_light sweep). No byte change.
