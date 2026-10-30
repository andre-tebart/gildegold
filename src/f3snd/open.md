# f3snd.c (was unk_4c01b0.c)

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the findings of each session, one section per author.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): the sound queue, rain / gust / animal ambience by season, the music (location / outdoor tracks, pauses, special tracks), voice samples and voice banks per building, the market ambience
- Boundaries: separated (proven): tag "f3snd: snd_AddToQueue" (mem_Alloc, 0x4c0230). .data in two parts: initialized arrays 0x602b08.. (voice volume, "Boehe_*" names, gust table 0x602ba8, animal sounds 0x602bd8, 0x602d68 music volume, track table 0x602d70) right after rt.c's "rt_load", and its literals 0x60b0dc..0x60bd7b (0x6094d0 location tracks, 0x60b0d8 also its); the next block 0x60bd88 starts with -1 globals (0x60bd94, used by 0x4c3480). Between the two parts lies data used from all over the exe (0x603800..0x608fff): open question how the linker ordered it. All drafted (layout 2026-10-22 session 8)
- Range then: 0x4c01b0..0x4c3180
- Was: unk_4c01b0.c (layout 2026-10-22)

## Open work
- **headers (2026-10-30, typing pass 2)**: head.c includes f3snd.h, data_views.h; keeps 2 cast / view macros with @original byte evidence (the address and the matched caller are in each comment): util_RandomU, sb_SetVolumeI; views / casts without byte evidence of their own (weak definitions or drafts' spellings, notes/headers.md debt item 1): sb_PlaySampleI, sb_IsVariationI, sb_GetVariationI, sb_SetRestartC, d3snd_DetachI, d3snd_StopI, d3snd_SetHoldI, d3snd_AttachI, d3snd_SetVolumeI, ai_MarketBuildingV, ai_FallbackBuildingB, ai_PlayerBuildingB, ai_CharsC.
- Near-miss (layout 2026-10-23): 0x4c2f30 (the favour comment, 417 B original): the original has a 0xbc frame (a 0xb0 byte local that VC6 drops when unused; make it used), keeps the random number as `mov bp,ax` + `and ebp,0xffff`, and re-reads the owner through a second `xor eax,eax` / `mov ax` pair; the returns are duplicated epilogues (no shared tail).
- Near-misses: 0x4c1fc0 (35 B), 0x4c30e0 (50 B); 0x4c2c70 and 0x4c23f0 now match (session 3 below).
- 0x4c1bd0 (base-2 2026-10-24): fixed - byte + readable as snd_PlayOutdoorTrack (literals, messages, the signed
  % 4; the two track stores are now the G16TRACKHANDLES view of 0x6f0b6c/0x6f0b70, see the session below).
- 0x4c23f0 / 0x4c2480 (base-2 2026-10-24, session 3): byte + readable as snd_PlaySpecialTrack /
  snd_EndSpecialTrack with snd_MusicVolume (602d68), snd_MusicEnabled (6f2578 = the game.INI [Sound] "msx"
  setting), snd_AmbientOnSaved (6f0b64), snd_SpecialTrack (6f0b74) and snd_AmbientSaved (6f0b78).
- Far drafts left: 0x4c2d20. 0x4c2510 is with match-deep (@deep: codegen only - the 4-byte frame slot and the
  G16CHAR view at 0x770b84; lead: the real AICHAR view at 0x770b80). 0x4c28a0 is byte + readable (the wrong
  voice-bank literals were the whole difference).
- .data not defined in C: 0x602b08 (1.0f), 0x602b10 (char[3][0x32] gusts), 0x602ba8, 0x602bd8, 0x602d68, 0x602d70,
  0x6094d0, 0x60b0d8.
- Type debt: f3snd.h's views of sblib / msx / d3snd / ch (notes/layout.md, session 8); the camera alias
  ai_Unkfc2dec (0xfc2dec = d3_Camera, 127 references in 6 files) and the rain level ai_Unk760618 (int in cm.h /
  sv.c, int * in f3snd.h) block Q2/Q4 of snd_UpdateRain / ai_Unk4c0e60.

## layout 2026-10-22 (session 8)
- The queue (G16SND, 0x10 bytes "f3snd: snd_AddToQueue"): sample, delay in d3_Time * 13 units, playing flag, next.
  A "Fanfare" fades the music out (0x4c24c0 = msx_FadeMaster), a following "MausclickLinks" fades it back.
- Seasons by ai_Unk4fe350(ai_Time): gusts G16SEASON 0x602ba8 (chance, interval), animal sounds G16AMB 0x602bd8 (5
  per season: birds, crickets, chickens, horses, dogs; the dogs use the birds' chance).
- Music: location tracks G16LOC 0x6094d0 (kinds, track name, priority, active), outdoor = kind 0x2694; tracks by
  kind / room / season G16TRACKS 0x602d70; outdoor names by season in 0x4c1bd0.
- 0x4c2830 returns the bank (the matched void draft and the int one give the same bytes); the original path is
  "sprache_%s\%s" (`uv run link` caught "sprache\%s\%s").
- Matched: 0x4c01b0 (no final `= 0` after the free loop), 0x4c0210 (`if (head) {..; return p;} return 0;`), 0x4c1f90,
  0x4c2480, 0x4c24c0, 0x4c24e0, 0x4c2760, 0x4c27d0, 0x4c2830, 0x4c2c00, 0x4c2ce0, 0x4c3140.

## match-base 2026-10-24 (base-2 worker, shard 2/5)
- Raised to byte + readable: snd_ClearQueue (0x4c01b0; names from the queue's role) and snd_QueueTail
  (0x4c0210, the tail of the queue).
- snd_AddToQueue (0x4c0230) -> match-deep: the only difference is the zeroing of the new record. Ours compiles
  memset(s, 0, 0x10) to four dword stores in an esi/edi frame; the original has VC6's generic inline memset
  (value and size in [ebp-8]/[ebp-4], byte broadcast mov ah,al / shl / shr / add, alignment lea / and / sub,
  rep stosb / rep stosd / rep stosb) and the ebp frame that brings. sizeof(G16SND) gives the same four stores.
- snd_UpdateQueue (0x4c02e0) -> match-deep: readable now (the G16SMP / G16SMPENTRY union view of G16SND.smp
  replaces the raw +4 reads, SBENTRY.name, F3SND_TIME); B draft is scheduling / register allocation only (75
  hunks; ours loads delay first, hoists mov esi,1, re-reads snd_Queue into another register, no second test ecx,ecx).
- snd_UpdateRain (0x4c0470) -> match-deep. Meaning fix: the gust volume is snd_Gusts[season].vol * 127.0f
  (0x602ba8 entry +4 is a float 0.5 / 0.2 / 0.6, 0x5edbe8 is 127.0f); the source had rand()*3.0518509e-05f*100.0f.
  The rest (four rain levels, formulas, Tropfen delays 1000 / 0xa28 / 3000 / 4000, stop lists, gust chance and
  k = rand()%3 / %2) corresponds. Left: scheduling / regalloc (571 hunks), Q2/Q4 camera + rain-level debt.
- ai_Unk4c0e60 (animal ambience + atmosphere) -> match-deep. Meaning fixes: the atmosphere volume is
  n = ch_Count(d3_CurrentScene, 0) * 9 for the Witshaus (0x16) and * 6 for Handwerk (0x12 / 0x14 / 8 / 0x15 / 0xe)
  and Schreibstube (7), capped with min(n, 64.0f) (0x5eb808 = 64.0f) and kept for the play / set-volume call (the
  source had ch_Count * 100.0f recomputed per call); F3SND_TIME at the two time sites. Left: scheduling / regalloc,
  the min comes out fcom + jnp where the original has fld 64.0f / fcomp st(1) / jp (likely max(64.0f, n)).
- snd_EnterBuilding (0x4c18a0) -> match-deep: the whole body read, no meaning error (the table walk, the kind /
  room / season tests, the 6-name scan, util_Random, the messages and the msx_Play tail all correspond);
  337 hunks of scheduling / register allocation.
- VC6: memset(s, 0, const) does NOT produce the generic rep-stos memset; the unrolled stores are what VC6 emits
  for the folded constant. The generic form means the original's arguments were not folded in that TU.

## match-base 2026-10-24 (base-2 worker, shard 2/5, session 2)
- snd_PlayOutdoorTrack (0x4c1bd0) byte + readable. Meaning fixes: the twelve music names use backslashes
  ("CD2\KraeuterUndPhiolen.mp3", "cd1\Burgfraeulein.mp3", "CD3\Crescendo.mp3", ...), the five messages are the exe's
  ("Outdoor-Track for GM_<SEASON> selected: %s.", "Resuming Outdoor-Track %s !"), and r = util_Random(0x75) % 4
  (signed modulo, an int r), not `& 3`.
- The two track words: 0x6f0b6c is the music track, 0x6f0b70 a write-only copy (no reader in the exe). They are now
  a G16TRACKHANDLES { current; copy; } at 0x6f0b6c. The struct also settles `uv run link`: the compiler's two stores are
  order-invariant in C (any source order gives current first), while the original stores copy (0x6f0b70) first;
  with the struct the store relocations carry +4/+0 addends, so the site inference of both stores agrees on the
  base 0x6f0b6c regardless of the order. Without it the masked byte compare still said matched but `uv run link` reported
  "matched sites disagree" for both names.
- snd_FindTrackKind (0x4c1fc0) -> match-deep: 24 code-hunk instructions, register allocation only (ours keeps
  l->kinds[0] in esi and tests it after advancing edi; the original reloads it into ecx and advances esi (l) and edi
  (a pointer it increments but never reads)).
- ai_Unk4c2000 (0x4c2000) -> match-deep after meaning fixes. t = F3SND_TIME (the original reads the volatile
  d3_Time three times; the source had d3_Time * 0xd, one read). The pause length is
  (int)((1.0f - ai_Unk602d68) * 240000.0f) (0x5eb780 = 1.0f, 0x5edc0c = 240000.0f, call 0x5de518 = __ftol), not
  util_RandomF() * 60000.0f. The five debug texts are the exe's: "Outdoortrack %s stopped because of seasonchange !",
  "Track still running after seasonchange is %s", "track %s has reached its end!", "Pause starting at %d,
  pauseLength is %d " (trailing blank), "Random decision: interrupt running track %s, propability was %d.". Left:
  register allocation / basic-block layout only (hoisted loads, the cb4==0 early return inlined vs out of line,
  inverted final branch polarity), 279 code hunks; plus Q1/Q2/Q4 readability debt.
- ai_Unk4c23f0 (0x4c23f0) byte, readable still open (see Open work). Meaning fix: snd_AmbientOn is read as a word
  here - the original is `mov eax, ds:0x6f0b60` (a1) then `and eax, 0xff`, a 4-byte load of the char flag; it is now
  the G16AMBIENTON { char on; unsigned int word; } union at 0x6f0b60, the word read only in this function. The save
  block's order is load word, amb = 0, flag = 1, saved = (unsigned char)on.
- Also readable as a side effect of the renames: snd_FindActiveTrack (0x4c1f90) and snd_FadeMusic (0x4c24c0), and
  snd_FindTrackKind (0x4c1fc0).

## match-base 2026-10-24 (base-2 worker, shard 2/5, session 3)
- byte + readable: snd_PlaySpecialTrack (0x4c23f0), snd_EndSpecialTrack (0x4c2480), snd_VoiceVariant (0x4c24e0),
  snd_PlayVoice (0x4c2760), snd_LoadVoiceBank (0x4c2830), snd_UnloadBuildingVoices (0x4c2c00) and
  snd_PlayWorkerClick (0x4c2c70, was draft behaviour: the random is an `int` - the original zero-extends the
  unsigned short return with `and eax,0xffff` - the record pointer is `unsigned short *`, and the record start is
  the G16CHARCLICK view { id +0, kind +2 } instead of `*(char *)(c + 1)`).
- snd_LoadBuildingVoices (0x4c28a0): draft -> byte + readable. The 30 code hunks were wrong literals: the exe's
  banks are ADDON\ARBEITERKOMMENTARE_BEFEHL/ KLICK_GEHILFEN.sbf for kind 0x1d (not GRABRAEUBER),
  ADDON\ARBEITER_KOMMENTARE_BEFEHL_ALCHIMIST.sbf for 0x1e (not DOMIZIL) and ARBEITER_KOMMENTARE_BEFEHL/
  KLICK_HANDWERK.sbf for the craftsmen (not HANDWERKER). Read them from .rdata at the `mov esi,<addr>` of each
  inline strcpy; `uv run link` did not flag the wrong strings (not located data symbols).
- snd_FindVoiceSample (0x4c2510, was ai_Unk4c2510) -> match-deep: the whole original read against the source, no
  missing statement / constant / callee; the 268 code hunks are codegen (frame 0x12c vs 0x128, the G16CHAR view
  at 0x770b84 where the original reads the record base 0x770b80, register allocation / block layout). The @deep
  tag carries the lead (real AICHAR view at 0x770b80).
- Side effect: snd_QueueVoice (0x4c27d0) became readable through the renames; the ai_Unk4c2000 @deep tag now
  names snd_MusicVolume / snd_SpecialTrack.
- Project-wide renames of this session: the special-track pair and its globals, the voice-player triple
  (snd_FindVoiceSample / snd_PlayVoice / snd_LoadVoiceBank), the four voice-bank handles and
  ai_Unk631090 -> ch_EmptyName (0x631090, "", already the ch.h/d2.h/hmt.h name; 135 references in 13 files).

## match-base 2026-10-24 (base-2 worker, shard 2/6, session 4)
- snd_PlaySelectedClick (0x4c2ce0) byte + readable: renamed from ai_Unk4c2ce0; ai_Unk770d08 -> ai_Selected (the
  characters' selected flag, AICHAR +0x188, one byte per 0x21c record; sv.c sets/clears it for the selection).
- snd_PlayFavourComment (0x4c2d20) -> match-deep. Meaning fixes on the way: `f += ai_Unk50c550(...)` without the
  (int) cast (the original adds the int accumulator into the float return with fiadd and truncates once per
  iteration with __ftol; the cast truncated first and lost the fiadd); the random gate compares the double 0.4
  (fcomp QWORD, not 0.4f); c is `unsigned short *` and w/m are 32-bit with the 0xffff sentinel (word loads
  zero-extend, the compares are 32 bit), and the id temp is gone (m / w are pushed into the call directly).
  Header: ai_Unk7603a0 is now `unsigned short *[0x20]`. Left: ours CSEs snd_VoiceBankOrders into edx across the
  tail where the original compares memory (cmp ds:0x6f0cbc,esi) and reloads in every play branch, plus the
  original's out-of-line early-return block at 0x4c2e2a; the whole original was read, every callee/constant
  corresponds.
- ai_Unk4c2f30 -> match-deep. Meaning fixes: out is `unsigned out[0x2e]` (0xb8 bytes) declared after x - the
  0xbc frame is x (4) + out (0xb8), not a separate 0xb0 buffer; the accesses are out[0]/out[1]. r is
  `unsigned int` (mov bp,ax + and ebp,0xffff once, then plain pushes) instead of unsigned short (masked at every
  use). x is volatile: the original stores it to its slot and reloads it before every use (matching trick,
  commented in the source). The special-kind / generic callees are one ternary so the compiler emits the
  original's single shared test tail. Left: only the pop esi position (ours right after the calls, the original
  after the level test), which shifts the esp offsets of out[1] / x by 4 in that block; the next 102
  instructions are equal (19 bytes differ).
- snd_StartMarketAmbience (0x4c30e0) and snd_StopMarketAmbience (0x4c3140) byte + readable. The start function
  was 33 code hunks with the nested `d3snd_Attach(sb_GetSample(...), o, 0x7f, 2200.0f)`: with the inner call in
  a temp (`smp = sb_GetSample(0, "Athmo_Marktplatz_Mono_4Bit"); ... d3snd_Attach(smp, o, ...)`) VC6 keeps o in
  the callee-saved esi and pushes the outer arguments after the inner call - byte. The market building's object
  is G16BLDVIEW->object (a packed view of AIBLD +0x61 in f3snd.h; f3snd cannot include ai.h).
- Header type fixes: ai_Unk6f25b8 -> `unsigned short` (am.h / cm.h / gv.h already declare it so; the player id
  zero-extends: 0x4c2d20 287 -> 44 asm lines, snd_EnterBuilding also improved 484 -> 461 bytes);
  ai_Unk7603a0 -> `unsigned short *[0x20]`; new G16BLDVIEW under #pragma pack(push,1) (without the pragma its
  object field lands at 0x64; ai.h's AIBLD is packed).
- f3snd.c's base queue is empty. Next: shp.c.
