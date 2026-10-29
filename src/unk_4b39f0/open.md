# unk_4b39f0.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the findings of each session, one section per author.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (-): missions ("Auftraege"): the running missions, mission types, own LCG random 0x75e120, the mission dialogs, the 49 mission conditions and their dispatcher 0x4b4f00, the result dialogs, save / load 0x4b62e0 / 0x4b63b0
- Boundaries: separated (plausible): missions, all drafted (layout 2026-10-22 sessions 6-7). Start: .data block 0x601f88 (48 mission types of 0x1c bytes) right after hst.c's literals, then "_AUFTRAEGE_VERGABE_HS_%.2d", "$N", "special\mission", "%ia[%s]$N", "special\CHOOSE_MISSION", "%it[%s]", "Menu\CHOOSEHISTORY", "_AUFTRAEGE_ERFOLG_HS_%.2d", "AUFTRAEGE_ALLGEMEIN", "$C" 0x602568; .bss 0xf82320 (running missions) right before hst's 0xf83520. End: save / load 0x4b62e0 / 0x4b63b0 close it; the next code (0x4b6480..) is a table-driven message module whose only .data is its table 0x602570 right after "$C" (consistent with a new obj, no proof). No module tag (name guess only)
- Range then: 0x4b39f0..0x4b6480

## Open work
- Unit complete (0x4b39f0..0x4b6480, layout 2026-10-22 session 7); name a guess (no module tag).
- Near-misses: 0x4b3b70 (the &t[47] bound in a register),
  0x4b3ca0 21 B, 0x4b4040 (view / first register pair swapped), 0x4b6040 (c / view swapped), 0x4b58b0 (register
  choice, `push ebx` first).
- Drafts far off: 0x4b4760, 0x4b4af0 (320 vs 352: the counts are re-read per call in the original), 0x4b5460 (same
  size; G13_PLAY / G13_WAIT macros), 0x4b5970 (same size, 377 B), 0x4b5f10, 0x4b6110.
- Type debt: see notes/layout.md (session 6): local d2 / net / sb views, ai_Unk4c4db0 / ai_Unk515960 int views,
  G13_501190, G13_4FE1E0; G13_B reads AICHAR / AIBLD bytes; AIFIN fields 0x2c..0x3c read by offset.

## layout 2026-10-22 (session 6)
- Data: G13MIS ai_Unkf82320[0x80] (type 0 = free, owner = a character's unk004, AITIME when given, a counter at 0x1c
  for the event missions 11 / 19 / 23 / 28 / 40), G13TYPE ai_Unk601f88[48] (type, kind 0..5, text id, speech nn,
  goal values 0x14 / 0x18), ai_Unk624580 the chosen kind (char, -1 none, 5 = one mission only, slot 0).
- 0x4b4f00 dispatches the 49 mission types to the condition callbacks 0x4b4550..0x4b4eb0 (switch in the original
  order of the case bodies = their addresses); callbacks take (AICHAR *c, G13TYPE *t) or (c, G13MIS *m, t), the
  latter time the condition with 0x4b5160.
- Loop counters: `for (i = 0, n = 0; ..)` zeroes i first (0x4b3c40, 0x4b4670 matched so); `b = next(), n++` in the
  for increment puts the count after the call (0x4b51f0 / 0x4b5270).
- A call written as an argument of another call shares its pushed arguments: 0x4b3c10 is
  `ai_Unk4b3ca0(owner, ai_Unk4b3c40(kind), x)` (x pushed before the inner call, left on the stack for the outer).

## layout 2026-10-22 (session 7)
- 0x4b3a10 matches with `% 0x7fffu` (the unsigned modulo gives `xor edx; div`; `% 0x7fff` on the promoted
  unsigned short is a signed `cdq; idiv`).
- The unit ends with save / load 0x4b62e0 / 0x4b63b0; 0x4b6480.. is the message module (unk_4b6480.c).

## match-base 2026-10-25 (base-4 worker)
- Reference-only renames (no code change; every use updated, build/link/ledger clean): ai_Unk4c3ae0 -> cm_GetSlotState (0x4c3ae0) and ai_Unk4767d0 / ai_Unk476860 -> d2_StartFade / d2_ReleaseFade (0x4767d0 / 0x476860, the old ai_ aliases folded into the d2 names). See notes/units/unk_5d33b0.md for the whole rename list.

## match-base 2026-10-25 (base-2 worker, shard 2/6)

Readability-led session: six functions of the unit became byte + readable through names alone (no bytes
changed, check 2567/4664 unchanged, unitdiff 48/59 and 1742 differing bytes before and after).
- byte + readable: ai_ClearMissions (0x4b39f0), ai_MissionRandom (0x4b3a10), ai_FindOwnerMission
  (0x4b3a50), ai_GetMissionType (0x4b3ae0), ai_HasMission (0x4b3b10), ai_CountMissionTypes (0x4b3b40).
- Renames (evidence = the functions' own comments; every use updated in src/ and include/):
  ai_Unkf82320 -> ai_Missions (the 0x80 running missions), ai_Unk75e120 -> ai_MissionSeed (the LCG seed,
  timeGetTime at start), ai_Unk624580 -> ai_MissionMode (the chosen mission kind, 5 / -1 / -2),
  ai_Unk4b39f0 -> ai_ClearMissions, ai_Unk4b3a10 -> ai_MissionRandom, ai_Unk4b3a50 -> ai_FindOwnerMission,
  ai_Unk4b3a90 -> ai_RestartMission, ai_Unk4b3ae0 -> ai_GetMissionType, ai_Unk4b3b10 -> ai_HasMission,
  ai_Unk4b3b40 -> ai_CountMissionTypes, ai_Unk4b3b70 -> ai_FindMissionType, ai_Unk601f88 -> ai_MissionTypes.
  Touched units: cm.c / cm.h, f3init.c, main.c, f3gm.c, unk_58bbd0.c (reference renames only).
- ai_FindMissionType (0x4b3b70) -> match-deep: 6 differing instructions, the original keeps the loop end
  &ai_MissionTypes[47] in ecx, ours compares the immediate twice (leads tried in its @deep tag).
- ai_NewMission (0x4b3ca0, was ai_Unk4b3ca0) is BYTE now (check 2567 -> 2568): the mode-5 block and the
  free-slot loop both store `type` before `owner` in the original (the loop draft had owner first) - the
  store order decides the argument loads (type in eax first), 10 -> 4 -> 0 differing instructions. The
  unit note's 2026-10-22 lesson "0x4b3c10 is ai_Unk4b3ca0(owner, ai_Unk4b3c40(kind), x)" still holds.
  Escalated to match-deep on Q2 only: G13MIS.unk20 (+0x20) is zeroed at creation / restart and saved / read
  back by 0x4b62e0 / 0x4b63b0, never read otherwise - no evidence for a name.
- Still blocked readability here: unk18 / unk1c / unk20 of G13MIS and ai_Unk4b3a90's TMX macro (Q4); the
  rest of the unit's drafts need names with evidence (ai_Unk4b3bd0, ai_Unk4b3c10, ai_Unk4b3c40,
  ai_Unk4b3ca0, ai_Unk4b3d50, ai_Unk4b3e70, ai_Unk515960, ai_Unk506430 / ai_Unk5085b0 / ai_Unk504ce0).
  ai_Unk4b3ca0 (0x4b3ca0) stays in the base queue: 28 code hunks and the unk20 Q2 gap.

## match-base 2026-10-25 (base-2 worker, shard 2/6, 7th)

Check 2569 -> 2570; readable 1538 -> 1542; unit 49/59 -> 50/59 matched, 1721 -> 1486 differing bytes.
- Project-wide renames (every use updated in src/ and include/): ai_Unk515960 / unk_Unk515960 -> io_PrintText
  (the text engine 0x515960, 865 refs; unk_5d33b0.c's redundant K&R io_PrintText declaration dropped - the
  prototype above it covers the TU), ai_Unk4b3e70 -> ai_ChooseMissionType, ai_Unk4b4040 -> ai_ChooseMission,
  ai_Unk4b4760 -> ai_HasThreeStockedBuildings, ai_Unk503fa0 / gm_Unk503fa0 -> gm_BuildingTypeOfJob (f3gm.c),
  ai_Unk4b4af0 -> ai_PlayerHeldBuildings, ai_Unk4b5270 -> ai_PlayerShare, ai_Unk4b5460 ->
  ai_MissionSuccessDialog, ai_Unk4273c0 -> sb_IsInit (sblib.c's canonical name; unk_575860.c's K&R alias is
  a prototype now), G13STAT.unk00 -> same, G13TYPE.unk14 -> goal, G13TYPE.unk10 -> speech.
- ai_ChooseMission (0x4b4040) -> match-deep: Q1/Q2 cleared; behaviour is a pure register-colour swap (view:
  ours edi / original esi, the first/o5/g colour the other way; asmdiff --regs esi=edi,edi=esi leaves only
  the prologue/epilogue push-pop order). c2prio: _first prio 182 -> esi, _view prio 181 -> edi, a 1-point
  tie-break; declaration order tried, no effect. Reason in the @deep tag.
- ai_HasThreeStockedBuildings (0x4b4760) -> match-deep: the real difference was the loop condition
  `for (b = gm_BuildQuery(...); n < 3 && b != 0; b = gm_BuildNext())` with `n += ok` at the body end (44 ->
  5 differing asm lines, 99/208 bytes; lesson in notes/vc6-matching.md). One code hunk left: AIBTYPE.slot's
  +0x23 folds into our lea, the original materialises the record address and adds it in a second lea; six
  spellings tried (typed member, &slot[0], char* record local, AIBTYPE* local, two-step w), all fold.
- ai_PlayerHeldBuildings (0x4b4af0) BYTE: the condition sums G13STAT.same (the holders that match), not n
  (the total): `n = s.same; n += s.same;` per call and `return s.same + n >= goal` (352/352 bytes,
  instructions identical). G13STAT.unk00 -> same and G13TYPE.unk14 -> goal came with it.
- ai_MissionSuccessDialog (0x4b5460) -> match-deep: Q1/Q2/Q4 cleared. The four code hunks are the G13_WAIT
  end time: ours `mov ebx,[d3_Time]; add ebx,0xfa`, the original `mov eax,[d3_Time]; lea ebx,[eax+0xfa]`
  (a temp, then lea). `0xfa + d3_Time` is identical; the inline `while (d3_Time + 0xfa > d3_Time)` without
  _end is much WORSE (578 -> 606 bytes, 20 -> 222 asm lines, reverted).
- In progress at TIME UP, still in the base queue: ai_Unk4b58b0 (0x4b58b0, 87/160 bytes, 11 asm lines): the
  original keeps the literal 1 in ebx across the dialog (`mov ebx,1; push ebx` for d2_SetForm(view, 1) and
  `mov [sv_PopRequest],ebx`), ours uses immediates. An `ok = 1;` local tried: VC6 folds it (same bytes as
  the immediates, reverted) - the constant web's register is the lead (c2prio: const#0 not materialised).
- Open: G13TYPE.unk18 (12 t->unk18 uses, a per-item threshold in the value conditions or minutes in the
  time ones) is the next field-name candidate; G13MIS.unk18 / unk1c / unk20 stay blocked (no evidence).

## match-base 2026-10-26 (base-2 worker, shard 2/10)

Check 2573/4664 before and after (no byte change: the session is readability + one escalation); unit 50/59
matched, 1486 differing bytes (unchanged). Readable 10 -> 15 in the unit.
- Escalated: ai_MissionEndDialog (0x4b58b0, was ai_Unk4b58b0) -> match-deep. The one open item is still the
  constant web: the original keeps the literal 1 in ebx (`mov ebx,1; push ebx` before d2_SetForm(view, 1),
  `mov [sv_PopRequest],ebx` in the loop, `push ebx`/`pop ebx` in the prologue), ours emits `push 1` /
  `mov [sv_PopRequest],1`. Meaning read: same `if (di_MouseEvent != 0 || d2_ClickedId == 0) sv_PopRequest = 1;`
  (one shared store). Spellings tried, one edit + one build each, reverted with the bytes unchanged:
  `ok = 1; d2_SetForm(view, ok); ... sv_PopRequest = ok`, `ok = 1; d2_SetForm(view, 1); ... sv_PopRequest = ok`,
  `for (ok = 1; sv_RunFrame(...); ) { ... sv_PopRequest = ok; }` (plus the 2026-10-25 `ok = 1` local) - VC6
  folds every one, no ebx at all. The only matched sibling with this idiom is ai_Unk4b3d50, where
  `mov ebx,1` comes from `r = 1`, a variable with two definitions that is returned (see notes/vc6-matching.md).
- Names (evidence = each function's own address comment; every use updated in src/ and include/):
  0x4b3c40 ai_Unk4b3c40 -> ai_RandomMissionType, 0x4b3c10 ai_Unk4b3c10 -> ai_NewRandomMission (both byte +
  readable now), 0x4b48f0 -> ai_CondCharShare, 0x4b4d50 -> ai_CondPlayerShare, 0x4b4280 ->
  ai_ChooseMissionFrom, 0x4b5310 -> ai_ShowMissions, 0x4b5ef0 -> ai_MissionCompleteByMode (byte + readable
  now), 0x4b5360 -> ai_MissionInfoDialog, 0x4b5970 -> ai_MissionCompleteDialog, 0x4b5c20 ->
  ai_MissionNextDialog, 0x4b5f10 -> ai_MissionResultDialog, 0x4b60f0 -> ai_MissionResultByMode (Q1 cleared;
  Q2 and behaviour still open).
- Q2 blockers left in the unit: G13TYPE.unk0c (0x4b3d50, 0x4b5360), G13TYPE.unk18, G13MIS.unk18 / unk1c /
  unk20 - unchanged, still no evidence. The foreign placeholders ai_Unk5085b0 / ai_Unk504ce0 / ai_Unk509ee0 /
  ai_Unk4f9d40 / ai_Unk4f9e00 / ai_Unk4b5160 / ai_Unk4b51f0 / ai_Unk4b4550..4b4eb0 (the 49 condition
  callbacks, Q4 G13_501190 / G13_4FE1E0) name the rest of the unit's Q2 failures.
- The two remaining base items of the unit are the far drafts ai_MissionCompleteDialog (0x4b5970, 157 asm
  lines, 195 code hunks) and ai_MissionResultDialog (0x4b5f10, 20 code hunks): both are dominated by the
  c/t (ebx/ebp) register colour swap and the byte-form `or bl,0x80`; `char flags` tried on 4b5f10 and was
  WORSE (it turned the `| 0x100` into `or bh,0x1` too, reverted).
- Also renamed (evidence: message 8 of 0x4c4db0 sets it to a character's netId, message 10 clears it, and
  0x4b62a0 / 0x4b6040 branch on it as the other player): ai_Unk6248a0 -> ai_MissionPartner (0x6248a0, -1
  none). Include/cm.h and cm.c/unk_4b39f0.c references updated, build and lint clean.
- 0x4b5950 ai_QuitGame (was ai_Unk4b5950) BYTE + READABLE: its only placeholder callee was the old alias
  ai_Unk52fb50 of 0x52fb50, which is sv_PopTo (sv.c) - the alias is folded into sv_PopTo in every caller
  (include/cm.h, cm.c, he.c, main.c, sv.c, unk_4b39f0.c; the local `void sv_PopTo(int v);` declarations stay,
  same signature, lint clean, no unit worse). The function sets ai_GameOver and pops to main_GameLoop.
- Batch after sv_PopTo: 0x4b6040 ai_Unk4b6040 -> ai_MissionPartnerDialog (behaviour + readable), 0x4b6110
  ai_Unk4b6110 -> ai_MissionCompleteSingle (Q2 still ai_Unk53cb40), 0x4b62a0 ai_Unk4b62a0 ->
  ai_MissionCompleteSequence (byte + readable), 0x6f072c ai_Unk6f072c -> ai_MissionSampleBank (the sample bank
  its comment names). With them 0x4b60f0 ai_MissionResultByMode (byte) and 0x4b5c20 ai_MissionNextDialog (byte)
  reach readable; 0x4b5f10 ai_MissionResultDialog and 0x4b5970 ai_MissionCompleteDialog are readable with
  behaviour still draft.
- Helpers named (evidence: their own comments): 0x4b5160 ai_Unk4b5160 -> ai_TimedCondition (the timed mission
  condition, not ok resets its time, ok starts it and compares min minutes), 0x4b51f0 ai_Unk4b51f0 ->
  ai_CharBuildingShare (c's share among the holders of a 0x501190 list). Both help the callers' Q2; the
  functions themselves keep Q4 (`*(unsigned short *)((char *)b + 0x27)`, G13_4FE1E0 / G13_501190 macros).
  Unit readability at the end of the session: 11 -> 26 of 59; behaviour 50 byte + 2 behaviour, unchanged.

## match-base 2026-10-26 (base worker, shard ?/10)

Check 2593/4664 before and after (no byte change); unit 50/59 matched, 1486 differing bytes (unchanged).
Two escalations, no readability change needed (both functions were already readable).
- ai_MissionCompleteDialog (0x4b5970, 377/720, 157 differing) -> match-deep. Read every hunk: no meaning error;
  the three branches, the merged sv_PopRequest tail and the closing again/quit/sample-bank block are the same
  statements. The original keeps h in ebx (its 0 constant web, reused as h), ours spills h to a slot - that is
  the whole 0x194 vs 0x190 frame difference (8 stack locals vs 7) - keeps the 0 web in esi instead of ebx and
  flags in ebx instead of eax. `uv run spell`: reload x2, dup-tail, width; leads tried one edit + one build each, all
  still 157: `G13TYPE *t` at function scope instead of block scope, `int h` declared inside the do body,
  nm/again/quit as declaration initializers. c2prio not run (frida missing).
- ai_MissionResultDialog (0x4b5f10, 137/304, 20 code + 16 regs + 4 fill) -> match-deep. It is one callee-saved
  colour swap: `asmdiff --regs ebp=ebx,ebx=ebp` leaves only the ebp/ebx push order, the `[ebp+0]` addressing
  byte, `or ebp,0x80` against `or bl,0x80` (no bpl in 32-bit; downstream of the swap) and the tail fill. c
  (&ai_Chars[ai_PlayerId]) and t (ai_GetMissionType) are the swapped pair: ours c=ebp/t=ebx, the original
  c=ebx/t=ebp; flags and d2_ClickedId's -1 inherit c's register. Leads tried: t declared before c (no change),
  t assigned before c (c -> ebx, but ai_GetMissionType moves ahead of the lea and the prologue reorders:
  20 -> 32 differing, reverted). `uv run spell` has no regs entry.
- Lesson: a function whose only remaining hunk is the same instruction sequence with two callee-saved registers
  swapped is not base work; the menu has no entry for it (diffclass `regs`), it needs the priority tools.
- Next: the base queue moved to unk_4b6480.c (started reading ai_Unk4b82c0 0x4b82c0: one hunk, the 4th
  txt_Format argument; the original does `add ecx,0x20d; add ecx,edx` where VC6 folds our expression into one
  `lea eax,[ecx+edx*1+0x20d]` - an add-lea lead, not finished).
