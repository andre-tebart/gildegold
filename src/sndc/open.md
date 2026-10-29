# sndc.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Also about this unit
- notes/units/ecmd.md, section [sndreg]

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (guess): sound script commands (sndc_*)
- Boundaries: separated (plausible): coherent sndc_* commands, own string block 0x5f3110..
- Range then: 0x40ffc0..0x410220
- Was: unk_40ffc0.c (round 12)


## match-base 2026-10-24 (match worker)
- Shared rename (evt.h / evt.c / evtc.c / chc.c / sndc.c): EVTSCRIPT.unk104 -> group and
  evt_Unk5f8430 -> evt_NoGroup. Evidence: chc.h's CHC_GROUP(a) is the same 0x104 field of the action record
  and the macro's own comment names it group; evt.c:1855 sets the global to -1 and sndc/chc compare against
  it as the "belongs to no group" sentinel.
- Raised: sndc_KillSampleBank, sndc_PlaySampleHandle (Q6 + the rename).
- Still Q6 only: sndc_PlaySample, sndc_PlaySample3D, sndc_StopSample, sndc_GetSampleBankHandle,
  sndc_GetSampleHandle, sndc_Register. sndc_SpeechQueued also needs snd_Unk4c27d0 named (0x4c27d0, sndc.h
  call it "speech"); sndc_WaitForSample needs EVTSCRIPT.unkc60 (0xc60).

## match-base 2026-10-24 (second pass, match worker)
- Raised: sndc_PlaySample, sndc_PlaySample3D, sndc_StopSample, sndc_GetSampleBankHandle,
  sndc_GetSampleHandle, sndc_Register (Q6 descriptions), sndc_SpeechQueued (Q2 + Q6),
  sndc_WaitForSample (Q2 + Q6). sndc.c is now byte + readable in all 11 functions (unit done).
- Rename: ai_Unk4c27d0 / snd_Unk4c27d0 -> snd_QueueVoice (0x4c27d0 in f3snd.c; queues a voice sample
  (0x4c2510) with a delay, the backend of the SpeechQueued command).
- Rename: EVTSCRIPT.unkc60 -> waitfn (evt.h: the wait command the script is suspended in, 0 = running;
  evt_Step / evt_ResumeCall).
- sndc_SpeechQueued's parameters named by role (id, bank, name, var, delay), like snd_QueueVoice's.

