# evtc.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (guess): script builtins, evtc_Register
- Boundaries: separated (plausible): coherent script builtins (evtc_Register), own string block 0x5f1b68.. between ch.c and chc.c
- Range then: 0x40b580..0x40b8a0
- Was: unk_40b580.c (round 12)

## Open work
- none recorded (every function matched)
- (match-base 2026-10-24) The shared rename EVTSCRIPT.unk104 -> group (evt.h) applies here too; name only,
  all 12 positions unchanged.

## match-base 2026-10-24 (second pass, match worker)
- Raised: evtc_GetTime, evtc_StopScript, evtc_FindScript, evtc_CallUserFunction, evtc_CallUserFunctionExt
  (Q6), evtc_RunScript / RunScript2 / RunScript1 (Q2: the EVTSCRIPT scene fields), evtc_Wait (Q2),
  evtc_Register (Q2: the two stub callees). evtc.c is now byte + readable (12/12, unit done).
- Renames in evt.h (+ evt.c, evtc.c): EVTSCRIPT.unkc60 -> waitfn, unkc88 -> waitStart (the d3_Time tick
  of the current Sleep), unkc8c -> ownScene, unkc90 -> scene (evt_Step selects the scene and restores it).
- Rename: ai_Unk4a8c10 / d3_Unk4a8c10 -> ai_AlwaysTrue (0x4a8c10 in unk_4a7140.c: "eval callback:
  always", returns 1; also d3e_DefaultResolve's handler and the dummy actions' exec).
- Rename: ai_Unk52a040 / sv_Unk52a040 / lp_Unk52a040 -> sv_ReturnZero (0x52a040 in sv.c: "an empty
  callback returning 0"). The address is ICF-folded; its per-site aliases ai_SeanceDecide /
  ai_SeanceExec (am.h) stay.

## match-base 2026-10-24 (base-4 worker, shard 4/5)
- Unit byte + readable now: the last foreign declaration (evt.h's `int Unk40b710()` for evtc_Wait) is gone;
  evt.c includes evtc.h and compares its waitfn against evtc_Wait. No code changed (evt.c 47/52 before and after).

