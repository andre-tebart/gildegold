# d3_anim.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (leaked): named from a source path before round 12 (not in tu_anchors.csv)
- Boundaries: separated (proven): vfs.c tag / d3_d3d.c leaked
- Range then: 0x4315c0..0x435950

## Open work
- Nonmatching functions (177 after round 17): `uv run ledger check --verbose | grep FAIL`; per-function attempts in
  notes/vc6-matching.md and notes/units/<unit>.md (worker sections d3_d3d, dd_vesa, net, script, sblib, d3_engine, d3_anim, evt,
  d3_left, d3_engine2, start401, sfx, d3fl, ch, ch2, ch3, ch4, map, ch7, d3fl4, d3if, d3if2; round 6 left none). Only worth a worker when grouped by TU; d3_anim.c has 5 close ones (1-16 bytes
  off), evt_* 4 near-misses, d3fl_SetDetail / sfx_OpenDigOutput / ch_RemoveAction / ch_AnimDone / ch_Unk404a80 are a few instructions off; ch4's three path functions share one
  unresolved x/z stack-slot swap; d3fl_UpdateWater / d3fl_TileBox differ in FPU scheduling only.
- d3_Unk432750 (0x432750..0x433b0f, the per-object animation step, d3_ForEachObject callback; layout 2026-10-23): ROUGH draft (4480 B vs 5056 B): vertex animation slots of the five controls of the material (D3ANIMMATV view: ctl at +0xf4, nctl +0x96c, flags +0x96d), then the object path (hermite through the keys, D3PATH) and the tail (0x4535b0 / 0x435900). Raw offsets (SLOT_F, OBJ_LAST) and D3ANIMMATV are local to d3_anim.c; the bone attach block (slot +0x14 / +0x18 / +0x1c / +0x30) and the spline end-point clearing are in. Next: struct fields for the slot (+0x4c / +0x54 / +0x5c floats, +0x30 attached object), the key types of the jump-free path loop (`stepped` label), then the matching.
- Type cleanup: D3MATERIAL+0xf4 is the D3ANIMCTL (slots at +0x110, so `tex[3]` at 0x178 is really slot
  data) and D3BONEANIM == D3ANIMDATA (notes/units/d3_anim.md). unk_45db40.c declares d3_Unk43b300 as
  `(int, int)`; it is d3_RadixSort(unsigned n, unsigned char twobytes). d3s_Count 0x633dc0 is unsigned in
  d3_engine.c, int in unk_45b510.c. d3.h's d3_Unk439870 takes a char third parameter (hidden by a #define);
  d3_Unk4385a0's first parameter is a byte. d3_Planes (0xfc5068) is 6 planes (4/5 = znear/zfar).
  d3_engine.h's d3_Unk43f930(int) / d3_Unk43f8a0(int) are d3fl_SetMips(char) / d3fl_SetDetail(unsigned).
- see notes/units/d3_interface.md, Open work: "Round 9 header cleanup: unk_40d680.c's d3_Unk449de0 / d3_Unk44a170 are char d3_SetLightSet ..."

## Worker d3_anim (round 2, scope 0x430130..0x434a80) [d3_anim]

### Report
- Nonmatching (12): close ones are 0x431750 (1 byte), 0x4319e0 (2 insns), 0x433b50 d3path_Init (12 bytes),
  0x433d70 (16 bytes), 0x430fa0 vfs_fseek (register permutation); far: 0x430470 vfs_Tfopen, 0x430bd0 fread,
  0x430e30 fwrite, 0x4311d0 MemOpen, 0x431ae0 (first draft, no tries yet), 0x432190.
- Still without source: 0x432750 (missed function start: Ghidra's 0x432530 "5.6 KB" is 0x432530..0x432744
  plus a 5 KB function at 0x432750..0x433b0f, not in functions.csv/names.csv yet) and 0x433f80 (2.8 KB).
- TU boundary: unk_430130.c (vfs) ends with vfs_Exit at 0x4315bb; d3_anim.c starts at 0x4315c0 (root-motion
  helpers taking D3ANIMSLOT). d3_anim.c now runs 0x4315c0..0x4359f0.
- Shared-header additions: none (d3.h untouched). Own headers: unk_430130.h (VFS_COPY macro, VFSENTRY
  unk0c/unk10 now uLong), d3_anim.h (D3PATH/D3PATHKEY, D3_OBJPATH/D3_OBJOFFSET, D3PATH_COPY, D3BONEANIM/
  D3BONESLOT, D3ANIMBONE; D3ANIMFRAME move/turn/bone[4], D3ANIMDATA.nbones, D3ANIMSLOT.map, D3ANIMMESH
  bones/nbones; d3_Unk4319e0 returns char and 4319e0/431ae0 take D3OBJECT*; prototypes for 0x4539b0 lerp,
  0x454690). Finding: D3MATERIAL+0xf4 is the D3ANIMCTL (slots at +0x110, mesh at +0x104, active at +0x288),
  so D3TEXSLOT tex[3] at 0x178 is really slots[i].data..; D3BONEANIM == D3ANIMDATA.
- VC6 lessons: (1) `v = M*d + pos` written with the position added in separate statements (`v.x = dot; ...;
  v.x += pos.x;`) - several root-motion functions only match that way; same for spline sums then `*= 0.5f`.
  (2) A flags parameter tested `and eax,1; cmp al,1` is an int with `(flags & 1) == 1`. (3) `(b & 1) || (b & 0x10)`
  keeps 0x11 an immediate where `b & 0x11` gets hoisted into a register. (4) Strength reduction over
  `obj->material->tex[i]` needs the material in a local. (5) strcpy/strcat/strchr/strrchr are intrinsics; ftell
  needed one `return r` with if/else on the flag to get the tail calls.

### Attempts on nonmatching functions

#### vfs_fwrite (0x430e30)
- Original keeps -1 in eax from the entry (`or eax,-1` before `test edi,edi`) and shares one epilogue with the
  fwrite() path; flags byte in dl, mem bit in cl, fp in ecx. Ours: fp in eax, flags in cl, so -1 can't be hoisted.
- Tried: a `ret` variable (all forms, also `register`): it goes to memory (function has __asm) and gets worse;
  guard-style early returns; `(fp = f->fp)` inside the condition; no `mem` local; int/uchar `mem`;
  declaration order. None moved the registers.
- `&f->zs` is hoisted to the top in ours (`lea edx,[edi+0x100]`), the original computes it at the deflate call.

#### vfs_fread (0x430bd0)
- Same register swap as fwrite (orig flags dl, fp ecx; ours cl, edx). `ret` is in memory in the original
  and stored before every return (`mov [ebp-4],x; mov eax,x`); ours drops those stores. ret is unsigned (jb).

#### vfs_fseek (0x430fa0)
- Structure matches with `if (cur == offset) ret = 0; else { skip }; return ret;` (the original stores 0 into ret
  and returns it). Left: register permutation, orig f=ebx, ret=edi, p=ebp; ours f=edi, ret=ebp, p=ebx.
- Tried: block-scoped p, `ret = 0` returns as expressions, a switch on origin (much worse), compare operand
  order, initializing ret late.

#### vfs_MemOpen (0x4311d0)
- Mode loop needs a separate pointer (`for (p = mode; *p; p++)`). Fail paths: `goto fail` from the size check
  and the inflate test into the deflate failure block (closest).
- Left: (1) slots of read/z swapped (orig read [esp+0x18] initialized with an immediate, z/detect from bl);
  declaration/initialization order makes it worse. (2) `xor eax,eax` hoisted at entry for the `return 0`
  path in the original (shared epilogue). (3) The gzip detection: original places the not-compressed
  stores right after `z = 0` (jump-threaded), and tests the inflate result as `je ok; jmp fail`.
- Tried: `!z` branch first, `z = size > 10 && ...`, inverted condition, `size <= 0`, char/uchar read,
  explicit fail blocks per site.

#### d3path_Init (0x433b50)
- `(b & 1) || (b & 0x10)` instead of `b & 0x11` keeps the 0x11 an immediate (VC6 merges the tests but
  does not hoist the constant into a register then).
- Left: in the first loop the strength-reduced key offset comes first in the address (`[edi+ecx+0x20]`),
  original `[ecx+edi+0x20]`. Tried `(p->keys + i)->`, `i[p->keys]`, 0.0f vs 0.

#### d3_Unk433d70 (0x433d70)
- Position/angle copies are per component (a struct copy gives a different pattern). The key copy is an
  inline-asm macro with the destination temp declared first (D3PATH_COPY in d3_anim.h).
- Left: the two flag RMWs (b[1] &= ~0x20; b[8] = b[8] & ~0x20 | 0x10) are interleaved through bl/cl in ours,
  serialized through cl in the original. Tried bitfield overlays, statement order, a `D3ANIMFLAGS *` local
  (the original has &p->flags in [ebp-8], but using it makes things much worse).

#### d3anim_Unk431750 (0x431750), 1 byte off
- Matches except `lea ecx,[esi+edx]` vs original `lea ecx,[edx+esi]` (frames + frame*0xc0 before the last
  segment). Tried `1 + frame`, `(data->frames + frame)->`, an int temp for the sub-frame count, a ternary,
  declaring the counts without initializers, re-reading the count instead of n1.
- Lesson (also 0x4315c0): `v = M * d + pos` where the original adds pos in separate statements
  (`v.x = dot; v.y = dot; v.z = dot; v.x += pos.x; ...`) - the z sum stays on the FPU stack, x/y are reloaded.
  The flags parameter is an int tested as `(flags & 1) == 1` (`and eax,1; cmp al,1`).

#### d3anim_Unk432190 (0x432190)
- Structure and FPU code are close; the sum is deliberately left uninitialized when key >= frame (the
  original loads garbage from the stack there too). Left: frame 0x24 vs ours 0x20 - the original keeps
  &frames[key] in a stack slot and &frames[frame] in ebx, the counts and sub0 in dead parameter slots.
  Tried pointer locals f0/f1 (worse), plain `data->frames[...]` (current), a cached frames pointer.

#### d3_Unk4319e0 (0x4319e0), 2 instructions swapped
- Needs the material in a local (`mat = obj->material`), then the loop over tex[i] is strength-reduced to
  &tex[i].map as in the original. Left: after the child loop the original reloads obj (esi) before anim (eax);
  ours the other way. Tried while/do/for forms, a dead `anim = 0`, compare operand order, no anim local.
- Changed d3_anim.h: d3_Unk4319e0 returns char (`xor al,al`), both 4319e0/431ae0 take D3OBJECT *; the two
  callers cast (no code change, they still match).

#### vfs_Tfopen (0x430470)
- VFSENTRY unk0c/unk10 are uLong (unzGoToFilePos arguments; `jbe`).
- Left: the original keeps a VFSFILE* in a stack slot ([esp+0x24], initialized 0 next to ebp=0) that is
  stored only in the zip path and returned on "not found"; ours constant-folds it (frame 0x364 vs 0x368), which
  shifts all locals. Also the binary flag is initialized with an immediate in the original. Tried a separate
  `ret`, `return f` at the early exits, an `if (f) goto opened` after the zip search.

#### d3_Unk431ae0 (0x431ae0)
- First draft only (no try runs): structure from the asm; 1158/1712 bytes differ. Next: compare slot/frame
  layout (orig frame 0x34c, ebp frame from D3_ZERO), the material reload per slot, the `i >= n` branch order.

## Also about this unit
- notes/units/d3_d3d.md, section [d3_d3d]
- notes/units/d3_engine.md, section [d3_left]

## match-base 2026-10-24 (match worker, 5th)

- d3anim_Unk431750 -> d3anim_ApplyMotionTo, behaviour + readable: D3ANIMSLOT.unk0c -> lastFrame,
  unk10 -> lastSub (the slot's last root-motion frame / sub-frame; all uses are in d3_anim.c).
- d3_Unk4319e0 -> d3anim_MapBones, behaviour + readable: Q4 `*(int *)bs->map = -1` -> D3BONESLOT.mapAll, an
  anonymous-union view (map[4] / mapAll) confined to d3_anim.c.
- d3_Unk4324e0 -> d3anim_NextFrame, byte + readable; the d3_io_t.h declaration and its call in d3_io_t.c
  follow the rename, and the two lint_baseline lines were renamed too (same declared-differently conflict).
- d3anim_Unk4315c0 -> d3anim_ApplyRootMotion, Q1/Q6 done, Q2 escalated (unk084).
- d3_Unk433b10 -> d3anim_FreePath, Q1 fixed, Q4 escalated.
- Escalated: d3anim_ApplyRootMotion (Q2 obj->unk084, 118 refs; the d3.h union view made d3_Unk4550e0 stop
  matching by 8 bytes, reverted), d3_Unk431ae0 (B draft, 551 code hunks), d3anim_Unk432190 (B draft, 407 code
  hunks, recorded attempts), d3_Unk432750 (B draft, 2599 code hunks, ROUGH), d3anim_FreePath (Q4 D3_OBJPATH:
  the typed `struct D3PATH *path` field at 0x1d0 regressed d3_Unk4550e0 the same way and moved 4 bytes in
  d3_anim.c, reverted), d3path_Init (B draft, 46 code hunks; Q2 callee 0x453d60 = d3_UnwrapAngles in
  d3_vec.h but d3.h declares the D3VECTOR* view as d3_Unk453d60), d3_Unk433d70 (B draft, 20 code hunks; Q2
  unk084 and Q4 D3_OBJPATH blocked as above), d3anim_CreateMorphAnim (B draft, 306 code hunks),
  d3anim_CalculateAnimNormals (B draft, 34 SIB base/index-order code hunks, otherwise readable).
- Next in the unit: d3_Unk435550 (Q1/Q2 naming).

## match-base 2026-10-24 (match worker, 6th)

- The slot/data fields are named (all uses in d3_anim.c, ch.c, chc.c, ecmd.c, unk_4591a0.c):
  D3ANIMSLOT.unk04 -> `nextFrame` (the frame d3anim_NextFrame returns, the blend partner of `frame`),
  unk08 -> `subFrame` (position inside the frame), unk14/unk18/unk1c -> `attachFirst`/`attachLast`/
  `attachBone` (the bone-attach frame range and bone index), unk30 -> `attached` (the attached object),
  unk34/unk38 -> `fadeStart`/`fadeEnd` (int ms, as before), unk44/unk48 -> `fadeFrom`/`fadeTo` (weights),
  unk3c -> `timeout` (how long the animation may run, -1 = none), unk60 -> `speed`, unk64 -> `frameTime`;
  D3ANIMDATA.unk150/unk154/unk169 -> `first`/`last`/`relative`, the D3BASTREAM (d3_io_b.h) names for the
  same 0x150/0x154/0x169 fields. CHSLOT (ch.h) keeps its own unk60 view.
- Function names: d3_Unk431ae0 -> `d3anim_PoseBones` (poses the child objects from the skeleton animations),
  d3_Unk435550 -> `d3anim_StartAnim` (starts the named animation in a free slot), d3anim_Unk435810 ->
  `d3anim_PauseAnim`, d3anim_Unk435870 -> `d3anim_PlayAnim` (the pause/resume pair of ch_PauseAnim).
- Raised to byte + readable: d3anim_StartAnim, d3anim_PauseAnim, d3anim_PlayAnim, d3anim_SetFrame (Q6).
  The unit's base queue is empty; the deep functions are unchanged. The @deep reasons that named unk04/unk08/
  unk169 as Q2 blockers now say what remains (d3_Unk4539b0, unk0f4, unk084, D3_OBJPATH).
- Lesson (also in notes/vc6-matching.md): a field rename must keep the declared type. Writing
  D3ANIMSLOT 0x34/0x38 as `float` instead of the old `int` grew d3_Unk432750 from 4480 to 4528 bytes
  (frame 0x70 -> 0xbc, register allocation differ); unitdiff --against caught it as WORSE, the bisect was a
  single-file cl /c /O2 /Gy + COFF section size. The comments alone never matter, the type does.

## Crash fix 2026-10-25 (hybrid, New Game)

- D3ANIMMATV's pad after ctl[5] is 0x80, as the field comments already said: 0xf4 + 5*0x198 = 0x8ec, and the
  original reads nctl as a byte at +0x96c (`mov 0x96c(%eax),%dl`) and flags at +0x96d
  (`testb $0x40,0x96d(%eax)`). The view declared pad[0xe0], so the compiled reads sat at 0x9cc/0x9cd:
  d3anim_StepObject's control loop walked slots in uninitialized heap (0xbaadf00d / 0xabababab) and crashed
  at 0x432aa5 in `FRAME_AT(data, s->frame)->count` (hybrid exe: click New Game; the same build also died at
  the menu after ~12 s). Fixed to pad8ec[0x80]; the hybrid then stays at the menu and starts a game. Both
  grades stay draft (the function is still the ROUGH layout draft above).

## match-base 2026-10-26 (base-9 worker, shard 9/10)

- Reference-only rename from ch.c: `D3ANIMFRAME.unk00` -> `time` (the key frame's start time; ch.c 0x2332
  copies it into D3ANIMKEY.time, d3anim_CreateMorphAnim uses the gap between two frames for the blend time).
  No codegen change (15/24 matched, 6910 B unchanged). d3anim_CreateMorphAnim became readable (it only failed
  Q2 on the field name).

## match-base 2026-10-26 (base-3 worker, shard 3/6)

d3_anim.c keeps its local d3_RelightObjectFrame prototype: including d3_light.h perturbs d3anim_CreatePath and d3anim_CreateMorphAnim (+2 instrs, +4 B each; tried after d3.h and after d3_anim.h). See notes/units/d3_light.md.
