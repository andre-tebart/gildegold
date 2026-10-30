# hst.c (was unk_4adf40.c)

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the findings of each session, one section per author.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): history texts (historie\text_H_<player>): the text tokens / text commands, label replacement, date labels, the yearly history dialogs (0x4b3350 / 0x4b36d0 / 0x4b3960)
- Boundaries: separated (plausible): tag "hst_" (hst_ParseText_Fake_1st_Pass / _2nd_Pass, hst_ParseContext, hst_ParseCommandline_1st_Pass / _2nd_Pass, hst_ParseText_Real, hst_ParseDate in its error texts, layout 2026-10-22 session 6). Start: its .data block starts at 0x601058 (-1 globals, "FEST") right after hmt's (proven end of hmt); the helpers 0x4adf40..0x4ae120 have no data (called by the callbacks: plausible). Block in function order: callback tables 0x601b10 (15 replacements, names 0x601730) / 0x601b50 (27 text commands, names 0x601070), the commands' literals 0x601bbc.., "historie\text_H_%s" 0x601c48, the hst_ messages, "$C$Z$[%s$]" / "$C%s" 0x601f70 (0x4b3350 / 0x4b36d0). End: the next block 0x601f88 (mission types) is used from 0x4b3ae0 on, and the code after 0x4b3960 is about missions (plausible)
- Range then: 0x4adf40..0x4b39f0
- Was: unk_4adf40.c (layout 2026-10-22, renamed session 6)

## Open work
- **headers (2026-10-30, typing pass 2)**: head.c includes hst.h, txt.h, net.h, d2_types.h; keeps 2 cast / view macros with @original byte evidence (the address and the matched caller are in each comment): util_RandomU, gm_AddHealthS; views / casts without byte evidence of their own (weak definitions or drafts' spellings, notes/headers.md debt item 1): ai_GoodsT, ai_BldsT, ai_BTypesT, ai_CharsT, ai_TimeT.
- Session 6 near-misses: 0x4b1bd0 (only the out-of-line `n = 10` block placement; tried `if (n > 10) .. else if`
  and `if (n <= 10) {..} else`), 0x4b3280 (the else-if chain after `d == 1` sits inline, original out of line; tried
  `r = d` and swapped branches), 0x4b35e0 65 B (same shape as 0x4b3280), 0x4b2530 hst_ParseCommandline_2nd_Pass
  124 B (p lives in k's parameter slot in the original, AI_ZERO temps in the other order).
- Session 6 drafts far off: 0x4b11e0 (780 B, register / slot choice of the two name tables), 0x4b21c0
  hst_ParseContext (stack slots: s written back to its parameter slot), 0x4b28d0 hst_ParseText_Fake_2nd_Pass
  (block layout), 0x4b2ed0 hst_ParseDate (992 vs 944: the copy temps of G12_COPY2 differ per call: [ebp-4] src,
  [ebp-8] dst, the count in the dead parameter slot k; the mode-4 copy reads name / buf straight from their slots).
- Near-misses: 0x4afdd0 / 0x4aff30 / 0x4affc0 / 0x4b0050 / 0x4b1060 (22-24 B: AIMSG store order, cf. 0x4afe80
  which matched with `msg.type` before `TMX(msg) = NOWX`), 0x4af2c0 47 B (the money-text tail, see below), 0x4ae160
  111 / 0x4ae280 114 (two attempts used).
- Drafts far off: 0x4adf40, the text tokens 0x4ae3a0 / 0x4ae600 / 0x4ae900 / 0x4aec00 / 0x4af3f0 (the random search
  loops), 0x4aef10 / 0x4af8d0 (slot reuse: n2 lives in fmt's parameter slot, the count 0x200 in a local), 0x4af6c0
  / 0x4afa20 (top-5 lists), 0x4afc30, 0x4af1b0, 0x4b00e0 (335; the inner-block tables shortened it to 416 / 448),
  0x4b02a0, 0x4b0360, 0x4b0640 (G12XMSG, list copy with rep movsd / stosd), 0x4b09f0, 0x4b0b50, 0x4b0d40, 0x4b0fd0.
- Type debt: G12_5176D0 casts ai_Unk5176d0 (a format string instead of a text id); G12_B reads AICHAR bytes;
  G12OBJ repeats hmt.h's HMTOBJ (0x4e6ec0 / 0x4e6680 declared in both files); G12XMSG (0x4c4fa0's message) is ours.

## layout 2026-10-22 (session 4)
- 0x4adff0..0x4ae120: predicates by character id (exists, unk008 > 0, kind ranges); `unk008 > 0` (not `!= 0`)
  gives the original jbe. 0x4adfb0 searches ai_Unkf83540[4] (G12ROW, 8 id pairs each).
- Text tokens (int mode, G12ARG *args, int k, char *fmt, char *out): mode 1 checks args[k].id, otherwise a character
  is picked (mode 0 stores it); fmt "...-n" selects anybody / no human player (default) / a computer one first.

## layout 2026-10-22 (session 5)
- The unit's callbacks are listed in the .data table 0x601b10..0x601bb8 (one slot 0x601b4c is 0): text tokens
  (int mode, G12ARG *args, int k, char *fmt, char *out) up to 0x4afc30, then text commands (args / k / fmt or
  (a, b, fmt)) from 0x4afdd0. The table sits in the block that starts at 0x601058, so this unit starts after hmt.c.
- The text tokens clear their buffer with AI_ZERO (inline asm: the ebp frame and Ghidra's three zero loops), not
  memset; the count 0x200 and the buffer pointer are the two out variables of 0x4adf40, often in dead parameter
  slots (0x4ae160 / 0x4ae280: `args = buf; k = 0x200; AI_ZERO(args, k)`). That fixed 0x4af090 (matched with
  `pick = b; best = v;` order).
- "-PLUS" / "-MINUS" commands: `int sign[2] = {-1, 1}; char name[2][6] = {"MINUS", "PLUS"};` declared in the
  block after the test that guards them (the copy from .data comes after that test): 0x4b04a0 matched so.
  Matching is `memcmp(fmt, name[i], strlen(name[i])) == 0` (inline repnz scasb + repz cmpsb).
- The money texts (0x4af1b0 / 0x4af2c0): `0x5085b0((int)(n * 0.01f * v), ai_Unk6f1bc5)` is pushed with a third
  dword (eax with al = ai_Unk6f1bc5 and the zero-extended copy in ecx): unsolved.
- 0x4b0950 calls tolower() with a pointer (fmt + 5): kept (sic). 0x4b0b50 skips the decision name with the sign's
  index (sic). 0x4fca40 returns a float that 0x4b05e0 discards (fstp st(0)).


## layout 2026-10-22 (session 6)
- Name: the error texts leak hst_ParseText_Fake_1st_Pass (0x4b1fa0), hst_ParseContext (0x4b21c0),
  hst_ParseCommandline_1st_Pass / _2nd_Pass (0x4b2380 / 0x4b2530), hst_ParseText_Fake_2nd_Pass (0x4b28d0),
  hst_ParseText_Real (0x4b2ce0), hst_ParseDate (0x4b2ed0); the errors go to a local buffer only (no output).
- Texts: "[_NEWn_NAME..." replacements (mode _NEW / _USE / _REL, slot n of a G12ROW list, the 15 callbacks of
  0x601b10), "[_SET_n" / "[_USE_n" select list n of ai_Unkf83540 (the text commands of 0x601b50 get the list), dates
  "DD.MM.YYYY" (year - 1400 = game year). The history file is txt.c's "historie\text_H_<player name>"; 0x4b3280 /
  0x4b35e0 step through it 3 texts per entry (date, text, commands) from ai_Unk601060.
- Copy idioms: AI_ZERO's twin with rep movsb / movsd exists in three shapes (G12_COPY: global source as an
  immediate; G12_COPY_P: source pointer variable; G12_COPY2: all three temps); G12_ZERO_G zeroes a global by
  immediate. The byte copy loop `while ((*d++ = *s++) != 0);` gives the original's inline strcpy (mov cl / mov
  [edx+eax] / inc / test).
- Matched on the first or second try with the shapes of earlier sessions: message senders (`memset(&msg)` first,
  `msg.unk36` before `TMX(msg) = ..`), `for (i = 0; i < 0x300 && n < 8; i++)` (count test at the loop top), a
  pointer local computed before the call that might change it (0x4b2380 `row = &ai_Unkf83540[n]`).

## match-base 2026-10-25 (base-0 worker, shard 0/6)

- 0x4f2720 calls renamed ai_Unk4f2720 -> gv_GetTableRecord; byte unchanged.

## match-base 2026-10-26 (base-2 worker, shard 2/10)

Readability only (no code change, check 2573/4664 unchanged, unitdiff hst.c no worse, lint clean).
- Names (evidence = each function's own address comment; every use updated in include/hst.h and src/):
  0x4adfb0 ai_Unk4adfb0 -> ai_NotInLists (1 if the id is in none of the four hst_Lists lists),
  0x4adff0 -> ai_IsLivingCharacter (id exists, lives, kind < 10), 0x4ae030 -> ai_IsLivingPlayer
  (... and kind 5 / 6 / 7), 0x4ae080 -> ai_IsLivingNonPlayer (kind < 10 and not 6 / 7),
  0x4ae0d0 -> ai_IsLivingHuman (kind 6 / 7). All five were Q1-only and are byte + readable now
  (readable 1 -> 6 of 67).
- Left in the same predicate family: 0x4ae120 (`/* 0x4ae120, ... of kind 0 */`) stays a placeholder - the
  comment only restates `kind == 0`, no role evidence.

## match-base 2026-10-26 (base-2 worker, shard 2/6, second session)

### The 15 replacement-token callbacks: evidence from the data tables
`ai_Unk601b10[15]` (0x601b10) holds the callback pointers in table order, `ai_Unk601730[15]` (0x601730) the
token names in the same order (dumped from the exe `.data` with a PE section walk; both are externs in hst.c):
0 0x4ae160 BUERGERMEISTER, 1 0x4ae280 BISCHOF, 2 0x4ae3a0 RND_GILDENMEISTER, 3 0x4ae600 RND_AMTSTRAEGERIN,
4 0x4ae900 RND_AMTSTRAEGER, 5 0x4aec00 RND_AMTSPERSON, 6 0x4aef10 REICHSTER_EINWOHNER, 7 0x4af090
BESTES_WIRTSHAUS, 8 0x4af1b0 GELD, 9 0x4af2c0 STADTKASSE, 10 0x4af3f0 RND_KIRCHENBERUF, 11 0x4af6c0 RND_REICH,
12 0x4af8d0 RND_NPC_EINWOHNER, 13 0x4afa20 RND_HANDELSHERR, 14 0x4afc30 RND_SPIELER. Every token function now
has the name of its token (ai_TokenMayor, ai_TokenBishop, ai_TokenRandomGuildMaster,
ai_TokenRandomOfficeHolderFemale / Holder / Person, ai_TokenRichestInhabitant, ai_TokenBestInn,
ai_TokenMoney, ai_TokenCityTreasury, ai_TokenRandomChurchProfession, ai_TokenRandomRich,
ai_TokenRandomNpcInhabitant, ai_TokenRandomMerchant, ai_TokenRandomPlayer). The spelling of the predicates
also confirms the semantics: 0x4ae160 is officeKind 0x0f = the mayor, 0x4ae280 officeKind 0x15 / 0x1a = the
bishop, 0x4ae3a0 picks secondOffice 0x1e..0x21 = the guild-master offices (it is *not* "aged 30..33").

### Matched this session
- 0x4adf40 ai_SplitTokenFormat (was ai_Unk4adf40): byte + readable. The dst pointer must be a *local*
  (`d = dst;` used everywhere) so VC6 keeps it in eax; with `dst` itself the parameter is reloaded from its
  slot after every store (aliasing) and the whole prologue/loop shifts.
- 0x4ae280 ai_TokenBishop: byte + readable. Its search condition is
  `!(ai_IsLivingCharacter(i) && ai_Chars[i].officeKind == 0x1a || ai_Chars[i].officeKind == 0x15)` - the
  `|| 0x15` sits *outside* the living test (the original's `je` jumps from the living test straight to the
  0x15 compare), so a dead bishop is still found. The outer `||` also makes VC6 compare the byte twice
  (memory operands, no `mov al` CSE) *and* removes the folded return tail below.
- 0x4af090 ai_TokenBestInn: was Q1-only, now byte + readable (name only).

### The folded return tail (why so many original tails are duplicated)
VC6 merges a `return <constant>;` block into an earlier block with the same constant - `test; je <old block>`
instead of `test; jne <continue>` + its own copy. The originals usually keep the copy. What removes the fold
(seen here): a different *flow shape* earlier in the same function - in 0x4ae280 fixing the loop condition's
outer `||` was enough, in 0x4ae3a0 the `for (n = 0x300; n != 0; n--) { if (found) break; ... } if (n == 0)
return 0; pick = ..` form gave the original's `dec esi; test esi,esi; jne` + inline return. `if (--n == 0)
return 0;` inside the loop gives `dec; je` and the fold. 0x4ae160's one remaining hunk is exactly this fold and
is with match-deep (see its @deep comment); the simple spell forms (if/if!, braces, if/else, while, do/while,
switch, nested if, reorder, dead statements between) all still fold - checked in build/scratch reproductions.

### AI_ZERO / ai_SplitTokenFormat frame (the text-token prologue)
The original prologue of the tokens is: `n1 = (int)buf;` then a variable-typed size for AI_ZERO, then
`ai_SplitTokenFormat(fmt, buf, &n1, &n2)` with **n2 never initialised** - so VC6 places n2 in the dead `fmt`
parameter slot (`lea ecx,[ebp+0x14]`, the 4th push) and only `_zn` gets a frame slot (the 0x200 store), frame
0x208. Writing `AI_ZERO((char *)n1, 0x200)` + no `n2 = 0x200;` reproduces that in 0x4ae3a0 / 0x4ae600.
Still unsolved there: our `_zp` temp gets its own 4-byte slot and is read for edi, the original reads n1's slot
directly (frame 0x20c vs 0x208) - tried AI_ZERO(n1,..), AI_ZERO(buf,..), AI_ZERO((char *)n1,..).

### Meaning fixes applied to the far token drafts (all kept, no WORSE anywhere)
- 0x4ae3a0 (ai_TokenRandomGuildMaster, 175 -> 65 code hunks): `c->health <= 0` in the mode-1 arm (the
  original `test cl,cl; jbe`, i.e. <= 0, not == 0); `secondOffice >= 0x1e && <= 0x21` in all four predicates
  (the original compares 0x1e / 0x21, not `> 0x1d` / `< 0x22`); the sel 0/1 search loops as above.
- Unit-wide (all remaining token functions): the `n1` / AI_ZERO / n2 prologue above, and the two-shape search
  loop. unitdiff hst.c 9906 -> 9824 differing bytes, 33/67 matched (unchanged), no grade fell.

### With match-deep
ai_TokenMayor (0x4ae160, the folded tail), ai_TokenRandomGuildMaster (0x4ae3a0, _zp slot + sel==2 regs +
tail), ai_TokenRandomOfficeHolderFemale (0x4ae600), ai_TokenRandomOfficeHolder (0x4ae900),
ai_TokenRandomOfficePerson (0x4aec00), ai_TokenRichestInhabitant (0x4aef10, _zp slot + filt/sex slots).
0x4aef10's -10000000 start value is now `HST_NO_WORTH` (Q3).

### Open work for the next base session
- Q3/Q1 only, cheap: the remaining token functions' address comments can take the token names (done for all
  15); `ledger grade next` now gives ai_TokenCityTreasury / ai_TokenRandomChurchProfession / ai_TokenRandomRich /
  ai_TokenRandomNpcInhabitant / ai_TokenRandomMerchant / ai_TokenRandomPlayer next.
- The `_zp` slot question above is the single biggest lever left in this family (it shifts every ebp offset).

### Second half of the session (same worker)
- The token names above are now on all 15 functions; ai_TokenBestInn was Q1-only and is byte + readable.
- ai_Unk4ae120 -> ai_IsLivingInhabitant: its only caller is the RND_NPC_EINWOHNER token (0x4af8d0), so kind 0
  is the plain inhabitant kind (the previous session left it as "no role evidence").
- `-10000000` (the start value of the richest/merchant top-5 lists, 10 sites) is `HST_NO_WORTH` now: the
  literal is inside the image range and failed Q3.
- gm_Unk5085b0 / ai_Unk5085b0 (0x5085b0, f3gm.c) -> gm_ConvertMoney, project-wide (60 references in 16
  files; no byte changed). Its role: "a / the divisor of the player slot k, rounded". The K&R declaration in
  unk_5d33b0.c had to become the canonical prototype (lint cross-TU conflict).
- Also with match-deep (behaviour only, all read and described): ai_TokenRandomRich (0x4af6c0, 173 code
  hunks), ai_TokenRandomNpcInhabitant (0x4af8d0, 180), ai_TokenRandomMerchant (0x4afa20, 82),
  ai_TokenRandomPlayer (0x4afc30, 70), ai_TokenRandomChurchProfession (0x4af3f0, 105), ai_TokenMoney
  (0x4af1b0, the 3-push/2-pop gm_ConvertMoney tail), ai_TokenCityTreasury (0x4af2c0, Q2 blocked on the
  0x50b010 return type: f3gm.c defines it as unsigned char *, all callers use AIBLD *).
- End state: hst.c byte 31/67 -> 33/67, readable 6/67 -> 24/67, differing bytes 10128 -> 9824; its base
  queue went from 33 draft functions to 18 (15 raised: 2 byte + readable, 11 readable + escalated to
  match-deep, 2 Q-only readability gains that stay in the deep queue). check 2603 -> 2605/4664.


## match-base 2026-10-26 (base-2 worker, shard 2/6, third session)

### The text-command callbacks: names from the data table 0x601070 / 0x601b50
Dumped from the exe `.data` (PE section walk): `ai_Unk601070[27]` (0x601070, the command names) and
`ai_Unk601b50[27]` (0x601b50, the callbacks) pair up index by index, so every command function can be named
after its own command. Two idioms found this session fixed most of the family (see notes/vc6-matching.md):

1. **The guard is an early return, the tables live in a block after it**: `if (*fmt != '-') return 0;` +
   `{ int sign[2] = ..; ... }` instead of `if (*fmt == '-') { .. } return 0;`. The `if (..) { body }` spelling
   makes VC6 put the return-0 block *after* the body, which shifts the whole schedule; the early-return form
   puts it inline at the top like the original (0x4b00e0: 96 code hunks -> 3; 0x4b09f0 / 0x4b0b50: full
   instruction match).
2. **Compute the cast result once**: `v = (int)(gm_GetGoodsValue(c) * f); cm_SendMoveGoodsUnchecked(.., v)`
   instead of `(int)(v * f)` at both call sites - that one extra local slot was the whole frame difference
   (0x4b0360 frame 0x18 -> 0x14, byte).

### AIMSG: unka0 (0xa0) is not the field the 0x4e / 0x50 event messages write
0x4affc0 / 0x4b0050 store 1000 as a **dword at msg+0x9c** (and 0x4f2750's message writes unka0 at 0xa0 - both
fields exist). A new local view `G12EVENTMSG` in hst.c (0x9c `value`, 0xf8 bytes) reproduces it, and with the
guard/flags order (`msg.flags` before the value, value before `TMX`) both functions went **byte**.

### Matched this session
- 0x4affc0 ai_CommandFire (BRAND) - byte + draft (Q4 blocked, see below)
- 0x4b0050 ai_CommandTornado (WIRBELSTURM) - byte + draft (Q4 blocked)
- 0x4b0360 ai_CommandWealth (VERMOEGEN) - byte + readable
- 0x4b04a0 ai_CommandOfficeStanding (ANSEHEN_BEI_AMTSTRAEGERN) - byte + readable (was Q1 only)
- 0x4b05e0 ai_CommandKillPlayer (KILL_PLAYER) - byte + readable
- 0x4b09f0 ai_CommandLaw (GESETZ) - byte + readable
- 0x4b08e0 ai_CommandPrison (STRAFE_KERKER), 0x4b0950 ai_CommandOfficeInventoryPlus
  (AMTSTRAEGER_INVENTAR_PLUS), 0x4b1c90 ai_CommandInventoryPlus (INVENTAR_PLUS) - byte + readable (Q1 only)

### Escalated
- 0x4afdd0 ai_CommandFest (FEST): 38 code hunks, all scheduling - the original hoists ai_Time's first dword
  above the msg zeroing loop and stores the TMX copy a,c,b,d; source-order variants move the hoist but not the
  store order (29 differing then). Q4 TMX blocked.
- 0x4b00e0 ai_CommandCityTreasury (STADTKASSE): 184/184 instructions, 3 hunks - n sits in the dead fmt slot
  and v in sign's dead slot in the original, swapped in ours; declaration order / block scope of v tried.
- 0x4b02a0 ai_CommandWealthTax (VERMOEGEN_STEUER): the original opens with `push ecx` (a 4-byte local), ours
  with `push ebx`; the frame is +4. Q2 blocked on ai_Unk50b010.
- 0x4b0640 ai_CommandExecution (STRAFE_HINRICHTUNG): nested-block slot reuse (frame 0x158 vs 0x16c).
- 0x4b0b50 ai_CommandLawChange (GESETZ_REL): kinds[2] at [esp+0x68] vs the original [esp+0x70].

### Q4 / Q2 blocks found (evidence in the reasons)
- The TMX/NOWX macro of am.h is a deref-cast macro (Q4). A typed view needs an `AITIMEX` member on `AITIME`
  in ai.h; both spellings tried (a union inside the struct, and `typedef union AITIME`) and **both regress
  ai_Unk483a60 in ai.c from byte to the [esp+0x10]/[esp+0x18] slot swap** - the same regression
  ai_StartBuildingWork's note records for an AIMSG char view. Reverted; shared-header decision.
  Superseded 2026-10-27: `msg.time = ai_Time` is byte-identical to `TMX(msg) = NOWX` and Q4-clean here, so this
  is not a block for hst.c - see the 2026-10-27 "Q4/Q2 blocks confirmed". The AITIME/AITIMEX header work is
  only needed where the typed assignment changes the schedule.
- ai_Unk50b010 (0x50b010, f3gm.c) still returns `unsigned char *` while every caller uses `AIBLD *`.
- The 0x4c4fa0 vote/execution sender and G12XMSG's unk0c/unk14/unk26/unk7d have no evidenced names.

### Renames done this session (project-wide)
ai_Unk4afdd0 -> ai_CommandFest, ai_Unk4affc0 -> ai_CommandFire, ai_Unk4b0050 -> ai_CommandTornado,
ai_Unk4b00e0 -> ai_CommandCityTreasury, ai_Unk4b02a0 -> ai_CommandWealthTax, ai_Unk4b0360 ->
ai_CommandWealth, ai_Unk4b04a0 -> ai_CommandOfficeStanding, ai_Unk4b05e0 -> ai_CommandKillPlayer,
ai_Unk4b0640 -> ai_CommandExecution, ai_Unk4b09f0 -> ai_CommandLaw, ai_Unk4b0b50 -> ai_CommandLawChange,
ai_Unk4b08e0 -> ai_CommandPrison, ai_Unk4b0950 -> ai_CommandOfficeInventoryPlus, ai_Unk4b1c90 ->
ai_CommandInventoryPlus; ai_Unk4fc820 / unk_Unk4fc820 -> gm_CharacterHealth (f3gm.c, health from the address
comment; cm.h/hst.h prototypes unified to `short *`); ai_Unk4f2750 / unk_Unk4f2750 -> gv_SetTableRecord
(gv.c, sends 0x4c5930 setting table record i of character c; unk_58bbd0.c's declaration unified).

### End state
hst.c byte 35/67, readable 24 -> 36/67 (12 more), differing bytes 9776 -> 8753; check 2612 -> 2614/4664
(the session started at 2610, +4 byte functions: ai_CommandFire, ai_CommandTornado, ai_CommandWealth,
ai_CommandLaw).

## match-base 2026-10-27 (solo, main)

### Matched this session
- 0x4b0d40 ai_CommandFaithChange (GLAUBENSWECHSEL, was ai_Unk4b0d40) - **behaviour + readable**, 165 -> 6
  differing instructions (48 bytes). The two fixes that did it were meaning errors the instruction diff could
  not see (both sides compile the same shapes, only the symbol/slot differs):
  1. **The name array order was swapped**: the original's `ds:0x601c20` (KATHOLISCH) goes into the *lower*
     slot `[esp+0x14]` and is element 0, so the "-KATHOLISCH" word counts the religion byte 0x0c == 1
     (protestant, f3init.c INIT_FIELD_RELIGION) and sends ch = 1. Ours counted the 0s for KATHOLISCH - the
     two commands were effectively exchanged. `char name[2][16] = { "KATHOLISCH", "EVANGELISCH" }`.
  2. **The percent product is a float product**: the original does `fild cnt; fimul p; fmul 0.01f`, ours an
     integer `imul` then a conversion. `n = (int)((float)cnt * p * 0.01f)` reproduced it and, with it, the
     frame and every slot (our frame was 8 bytes smaller with the `imul`): 149 -> 6 differing instructions in
     one edit (the remaining hunks at that point were the d1/d2 pair order and the prologue).
  Further fixes: `char d1[4]` before `char d2[4]` (the original's d1 is the higher slot), the two 4-byte zero
  stores as `memset` of each buffer (Q4), `msg.to = ((AICHAR *)ai_PlayerChar)->netId` (the file's idiom, Q4),
  and `msg.time = ai_Time` for `TMX(msg) = NOWX` - **byte-identical** here and Q4-clean, so the TMX block below
  is not a block for this function.
  Escalated to match-deep for the last 3 order hunks: the two digit-loop `i = 0` inits sit after `fmt++` / after
  the `*fmt == 0` test here and before them in the original (0x4b0e05 / 0x4b0e4d), and the TMX word store
  (ids[0], [esp+0x7c]) is kept until after fromId/flags/car/n in the original (0x4b0fa9) but emitted right
  after the TMX dwords here (0x4b0f82).
- 0x4b0fd0 ai_CommandRiot (AUFRUHR, was ai_Unk4b0fd0) - readable (Q1 + Q4: typed netId, `msg.time = ai_Time`),
  23 asm / 91 B. Escalated: the same blocked pattern as 0x4afdd0 ai_CommandFest - the original hoists ai_Time's
  first dword above the msg zeroing loop (0x4b0fd6) and stores the TMX copy a,c,b,d; ours keeps source order and
  stores b,c,a,d. `TMX` before `msg.to` tried: 29 asm, worse.
- 0x4b11e0 ai_CommandDemand (NACHFRAGE) - Q1 + Q4 only (still Q2), 332 -> 275 asm. Two **meaning fixes**:
  1. The three stock loops pass `&e` (the whole 0x80-byte record) to 0x4c56d0, not `e.unk08`: the original's
     `lea ecx,[esp+0x54]` with 8 bytes pushed is e+0 (e's `rep movs` destination is [esp+0x4c]) and 0x4c56d0
     memcpy's 0x80 bytes from it, so `e.unk08` sent a record 8 bytes in and 8 bytes past its end.
  2. **The stock block stride was wrong**: `G12STOCK init_StockBlocks[][0x55]` (0x7d6a70, stride 0x2a80) read
     the wrong player's records for every ai_OwnPlayer > 0. The original's address chain is
     `0x7d6a70 + player * 0x2a90 + i * 0x80` - f3gm.c's GM_GOODS and f3init.c's INIT_STOCKBLOCK (0x2a90 per
     player: four price-index ints, then the 0x55 records). A block struct with the records first and a
     0x10-byte tail pad keeps the symbol at 0x7d6a70 and gives the right stride; that alone took 330 -> 275.
  The record's fields are now named from f3init.c's INIT_STOCK (item / amount / demand 0x20 / mean 0x24 /
  price 0x38 - all three match the arithmetic here), so Q2's G12STOCK placeholders are gone. Left: Q2 on
  ai_Unk4c56d0 (0x4c56d0 = message 0x3f, k and 0x80 bytes of p, cm.c - `cm_SendGoodsRecord` would be the name)
  and ai_Unk4cd9d0 (0x4cd9d0, the record of ten with id, cm.c) - both need their own units' evidence.
  Remaining code: mostly the register permutation fmt/i/cursor (original ebp/edx/ebx, ours edx/ebx/ebp) and
  ours spilling `i` to [esp+0x10] inside the name loop; a hand-walked pointer-local variant
  (`G12STOCK *s = &init_StockBlocks[ai_OwnPlayer][i]`) is worse (336 asm) and the original recomputes the
  player base every iteration, so no base hoisting - allocation, not meaning.

### Q4 / Q2 blocks confirmed
- `msg.time = ai_Time` is a byte-neutral Q4 replacement for `TMX(msg) = NOWX` in this unit (proven here and
  worth trying in ai_CommandFest before the AITIME/aitimex header work).
- The TMX/NOWX deref-cast macro block (above) still stands for the functions where the typed assignment changes
  the schedule.

### Data names (Q2) cleared this session
- 0x601730 ai_Unk601730 -> hst_TokenNames, 0x601af0 ai_Unk601af0 -> hst_Modes, 0x601b10 ai_Unk601b10 ->
  hst_TokenCallbacks (hst.c only)
- 0x601070 ai_Unk601070 -> hst_CommandNames, 0x601b50 ai_Unk601b50 -> hst_CommandCallbacks, 0x601b00
  ai_Unk601b00 -> hst_ListModes (hst.c only; the notes call them the command tables)
- hst.h G12ROW.unk00 -> inUse (1 while the list holds entries, 0x4b1ee0 clears it, [_SET_ / [_USE_ need it);
  that rename alone made 0x4b28d0 hst_ParseText_Fake_2nd_Pass readable
- G12STOCK fields: unk08 -> padding, unk20/unk24/unk38 -> demand/mean/price (f3init.c's INIT_STOCK; the record
  is amt.h's AMTGOODS), plus the G12STOCKBLOCK stride fix (see 0x4b11e0 above)

### Escalated this session (all with the reason in the address comment)
- 0x4b0d40 ai_CommandFaithChange: behaviour + readable, 3 order hunks (digit-loop i = 0 inits, TMX word store)
- 0x4b0fd0 ai_CommandRiot: readable, the ai_CommandFest ai_Time-hoist / a,c,b,d store pattern
- 0x4b11e0 ai_CommandDemand: Q1 + Q4, the register rotation (fmt/i/cursor) + AI-kept-alive g; Q2 block on
  0x4c56d0 / 0x4cd9d0 (cm.c needs the names)
- 0x4b21c0 hst_ParseContext: 4-byte frame (AI_ZERO _zn/_zp slots) + slot order; Q2 block G12ARG.unk00, Q4 block
  the 4-byte mode compare
- 0x4b2530 hst_ParseCommandline_2nd_Pass: 317/317 instructions, the same AI_ZERO temp + slot-pair problem; Q4
  block the 8 prefix compares (memcmp measured worse and lost 0x4b2380's byte match)
- 0x4b28d0 hst_ParseText_Fake_2nd_Pass: 266 differing, eax/ecx/edx rotation + the s spill to [ebp+0x8]

### End state (this session)
hst.c: byte 35/67 (unchanged), behaviour 141 -> 142, readable 1821 -> 1824 overall (+1 / +3):
- 0x4b0d40 ai_CommandFaithChange: behaviour + readable, 165 asm / 524 B -> 6 asm / 48 B
- 0x4b0fd0 ai_CommandRiot: readable, 23 asm / 91 B
- 0x4b11e0 ai_CommandDemand: 332 asm / 780 B -> 275 asm / 881 B
- 0x4b2530 hst_ParseCommandline_2nd_Pass: Q2 cleared (Q4 left)
- 0x4b28d0 hst_ParseText_Fake_2nd_Pass: readable (the G12ROW.inUse rename)
`ledger check`: 0 errors, 2620/4664 matching before and after; `uv run link` 0 conflicts, 0 data bytes differ.

## match-base 2026-10-27 (solo session on main)

- 0x4b2ed0 hst_ParseDate: `strlen(buf) != 10` (was `< 10`; the original is `cmp 0xa / je` to the parse, so a longer
  text takes the error path), and `*year = atoi(tmp) - 1400` written after the day / month normalisation (the
  original calls atoi(tmp) twice back to back and sinks the store below the two `if (d == 0)` / `if (m == 0)`
  blocks). 187 -> 183 differing instructions. Escalated: the frame is 0x17f4 against 0x17f8 and d / m land in dead
  slots ([esp+0xb], [esp-0x4]) where the original gives them -0x28 / -0x27, which shifts known / tmp by 4 and
  rotates every G12_COPY2 temp and the name parameter slot. Tried `short y` (194, the original sign-extends with
  movsx but the cascade grows), `m + 0x4f` (191), both (196), the block-scoped d / m (worse), all 120 declaration
  orders and s / t at function scope (slots unchanged).
- 0x4b3280 -> hst_NextEvent: behaviour + readable. The branch polarity is `if (d == 1) ... else if (d > 1) r = 0;
  else ...` (as the sibling 0x4b35e0 already had), and the `r = 2` arm is `break` out of the do-while, not
  `return r` (see notes/vc6-matching.md): the constant then stays in the variable and the epilogue is copied,
  which turns the code hunks into moved instructions. 26 -> 24 asm.
- 0x4b35e0 -> hst_NextEventCmd: behaviour + readable with the same `break`; ai_Unk60105c -> hst_TextLast (the
  last text of the history file, next to hst_TextFirst / hst_TextCurrent) and 0x4b2c90 -> hst_ParseEventText.
  31 -> 30 asm.
