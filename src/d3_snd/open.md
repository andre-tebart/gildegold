Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (leaked): named from a source path before round 12 (not in tu_anchors.csv)
- Boundaries: separated (plausible): leaked; its literals end at 0x5f39bc, dd_vesa.c (with the former unk_418ce0.c) follows
- Range then: 0x417650..0x418ce0

## Open work
- d3sndw_Update 0x4183d0: 1622 B / 152 asm lines (was 1748 / 169, match 2026-10-23 (4th)). What helped: `sc = d3_DefaultScene;` as a local before the scene test (the original loads d3_DefaultScene once before the compare; the old form loaded it again on the else path). Left: callee-saved register assignment. The original keeps the zero in ebx (ours ebp), `ground` in edi (ours esi), and has three zero registers later (ebx, esi, ebp: `xor ebp,ebp / xor esi,esi`) where ours has ebp / edi; the FPU hunks at 0x418515 and several `cmp reg,0` vs `test` / `cmp mem,0` hunks follow from that. 29 hunks at the start of the session (diffclass). Tried without change: 150 random orders of the `int i, j, n, idx, type, cnt;` declaration, other spellings of the scene test (nested, `&&`, swapped compare: worse). Also no change: permutations of the declaration block (720) and of the 8 initial zero stores (800 sampled). The original splits the constant 0 into three webs (ebx: found[0].type and the early tests, esi: five found[] stores + dir.y, ebp: nfound). Next: c2prio per candidate (the order of the zero constant vs `ground`), then the loop bodies.

## match-base 2026-10-24 (match worker)

- Raised to byte + readable: d3snd_Exit, d3snd_Attach, d3snd_Detach, d3snd_Alloc, d3snd_Update,
  d3snd_DetachAll, d3snd_UpdateVolume, d3snd_Play, d3snd_Stop, d3snd_SetHold, d3snd_SetVolume,
  d3snd_IsPlaying, d3snd_PlayAt, d3sndw_Exit; d3sndw_GetSound followed from the callee renames.
- d3sndw_Update escalated (B: 143 code / 52 order / 36 regs / 14 fpu hunks, the same operations under the
  original's callee-saved register assignment - ebx zero / edi ground vs our ebp / esi; the rounds above read
  the whole diff; register allocation is match-deep's).
- SNDHANDLE is a 0x30-byte view of sblib.c's SBSAMPLE (loops 0x0c, delay 0x18, volume 0x24). Named from that
  overlay and the sb_* callees in names.csv: snd_Unk426170 -> sb_SetRestart, snd_Unk426030 -> sb_GetSample,
  snd_Unk427390 -> sb_SetLoops, snd_Unk427010 -> sb_Resume; D3SOUND.unk44 -> loops, unk4c -> delay,
  SNDHANDLE.unk0c/unk18/unk24 -> loops/delay/volume.
- Type debt: d3_snd.c declares sb_GetSample with sndc.h's SBSAMPLE* view (f3snd.h declares it as int). One
  lint_baseline line added (`sb_GetSample src/game/d3_snd.c`): including sndc.h in the TU is blocked by the
  in-TU conflict on d3snd_PlayAt (SNDHANDLE* here, void* in sndc.h).
- The same `uv run build` found sb_GetSample / sb_Resume / sb_SetLoops (sblib.c) readable: the lint caches
  were stale before (Q7 calls), refreshed by uv run lint this session.
