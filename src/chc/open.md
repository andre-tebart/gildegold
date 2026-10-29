# chc.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Worker evtc (round 7): 0x40b580..0x40d270 [evtc]

### Report
- unk_40b580.c (script builtins): evtc_* 0x40b580..0x40b860 plus evtc_Register 0x40b8a0 (was "FUN_0040b8a0", registers rnd, GetTime,
  RunScript*, Sleep, ... via evt_ImportCommand 0x42b5b0). The TU ends at 0x40ba08 with it.
- unk_40ba10.c (character script commands): 44 functions 0x40ba10..0x40d220, named chc_ + the script
  command name from the registration table at 0x40d270, plus three action callbacks Ghidra missed
  (chc_AniSoundCB 0x40bf60, chc_AniScriptCB 0x40c090, chc_AniScriptIntCB 0x40c1d0).
- TU boundaries: each script-command file ends with its own registration function: 0x40b580..0x40ba08
  (evtc_Register last), 0x40ba10..0x40d270+ (registration 0x40d270 is outside the scope, not written; it
  belongs at the end of unk_40ba10.c). The two TUs share no string literals; 0x40d150 reuses
  "PlayCharacterAni(): invalid character or dummy" from 0x40bec0 (one literal, so same TU).
- Shared headers: none changed. Own headers include/unk_40ba10.h (also declares mem_IsValid, the
  CHC_* CHACTION offsets, CHCALPHA and extern ch_StepSample 0x5f1230, the "normal_s" array defined by
  unk_402140.c data); include/unk_40b580.h got evtc_Register plus prototypes of d3_Unk4a8c10 and
  lp_Unk52a040 (the latter unprototyped, it is registered as the Print/PrintInt/PrintFloat/PreloadSitMesh
  no-op).

### VC6 lessons
- Recompute parameter offsets after every push: chc_CreateCharacterAtDummy has a third parameter (the
  name copied into ch->name / obj->name); taking it for the first one put the pointer in edi for the whole
  function instead of only around the strcpys.
- `if (x & 0x10) return 1; return 0;` compiles to `xor edx,edx; mov dl,[m]; mov eax,edx; shr eax,4; and eax,1`.
  Every expression form (`(x >> 4) & 1`, bitfields of any width, `!= 0`, ternaries, volatile, inline
  helpers) gives the narrowed `mov al,[m]; shr eax,4; and eax,1` instead.
- `memset(&f, 0, sizeof f); f.bit = 0;` keeps the read-modify-write of the bitfield byte; `*(int *)&f = 0`
  lets VC6 fold it into a byte store of 0.
- `if (!a) error(); else b->x = y; return b;` returns the pointer from esi on both paths (`mov eax,esi`);
  an early `return b;` in the error branch becomes `xor eax,eax` and drops the esi save.
- An if/else-if strcmp chain with one `return 0` at the end defers the callee-saved pushes past the
  argument check; separate `if (...) { ...; return 0; }` blocks push them at entry.
- `if (a = list) { ...; return 0; } return 0;` keeps `push esi` at entry; with `a = list; if (a) {...}` VC6
  shrink-wraps it into the branch.
- A loop counter zeroed before a call (`xor esi,esi` ahead of the call) is `i = 0;` written before the call
  with `for (; i < n; i++)`.
- `angle = f(...); g((int)(angle * k))` defers all argument pops to one `add esp`; `g((int)(f(...) * k))`
  pops f's arguments right after the call.
- Uninitialized local read from a parameter's stack slot again (chc_AttachObject: `hand` when no name
  matches, read as `[esp+0x10]` = the first parameter).

### Outside the scope
- 0x40d270 (registration of the character commands) has no source yet; it calls evt_ImportCommand 41 times,
  with lp_Unk52a040 (0x52a040) as the PreloadSitMesh handler.
- chc_AttachObject passes its second argument straight to strcmp (no dereference), as in the original.

## Also about this unit
- notes/units/ecmd.md, section [ocmd]

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (guess): character script commands (chc_*)
- Boundaries: separated (plausible): coherent chc_* script commands, own string block 0x5f1df0..
- Range then: 0x40ba10..0x40d270
- Was: unk_40ba10.c (round 12)

- (match-base 2026-10-24) EVTSCRIPT.unk104 -> group / evt_Unk5f8430 -> evt_NoGroup (the CHC_GROUP macro was
  the evidence); name only, all 44 positions unchanged.

## match-base 2026-10-24 (base-1 worker, shard 1/5)

Unit chc.c: 43 of 44 functions byte + readable (all were byte, none readable). Raised with Q6
descriptions from the read bodies and the Q4 macros replaced by typed fields:
- ch.h views: CHARACTER.unk02c -> building, unk030 -> group, unk124 -> transport, flags.unk9 ->
  lowPoly (CHC setter), the flags word/byte view (CH_FLAGB uses -> ch->flags.bytes.lo), the 0x128
  union CHANIM *anim / CHACTION *actions (CH_ACTIONS), the CHACTION 0x30 union with scriptName at
  0x90 (CHC_SCRIPT - NOT u.name at 0xf0) and the scriptAction view group/arg/flags at 0x17c /
  0x180 / 0x18c (CHC_GROUP / CHC_ARG / CHC_UNK18C); CH_UNK02C removed.
- Project-wide: CHARACTER.unk030 -> group, unk02c -> building, unk124 -> transport,
  flags.unk9 -> lowPoly, D3OBJECT.unk084 -> rotation, d3_Unk448ac0 -> d3_YawTo,
  d3_Unk453d60 -> d3_UnwrapAngles (its d3_vec.c definition name).
- chc_TakeObjectScript escalated: Q4 D3MATERIAL_NAME104 stays - a typed union (ops / name) at
  D3MATERIAL 0x104 in d3.h flips d3_Unk4550e0 (d3_mir.c) by 8 bytes at +0x1be; build REGRESSED,
  reverted (the known type debt, notes/vc6-matching.md).
- The rotation rename raised d3anim_ApplyRootMotion (d3_anim.c) to byte + readable; its stale @deep
  tag was cleared.
- Unit readability stays draft only because of the escalated function (0 foreign declarations).
