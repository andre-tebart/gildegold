# d3_tools.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (leaked): D:\develop\Units\d3_tools\d3_tools.c
- Boundaries: separated (plausible): leaked; start next to di_input.c (plausible)
- Range then: 0x411680..0x4175c0

## Open work
- **headers (2026-10-30, typing pass 2)**: head.c includes d3.h, d3_d3d.h, d3_io_t.h, surf.h, d3_tools.h; no @original cast or view.
- match 2026-10-23 (4th), no gain: d3t_CreateSmoke (21 B): `now` as unsigned int / DWORD / int and all 6 term orders of the time sum: same bytes. d3t_UpdateSnow (33 B / 8 lines, the int loads of hw / hh sit earlier in the original, between the fmuls of the fall.z statement): hw / hh (either order) moved to every position among the wind.z / gy / fall.x..z statements (42 variants): same or worse.
- match 2026-10-22 (2nd pass), no gain: d3t_CreateSmoke (21 B; `now` load and `i++` scheduling in the last statement): term orders of `(rand() & 7) + i * delay + now` (5), `p++, i++`, tm temps (4): no change. d3t_EarthCB (15 B): the original's `dead` / `a` stack slots swapped; all 719 declaration orders of the six local declaration lines give the same code (slots follow use weights, not order).
- see notes/units/d3_engine.md, Open work: "Header cleanup from round 8: d3_engine.h's d3_Unk4538a0(int) takes a char (ocmd's d3_Light ..."

## match-base 2026-10-24 (match worker, reference update from d3_anim.c)

- The camera-flight reset calls d3anim_FreePath now (d3_Unk433b10 rename); byte unchanged (52/59, 978 B).

## match-base 2026-10-24 (base-3 worker, shard 3/5, reference update)

d3_Unk43f990 -> d3fl_Flush (the d3_fl.c name) at the one call; byte unchanged.

## match-base 2026-10-24 (base-0 worker, shard 0/6)

d3t_PlaceObject escalated (same statements, other registers/slots and scheduling; @deep reason). Raised to byte +
readable: d3t_PlaceObjectV, d3t_FindObjectCB, d3t_FindObject, d3t_PlayAnim, d3t_ReleaseObjectTexture,
d3t_ReleaseObjectTextures, d3t_FindFreeScene, d3t_SceneIndex, d3t_GetHeightRange, d3t_MoveCameraPath (byte) and
d3t_PlaceAtObject. d3t_GetBoundingSphere is readable but its behaviour is still draft (recorded stuck: the
`fcom [d]` direction); d3t_MoveObjectPath is readable, behaviour still draft (79 code hunks, not read this session).
Renames: D3MATERIAL.unk0f4 -> animCtl (the D3ANIMCTL block at 0xf4 passed as "list" to d3anim_StartAnim /
d3_ReleaseTexture), D3MATERIAL.unk008 -> meshData (the D3MESH layout from 0x08; its worldMatrix is what
d3_GetBoxVerts / GetBox get), d3_Unk433d70 -> d3anim_CreatePath (with the ch.h macro ...K). New local D3OBJPOSE view
in d3_tools.c (type debt): a camera / zoom target stores its pose in D3OBJECT 0x5c (pos) and 0x90 (angles) - the
bytes D3LOBJ calls color and range/power/atten; the union view in D3OBJECT flips other units' code, so it stays a
cast view.

## match-base 2026-10-24 (base-0 worker, shard 0/5)

Raised to byte + readable: d3t_InflateObjectTree (was d3t_Unk413b00), d3t_CreateRain, d3t_DeleteRain (was
d3t_Free413d00), d3t_DrawRain, d3t_DrawSnow. d3t_GetBoundingSphere behaviour + readable (was draft; the Open work
entry is resolved): the squared distance is stored through a volatile union view and read through a plain one
(`union { volatile float sum; float dist; }`), which gives the original's `fstp [slot] / fcom [slot] / je` max; only
the fsqrt position is left (order 1 hunk).
Escalated: d3t_MoveObjectPath (all 6 diffclass hunks read: the code hunks are the same statements with i/k in
swapped registers - ours i=ecx/k=edx, original k=ecx/i=edx - plus k store/reload placement and nop fill; no meaning
error), d3t_CreateSnow (Q2: D3TSNOW.unk10/unk14[9]/unk54/unk58 only ever written, never read), d3t_FreeSnow (Q2:
D3TEXTURE.unk58 only zero-stored, the same evidence gap the d3_fl.c 0x43faa0 note records; refs/filebpp, first,
d3fl_ReloadTextures and the fourcc alias are resolved), d3t_SetSnow (Q2: D3TSNOW.unk54).
Renames: d3t_Unk413b00 -> d3t_InflateObjectTree; d3t_Free413d00 -> d3t_DeleteRain (the D3TUNK413D00 view is gone,
the function takes D3TRAIN* and frees D3TRAIN.drops); D3TRAINDROP.unk10/unk14 -> windScale/fallScale and the same
on D3TSNOWFLAKE; D3TEXTURE.unk40 -> refs, unk7d -> filebpp (TSTEXTURE); D3SCENE.unk080 -> first (0x80 is
D3SCENELISTS.first, the scene's top-level object list); d3_Unk442280 -> d3fl_ReloadTextures (d3.h prototype, type
struct D3FLMAP*); d3_Unkfe79c4 removed - 0xfe79c4 is d3d_UsedTexFormat.fourcc, d3_d3d.h now included by d3_tools.c.
Unit header debt (Q7): ecmd.h still declares d3t_FindObject / d3t_PlaceAtObject / d3t_MoveCameraPath /
d3t_MoveObjectPath / d3t_CreateRain / d3t_DeleteRain because the unit has no d3_tools.h yet.
Next in the base queue: d3t_EmitterCB (Q2: unk10, unk24).

## match-base 2026-10-25

Unit d3_tools.c; check 2522/4664 before and after; d3_tools.c unitdiff 892 differing bytes unchanged
throughout; no grade fell.
- d3t_EmitterCB readable (byte) through the evidenced field renames D3PARTICLE.unk10 -> start (start
  position; .x is the brightness in the emitter: `p->start.x = 255.0f`) and D3PARTSYS.unk24 ->
  spawnCount (`spawned < ps->spawnCount`, d3t_CreateEmitter sets it to 2; the union keeps color24).
  The same rename raised d3t_BloodCB, d3t_ExplosionCB, d3t_SmokeCB, d3t_EarthCB (byte + readable) and
  ocmd_ParticleCB (ecmd.c). Renaming in d3.h and fixing the compiler-flagged D3PARTICLE sites (the
  #line chain maps the reported lines to the source lines; d3_tools.c has an unrelated D3TSNOW.unk10).
- D3TEXFLAGS gained an anonymous-union dword view `all`; `flags.all = 0` replaces every `*(int *)&flags`
  (d3t_CreateEmitter / Blood / Explosion / Smoke / Earth, d3t_UpdateSunRays, d3t_CreateSnowMipmaps) -
  byte-neutral.
- Readable (byte): d3t_CreateEmitter, d3t_CreateBlood, d3t_CreateExplosion, d3t_CreateEarth,
  d3t_CreateSunRays, d3t_SmoothStep, d3t_UpdateSunRays (Q6 descriptions + Q4). d3t_CreateSmoke became
  behaviour + readable (Q4 only; order 14 stays).
- d3t_CreateSnowMipmaps: readability done (Q3: D3_MASK_RED names the 0xff0000 red-channel mask; Q4
  flags.all) and escalated: 231 code hunks / 30 slots; the recorded n-reload lead was tried (size*size
  inlined in the random-fill loop) and changed nothing; left is the D3_ZERO slot forms, the ddpf height
  stores, the pitch mul/store order and the n/up/row/down slots (allocation/scheduling).
- d3t_FadeOutSunRays / d3t_FadeInSunRays escalated: Q2 d3t_Unk631128 is write-only (FadeOut stores its
  argument, FadeIn 0) with no reader in src and no caller of either function; no role evidence. Q6 for
  0x417170 fixed.
- **d3t_RemoveSunRays blocked, lead recorded (not started):** its callee 0x447190 is already canonically
  `d3if_Delete` (d3_interface.c's 0x447190 comment "unlinks and disposes an object" and definition
  `char d3if_Delete(D3OBJECT *obj)`; canonical prototype in d3_interface.h, also rd.h / ecmd.h). The
  same function is called under two spellings in 41 places: d3_Unk447190 (ch.c 6, d3_tools.c 1, d3.h
  declaration `void d3_Unk447190(D3OBJECT *)` - wrong, it returns char) and ai_Unk447190 (cm.c 10,
  gv.c 3, he.c 3, main.c 2, sv.c 2, unk_58bbd0.c 1, f3init.c 2, plus K&R declarations in cm.h, gv.h,
  f3init.c, he.c, sv.c). The dual spelling exists to dodge the void declaration; d3_interface.c works
  around it with `#define d3_Unk447190 d3_Unk447190_void`. The merge: fix d3.h's line to
  `char d3if_Delete(D3OBJECT *obj)`, drop the K&R declarations, remove the macro trick, include
  d3_interface.h where needed and cast the int / char* call sites to D3OBJECT*. Q6 also needed.
  Left for the next base session (a wide type-debt cleanup, not a match-deep item).

## match-base 2026-10-25 (base-0 worker, shard 0/6)

- d3t_RemoveSunRays byte + readable: its callee 0x447190 was already canonically d3if_Delete
  (d3_interface.c). The dual spellings d3_Unk447190 (ch.c 6, d3_tools.c 1, d3.h's wrong void prototype) and
  ai_Unk447190 (cm.c 10, gv.c 3, he.c 3, main.c 2, sv.c 2, unk_58bbd0.c 1, f3init.c 2) were merged
  project-wide: d3.h now declares `char d3if_Delete(D3OBJECT *obj);`, the K&R declarations in cm.h, gv.h,
  f3init.c, he.c, sv.c (2), unk_58bbd0.c (2) became one full prototype
  `char d3if_Delete(struct D3OBJECT *obj);` with `struct D3OBJECT;` forward declarations (gv.h, cm.h, he.c,
  unk_58bbd0.c, main.c), d3_interface.c's `#define d3_Unk447190 d3_Unk447190_void` trick is gone and the int
  call sites cast to `struct D3OBJECT *`. All ten touched units byte-unchanged. Side effect: ch_SetHandObject
  (ch.c) became readable (it called that placeholder).
- d3t_FreeSnowMipmaps byte + readable (Q6 description of the four freed snow mipmap chains).

## match-base 2026-10-26 (base-3 worker, shard 3/6)

No change beyond a test: d3_light.h was included and removed again (d3t_CreateSnowMipmaps gained 2 instructions; the d3.h declaration of d3_RelightObject is enough).
