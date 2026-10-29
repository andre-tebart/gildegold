# msx.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (leaked): D:\develop\units\Msx\msx.c
- Boundaries: separated (plausible): leaked; start next to mov.c (plausible)
- Range then: 0x423770..0x4244b0

## Open work
- match 2026-10-23 (8th), no gain: msx_Update IV base. Both asm versions have the same multiset of 54 IV references (offsets 0x100:13 .. 0x124:15), so the base is not the median of the final references; toy loops (build/toy, not kept) show the base is chosen from the ORDER of the references in the loop (IV candidates are the loads / stores after CSE; dead loads do not count): for a loop of distinct fields b c d e ... it is always the 2nd referenced field (offset 0 never counts as a candidate), with repeats it follows a count / recency rule not fully worked out (bbbc -> b, bbcc -> c, bccb -> b, bcdb -> b, bcdc -> c, bcdd -> d, bcded -> c). Dead refs, extra distinct fields (`dur = t->length`, `t->queued`, `t->name[0]`) do not change it.
- match 2026-10-23 (5th): msx_Update: 582 variants of operand orders (`fadepos > pos`, `!dur`, product orders, `playing == 0`) in the fade formulas: the IV base stays &t->fadepos (the original's whole 230-instruction difference is that base: every field offset is shifted by 8).
- match 2026-10-23 (4th): msx_Update loop forms (pointer for-loops `t < msx_Tracks + 10` / `!=` / `&msx_Tracks[0]`, `t++` beside i, do-while, count-down): no change or worse (428 B at best). Note the original's IV start is the constant 0x6326e8 = &msx_Tracks[0].volume exactly.
- match 2026-10-23 (3rd): msx_Update IV base: operand-order family (64 variants: `fadepos > pos`, `pos - fadepos >= dur`, `abs(curvol - volume)`, `volume * (...)` in both curvol formulas, `dur + fadepos - pos`, `fadepos + 0`): base unchanged (best 419 B / 110 lines from `abs(t->curvol - t->volume)`, not kept: more lines).
- match 2026-10-23 (2nd): msx_Update IV base (original &t->volume, ours &t->fadepos), no gain: `vp = &t->volume` local pointer (worse: 178 lines), `(void)&t->volume`, a cast-built `t` from &msx_Tracks[i].volume - 0x118, volume read first in both fade blocks (dur before the pos clamp), dead `vol = t->volume` stores at 6 positions of the loop head. Field reference counts do not explain the choice (curvol 10, next 22 are not the base).
- match 2026-10-23: msx_Update: `msx_Tracks[i].field` everywhere (no pointer local), `t = msx_Tracks + i`, a mix of both: unchanged (428 B). The original base is &t->volume (esi-0x14 = playing, esi-0x18 = stream, esi+0xc = next), ours &t->fadepos.
- match 2026-10-22: msx_Update (base register of the track loop is `&t->volume` in the original, `&t->fadepos` in ours): an `el = pos - fadepos`
  local in one or both fade blocks makes it far worse (626 B).
- see notes/units/dd_vesa.md, Open work: "Header cleanup: Dd_vesa.c globals/functions are declared both in d3_d3d.h (with D3DCARD) a ..."

## match-base 2026-10-24 (match worker)

- msx_Init, msx_Exit, msx_SetVolume, msx_SetAmbVolume, msx_Start: Q6 descriptions added, all byte + readable.
- Next in the unit: msx_Stop (Q6), then the msx_Update block (behaviour: 428 B, the &t->volume base register,
  see Open work).

## Also about this unit
- notes/units/net.md, section [net]

## match-base 2026-10-24 (match worker, 5th)

- msx_Stop, msx_SetFadeTime, msx_FadeMaster, msx_Find, msx_GetPlaying, msx_Alloc, msx_SetFade, msx_SetPath,
  msx_PlayAmbient, msx_StopAmbient: Q6 descriptions added, all byte + readable (the unit is 18/19 byte).
- msx_Update: Q6 description added, then escalated - the only difference is the track-loop IV base (ours
  &t->fadepos, original &t->volume, every field offset +8), the same operations spelled differently; every
  Open work attempt had failed.
- The base queue of the unit is empty.
