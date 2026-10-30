# ecmd.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): "ecmd_KillObject" message (object script commands, our prefix ocmd_)
- Boundaries: separated (plausible): "ecmd_KillObject" tag; own string block 0x5f2834..
- Range then: 0x40d680..0x40f960
- Was: unk_40d680.c (round 12)

## Open work
- include/ecmd.h names par_CreateEmitter's first parameter count and the third parent; it is (parent, pos,
  count, ...) (names only, ecmd.c passes the right values).
- see notes/units/d3_interface.md, Open work: "Round 9 header cleanup: unk_40d680.c's d3_Unk449de0 / d3_Unk44a170 are char d3_SetLightSet ..."
- see notes/units/d3_engine.md, Open work: "Header cleanup from round 8: d3_engine.h's d3_Unk4538a0(int) takes a char (ocmd's d3_Light ..."

## Worker ocmd (round 8): 0x40d270..0x40e560 [ocmd]

### Report
- 27 functions in 0x40d270..0x40e560 (Ghidra knew 3 of them).
- unk_40ba10.c: chc_Register 0x40d270 (41 evt_ImportCommand calls, PreloadSitMesh = lp_Unk52a040 cast to
  `int (*)()`). The character-command TU ends at 0x40d678 with it.
- unk_40d680.c / include/unk_40d680.h (object script commands; the error strings say "ecmd_<Name>", our
  prefix is ocmd_ + the script name from the registration table at 0x40f960):
  0x40d680 ocmd_GetObjectHandle, 0x40d6e0 ocmd_FindCB (ForEach callback, OCMDFIND ctx), 0x40d730
  ocmd_GetRndObjectHandle, 0x40d7b0 ocmd_GetRndSubObjectHandle, 0x40d860 ocmd_GetSubObjectHandle, 0x40d8d0
  ocmd_CreateObject, 0x40d940 ocmd_CreateObjectAtDummy, 0x40d9a0 ocmd_CreateObjectGroupAtDummy, 0x40da10
  ocmd_KillObject, 0x40da50 ocmd_SetPos, 0x40dab0 ocmd_SetAngle, 0x40db30 ocmd_MoveObject, 0x40dbb0
  ocmd_RotateObject (returns its first argument, the pointer), 0x40dc30 ocmd_MoveObjectRelative, 0x40dcb0
  ocmd_RotateObjectRelative, 0x40dd50 ocmd_SetAmbiente, 0x40dd90 ocmd_LightCalc, 0x40ddb0
  ocmd_ReplaceObject, 0x40ddf0 ocmd_LoadScene, 0x40de30 ocmd_AttachAnim, 0x40dfc0 ocmd_PreloadAnim, 0x40e0e0
  ocmd_AttachAnimLooped, 0x40e2b0 ocmd_AttachAnimCB (callback, missed by Ghidra), 0x40e430
  ocmd_AttachAnimLoopedToAll, 0x40e4c0 ocmd_DetachAnimCB (callback, missed by Ghidra), 0x40e4f0
  ocmd_DetachAnimFromAll. The file ends with the marker `/* ---- 0x40e560.. (worker ocmd2) ---- */`; the
  TU continues past the scope (0x40e560 callback, then ocmd2's handlers, registration 0x40f960).
- Argument order note for sndreg's registration: CreateObject is (float *x, float *y, float *z, char **name);
  CreateObjectAtDummy / CreateObjectGroupAtDummy are (char **name, D3OBJECT **dummy).
- Shared headers: none changed. Own header declares (with our guessed names) d3_LoadScene44be60 (char),
  d3_LoadGroup44c3f0, d3_Unk453580, d3_LightCalc4538a0(char) (d3_engine.h has d3_Unk4538a0(int)),
  d3_Unk458910, plus d3t_PlaceObject, d3if_Delete, d3if_Reload, mem_IsValid and d3if_AmbR/G/B.

### VC6 lessons
- D3ANIMFLAGS built field-wise (CHANIMFLAGS): the store/compute order of the byte fields matters even
  though they are independent. `f.u.c.b1 = *n == 1 ? 0x10 : 0;` must come before `f.u.c.b0 = *n;` (the
  original loads the int once for the compare, and the byte afterwards / reuses cl).
- `setne cl; dec cl; and ecx,8; or cl,0x10` (with `or dl,2` for b8 in between) is
  `f.u.c.b1 = x ? 0 : 8; f.b8 = 1; f.u.c.b1 |= 0x10;`. The single ternary `x ? 0x10 : 0x18` gives
  `sete; lea [ecx*8+0x10]`, and `(x ? 0 : 8) | 0x10` folds back into it.

## Worker ocmd2 (round 8): 0x40e560..0x40f960 [ocmd2]

### Report
- 41 functions in 0x40e560..0x40f960. Ghidra knew 4 of them; the other 37 handlers come from the registration at 0x40f960 plus the gap scan
  (0x40f250 SetEmitterDirection is registered between Phasespeed and Size, outside the part of the
  pseudocode I listed first; 0x40eac0 is the CreateParticle callback, not registered).
- src/game/unk_40d680.c, block `---- 0x40e560.. (worker ocmd2) ----`; include/unk_40d680.h, same marker.
  Names: `ocmd_` + script name (ocmd_SelectAllTextureSets, ocmd_CameraFlight, ocmd_CreateEmitter ...),
  plus ocmd_Unk40e560 (ForEach callback of SelectAllTextureSets) and static ocmd_ParticleCB 0x40eac0.
  sndreg's registration can reference `ocmd_<ScriptName>` for every command in my range, prototypes in the header.
- TU boundary: the block runs to 0x40f8f0 (ocmd_SetParticlePos); 0x40f960 is the registration (not mine).
  Strings: my first literal is 0x5f28a4 ("CameraFlight(): Could not find one or few dummies"), so the
  literals of ocmd's part end before it (0x5f2864 ".baf", 0x5f286c AttachAnimLooped message).
- Shared headers: none changed. The header declares (unprototyped elsewhere) par_CreateEmitter 0x457b30,
  d3t_FindObject / d3t_PlaceAtObject / d3t_MoveCameraPath / d3t_MoveObjectPath / d3t_CreateRain /
  d3t_Free413d00 (defined in d3_tools.c, no header yet), d3_YAxis 0x5ebb94 and the emitter template
  global d3_EmitterCfg 0x1468620. The .c file declares d3_Unk449de0 (SetLightSet), d3_Unk44a170 (SetFogSet)
  and `int d3_NoTextures` at 0x63345d (unaligned dword store; d3_engine.h has it as unsigned char d3_Unk63345d).
- D3EMITTER (header) is the D3PARTSYS returned by par_CreateEmitter seen through the fields the commands set
  (0x40 amplitude, 0x50 phase speed, 0x60 size, 0x6c direction, 0x78 velocity, 0x84 random velocity,
  0x90 acceleration, 0x9c plane, 0xac max trigger, 0xb8 time, 0xbc alpha[3], 0xc8 color, 0xcc/0xcd bitfields).

### VC6 lessons
- The `if (!mem_IsValid(*h)) error` handlers match only with a named local: `em = *h; mem_IsValid(em)`
  (esi holds it; otherwise the pointer is re-read from the argument).
- Three or five int args scaled into floats and stored to struct members: write the conversions into float
  locals first, then store (`fx = *x * k; fy = ...; fz = ...; em->v[0] = fx; ...`). The direct form stores
  after each conversion; with locals the stores come last and the stack order is y, z, x with `fxch`.
  For 5 values (amplitude, phase speed) the compute order a, b, c, f, d and store order a, b, c, d, f matched.
- A comma of `min(max(expr, 0), 1)` as a call argument with the expression inline (no variable) gave the
  original's `fcom` / `fld st(0); fcomp` mix: `f(.., max(min(*b * 0.01f, 1.0f), 0.0f))`; min inside max.
- Zeroing a struct then `flags.b[2] |= 3`: with `*(int *)&flags = 0; flags.b[2] |= 3; flags.b[0] = 0x7f;` VC6
  folded the OR into a constant store. Setting b[0] before the OR (`flags.b[0] = 0x7f; flags.b[2] |= 3;`) keeps
  the read-modify-write of the original.
- Bitfield assignments `x.f = v` compile to the `(v << n ^ old) & mask ^ old` sequence only for fields of a
  plain `unsigned char` struct; D3EMITTER.texmode:5 / initfill:1 / rebirthfill:1 / istrigger:1 in one byte
  and OCMDOBJBITS.blocked / transient:2 reproduce the original exactly.
- A struct template in a global (D3_ZERO_AT, then member stores) needed its stores in a particular order
  (unk00, 10/50/30 floats, d3_YAxis copy, 1.0/255.0/20/160/180, the 0xff bytes, flags |= 0x20 last).
- A local `D3TEXFLAGS`/flags struct read uninitialized from a dead parameter slot (CreateEmitter: bytes 1 and 3
  of the flags dword are leftover bytes of the first argument pointer) comes from simply not initializing it.
- A particle callback with no return statement (`static void`, cast to D3PARTCB): the original returns whatever
  is in eax.

### Outside the scope
- Nothing.

## Worker sndreg (round 8): 0x40f960..0x410770 [sndreg]

### Report
- 24 functions: 14 Ghidra functions plus the 10 sound command handlers Ghidra missed.
- src/game/unk_40d680.c (orchestrator joins with ocmd / ocmd2): block
  `---- 0x40f960.. (worker sndreg) ----` with `ocmd_Register` (0x40f960, 62 evt_ImportCommand calls, 1632 bytes).
  Handlers are declared there as `int ocmd_<ScriptName>();` (unprototyped placeholders, only their
  address is taken): CreateObject 0x40d8d0, CreateObjectAtDummy 0x40d940, CreateObjectGroupAtDummy 0x40d9a0,
  KillObject 0x40da10, SetPos 0x40da50, SetAngle 0x40dab0, MoveObject 0x40db30, RotateObject 0x40dbb0,
  MoveObjectRelative 0x40dc30, RotateObjectRelative 0x40dcb0, SetAmbiente 0x40dd50, LightCalc 0x40dd90,
  ReplaceObject 0x40ddb0, LoadScene 0x40ddf0, AttachAnim 0x40de30, PreloadAnim 0x40dfc0, AttachAnimLooped
  0x40e0e0, AttachAnimLoopedToAll 0x40e430, DetachAnimFromAll 0x40e4f0, GetObjectHandle 0x40d680,
  GetRndObjectHandle 0x40d730, GetRndSubObjectHandle 0x40d7b0, GetSubObjectHandle 0x40d860, SetNoTextures
  0x40e620, SetLightSet 0x40e640, SetFogSet 0x40e670, CameraFlight 0x40e6f0, ZoomOnObject 0x40ea00,
  CameraFlightEnhanced 0x40e750, NewCameraFlight 0x40e810, ObjectFlight 0x40e8b0, CreateRain 0x40ea90,
  DeleteRain 0x40eaa0, CreateParticle 0x40ec70, KillParticle 0x40edc0, SetCameraToDummy 0x40ee10,
  StopCameraFlight 0x40eea0, RenameObject 0x40eec0, SetBlocked 0x40eef0, SetTransient 0x40ef40, CreateEmitter
  0x40ef90, KillEmitter 0x40f0c0, SetEmitter{Amplitude 0x40f110, Phasespeed 0x40f1b0, Direction 0x40f250,
  Size 0x40f2c0, Velocity 0x40f320, Acceleration 0x40f390, RndVelocity 0x40f400, Plane 0x40f470,
  TimeAndAlpha 0x40f4f0, Color 0x40f560, Flags 0x40f5d0, InitFill 0x40f690, RebirthFill 0x40f6f0,
  TextureMode 0x40f750, IsTrigger 0x40f7a0, TriggerOnce 0x40f800, MaxTrigger 0x40f850}, TriggerEmitter
  0x40f8a0, SetParticlePos 0x40f8f0, SelectAllTextureSets 0x40e5a0. Naming scheme: `ocmd_` + script command name
  (the list is also the call order in ocmd_Register). The orchestrator must make the real definitions use
  these names (or edit the registration) when joining; the file's own header comment and the includes are
  placeholders too.
- src/game/unk_40ffc0.c + include/unk_40ffc0.h (new TU, sound commands): sndc_LoadSampleBank 0x40ffc0 ..
  sndc_WaitForSample 0x4101d0 (10 handlers, none known to Ghidra), sndc_Register 0x410220 (ends the TU, with
  the script constants snd_W2 .. snd_VAR = -1..-8 at 0x5f3108, defined with initial values in the file).
  The header declares prototypes of the sample library (unk_425bc0.c has no header: sb_GetSample, sb_Play,
  ...) with opaque struct tags, snd_Unk4c27d0 (0x4c27d0, SpeechQueued backend, 5 int-ish args), and
  d3snd_PlayAt.
- src/game/unk_4103a0.c + include/unk_4103a0.h (new TU, probably the start of the input/system TU that continues
  past 0x410770): registry helpers (reg_OpenKey 0x4103a0 for "Software\Ahead Entertainment\%s" under HKCU,
  mode 1 read / 2 create), keyboard scan code map (kb_Map 0x1467dc0, kb_BuildMap, kb_FindKey, kb_Base 0x5f3234
  initial 0x200), DirectInput clear/acquire helpers (di_ClearButtons 0x410620 / di_ClearAxes 0x410690 clear
  three int[8] blocks at 0x1467390, 0x14683d0, 0x14673f0, di_Error 0x4106d0 = two wsprintf, di_AcquireKeyboard
  0x410710 on the device at 0x1467360, di_AcquireMouse 0x410740 on 0x14673cc). The TU continues past the scope end (0x410770).
- No shared header changed. Gaps checked: 0x40f960..0x410770 is contiguous (registration ends at 0x40ffb8,
  handlers follow, no further missed functions).

### VC6 lessons
- Shrink-wrapped callee-saved pushes (pushes after the `if (guard) return 0;` test) come with
  `if (guard) return 0; s = f(); if (s) {...} else g(); return s;`; returning s from inside the success branch
  (`return s;` in the `if`) pushes at entry and replaces the failure-path return value with a constant 0
  (`xor eax,eax`), because VC6 knows s == 0 there.
- `mov al,imm; ret` for returns means a `char`/`unsigned char` return type (kb_FindKey).
- A `switch (mode)` with two cases compiles to `dec eax; je; dec eax; jne` (reg_OpenKey), and the later
  `if (r == 0) return key; return -1;` gives the original layout.
- Ghidra's `(BYTE)(a ^ b) ^ b` is `((a ^ b) & 0xff) ^ b` on unsigned ints (replaces the low byte of b by a).
- `if (on) {Acquire} else {Unacquire}` order in COM vtable calls decides which side is the fall-through.
- `p = buf + wsprintf(buf, ...)` (rather than an int temp) kept the second wsprintf pointer in eax.

## match-base 2026-10-24 (match worker, reference update from d3_anim.c)

- ocmd_StopCameraFlight calls d3anim_FreePath now (d3_Unk433b10 rename); ecmd.c stays 68/68 byte.

## match-base 2026-10-24 (base-0 worker, shard 0/5, reference update)

ocmd_DeleteRain calls d3t_DeleteRain now (was d3t_Free413d00) and ecmd.h's two d3_tools prototypes use
struct D3TRAIN *; ecmd.c stays 68/68 byte.

## match-base 2026-10-25 (base-5 worker, shard 5/6, 2nd)

The unit's base queue is empty: 61 items -> 60 raised, 1 escalated. ecmd.c stays 68/68 byte (0 differing
bytes); readable 67 of 68.
- Q6: a description after every address from 0x40d730 to 0x40f8f0 (the object, anim, camera, particle and
  emitter commands).
- Q2 names (all project-wide where the name exists elsewhere): d3_LoadAnimStream replaces d3_Unk44e4a0 in the
  four AttachAnim commands (a file-scope prototype above the first use, since d3.h's @original void alias must
  keep its own name to avoid a conflicting-declaration TU with d3_io_t.h); d3if_SelectTextureSet /
  d3_SetLightSet / d3_SetFogSet replace d3_Unk446f30 / d3_Unk449de0 / d3_Unk44a170 (d3_interface.c's canonical
  names); ocmd_SelectTextureSetCB replaces ocmd_Unk40e560; OCMDTEXSEL.unk40 / unkc0 / unkc4 -> file / n / rnd
  (same layout and the same "empty the file, n = rnd = 0" initialisation as OCMDANIM, which the same worker
  wrote for the other ForEach commands); d3_LoadScene44be60 -> d3_LoadScene (an address-suffixed name) in
  d3_int_io.c/.h, cm.c/cm.h and ecmd.c/.h.
- Q4: flags.all = 0 (was *(int *)&flags); D3EMITTERCFG.dir is a D3VECTOR field now (was float[3]), so the
  d3_YAxis copy needs no cast. Q7: par_Delete prototype added to ecmd.h.
- Escalated: ocmd_SetCameraToDummy (0x40ee10, Q2 only) - unk05c / unk090 are a camera dummy's position /
  angles here but D3LOBJ.color and D3LOBJ.range/power/atten in the light code; the D3OBJECT union is the
  d3_CloneObject codegen hazard, a shared-header decision (reason in the @deep tag).
- Trap: a call that comes before the file-scope prototype makes VC6 assume `int f()` and the later prototype
  is C2040 ("differs in levels of indirection" with two identical-looking signatures); the prototypes go
  above the first use.
- Side effect: the ecmd.h / cm.h edits recompiled am.c, cm.c, f3gm.c, f3init.c, sv.c, unk_575860.c and
  `uv run ledger check` then recorded 8 functions draft -> byte there (ai_Unk48f990, ai_Unk4c6ec0,
  ai_Unk4c8de0, gm_Unk50c4e0, unk_Unk5112d0, unk_Unk511c70, sv_Unk528290, ai_Unk577230): their objects in
  the seeded build/ were stale, the sources already match. Not source edits of this session.
Open work: the unit is not readable until ecmd.h is canonical (`uv run ledger grade` ecmd.c lists the foreign declarations
of ecmd functions in d2.h / cm.h / rt.h / shp.h and the K&R prototypes at the end of ecmd.c, 0x40f960 area).

## match-base 2026-10-26 (base-3 worker, shard 3/6)

ecmd.c includes d3_light.h; ecmd.h lost its d3_RelightObjects / d3_LightInvalidate declarations (d3_light.c Q7). No byte change (68/68 matched).
