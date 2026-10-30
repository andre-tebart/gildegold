# unk_58bbd0.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the findings of each session, one section per author.

## Unit
The production / trade windows (layout 2026-10-23 session 21). Start 0x58bbd0, name guess (`@named_by guess`), the
"Handel\Handel_PRODUKTION" screen 0x58bd60 is its first literal user.
- Boundary `plausible`: 0x58bbd0 has no literals; it initializes the tables 0x6f4f08 .. 0x6f5a64 that 0x58bd60 and
  0x58c680 read, is called from 0x58bd60 and from the "HANDEL\HANDEL_SUCHEN" helper 0x584d70 of the previous unit.
  The previous unit (unk_589f10.c, the network lobby) ends with the chat lines 0x58ba70 and its last literal
  0x62aad0; the next literal run starts with "Handel\Handel_PRODUKTION" 0x62b038 (0x58bd60, +0x568).
- Globals: the grid / column tables 0x6f4f08 .. 0x6f5a64, the screen 0x6ed938, the building record 0x7e3f30
  (0x24d bytes per kind), the character goods 0x91aaf0 (0x41 words per record), the goods list 0x74e1f4.
- Current extent (layout 2026-10-24, sixth): 0x58bbd0 .. 0x5ae870, 263 functions, 65 byte. The
  sixth session added the Domizil family (0x5a9b50 .. 0x5aa9a0: the message texts of the kinds 6 / 7 with the
  "NACHRICHTEN_DOMIZIL" keys, the table 0x62c420 of 0x42 entries 0x1c bytes with the count 0x62cb58, the
  percent boxes 0x5a9f80 / 0x5ad000 and the window records) and the transport block 0x5aab80 .. 0x5ae6b0 (the
  Personnel / mercenary book / transport windows, the "Handel\Handel_Transport_neuer_Kauf" purchase window, the
  confirmation and the AIMSG type 0xf order message); the next frontier is 0x5ae870. The
  previous extent (layout 2026-10-24, fifth): 0x58bbd0 .. 0x5a9b50, 239 functions, 59 byte. The
  unit is the last one; its literals climb in function order from "Handel\Handel_PRODUKTION" 0x62b038 to
  "hp_LoadProdSetting" 0x62c298 (the 5.33f 0x62c270 and "Misc\Landkarte2" 0x62c274 before the "hp_" strings),
  one .data block (D8), so no split was found inside. The church rooms (0x5a1af0 .. 0x5a2e60), the thieves'
  guild / dungeon module (0x5a36c0 .. 0x5a4b90), the **kidnapping** family (0x5a4d40 .. 0x5a5bc0), the **city
  watch** ("LOCATIONS\STADTWACHE\STANDARD", 0x5a5d70 .. 0x5a6c40), the **town map window** (0x5a6cb0 ..
  0x5a82f0, "Misc\Landkarte2"), the **production settings** module (0x5a8340 .. 0x5a8cb0, the "hp_" list
  0x6f26c0) and the **plague / gypsy messages** (0x5a8e10 .. 0x5a9470, "KATASTROPHE_PEST" 0x62c2c0, the
  "NACHRICHTEN_ZIGEUNER" keys) and the **graveyard texts** (0x5a9690 .. 0x5a9af0, the FRIEDHOF keys
  0x62c32c .. 0x62c39c) end the stretch; the next frontier is 0x5a9b50.

## Open work
- **headers (2026-10-30, typing pass 2)**: head.c includes txt_views.h, unk_58bbd0.h, di_input.h, evt.h, d3.h, d3_light.h, data_views.h, d2_types.h, f3snd_types.h, ai_types.h, unk_5d33b0_types.h, am_types.h, cm_types.h, time_view.h, ls.h, f3init.h; keeps 3 cast / view macros with @original byte evidence (the address and the matched caller are in each comment): unk_Unk5136b0K, sv_SelectPanelObjectK, gm_Unk50a970K.
- ai_Unk58ff00 (0x58ff00, draft): base-3's meaning fix (the call at 0x59035d is `di_ClearEvents(2)`, the original
  pushes 2) moved it from 643 / 896 to 642 / 901 (differing bytes / differing instructions, decomp 2026-10-25):
  one byte closer, five instructions worse in the diff alignment inside an otherwise very different draft (frame /
  register allocation). Kept for the meaning; unitdiff baseline refreshed.
- ai_Unk5a5910 (0x5a5910, draft): 391 differing bytes / 187 asm lines, while the session-start
  unitdiff baseline says 417/181. The baseline is stale: at the session-start commit e2f2d781 the function
  already compiles to 391/187 (checked in a worktree), so this is not a regression of the 2026-10-24 merge.
- **Frontier 0x5d0f20** (984 B, a data pointer; the trial / message helper family of the last stretch
  continues to the end of the game code 0x5de450). The ROUGH drafts of the twelfth session are listed in
  its section below (0x5cf8a0 / 0x5cff70 / 0x5cfc00's field blocks, the 0x4c3c00 argument details).
- layout 2026-10-24 (fifth): the drafted-but-unmatched functions of the map / production settings stretch:
  0x5a8360? no - 0x5a8340 (63/64: the ushort / 0x7fff division shape), 0x5a83b0 (250/272), 0x5a8560
  (203/240: the inlined memset of the create), 0x5a8710 (340/352), 0x5a8870 (126/144: the register choice of
  the pick loop), 0x5a8940 (63/224), 0x5a8a20 (258/304), 0x5a78a0 (215/272), 0x5a79e0 (435/464), 0x5a7bc0
  (222/224), 0x5a7cd0 (1223/1376 ROUGH SKELETON) and 0x5a6cb0 (2322/2672 ROUGH).
- Matched this session: 0x5a82f0, 0x5a8390, 0x5a84b0, 0x5a84d0, 0x5a8540, 0x5a86c0, 0x5a8850, 0x5a88e0,
  0x5a8900, 0x5a8920, 0x5a8b80 (the clock compare as `0x75 <= x`), 0x5a8c00, 0x5a8c50, 0x5a8cb0, 0x5a8d40
  and 0x5a8d80 (the original's redundant `kind == 6 || kind != 7`).
- Ghidra-missed start: 0x5a8d80 (read from uv run disasm, it is reached only from 0x544070).
- layout 2026-10-24 (fourth): the drafted-but-unmatched functions of the kidnapping / city watch stretch and
  their closest causes:
  0x5a4f70 (8/272: the two id pairs swapped registers - esi/edi vs ebx/ebp; the declarations and the
  comparison order are right, the c2prio ties are equal, so the cause is outside the source form),
  0x5a5080 (9/160: param_1 and the mask constant 0x200 swapped ebx/ebp), 0x5a53e0 (71/336: the request
  building block's scheduling: the original loads 0x5eed68 and pushes 0x19b8 before the two rep stosd fills),
  0x5a5120 (627/736: the AIMSG zeroing, the local slots and the object block 0x449690 / 0x447300 shape),
  0x5a5530 (790/1024 ROUGH: the frame is 32 B too large and the min() double evaluation is only guessed),
  0x5a5910 (409/720), 0x5a5bc0 (296/432), 0x5a5d70 (410/544), 0x5a5fd0 (455/560), 0x5a6270 (434/544),
  0x5a64d0 (431/592), 0x5a67a0 (293/400), 0x5a6920 (185/416), 0x5a6a90 (134/192), 0x5a6b70 (214/272).
- Matched this session: 0x5a4f50, 0x5a5f60, 0x5a61f0, 0x5a6480, 0x5a6720, 0x5a6b50, 0x5a6c40 (first try each).
- Ghidra-missed starts found and drafted this session (the nop scan of 0x5a5120 .. 0x5a7bc0, plus the ledger):
  0x5a5120, 0x5a5d70, 0x5a5fd0, 0x5a6270, 0x5a64d0 (all callbacks / windows reached by address only); the
  batch_decompile exports of the stretch (0x5a53e0 .. 0x5a6c40) are in the main checkout's build/ghidra now.
- Type debt (this session): the AIMSG is built by offsets for the city watch types too (0x43 at +4 with the
  crew ids +0x70, 0x44 with the crew at +0x38 and the clock +0x28 .. +0x34, 0x65 with the name string at
  +0x58 and the wanted ids at +0x38, 0x64 with the record +1 at +0x5c); the control records from 0x475b80 are
  compared with 0x5fe710 and read `U_I(ctrl + 1)` (0x5a5120, msg+0x5c); 0x4a63f0 takes seven arguments
  (param_1, 0, the id 0x4080a0, 0, 0, 100.0f, &out); 0x42ec20 is a float; 0x504ec0 is a float (char *, int).
- 0x5a48d0 (the pickpocket window "locations\diebesgilde\taschendiebstahl", 596 B, ROUGH, nonmatching
  503/592): the carrier 0x53e400 (3, 0, 0x49, 2, the guild slot, the guild id, 3) / 0x53e300 lookup whose
  +0xac is param_1 + 1; the found path 0x524260 over the live table 0x7603a0 .. 0x760420, the AIMSG type 0x49
  with the clock and the +4 ids; the frame is 4 bytes larger.
- 0x5a4b30 (the pickpocket start, 95 B, **matched** after the store order: `req[4] = 7;` before
  `req[2] = 0x90010;`).
- 0x5a4b90 (the pickpocket victim window, 421 B, ROUGH, nonmatching 351/464): 0x524990 (param_1) each frame,
  the six-entry list, the AIMSG type 0x61 with the ids at +0x38 and the owner again at +0x80.
- 0x5a41d0 (the espionage window "locations\diebesgilde\diebesgilde_spionage", 777 B, ROUGH, nonmatching
  691/816): the room walk 0x4ffe90 / 0x4ffcc0 with the room +0x2a / +0xac match, the carriers 0x53e400 /
  0x53e300, the 2 * the kind byte +0x247 threshold and the AIMSG type 0x40; the 39-byte frame excess is the
  list array (0x40) not reproduced.
- 0x5a44e0 (the espionage start, 216 B, nonmatching 129/224): the request record and the kind 0x13 branch
  (0x90010 vs 0xa7fd43b4); close.
- 0x5a45c0 (the spy ring window "locations\diebesgilde\diebesgilde_spionagering", 1540 B, ROUGH SKELETON,
  nonmatching 598/768): the 8-row list with 0x501190 / 0x5091a0, the list control 0x46c490 / 0x46c560 /
  0x46c600 / 0x46c6a0, the window table 0xf8f080 (0x2ec bytes) and the payment 0x4c5010.
- 0x5a1af0 (Spenden, 1141 B, nonmatching 900/1136): the calls are complete; only the frame / local layout is
  off by 8 bytes (three -1 controls, the 512-byte text buffer) - the register roles of the item loop.
- 0x5a36c0 (the production building listener, 330 B, nonmatching 160/368): the five contacts and their five
  windows are written; the frame differs by 38 bytes (the five cached ids in registers).
- 0x5a3810 (the church listener, 412 B, nonmatching 207/432): the dispatch to the church windows and the
  automatic confession (0x1468410 == '0', 0x6f2448); frame 20 bytes larger than the original.
- 0x5a39b0 (the top-5 / indulgence listener, 141 B, **matched** first try after the click id was cached in a
  local: `click = U_I(0x6f22e4)` gives the original's `cmp ecx,eax`; the same fix applies to 0x5a2de0).
- 0x5a3a40 (the dungeon plan window "locations\kerker\kerkerplan", 219 B, nonmatching 89/224): only the
  increment of the line counter differs (`inc edi; push edi` vs `lea eax,[edi+1]; mov edi,eax; push eax`);
  `++n` as the argument did not change it - a match-session probe (the loop pointer vs index).
- 0x5a3b20 (the dungeon listener, 100 B) and 0x5a3ef0 (the empty listener, 28 B): **matched** first try.
- 0x5a3b90 (the bribe window "locations\kerker\kerker_besteche", 599 B, nonmatching 289/592): the AIMSG is
  built by offsets (+4 type 0x39, +8 the character, +0xc -1, +0x36 9, +0x58 / +0x5c the two ids, +0x60 the
  amount) and sent 0x4c4540 / 0x4c47e0 / 0x4c4b70 / 0x4c4580; the frame (0x910) is not reproduced.
- 0x5a3df0 (the bribe listener, 245 B, nonmatching 155/256): the pump mask depends on the flag byte 0x770d31
  (0x67fff / 0x677ff) and is recomputed at both ends of the loop; the 0x770d31 byte and the class 0x10 lookup.
- 0x5a3f10 (the burglary window "locations\diebesgilde\diebesgilde_einbruch", 542 B, ROUGH): the AIMSG type
  0x3f with the crew at +0x38 (up to 8 records with byte +0x184); the gates 0x553cc0 / 0x509120.
- 0x5a4130 (the burglary start, 158 B, nonmatching 32/160): the request 0x5b2180 (1, the record, the text,
  0x5a3f10, 0) and the record {0, 0x400, 0xa07443b4, 1, 7, 0x727}; only the frame / the record slot order
  differ.
- 0x5a1af0 (Spenden, 1141 B, nonmatching 900/1136): the calls are complete; only the frame / local layout is
  off by 8 bytes (three -1 controls, the 512-byte text buffer) - the register roles of the item loop.
- 0x5a1f70 (Ablass2, 1063 B, nonmatching 821/1072): the price block is the am.c 0x49c8e0 formula
  (`n1 = 0x4f39e0`, `x = (200.0f - 0x50c550(...)) * 0.01f`, `w = n1 * x * 0x50a640(...) * 0.005f`), but the
  church copy uses 0.1 instead of 0.01 before the 3200.0 .. 320000.0 clamp (constant 0x5eb8d8 = 0.1 double);
  the button / frame slots are from the disassembly only.
- 0x5a23a0 (Taufe, 1360 B, nonmatching 1074/1344): the two paths (own building / another owner's religion) are
  written; the missing 16 bytes are in the own path (the item loop +3 .. +7 and the 0x53dda0 loop).
- 0x5a28f0 (Top5, 620 B, **Ghidra start, not missed** - the tail call of 0x5a1f70; nonmatching 497/592): the
  row records of the draw loop (the 0xe-int stride of the frame, 0x5be1d0 / 0x5be720 / 0x5bd1f0) are written
  as arrays; the first two rows come from 0x50b1e0.
- 0x5a2b60 (Beichte, 625 B, nonmatching 529/656): the calls are complete; the frame differs by 31 bytes.
- 0x5a2de0 (the confession listener, 128 B, nonmatching 54/144 - the closest of the session): the original keeps
  param_1 in edi and the constant 0x200 in ebx (ours the other way round) and reloads 0x6f22e4 where ours keeps
  it in eax; a c2prio job for the match session.
- 0x5a2e60 (Predigt, 2138 B, ROUGH SKELETON, nonmatching 1367/1776): the three states (0 no priest, 1 / 2 with
  a priest, 3 closed) and their buttons are written; the 0x65-tick refresh of 0x6333cc, the 0x4c4b70 message
  record of the missing object path and the widget records (0x5be1d0 / 0x5be3b0) are condensed.
- Type debt: the church family shares am.c's helpers - 0x4f39b0 (fame), 0x4f39e0 (rank), 0x50c550 (the relation
  word 0..200, the same call am.c uses), 0x5ba6b0 (the flag 0x2c6 halves the price), 0x5a8b80 / 0x5a8c00 (the
  relation bookkeeping of a building), 0x4c5f60 (a relation delta); am.c has the same code (0x49c8e0 /
  0x49cb60) with the typed views, so unify when this unit is matched.
- The unit's name (`unk_58bbd0.c`) is still open; the string block now runs 0x62b038 .. 0x62bf4c+ in function
  order (no new boundary).

## layout 2026-10-24 (third)
Drafted the frontier 0x5a1af0 .. 0x5a4130 (14 functions; the unit now runs 0x58bbd0 .. 0x5a41d0, 157
functions, 25 matched, contiguous). The stretch is the **church rooms** of the town hall building driver
(0x59e420's room list) and the **thieves' guild / dungeon** listeners:
- 0x5a1af0 (donation "Locations\Kirche\Spenden"), 0x5a1f70 (indulgence "Locations\Kirche\Ablass2"), 0x5a23a0
  (baptism "Locations_Kirche_Taufe"), 0x5a28f0 (top-5 "Locations\Kirche\Top5", a real Ghidra start reached by
  the tail call of 0x5a1f70), 0x5a2b60 (confession "Locations\Kirche\Beichte") with its listener 0x5a2de0
  ("contact_BEICHTE") and 0x5a2e60 (sermon "Locations\Kirche\Predigt").
- The building listeners of the rooms: 0x5a36c0 (the production building: contact_PRODUKTION_SCHREIBEN /
  LAGER / TRANSPORT / ob_PERSONALBUCH / ob_MEISTERBRIEF and their five windows), 0x5a3810 (the church:
  contact_PREDIGT / ob_WEIHWASSERBECKEN / ob_KIRCHE_BEICHTSTUHLRAUM / ob_SPENDENKAESTCHEN and the automatic
  confession), 0x5a39b0 (contact_TOP5 / contact_ABLASS).
- The dungeon: 0x5a3a40 (the plan window "locations\kerker\kerkerplan"), 0x5a3b20 (its listener
  ob_KERKERPLAN), 0x5a3b90 (the bribe window "locations\kerker\kerker_besteche") with 0x5a3df0
  (contact_BESTECHEN, the flag 0x770d31) and 0x5a3ef0 (an empty pump).
- The thieves' guild: 0x5a3f10 (the burglary window "locations\diebesgilde\diebesgilde_einbruch", the crew
  AIMSG type 0x3f) and 0x5a4130 (its start, the request 0x5b2180); the espionage 0x5a41d0 (type 0x40) and its
  start 0x5a44e0, the spy ring window 0x5a45c0; the pickpocket 0x5a48d0 (type 0x49) with its start 0x5a4b30
  and the victim window 0x5a4b90 (type 0x61).
- Matched this session: 0x5a39b0 (after the cached click id), 0x5a3b20, 0x5a3ef0, 0x5a4b30 (after the store
  order of the request fields). Closest nonmatching: 0x5a4130 (32/160), 0x5a2de0 (54/144), 0x5a3a40 (89/224),
  0x5a44e0 (129/224), 0x5a3df0 (155/256), 0x5a36c0 (160/368).
- Boundary evidence: the literals climb 0x62beb8 .. 0x62c064 in function order (one .data block, D8, no new
  boundary); 0x5a2e60 / 0x5a36c0 / 0x5a3810 / 0x5a3a40 / 0x5a3b90 / 0x5a3f10 are real Ghidra functions (the
  earlier ledger lumped some of them into the neighbours' gaps). 0x5a3f10 has no Ghidra procedure: drafted
  from uv run disasm (the REA decompile refuses: "No procedure exists at 0x5a3f10").
- Method: 0x5a1f70's Ghidra decompile dies; its price formula was found again in am.c (0x49c8e0 / 0x49cb60),
  which gave the names of 0x4f39b0 / 0x4f39e0 / 0x50c550 / 0x5ba6b0 / 0x5a8b80.
- Type debt: 0x5a1f70 and 0x5a23a0 take a second (ignored) argument (0x5a3810 / 0x5a39b0 push it); the
  declarations of the room handlers in the 0x59e420 block are `void` now.


## Open work (earlier sessions)
- 0x58bbd0 (393 B): drafted from the Ghidra pseudocode as pointer walks over the tables; nonmatching, the record
  structs (0x58 / 0x38 / 0x1c-byte strides) are not typed yet.
- 0x58bd60 (the production window, 2336 B, ROUGH SKELETON): the open / setup path (0x520fe0, the event 0x576040,
  the kind texts 0x70b .. 0x717, 0x525220, 0x58bbd0, the control 0x475ff0 / 0x52e860) and the frame loop (the
  two 0x4ffe90 queries, the exit with the progress byte param_1[0x59]) are written; the goods list logic (the
  columns 0x6f5320 / 0x6f5354, 0x50ab80 / 0x50aaf0 the amounts, 0x53e400 / 0x53e300 the carriers, 0x576340 the
  confirmation, the 0x3e-dword order record and 0x4c4b70 / 0x4c3ae0, the 0x75f290 / 0x75f2a0 marks, 0x524260)
  and the calls 0x58cc10 / 0x58d720 (their arguments still wrong: 0x58cc10 takes (window, 2, n, record, 0))
  are not written yet.
- 0x58c680 (the column builder, 1421 B), 0x58cc10 (the goods rows, 2827 B), 0x58d720 (the goods list, 1491 B),
  0x58dd00 (the column display, 1058 B), 0x58e130 (the reverse direction table, 769 B), 0x58e440 (the short
  column display, 608 B), 0x58e6a0 (one goods entry, 1936 B), 0x58ee30 (the reverse entry fill, 1117 B) and
  0x58f290 (the third column display, 637 B): all drafted ROUGH from the Ghidra pseudocode (REA batch_decompile,
  no build/ghidra exports), all nonmatching (differences in the hundreds of bytes), no detail work done.
- The next frontier: 0x58f510 (264 B). The unit continues past it (the goods / column family); the boundary to the
  next unit is not looked for yet.

## layout 2026-10-23 (session 21)
Opened the unit from the frontier: 0x589f10 turned out to be the network lobby (a unit of its own), and the
production window follows at 0x58bbd0 / 0x58bd60. Drafted 0x58bbd0 and then the whole stretch 0x58bd60 ..
0x58f290 from the REA pseudocode (all ROUGH, the tables 0x6f4f08 .. 0x6f5a64 walked as int arrays with the
column / row / entry strides 0x58 / 0x38 / 0x1c found in the pseudocode). Nothing is matched; the frontier is
0x58f510.

## layout 2026-10-24 (second)
Drafted 0x599af0 .. 0x5a1a60 (51 functions; the unit now runs 0x58bbd0 .. 0x5a1af0, 143 functions, 16 matched,
contiguous: no undrafted bytes inside). The stretch is the tavern / locations contacts continued, the money
lender family and the Rathaus / church / guild rooms:
- 0x599af0 (the tavern listener: "contact_WUERFELSPIEL" / "contact_STAMMTISCH" / "ob_DUNKLE_ECKE", the AIMSG
  0x74), 0x599cb0 (the production listener "contact_PRODUKTION_BRAUEN"), 0x599e00 / 0x599ef0 (the city info
  window "Handel\Handel_Stadtinfo" with the object "ob_KARTENTISCH", the 4-column good rows and the progress
  bar of the control +0x394), 0x59a2c0 / 0x59a3c0 (the three city points 0x7e4234 + i * 0x2f4, the object names
  "stadt_%s", the town map screen), 0x59a560 (the fair hall "ob_KARTENTISCH_germ"), 0x59a710 (leaving the
  market: the ids 0x92 .. 0x2db), 0x59a800 (the market stand listener, the Nuernberg market flecks), 0x59aae0 /
  0x59ad00 (the wine cellar "locations\wohnsitz\weinkeller"), 0x59ad60 (the new credit window) and 0x59b030 (a
  credit payment), 0x59b160 / 0x59b630 (the credit lists of the two sides), 0x59b880 (the credit release),
  0x59bad0 (the credit giver with the city grouping), 0x59be90 / 0x59c120 / 0x59c450 (the exchange row lists and
  their name sort), 0x59c6b0 / 0x59c6e0 (the click / count helpers), 0x59c700 (the exchange window "..Wechsel"),
  0x59d2d0 (the exchange fees), 0x59d410 (the money messenger), 0x59d5e0 (the exchange with the city list),
  0x59dc00 (the Ghidra-missed callback of "geldleihe_info"; found by the ledger after the first build of the
  stretch), 0x59df10 (the fortune overview request 0x5c1130, callback 0x59dc00), 0x59df80 (the money lender
  listener), 0x59e0a0 (the room switch "RaumWechseln_s"), 0x59e170 (the opening hours table 0x62afa0), 0x59e210
  (the enter check), 0x59e420 (the building driver: the room dispatch switch 0x13 .. 0x29e, which names the
  handlers of the following rooms) and 0x59ee40 / 0x59f410 (the other building drivers of the own / kind 0x1c
  buildings), 0x59f9a0 (the guild hall), 0x59fbf0 / 0x5a0950 (the guild cash and its payment window), 0x59fca0
  (the town cash office listener), 0x59fd30 (the Rathaus contact listener), 0x59fe40 (the citizen right
  window), 0x5a0220 / 0x5a0590 (the office application and its town list), 0x5a07d0 (the law book listener),
  0x5a0f10 (the office application entry) with its Ghidra-missed callback 0x5a0ca0, 0x5a1000 (the guild level
  1 window with the 7 icon buttons), 0x5a13f0 / 0x5a1530 / 0x5a1670 (the three Ghidra-missed callbacks of the
  guild level 3 requests), 0x5a17b0 (the guild level 2 / 3 entry) and the listeners 0x5a18d0 / 0x5a1960 /
  0x5a1a60.
- Matched this session: 0x59ad00, 0x59c6e0, 0x59fbf0, 0x59fca0, 0x59fd30, 0x5a18d0, 0x5a1a60 (0x59fbf0 after
  the `(int (*)(int, unsigned char))` cast: the original pushes the bare byte, the K&R declaration
  zero-extends).
- ROUGH SKELETONs still to fill: 0x599ef0 (the window path), 0x59a3c0, 0x59b160, 0x59b880, 0x59bad0, 0x59be90
  (the name sort), 0x59c120, 0x59c450, 0x59c700 (the exchange), 0x59d5e0, 0x59dc00, 0x59e420 (the device
  callbacks), 0x59ee40 / 0x59f410 (the view / device bookkeeping), 0x59fe40 (the citizenship messages),
  0x5a0220 (the applicant positions), 0x5a0590, 0x5a0950, 0x5a1000 (the icon positions), 0x5a23a0 (not
  drafted yet), 0x5a17b0 / 0x5a1960 near-misses (0x5a1960 50/272: the pump pushes 0x5a18d0 in the original).
- Ghidra-missed starts in the stretch (all found by `uv run ledger` undrafted after a build): 0x59dc00 (771 B),
  0x5a0ca0 (619 B), 0x5a13f0 / 0x5a1530 / 0x5a1670 (313 / 308 / 308 B, the callbacks of 0x5a17b0). 0x5a1f70
  (2144 B) makes Ghidra's decompiler die ("Decompiler process died") - read it from uv run disasm.
- The room dispatch switch of 0x59e420 gives the room -> handler map: 0x13 0x597500, 0x14 0x596400, 0x15
  0x5965f0, 0x16 0x59ad00, 0x17 0x5966f0, 0x18 0x596f90, 0x1f 0x59a560, 0x4d 0x5a4f50, 0x54 0x5a4d40, 0x5c
  0x5a4f70, 0x60 0x5a5080, 0x65 0x5a5bc0, 0x74 0x595170, 0x85 0x595330, 0x9b 0x5962b0, 0xa5 0x59df80,
  0xb7 0x5a3b20, 0xba 0x5a3df0, 0xbb 0x5a3ef0, 0xcf 0x596120, 0xd6 0x596060, 0xdd 0x596080, 0xe5 / 0xe6
  0x5a3810, 0xf0 0x5a39b0, 0xf2 0x5a2de0, 0xf7 0x5a36c0, 0x10e 0x59fd30, 0x110 0x5a07d0, 0x114 0x59fca0,
  0x118 0x595ed0, 0x120 0x599af0, 0x12e 0x599cb0, 0x137 0x5a18d0, 0x138 0x5a1a60, 0x139 0x5a1960, 0x145
  0x5a6a90, 0x146 0x5a67a0, 0x14c 0x5a6920, 0x24b 0x5821d0, 0x256 0x582560, 0x266 0x582600, 0x29e 0x5d8420,
  default 0x58f620.
- Data: the opening hours table 0x62afa0 (dword kind, open byte +4, close byte +5, 10 entries, 0-terminated),
  the name table 0x143c860 (char * per building kind), the city factor words 0x7e3f94 + city * 0x2f4, the city
  names 0x7e3f40 + city * 0x2f4, the object / character tables as before.
- Type debt: see notes/layout.md (the third ignored argument of 0x5085b0, the unused second argument of
  0x599ef0, the object name at D3OBJECT +0, the AIMSG offsets of the new listeners).

## layout 2026-10-24
Drafted the frontier 0x58f510 .. 0x599aa0 (the unit now runs 0x58bbd0 .. 0x599af0, 89 functions, 13 matched).
The stretch is the GUI module around the "Handel\Handel_PRODUKTION" screen continued:
- 0x58f510 .. 0x58fb60: the purchase contacts ("contact_EINKAUF*", 0x58f510), the frame pump 0x58f620, the
  building-name lookup 0x58f640 (the 0x62ab08 / 0x62aae8 table), the placement-candidate filter 0x58f6a0
  (a Ghidra-missed start), the collectors 0x58f7a0 / 0x58f920 / 0x58f980 / 0x58fa20 / 0x58fb60.
- 0x58fba0 .. 0x58feb0: the "bk_" site finder 0x58fba0 (Ghidra-missed), the building site 0x58fc40
  ("lc_GetGebaeudeBauplatz(): FindPos->Pos:%f  %f  %f"), the vector helper 0x58fd40 / 0x58fd90, the nearest
  object 0x58fe10 and the drop 0x58feb0.
- 0x58ff00 / 0x5909a0: the building placement (2709 B) and the "BAUEN_GEB_BAUEN" window (1988 B), ROUGH
  SKELETONs (the interactive loop of 0x58ff00 and the selection loop of 0x5909a0 are comments).
- 0x591170 .. 0x593960: the game frame callback 0x591520 and the tech tree 0x591bf0 (ROUGH), the building
  listeners 0x591170 / 0x5912c0 / 0x591ad0, the trade search windows 0x5923c0 / 0x592b90 / 0x593150 (type 4),
  and the market / kontor listeners 0x5938c0 / 0x593960.
- 0x593b00 .. 0x595a00: the criminal and kontor listeners (0x593b00 / 0x593ca0 / 0x593e80), the burglar guild
  0x593f40 (Ghidra-missed) / 0x594180 / 0x5941c0 (Ghidra-missed), the bribe windows 0x5944e0 (Ghidra-missed) /
  0x594790 / 0x594810 (Ghidra-missed) / 0x594bb0 (Ghidra-missed), the criminal contact listener 0x594e50, the
  trade listener 0x5950e0 and the "HANDEL\HANDEL_SUCHEN" windows 0x595480 / 0x595a00 (ROUGH).
- 0x595ed0 .. 0x599aa0: the Wirtshaus / locations module: the contact listeners (0x595ed0 / 0x5962b0 /
  0x596060 / 0x596080 / 0x596120 / 0x596400 / 0x5965f0 / 0x5966f0 / 0x596f90 / 0x597500), the windows
  0x5967c0 / 0x596ba0 (mistress), 0x596ff0 (master exam), 0x597380, 0x5973f0 (wine cellar), 0x5976a0 (rules),
   the dice 0x597780 / 0x597810 / 0x597920 / 0x597a10 / 0x597e70, the tavern 0x598170 / 0x598b80 / 0x598dc0 /
  0x599240 and the dark corner 0x5992c0 / 0x5996b0 / 0x599aa0 (ROUGH skeletons).
- The literals climb 0x62b038 .. 0x62b83c in function order (one .data block, data order D8: the strings
  "contact_*" / "ob_*" / "locations\*" / "HANDEL\HANDEL_SUCHEN" / "BAUEN_GEB_BAUEN"), so no new boundary
  inside the stretch.
- Matched first try: 0x58f620, 0x58f920, 0x58fb60, 0x58feb0, 0x592230, 0x594180, 0x594470, 0x594790,
  0x594b10, 0x594db0, 0x596060, 0x596080, 0x596f90.
- Near-misses (for the match sessions): 0x58f640 (29/96: the inlined strcpy shape), 0x58f510 (211/288: the
  chained else-if ladder), 0x58fba0 (88/160), 0x58fc40 (184/224), 0x58fd40 / 0x58fd90 (the vector helpers),
  0x58fe10 (33/160), 0x5938c0 (34/160: the two contact fetches), 0x596120 (177/432), 0x5962b0 (139/368),
  0x5965f0 (63/256), 0x5966f0 (56/208), 0x597380 (74/112), 0x599240 (60/144).
- ROUGH SKELETONs still to fill: 0x58ff00, 0x5909a0, 0x591520, 0x591bf0, 0x5923c0, 0x592b90, 0x593150,
  0x593f40, 0x5941c0, 0x5944e0, 0x594810, 0x594bb0, 0x595480, 0x595a00, 0x5967c0, 0x596ba0, 0x596ff0,
  0x597a10, 0x597e70, 0x598170, 0x598b80, 0x598dc0, 0x5992c0, 0x5996b0.
- Missing Ghidra starts drafted from the disassembly (address-taken callbacks): 0x58f6a0, 0x58fba0, 0x593f40,
  0x5944e0, 0x594810, 0x594bb0.
- Type debt: the listeners pass a record / window pair (param_1 = the building or character record, param_2 =
  the message record); 0x4731d0 (x, y, id, screen) has 4 arguments; 0x4ffe90 / 0x4ffcc0 are the record walk;
  0x5e56c4 (n, buffer, base) formats; the message records are built by byte offsets (type at +4, the args at
  +0x58), consistent with ai.h's AIMSG.
- The build/ghidra exports are missing for this whole stretch: the pseudocode came from REA batch_decompile
  (inline results, not exported to build/ghidra; the rea_export JSON was not saved).

## layout 2026-10-24 (fourth)
Drafted 0x5a4d40 .. 0x5a6c40 (23 functions; the unit now runs 0x58bbd0 .. 0x5a6cb0, 189 functions, 33 byte,
34 behaviour, contiguous: no undrafted bytes inside). The stretch has two subsystems:
- The **thieves' guild kidnapping** (0x5a4d40 .. 0x5a5bc0, room 0x54 of the driver 0x59e420 and the rooms it
  opens): the guild listener 0x5a4d40 (contact_EINBRUCH / contact_GEBAEUDE_AUSSPIONIEREN / contact_ANGRIFF
  only with the crew byte +0x247 of the kind record >= 2 / contact_TASCHENDIEBSTAHL / ob_INFORMATIONSPERGAMENT
  under the mask 0x200, ob_PERSONALBUCH / ob_MEISTERBRIEF under 0x400, and the great sword "ub_GROSSSCHWERT"
  shown by 0x447300), the empty room 0x5a4f50, the smithy listener 0x5a4f70 (LAGER -> 0x5b26b0 with the record
  0x6f2310 +0x14, TRANSPORT, AUSRUESTEN, REGENERATION) and 0x5a5080 (ZIELSCHEIBE / NACHT), the kidnapping
  window 0x5a5120 ("locations\diebesgilde\diebesgilde_entfuehrung", the crew scan of the +0x184 characters,
  the AIMSG 0x3c with the up-to-eight crew ids), its start 0x5a53e0 (types 0x3c / 0x3d records, the request
  0xa7fdfbb6), the ransom 0x5a5530 (the relation 0x50a640 and the random roll
  min((rand * 0.5 + 0.75) * 1600000.0, the relation) * the share factor 0.1 / 0.06 / 0.04 / 0.02 by the byte
  0x1b1; the three controls send 0x3e / release / show the texts 0x19c5 .. 0x19c7), the attempt 0x5a5910 (the
  chance (0x504ec0 (4) + (3) + (2)) * 0.2f + 0.15f against a 0x42ec20 roll) and the listener 0x5a5bc0
  (contact_ENTFUEHREN / contact_LOESEGELD / contact_AUSBRECHEN, the cell 0x184 / 0x4fbb80 / 0x4c51b0).
- The **city watch** ("LOCATIONS\STADTWACHE\STANDARD", 0x5a5d70 .. 0x5a6c40): the four request starts
  0x5a5f60 (contact_PATROL) / 0x5a61f0 (contact_RAZZIA, the record 0x90010 / 0x727) / 0x5a6480 (ob_WACHPLAN) /
  0x5a6720 (the customs box, 0xa0744300), their callbacks (0x5a5d70 the plain window with four crew ids in
  the AIMSG 0x43, 0x5a5fd0 the six-slot AIMSG 0x44 with the "above six" text 0x19ef, 0x5a6270 the search
  window AIMSG 0x65 with the name string and the wanted list 0x7603a0, 0x5a64d0 the arrest window AIMSG 0x64
  with the gate byte +0x5b & 8) and the listeners 0x5a67a0 (contact_PATROL / ob_WACHPLAN / ob_PERSONALBUCH /
  ob_MEISTERBRIEF / the customs box 0x147 / 0x148), 0x5a6920 (six contacts) and 0x5a6a90 (ZIELSCHEIBE /
  NACHT / REGENERATION), plus the text helper 0x5a6b50 ("%1G") and the map helpers 0x5a6b70 (the 5.33f map
  projection) / 0x5a6c40 (four controls 0x4731d0 / 0x472f50).
- Boundary: the literals climb 0x62c0d4 .. 0x62c270 in function order (the kidnapping contacts 0x62c0d4 ..
  0x62c1d8, then the city watch "LOCATIONS\STADTWACHE\STANDARD" 0x62c1d8 and its contacts / texts 0x62c19c ..
  0x62c270), one .data block, so the city watch is the same TU as the guild / kidnapping code (D8, no
  boundary).
- Method: the whole stretch was missing from build/ghidra; REA batch_decompile + uv run rea_export were used
  (the exports land in the main checkout's build/ghidra), the Ghidra misses (0x5a5120, 0x5a5d70, 0x5a5fd0,
  0x5a6270, 0x5a64d0) were read from uv run disasm; the function starts came from a 0x90-run scan of .text
  (0x5a5120 .. 0x5a7bc0) plus the ledger after each build (which caught 0x5a6b50, a 24 B helper between
  0x5a6a90 and 0x5a6b70 that the scan missed).
- Note: the pre-existing names.csv record of 0x4a13e0 (am.c, "behaviour") does not build to behaviour any more
  (the ai_Unk503ee0 -> &x change of commit 257bad90): `uv run ledger check` lowers it to draft. Not
  this unit and not this session's change; the record was left as at HEAD and the case reported to the
  orchestrator (the am.c match session owns it).

## layout 2026-10-24 (fifth)
Drafted 0x5a6cb0 .. 0x5a8d80 (28 functions; the unit now runs 0x58bbd0 .. 0x5a8e10, 217 functions, 16 matched
this session). The stretch is two modules that one .data block joins:
- The **town map window** (0x5a6cb0 .. 0x5a82f0): the per-frame rebuild 0x5a6cb0 (3049 B, ROUGH: the 0x100 x
  6-dword entry array at +0xdc, the buildings at the kind direction scaled by the scene cell 0xfd7c50 + 0x20
  * 5.33f and 0x5eed90 / 0x5eed8c, the carried objects projected by 0x458a20 / 0x45f170 / 0x5a6b70, the
  picture 0x4731d0 = the kind letter + 0x47f, the name table 0x7e3f40 + 0x65 * 0x2f4, the eight
  "sp_AUFLAUERLEGEN_*" spots 0x625720 (flag 4) and the effect list 0x6f26b0 (flag 8), the view centred on
  0x6f2308 / 0x6f230c / 0xfc2dec and 0x46f840), the open 0x5a78a0 ("Misc\Landkarte2" 0x62c274, the control bar
  0x5a6c40, the title 0x515960), the drag scroll 0x5a79e0 and key scroll 0x5a7bc0 (the scroll +0x258 / +0x248
  of the screen record 0xf8f120 + screen * 0x3c4, clamped 0x6f4ee8 - 0x200 / 0x6f4ee4 - 0x168), the per-frame
  0x5a7cd0 (1556 B, ROUGH SKELETON: the hover 0x5890c0, the object list 0x1468ae0 .. 0x14696df, the camera
  projection of the scene centre +0 / +8 and scale +0x10 / +0x18, the carts 0x50a050 and the pictures 0x46cd80
  / 0x46e710, the arrows 0x472760) and the close 0x5a82f0 (the 0x300 controls +0x18dc).
- The **production settings** of the map (0x5a8340 .. 0x5a8cb0): the linked list 0x6f26c0 of 0x4c-byte
  records {the good at +0 (the callers pass it at the odd +1 of their block), the seed +4, the rotation byte
  +8, seven {good, weight, lock} slots at +0xc (stride 8), the factor +0x44, next +0x48}; the lookup
  0x5a84b0 / 0x5a8540, the create 0x5a8560 ("hp_AddProdSetting" 0x62c284), the remove 0x5a86c0, the weight
  adjust 0x5a8710, the refresh 0x5a8850, the weighted pick 0x5a8870 (0x5a8340 (the LCG) * 100.0f
  0x5ebb40), the factor get / set 0x5a88e0 / 0x5a8900, the free 0x5a8920, the save / load 0x5a8940 /
  0x5a8a20 (0x430e30 / 0x51c920, "hp_LoadProdSetting" 0x62c298) and the weights of a good 0x5a83b0 with
  0x5a84d0 (the character blocks 0x5eed94 / 0x61a348).
- The production messages 0x5a8b80 / 0x5a8c00 / 0x5a8c50 / 0x5a8cb0 / 0x5a8d40 / 0x5a8d80 (the texts
  0x20b6 .. 0x20c0 into the buffer 0x6f26c8 and 0x53dda0 with 0x5ec / 0x5fb / 0x5fd / 0x5ff): they send the
  character id at +4 and are gated by the character kind +2 (6 / 7).
- Boundary: the literals climb 0x62c270 (5.33f) / 0x62c274 ("Misc\Landkarte2") / 0x62c284 / 0x62c298 (the
  "hp_" tags) in function order after the city watch texts, one .data block (D8), so the map window and the
  production settings are one TU and no boundary was found inside 0x5a6cb0 .. 0x5a8e10.
- Method: REA batch_decompile for 0x5a6cb0 .. 0x5a8d40 (the exports land in the main checkout build/ghidra);
  0x5a8d80 is a Ghidra-missed start, read from uv run disasm. The REA pseudocode of 0x5a6cb0 / 0x5a7cd0 loses
  the __ftol arguments; the projections were decoded from the disassembly.
- Byte lessons: `0x75 <= x` and `0x74 < x` are the same test but different bytes (0x5a8b80, `setge` vs
  `setg`); the original of 0x5a8d80 really tests `kind == 6 || kind != 7` (the redundant compare is in the
  bytes; the plain `== 6 || == 7` gives a `jne`).
- Type debt: the entry array {id, object, x, y, float map x, float map y} at +0xdc (0x100 x 0x18); the screen
  record 0xf8f120 + screen * 0x3c4 (the half 0x8 / 0xa, the scroll +0x248 / +0x258, the field 0 +0x38c) and
  the control records 0xf8f080 + id * 0x2ec (the visible 0x4c, the picture 0x1bc / 0x1c0, the rectangle
  0x1e .. 0x24, the user pointer 0x2e8, the record 0x40); the settings record above; the character record
  0x770b80 + index * 0x21c (the kind +2, the +0x54 id); 0x458a20 / 0x45f170 keep the rt.h views.
- No new unit / header: the work stays in unk_58bbd0.c; the @boundary stays plausible (the start 0x58bbd0
  rests on the call / table coupling, see the Unit section).
- Continuation of the fifth session: drafted 0x5a8e10 .. 0x5a9470 (5 functions; the unit runs
  0x58bbd0 .. 0x5a9690, 229 functions, 21 matched this session): the plague / gypsy catastrophe messages
  0x5a8e10 (three messages, the character loop), 0x5a8ee0 / 0x5a8f90 / 0x5a9050 (the city / good texts of a
  character pair through 0x42ec00 (3) and 0x5176d0), 0x5a9110 / 0x5a9170, 0x5a91c0 (the plague text
  0x62c2c0), 0x5a9240 / 0x5a92b0 and 0x5a9330 / 0x5a9430 / 0x5a9470 (the "NACHRICHTEN_ZIGEUNER" keys
  0x62c2f4 / 0x62c310, 0x144a8a8 / 0x144a8b4 / 0x144a8b8). Matched: 0x5a8e10, 0x5a9110, 0x5a9170, 0x5a9330,
  0x5a9430. Near-misses: 0x5a8ee0 (82/160), 0x5a8f90 (103/176), 0x5a9050 (103/176), 0x5a91c0 (95/128),
  0x5a9240 (106/128), 0x5a92b0 (91/128), 0x5a9470 (474/544 ROUGH). The .data order keeps climbing (0x62c2ac ..
  0x62c310), so the catastrophe messages are the same TU.
- End of the fifth session: drafted 0x5a9690 .. 0x5a9af0 (10 functions; the unit runs 0x58bbd0 .. 0x5a9b50,
  239 functions, 26 matched this session): the graveyard / Friedhof texts over the 0x5eecd0 table with the
  0x5190a0 order used by 0x5a96e0 / 0x5a97e0 / 0x5a9880, and 0x5a9690 / 0x5a9780 / 0x5a9910 / 0x5a9990 /
  0x5a99e0 / 0x5a9a30 / 0x5a9af0 (0x42ec00 (3) text picks, the keys 0x62c32c .. 0x62c39c). Matched: 0x5a9690,
  0x5a9780, 0x5a9990, 0x5a99e0, 0x5a9af0. Near-misses: 0x5a96e0 (147/176), 0x5a97e0 (147/176), 0x5a9880
  (131/160), 0x5a9910 (108/128), 0x5a9a30 (100/192). The .data block climbs to 0x62c39c, so the Friedhof texts
  are the same TU. Session totals: 2455 -> 2481 byte matches; 26 of them in this unit.

## layout 2026-10-24 (worktree, sixth)
- Frontier 0x5a9b50 -> 0x5ae6b0, 24 functions; the unit is now contiguous 0x58bbd0 .. 0x5ae870 (263 functions,
  65 byte; session 2481 -> 2487 byte matches, all 6 in this unit). The next frontier is 0x5ae870 (100 B).
- Drafted: the Domizil message family 0x5a9b50 / 0x5a9bc0 / 0x5a9c30 / 0x5a9ca0 / 0x5a9d00 / 0x5a9d60 /
  0x5a9dd0 / 0x5a9e40 (the kinds 6 / 7 test at +2, the texts 0x4070 / 0x4074 .. 0x407f, 0x53dee0 with the
  "NACHRICHTEN_DOMIZIL_01 / 03 / 04 / 12" keys 0x62c3b8 .. 0x62c400); the table pickers 0x5a9e90 / 0x5a9f10
  (the table 0x62c420, 0x42 entries of 0x1c bytes, the count 0x62cb58, the +0x18 word as a range key); the two
  percent message boxes 0x5a9f80 / 0x5ad000 ("misc\Messagebox", the relation 0x509e60 clamped at 0x12c0 / 32);
  the Domizil window family: the child builder 0x5aa0d0, the info panel 0x5aa1f0 (the building kind
  0x7e3f30 + kind * 0x24d, the opinion 0x50c520 in percent (x + 128) * 100 / 255), the control destructor
  0x5aa4b0, the selected entry 0x5aa520, the worker list 0x5aa700 / 0x5aa9a0 (the array 0x6f40a4, the count
  0x6f40a0, the controls 0x6f40b0 / 0x6f40bc, the master name 0x501190 (1, 1, +0xb4), the free one 0x50a0f0);
  the transport block: the Personnel book 0x5aab80, the mercenary book 0x5ac110 (both ROUGH stubs), the
  transport window 0x5ad150 (ROUGH stub), the "Handel\Handel_Transport_neuer_Kauf" purchase 0x5ae240 (the list
  0x46c490 of the three controls 0xd8, the quantity 0x5087f0, the text 0xd9, the payment 0x4c4830 / 0x506430 /
  the order 0x4c4880), the confirmation 0x5ae570 (the text 0xdc, 0x5034b0, 0x4c47e0) and the order 0x5ae6b0
  (the carrier walk 0x53e400 (2, 3, the +2 field, 0, 0x11), 0x576340 (0x24), the AIMSG type 0xf with the clock
  0x770ae0 .. +0x36 and the +0x58 / +0x5c / +0x60 fields, 0x50aa00 / 0x50a970 the amount, 0x4c4830).
- Matched first try (6): 0x5a9b50, 0x5a9bc0, 0x5a9d00, 0x5a9d60, 0x5a9dd0 (the Domizil message shapes) and
  0x5aa4b0 (the control destructor). The 0x53dee0 call order is (id, -1, buffer, flag, 0x5ec, the master name
  / -1, the text id / the +2 field, 0, the key).
- Near-misses (not chased, no time): 0x5a9c30 (65/96, first at +0xf), 0x5a9ca0 (65/96, +0x0), 0x5a9e40
  (66/80, +0x0), 0x5a9e90 (111/128), 0x5a9f10 (115/128), 0x5a9f80 (259/336, +0x1), 0x5aa0d0 (203/272, +0x1f),
  0x5aa1f0 (516/656, +0x1a), 0x5aa520 (355/448), 0x5aa700 (512/608), 0x5aa9a0 (339/400), 0x5ad000 (259/336,
  +0x1), 0x5ae240 (662/848, +0xf), 0x5ae570 (226/272), 0x5ae6b0 (349/416). 0x5a9b50 / 0x5a9bc0 matched at
  112 B while the original spans are 108 B (the compared symbol size includes the trailing nops).
- ROUGH stubs: 0x5aab80 (5328 B), 0x5ac110 (3680 B) and 0x5ad150 (4158 B) are stubs with the structure in their
  comments only. The Ghidra pseudocode of all three loses the call arguments (frames 0x1a50 / 0x1110 / 0x136c);
  a full draft needs uv run disasm. Their common shape: the record param_1, the kind record
  0x7e3f30 + *param_1 * 0x24d, the entry array r + 0x22c (0xc entries of 0x5c bytes, the count +0x231 +
  +0x232, up to three rows), 0x5aa0d0 per entry, the worker list 0x5aa700, the pump 0x5309a0 and the click
  dispatch on 0x5fe710 with the AIMSG messages 6 / 7 / 0x13 / 0x31 / 0x32.
- VC6 pathology: compiling a first ROUGH draft of 0x5aab80 with its full statement list (a >4 KB frame, the
  0x114-dword array copies and ~150 calls) made cl.exe spin for more than 10 CPU minutes without finishing (the
  build had to be killed). The stub compiles in seconds. Write big functions as a small stub first, then grow
  them in pieces, each built.
- New typed facts: 0x50c520 is `int (unsigned short, unsigned short)` (ai.h; the K&R declaration in this file
  was changed to the ai.h prototype to keep the cross-TU lint clean); 0x50a0f0 returns `char *`; the Domizil
  table entry is 0x1c bytes with the id at +0 and a range word at +0x18; the 0x53e400 carrier walk takes
  (2, 0, param_3, 3, the +1 field) in 0x5aa520 and (2, 3, the +2 field, 0, 0x11) in 0x5ae6b0; the character id
  0x770b84 is indexed by 0x6f25b8 * 0x21c; the percent slider is (opinion + 128) * 100 * (1/255) (the doubles
  0x5eedd8 = 128.0, 0x5eba38 = 100.0, 0x5ee0e0 = 1/255).
- Boundary: no new literal breaks the data order (the Domizil keys climb 0x62c3b8 .. 0x62c400, the table
  0x62c420 .. 0x62cb58 follows, then the pooled "arbeiter_MANN" 0x624ec4 / "handwerker_MANN" 0x624fcc /
  "handwerker2_MANN" 0x62cbb4 / "handwerker3_MANN" 0x60d38c); one .data block, D8, no boundary inside
  0x5a9b50 .. 0x5ae6b0. The @boundary of the unit stays plausible.
- Method: REA batch_decompile for 0x5a9f80 / 0x5aa0d0 / 0x5aa1f0 and 0x5aa4b0 .. 0x5ae6b0 (the pseudocode was
  inline; the exports of the stretch are NOT in build/ghidra yet), uv run disasm for 0x5a9b50 .. 0x5a9e40.

## layout 2026-10-24 (worktree, seventh)
- Frontier 0x5ae870 -> 0x5b2530, 11 functions; the unit is now contiguous 0x58bbd0 .. 0x5b26b0 (274 functions,
  65 byte; session 2487 -> 2491 byte matches, 4 of them in this unit).
- Matched first try: 0x5ae870 (the transport order of one entry: the carrier walk 0x53e400 (1, 0, 0xf), the
  +0xac id match against the word +2, the clock and 0x4c4c60 (-1, the carrier)) and the three forwarders of
  0x5af1d0: 0x5af1a0 (mode 1), 0x5af1b0 (mode 2) and 0x5af1c0 (mode 4); they are called from everywhere (the
  main.c module init 0x53bb20, the locations contacts, the tavern and map code), so they are the common
  "start the thing" entry of the module.
- Byte lesson (new, worth keeping): the 14-byte game clock is ONE struct assignment from the named extern -
  `*(HETIME *)(carrier + 0x52) = ai_Time;` with a TU-local `#pragma pack(push, 2) struct {int; unsigned short;
  int; int;}` and `extern HETIME ai_Time;` (he.c's view) - and it matches (the original: `lea edx,[eax+0x52]`
  then `mov ecx, ds:0x770ae0 / [edx] / [edx+4] / [edx+8]` and the word to [edx+0xc]). The field-by-field
  `U_I(carrier + 0x52) = U_I(0x770ae0)` form does NOT (it stores through [eax+0x52] and loses the base
  register; 44/112 differed) and neither does a pointer local (`clk[0] = ...`: VC6 folds it back).
- Drafted nonmatching (not close, no chase): 0x5ae8e0 (a transport / owner check, 192 B: d3_PointToWorld +
  d3_VecEqual within 1350.0f, then the id chain 0x4fea50 (character, building, object) from the +6 field;
  182/208 - the original caches BOTH parameters in edi / ebx and reloads them from the parameter slots, ours
  caches one and reuses both slots, a register-allocation difference in the prologue), 0x5af010 (the total
  amount of a transport record, 400 B, 336/400: the original keeps `sum` in a frame slot and `n` in the
  param_1 slot, ours the other way round, and the original converts n with `fild` where ours merges with
  `fimul`; the `(float)n` cast does not change it), 0x5b2090 (the player's screen image: sprintf 0x62cce8 /
  0xf8ef80 / the names 0x7e3f40 + player * 0x2f4, the picture 0x478b50 / 0x478cc0 / 0x478bd0 / 0x479020 /
  0x4790e0 / 0x478fc0, the scale 0x62c270 = 7.5 / 5.33 below the width 0x780; the local layout differs) and
  0x5b2530 (a two-choice window over the kind bytes +0x240 / +0x241 / +0x242, the texts 0x10b .. 0x111 and the
  pump 0x5309a0).
- ROUGH drafts: 0x5ae9a0 (1648 B, the transport from the warehouse: 0x4ffe90 (0, 2, 7, 1, param_1), the
  message 0x4c3b30 / 0x4c3b40 / 0x4c3b50, the two carrier tables, the events 0x576340 / 0x576040 and the order
  0x4c4880; the FPU / __ftol details are approximations), 0x5af1d0 (11968 B, the transport window - a STUB: a
  single function, one ret at 0x5b2088, frame 0x1848 via __chkstk, the 0x2d-dword table at +0x15c cleared;
  it calls the transport helpers of this unit (0x5ae870 / 0x5ae6b0 / 0x5ae8e0 / 0x5ae9a0) and the transport
  windows 0x58d720 / 0x58dd00 / 0x58ee30 / 0x58f290; the Ghidra export build/ghidra/5af1d0.c is 1354 lines
  with the parameters lost, so the real draft needs the disassembly; VC6 must be grown in pieces, see
  notes/vc6-matching.md) and 0x5b2180 (944 B, the run callback of the map window: the 0x14-dword request copy,
  the name 0x631090, 0x5a78a0 / 0x5a6cb0 / 0x5a7cd0 and the pump loop).
- Boundary: no new literal run. The literals after 0x5af1d0 climb in function order - "Handel\Handel_
  Transport_neuer_Kauf" 0x62cc6c (0x5ae240), "ob_TransportFromLager..." 0x62cc94 (0x5ae9a0), the format
  0x62cce8 (0x5b2090), the window "0x62ccfc" (0x5b2530), 0x62cd1c (0x5b26b0) - one .data block (D8), so no
  boundary at 0x5af1d0 / 0x5b2090. `uv run ledger data-order 0x5b2090` shows a +0xa7d jump because the Domizil
  table 0x62c420 .. 0x62cb58 and the transport strings are not located data symbols; the code order is
  monotonic, which is the stronger evidence.
- Method: REA batch_decompile for 0x5ae9a0 / 0x5af010 / 0x5af1d0 (the exports are in the main checkout's
  build/ghidra), uv run disasm for everything else (0x5ae870, 0x5ae8e0, 0x5af1a0 .. 0x5af1c0, 0x5b2090,
  0x5b2180, 0x5b2530).
- Continued: 0x5b26b0 (2688 B, ROUGH stub), then **three Ghidra-missed starts** found by the ret + nop scan
  inside Ghidra's 0x5b3130 record: 0x5b3630 (after the ret at 0x5b3626 and nine nops; ends with the ret at
  0x5b3acf; 1184 B, frame 0x7c4), 0x5b3ad0 (the ret at 0x5b3acf follows with NO nop padding; it is an
  address-taken pump callback, `0x5309a0 (0x5b3ad0, 0x677c7)` at 0x5b3cf5; ends at the ret 0x5b3f2c; 1117 B,
  frame 0x510) and 0x5b3f30 (after the ret at 0x5b3f2c and three nops; 1035 B, frame 0x744). 0x5b3130 itself is
  1272 B (the ret at 0x5b3626), so Ghidra merged FOUR functions into the 0x5b3130 .. 0x5b4340 range (the
  functions.csv start 0x5b4340 was already known). All four are drafted as ROUGH stubs. The ret+nop scan alone
  found only two of them: a start right after a ret with no padding needs the ledger's reference scan too. The
  unit now runs 0x58bbd0 .. 0x5b4340 (278 functions).
- Session end: 0x5b26b0 .. 0x5bc9c0 (99 functions in all this session; the unit now runs 0x58bbd0 .. 0x5bcc70,
  327 functions, 73 byte, contiguous; session 2487 -> 2495 byte matches, 8 of them in this unit). Matched:
  0x5ae870, 0x5af1a0 / 0x5af1b0 / 0x5af1c0, 0x5ba600, 0x5ba620, 0x5ba660, 0x5bc0f0.
- Four more **Ghidra-missed starts** inside the merged records: 0x5b3630 (ret 0x5b3626 + 9 nops), 0x5b3ad0
  (ret 0x5b3acf and the next function with NO padding; only the address-taken reference at 0x5b3cf5 -
  `0x5309a0 (0x5b3ad0, 0x677c7)` - and the ledger give it away), 0x5b3f30 (ret 0x5b3f2c + 3 nops), 0x5b4820
  (0x5b4590 ends with the tail call `jmp 0x52d7a0` at 0x5b480c + 15 nops - a ret scan cannot see it), 0x5b4c60
  (the same tail-call end at 0x5b4c54 + 7 nops; the callback that 0x5b4fb0 pushes) and 0x5b5310 (ret 0x5b530b
  + 4 nops, missing from functions.csv). Ghidra's 0x5b5610 record also swallows the functions.csv starts
  0x5b5690 and 0x5b5a20 (both drafted). Total this session: 10 starts that Ghidra's own function list did not
  have (or had merged).
- The Domizil table lookups 0x5ba620 / 0x5ba660: an INDEX loop over absolute addresses
  (`U_I(0x62c420 + i * 0x1c)`) is the form that matches; the pointer-walk form compiles to `mov eax, 0x62c420`
  instead of `mov eax, ds:0x62c420` (see notes/vc6-matching.md).
- 0x5ba600 is void (32 B): `0x476c70(); 0x472ce0 (0x6f26ac); 0x6f26ac = 0;` - an int return adds a
  `xor eax,eax` and loses the byte match. 0x5bc0f0 has the same shape but takes the id through 0x478fc0 and
  returns nothing.
- Near-miss for the match session: 0x5bb2d0 (112 B, 95/128). The six order kinds 0x172 / 0x174, 0x158 / 0x160
  and 0x16e / 0x176 must match the building kind byte (4 / 0x13 / 0x10) of the kind record
  0x7e3f30 + kind * 0x24d of the +0x16c record; the original merges the return-0 paths into one block at the
  end, which our `if (a) return 0; if (b) return 0; ...` form does not (it emits inline `ret`s); the combined
  condition form got 95/128 but the table-address computation order still differs.
- ROUGH stubs drafted: 0x5b26b0, 0x5b3130, 0x5b3630, 0x5b3ad0, 0x5b3f30, 0x5b4340, 0x5b4590, 0x5b4820,
  0x5b4bf0, 0x5b4c60, 0x5b4fb0, 0x5b5010, 0x5b5310, 0x5b5610, 0x5b5690, 0x5b5a20, 0x5b5e80, 0x5b6950,
  0x5b6b20, 0x5b6eb0, 0x5b6fe0, 0x5b7690, 0x5b7b80, 0x5b7dd0, 0x5b82a0, 0x5b84f0, 0x5b8fc0, 0x5b92e0,
  0x5b9730, 0x5b9db0, 0x5b9f30, 0x5ba1c0, 0x5ba290, 0x5ba6b0, 0x5ba700, 0x5ba750, 0x5ba980, 0x5bac10,
  0x5bae10, 0x5bae50, 0x5bb2d0, 0x5bb350, 0x5bb950, 0x5bba00, 0x5bc110, 0x5bc420, 0x5bc6d0, 0x5bc790,
  0x5bc9c0 - each has its frame size, its first calls and (where found) its seam in the comment.
- No new unit, no header change: the @boundary stays plausible. Next frontier: 0x5bcc70 (1000 B), then the rest
  of the module to 0x5de450.

## layout 2026-10-24 (eighth session): 0x5bcc70 .. 0x5c1df0

Drafted 28 functions in address order (the unit now runs 0x58bbd0 .. 0x5c1df0); no function matched; the
session start 2495 byte matches stayed 2495 (the new drafts are all nonmatching). The .data order keeps
climbing in function order (0x62d134 "Panel_player_stats", 0x62d1e8 .. 0x62d1fc, 0x62d204 "special_amt2",
0x62d214 "special_amt3", 0x62d224 "special_amt1", 0x62d234, 0x62d244 "special_amtinfo"), one .data block (D8):
no boundary; the special-amount windows are the same TU.

**Unit** (new facts):
- The player stats panel 0x5bcc70: 0xfa5aa0 + id * 0x8c is a dialog record (dword +0 the flags `|= 1`, +8 the
  selected entry; cm.c 0x4e5470 reads the same record as `ai_Unkfa5aa8[radio * 35]`). Its radio group comes from
  `0x52e350 (screen, 0, out, 3, 0x144a134, 0x144a138, 0x144a13c)`; the three entry controls are
  0x5bc420 / 0x5bc6d0 / 0x5bc9c0; the row list 0x6f40e4 .. 0x6f4cf4 (6 dwords per row: id -4, cached value -1,
  the three texts and the character) is refreshed through 0x4c3ba0 / 0x4c3c00 / 0x4c3d00 with the offsets
  0xa68d2c; the carrier record of the player is 0x770cf8 (character +0x178).
- The special-amount record (0xe dwords, passed to 0x5bd5e0 / 0x5bdb60 / 0x5bf0d0 / 0x5bf300 / 0x5bf6f0):
  +0 the character, +4 the row control, +8 the slider (-1 until built), +0xc a text id, +0x10 / +0x14 x / y,
  +0x18 the icon control, +0x1c / +0x20 / +0x24 / +0x28 the four text controls, +0x2c the class byte
  (0..8, the fill functions set it), +0x30 the 0x5c33c0 result (1 = selectable).
- The row builders: 0x5bd5e0 (layout switch on the kind 1 .. 8: one 0x80 column / two 0x40 / three 0x80, the
  x steps 0xca within a group and 0x80 between groups), 0x5bdb60 (the kind 8 variant whose switch writes the
  x / y of six fixed rows at param_5 + 0x10 / +0x14, +0x48 / +0x4c, +0x80 / +0x84, +0xb8 / +0xbc,
  +0xf0 / +0xf4, +0x128 / +0x12c, and the page word 0x23 .. 0x27 from the flags +0x1cc), 0x5be3b0 (one row at
  x, y with the icon first), 0x5be720 (the compact row; the winner +0x210 gets the 0x6d3 mark).
- 0x5be1d0 is the status of a record (0x6ca .. 0x6d2, the carrier 0x53e400 (1, 0, 0x6f) / (1, 0, 0x18) /
  (1, 0, 0x45) matches, 0x4f4130, the price class from 0x4fc820 with 100 / 350 / 650). 0x5bf0d0 / 0x5bf300 /
  0x5bf4f0 / 0x5bf6f0 are the page fills; the record relation slot +0x2c is where the sort key lives
  (0x5bf300 inserts by 0x50c550 >= 75.0 and different world +0x210, 0x5bf4f0 by the constant 0x5ed0fc with
  the same-world shortcut 0x5eb788; 0x5bf6f0 fills up to 4 carrier records only). The __ftol at 0x5de518 is
  the int relation slot.
- The tab table 0x62d14c: eight entries of 5 dwords ([0] the count, [1] the tab control id, [2] the fill
  function, [3] the row builder, [4] unused); 0x5c08b0 clears [0] / [1], 0x5c0970 ("special_amt3")
  dispatches on the selected index 0x6f2f8c, 0x5bfed0 is the empty page. The tab controls are 0x46bf10
  (0xbf) with 0x46c260 (0xb9) and the images 0x143fbe0.
- The menus: 0x5c1130 ("special_amt1", 9 buttons, the order 0, 8, 6, 4, 5, 1, 2, 3, 7, the opener callback
  0x5bea50 (page) and 0x5c0970), 0x5c1b40 (7 buttons, the order 8, 6, 4, 5, 1, 2, 3, the callback param_1
  receives the order value), opened by 0x5c1b20 (callback 0x5c1500, record 0x143a3f0) and 0x5c1df0
  (callback 0x5c1e10, record 0x143f030), both ending with 0x52d7a0. 0x5bea00 builds a 0x50-byte config
  (+4 = 0x204, +8 = 0x5eed68, +0xc = 0x5eed6c, +0x10 = 6) and runs 0x5c0970 with it.

**Open work** (for the match / layout sessions):
- **Ghidra-missed starts**, all drafted as ROUGH stubs from the disassembly (they are reachable only through
  the function pointer tables / callbacks, so Ghidra's function list lacks them): the page fills 0x5bf7c0
  (717 B: the carrier walk 0x53e400 (1, 0, 0x2c), the two 0x505df0 ids, the local 0xc8 = 200),
  0x5bfa90 (428 B: the +0x1c8 flag 0x400 and 0x53e400 (1, 0, 0x47)), 0x5bfc40 (335 B: the +0x168 guard byte),
  0x5bfd90 (314 B: the +0x166 guard byte); the tab fills 0x5bffb0 / 0x5c00b0 / 0x5c01c0 / 0x5c02d0 /
  0x5c03e0 / 0x5c0550 / 0x5c0650 / 0x5c0790 (all start with the count guard and 0x5bfed0 (param_1, param_2,
  param_3), then their rows; frames 0, 0x104 (0x5c03e0) and 0x204 (0x5c0650)); 0x5bff80 (39 B: the release
  loop 0x475e40 (record + 8, 1) over n records of 0x38) and 0x5c1500 (1394 B, the 0x5c1b20 callback: the
  "special_amt2" 0x62d204 window, frame 0x2ec). 0x5c1e10 (846 B, the 0x5c1df0 callback; frame 0xac, the
  call 0x4e6fc0 (param_1, 6, the 0x8c record buffer) and then the list rows) is a ROUGH stub too, so the unit
  is contiguous again 0x58bbd0 .. 0x5c2160 (370 functions, 73 byte, 74 behaviour). Next: 0x5c2160 (657 B, a
  call from 0x5a07d0).
- ROUGH still to fill: 0x5bea50 (the special_amt2 window body), 0x5c0970 (the tab window body, the Ghidra
  call arguments of the function pointer calls are scrambled), 0x5c1130 (the menu loop is complete, the
  button flags / images are drafted), 0x5bf4f0 (the compare directions noted from the flags: `jnp` after
  `test ah,0x41` = not-less, `test ah,0x5` + `jp` = unordered-or-less).
- The near-misses of this session: 0x5c1df0 (4/32: the wrapper call, only the constant record / callback
  differ), 0x5c1b40 (575/672).

## layout 2026-10-24 (eighth session, continued): 0x5c2160 .. 0x5c26a0

Four more functions, the law book / master letter module: 0x5c2160 (the listener of the pages 0x111
Strafrecht / 0x112 Steuern / 0x113 Verfassung: 0x52f360 (the object names 0x62bde4 / 0x62be00 / 0x62bdc8,
the book window 0x62d294), the sections 0x13f8 / 0x13f7 / 0x13f6, the entries start .. end in the two pages
+0x228 / +0x22c with the break after 6 / 4 / 5 rows, the row text 0x4f2fe0 + 0x515960 (0x62d284); a click
opens 0x5c2410 (the base 0x1452 / 0x142a / 0x13fd + i * 5 and the kind text of 0x4f2720 +0x18 chosen by the
byte +1: +0x13dd / +0x13e6 / +0x13ec); 0x52f600 each frame, 0x52f530 + 0x52d7a0 at the end), 0x5c2410
("Gesetze\Gesetzbuch_Info" 0x62d2a4, **matched**, 128 B), 0x5c2490 (the law entry controls: the checkboxes
0x1951 .. 0x195b, the counter 0x1953, the slider 0x46a960 (0x1002), 9 parameters) and 0x5c26a0
("berufe\meisterbrief_1" 0x62d2bc, **matched**, 144 B). The literals climb 0x62d284 .. 0x62d2bc in function
order (one .data block, D8), so no boundary. Next: 0x5c2730.

## layout 2026-10-24 (ninth session): 0x5c2730 .. 0x5c7560

Drafted 22 functions in address order (the unit now runs 0x58bbd0 .. 0x5c76b0, 396 functions); 2497 -> 2498
byte (0x5c2730 matched first try). The stretch is the master letter module, the law / office selection, the
player info windows and the advertising family. The .data order keeps climbing in function order
(0x62d2bc "berufe\meisterbrief_1", 0x62d2d4 "berufe\meisterbrief_2", 0x62d2ec "Gesetze\Auswahl2", 0x62d300
"Special_Stammbaum", 0x62d3e0 "panel_info_spieler", 0x62d3fc "misc\geschenck", 0x62d40c
"privillegien_werbung2", 0x62d424 "privillegien_werbung1"), one .data block (D8): no boundary.

**Unit** (new facts):
- **The master letter module** (0x5c26a0 .. 0x5c3200): 0x5c2730 is the 0x5c26a0 clone with the single caption
  0x195e (**matched first try**). 0x5c3200 is the contact dispatcher: the character param_1 (its kind byte 0,
  the owner word +0x27 against 0x6f25b8), 0x50a810 the character; the mode 0x40 for the kinds 5 / 9, 0x30
  with a character, 0x20 without (the profession byte 0x223 against the six 0x504ad0 / 0x5044b0 slots), 0x50
  when the 0x53e400 (2, 0, 7, 3) carrier chain ends; the windows 0x5c27b0 (0x10 / 0x20), 0x5c2b90 (0x30),
  0x5c2730 (0x40), 0x5c26a0 (0x50), then 0x52d7a0.
- 0x5c27b0 (608 B): the callback of the master letter window; the message 0x10 builds one control (the texts
  0x1949 / 0x194b / 0x194d onto 0x475b80 (win, 1, ..)), 0x20 two (0x1938 and its +1 char); a click on the first
  walks the carriers 0x53e400 (2, 0, 7, 3, the record +1) / 0x53e300, and when the chain ends sends the AIMSG
  type 7: +0xc the record +1, +8 the character 0x770b84 + 0x6f25b8 * 0x87, the clock 0x770ae0 .., +0x36 = 1,
  +0x58 = 0, +0x60 = -1 or 0x6f2310 + 2, and the text 0x1939 with the kind record 0x7e3f30 + kind * 0x24d byte
  +0x223 + 0x130 (0x521150 (text, 0)); a click on the second sets 0x6f22a0.
- 0x5c2a10 (400 B): seven profession rows of 0x30 bytes; the record param_2 + i * 8 (+0 the object id, +4 / +5
  two bytes); the picture 0x4731d0 (0xf, i * 0x30, id + 0xdf, screen) into the control record 0xf8f080 +
  id * 0x2ec (+0x1c0 = 3, +0x2e8 = 0x91aaf0 + id * 0x41, +0x4c = 1, +0x28 = the +5 byte == 0), param_4[i] =
  the control; the slider 0x46a960 (100, 0xc + i * 0x30, 200, 0, 100, 0x7b, 0x802, screen) into param_3[i];
  a zero record gives -1 in both arrays.
- 0x5c2b90 (1648 B, ROUGH): "berufe\meisterbrief_2" 0x62d2d4: the production settings record 0x5a8540
  (param_1 + 1) / 0x5a8560 (param_1), 0x5a8850, the percent 0x5085b0 (param_1 + 0x39); the page 0 the
  checkboxes 0x5c2490, the page 1 the seven rows 0x5c2a10; the row clicks 0x475980 / 0x5a8710 / 0x475cb0, the
  master 0x5a8900 (the percent * 0.01), the four order flags (0x4c3ba0 / 0x4c3c00 on param_1 + 0x39 and
  param_2 + 0x1b4 / +0x1b8), the exit 0x5085f0.
- **The law / office selection**: 0x5c33c0 (992 B) is the eligibility predicate - the filter param_3 (+4 the
  mask, +0x14 bit 2 with the four +0x1c ids, +0x40 / +0x44 / +0x48 the relation range 0x50c550) against the
  character record 0x770b80 + idx * 0x21c (the mask bits 1 / 2 the +9 byte, 4 / 8 the +0x50 / +0x5c pair,
  0x10 the +0x165 byte with kind 0, 0x20 the +0x1c8 0x40000000, 0x40 kind 1, 0x80 the +0x1f0 int, 0x100 the
  +0x166 / +0x169 bytes, 0x200 / 0x400 0x4fd7e0, 0x800, 0x1000 the +0x5c handle 0x505df0, 0x2000 +0x184,
  0x4000 / 0x8000 the kinds 5 / 6 / 7, 0x10000 +0x170, 0x20000 / 0x20 / 0x40000 the +0x1c8 / +0x1cc dwords,
  0x100000 / 0x200000 / 0x400000 / 0x800000 the four kinds with 0x47dd70). 0x5c37a0 / 0x5c39a0 are the two
  filter tests (the building kind bitmask 1 << kind, the target cases 0 .. 7, the own character 0x6f1bf8 for
  case 7, the four +0x1c exclusions; 0x5c39a0 adds the 0xf82806f / 2 mask against 0x40000000). 0x5c3bb0
  (1360 B, ROUGH) is "Gesetze\Auswahl2" 0x62d2ec: the character ids of param_1 (+0x25 / +0x27 and the
  characters whose +0x16c / +0x170 owner is param_1 for the kinds 1 / 2 and 4 .. 0x16), up to 0x10; the rows
  0x4731d0 / 0x50c550 / 0x5176d0 (0x62a564) / 0x46a960 (0x8c, y + 4, 0x40, 0, 100, 0x75, 0x442), the filter
  0x5c33c0, the callback param_4. 0x5c4100 is its entry (0x50b090 redirects, else 0x5c3bb0).
- **The big player windows**: 0x5c4150 (4542 B) "Special_Stammbaum" 0x62d300 (the family tree: the gate
  0x5763c0 (1) / the event 0x576040 (6, 0, 0, 0), the parents / partner / six children / grandparents as
  portraits in the boxes 0x4764f0 with the labels 0x5176d0 (0x62d1fc) / 0x5bd0c0, the click 0x5c5310 between
  0x475590 (win, 0 / 1)); 0x5c5310 (4544 B) "panel_info_spieler" 0x62d3e0 (the pages 0x1f53 .. 0x1f63, the
  five skill rows 0x52eb50, the flags of +0x16, the buttons 0x1f49 / 0x1f50 and the click 0x5c6050 /
  0x5cf260 / 0x5cf4f0 / 0x4b5310); 0x5c6050 (1136 B) "misc\geschenck" 0x62d3fc (the gift slider, the
  0x521040 gold check, the +0x1c8 bit 0x20 flag, 0x4c47e0 the gift).
- **The advertising family**: 0x5c64c0 the type 9 candidate list (0x4a5ef0 (param_1, 9, 0xc0540a - the +9
  byte, 0.0f, 100.0f, ids)); 0x5c6530 the marriage test (the errors 0xfffffc00 missing / 0xfffffbff /
  0xfffffbfe the +8 byte / 0xfffffbfd / 0xfffffbfc the +0x5c partner / 0xfffffbfb / 0xfffffbfa the +0x1b1
  flag, the age difference 0x505e60 < 6); 0x5c6620 the marriage rating (the relation 0x50c550, the age
  0x505e30 clamped 15 / 30, 0x4f81c0's bit 2 halves it); 0x5c6910 "privillegien_werbung2" (the proposal
  confirm, AIMSG 0x41, the 0x4c3ae0 / 0x52fe00 wait, 0x4115b0 (0x11)); 0x5c6aa0 "privillegien_werbung1" (the
  advertising list, the rows 0x5be1d0 / 0x5be3b0 / 0x5bd0c0, the click 0x5c6910 -> 0x92); 0x5c6d00 (the
  office application: the 0x53e400 (1, 0, 0x41) job walk, the states +0xba / +0xbb, the rank +0xb8 / +0xb9,
  the 0x42ec00 rolls, 0x4fe140 / 0x4c4c60, ROUGH); 0x5c72d0 / 0x5c7460 / 0x5c7560 the three bar charts over
  the 0x91ab0c .. 0x91ac4c table (5 dwords per entry, the float +4; 0x4764f0, ROUGH).

**Open work** (for the match / layout sessions):
- Nonmatching with complete calls: 0x5c27b0 (430/592: our prologue uses edi for the constant 1 where the
  original uses ebx, and our msg / text frame is 4 bytes tighter - one of the two control ids collapsed into
  a register), 0x5c2a10 (319/368: our first instruction is `push ecx`, the original pushes ebx / ebp / esi /
  edi), 0x5c3200 (246/432), 0x5c33c0 (1041/1088; our draft 1088 B vs 992), 0x5c37a0 (389/448), 0x5c39a0
  (413/464), 0x5c6530 (55/240 - the closest: the error ladder / the age check), 0x5c6620 (459/624), 0x5c6910
  (338/432), 0x5c6aa0 (353/496), 0x5c6d00 (644/800), 0x5c64c0 (96/112), 0x5c4100 (35/48 - the original keeps
  param_1 in esi and reloads it after the address-taken 0x50b090 call; we keep it in the slot).
- ROUGH skeletons still to fill: 0x5c2b90 (the 0x5c2490 call passes four flag words, the callee draft has
  one), 0x5c3bb0 (the hidden rows and the 0x46cda0 x / y), 0x5c4150 (the generation positions), 0x5c5310 (the
  page / row coordinates and the bottom-button ladder), 0x5c6d00 (the rank rolls), 0x5c72d0 / 0x5c7460 /
  0x5c7560 (the __ftol float arguments of 0x4764f0).
- 0x5c2490's call sites (0x5c2b90) pass 12 values while its draft takes 9: see notes/layout.md Type debt.
- The REA pseudocode of 0x5c2730 .. 0x5c7560 was queried inline (batch_decompile) and is not exported to
  build/ghidra (the JSON was not kept, as in the earlier sessions).

## layout 2026-10-24 (tenth session): 0x5c76b0 .. 0x5c9df0

15 functions, the statistics / privilege module; the unit now runs 0x58bbd0 .. 0x5ca560 (411 functions,
76 byte, no function matched this session). Drafted in address order: the two bar charts 0x5c76b0 /
0x5c7800, the chart window 0x5c7970 (1422 B, ROUGH), the statistics panel 0x5c7f00 (1520 B, ROUGH), the
player comparison 0x5c82f0 (837 B), the privileges panel 0x5c8640 (1403 B, ROUGH), and the privilege
handlers 0x5c8bc0 (803 B), 0x5c8ef0 (927 B), 0x5c9290 (695 B), 0x5c9550 (1082 B, ROUGH), 0x5c9990 (694 B),
0x5c9c50 (406 B), 0x5c9df0 (511 B), the blackmail contact variant 0x5c9ff0 (342 B) and the law /
privilege window 0x5ca150 (1019 B, ROUGH, "privillegien_gesetz" 0x62d518: the law info 0x4f2720, the
setter 0x4f2750 (declared void, the hst.c view; the click branch casts it back to int), the four button
pairs 0x13d0 / 0x13da / 0x13e3 / 0x13e9 and the slider 0x475cb0 / 0x475980 / 0x475e40). The last three have no Ghidra function (found by the ledger: the
dword in code at 0x5c9c1c / 0x5c9fc2 and the reference from 0x4f5350); they are drafted from
uv run disasm.

**Typed facts** (for the match sessions): the five bar charts take TWELVE int parameters - the caller
0x5c7970 copies ten dwords (its own param_2 .. param_11) and passes the two screen words of the record
0xf8f120 + screen * 0x3c4: param_11 = [record + 8] - 1 (the height), param_12 = [record + 0xa] (the width).
The rectangle function 0x4764f0 is a SEVEN argument fill (x1, y1, x2, y2, r, g, b) - the old drafts of
0x5c72d0 / 0x5c7460 / 0x5c7560 with six parameters and two-argument calls were wrong and are converted
to the 12-parameter / 7-argument form (still ROUGH). The chart loop pairs: the floats of the statistics
table 0x91ab08 / 0x91ab0c / 0x91ab14 / 0x91ab18 / 0x91ab1c (5 dwords per entry, stride 0x14) minus the
baseline (float)(w - 0x16) times the height (float)(w - 0x1e); the scale is (h - 0x69) / 16.

**Open work**:
- The two new charts 0x5c76b0 / 0x5c7800 are drafts 300/368 and 331/400; 0x5c7800 computes the maximum of
  0x91ab0c + i * 0x14 into a local and never reads it again (the original really does pop it: `fstp st(0)`
  after the loop) - a match session must find the source form that keeps the dead loop.
- The five chart drafts still differ (0x5c7560 66/96 is the closest); the max()/min() spelling of the
  0-clamp (fcomp QWORD 0x5eb830 = 0.0) is the open point.
- 0x5c7970's ROUGH tail (the axis labels 0x5176d0 / 0x473650 / 0x46ea10 / 0x4fe2c0 and the fifteen rows)
  is sketched only; 0x5c7f00's label section and 0x5c9550's candidate records (stride 0xe ints) too.
- The handlers 0x5c8bc0 / 0x5c8ef0 / 0x5c9290 / 0x5c9990 / 0x5c9c50 / 0x5c9df0 share one shape (window,
  click id 0x5fe710, the 0x4c4b70 record, the pump): 0x5c9df0 415/496 and 0x5c9c50 291/400 are the closest.
- The AIMSG by offset: +2 the type, +4 the handle, +9 a flag, +0xd a byte, +0x24 an amount, +0x5c the
  partner record, +0x194 the choice count, +0x1c8 the flags (0x100 / 0x200 / 0x400 / 0x800). The record
  builder 0x4c4b70 takes the record by offset (kind byte +4, type byte +0x36, the clock +0x28 .. +0x38,
  the character +8 / +0xc / +0x58, the handles +0x5c / +0x60 / +0x62).

## layout 2026-10-24 (eleventh session): 0x5ca560 .. 0x5cb250

7 functions; the unit now runs 0x58bbd0 .. 0x5cb4f0 (418 functions, 2498 byte matches, unchanged).
Drafted in address order: 0x5ca560 (the embezzlement privilege handler: "absetzen" 0x4c4540 /
0x4c4580, the kind check 0x4e6f80 (owner +0x166) against 0x505df0 / 0x4e6030, the config (0x400,
0x5eed68, 0x5eed6c, 6, 3, +0x1c the two ids) into the window 0x5bea50 and the 0x1d98 / 0x1d99 /
0x1d9a / 0x1d9b texts), 0x5ca860 ("privillegien\unterschlagung" 0x62d538: the price
(int)((float)((0x42ec00 (0xb0) & 0xffff) + 0x19) * 0x5eceb0 * the 0x4fa700 record +4), the payment
0x4c4830 and the click flag 0x520f90 (4)), the **Ghidra-missed start 0x5caab0** (the bewitch window
"privillegien\becircen" 0x62d554, reached only through the menu callback of 0x5cacd0; the player's
character byte +9 against the message +9, then the slider 1 .. 5 and the payment 0x494d40), 0x5cacd0
(the contract handler, a char result: the 0x5c1130 menu with the callback 0x5caab0, or the amount
1 .. 5 payment), 0x5cae00 (the counter-espionage "privillegien\gegenspionage" 0x62d56c: the AIMSG
record and the carrier walk 0x53e400 (1, 0, 0x18) / 0x53e300, the clock at +0x52 AND +0xc8 as
`*(HETIME *)(carrier + n) = ai_Time;` struct assignments, the text 0x1db8 appended with 0x1db9 per
carrier and 0x1dba when none), 0x5cb1e0 (the wrapper of 0x4e6ba0; **behaviour**, 49/112) and
0x5cb250 ("privillegien\gegenspionage2" 0x62d588, ROUGH from the disassembly - no Ghidra function:
the owner kind 0xf / 0x15 / 0x1b picks the two character kinds 0xd / 0xe, 0x13 / 0x14, 0x19 / 0x1a
and the walk over the 0x300 records 0x770b80 .. 0x7d5f80 finds them; the message wrapper 0x5cb1e0
decides 0x10 / 0x20).

The literals climb 0x62d52c "absetzen" .. 0x62d588 "privillegien\gegenspionage2" in function order
(one .data block, D8), so no boundary. Then the espionage family 0x5cb4f0 .. 0x5cbe00 (ROUGH
SKELETONs, no Ghidra exports: from uv run disasm): 0x5cb4f0 (1440 B, the espionage window: the two
characters 0x505df0, the price over 0x50a640 / 0x5eb78c, the money 0x509e60, the relations 0x50c520,
the 0x5c1130 menus with the config 0x400 / 0x5eed68 / 0x5eed6c / 6 and the texts 0x1dcf / 0x1dd0; the
payment and the 0x53dda0 messages are comments), 0x5cba90 (880 B, the payment handler: the flag 0x8000
of +0x1c8, the target 0x505df0, the closes 0x4c5f60 (-2 / -3), the text 0x1dda and the 0x53dda0
message; the type-6 menu with the text 0x1dd7 and the 0x1ce0 refusal are written, the rest is a
comment) and 0x5cbe00 (1195 B, the order handler: the amount 5 / the owner flag 0x80, the building
0x501190, the text over the carrier owner 0x770b82 + U_W (carrier + 0x27) * 0x21c and the 0x506890 /
0x4c4e00 tail; only the non-6 head to 0x5cbf50 is drafted, the type-6 window 0x5cbf5b .. 0x5cc2aa is
one comment), 0x5cc2b0 (1104 B, the relation handler: the rolls 0x42ec00 (0x28) + 0xf, the two
characters 0x505df0, the two lines 0x4c4b00 and the close 0x4c5f60 (-3); the type-6 window is a
comment), 0x5cc700 (827 B, the payment handler: the price 0x50a640 / 0x5eb844 against 0x509e60, the
payment 0x4c47e0 and the close 0x4c5f60 (-6), then the walk over 0x770b80 skips the word -2 == 0xffff;
the window 0x5cc836 is a comment) and 0x5cca40 (455 B, complete: the value 1 .. 9 and the record
+0x194, the setter 0x4f3a30 and the window 0x62d634 with the three controls t / t+1 / t+2, the slider
0x475cb0 (1, 9, 1) and the texts 0x1dea / 0x1deb), then the rest of the espionage family: 0x5ccc10
(1738 B, the overview window, ROUGH SKELETON with the page 0 .. 5 and the 0x4a63f0 / 0x4fea50 /
0x50a640 price head), 0x5cd2e0 (282 B, complete: the type 7 no-op, the amount 4, the close -4 and the
overview 0x5ccc10 (0x42ec00 (6)), or the window 0x62d64c with the caption 0x1df6), 0x5cd400 (1211 B,
the result window 0x62d660, ROUGH SKELETON), 0x5cd8c0 (713 B, the spy-slot handler, ROUGH SKELETON:
the building walk 0x501190 (1, 6) / 0x501030 on the kind 0xf of 0x7e3f30 + kind * 0x24d), 0x5cdb90
(760 B, the spy list "privillegien\spionage2" 0x62cd7c, ROUGH SKELETON: 0x4f3fe0, the records
0x91f700 + id * 0x2d, 0x4f4390) and 0x5cde90 (775 B, the spy target handler: the kind 0xd refusals, the
target 0x505df0, the list 0x4f3fe0 and 0x5cd8c0 with the flag 0; the window 0x5cdf48 is a comment),
0x5ce1a0 (775 B, its twin with the flag 1; the window 0x5ce258 is a comment) and 0x5ce4b0 (345 B, the
spy payment: the kind list 0x4e6f10 / 0x4e6fc0 (6), the id equal to the message +4 pays 0x4c4b00 (0x64),
the other ids pay -0x14 after 0x505df0, and the types 6 / 7 send 0x53dda0 with the key 0x62d694 / 0x5fb;
the return values are ROUGH). Next: `uv run ledger next` = 0x5ce610 (469 B, a call from
0x4f7a60).

**Open work**:
- 0x5cb250 and the tail 0x5cb250's character walk are ROUGH; the text 0x624204 (the pooled format
  string) and the id order of 0x515960 (0x1dc9, 6, the two words) need checking against the disasm.
- 0x5ca560 / 0x5ca860: the record offsets from Ghidra differ between the type-6 and the other path in
  the disassembly (the clock at +0x28 vs +0x2c); check which frame slot is the real one when matching.

## layout 2026-10-24 (twelfth session): 0x5ce610 .. 0x5d0e60

25 functions drafted in address order; the unit now runs 0x58bbd0 .. 0x5d0f20 (457 functions, 78 byte,
2501 matches). The stretch is the **interrogation / trial-step message module**: the interrogation
windows "privillegien_verhoer" 0x62d5c4 (0x5ce610, 0x5ce950, 0x5cede0, 0x5cf020 with the config 0xc00 /
0xa7fdfbb6), "privillegien_arbeiter_ausweisen" 0x62d5dc (0x5ce7f0), "privillegien_CHOOSEPROFESSION2"
0x62d660 (0x5cf260) and "special_uni" 0x62d6bc (0x5cf4f0), the AIMSG 0x45 "refused" record 0x5ced50, the
object / use-entry callback 0x5cf8a0 ("panel_useobj" 0x5cf820), then the trial-step callbacks that reach
0x5d08c0: 0x5cfb60 (matched, the close), 0x5cfb80 (the mode-1 float payment), 0x5cfc00 / 0x5cff70 (the
espionage owner messages 0x76 / 0x77), 0x5d0320 / 0x5d0530 (the type 0x82 records with the float
0xbe2aaaab / 0xbeaaaaab), 0x5d06e0 (matched, the class-dword ladder), 0x5d0780 (the character walk of a
building request), 0x5d08c0 (the trial step: (252 - n) / 252 chance, 42.0f scale), 0x5d09d0 / 0x5d0a40 /
0x5d0ab0 (its wrappers with the step 1 / 3 / 4), 0x5d0b20 / 0x5d0c70 (the amount -25 / -50 / -40 / -80
transfers between two records) and 0x5d0e60 (the shuffled 0x505ca0 pick). **Thirteen Ghidra-missed starts**
were found by the ledger and drafted: 0x5cfb60, 0x5cfb80, 0x5cfc00, 0x5d0320, 0x5d0530, 0x5d06e0,
0x5d0780, 0x5d09d0, 0x5d0a40, 0x5d0ab0, 0x5d0b20, 0x5d0c70 and 0x5d0e60 (only 0x5ce610 .. 0x5cff70 and
0x5d08c0 are in functions.csv). Matched this session: 0x5cfb60 (24 B) and 0x5d06e0 (156 B). No new
literal run: 0x5d08c0 reads 0x61126c .. 0x611680, a backward jump into the earlier pooled strings (the
he.c module), so the .data order gives no boundary inside the stretch; the unit continues (a leaked name
for the last stretch is still the open naming question).

**Typed facts** (for the match sessions):
- 0x4c4ac0 has a raw **4-byte float** third argument (`0x4c4ac0 (the handle, 0x1c, the float)` in
  0x5cfb80); the cm.h `void (int, int, int)` view is wrong for that call site - the draft calls it
  through `((void (*)(int, int, float))0x4c4ac0)`.
- 0x5b2180 takes a **fifth argument** (the request record is 0x14 dwords); its definition in this file
  now has `param_5` (unused in the ROUGH body).
- ai_Unk42ec20 is declared `float` in this file now (as in unk_575860.c). The three older callers
  0x598170 / 0x5a5530 / 0x5a5910 (all drafts) now see the float return; their byte diffs changed with
  the declaration, they are not matched.
- The request record 0x5b2180 returns: the dword at **+1** is the +0x58 field of the message and the
  4th argument of 0x4c3c00 (both 0x5cfc00 and 0x5cff70); +0x27 is the owner word.
- The character records 0x770b80 + i * 0x21c: +9 the byte compared against the player's 0x6f25b8
  record (the 0x401 / 0x402 config in 0x5d0b20), +0x1c5 a packed byte read as `(x & 1) + (x >> 1)`
  (0x5d0780), +0x16c the owner / building pointer compared against the 0x5b2180 request (0x5d0780).
- 0x505ca0 (n, out) fills out with a shuffled 0 .. n-1 (f3gm.c); 0x5d0e60 uses the indices on the
  message record's counters +0x80 .. +0x84.
- The message +0x58 / +0x59 / +0x5c / +0x60 / +0x64 fields of the trial records: the bytes 3 / 2
  (0x5d0320), 0 / 4 (0x5d0530), the float bit patterns 0xbe2aaaab (-1/6) and 0xbeaaaaab (-1/3), the
  dword 0x18; +0x6e = 0x177 / 0x178.
- 0x53dda0's 6-argument shape here is `(the character record + 4, -1, text, 0, 0x5fb, 0)`; the
  text 0x5176d0 takes the text id and up to two values.


## layout 2026-10-24 (thirteenth session): 0x5d0f20 .. 0x5d3270

23 functions drafted in address order; the unit now runs 0x58bbd0 .. 0x5d33b0 (480 functions, 78 byte,
2501 matches, unchanged). The stretch is the **trial / action callbacks of the record table 0x62c420**:
every one of them is reached only through that table (the function pointer sits at record offset +0x14;
the records are 0x1c bytes, the text id at +0, the value at +4): record 5 -> 0x5d1830, 17 -> 0x5d0f20,
19 -> 0x5d1300, 21 -> 0x5d1680, 22 -> 0x5d13e0, 32 -> 0x5d2580, 34 -> 0x5d2720, 36 -> 0x5d2960,
37 -> 0x5d2bc0, 38 -> 0x5d31c0, 39 -> 0x5d2f90, 41 -> 0x5d3270, 43 -> 0x5d3030, 44 -> 0x5d2e80,
49 -> 0x5d2f20, 50 -> 0x5d18c0, 53 -> 0x5d1990, 54 -> 0x5d1b30, 55 -> 0x5d1da0, 57 -> 0x5d1fb0,
59 -> 0x5d22e0, 60 -> 0x5d2320, 61 -> 0x5d2440. The table also points at the previous session's
0x5d0e60 and at 0x5cfb60 / 0x5cfb80 / 0x5cfc00 / 0x5cff70 / 0x5d0320 / 0x5d0530, so it registers
callbacks of both address ranges (cross-TU registration table; no boundary evidence either way).

**Drafted**: 0x5d0f20 (992 B) - the two random character picks are written: 0x42ec00 (0x300) gives an
index, the walk over the 0x300 records 0x770b80 + i * 0x21c skips the ids of the actor record, of param_2
and (second pick) of the first pick, the free records (word +0 == -1), the kinds +2 >= 0xa and the
byte 0x770ce6 == 0, advancing (seed + 1) % 0x300 for 0x300 tries; the type 6 menu config (0x5c1130)
and the text / message tail are comments. The other 22 are **ROUGH stubs** from uv run disasm with the
record index, the size, the frame and the call sequence in the header comment.

**Typed facts** (for the match sessions): the callbacks take two arguments (param_1 the message record:
+2 the type, +0 / +4 the handle; param_2 a state record: +0x10 set to 1, +4 the character handle). The
character record is 0x770b80 + i * 0x21c (word +0 the id, byte +2 the kind, byte 0x770ce6 a second flag).
0x5ba6b0 (param_1, 0x2c5) is a yes / no (the disassembly makes 1 / 2 of it). 0x5ba660 is the Domizil
lookup. The text / message pattern of the family: 0x5176d0 (the text buffer, the id), 0x53dda0 (the
message: the record +4, -1, ...), 0x4c4b00 (a line), 0x4c4b70 (a record build), 0x4c4a60 / 0x4c5fa0
(closes), 0x5cf820 (the panel). 0x5de634 / 0x5de652 are CRT helpers (0x5de652 = sprintf) called from
the later 0x5d2xxx functions and from the next unit.

**Open work**:
- 22 ROUGH stubs (all of 0x5d1300 .. 0x5d3270) need the real bodies; the closest to a match is 0x5d2e80 /
  0x5d2f90 (the same 160 B shape: 0x5b2180, 0x502020, 0x4c4a60).
- 0x5d0f20: the type 6 caption id and the config field order, then the text / message tail.
- The next unit starts at 0x5d33b0 (the screen-file module), see notes/units/unk_5d33b0.md.

## match-base 2026-10-24 (base-4 worker, shard 4/5)

- ai_Unk58bbd0: meaning fix (the two grid loops compute the x column with % 4 and the y row with / 4 - the
  original's and eax,X / jns / dec / or / inc and cdq / and edx,3 / add / sar idioms; the source had n & 3 and
  n >> 2); 242 -> 231 code hunks. Remaining: the original's table walks compare jl (signed) against our jb
  (pointer compare), the 0x58-byte record init unrolls the 9 -1 stores against our rep stos, the field stores
  interleave differently. -> deep.
- ai_Unk58bd60 -> deep: 811 code hunks, a ROUGH SKELETON: the sell/buy list block (control 0x5fe710, the goods
  records 0x91aaf0 + id * 0x41, 0x50ab80 / 0x521150 / 0x53e400 / 0x53e300, the order 0x4c4b70 + wait 0x4c3ae0,
  the 0x75f290 / 0x75f2a0 marks) and the column handlers are comments only.
- ai_Unk58c680 -> deep: 603 code hunks; the availability logic is unmodelled (rec = 0 / sum = 0 with the
  comments "the goods id of the row" / "0x74d .. 0x74f"; the 0x5098a0 have[] array and its count are unused).
- ai_Unk58cc10 -> deep: 1112 code hunks; 2827 B; the availability loop uses the pseudocode registers, the
  0x5098a0 count is not modelled and the label branches 0xc1 .. 0xc6 are condensed.
- ai_Unk58e130: meaning fix (the 68-dword kind table is a local automatic array: the original does
  sub esp,0x110 and initialises every entry on the stack; ours was static const); 255 -> 186 code hunks and the
  unit's .rdata debt drops from 544 B / 2 symbols to 272 B / 1 symbol. The rest: the init order, jl vs jb and
  the id-in-dx pointer walk. -> deep.
- ai_Unk58d720 -> deep: 631 code hunks; frame 4 B too large (0xe0 vs 0xdc), the 54 separate tbl[] stores here
  against the original's zero-first sequence, jl vs jb.
- ai_Unk58dd00 (579), ai_Unk58e440 (309), ai_Unk58e6a0 (914), ai_Unk58ee30 (445) -> deep: ROUGH layout
  transcriptions, no meaning error found by reading. 0x58ee30's kind table looked like 0x58e130's but making it
  automatic made it WORSE (445 -> 470, reverted): that table stays static, the 0x114 frame is another local.
- Project-wide: ai_Unk5309a0 / sv_Unk5309a0 -> sv_RunFrame (0x5309a0, "one frame of the game screen loop"): the
  alias and the sv.c name merged, one prototype int sv_RunFrame(int cb, unsigned mask) in all 16 files (the
  callback is an int in the definition), 4 lint baseline lines dropped. ai_Unk4c4b70 -> cm_SendLongMessage
  (0x4c4b70, message 0x1c) with int return (the callers use the result), 8 baseline lines dropped.
- ai_PumpFrames (0x58f620, was ai_Unk58f620): byte + readable - the build screen's frame pump running
  sv_RunFrame(this, 0x67fff) until the screen ends; the context argument is unused in the body.
- ai_BuildingName (0x58f640): meaning fix (the outer table walk steps 0x50 bytes, not 0x14: 15 groups of 0x50 =
  name[0x20] + 12 ids, the ids at 0x62ab08 + g * 0x50 and the names at 0x62aae8 + g * 0x50; the layout note's
  "60 groups of 0x14 bytes" was wrong). 4 -> 2 code hunks. A typed AIBUILDINGREC view + extern in unk_58bbd0.h
  cleared Q3 (draft -> readable) but made the code WORSE (2 -> 20 code hunks: the compiler stops using the
  running pointer when it indexes the struct), so it was reverted per the no-WORSE rule and the header additions
  were removed again. Q3 still open: 0x62ab08 / 0x62afb8 / 0x62aae8. The remaining code hunk is the inlined
  strcpy's add vs lea.

## match-base session 2026-10-24 (shard 4/6 worker)
- ai_MarketContact (0x58f510, was ai_Unk58f510): byte + readable. The record is the AIBLD building record
  (local pack(1) view, owner at 0x27), the calls go through d3_FindObject (d3.h), sv_RegisterNamePlate and
  ai_OpenTransport. The five plate text ids are externs in unk_58bbd0.h (0x1441f80 / 0x14420b0 / 0x1442068 /
  0x1442120 / 0x14420b8).
- ai_BuildingName (0x58f640): readable, escalated. The group table 0x62aae8 is AIBUILDNAME[15] (name[0x20] +
  12 int ids, 0x50 each) in unk_58bbd0.h; Q3 clear. One code hunk left: `add eax,0x62aae8` (ours) vs
  `lea eax,[eax+0x62aae8]` (original). Tried struct field, pointer arithmetic, a 2D char view and the
  char*-cast form - all give add. (The earlier session's typed view made the code WORSE; this one does not:
  2 code instructions, the unit stayed at 122404 differing bytes.)
- ai_BauplatzFilter (0x58f6a0, was ai_Unk58f6a0): readable, escalated. ai_BauplatzList (0x6f5308) /
  ai_BauplatzCount (0x6f2688) externs, AIBLD.object (0x61). The original inlines strlen(prefix)
  (`repnz scas`); `#pragma intrinsic(strlen)` before and after <string.h> does not expand it under /O2
  (VC6 SP5+PP), so /Oi for the TU is the deep lead. Calls now through d3_PointToWorld, gm_BuildQuery,
  gm_BuildNext, d3_VecEqual prototypes (the aliases stay in the other units; the canonical K&R names conflict).
- ai_FindBauplaetze (0x58f7a0): -> match-deep. The one meaning difference (the list pointer re-read through
  0x6f5308 each iteration instead of the parameter) compiles to 384 B against 369 and unitdiff marks it WORSE,
  reverted. The rest are register / slot / scheduling hunks.
- ai_VgFilter (0x58f920, was ai_Unk58f920): byte + readable. Typed views AIOBJVIEW (parent 0x1f8, kind 0x220)
  and AIVGLIST; 0x5de6b0 is the CRT's _strstr (build/link/symbols.csv) - use strstr.
- ai_ShowVgObjects (0x58f980, was ai_Unk58f980): byte + readable. d3_DefaultScene declared as an extern makes
  VC6 hoist the load into the prologue (U_I did not); ai_VgHideFrom (0x6f35c4) declares the hide threshold;
  the two identical d3if_ShowTree calls in the if/else push the constants, the `v = list[i]` hoist keeps one
  list load.
- Next base item: 0x58fa20 (ai_Unk58fa20).

## match-base session 2026-10-24 (shard 4/6 worker, session 2)

Shard 4/6, unk_58bbd0.c (the unit's only base item). The base queue is again the whole unit; the seven items
0x58fa20 .. 0x58fe10 were worked in address order. Check count 2514 -> 2516, readable 1050 -> 1057,
behaviour-or-better 2629 -> 2633, byte + readable 968 -> 974; the unit's unitdiff 79/480 -> 81/480 matched,
122317 -> 122090 differing bytes.
- ai_CheckBuildable (0x58fa20, was ai_Unk58fa20): byte + readable. The AIOBJVIEW got next / prev / child
  (+0x1f0 / 0x1f4 / 0x1fc); the callees are d3_PointToWorld / d3_VecEqual / gm_BuildQuery / gm_BuildNext;
  the record field is AIBLD.object. The 0x58f6a0 / 0x58f7a0 walkers stay as they are.
- ai_CheckBuildSites (0x58fb60): byte + readable (the loop over ai_FindBauplaetze's list).
- ai_FindBauplatz (0x58fba0, was ai_Unk58fba0): behaviour + readable, readable from `strstr`. The B hunks were
  the block layout of the two `return 1`: VC6 lays the shared return-1 block at the end only when the body is
  a positive `if (d3_VecEqual(...) != 0) { ...; return 0; } return 1;` (26 -> 7 -> 2 code hunks; the last two
  are the ctx->object store scheduled before the strstr call in the original, after the loop here - `order`).
  The AIBUILDSITE view (name[0x40] / pos[4] / object / siteName) is the record of 0x58fc40; cm.c passes z+0xc.
- lc_GetGebaeudeBauplatz (0x58fc40): readable, -> match-deep. The meaning fix: the sprintf destination is an
  unused char[0x100] local, not `out` (the 0x100-byte frame error and 73 hunks); the rest is the register
  roles (see the @deep tag: rec=ebx + rematerialised owner in the original, owner=ebx + rec=edi here).
  The first parameter is unused (cm.c calls with two arguments); cm.h keeps the K&R declaration for that.
- ai_ObjectPosition (0x58fd40, was ai_Unk58fd40) and ai_ObjectAngle (0x58fd90, was ai_Unk58fd90): byte +
  readable, first try after the naming. Both save d3_CurrentScene into a local before d3_SelectScene(0, 1)
  (the original loads it before the call); the first is d3_LocalToWorld(plot, zero, out), the second
  d3_DirToWorld(plot, d3_ZAxis, v) + d3_VecAngle(d3_ZAxis, v) into out[1]. d3_ZAxis (0x5ebb74) and the
  struct XOBJ view of d3_LocalToWorld / d3_DirToWorld are local prototypes (the canonical one needs
  d3.h + unk_458810.h); am.h / amt.h carry the new prototypes (amt.h had a conflicting K&R one).
- ai_FindNearestObject (0x58fe10, was ai_Unk58fe10): behaviour + readable. Meaning fix: `d < 100.0` (double
  literal, the original `fcom QWORD PTR ds:0x5eba38`), not 100.0f (20 -> 4 fpu hunks). The frame: the source
  keeps the reference position in a `float pos[4]` local of which only [0] and [2] are set (the original's
  0x24 frame with the holes at pos[1] / pos[3]); separate float x, z give a 0x18 frame. Left: 12 `order`
  hunks (the p[1] store / the FPU op scheduling).
- Type debt: unk_58bbd0.h gained d3_CurrentScene (0x633444). The local `struct XOBJ` prototypes of
  d3_LocalToWorld / d3_DirToWorld match unk_458810.h's XOBJ; d3_LocalToWorld's third declaration view adds
  src/game/unk_58bbd0.c to the existing rd.h / unk_458810.h conflict in the lint baseline (rd.h still says
  char *). The two stale ai_Unk58fd90 conflict lines dropped.
- Next base item: 0x58feb0 (ai_Unk58feb0, byte + draft, Q1/Q2 across the 0x58ff00 / 0x5909a0 placement).

## match-base 2026-10-25 (base-0 worker, shard 0/6)

- 0x447190 call sites renamed ai_Unk447190 -> d3if_Delete; 0x4f2720 calls renamed ai_Unk4f2720 -> gv_GetTableRecord; byte unchanged.

## match-base 2026-10-25 (base-4 worker)
- Reference-only renames (no code change; every use updated, build/link/ledger clean): ai_Unk4c3ae0 -> cm_GetSlotState (0x4c3ae0) and ai_Unk4767d0 / ai_Unk476860 -> d2_StartFade / d2_ReleaseFade (0x4767d0 / 0x476860, the old ai_ aliases folded into the d2 names). See notes/units/unk_5d33b0.md for the whole rename list.
- Reference-only renames this session (no code change): 0x5a9330 ai_Unk5a9330 -> ai_SendSwapText (its only caller is
  amt.c amt_ExecuteSwapMessage, message 0x5c; the definition comment now says so); 0x5cb1e0's call to amt_SendOfficeSwap
  updated (0x4e6ba0). ai_Unk4e6ba0 -> amt_SendOfficeSwap is in amt.c/amt.h.

## match-base 2026-10-26 (base-2 worker)
- Reference-only rename (no code change; build/link/ledger clean): ai_Unk4b5310 -> ai_ShowMissions (0x4b5310,
  defined in unk_4b39f0.c; see notes/units/unk_4b39f0.md).

## match-base 2026-10-26 (base-4 worker, shard 4/10)
- Reference-only renames from the tailor-contact work (no code change, every use updated, build/link/ledger
  clean): 0x58bd60 ai_ProductionWindow ("Handel\Handel_PRODUKTION"), 0x595480 ai_TradeSearchWindow
  ("HANDEL\HANDEL_SUCHEN"), 0x5aab80 ai_PersonnelBook, 0x5b26b0 ai_StorageWindow, 0x5c3200
  ai_MasterLetterWindow, 0x5c1130 ai_AmountMenu ("special_amt1"). The definitions still carry their ROUGH
  drafts; the names come from the units' own window vocabulary and the contact captions.

## match-base 2026-10-26 (base worker, main checkout)
- ai_InventoryHas (0x5ba6b0, was ai_Unk5ba6b0) **byte**: the ROUGH stub's body written from the 80-byte
  disassembly (the stub also called gm_Query with the wrong arguments - `(ch + 0x178, 0, id)` instead of
  `(ch->inventory, 1, 0, id)`): the inventory object of the id must have a 0x62c420 record (0x5ba620) and a
  non-zero +0x21. unk_58bbd0.h gained its two views G8INVCHAR / G8INVOBJ (pack(1)). Unit 82 -> 83 matched of
  481, 121798 -> 121775 differing bytes. The name: cm.c's comments call the ids skills ("the skill 0x16f" /
  0x2b5) and 0x4ffe90 finds them in the character's inventory (AICHAR.inventory at +0x178).

## match-base 2026-10-26 (base-3 worker, shard 3/6)

unk_58bbd0.c includes <d3d.h>, d3.h and d3_light.h; the two ai_Unk453280 K&R prototypes are gone and the call uses d3_RelightObject. No byte change.

## match-base 2026-10-26 (base-4 worker, part 2)
- Contact listeners / helpers raised to byte (or behaviour) + readable:
  - **0x595ed0 ai_PerfumeryContact: behaviour + readable** (was draft + draft; it was 81 code hunks before the
    readable rewrite, so no byte regression). Params building / room, ai_BuildingPerms / ai_ClickedObject, the
    contact text slots as externs, AIBLD.objList typed in the unit's local packed AIBLD view.
  - **0x596060 ai_IdleContact**, **0x596080 ai_AquariumContact** (the aquarium also named 0x595a00 ->
    ai_AquariumSearchWindow, "the trade search window of the kind 0x15"), **0x59c6e0 ai_UsedRecordCount**,
    **0x5a3b20 ai_DungeonContact** (+ 0x5a3a40 ai_DungeonPlanWindow), **0x5a39b0 ai_ChurchContact**
    (+ 0x5a1f70 ai_IndulgenceWindow, 0x5a28f0 ai_Top5Window), **0x592230 ai_UpgradeContact** (+ 0x591bf0
    ai_TechTreeWindow, also renamed in sv.c), **0x5a6b50 ai_FormatCityName**, **0x5a3ef0
    ai_DungeonEmptyListener**, **0x5a4f50 ai_ThievesGuildEmptyListener**, **0x596f90 ai_MistressContact**
    (+ 0x596ba0 ai_MistressWindow), **0x59ad00 ai_WineCellarContact** (+ 0x59aae0 ai_WineCellarWindow).
- include/unk_58bbd0.h gained the contact text slots (0x14401b4, 0x1442098, 0x1441f90, 0x1441fa0, 0x143c9f0,
  0x143cbc8, 0x143cf58, 0x143cf78, 0x143cf50, 0x1440870, 0x1440768, 0x144073c), ai_CityNameFormat 0x61168c
  ("%1G") and prototypes for ai_StorageWindow / ai_PersonnelBook / ai_MasterLetterWindow (Q7 of the perfumery
  listener called them through K&R declarations).

## play 2026-10-29 (the map window record was 0x27a8 bytes short)

- `ai_Unk5b2180` (0x5b2180) declared `int win, id` for the map window record, but `ai_Unk5a78a0` stores the
  saved background tile at `m_ + 0x27a8`; the original's frame is 0x27b0 bytes (`mov eax,0x27b0; call
  __chkstk`), the record 0x27ac bytes at frame+4. Opening the map window wrote past the caller's frame into
  uncommitted stack (`write of 0x001b1d38`, ai_Unk5a78a0+0x15).
- The prologue was read again while fixing it: the request memcpy source is **param_2** (`[esp+0x27c0]` at
  5b218c, before the edi push = the second argument), param_1 is the record's flags (`mov [esp+0x14],esi` at
  5b21c2 = record+4, which ai_Unk5a78a0 tests), param_3 the title (copied to record+0x5c by the hand-rolled
  strcpy). The local is now one struct: view +0, flags +4, screen +8, req[0x14] +0xc, name[0x68] +0x5c,
  pad, tile +0x27a8 (0x27ac bytes). The `win` pointer passed to ai_Unk5a7cd0 is `&map.flags` (record+4, the
  original's `lea ecx,[esp+0x14]`). 2 fewer differing instructions; the dispatch is still the ROUGH comment.

## play 2026-10-29/30 (the panel selection resolves in the game frame; the dice window's character stride)

- `ai_GameFrame` (0x591520) now has the panel block 0x591986..0x591a14 before the frame pump (the comment
  already listed it): `sv_PanelFlag` set -> `sv_PanelRecord == 0` focuses the camera on `sv_PanelObject`
  (`sv_Unk527840`) and clears the flag; with a record it stores the record word in +0x29 and dispatches on the
  object's +0x5a bit 0 / `gm_IsHomeType` / `gm_IsType10` into `ai_Unk59ee40` / `ai_Unk59e420` or only
  `sv_Unk527bb0` + `sv_Unk527840`. Without it the tutorial's panel step left `sv_PanelFlag` at 1 forever
  (ai_FrameWait alternate 0/1, no draw). 323 -> 143 asm hunks; the byte debt grew 46 (ours 1472 vs the
  original 1445) because the draft is now over-long rather than short - the statements are the original's.
- `ai_Unk598170` (the tavern dice window) took the character record as `ai_Chars + player * 0x438`; the
  original computes `0x770b80 + player * 0x21c` (0x5981a5: `lea eax,[eax+eax*2]` / `*5` / `*9` / `[eax*4+0x770b80]`),
  i.e. `ai_Chars + player * 0x21c`. Fixed for its `gm_PlayerRecord` and `ai_Unk493200` calls (-42 asm). The
  same `* 0x438` transcription is still in the drink / offer windows (0x599240's `gm_Unk506480` and 0x506xxx).

## play 2026-10-29/30 (the remaining `* 0x438` character strides)

The rest of the same transcription bug is fixed: `0x5973f0` (the wine-cellar window: `gm_Unk506480` and
`gm_AddHealth` on the drink record), `ai_Unk5992c0` and `ai_Unk5996b0` (the offer windows:
`unk_Unk4f4d30` on the seller and the item record `items[i * 0xe]` written as a character address) now use
`ai_Chars + n * 0x21c`, the `0x770b80 + 540 * n` the originals compute (same `lea`/`[eax*4+0x770b80]` shape as
0x5981a5); the comments that repeated the 0x438 stride are corrected too. No grade fell: `ai_Unk599240`
(the regulars' table selector) is now **byte**-matched (+0x8d is the only site that changed) and `ai_Unk5996b0`
moved closer (298 asm, -8); `0x5973f0` / `ai_Unk5992c0` stay draft with their pre-existing code hunks. The
whole run is unchanged (the tutorial does not take these windows), but the addresses are no longer 540 bytes
past a 540-byte record.
