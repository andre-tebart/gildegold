# Layout sessions (skill: layout)

Solo sessions that find translation-unit boundaries and write draft source for every function of each unit.
Units computed `separated` (proven or plausible; `uv run ledger units`) go to match sessions (skill `match`, queue `uv run ledger grade --queue --debt`).

## Frontier
The frontier is computed (`uv run ledger next`: the first undrafted byte); below are the leads for the next
frontier functions, kept by hand.
Session 16 (2026-10-23) closed he.c: drafted 0x575710 (the message filter bit setter, the setter side of
he_Unk575640; nonmatching, the original keeps the mask in the hoisted eax and uses byte ALU). The work after it is
the new unit **unk_575860.c** (0x575860 .., name unknown, `@boundary plausible`): drafted 0x575860 (the end-of-game
tick, 1984 B, ROUGH SKELETON: the state record 0x5eec60 and its state switch, jump table 0x575fe8, states 1 .. 0xc;
the case bodies are comments), 0x576020 (matched: clears the gate 0x6f2660), 0x576030 (matched: sets it),
0x576040 (the state step, nonmatching), 0x576340 (nonmatching), 0x5763c0 / 0x576410 / 0x576460 (matched) and
0x576490 (a Ghidra-missed start: the callback predicate, matched). Then the callback / predicate family
0x5764b0 .. 0x577540 was drafted (the functions Ghidra merged into its 1776 B record at 0x5764b0 and its 1040 B
record at 0x577330; starts found by a ret + nop scan, see the unit note): matched first try 0x5765e0, 0x5768a0,
0x576a80, 0x5770a0, 0x577320, 0x577490; the rest nonmatching (the family shares one codegen pattern, near-misses
of 8 .. 45 bytes, see the unit's Open work). Then 0x5775c0 / 0x577610 (two character-record message helpers,
nonmatching) and 0x577690 (a save-record builder, 176 B, ROUGH: the record layout from the disassembly).
Then 0x577740 (the first function with the unit's literals, "tutorial\main" 0x629304; matched first try),
0x577820 (0x53d800) and 0x577850 (the run window controls), both matched, and 0x5778b0 (326 B, the run window's
text tick; nonmatching, ROUGH on the window recreation path). Session 17 drafted 0x577b90 .. 0x578570 and the unit
turned out to BE the tutorial module: 0x578180 (called from main.c 0x53bb20) initializes the state record 0x5eec60
that the tick 0x575860 reads; 0x577cd0 / 0x577e50 / 0x578180 use the "tutorial\*" forms and the TUTORIAL_* texts.
Matched: 0x577c60, 0x577cd0, 0x577d60 (a Ghidra-missed start), 0x578120, 0x578150, 0x578170, 0x578440. Near-misses:
0x577b90 (9/208: the load order before the push and the eax choice), 0x577d90 (93/128: the `MIN(TDIFF ? TDIFF : 0,
total)` shape - the original compiles the `?:` twice and uses `fild DWORD` while the compare is unsigned),
0x5780e0 (8/64: the operand load order; swapping the C operands makes it 25/64), 0x578370 (10/208: only the
`mov ecx, ds:0x5eec60` scheduling against the `0x6f2658 = 0` store). ROUGH, drafted: 0x577e50 (632 B, chapter form),
0x578180 (493 B, the tutorial start), 0x5784a0 (the intro record), 0x578570 (2254 B, the chapter 1 chain: eleven 0x74-byte records generated mechanically
from the disassembly), 0x578e40 / 0x579570 / 0x57aa80 / 0x57aeb0 (the chapter 2 .. 5 chains, the same generator:
9 / 18 / 5 / 9 records, the sample banks into 0x6f2648 .. 0x6f2654), 0x57b750 (the main-outro record) and
0x57b810 (the cubic Bezier; nonmatching 91/144, the term order). Next: 0x57b8a0 (910 B, the tutorial per-frame
update: the callback r+0x60 with the argument [esp+0x18], the -1 / range check, S+0x58 = 2, S+0x5c = the clock,
S+0x54 = the value, then 0x472f50) - drafted ROUGH, then 0x57bc30 (the info block 0x75e5a0) and 0x57bce0 (the
chapter window callback, ROUGH SKELETON: the five chapter texts, the window table 0xf8f080, the pump 0x5309a0;
the input tail is comments) and 0x57bf00 (the results window, ROUGH SKELETON: the three player records via
0x4ec2d0 / 0x4ec320, the window / controls / pump 0x5309a0 left as comments) and 0x57c560 (the result string
writer, ROUGH SKELETON: 0x4eaa50, 0x521040 on the player user record 0x770b80 + U16(0x6f25b8) * 0x21c, the
0x5176d0 / 0x521150 string, 0x4ec490; the tail is comments) and 0x57c690 (the selection handler, ROUGH
SKELETON: 0x4eaa50, the record +0x65, 0x4eb180, the clock, the window 0x62a284, 0x476f80 / 0x475600; the rest is
comments) and 0x57cde0 (a message builder, ROUGH SKELETON: 0x4eb180, the 0xf8 struct, the clock globals, the
character record 0x770b84, 0x42ec00 (0x5a0); the rest is comments) and 0x57ced0 (its sibling: byte 0x95,
0x4c4b70, returns 1; ROUGH SKELETON), 0x57c690 (the selection handler, ROUGH SKELETON) and 0x57cf90 (the
window handler, ROUGH SKELETON: 0x4eaa50, the record +0x65, 0x4eb180, the slot fill at esp+0x2c, the window
0x62a2a4, 0x476f80 / 0x475600 / 0x475f70; the rest is comments). Session 18 drafted 0x57dc50 .. 0x583700 (31
functions, the tutorial's location / contact windows; the unit extent is now 0x583700): matched first try 0x57f5d0,
0x580a50, 0x580fe0, 0x5830f0; ROUGH skeletons still to fill 0x57e270 (the cannon scene, 2305 B), 0x57f0e0 (the
camera), 0x580160 (the explosion spawn), 0x582b40 (the detail widgets) and 0x583240 (the graveyard window's click
dispatch); near-misses 0x57fdb0 (11/32), 0x57e230 (rep movs vs indexed copy), 0x57dc50 (196/528, the message
record slot base). The family is the "Locations_*" contact code: dispatchers 0x5821d0 / 0x582600 with the contact
ids 0x52e1c0, the spy windows 0x581020 .. 0x581db0 (message type 0x99), the window list type (0 window, 2 / 3
screen, 4 .. 6 fonts, 0x4d entry, 0x4e detail, 0x50 items, 0x51 widgets), and the literals climb in one block
0x62a2a4 .. 0x62a580 (data order, no new boundary). Next: 0x583700 (1123 B, the function after the graveyard
window).
Session 19 (2026-10-23, worktree) drafted 0x584410 .. 0x586840 (17 functions, the graveyard contact / seance
family). Matched first try: 0x5867c0 (the "Schlag" action; its definition takes an unused char * because the
call site pushes one). Nonmatching with complete calls: 0x584410 (the two-button confirmation), 0x584520 (the
grave action), 0x5847a0 (the rent-a-grave selection), 0x584900 and 0x584b70 (both Ghidra-missed starts: the
0x8d grave action and the "Gehilfen erschaffen" window), 0x585380 (the Fog window), 0x585750 (the seance
selection), 0x5859c0 / 0x585c40 (the Seance2 ritual and its formula filler), 0x585cb0 / 0x585f30 / 0x586200
(the seance outcomes: GEIST_KOMMT / the sacrifice / the 15-item lottery), 0x586560 (the selection window),
0x586610 (the seance driver) and 0x586840 (the contact dispatcher). ROUGH: 0x584d70 (1549 B, the
"HANDEL\HANDEL_SUCHEN" helper window) and 0x586200 (the lottery, the 0x4a5760 array layout taken from am.c).
Near-misses: 0x586560 (88/176), 0x5847a0 (119/352), 0x584410 (229/272), 0x585cb0 (213/640, after the float
prototypes of 0x4f2660 / 0x42ec20 were fixed). The literals climb 0x62a5bc .. 0x62a718 in one block (data
order, no new boundary). Next: 0x586ab0 (902 B, the HEIMSUCHUNG handler; decoded, see the unit note), then
0x586e40, 0x5871e0, 0x5871c0, 0x587300 / 0x587340 / 0x587370, 0x587c40 / 0x587c90 and 0x5912c0.
Session 20 (2026-10-23, worktree) drafted 0x586ab0 .. 0x589670 (30 functions): the HEIMSUCHUNG and
ALTERN_LASSEN contacts (0x586ab0 / 0x586e40, the 0x5c1130 query and the 0x88 / 0x89 messages), the contact
stubs 0x5871c0 / 0x587300 / 0x587340 / 0x587560 / 0x5876a0 / 0x587ad0 (0x583240 with the callbacks 0x584900 /
0x584b70, the contact lists 0x62a818 .. 0x62a88c), 0x5871e0 / 0x587370 / 0x587700 / 0x587810 / 0x587b70 (the
graveyard detail windows and the building flag mask), then the item drag-and-drop family 0x587de0 .. 0x589670
(the drag handler 0x587e00, the tooltip windows 0x588160 .. 0x588c60, the slot table 0x6f5ec0 (0x589400 /
0x589410 / 0x589450 / 0x5894d0 / 0x589570) and the item-slot window 0x589670). Matched first try: 0x5871c0,
0x587300, 0x587340, 0x5876a0, 0x587de0, 0x589400, 0x589410. The rest are drafts; the big window builders
(0x587e00, 0x588310, 0x588160, 0x588950, 0x588bb0, 0x588c60, 0x5890c0, 0x589670) are ROUGH SKELETONs, see
notes/units/unk_575860.md. Next: `uv run ledger next` = 0x589aa0 (142 B, a call from 0x5190b0), then
0x589b30, 0x589bc0, 0x589c90, 0x589cf0, 0x589e00, 0x589e50 (a call from main.c 0x53bb20) and 0x589f10
(2595 B).
Session 20 continued past 0x589670 to 0x589cf0: the slot-table loaders 0x589aa0 / 0x589b30 (0x430e30 /
0x51c920), the screenshot 0x589bc0 (0x4223c0 / 0x478c50 / 0x401b90 / 0x422620) and the slot keys 0x589c90.
Session 21 (2026-10-23, worktree) closed unk_575860.c at 0x589f10 and opened two units after it: drafted
0x589cf0 (the speed keys, matched first try: the game speed 0x6f3608 in steps of 40, the keys 0x4e / 0x4a,
0x4c3f20 / 0x4c3f50 and 0x5d5e90, the network mode 0x6f23f8 & 4 / & 0x10, the screenshot key 0x1f, 0x5d7430
for the key 0x10), 0x589e00 (the slot fill, matched: 0x501b50 / 0x501ce0 into the table 0x6f5ec0 + i * 0xc)
and 0x589e50 (the building slots: the market 0x6f1bc8, the town hall 0x501190 (1, 5, 0xf), the home +0x170,
the player's buildings 0x501190 (1, 4, i) / 0x501030; nonmatching 43/192, only the kind-record address
computation block differs - the 7 asm lines around the table 0x7e3f30 + kind * 0x24d access; every spelling
variant tried (byte pointer, local pointer, offset local, loop form, types) gives identical code, so the cause
is outside the block - for the match session). Then the unit **unk_589f10.c** (the network lobby) and
**unk_58bbd0.c** (the production window) were opened, see below.
he.c (0x53d430 ..): drafted 0x53d430 .. 0x56e460 (session 11 added 0x5690c0 .. 0x56ba97; session 12 added 0x56bac0, 0x56bc50, the 0x56be30 ROUGH SKELETON (the run of the construction type 0xd, states 0 .. 7 from the jump table 0x56cda8, most case bodies only comments), 0x56cdd0 (matched), 0x56ce40, the module inits 0x56d050 (matched) / 0x56d220 (matched), the research handlers 0x56d2d0 / 0x56d3d0 / 0x56d610 / 0x56d730 / 0x56d990 / 0x56daa0 / 0x56dd20 / 0x56ddc0, 0x56de10 / 0x56e070 and 0x56e400 (matched) and 0x56e460 (matched); session 13 added 0x56e5b0 (the run of 0x99, 2512 B, ROUGH from the disassembly: the states -1 / -2 free path with the text 0x628dd4, the outer switch on +0x70 (table 0x56ef80), the building kind +0xbc, the texts 0x628dc8 / 0x628dec via 0x4c52e0 / 0x53dda0, the domicile search 0x4a63f0 / 0x56f600 / 0x56f740; nonmatching 1850/2752 B — asmdiff: control flow right, diffs are register allocation and unshared epilogue tails), 0x56efc0 (the alloc of 0x9a), 0x56f040 (the free forwarder, 5 B: jmp 0x53e1f0, matched), 0x56f050 (the alloc of 0x9b, matched) and 0x56f0b0 (the run of 0x9b)). Next: `uv run ledger next` = 0x572fd0 (the run of the type 0x94, 558 B: the state dispatch -2 .. 0, the character +0xac (0x505df0), its boss building +0x16c, 0x50c520 (the building word +0x27, the character word) against -0x1a, the helper pair 0x4ffe90 (+0x5d, 2, 6, 0, 0x2a / 0x116), the group walk 0x4ffe90 (x, 1, 5) / 0x4ffcc0, a random pick 0x42ec00, 0x5089c0, 0x4c4880, 0x5190a0, the text 0x414b and the message 0x53dee0 (0x5f2, 0x629000); the disassembly is in build/dis572bf0.txt after 0x572fd0) then 0x572fd0 (the run of 0x94), the inits 0x573260 / 0x5733c0 and the stretch 0x573400 .. 0x575700 (the rest of the module declared in 0x553060). Session 14 drafted 0x56f170 .. 0x572b90: matched 0x56f170, 0x56f1b0, 0x56f2e0, 0x56f360, 0x56f3b0, 0x56f980 (the init of 0x88 .. 0x94, 576 B), 0x56fc50, 0x570130, 0x570bf0, 0x570c70, 0x570ea0, 0x571990, 0x5725b0, 0x5726a0, 0x572860, 0x572b90; drafted nonmatching: 0x56f260 (94/128: register swap want / round and the dec placement), 0x56f3f0, 0x56f600, 0x56f740, 0x56fbc0 (91/128: the original spills (float)num to a stack slot and hoists the +0x18 load; ours keeps it in st0), 0x56fda0 (the run of 0x88, 912 B ROUGH), 0x5702e0, 0x5706e0 (127/192: the mask 4 is hoisted into eax in the original, a constant web), 0x5707a0 (the run of 0x8a, 992 B ROUGH), 0x570cc0, 0x570ef0, 0x571000 (the run of 0x8c, 1040 B ROUGH: the night window 0x617840 / 0x617830, the four characters +0x8c .., the texts 0x421e / 0x4128, the message skeleton), 0x571550, 0x571640, 0x5719e0, 0x571ac0 (80/352), 0x571c30, 0x571f60, 0x5720f0 (the skill lottery, 592 B), 0x572310 (the meeting search, 656 B), 0x5726b0, 0x5728d0, 0x5729c0, 0x572a50, 0x572bf0 (the run of 0x93, 1248 B ROUGH). The 0x56efc0 draft (session 13) now uses he_Unk56f360 / he_Unk56f600 / he_Unk56f740 (renamed from ai_Unk*). Session 15 drafted 0x572fd0 .. 0x575640 (the events 0x94, 0x9d .. 0xa4): matched 0x573260 / 0x573280 / 0x5732d0 (the 0xa2 init / alloc / run, the daily Hansa politics: 0x4ace40 / 0x4ad0a0 / 0x4ad1c0 / 0x4ad510 / 0x4ad580, 0x4fe140 / 0x4fe1e0), 0x5733c0 (the init of 0x9d .. 0xa1, 0xa3, 0xa4: pairs (0x5734b0, 0x573580), (0x573f20, 0x573fb0), (0x574060, 0x5740d0), (0x574120, 0x5743c0), (0x574550, 0x5748d0), (0x575150, 0x5751e0), (0x575320, 0x5753a0), the text 0x50f of 0xa1), 0x5734b0, 0x573580 (the 0x9d event: 0x4f2e30, 0x5736d0 / 0x573e30), 0x5736d0 (the approach dispatch), 0x573f20, 0x574060, 0x5740d0, 0x5740f0 / 0x5741d0 (searches), 0x574120 (0x6f2448 gate), 0x575150 / 0x575320 / 0x575540 / 0x575570 (the 0xa3 / 0xa4 allocs and outcomes), 0x5755c0 / 0x575600 (the INI path 0x6fa0e0 and "Game" / "message_filter_mask" 0x6f263c). Drafted nonmatching: 0x572fd0 (the run of 0x94), 0x573200 (a type 0x8d search), 0x573760 / 0x573820 / 0x573970 / 0x573ae0 / 0x573c20 / 0x573d30 (the six approaches of 0x9d: texts 0x41db .. 0x41e3 / 0x5fa .. 0x5fc, the name blocks 0x62901c / 0x629038, 0x4c6020 on per +0x80 .. +0x84), 0x573e30, 0x573fb0 (the 0x9e pair: per +0x194), 0x5741f0 / 0x5743c0 / 0x574550 (0xa0 / 0xa1: 0x4fea50, 0x502020, 0x503070?), 0x5746b0 (0xa1 scene), 0x5748d0 (0xa1 run, ROUGH SKELETON, 1802 B), 0x574fe0 (the meeting room), 0x575030 (the 0xa1 income), 0x5751e0 (the 0xa3 run), 0x5753a0 (the 0xa4 run), 0x575430 (the 0xa4 outcome), 0x575640 (the message filter switch 0x6f263c, bits 1 .. 0x100 for the kinds 0x605 .. 0x60d). Next: `uv run ledger next` = 0x575710 (288 B, ghidra). NOTE: run `uv run backlog` over EVERY unit range and the aligned-start scan over every unit.
Session 21 opened, beside the above: the network lobby **unk_589f10.c** (0x589f10 .. 0x58bbd0: the profile
"Network" 0x75e480 / the player record 0x6f5e20, the "menu\netlobby" screen 0x589f10, the choose-player and
choose-profession pickers 0x58af50 / 0x58b880, the chat lines 0x58ba70) and the production window
**unk_58bbd0.c** (0x58bbd0 ..: the table init 0x58bbd0, "Handel\Handel_PRODUKTION" 0x58bd60, the goods
columns 0x58c680, 0x58cc10, 0x58d720, 0x58dd00). Frontier: `uv run ledger next` = 0x58c680 (1421 B,
the column builder of the production window: the tables 0x6f5320 / 0x6f5350 (0x58 bytes per column), the four
goods rows 0x472aa0 (0x43, 4, 0x88, 0x32, 0x10), the availability 0x5098a0, the carrier check 0x53e400 /
0x53e300, the time ratio 0x4fe280, the screen 0x6f536c). Session 21 continued past it and drafted the whole
stretch 0x58c680 .. 0x58f290 ROUGH (0x58c680, 0x58cc10, 0x58d720, 0x58dd00, 0x58e130, 0x58e440, 0x58e6a0,
0x58ee30, 0x58f290; see notes/units/unk_58bbd0.md). Next: 0x58f510..0x5de450  323392 B  (after the last unit) 0x58f510 264B (ghidra); reached: 0x5a8f90 called from 0x47ddf0 (ai_Unk47ddf0); 0x5a9050 called from 0x47ddf0 (ai_Unk47ddf0); 0x5a8ee0 called from 0x47ddf0 (ai_Unk47ddf0); 0x5a8870 called from 0x47edb0 (ai_Unk47edb0) = 0x58f510 (264 B),
then the rest of the production / trade module.

he.c (0x53d430 ..): drafted 0x53d430 .. 0x56e460 (session 11 added 0x5690c0 .. 0x56ba97; session 12 added 0x56bac0, 0x56bc50, the 0x56be30 ROUGH SKELETON (the run of the construction type 0xd, states 0 .. 7 from the jump table 0x56cda8, most case bodies only comments), 0x56cdd0 (matched), 0x56ce40, the module inits 0x56d050 (matched) / 0x56d220 (matched), the research handlers 0x56d2d0 / 0x56d3d0 / 0x56d610 / 0x56d730 / 0x56d990 / 0x56daa0 / 0x56dd20 / 0x56ddc0, 0x56de10 / 0x56e070 and 0x56e400 (matched) and 0x56e460 (matched); session 13 added 0x56e5b0 (the run of 0x99, 2512 B, ROUGH from the disassembly: the states -1 / -2 free path with the text 0x628dd4, the outer switch on +0x70 (table 0x56ef80), the building kind +0xbc, the texts 0x628dc8 / 0x628dec via 0x4c52e0 / 0x53dda0, the domicile search 0x4a63f0 / 0x56f600 / 0x56f740; nonmatching 1850/2752 B — asmdiff: control flow right, diffs are register allocation and unshared epilogue tails), 0x56efc0 (the alloc of 0x9a), 0x56f040 (the free forwarder, 5 B: jmp 0x53e1f0, matched), 0x56f050 (the alloc of 0x9b, matched) and 0x56f0b0 (the run of 0x9b)). Next: `uv run ledger next` = 0x572fd0 (the run of the type 0x94, 558 B: the state dispatch -2 .. 0, the character +0xac (0x505df0), its boss building +0x16c, 0x50c520 (the building word +0x27, the character word) against -0x1a, the helper pair 0x4ffe90 (+0x5d, 2, 6, 0, 0x2a / 0x116), the group walk 0x4ffe90 (x, 1, 5) / 0x4ffcc0, a random pick 0x42ec00, 0x5089c0, 0x4c4880, 0x5190a0, the text 0x414b and the message 0x53dee0 (0x5f2, 0x629000); the disassembly is in build/dis572bf0.txt after 0x572fd0) then 0x572fd0 (the run of 0x94), the inits 0x573260 / 0x5733c0 and the stretch 0x573400 .. 0x575700 (the rest of the module declared in 0x553060). Session 14 drafted 0x56f170 .. 0x572b90: matched 0x56f170, 0x56f1b0, 0x56f2e0, 0x56f360, 0x56f3b0, 0x56f980 (the init of 0x88 .. 0x94, 576 B), 0x56fc50, 0x570130, 0x570bf0, 0x570c70, 0x570ea0, 0x571990, 0x5725b0, 0x5726a0, 0x572860, 0x572b90; drafted nonmatching: 0x56f260 (94/128: register swap want / round and the dec placement), 0x56f3f0, 0x56f600, 0x56f740, 0x56fbc0 (91/128: the original spills (float)num to a stack slot and hoists the +0x18 load; ours keeps it in st0), 0x56fda0 (the run of 0x88, 912 B ROUGH), 0x5702e0, 0x5706e0 (127/192: the mask 4 is hoisted into eax in the original, a constant web), 0x5707a0 (the run of 0x8a, 992 B ROUGH), 0x570cc0, 0x570ef0, 0x571000 (the run of 0x8c, 1040 B ROUGH: the night window 0x617840 / 0x617830, the four characters +0x8c .., the texts 0x421e / 0x4128, the message skeleton), 0x571550, 0x571640, 0x5719e0, 0x571ac0 (80/352), 0x571c30, 0x571f60, 0x5720f0 (the skill lottery, 592 B), 0x572310 (the meeting search, 656 B), 0x5726b0, 0x5728d0, 0x5729c0, 0x572a50, 0x572bf0 (the run of 0x93, 1248 B ROUGH). The 0x56efc0 draft (session 13) now uses he_Unk56f360 / he_Unk56f600 / he_Unk56f740 (renamed from ai_Unk*). Session 15 drafted 0x572fd0 .. 0x575640 (the events 0x94, 0x9d .. 0xa4): matched 0x573260 / 0x573280 / 0x5732d0 (the 0xa2 init / alloc / run, the daily Hansa politics: 0x4ace40 / 0x4ad0a0 / 0x4ad1c0 / 0x4ad510 / 0x4ad580, 0x4fe140 / 0x4fe1e0), 0x5733c0 (the init of 0x9d .. 0xa1, 0xa3, 0xa4: pairs (0x5734b0, 0x573580), (0x573f20, 0x573fb0), (0x574060, 0x5740d0), (0x574120, 0x5743c0), (0x574550, 0x5748d0), (0x575150, 0x5751e0), (0x575320, 0x5753a0), the text 0x50f of 0xa1), 0x5734b0, 0x573580 (the 0x9d event: 0x4f2e30, 0x5736d0 / 0x573e30), 0x5736d0 (the approach dispatch), 0x573f20, 0x574060, 0x5740d0, 0x5740f0 / 0x5741d0 (searches), 0x574120 (0x6f2448 gate), 0x575150 / 0x575320 / 0x575540 / 0x575570 (the 0xa3 / 0xa4 allocs and outcomes), 0x5755c0 / 0x575600 (the INI path 0x6fa0e0 and "Game" / "message_filter_mask" 0x6f263c). Drafted nonmatching: 0x572fd0 (the run of 0x94), 0x573200 (a type 0x8d search), 0x573760 / 0x573820 / 0x573970 / 0x573ae0 / 0x573c20 / 0x573d30 (the six approaches of 0x9d: texts 0x41db .. 0x41e3 / 0x5fa .. 0x5fc, the name blocks 0x62901c / 0x629038, 0x4c6020 on per +0x80 .. +0x84), 0x573e30, 0x573fb0 (the 0x9e pair: per +0x194), 0x5741f0 / 0x5743c0 / 0x574550 (0xa0 / 0xa1: 0x4fea50, 0x502020, 0x503070?), 0x5746b0 (0xa1 scene), 0x5748d0 (0xa1 run, ROUGH SKELETON, 1802 B), 0x574fe0 (the meeting room), 0x575030 (the 0xa1 income), 0x5751e0 (the 0xa3 run), 0x5753a0 (the 0xa4 run), 0x575430 (the 0xa4 outcome), 0x575640 (the message filter switch 0x6f263c, bits 1 .. 0x100 for the kinds 0x605 .. 0x60d). Next: `uv run ledger next` = 0x575710 (288 B, ghidra). NOTE: run `uv run backlog` over EVERY unit range and the aligned-start scan over every unit.
main.c (0x531670 .. 0x53d426): fully drafted (session 10 filled 0x532da0, 0x5359f0, 0x5361d0, 0x536980, 0x537230, 0x537b60). Earlier units (sv.c, gv.c, ord.c, f3gm.c, f3init.c, io_text.c, ls.c) are fully drafted (see their notes).
Tooling: `batch_decompile` returns inline when small; a Ghidra function whose float arguments are lost (`__ftol()` with no argument) needs the disassembly. Python scripts: Windows python cannot read git-bash `/tmp` or `/c/...` paths inside `python -` code (use `C:/...`); a bash heredoc with an apostrophe (`player's` in a C comment) can break the whole command even when quoted, so write C files with the Write/Edit tool; a backslash in a python string needs `chr(92)`. A call-site `jbe` / `ja` after `test eax,eax` means the callee's result is unsigned (cast the function to `unsigned (*)()`); a double constant (`* 0.001`) vs float (`0.001f`) shows as `fmul QWORD` vs `DWORD`; a message struct local of 0x15 bytes gives a 0x18 frame, the original's 0x20 frame needs `char m[0x1d]`. Handler families (jobs / levels / flags) are generated from templates: see the matched ones in ord.c. The 14-byte game clock is copied with a STRUCT assignment from the named extern (`*(HETIME *)(r + 0x52) = ai_Time;`, pack(2) struct {int; ushort; int; int}): it matches the `lea ecx` + `mov [ecx+4]` copy of the original; field-by-field copies from absolute addresses do not (he.c 0x541200, 0x543180, 0x543020, 0x542960 ...).

Session 2026-10-24 (worktree) drafted the frontier 0x58f510 .. 0x599aa0 into unk_58bbd0.c (the unit now runs
0x58bbd0 .. 0x599af0, 89 functions, 13 matched): the building purchase contacts and placement (0x58f510 ..
0x5909a0), the game frame 0x591520 / tech tree 0x591bf0, the trade search windows (0x5923c0 / 0x592b90 /
0x593150 / 0x595480 / 0x595a00), the market / kontor / criminal listeners (0x593960 .. 0x596f90) and the
Wirtshaus / locations module (0x5967c0 .. 0x599aa0: the mistress, master exam, wine cellar, dice game, tavern
and dark corner windows). Matched first try: 0x58f620, 0x58f920, 0x58fb60, 0x58feb0, 0x592230, 0x594180,
0x594470, 0x594790, 0x594b10, 0x594db0, 0x596060, 0x596080, 0x596f90. The literals climb 0x62b038 .. 0x62b83c
in function order (one .data block; data order D8, no new boundary). Next: `uv run ledger next` =
0x599af0 (440 B, a Ghidra function with an address-taken callback at 0x599b04), then the rest of the
Wirtshaus / locations module; the unit's extent and its name (a leaked name would settle it) are open.

Session 2026-10-24 (worktree, second) drafted 0x599af0 .. 0x5a1a60 into unk_58bbd0.c (51 functions; the unit now
runs 0x58bbd0 .. 0x5a1af0, 143 functions, 16 matched): the tavern dice game / regulars' table listener 0x599af0
(0x120) and the fair hall 0x59a560 (0x1f), the city info window 0x599ef0 / 0x599e00 and the three city points
0x59a2c0 / 0x59a3c0 (the "stadt_%s" object names at D3OBJECT +0), the market stand listener 0x59a800 and the
market leave 0x59a710 (the id ladder 0x92 .. 0x2db), the wine cellar 0x59aae0 / 0x59ad00, the money lender
family 0x59ad60 .. 0x59d410 (the credit list 0x59b160 / 0x59b630, the release 0x59b880, the credit giver
0x59bad0, the exchange 0x59c120 / 0x59c450 / 0x59c700, the fees 0x59d2d0 and the messenger 0x59d410; the
"locations\geldleihe\*" windows), the fortune overview 0x59df10 with its Ghidra-missed callback 0x59dc00, the
money lender listener 0x59df80, the room switch 0x59e0a0, the opening hours table 0x62afa0 / 0x59e170 and the
enter check 0x59e210 + the building drivers 0x59e420 / 0x59ee40 / 0x59f410 (their room dispatch switches list
the handlers of the rooms 0x13 .. 0x29e, including this unit's listeners), then the Rathaus family 0x59f9a0 ..
0x5a1a60 (the guild hall 0x59f9a0, the guild cash 0x59fbf0 / 0x5a0950, the citizen right 0x59fe40, the office
application 0x5a0220 / 0x5a0590 / 0x5a0f10 with the missed callbacks 0x5a0ca0 and 0x5a13f0 / 0x5a1530 /
0x5a1670, the law book listener 0x5a07d0, the guild level windows 0x5a1000 / 0x5a17b0 and the listeners
0x5a18d0 / 0x5a1960 / 0x5a1a60). The literals climb 0x62b038 .. 0x62bf3c in function order (one .data block,
D8, no new boundary). Matched this session: 0x59ad00, 0x59c6e0, 0x59fbf0, 0x59fca0, 0x59fd30, 0x5a18d0,
0x5a1a60 (and 0x58f510 flipped to byte with the U_W macro fix). Ghidra-missed starts found by the ledger after
the builds: 0x59dc00, 0x5a0ca0, 0x5a13f0, 0x5a1530, 0x5a1670 (all drafted). Next: `uv run ledger
next` = 0x5a1af0 (1141 B, the church donation window; its pseudocode is at hand, the draft was not started).

Session 2026-10-24 (worktree, third) drafted 0x5a1af0 .. 0x5a2e60 into unk_58bbd0.c (7 functions; the unit now
runs 0x58bbd0 .. 0x5a36c0, 150 functions, 22 matched): the church rooms of the town hall building driver
0x59e420 - the donation window "Locations\Kirche\Spenden" 0x5a1af0 (the relation total max(0x50a640, 1) +
max(0x50a640, 1), the percent * 0.01f, the slider chain 0x5085b0 and the same-religion share 0.2f over the
0x300 character records), the indulgence "Locations\Kirche\Ablass2" 0x5a1f70 (Ghidra's decompiler dies on it:
read from uv run disasm; the price is am.c's 0x49c8e0 formula with 0x4f39e0 / 0x4f39b0 / 0x50c550 /
0x5ba6b0, the clamp 3200.0 .. 320000.0, and 0x5a8b80 / 0x4c47e0 on the click), the baptism
"Locations_Kirche_Taufe" 0x5a23a0 (two paths, the own building * 0xf and another owner's religion), the top-5
window "Locations\Kirche\Top5" 0x5a28f0 (a real Ghidra start, reached by the tail call of 0x5a1f70; the table
0x50b1e0 and the rows 0x5be1d0 / 0x5be720 / 0x5bd1f0), the confession "Locations\Kirche\Beichte" 0x5a2b60
(0x520f90 / 0x4c5f60 / 0x5a8c00), its listener 0x5a2de0 ("contact_BEICHTE", nonmatching 54/144, the closest
of the session) and the sermon "Locations\Kirche\Predigt" 0x5a2e60 (ROUGH SKELETON, the states 0 .. 3, the
request 0x5c1130 / 0x42ec00). No function matched. The literals climb 0x62beb8 .. 0x62bf4c in function order
(one .data block, D8, no new boundary). Matched: 0x5a39b0 (after the cached click id), 0x5a3b20, 0x5a3ef0.
Next: `uv run ledger next` = 0x5a41d0 (777 B, the thieves' guild family), then the rest of the
thieves' guild / dungeon module.

Session 2026-10-24 (worktree, third, continued) drafted 0x5a36c0 .. 0x5a4130 into unk_58bbd0.c (7 more
functions; the unit now runs 0x58bbd0 .. 0x5a41d0, 157 functions, 25 matched): the building listeners
(0x5a36c0 the production building with contact_PRODUKTION_SCHREIBEN / LAGER / TRANSPORT / ob_PERSONALBUCH /
ob_MEISTERBRIEF, 0x5a3810 the church with contact_PREDIGT / ob_WEIHWASSERBECKEN / ob_KIRCHE_BEICHTSTUHLRAUM /
ob_SPENDENKAESTCHEN and the automatic confession, 0x5a39b0 contact_TOP5 / contact_ABLASS), the dungeon
(0x5a3a40 the plan window, 0x5a3b20 its listener, 0x5a3b90 the bribe window, 0x5a3df0 contact_BESTECHEN with
the 0x770d31 flag, 0x5a3ef0 an empty pump) and the thieves' guild burglary (0x5a3f10 the crew AIMSG type 0x3f,
0x5a4130 its request 0x5b2180). Matched first try: 0x5a3b20, 0x5a3ef0; 0x5a39b0 after caching the click id in
a local (`click = U_I(0x6f22e4)` reproduces the original's `cmp ecx,eax`). Near-misses: 0x5a4130 (32/160),
0x5a3a40 (89/224, the line counter increment), 0x5a2de0 (54/144, the same click-id caching), 0x5a3df0
(155/256), 0x5a36c0 (160/368). 0x5a3f10 has no Ghidra procedure (the REA decompile refuses); drafted from
uv run disasm.
Session 2026-10-24 (worktree, third, continued 2) drafted 0x5a41d0 .. 0x5a4b90 (6 more functions; the unit now
runs 0x58bbd0 .. 0x5a4d40, 163 functions, 26 matched): the thieves' guild espionage window 0x5a41d0 (the crew
threshold 2 * the kind byte +0x247, the AIMSG type 0x40), its start 0x5a44e0 (the request record, kind 0x13),
the spy ring window 0x5a45c0 (the 8-row list 0x501190 / 0x5091a0, the list control 0x46c490 .., the window
table 0xf8f080, the payment 0x4c5010), the pickpocket window 0x5a48d0 (AIMSG type 0x49, the carrier lookup
0x53e400 / 0x53e300 and 0x524260) with its start 0x5a4b30 (**matched**: `req[4] = 7;` has to be stored before
`req[2] = 0x90010;`) and the pickpocket victim window 0x5a4b90 (AIMSG type 0x61). Next:
`uv run ledger next` = 0x5a4d40 (522 B, a call from the building driver 0x59e420).
Session 2026-10-24 (worktree, fourth) drafted 0x5a4d40 .. 0x5a6c40 (23 functions into unk_58bbd0.c; the unit
now runs 0x58bbd0 .. 0x5a6cb0, 189 functions, 33 byte, 34 behaviour, contiguous): the thieves' guild kidnapping
(the listener 0x5a4d40, the empty 0x5a4f50, the contact listeners 0x5a4f70 / 0x5a5080, the window 0x5a5120 with
its start 0x5a53e0, the ransom 0x5a5530, the attempt 0x5a5910 and the listener 0x5a5bc0) and the city watch
"LOCATIONS\STADTWACHE\STANDARD" (the request starts 0x5a5f60 / 0x5a61f0 / 0x5a6480 / 0x5a6720, the windows
0x5a5d70 / 0x5a5fd0 / 0x5a6270 / 0x5a64d0, the listeners 0x5a67a0 / 0x5a6920 / 0x5a6a90, the text helper
0x5a6b50 and the map helpers 0x5a6b70 / 0x5a6c40). Matched first try: 0x5a4f50, 0x5a5f60, 0x5a61f0, 0x5a6480,
0x5a6720, 0x5a6b50, 0x5a6c40. Near-misses: 0x5a4f70 (8/272, the two id pairs swapped esi/edi vs ebx/ebp),
0x5a5080 (9/160, param_1 vs the mask constant 0x200), 0x5a53e0 (71/336, the request block's scheduling).
The literals climb 0x62c0d4 .. 0x62c270 in function order (one .data block, D8), so the city watch is the same
TU. The Ghidra-missed starts 0x5a5120 / 0x5a5d70 / 0x5a5fd0 / 0x5a6270 / 0x5a64d0 and 0x5a6b50 (a 24 B helper
the nop scan missed, found by the ledger) are drafted; the REA batch_decompile exports of the stretch are now in
the main checkout's build/ghidra. Next: `uv run ledger next` = 0x5a6cb0 (3056 B, the map module).
Session 2026-10-24 (worktree, fifth) drafted 0x5a6cb0 .. 0x5a8d80 into unk_58bbd0.c (28 functions;
the unit now runs 0x58bbd0 .. 0x5a8e10, 217 functions): the town map window (0x5a6cb0 the per-frame rebuild,
ROUGH 3049 B: the entry array +0xdc, the building directions, the projection 0x458a20 / 0x45f170 / 0x5a6b70,
the pictures 0x4731d0, the name table 0x7e3f40, the ambush spots 0x625720 (flag 4), the effect list 0x6f26b0
(flag 8) and the scroll centre 0x6f2308 / 0x6f230c / 0xfc2dec; the open 0x5a78a0 "Misc\Landkarte2" 0x62c274;
the drag / key scroll 0x5a79e0 / 0x5a7bc0; the per-frame 0x5a7cd0 ROUGH SKELETON 1556 B; the close 0x5a82f0)
and the production settings module (the list 0x6f26c0 of 0x4c-byte records, the "hp_AddProdSetting" 0x62c284 /
"hp_LoadProdSetting" 0x62c298 tags: 0x5a8340 the LCG, 0x5a83b0 / 0x5a84b0 / 0x5a84d0 the weights and the
character blocks, 0x5a8540 / 0x5a8560 / 0x5a86c0 the lookup / create / remove, 0x5a8710 the weight adjust,
0x5a8850 / 0x5a8870 the refresh / pick, 0x5a88e0 / 0x5a8900 the factor, 0x5a8920 / 0x5a8940 / 0x5a8a20 the
free / save / load) and its production messages 0x5a8b80 .. 0x5a8d80. Matched first try: 0x5a82f0, 0x5a8390,
0x5a84b0, 0x5a84d0, 0x5a8540, 0x5a86c0, 0x5a88e0, 0x5a8900, 0x5a8920, 0x5a8c00, 0x5a8c50, 0x5a8cb0, 0x5a8d40;
0x5a8850 / 0x5a8b80 / 0x5a8d80 after the byte fixes (the settings good at +1, the clock compare `0x75 <= x`,
the redundant `kind == 6 || kind != 7`). The literals climb 0x62c270 .. 0x62c298 in function order (one .data
block, D8), so the map window and the production settings are one TU; no boundary inside. Next:
`uv run ledger next` = 0x5a8e10 (201 B, a call from 0x542590). The session continued with
0x5a8e10 .. 0x5a9470 (5 more functions; the unit runs 0x58bbd0 .. 0x5a9690, 229 functions, 21 matched):
the plague / gypsy catastrophe messages (0x5a8e10 the three messages over the 0x300 characters, 0x5a8ee0 /
0x5a8f90 / 0x5a9050 the city / good texts of a character pair via 0x42ec00 (3) + 0x5176d0, 0x5a9110 /
0x5a9170, 0x5a91c0 the "KATASTROPHE_PEST" text 0x62c2c0, 0x5a9240 / 0x5a92b0, 0x5a9330 / 0x5a9430 /
0x5a9470 with the "NACHRICHTEN_ZIGEUNER" keys 0x62c2f4 / 0x62c310). Matched: 0x5a8e10, 0x5a9110, 0x5a9170,
0x5a9330, 0x5a9430. The .data order climbs 0x62c2ac .. 0x62c310, so the catastrophe messages are the same
TU (the map window, the production settings and the catastrophe texts share one block). The session also
drafted 0x5a9690 .. 0x5a9af0 (10 more functions; the unit runs 0x58bbd0 .. 0x5a9b50, 239 functions, 26
matched this session): the graveyard / Friedhof texts over the table 0x5eecd0 with the 0x5190a0 order
(0x5a96e0 / 0x5a97e0 / 0x5a9880) and 0x5a9690 / 0x5a9780 / 0x5a9910 / 0x5a9990 / 0x5a99e0 / 0x5a9a30 /
0x5a9af0 (0x42ec00 (3) picks, the keys 0x62c32c .. 0x62c39c). Matched: 0x5a9690, 0x5a9780, 0x5a9990,
0x5a99e0, 0x5a9af0. Session total 2455 -> 2481 byte matches, all 26 in this unit. Next:
`uv run ledger next` = 0x5a9b50 (108 B, a call from 0x56daa0; the graveyard texts continue).
Regression note: 0x4a13e0 (am.c, recorded behaviour at HEAD) builds to draft since commit 257bad90 changed its
ai_Unk503ee0 call to `ai_Unk503ee0(&x, ...)`; `uv run build` lowers the record, so this pass left the record
at HEAD's value (the am.c match session owns the re-grade / fix).

Session 2026-10-24 (worktree, sixth) drafted 0x5a9b50 .. 0x5ae6b0 into unk_58bbd0.c (24 functions; the unit now
runs 0x58bbd0 .. 0x5ae870, 263 functions, 65 byte; session 2481 -> 2487): the Domizil message family
(0x5a9b50 / 0x5a9bc0 / 0x5a9c30 / 0x5a9ca0 / 0x5a9d00 / 0x5a9d60 / 0x5a9dd0 / 0x5a9e40, the kinds 6 / 7 and the
"NACHRICHTEN_DOMIZIL" keys 0x62c3b8 .. 0x62c400), the table pickers 0x5a9e90 / 0x5a9f10 (the table 0x62c420,
0x42 entries of 0x1c bytes, the count 0x62cb58), the percent boxes 0x5a9f80 / 0x5ad000 ("misc\Messagebox", the
relation 0x509e60), the Domizil window family (0x5aa0d0 the child builder, 0x5aa1f0 the info panel with the
opinion 0x50c520 in percent, 0x5aa4b0 the control destructor, 0x5aa520 the selected entry, 0x5aa700 / 0x5aa9a0
the worker list 0x6f40a0 / 0x6f40a4 / 0x6f40b0 / 0x6f40bc) and the transport block (0x5aab80 the Personnel book,
0x5ac110 the mercenary book and 0x5ad150 the transport window, all three ROUGH stubs whose Ghidra pseudocode lost
the call arguments, then 0x5ae240 "Handel\Handel_Transport_neuer_Kauf", 0x5ae570 the confirmation and 0x5ae6b0 the
AIMSG type 0xf order with the carrier walk 0x53e400 (2, 3, ..., 0x11) and the amount 0x50aa00 / 0x50a970).
Matched first try (6): 0x5a9b50, 0x5a9bc0, 0x5a9d00, 0x5a9d60, 0x5a9dd0 and 0x5aa4b0. No new unit, no boundary
inside (the Domizil keys 0x62c3b8 .. 0x62c400 and the table 0x62c420 .. 0x62cb58 keep the .data order, D8).
Next: `uv run ledger next` = 0x5ae870 (100 B), then 0x5ae8e0, 0x5ae9a0 (1648 B), 0x5af010 (400 B) and
the stubs at 0x5af1a0 / 0x5af1b0 / 0x5af1c0 (16 B each). VC6 lesson: a first full ROUGH draft of 0x5aab80 made
cl.exe spin >10 min (see notes/vc6-matching.md); the stubs must be built in pieces.

Session 2026-10-24 (worktree, seventh) drafted 0x5ae870 .. 0x5b2530 into unk_58bbd0.c (11 functions; the unit now
runs 0x58bbd0 .. 0x5b26b0, 274 functions, 65 byte; session 2487 -> 2491 byte matches): the transport order
0x5ae870 (**matched**: the clock is one `*(HETIME *)(carrier + 0x52) = ai_Time;` struct assignment with a
TU-local pack(2) HETIME), the transport / owner check 0x5ae8e0 (192 B, 182/208: the original caches BOTH
parameters in edi / ebx, ours reuses parameter slots - a prologue allocation difference), the warehouse
transport 0x5ae9a0 (1648 B, ROUGH), the total amount 0x5af010 (400 B, 336/400: the frame slot choice and
`fimul` vs `fild` + `fmul`), the three forwarders 0x5af1a0 / 0x5af1b0 / 0x5af1c0 of 0x5af1d0 (**matched**, 16 B
each: `return ai_Unk5af1d0(param_1, 1 / 2 / 4);`; they are called from the main.c module init 0x53bb20 and
about 20 more sites all over the game), the transport window 0x5af1d0 (11968 B, STUB: one function, one ret at
0x5b2088, frame 0x1848 with __chkstk, the 0x2d-dword table at +0x15c; it calls this unit's transport helpers
and the windows 0x58d720 / 0x58dd00 / 0x58ee30 / 0x58f290), the player's screen image 0x5b2090 (235 B, the
picture loader 0x478b50 / 0x478cc0 / 0x478bd0 and the scale 0x62c270 = 7.5 / 5.33 below the width 0x780), the
map run callback 0x5b2180 (944 B, ROUGH) and the two-choice window 0x5b2530 (384 B). The literals climb
0x62cc6c .. 0x62cd1c in function order (one .data block, D8), so **no boundary** at 0x5af1d0 / 0x5b2090;
`uv run ledger data-order 0x5b2090` reports a +0xa7d jump only because the Domizil table 0x62c420 .. 0x62cb58 and
the transport strings are not located data symbols. The session then walked on to 0x5bc9c0 (99 functions in
all; the unit now runs 0x58bbd0 .. 0x5bcc70, 327 functions, 73 byte, contiguous) and found FOUR more
**Ghidra-missed starts** inside the merged records (ret / tail-call + nop seams, then confirmed by the ledger's
reference scan): 0x5b3630, 0x5b3ad0 (no padding after the previous ret - only the address-taken reference at
0x5b3cf5 gives it away), 0x5b3f30, 0x5b4820 and 0x5b4c60 (both end the previous function with `jmp 0x52d7a0`,
so a ret scan cannot see them), and 0x5b5310 (a plain ret + four nops, but missing from functions.csv). Also:
Ghidra's 0x5b5610 record swallows the functions.csv starts 0x5b5690 and 0x5b5a20 (drafted). Three more
matches: 0x5ba600 (the picture release: 0x478fc0 + `0x6f26ac = 0`, and the return type is void - a `return 0`
adds a `xor eax,eax`), 0x5ba620 / 0x5ba660 (the Domizil table lookup: the INDEX-loop form is the one that
matches, see notes/vc6-matching.md) and 0x5bc0f0 (32 B, the same picture release shape). Near-miss worth a match
session: 0x5bb2d0 (112 B, 95/128 - the merged return-0 block needs the combined-condition form; the kind record
0x7e3f30 + kind * 0x24d is compared against 4 / 0x13 / 0x10 for the order kinds 0x172 / 0x174, 0x158 / 0x160
and 0x16e / 0x176). The rest of the session's functions are ROUGH stubs with the frame size, the first calls
and the seams recorded. Next: `uv run ledger next` = 0x5bcc70 (1000 B), then the rest of the module
(the region runs to 0x5de450; the 0x5b... names are the Rathaus / window builders).

Session 2026-10-24 (worktree, eighth) drafted 0x5bcc70 .. 0x5c1df0 (28 functions; the unit now runs
0x58bbd0 .. 0x5c1e10, 355 functions, 73 byte / 74 behaviour): the player stats panel 0x5bcc70
("Panel_player_stats" 0x62d134, the dialog record 0xfa5aa0 + id * 0x8c and the row list 0x6f40e4 .. 0x6f4cf4),
the control helpers 0x5bd060 .. 0x5bd1f0 and the special-amount window family ("special_amt1" 0x62d224 /
"special_amt2" 0x62d204 / "special_amt3" 0x62d214 / "special_amtinfo" 0x62d244): the record block 0x5bd250,
the row builders 0x5bd5e0 / 0x5bdb60 / 0x5be3b0 / 0x5be720, the record status 0x5be1d0, the page fills
0x5bf0d0 / 0x5bf300 / 0x5bf4f0 / 0x5bf6f0, the page switch 0x5bfed0, the tab table 0x62d14c with 0x5c08b0 /
0x5c08d0 / 0x5c0970, the menus 0x5c1130 / 0x5c1b20 / 0x5c1b40 / 0x5c1df0 and the info box 0x5c1a80. No
function matched (session 2495 -> 2495); the literals climb 0x62d134 .. 0x62d244 in function order (one
.data block, D8), so no boundary. Three Ghidra-missed starts were found by the ledger and drafted from the
disassembly: 0x5bea00 (79 B, a config wrapper of 0x5c0970), 0x5bf4f0 (501 B, a sort / fill variant) and
0x5bf6f0 (195 B, the up-to-4 carrier records). The ledger then showed the function-pointer-only starts the
Ghidra function list lacks; they are drafted as ROUGH stubs from the disassembly: the page fills 0x5bf7c0 /
0x5bfa90 / 0x5bfc40 / 0x5bfd90, the tab fills 0x5bffb0 / 0x5c00b0 / 0x5c01c0 / 0x5c02d0 / 0x5c03e0 /
0x5c0550 / 0x5c0650 / 0x5c0790, the release 0x5bff80 and the menu callback 0x5c1500, so the unit is
contiguous again, and 0x5c1e10 (846 B, the 0x5c1df0 callback, frame 0xac, 0x4e6fc0 (param_1, 6, the 0x8c
record buffer)) is drafted as a ROUGH stub, so the unit runs 0x58bbd0 .. 0x5c2160 (370 functions). Next:
`uv run ledger next` = 0x5c2160 (657 B, a call from 0x5a07d0, dword in code at 0x5c23bb), then the
rest of the module to 0x5de450.
Session 2026-10-24 (worktree, eighth, continued) drafted 0x5c2160 .. 0x5c26a0 (4 functions; the unit now runs
0x58bbd0 .. 0x5c2730, 374 functions): the law book listener 0x5c2160 (the pages 0x111 Strafrecht / 0x112
Steuern / 0x113 Verfassung of the window 0x52f360 with the two pages +0x228 / +0x22c, the entries in 0x4f2fe0
+ 0x515960 (0x62d284) and the info 0x5c2410), the info box 0x5c2410 ("Gesetze\Gesetzbuch_Info" 0x62d2a4)
and the master letter window 0x5c26a0 ("berufe\meisterbrief_1" 0x62d2bc) **both matched first try**, and the
law entry controls 0x5c2490 (the checkboxes 0x1951 .. 0x195b, the counter 0x1953, the slider 0x46a960
(0x1002)). Session 2495 -> 2497 byte matches; the literals climb 0x62d284 .. 0x62d2bc in function order (one
.data block, D8), no boundary. Next: `uv run ledger next` = 0x5c2730.

Session 2026-10-24 (worktree, ninth) drafted 0x5c2730 .. 0x5c7560 (22 functions into unk_58bbd0.c; the unit now
runs 0x58bbd0 .. 0x5c76b0, 396 functions): the **master letter** module (0x5c2730 the second master letter window,
**matched first try**: the 0x5c26a0 clone with the single caption 0x195e; 0x5c27b0 the master letter callback:
the messages 0x10 / 0x20 build one / two controls, a click walks the carriers 0x53e400 (2, 0, 7, 3) and sends
the AIMSG type 7 with the text 0x1939 (+0x130) and the kind record byte +0x223; 0x5c2a10 the seven profession
rows (param_2 + i * 8, the control record 0xf8f080 + id * 0x2ec, +0x2e8 = 0x91aaf0 + id * 0x41, the slider
0x46a960); 0x5c2b90 the window "berufe\meisterbrief_2" 0x62d2d4, ROUGH, and **0x5c3200 the master letter
contact dispatcher** (the modes 0x10 / 0x20 -> 0x5c27b0, 0x30 -> 0x5c2b90, 0x40 -> 0x5c2730, 0x50 -> 0x5c26a0;
it is called from about 20 sites of the building / contact code). Then the **law / office** block: 0x5c33c0 the
eligibility predicate (the param_3 mask bits against the character record, the 0x4fd7e0 / 0x47dd70 / 0x505df0
calls and the relation range 0x50c550), 0x5c37a0 / 0x5c39a0 the two filter tests (the kind bitmask of the
building kind record, the target cases 0 .. 7, the four +0x1c ids, the own character 0x6f1bf8), 0x5c3bb0 the
selection window "Gesetze\Auswahl2" 0x62d2ec (ROUGH), 0x5c4100 the selection entry (0x50b090 / 0x5c3bb0), then
the big player windows: 0x5c4150 "Special_Stammbaum" 0x62d300 (the family tree, ROUGH SKELETON),
0x5c5310 "panel_info_spieler" 0x62d3e0 (the player info panel, ROUGH SKELETON) and 0x5c6050 "misc\geschenck"
0x62d3fc (the gift window), then the advertising / office family: 0x5c64c0 (the type 9 candidate list 0x4a5ef0),
0x5c6530 (the marriage test, the error codes 0xfffffc00 .. 0xfffffbfa and the age difference 0x505e60 < 6),
0x5c6620 (the marriage rating), 0x5c6910 "privillegien_werbung2" (the proposal confirm, AIMSG 0x41), 0x5c6aa0
"privillegien_werbung1" (the advertising list, the rows 0x5be1d0 / 0x5be3b0 / 0x5bd0c0), 0x5c6d00 (the office
application: the 0x53e400 (1, 0, 0x41) job walk, the ranks and 0x4fe140, ROUGH) and the three bar charts
0x5c72d0 / 0x5c7460 / 0x5c7560 over the table 0x91ab0c .. 0x91ac4c (ROUGH). Session 2497 -> 2498 byte matches
(0x5c2730). The literals climb 0x62d2bc "berufe\meisterbrief_1" .. 0x62d424 "privillegien_werbung1" in function
order (one .data block, D8), so **no boundary** inside 0x5c2730 .. 0x5c76b0. Next: `uv run ledger next`
= 0x5c76b0 (325 B).

Session 2026-10-24 (worktree, tenth) drafted 0x5c76b0 .. 0x5ca150 into unk_58bbd0.c (15 functions; the unit
now runs 0x58bbd0 .. 0x5ca560, 411 functions, 76 byte): the statistics panel (the five bar charts 0x5c72d0 /
0x5c7460 / 0x5c7560 / 0x5c76b0 / 0x5c7800, the chart window 0x5c7970, "panel_statistik_stadt" 0x5c7f00,
"panel_pcompare4plus6 / panel_pcompare8" 0x5c82f0, "panel_privilegien" 0x5c8640) and the privilege handlers
("privillegien_medicus" 0x5c8bc0, "privillegien_scheidung" 0x5c8ef0, "privillegien_erpressung2" 0x5c9290,
"privillegien_erpressung" 0x5c9550, the payment windows 0x5c9990 / 0x5c9c50 / 0x5c9df0 - the last three are
Ghidra-missed starts from uv run disasm) and the blackmail contact variant 0x5c9ff0 plus the law /
privilege window 0x5ca150 ("privillegien_gesetz" 0x62d518, ROUGH). No function matched (session 2498 -> 2498). The literals climb
"panel_statistik_stadt" 0x62d440 .. 0x62d4f8 in function order (one .data block, D8), no boundary inside.
Typed facts: the five charts take TWELVE parameters (ten caller values + the screen height - 1 and width);
0x4764f0 is a seven argument fill (x1, y1, x2, y2, r, g, b) - the three older chart drafts with six
parameters / two-argument calls were wrong and are converted. Next: `uv run ledger next` = 0x5ca560
(764 B, a call from 0x4f5c30), then the rest of the law / privilege module to 0x5de450.
Session 2026-10-24 (worktree, twelfth): drafted 0x5ce610 .. 0x5d0e60 (25 functions; the unit now runs
0x58bbd0 .. 0x5d0f20, 457 functions, 78 byte): the interrogation / trial-step message module (the windows
0x5ce610 .. 0x5cf820, the object callback 0x5cf8a0, the trial step 0x5d08c0 and its wrappers, the
amount transfers 0x5d0b20 / 0x5d0c70 and the shuffled pick 0x5d0e60). Matched 0x5cfb60 and 0x5d06e0; the
thirteen Ghidra-missed starts were found by the ledger and drafted. 0x5d08c0 reads the earlier he.c pooled
strings (backward), no new .data run: the unit continues. Next: `uv run ledger next` = 0x5d0f20
(984 B, a data pointer), then the rest of the last stretch to the end of the game code 0x5de450.
Session 2026-10-24 (worktree, thirteenth) drafted 0x5d0f20 .. 0x5d3270 (23 functions into unk_58bbd0.c;
the unit now runs 0x58bbd0 .. 0x5d33b0, 480 functions, 2501 byte): the **trial / action callbacks of the
record table 0x62c420** (0x1c-byte records, the function pointer at +0x14; the records 5 / 17 / 19 / 21 /
22 / 32 / 34 / 36 .. 44 / 49 / 50 / 53 .. 55 / 57 / 59 .. 61 point at 0x5d1830 / 0x5d0f20 / 0x5d1300 /
0x5d1680 / 0x5d13e0 / 0x5d2580 / 0x5d2720 / 0x5d2960 ..). They are reached only through that table; the
table also points at the previous session's 0x5d0e60 / 0x5cfb60 .. 0x5d0530 (a cross-TU registration
table, no boundary evidence either way). No literal belongs to them, so the .data order is unchanged.
0x5d0f20's random-pick loops are drafted (0x42ec00 (0x300) over the 0x300 records 0x770b80 + i * 0x21c,
the skip of the actor / param_2 / first pick ids, the free word -1, the kinds +2 >= 0xa, the byte
0x770ce6); the other 22 are ROUGH stubs with the record index, frame and call sequence. The new unit
**unk_5d33b0.c** was opened at 0x5d33b0 (the screen front-end module) and its first function
0x5d33b0 (the screen file list: vfs_FindDir, the extension pick, the "%s_%s" 0x62d7d8) is drafted
61/256; see notes/units/unk_5d33b0.md. The session went on into the unit: 0x5d34b0 (the
"menu\fileselector" window, drafted 630/800; it calls 0x5d33b0 and formats "%s\\%s%s" 0x62d7e0),
0x5d3810 (the misc\Messagebox family, drafted 403/512), 0x5d3a00 (the AUTOSAVE / QUICKSAVE slot writer,
drafted 132/192), 0x5d3ab0 / 0x5d4080 / 0x5d42b0 / 0x5d45d0 / 0x5d4920 / 0x5d4c70 (the save / load slot
windows, the options INI writer; ROUGH stubs with the flow, the INI keys and the call sequences in their
comments) and 0x5d44c0 (the edit field dialog, drafted 166/272). Then the options module: 0x5d5640
(the options reader GetPrivateProfileIntA, the twin of 0x5d4c70; stub), 0x5d5c40 (the shadow detail
setup; stub), 0x5d5e90 (**drafted 95/128**: the game speed factor 0x6f3608 * 0.00625 + 0.25 in double, the
network branch 0x6f23f8 & 4, the float 0x5f1290, 0x410610 (0x6f3610 - 0x40), 0x623814 = 100 - (0x6f360c
& 0xff)), 0x5d5f10 (the volume setters 0x427370 / 0x4239a0 / 0x423a50, the floats 0x602b08 / 0x602d68;
stub) and the three option screens "menu\options_gfx" 0x62daf8 (0x5d5fd0), "menu\options_sfx" 0x62db1c
(0x5d6580) and "menu\options_game" 0x62db30 (0x5d6950), all stubs with their call sets. The unit runs
0x5d33b0 .. 0x5d6f40 now. Next: `uv run ledger next` = 0x5d6f40 (84 B, a list control builder:
0x46bf10 (0x20, param_1 * 0x2e + 10, 0xbf, param_2, param_3), +0x5c = 1, 0x46c260 (id, 300)), then the
screen / options / start stretch to 0x5de450.

Session 2026-10-24 (worktree, fourteenth) drafted the last game-code stretch 0x5d6f40 .. 0x5dd100 into
unk_5d33b0.c (45 functions; the unit now runs 0x5d33b0 .. 0x5dd3e0, 63 functions, contiguous): the list
control builder 0x5d6f40, the black fades 0x5d6fa0 / 0x5d7080, the screenshot 0x5d7160, the loading screen
0x5d7320, the quicksave 0x5d73e0 / 0x5d7430, the network start 0x5d7530, the **options menu** 0x5d7770
(the seven buttons 0x5d6f40 with the texts 0x1441330 .. 0x1441348 and the radio group 0x46c490), the
progress bar 0x5d7ba0, the pause 0x5d7e90, the message filter 0x5d7f10, the Ghidra-missed tailor chat
**0x5d8080** (the "locations\schneider\plausch" callback of 0x5d8360), the gypsy / tailor window family
0x5d8360 .. 0x5da030 ("Locations\zigeunerlutrache", curse, homage, theatre, poker), the poker windows
0x5da030 .. 0x5db110 and the effect / explosion math 0x5dbc10 .. 0x5dd100 (the sphere volume 0x5dc1a0,
the particle step 0x5dc3a0, the curve samples 0x5dcf00, the trajectory 0x5dd100). Matched (3): **0x5d6f40**
(the d2_Objs pointer form), **0x5da010** (void) and **0x5d7e90** (0x6f25ac is a byte); near-misses:
0x5d7080 (27/224), 0x5d6fa0 (38/224), 0x5d73e0 (51/80), 0x5d7320 (59/192), 0x5d9b20 (37/80). **The end of the game code**: the last function is 0x5dd100
(ret 0x5dd3d6, nop fill 0x5dd3e0); 0x5dd3e0 .. 0x5de450 (4208 B) is a DATA table (256 records
{0x5eee70, index, 0x8000ii0c, 0} plus a second table {0x5eee90 / 0x5eeea0, index, 0xffff03, 0}), not
code (no prologue, no ret). The ledger cannot claim it (R5 `@bytes` requires the range inside the unit;
R7 ends the last unit at the first undrafted byte), so `uv run ledger next` still prints this tail; see Open
boundaries.

## Open boundaries
- **Undrafted bytes**: computed, never listed here. `uv run ledger next` = the first undrafted address with
  its probable functions and evidence, `undrafted` = every region, `uv run coverage gaps LO..HI` walks any
  range. Bytes that are no function's code and not fill stay undrafted until the draft that owns them matches or the
  unit header declares them (`@bytes 0xSTART..0xEND why`).
- **unk_58bbd0.c extent** (layout 2026-10-24, fourth): the unit covers the production / trade windows AND the
  building purchase / construction screens, the Wirtshaus / locations module, the money lender, the Rathaus /
  guild rooms, the church rooms, the thieves' guild / dungeon module, the thieves' guild **kidnapping** and the
  **city watch** ("LOCATIONS\STADTWACHE\STANDARD"); its literals climb 0x62b038 .. 0x62c270 in function
  order (one .data block, data order D8), so no boundary was found inside 0x58bbd0 .. 0x5a6c40. The unit is
  contiguous 0x58bbd0 .. 0x5a9b50 (no undrafted bytes inside; the Ghidra-missed starts 0x59dc00, 0x5a0ca0,
  0x5a13f0, 0x5a1530, 0x5a1670, 0x5a5120, 0x5a5d70, 0x5a5fd0, 0x5a6270, 0x5a64d0 are drafted, and 0x5a28f0 /
  0x5a2b60 / 0x5a2de0 / 0x5a2e60 / 0x5a36c0 / 0x5a3810 / 0x5a3a40 / 0x5a3b90 / 0x5a3f10 / 0x5a4130 / 0x5a41d0 /
  0x5a48d0 / 0x5a4b90 turned out to be real Ghidra starts, not misses). Session 2026-10-24 (fifth): the
  map window (0x5a6cb0 .. 0x5a82f0) and the production settings module (0x5a8340 .. 0x5a8cb0) follow; the
  literals climb 0x62c270 (5.33f) / "Misc\Landkarte2" 0x62c274 / "hp_AddProdSetting" 0x62c284 /
  "hp_LoadProdSetting" 0x62c298 in function order, one .data block (D8), no boundary. The next frontier is
  0x5a9b50 (108 B, a call from 0x56daa0); the message stretch extends the .data block to 0x62c39c
  (still one block, D8); a leaked name for the unit (`unk_58bbd0.c`) is still the only
  open naming question. Session 2026-10-24 (sixth): the Domizil family and the transport block extend the unit to
  0x5ae870 (contiguous, no undrafted bytes); the .data block climbs through the Domizil keys 0x62c3b8 .. 0x62c400
  and the table 0x62c420 .. 0x62cb58 (one block, D8), so no boundary; the front 0x5aab80 / 0x5ac110 / 0x5ad150
  are ROUGH stubs (the Ghidra pseudocode lost the call arguments). Session 2026-10-24 (seventh): the transport
  block extends the unit to 0x5b26b0 (contiguous): the literals climb "Handel\Handel_Transport_neuer_Kauf"
  0x62cc6c (0x5ae240), "ob_TransportFromLager..." 0x62cc94 (0x5ae9a0), 0x62cce8 (0x5b2090), 0x62ccfc (0x5b2530),
  0x62cd1c (0x5b26b0) in function order, one .data block (D8), no boundary; 0x5af1d0 is a 11968 B single
  function (one ret at 0x5b2088), not a Ghidra merge. The session went on to 0x5bc9c0: the unit is contiguous
  0x58bbd0 .. 0x5bcc70 (327 functions); the new literals of the stretch ("0x62ccfc" 0x5b2530, "0x62cd1c"
  0x5b26b0, "0x62cd7c" 0x5b4340, "0x62cda8" 0x5b4590, "0x62cdc8" 0x5b5690, "0x62cfdc" 0x5b9f30, "0x62d0b0"
  0x5bc110) keep climbing in function order (one .data block, D8), so still no boundary. Ghidra merged six more
  functions into its records (0x5b3630 / 0x5b3ad0 / 0x5b3f30 / 0x5b4820 / 0x5b4c60 / 0x5b5310, all drafted) and
  swallowed the two functions.csv starts 0x5b5690 / 0x5b5a20 (drafted). Next frontier: 0x5bcc70.
  Session 2026-10-24 (eighth): the stretch 0x5bcc70 .. 0x5c1df0 is the player stats panel and the
  special-amount window family ("special_amt1 / 2 / 3 / info"); its literals climb 0x62d134 .. 0x62d244 in
  function order, one .data block (D8), so the unit continues; the new Ghidra-missed starts 0x5bea00 /
  0x5bf4f0 / 0x5bf6f0 are drafted, and the function-pointer-only starts 0x5bf7c0 / 0x5bfa90 / 0x5bfc40 /
  0x5bfd90 / 0x5bffb0 / 0x5c00b0 / 0x5c01c0 / 0x5c02d0 / 0x5c03e0 / 0x5c0550 / 0x5c0650 / 0x5c0790 /
  0x5bff80 / 0x5c1500 are ROUGH stubs (drafted from uv run disasm), so the unit is contiguous to 0x5c1e10.
  Next frontier: 0x5c2160 (657 B, a call from 0x5a07d0), then the rest of the module to 0x5de450.
  Session 2026-10-24 (ninth): the stretch 0x5c2730 .. 0x5c7560 is the master letter module (the windows
  "berufe\meisterbrief_1 / _2", the contact dispatcher 0x5c3200), the law / office selection (0x5c33c0 ..
  0x5c4100) and the player info / advertising family ("Special_Stammbaum", "panel_info_spieler",
  "misc\geschenck", "privillegien_werbung1 / 2"); its literals climb 0x62d2bc .. 0x62d424 in function order,
  one .data block (D8), so the unit continues; the unit is contiguous to 0x5c76b0 (396 functions, 2498
  byte). Next frontier then: 0x5c76b0 (325 B).
  Session 2026-10-24 (tenth): the stretch 0x5c76b0 .. 0x5c9df0 is the statistics panel (the five bar charts
  0x5c72d0 / 0x5c7460 / 0x5c7560 / 0x5c76b0 / 0x5c7800, the chart window 0x5c7970, "panel_statistik_stadt",
  "panel_pcompare4plus6 / _8", "panel_privilegien") and the privilege handlers ("privillegien_medicus",
  "_scheidung", "_erpressung2", "_erpressung" and the three payment windows 0x5c9990 / 0x5c9c50 / 0x5c9df0,
  Ghidra-missed starts); its literals climb 0x62d43c .. 0x62d4f8 in function order, one .data block (D8),
  so the unit continues; contiguous 0x58bbd0 .. 0x5ca560 (411 functions, 76 byte). Next frontier: 0x5ca560
  (764 B, a call from 0x4f5c30).
  Session 2026-10-24 (eleventh): the stretch 0x5ca560 .. 0x5cb250 is the privilege / counter-espionage
  family ("absetzen" 0x62d52c, "privillegien\unterschlagung" 0x62d538, "privillegien\becircen" 0x62d554,
  "privillegien\gegenspionage" 0x62d56c, "privillegien\gegenspionage2" 0x62d588); its literals climb in
  function order, one .data block (D8), so the unit continues; contiguous 0x58bbd0 .. 0x5cb4f0 (418
  functions, 2498 byte). The Ghidra-missed start 0x5caab0 (the becircen window, reached only through the
  menu callback of 0x5cacd0) is drafted; 0x5cb1e0 is behaviour (49/112). Then the espionage family
  0x5cb4f0 / 0x5cba90 / 0x5cbe00 and the order handlers 0x5cc2b0 / 0x5cc700 / 0x5cca40 (the last
  complete; the others ROUGH SKELETONs from uv run disasm) and the spy family 0x5ccc10 .. 0x5cde90
  extend the unit to 0x5ce610. Next frontier: 0x5ce610 (469 B, a call from 0x4f7a60), then the rest of
  the espionage / privilege module to 0x5de450.
  Session 2026-10-24 (twelfth): the stretch 0x5ce610 .. 0x5d0e60 is the interrogation / trial-step message
  module (the "privillegien_verhoer" / _arbeiter_ausweisen / CHOOSEPROFESSION2 / "special_uni" windows
  0x5ce610 .. 0x5cf820, the object callback 0x5cf8a0, then the trial step 0x5d08c0 with its wrappers
  0x5d09d0 / 0x5d0a40 / 0x5d0ab0 and the two-record amount transfers 0x5d0b20 / 0x5d0c70); its strings
  are the unit's (0x62d5c4 .. 0x62d6bc) and 0x5d08c0 reads the earlier he.c pooled strings, so the .data
  order shows no new run: the unit continues, contiguous 0x58bbd0 .. 0x5d0f20 (457 functions, 78 byte).
  Next frontier: 0x5d0f20 (984 B, a data pointer).

- **main.c / he.c** at 0x53d430 (layout 2026-10-23, session 6): main.c ends with the program entry 0x53ccf0 (WinMain, ends 0x53d426); he.c starts at 0x53d430 with the two debug handlers "he_AllocNone" / "he_RunNone". Evidence: .data order (main.c: INI strings up to "game.INI" 0x6256fc; he.c: globals 0x625708 .. 0x62571c, then the strings from 0x626680) and the tag. Lead: a leaked name only; the end of he.c is open (see Frontier).
- **he.c Ghidra misses**: none open — all found starts are drafted (sessions 7 + 8, see the Frontier; 0x564830 is a free-tail label, deliberately without source).
- **he.c / unk_575860.c** at 0x575860 (layout 2026-10-23, session 16): he.c ends with 0x575710 (its jump table
  0x575830 .. 0x575853, nop fill); the new unit starts at 0x575860. Evidence: code seam + literals (the code up to
  0x577740 uses no he literal; he's last literal "Game" 0x62916c / 0x629188, the next .data block starts at
  0x62918c with the "FT" state name table, whose users start at 0x577740). Lead: leaked name only. Session 17
  settled the extent: the unit is one tutorial module (0x578180, called from main.c 0x53bb20, initializes the
  state record 0x5eec60 that the tick 0x575860 reads; the "tutorial\*" forms and TUTORIAL_* texts run through
  0x577cd0 .. 0x578570), so 0x575860 .. 0x577110 needs no split; only the boundary at 0x575860 itself still rests
  on a leaked name. Session 18: the unit extends to 0x583700 (the "Locations_*" contact windows; their literals
  climb 0x62a2a4 .. 0x62a580 in function order, one .data block, no new boundary). Session 19: the unit extends
  to 0x586ab0 (the graveyard contact / seance family; its literals climb 0x62a5bc .. 0x62a718 in function
  order, `uv run ledger data-order 0x584410`, one .data block, no new boundary). Session 20: the unit
  extends to 0x589aa0 (the HEIMSUCHUNG / ALTERN_LASSEN contacts, the graveyard detail windows and the item
  drag-and-drop family; no new literals before 0x62a8d8, the block keeps climbing, one .data block, no new
  boundary). Lead: the next function 0x589aa0 (142 B, a call from 0x5190b0) continues the drag-and-drop family;
  the boundary at 0x575860 itself still rests on a leaked name only. Session 21: the unit's last function is
  0x589e50 (the building slots, ends 0x589f07, nop fill to 0x589f10); the extent is 0x575860 .. 0x589f10.
- **unk_575860.c / unk_589f10.c** at 0x589f10 (layout 2026-10-23, session 21): new unit, the network lobby.
  Evidence (proven, D8 + code seam): the previous unit's last literal is "gamedata/screenshots/gilde%04i.bmp"
  0x62aa58 (0x589bc0); the four dwords 0x62aa7c .. 0x62aa88 (-1, 0, -1, -1) open a new .data block and its
  first literal "menu\netlobby" 0x62aa8c is used by 0x589f10; nop fill 0x589f08 .. 0x589f0f; 0x589f10 is
  called from main.c 0x53bb20. Extent of the lobby: its literals climb 0x62aa8c .. 0x62aad0 through 0x58ba70;
  the next run starts with "Handel\Handel_PRODUKTION" 0x62b038 (0x58bd60, +0x568).
- **unk_589f10.c / unk_58bbd0.c** at 0x58bbd0 (layout 2026-10-23, session 21): plausible only - 0x58bbd0 has no
  literals; it initializes the tables 0x6f4f08 .. 0x6f5a64 that 0x58bd60 / 0x58c680 read and is called from
  0x58bd60 and from the "HANDEL\HANDEL_SUCHEN" helper 0x584d70 (the lobby does not touch them). Lead: a leaked
  name, or a `uv run try` codegen test once 0x58bd60's callers are matched.
- **sv.c / main.c** at 0x531670 (layout 2026-10-23, session 5): sv.c ends with the frame loop 0x5309a0 and 0x5315c0 (the frame loop 0x5309a0 and its callback 0x5315c0); main.c starts at 0x531670 (the process level code: "Die Gilde" window class, tag "main_"). Evidence: .data order only (sv_ block ends with "MASTER(): .." 0x624534; "Die Gilde" 0x62493c follows after the sv_Unk / INI strings 0x6248a4 .. 0x6248b8 which are used by 0x53c.. and 0x52a680). Note: the strings 0x6248a4 .. 0x6248b0 (the screen rectangle ints) are used by the sv code 0x52a680 AND the main code 0x53c146: one shared global block, not a boundary proof. Lead: a leaked name only; the unit sv.c could also continue over 0x531670 .. if "main_" is one function group of sv (the tag main_ names an obj of its own, so the split is plausible).
- **Missing starts** inside the drafted units (Ghidra missed them, callbacks reached only through function pointers): 0x52d010 (drafted, sv.c), 0x5315c0 (drafted, sv.c), 0x528180 (drafted, sv.c); check with uv run disasm every `LAB_` in new pseudocode.
- **gv.c / ord.c** at 0x4f4e20 (session 17): the order handler table 0x611768 (0x4f4e20 .. 0x4f8140) follows gv's strings (last gv string 0x611698 "gs_AddBeweis", table at 0x611768); codegen: 0x4f5d50 / 0x4f5f00 match only with 0x4f2870 NOT defined above them in the file, so the handlers are another TU. Moved to ord.c (functions below 0x4f4e20 that sat after them in the file went back to gv.c). Lead: a leaked name only; gv.c's 0x4f4460 / 0x4f48f0 (before the table) may belong to ord.c (0x4f4460 sends messages like the handlers). Blocked (layout 2026-10-23 session 9): 0x4f48f0 matches, 0x4f4460 is nonmatching; only cm.c references them (different views) and no gv.c / ord.c caller sits below them, so a structural move test is neutral — nothing in the binary decides.
- **ord.c / f3gm.c** at 0x4f9e30 (layout 2026-10-23): closed as f3gm.c (tag "f3_gm:"): the string block 0x61774c .. 0x61ade0 runs monotonic over 0x4f9e30 .. 0x50d0b0 (see notes/units/f3gm.md, Unit); the big model-name table 0x6118ac .. 0x61774c is still owned by ord.c (users 0x4f8890 / 0x4f8ae0) and by f3gm.c (0x4fb390 / 0x4fb330: the head tables 0x611938 / 0x612288 / 0x612bd8 / 0x614518 / 0x617398 / 0x6118a8 / 0x6118f0 are in that table: its owner is the pair, probably f3gm.c: .data order only). Lead: a leaked name only.
- **f3gm.c / f3init.c** at 0x50d100 (layout 2026-10-23, session 2): closed as two units: f3gm's last string is "_NACHRICHTEN_HS_67" 0x61ade0 (0x50cad0), f3init's .data starts at 0x61adf8 (its globals and the 19-record table 0x61ae08 .. 0x622e94, used only by f3init functions) and its strings from "KONTOR" 0x622e94; tags "f3_gm:" / "f3_init:". A first attempt joined them (one block with the table between) and was split again: the table is used only by f3init functions (0x512f30 / 0x513590 / 0x5136b0 / 0x5124f0), which a single-unit reading cannot explain better than the two tags. Lead: a leaked name only.
- **f3init.c / io_text.c / ls.c / sv** (layout 2026-10-23, session 3): closed. io_text.c starts at 0x5142e0 (the .data block of "_STADTAUSWAHL_%s_INFO+0" 0x623564, right after f3init.c's city INI strings; the tag "io_Text():" 0x6235d8), ends before 0x519010 (the language getter owns 0x62364c, the first global of ls.c); ls.c ends at 0x520ec0 (0x520b80 ends at 0x520eb6; layout 2026-10-23 session 4 moved the tick callback 0x520ec0 to sv.c), the "sv" unit starts there (frontier). Lead for all: a leaked name only. The "%s %s" (0x60c7b8) / "%s" (0x5f8318) literals the text builders use are pooled from earlier objs (the exe merges identical strings across objs: a function can use strings that sit in an earlier unit's block, so a function's strings prove a boundary only by their FIRST occurrences).
- **ahm.c / gv.c** at 0x4ed170 (session 16): ahm ends with the save / load of its record (0x4ecf10 / 0x4ed040); gv starts with the avatar picker 0x4ed170. Evidence: .data order (ahm's last string "ahm_AllocAhmInfo" 0x60f2f8, then the avatar table 0x60f310 whose first user is 0x4ed170, then gv's strings 0x611190..) and bss (0x6f0e38 is gv's first). Lead: a leaked name only. gv.c may be several modules (avatars, effect slots / explosions, graveyard) in one .data block: scanned the string block 0x611190..0x611700 (layout 2026-10-23 session 9, python PE section parser on orig/GildeGold.exe): only the tags "gv_Alloc:_all_zombies" / "gv_Alloc:_all_graves" / "gv_AllocGraveyardField", no second module tag: keep one unit.
- **Files owned by the match worker this session** (no edits made by this session): no join, move or rename was needed in src/game/d3_io_b.c, bio.c, d3_int_io.c, d3_quad.c, unk_47d600.c, d3_par.c, d3_light.c, sfx.c, mem.c, d3_sky.c, d3_mir.c, net.c, msx.c, d3_snd.c, ts_texture.c, sblib.c, anm.c, d3_d3d.c, bmp.c, unk_4591a0.c.
- **amt.c / ahm.c** at 0x4eaa50 (session 15): amt ends with 0x4eaa00 (announce free office), ahm starts with the building-kind-0x1e accessors; evidence is subject and the .data order
  (amt's last string 0x60f1a8 "%s$A", then the 8-int table 0x60f1b0 used first by ahm's 0x4ead20). Lead: a leaked name only. A forwarding function (`jmp`, 0x4eabd0) is plain
  `return f(a, b);`.
- **d2.c / gfx.c** boundary in (0x476b70, 0x476b90] (only 0x476b70 undecided; later in this pass: 0x4768a0 uses d2's globals 0xfb9064..70, which gfx.c never touches, 0x476b10 / 0x476b40 d2's lists, so d2.c runs to 0x476b70 at least). Narrowed (layout 2026-10-22, blocked-TU
  pass): 0x476b90 / 0x476c00 / 0x476c70 / 0x476c90 are one family on one 6-entry table (0xf8ab80, 12 B: add / set / clear /
  count; the same 4 globals, nothing else), and 0x476c90 is proven gfx.c by codegen, so 0x476b90 moved into gfx.c (byte
  count unchanged, link ok). The remaining seam: 0x476860 / 0x4768a0 / 0x476b10 / 0x476b40 use the d2 object / window lists
  (0xf8f0a0, 0xf8f120) = d2.c side; 0x476b70 (clears input state: 0xf8a668, di_ClearButtons, kb_Last; only rt.c calls it)
  has no tie either way. Blocked: no strings in the seam; .bss is COMMON; the `uv run try` move cannot tell (session 9: no
  codegen coupling can show). Lead: a leaked name only.
- **unk_4591a0.c**: unnamed; neutral to a join with unk_458810.c and to moving d3_GetBoxVerts into d3s.c (layout
  2026-10-22). Blocked: no strings, no codegen difference; q2 suggests d3_mesh.c. Blocked-TU pass 2026-10-22: the
  unk_458810.c / unk_4591a0.c seam is 0x458e70, not 0x4591a0: d3_Unk458e70 (env-map UVs) is called only from 0x459300 (4
  call sites; a static helper right before its first user) and moved to unk_4591a0.c (typed with unk_458810.h's pose views,
  D3_458E70 cast macro for the mesh-side callers; bytes unchanged, 9 / 816 still). 0x458810..0x458da0 are generic pose
  math called from all over the game (chc / ch / d3 ..): a unit of its own, name unknown. The rest of the seam stays a name
  question only.
- **d3_mir.c start**: d3_Unk454780 moved in (codegen) and now matches (session 11); d3_Unk454890 still prefers not to see its body
  (235 B differ with 454780 above it, 92 with it below it; its draft is far off, so no boundary evidence): recheck when 454890 matches.
- **unk_4a7140.c** (0x4a7140..0x4ac140): one .data block, two subsystems; codegen split at 0x4aa580 neutral. Name:
  guess only.
- **hmt.c start**: 0x4ac140..0x4ac2f0 (table helpers, no .data) put into hmt.c; could be unk_4a7140.c's tail. Blocked-TU pass 2026-10-22: .rdata
  evidence for hmt.c: the const table they serve (0x33 records of 0x30 B at 0x5ed1e8..0x5edb78; 0x4ac140 returns the count 0x33, 0x4ac1e0 finds a record)
  lies after unk_4a7140.c's last float constant (-50.0 at 0x5ed1e4, used by 0x4ab6b0; the later functions 0x4ab7f0.. only reuse older pool entries) and
  before hmt's first float 0x5edb78 (0x4ad190): per-obj .rdata is emitted in source order, so the table was defined above hmt's first function, the
  natural place being the top of hmt.c with its access functions. Closed as hmt.c (plausible); a leaked name would still settle it.
- **unk_4b39f0.c / unk_4b6480.c** (missions / messages): both drafted and `separated (plausible)`; no module tag in
  either, names are guesses. The split at 0x4b6480 rests on subject only (save / load end the missions; the message
  table is the next .data). Blocked (layout 2026-10-22 session 8): a `uv run try` move cannot tell (no function on one
  side calls one on the other, so codegen coupling cannot show); .bss: missions 0x6f0728 / 0x6f072c, then
  0x6f0730 (0x4bb720's seed), no gap. Lead: a leaked name only.
- **unk_4b6480.c / anm.c** at 0x4bb810: anm's first function uses "anm:animals"; 0x4bb720 / 0x4bb7d0 (random
  messages, LCG seed 0x6f0730) went to unk_4b6480.c by subject, but their seed sits right before anm's .bss
  0x6f0734 / 0x6f0738. Blocked (session 8): `uv run try` cannot tell (0x4bb720 / 0x4bb7d0 call nothing of anm.c and anm.c
  calls neither), the .bss runs 0x6f0730, 0x6f0734.. without a gap (either side). Lead: a leaked name only.
  Blocked-TU pass 2026-10-22: the .bss commons are 0x6f0724 (0x4b1d20..0x4b3960, before unk_4b39f0.c), 0x6f0728 / 0x6f072c (missions), 0x6f0730 (the seed), 0x6f0734.. (anm); unk_4b6480.c has no
  other .bss user, so the seed is either its only common or anm.c's first; the neighbour 0x4bb6c0 uses the message table 0x602570 and the seed functions follow it (random message pick), which
  is the reason to keep them in unk_4b6480.c. Nothing in the binary decides it: closed as plausible, a leaked name would settle it.
- **anm.c / rd.c** at 0x4bc2e0 (session 8): 0x4bc2e0..0x4bc520 serve anm.c (free places, dummy lookup) but share the
  literal "dummy_TUER" with rd.c's road code 0x4bcc20 (one obj); the split rests on the .bss gap 0x6f0744 (unused
  slot after anm's 0x6f0740, rd.c's first global 0x6f0748 8-aligned). anm.c and rd.c could be one unit. Blocked
  (layout 2026-10-23 session 9): the join-test condition is met (anm_UpdateCat 0x4bc030 / anm_UpdateSheep 0x4bc1a0
  now match) but rd.c calls nothing from anm.c, so no codegen coupling can exist; the callers compile identically
  under both hypotheses. The boundary rests on the .bss gap 0x6f0744 + the shared literal "dummy_TUER" 0x602854;
  a leaked name only would settle it.
- **f3snd.c .data** (session 8): its arrays sit at 0x602b08.. (after rt.c), its literals at 0x60b0dc..; the data
  between (0x603800..0x608fff) is used from all over the exe. Not a boundary question for the code (both parts are
  used only by 0x4c01b0..0x4c3180), but it may matter for the units that own 0x603800..: record their .data when
  they are reached. (session 9: cm.c's block starts at 0x60bd80, right after f3snd.c's literals.)

- **unk_58bbd0.c / unk_5d33b0.c** at 0x5d33b0 (layout 2026-10-24, session 13): new unit, the screen /
  front-end module (0x5d33b0 .. 0x5dd3e0; the 0x5dd3e0 .. 0x5de450 tail is data, see the next item). Evidence (plausible): 0x5d33b0 is the
  first function of the screen-file family (the extension pick over vfs_FindDir and the "%s_%s" 0x62d7d8
  entries), its first literal 0x62d7d8 follows the previous unit's last literal "special_uni" 0x62d6bc
  (+0x11c, D8; the trial / action callbacks 0x5d0f20 .. 0x5d3270 that precede it have no literals of their
  own), and the subject changes from the action records of the table 0x62c420 to the screen definitions.
  No leaked name. The unit's literals climb 0x62d7d8 .. 0x62de6c in function order; no internal boundary
  found yet. Lead: a leaked name only. Session 14 drafted the unit to its end (0x5dd100; contiguous
  0x5d33b0 .. 0x5dd3e0, 63 functions) and found no internal boundary: the window / contact subjects and
  the literal order (0x62dbdc .. 0x62de6c) continue the one block.
- **The data tail 0x5dd3e0 .. 0x5de450** (layout 2026-10-24, session 14): 4208 B of tables, not code:
  256 records {0x5eee70, index, 0x8000ii0c, 0} then {0x5eee90 / 0x5eeea0, index, 0xffff03, 0}; no
  prologue (`55 8b ec`), no `ret` anywhere. The last function is 0x5dd100 (ret 0x5dd3d6 + nop fill). The
  ledger cannot explain the bytes: R5 `@bytes` requires the range inside a unit, and R7 ends the last
  unit at the first undrafted byte, so the tail is the frontier forever and the unit stays open.
  **Lead (tooling)**: allow `@bytes` declarations for the bytes after the last unit, or cut the game
  range in config/symbols/game_range.toml at 0x5dd3e0. Nothing about it is a source question.

## Type debt
- layout 2026-10-24 (session 14): unk_5d33b0.c repeats the control record 0xf8f080 + id * 0x2ec (+0x48 /
  +0x4c visible, +0x50, +0x1c0 = 3, +0x1c4 / +0x1cc / +0x1d0 the text box, +0x2e8 the value), the screen
  record 0xf8f120 + id * 0xf1, the counter 0xf8f368 + id * 0x3c4 and the 0x64-byte blit record (the local
  SCRSURF type: +8 height, +0xc width, +0x10 pitch, +0x24 bitmap, +0x54 = 0x10, +0x58 .. +0x60 the
  screen globals). The character record 0x770b80 + i * 0x21c (word +0 the id, +2 the kind, +4 the
  handle, +0x20a the dword, the byte 0x770d4c + i * 0x87 * 4 the flags) is read by offset all over the
  gypsy / listener code.
- layout 2026-10-24 (session 13): the save / load family (unk_5d33b0.c) repeats the slot record of 0x220
  bytes (the name at +9, the picture id at +8, the kind byte at +0, the 0x84 dwords at +0x10), the control
  table 0xf8f080 + id * 0x2ec (visible +0x4c, +0x5c, the word +0x14, the picture +0x1c0) and the screen
  records 0xf8f120 + id * 0xf1 / 0xf8f38c + id * 0x3c4 (the selected id 0xf8aad0) by offset. The option
  globals 0x6f35c0 .. 0x6f366c are packed bytes; 0x5d4c70 / 0x5d5640 read / write them by mask.
- layout 2026-10-24 (tenth): unk_58bbd0.c's five bar charts take twelve int parameters (the ten values and
  the two screen words of 0xf8f120 + screen * 0x3c4: +8 the height - 1, +0xa the width), and 0x4764f0 is a
  seven argument fill (x1, y1, x2, y2, r, g, b) declared K&R in the unit (`int ai_Unk4764f0();`); the
  statistics table 0x91ab0c (5 dwords per entry, stride 0x14, the float fields at +0 / +0xc / +0x10 / +0x14
  ...) is untyped, and the AIMSG record fields of the privilege handlers are read by offset (+2 the type,
  +4 the handle, +9 a flag, +0xd a byte, +0x24 an amount, +0x5c the partner record, +0x194 the choice count,
  +0x1c8 the flags). 0x506180 is declared locally as gm_Unk506180 (defined in f3gm.c), 0x505840 as
  gm_Unk505840; 0x4fe2c0 (the date formatter, am.h / unk_4b6480.h view) is not called in the drafts yet.
- layout 2026-10-24 (fifth): the town map record of 0x5a6cb0 / 0x5a7cd0 (the entries at +0xdc 0x100 x 0x18;
  +8 screen; the 0x300 ids +0x18dc; the effect ids +0x24dc; the ambush ids +0x26dc; +0x27a4 the count; +0x27a8
  the saved screen) and the screen record 0xf8f120 + screen * 0x3c4 (the half words +8 / +0xa, the scroll
  +0x248 / +0x258, the object +0) are untyped; the control records 0xf8f080 + id * 0x2ec (the visible +0x4c,
  the picture +0x1bc / +0x1c0, the rectangle +0x1e .. +0x24, the object +0x2e8, the record +0x40); the
  production settings records (the list 0x6f26c0, 0x4c bytes, the good at +0 but read as an int at the odd +1
  of the caller block, the seven {good, weight, lock} slots +0xc stride 8); the character record 0x770b80 +
  index * 0x21c (the kind +2, the id +0x54). 0x458a20 / 0x45f170 keep the rt.h views (the map calls them with
  an int and a 16-byte position).
- layout 2026-10-24 (worktree, fourth): the kidnapping / city watch family repeats the AIMSG by byte offsets (the
  city watch types 0x43 (the crew ids +0x70), 0x44 (the crew at +0x38) and 0x65 (the name at +0x58, the wanted
  ids at +0x38) carry the clock 0x770ae0 .. +0x34; the kidnapping types 0x3c / 0x3e / 0x64 as in the unit note).
  New typed facts: 0x42ec20 and 0x504ec0 return float (declared `float` in unk_58bbd0.c); 0x4a63f0 takes seven
  arguments (param_1, 0, the id, 0, 0, 100.0f, &out); the control records from 0x475b80 are compared with
  0x5fe710 and read `U_I(ctrl + 1)`; 0x5b2180's callback receives the request record and a text pointer (the
  city watch windows use the second argument as a name string, 0x5a6270); 0x501190 (1, 1, ctrl) starts a
  building walk whose result 0x5a5120 stores but never reads.
- layout 2026-10-23 (session 19): **0x573200 (he.c) has three parameters**, not two: it searches the events of
  the type 0x8d by `*(int *)(a + 1)` and compares the event's +0xb0 with the SECOND argument and its +0xb4 byte
  with the THIRD (disassembly 0x573221: `mov esi,[esp+0x14]` = arg3, `mov edi,[esp+0x10]` = arg2). Callers:
  unk_575860.c 0x582730 (`ai_Unk573200(rec, *e, (char)p[3])`) and 0x584900 (`(rec, ev + 4, ev + 8)`). he.c's
  draft `int he_Unk573200(char *a, int b)` compares `HM_I(r, 0xb0) == (int)a` and `HM_B(r, 0xb4) == b` - both
  wrong; fix when he.c is next claimed (the function does not match either, so no bytes move today).
- layout 2026-10-23 (session 19): unk_575860.c declares 0x4f2660 / 0x42ec20 as `float f();` - with int
  declarations the seance drafts compile to the wrong call sequence (0x585cb0 went 440/656 -> 213/640). The
  family's message senders: 0x4c27d0 (handle, 0, text, value, 0xfa), 0x4c2760 (handle, 0, text, value).
- layout 2026-10-23 (session 18): the message windows of unk_575860.c (0x57dc50, 0x581020 .. 0x581db0, 0x582b40)
  build the 0xf8-byte AIMSG by offset (`*(unsigned char *)(msg + 4) = 0x99`); ai.h's AIMSG is the type (the field
  offsets match: +4 type, +8 the character, +0xc the record + 1, +0x28 the time, +0x36 a byte, +0x58 .. the args).
  The contact entry records (0x4ef090 / 0x4ef170 / 0x4ef6b0 / 0x4ef7c0: +0 the id, +4 the sub-entries of 0x44
  bytes) and the window list (0 window, 2 / 3 screen part, 4 .. 6 fonts, 0x4d entry, 0x4e detail, 0x50 items of 3
  dwords, 0x51 widgets) are untyped. 0x5de518 is __ftol; its FPU arguments are lost in four drafts (0x581020,
  0x581400, 0x581db0, 0x582b40).
- layout 2026-10-23 (session 6): he.c declares its callees K&R (implicit int) and the event records by absolute offsets (`ev + 0x52` due clock, +0x70 state, +0x78 flags, +0x84 the pending 0x4c4c60 handle; ai.h has the AIJOB view of the same records, see ai.h): convert to AIJOB when the unit is matched. HETIME = ai.h AITIME (14 bytes, pack(2)): he.c has its own copy and `extern HETIME ai_Time`; unify with ai.h. The message sender he_Unk53dda0 / 53dee0 are variadic-free but have 6 / 9 parameters (the fourth of 0x53dda0 is unused).
- layout 2026-10-23 (session 5): sv.c / main.c use the SV_I / SV_S / SV_B / U_I absolute-address macros for the globals that have no extern (the drafts after 0x522f10); the callees are K&R `int ai_Unk<addr>()` (implicit int in main.c); ai_Unk45d9a0 / ai_Unk45d960 take floats (prototyped in sv.c), ai_Unk448630 is a char *(float, float, int, int); sv_Unk5309a0 (cb, mask) is called from main.c through an implicit declaration. LESSON: a named `extern int ai_Unk<addr>;` global schedules better than the absolute macros (0x52fa10 / 0x52fa90 matched after the switch): convert before matching.
- layout 2026-10-23 (session 3): ls.c / io_text.c declare their callees K&R and their globals by address (ai_Unk<addr>), the save game helpers LS_W / LS_R (macros, ls.c); f3gm.c: gm_Unk50c550 declares ai_Unk889f74 (also in ls.c, f3init.c: each file its own `extern char *`); ai_Unk6f35cc is an int (f3init.c), ai_Unk633738 an `int *` (ls.c; ai_Unk633738 is `int` elsewhere in f3init.c: cast).
- layout 2026-10-22 (session 17): ord.c shares gv.h with gv.c (K&R callees, the tables unk_Tab91.. by address name); gv.h declares ai_Unk4fa700 with an unsigned short first parameter, d2_Unk4741f0 / 475600 / d2_SetForm / 4758f0 and ai_Unk5309a0 / 515960 K&R (cm.h has the same as K&R), d3sm_GetVertex K&R, ai_Unk6f1b9c as an int (it is a float: the code casts), ai_Unk770ae0 as char[] (the clock; ai.h AITIME ai_Time), unk_Ptrfd7c50 (a char * of the scene at 0xfd7c50; rt.h / cm.h call it ai_Unkfd7c50, a char array of records: the same address, two views). Split ord.h from gv.h when ord.c gets a unit of its own.
- am.c / am.h (layout 2026-10-22, session 5: one file, one header): the character views AIPX / AGCH / AIC4 / G5CH /
  AIPER and ai.h's AICHAR, AIARGX = AIARG, AGARG, G8REC / G9REC / AIRECX are still separate structs in am.h; many
  callees are declared twice with different views (C4028 warnings; the list in the old Type debt lines below still
  holds, with "unk_*.h" now meaning the am.h part marked "was unk_*.h"). Rename defines around the was-unk_490830.h
  part (ai_Unk4900c0 / 4902b0 / 4904c0 / 490600 / 4c4830 -> *_ag) and the was-unk_4a3af0.h part (ai_Unk4e7c40 ->
  ai_Unk4e7c40_g9, k an unsigned char); cast macros AI_4A63F0_7 (7-int view of 0x4a63f0 for 0x48ff50..0x490690),
  AI_5044B0_US (0x490690), G8_5BA620 (int id; 0x49a0a0.. and 0x4a5760), G8_4C4830I (int c). 0x495a50 stays at the
  end of am.c until its draft matches (see notes/units/am.md, Unit).
- Duplicate prototypes with different views: ai_Unk4a5760, ai_Unk5ba620, ai_Unk5cf8a0, ai_Unk48fb70 (unk_48f250.h
  vs unk_494e70.h / unk_49a0a0.h); ai_Unk509ee0 / ai_Unk509d30 (return float at some callers), ai_Unk4a58d0 (b
  is a float: macro G8_4A58D0), ai_Unk4fea50 (out pointer at some callers), ai_Unk4c3ba0 (ai.h void * vs am.h
  AIPER *). Character views G5CH / AIPER / AIPX / AGCH / AIC4 vs ai.h's AICHAR.
- layout 2026-10-22: unk_49a0a0.h declares ai_Unk5ba620(int) (the callers zero-extend; unk_48f250.h has unsigned
  short), copies ai_Unk4a4ae0 from unk_4a3af0.h and calls it with 7 arguments (G8_4A4AE0), G8_4C4830 (c is an
  unsigned char); unk_48e070.c casts ai_Unk4a63f0 to the __int64-mask prototype of am.h at its call sites.
- layout 2026-10-22 (session 2): ai_Unk4a5760 is void (unk_4a3af0.h; int in unk_48f250.h / unk_49a0a0.h);
  ai_Unk4a63f0's `int c` (am.h) is a float lo (defined with *(float *)&c); ai_Unk4a5b50 (unk_4a3af0.h) vs
  unk_490830.h's AGCH view; ai_Unk4a58d0's b read as *(float *)&b; ai_Unk4948f0 as void * in unk_49a0a0.h
  (G8_4948F0); ai_Unk50cc50 unsigned id in unk_49a0a0.h vs int in unk_48e070.h; ai_Unk5ed140 extern in
  unk_49a0a0.h and unk_4a3af0.h (no definition yet); d3t_GetObjectBounds / vfs_GetPath with void * views in
  unk_4a3af0.h.
- layout 2026-10-22 (session 3): unk_4a7140.h repeats ai_Unk4a5ef0 / ai_Unk4a5a30 / ai_Unk4a61d0 (unk_4a3af0.h), ai_Unk4c4830 (int / unsigned char views), ai_Unk5ba6b0 (AIPER view; unk_4a3af0.h G5CH), ai_Unk50cc50 / ai_Unk4f39b0 / ai_Unk5085b0 / ai_Unk5085f0 / ai_Unk4e6f80 / ai_Unk4e6fc0 (other unk_*.h); G10_4A58D0 casts ai_Unk4a58d0 (b float); G10_CB casts the callbacks to the 5-parameter view 0x4a9a00 / 0x4aa120 call them with. 0x4a8c10 (`return 1`) has three names: ai_Unk4a8c10 (names.csv, unk_4a7140.c), d3_Unk4a8c10 (d3_engine.h / evtc.h), ai_Unk4a8c10() in unk_49a0a0.h.
- layout 2026-10-22 (session 4): hmt.h declares callees of other units locally (list in notes/units/hmt.md) and
  ai_Unk4c4650 with 8 parameters (the 4th an unsigned short); G11_4C3D00 / G12_5176D0 cast macros; AICHAR bytes
  0x09 / 0x0d through byte macros (an ai.h view changed ai_Unk483a60); d3_mir.h declares d3_Unk454780 as char
  (d3_vec.h's copy removed); shp.h carries unk_479610.h's views of gfx.c's 0x478930 / 0x478960 (unsigned char *
  vs gfx.h's char * / D2RGB *); d3_int_io.c renames d3.h's void d3_SetParent (d3_interface.h: char).
- layout 2026-10-22 (session 6): txt.h and d2.h do not compile next to am.h (vfs_Root / vfs_Tfopen views, dd_RGB),
  so hst.c and unk_4b39f0.c declare what they use locally: G12TXTFILE (txt.h TXTFILE), G12WIN / G13OBJ / G13RADIO
  (d2.h D2WIN / D2OBJ (pad2e8 = user pointer) / D2RADIO), net_Conn as a 1- / 5-field struct (net.h NETCONN), and the
  d2_* / txt_* / sb_* prototypes (sb_IsPlaying / sb_Stop / sb_UnloadSampleBank with void *). ai_Unk4c4db0 returns int
  in unk_4b39f0.c (hmt.h / hst.c: void); ai_Unk515960 int in unk_4b39f0.c (hst.c: void); G13_501190 casts
  ai_Unk501190 to the variadic view (am.h repeats a 3-int one); G13_4FE1E0 (char hour / min / x); hst.c's
  ai_Unk4b1d20 .. use ai_Unk7e3f40[][0x2f4] (player names) and ai_Unk7d6a70[][0x55] (G12STOCK) as plain externs.
- layout 2026-10-22 (session 7): unk_4b6480.h declares callees locally: ai_Unk4e6f10, ai_Unk4fca40 (float, unsigned
  short id), ai_Unk4c6020 (int v), ai_Unk4c5010, ai_Unk4c4c60, ai_Unk5054f0 / 5056a0 / 5059d0 / 505ad0 (AICHAR *),
  ai_Unk4fe2c0 / 4fe330; cast macros G14_4FEA50 (object out as 2nd argument) and G14_5087F0 (unsigned char n; ai.h
  int); ai_Unk7e3f40 repeated from hst.c. anm.h does not include d3.h / ai.h / ch.h (they clash): it repeats
  d3_CurrentScene, d3_Time, d3_SelectScene, ai_Time (as char[14]), util_Random / util_RandomF, ai_Unk501190 (2-int
  view returning void *), ai_Unk501030, ai_Unk502520 / 5024f0 (void *), d3_Unk458a20 / d3_VecEqual /
  d3_GetHeightAt (float * views), d3_DefaultScene (char *), ch_Create / ch_AddAction / ch_Delete / ch_QueueAnim
  (void * views), mem_Alloc / mem_Free.
- layout 2026-10-22 (session 8): anm.h includes rd.h (anm_Unk4bc2e0 / 4bc390 / 4bc520 moved there). rd.h declares
  views: d3_FindObject449690 (char * a; d3.h int), d3sm_GetVertex / ch_FindFreeCell / d3_GetHeightAt (char *, float
  * for D3SM / D3VECTOR), d3_ForEachObject (void *cb, void *ctx), d3sm_Create / d3sm_Destroy (G15GRID), path_Find
  (int *out, G15GRID *), ai_Unkfd7c4c as G15MAP * (am.h AIMAPREF[]), d3_Unk633738 as int * (d3.h void *). rt.h:
  d2_Unk475ff0 with 3 parameters (d2.h 2), d2_Unk472d20 with 1 (d2.h 2), d2_Unk475cb0 with 4 (no prototype in d2.h),
  ai_Unk509ee0 (void *), ai_Unk515960 variadic (int id, ...), cast macro ai_Unk52e350x (7-parameter view);
  d2_Objs as char *. f3snd.h: sb_* with char * / int views of SBSAMPLE / banks, msx_* with void *, ch_Count (int),
  ai_Time as char[14] (as anm.h); the tables 0x602b10 / 0x602ba8 / 0x602bd8 / 0x602d70 / 0x6094d0, rd.c's 0x602860
  and rt.c's 0x602a90 (defined, = -1) are these units' .data: only rt_Unk602a90 is defined in C so far.
- layout 2026-10-22 (session 9): cm.h declares its callees locally with char * / void * views: ai_Unk505df0,
  ai_Unk502020, ai_Unk501190 (variadic), ai_Unk4ffe90, ai_Unk53e400 (char *; ai.h AICHAR / AIBLD / AIOBJ / AIJOB *),
  ai_Unk4ffc40 (char *; ai.h int *), ai_Unk506430 (char *; ai.h AIFIN *(AICHAR *)), ai_Unk4fea50 (void *a, *b, *c;
  three object views out), ai_Unk500970 / 500a40 / 500ae0 (int good; hmt.h: void * n), ai_Unk4c4db0 (unsigned char
  id; hmt.h int), ai_Unk4c3b10 (int; hst.h AICHAR *), ai_Unk4c3ba0 (char *o; ai.h void *, am.h AIPER *), ai_Unk4c3c00 /
  4c3d00 / 4c3d90 (unsigned char size / n, unsigned short field; ai.h / am.h int / short views), ai_Unk4c4650 /
  4c4830 / 4c4880 (unsigned char params; am.h int views), rt_Unk4bdaa0 (char *; rt.h RTPOINT *), mem_Alloc /
  mem_Free / err_Message / sb_PlaySample / util_Seed repeated; ai_Goods / ai_Unk6f1bf8 as char * (ai.h AIGOOD * /
  unsigned short *), ai_Time as char[14], ai_Unk770b80 / ai_Unk7e3f40 as char[][n], ai_Unk770b84 as int[][0x87].
- layout 2026-10-22 (session 10): cm.h declares callees of other units with K&R `int f();` (no parameter list; the views differ per
  caller): ai_Unk4fea50 / 4ffe90 / 501190 / 505df0 users pass chars and ints, the ch_* / d3if_* / d3_* / evt_* / tb_* functions by
  their real names without prototypes (ch_FindByName, ch_StandUp, ch_UseGate, d3if_SelectScene, d3t_SceneIndex, d3_FindObject449690 /
  449770, d3_Unk458a20, d3_GetHeightAt, evt_FindById, evt_Stop, tb_*), float results declared per function (ai_Unk4539f0, ai_Unk504ec0,
  ai_Unk4fc8e0, ai_Unk448ac0). New views: G18LOG (chat line, 0x6c), G18POP (floating number, 0x14), G17B16, `_strupr` of the CRT
  (0x5ea986) for the "dummy_gate_ob_%s" names. Data: ai_Unk7e3f30 is a pointer (building table, 0x24d bytes per kind), ai_Unkfd7c50 a
  char array of 0x3e0-byte scene records (rt.h: void *).
- layout 2026-10-22 (session 11): cm.h keeps adding K&R callees (`int f();`) and externs for the fight code; sound handles ai_Unk6f0d24 .. 48 are ints
  (were char *); ai_Unk4d0380 is defined with a 4th (unused) parameter, called through G19_4D0380 / ai_Unk4d0460 through a cast; G19_5176D0
  (variadic cast of ai_Unk5176d0), G19_E / G19_R (record fields by offset). d3_tools.c / rd.c now call _strnicmp (0x5eaac0) where they had strncmp
  (0x5dfb40 is the real strncmp).
- layout 2026-10-22 (session 12): cm.h: ai_Unk4d4540 takes one argument; K&R callees of the fight UI (d2_Unk4741f0, d2_Unk475850, d2_SetForm,
  d2_Unk475b80, ai_Unk515960, ai_Unk4e5470, ai_Unk50aa00 ..) and externs (0x6f0cf8 / 0x6f0cfc / 0x6f22a0 / 0x143e948 ..); G19_4C4880U (int view
  of ai_Unk4c4880's short c: the callers zero-extend with xor; 4d3d10 / 4d3ea0 / 4d4d70 use it), G20_US / G20_ATT / G20_DEF (0x6f0ce8 / 0x6f0cec
  as char *), G20_PUMP / G20_TAKE / G20_FACE / G20_GUARDS (the repeated blocks of 0x4d4d70). ai_Unk6f0ce8 / cec stay ints (cast at the use).
- layout 2026-10-22 (session 13): cut.h declares everything with K&R `int f();` (the scene helpers 0x4e2c30 / 0x4e3490 / 0x4e32d0 / 0x4e3570 ..), char / int
  views of ai_Unk770b80 (characters, kind at +2) / ai_Unk770b84[][0x87]; ai_Unk4fb390 (a person's name record, +4 the text) is `int` there (callers cast to char *);
  ai_Unk4de530 takes (char *c, int dmg) (cm.h-free); the character copies are `char[0x21c]`. cm.h gained the fight-loop externs (ai_Unk6f0ce4, 0x143e674,
  0x92ca80 table, ai_Unk6f0da4 ..), msx_PlayAmbient / ch_PauseAnim as K&R.
- layout 2026-10-22 (session 14): cm.h now holds cut.h (cut.h removed): the scene code declares its callees K&R (`int f();`) and a few with a cast macro (G22_4C5580 (no arguments), 
  ((int (*)())ai_Unk4c61f0) / 4c4fd0 / 4c5f20 for the results the void prototypes of the net code drop, ((int (*)(unsigned short))ai_Unk50a640) for a zero-extension-free word), 
  AI_ZERO / AI_ZERO_AT copies of am.h / d3.h macros (the headers do not compile together), G22ENT, G28REC (the party records 0x925520, 0x114 bytes), K&R externs for the
  scene tables 0x92c500 .. 0x92c510 (5 ints per scene kind: run function, two callbacks, flag byte, check). amt.h repeats AI_ZERO / AI_ZERO_AT and declares ai_Unk4e6f10 / 504ad0 / 4e6f80 / 4f2720 K&R.
- layout 2026-10-23 (session 2): f3gm.c still includes gv.h only and declares K&R callees (ai_Unk<addr>() names; the real names of cm.c / d3 / d2 functions are not used: the link resolves them by address); f3init.c declares the f3gm.c functions it calls by prototype at its top (gm_Query / gm_QueryNext / gm_BuildQuery / gm_BuildNext / gm_Unk5085f0 / gm_Unk5087f0 / gm_Unk50ab50 / gm_Unk50b010 / gm_Unk508740 / gm_IsHomeType): move them to a header f3gm.h when f3gm.c gets one. A comment that names two addresses on one extern line makes uv run link resolve the symbol by the first address only: one extern per address.
- layout 2026-10-23 (session 14): he.c declares the callees of the events 0x88 .. 0x94 in one block before 0x56f980 (`int ai_Unk...();`), plus the forward declarations of the handler functions as `void he_Unk<addr>(int ev);` (they are address-taken as `(void *)` in 0x56f980, so the type change is byte-neutral). ai_Unk4fca40 is declared `int ()` file-wide but the original passes a bare word and pops the float result: the two new callers (0x56f2e0, 0x5719e0) cast it `(float (*)(unsigned short, int))`; the older callers still use the int view (check them when they are matched). ai_Unk4c3d00 is called `(1, 1, rec, &out)` from 0x571c30 and 0x5720f0. ai_Unk509ee0 takes (char *, char) in 0x5719e0 (cast).
- layout 2026-10-23 (session 13): he.c declares ai_Unk4a63f0 locally with a 7-dword view (0x56e5b0, the domicile
  action call: (per, 0, 0xa7f4c7a0, 1, 0, 100.0f, (int *)(ev + 0xac)); am.h has the 6-arg __int64-mask view) — same
  as unk_48e070.c (cast at the call sites). Unify when he.c's drafts match.
- layout 2026-10-23 (session 15): he.c: 0x5de518 is `__ftol` (the float -> int conversion of 0x5746b0 / 0x575430 / 0x575570; declared
  `int ai_Unk5de518()` so far). 0x5089c0 / 0x4c4880 / 0x4c4830 want an `unsigned char` parameter at the call sites that pass
  `*(unsigned char *)0x6f1bc5` (the original pushes a bare byte; a K&R declaration zero-extends and shifts the bytes, and the
  cast prototypes at 0x573570 / 0x573e30 fixed it). The message clock of the new handlers (0x573970 .. 0x575640) is written field
  by field (HM_I/HM_B at +0x28 .. +0x36, flag 1), not by a HETIME assignment (only 0x575150 / 0x575320 use the HETIME copy).
  New globals: 0x6f2448 (the 0xa0 gate), 0x6f263c (the message filter mask), 0x6fa0e0 (the INI path buffer, the copy target of
  0x5755c0), 0x62916c "Game" / 0x629174 "message_filter_mask" / 0x629188 "%d", 0x62901c / 0x629038 / 0x629054 / 0x629070 /
  0x631090 (the name / text blocks of the 0x9d / 0xa0 / 0xa1 approaches), 0x770b80 record fields +0x164 (a "no longer works"
  byte), +0x166 (the relation target), +0x170 (a record with the word +1), +0x194 (the amount / level). The 0xa1 events use the
  words +0xc0 .. +0xce (the meeting rooms, 7 / 0x16 / 0xf / 0xa) and +0xb0 / +0xb4 = 0xf0.
- Layout session 18 (continuation, same 10-minute timer): frontier 0x583700 -> 0x584410 (the graveyard windows
  "Locations_Friedhof_Friedhof" 0x62a5bc / "_RentAGrave" 0x62a5d8 / "_RentAGrave2" 0x62a59c). Drafted 0x583700
  (ROUGH, the detail panel), 0x583b70 (matched), 0x583c50, 0x583de0, 0x583ef0, 0x584010, 0x584260. Next:
  0x584410 (264 B, the function after the RentAGrave window).
- layout 2026-10-23 (session 21): the network lobby and the production window (unk_589f10.c / unk_58bbd0.c)
  declare their callees K&R (`int f();`) and their globals by address. Prototypes worth keeping: 0x5de652 is the
  sprintf helper (buffer, format, args); 0x475cb0 (control, value) sets a slider / counter and 0x475cb0 (control,
  0xc) the range; 0x475b80 (window, control, text) returns the control id; 0x473650 (x, y, text, screen) and
  0x4731d0 (x, y, image, screen) also return the control id, and 0x473650's y is a short (Ghidra shows a CONCAT22);
  0x5309a0 (callback, mask) is the frame pump; 0x4cd380 (id) resolves a network record id to its record (0 when
  gone); 0x4cd610 / 0x4c3ae0 answer a pending network request (0 = pending, 0x52fe00 pumps), 0x4cd5d0 / 0x4cd5f0 /
  0x4cd5f0 return the player list / the chosen player; 0x4cd650 / 0x4cd6a0 / 0x4cd6e0 / 0x4cd7c0 create / send the
  player records of the lobby. The lobby's player record 0x6f5e20: +0xb the profession, +0xc the city / emblem id
  (+0x59a based), +0x10 the gender word (byte +0x10 the gender, +0x11 the religion), +0x12 the name, +0x22 the
  family name, +0x2a / +0x2b / +0x30 / +0x31 the bytes copied to 0x75e5a2 .., 0x6f5e78 the chosen player record,
  0x6f267c the player list, 0x6f5ea0 the selected list index, 0x6f5e00 the selected record id, 0x6f5ea4 the lobby
  state record (+4 = a refresh is pending). The window table 0xf8f080 is 0x2ec bytes per window; the control
  table 0xfa5aac / 0xfa6030 / 0xfa6048 / 0xfa617c is 0x160 bytes per control. The production window
  (unk_58bbd0.c): the building kind record 0x7e3f30 + kind * 0x24d (byte 0 the kind), the character goods
  0x91aaf0 (0x41 words per record), the goods list 0x74e1f4, the columns 0x6f5320 / 0x6f5350 / 0x6f5354 /
  0x6f5358 / 0x6f536c (0x58 bytes each), the carriers 0x75f290 / 0x75f2a0 (10 dwords each), the flag array
  0x770d08 (0x21c bytes per character), the event 0x576040 (0x1f, ...) opens / closes the screen.
- layout 2026-10-24 (worktree): the unk_58bbd0.c stretch declares its callees K&R (`int f();`) and its globals by
  address (U_I / U_W macros). Worth keeping: 0x4731d0 (x, y, id, screen) returns the control id (4 arguments,
  the second a short y); 0x475b80 (window, tree, text) returns the control id; 0x475f70 (window, page) selects a
  page; 0x52e1c0 (name, message id, class) is the contact lookup (0 when absent); 0x52e190 resets the contact
  list; 0x5309a0 (callback, mask) is the frame pump (0x67fff / 0x677c7 / 0x777c7); 0x5176d0 (buffer, text id,
  ...) formats a text; 0x521150 (buffer, modal) shows it; 0x4c4b70 (message record) sends an AIMSG and returns
  the handle, 0x4c3ae0 (handle) confirms it, 0x4c3b10 releases it; 0x4c2d20 / 0x4c4540 / 0x4c4580 bracket the
  message building. 0x5992c0 takes a third argument (the offer source) that the Ghidra call of 0x599aa0 loses.
  The build/ghidra exports are missing for the whole 0x58f510 .. 0x599aa0 stretch (the pseudocode came from
  inline REA batch_decompile results; uv run rea_export needs the saved JSON, which was not kept).
- layout 2026-10-24 (worktree, third): the church rooms of unk_58bbd0.c repeat am.c's helpers with K&R views
  (0x4f39b0 the fame, 0x4f39e0 the rank of a character id, 0x50c550 (a, b, 1) the relation 0 .. 200 - am.c uses
  it typed -, 0x5ba6b0 (the record, 0x2c6) halves the price, 0x5a8b80 / 0x5a8c00 the relation bookkeeping of a
  building, 0x4c5f60 the relation delta; am.c 0x49c8e0 / 0x49cb60 is the same code with typed G5CH / AIARG
  views). The church price uses 0.1 where am.c uses 0.01 (0x5eb8d8 = 0.1 double in the church .rdata): do not
  "unify" that constant. 0x5a1af0 / 0x5a1f70 / 0x5a23a0 / 0x5a2b60 / 0x5a2e60 declare their callees K&R and
  their globals by address (U_I / U_W); 0x52d7a0 / 0x4758f0 are the two exits, 0x5309a0 the pump.
- layout 2026-10-24 (worktree, second): 0x5085b0 is called with a THIRD ignored argument at several sites
  (`ai_Unk5085b0(a, k, prev)`; e.g. 0x59aae0, 0x59b880: the callee reads only [esp+4] / [esp+8]), so the K&R
  declaration must stay argument-free. 0x599ef0 takes a second (unused) argument (0x59a3c0 pushes it); declared
  `void ai_Unk599ef0(char *param_1, int param_2)`. 0x59a800 is void (its earlier K&R declaration was changed to
  `void`), 0x59e420 likewise. The city point objects are renamed at D3OBJECT +0 (name[0x4c]): 0x59a3c0 writes
  "stadt_%s" over the object returned by 0x446e10 (object +0x4c is the position, +0x220 / +0x224 / +0x211 /
  +0x212 the city point flags). AIMSG offsets used by the new listeners: type at +4 (0x74 tavern / 0x23 credit /
  0x27 messenger / 0x69 citizen), the character at +8, +0xc a second id, the clock +0x28 .. +0x34, +0x36 a byte
  (2 / 0x12 / 1), +0x58 / +0x5c the two parties, +0x60 / +0x64 / +0x6c the goods triple, +0x5c the city byte
  (0x27 message). 0x475ff0 (window, 2, 0x100, &a, &b, &c) is the combo / list; 0x52e350 (screen, 0, out, n, id)
  fills n ids into out; 0x5bd470 / 0x5bd330 are the Rathaus list builders of 0x59dc00.
- layout 2026-10-24 (worktree, seventh): 0x4fea50's out view is (character, building, other object) - the
  result slots are compared against 0 and dereferenced at +0x5d / +0x14 / +0x178 (cm.c 0x4c6350); in 0x5ae8e0
  the first two out slots are the function's own parameter slots, so it is `ai_Unk4fea50(&param_2, &param_1,
  &out, id)` and the original parameter value must be saved in a local. 0x5089c0 is `float (int, unsigned char)`
  (ai.h / cm.h / rt.h) - the K&R view in this file made 0x5af010 convert its result as an int; the file now
  repeats the float prototype. 0x5af1d0 takes (param_1, the mode 1 / 2 / 4). The picture loader of 0x5b2090:
  0x478b50 (path) returns a record (width +4, height +8, +0x1c), 0x478cc0 (a 0x40-byte record) resolves it,
  0x479020 loads, 0x478bd0 (0, 0, width, height, the +0x1c, 0, 0, id) makes the control, 0x4790e0 / 0x478fc0
  free (globals 0x6f26ac the id, 0x6f4ee8 / 0x6f4ee4 the width / height, 0x62c270 the scale). The kind record
  0x7e3f30 + kind * 0x24d carries the option limits at +0x240 / +0x241 / +0x242 (0x5b2530).
- layout 2026-10-24 (worktree, sixth): the Domizil family of unk_58bbd0.c: the Domizil table 0x62c420 has 0x42
  entries of 0x1c bytes (the id word at +0, a range word at +0x18, the count dword 0x62cb58); the window record of
  the Domizil boxes: +0 x, +1 y (the builder 0x5aa0d0 takes x / y), +2 the character pointer (a word at +0), +3 the
  picture control, +4 the slider, +5 / +6 the two text controls, +0xa .. +0x15 the freed ids, +0x14 / +0x18 the
  text controls of the selected-entry panel, +0x20 the state byte (0 / 1 / 2), +0x16 the window / page; the
  character record 0x770b80 + index * 0x21c with the kind +2, the id +4, the +8 byte, the byte +9, the name
  buffer +0x4071? (0x5a9e40 copies from +0x4071 of the record, a pooled text) and the +0x165 byte; the building
  kind record 0x7e3f30 + kind * 0x24d with the kinds 6 / 7 and the bytes +0x22f / +0x230 / +0x231 / +0x232.
  0x53dee0 is `he_Unk53dee0(id, -1, buffer, flag, text, a6, a7, 0, key)`: a6 / a7 the two id slots (the master
  name at the +1 field of the record at +0x16c, the +2 field, or -1); 0x5176d0 also takes a format string
  (%s 0x5f8318, "%i %s" 0x60d568) in place of a text id. 0x50c520 is `int (unsigned short, unsigned short)`
  (ai.h; the file now repeats that prototype instead of a K&R one to keep the cross-TU lint clean). The worker
  list arrays: 0x6f40a0 the count, 0x6f40a4 the record pointers, 0x6f40b0 / 0x6f40bc the icon / text controls.
  The transport windows 0x5aab80 / 0x5ac110 / 0x5ad150 are stubs: their records hold the entry array at +0x22c
  (0xc entries of 0x5c bytes: +0 the character pointer, +8 the text control, +0xc the slider, +0x14 / +0x18 the
  text controls, +0x20 the state, +0x24 the +0x4c entry, +0x4c / +0x48 the freed units) and the count bytes
  +0x231 / +0x232; the AIMSG order messages: +4 type 0xf, +8 the character, +0x28 .. +0x34 the clock, +0x36 byte 2,
  +0x58 / +0x5c / +0x60 the record / the two parties, +0x78 the char +0x28 of the transport record (also the
  global 0x6f2f64). 0x50aa00 fills a 0xb8-byte inventory record, 0x50a970 its +0xb0 int field by the amount;
  0x4c4880 / 0x50aa00 / 0x4fe140 are called here through implicit declarations (their ai.h prototypes disagree
  with the K&R ones in other files; a local view needs the same return type); 0x50a0f0 returns `char *`.

- layout 2026-10-24 (worktree, eighth): the special-amount family of unk_58bbd0.c works on records of 0xe
  dwords (+0 the character, +4 the row control, +8 the slider, +0xc a text id, +0x10 / +0x14 x / y, +0x18 the
  icon, +0x1c .. +0x28 the four text controls, +0x2c the class 0 .. 8, +0x30 the 0x5c33c0 result). The fills
  0x5bf0d0 / 0x5bf300 / 0x5bf4f0 / 0x5bf6f0 and the row builders 0x5bd5e0 / 0x5bdb60 are declared K&R here
  (the fill functions are called through the table 0x62d154). New typed facts: 0x4fc820 returns float,
  0x46bf10 / 0x46c260 / 0x4e86e0 / 0x4e70f0 / 0x4e6fc0 are the list controls / fills (0x4e6fc0 returns
  unsigned int, am.h); 0x50c550 is float (already declared); 0x5c33c0 takes (word, word, unsigned int *, int)
  and its 3rd argument is the caller's `int *param_4` cast. Constants: the relation floors are the float
  0x5ed0fc and the same-world value 0x5eb788; the 0x5de518 __ftol converts the relation slot. The dialog
  record 0xfa5aa0 + id * 0x8c: +0 the flags (dword, bit 1), +8 the selected entry; 0x6f2f84 keeps the last
  selection of the stats panel. The tab table 0x62d14c: eight 0x14-byte entries ([0] count, [1] control id,
  [2] fill function, [3] row builder); 0x6f2f8c the selected tab. The menus' callbacks are the function
  pointers 0x5c1500 / 0x5c1e10 and the caption records 0x143a3f0 / 0x143f030 (36 B), the images 0x143ec88 /
  0x143fbe0; 0x475f70 (window, page), 0x475b80 (window, page, text), 0x476f80 (0, 0), 0x476030 (id),
  0x4760b0 (control, id), 0x46f1a0 / 0x476760 (the screen background / clip), 0x472d20 (screen, 1),
  0x472270 (-1, -1, screen, screen, 0x767), 0x46c490 (n, the control ids) and 0x46c630 / 0x46c600 (the
  dialog record id) are the control layer of this family.
- layout 2026-10-24 (worktree, ninth): the master letter / law / player-info stretch of unk_58bbd0.c:
  **0x5c2490 has 12 call values, not 9** - 0x5c2b90 calls it with (win, &a, &b, checks, param_1, *param_2,
  the four flag words at param_2 + 0x1b4 .. +0x1c0, &master, the record list), while its draft takes 9
  parameters; the callee reads only the first flag word. Unify when 0x5c2490 is approached. **0x5c3200 is
  void** (the draft declarations `int ai_Unk5c3200();` in unk_58bbd0.c x4 and unk_575860.c line 7440 are
  now `void`). 0x5c33c0 matches am.h's 4-parameter view (unsigned short, unsigned short, unsigned int *, int;
  the 4th is unused); the 3rd is the filter record. 0x4a5ef0 needs am.h's exact prototype
  (`struct AIPER *`, ...) or the cross-TU lint fails. 0x50a640 stays `int (unsigned short)` (Ghidra shows a
  second -1 argument at the 0x5c6050 call sites). New typed facts: the character record 0x770b80 + id *
  0x21c: +0 the id, +2 the kind, +4 the id dword, +8 the marriage byte, +9, +0x16c / +0x170 the owner
  pointers (kinds 4 .. 0x16 / 1 .. 2), +0x50, +0x5c the partner, +0x165 / +0x166 / +0x169, +0x184,
  +0x1b1, +0x1c8 the flags (0x20 the gift, 0x800000 / 0x40000000), +0x1cc 0x3000, +0x1f0, +0x210 the
  world; the building record (the law kind record): +0 the kind, +0x25 / +0x27 the owner words, +0x5a bit 1
  the dead flag, +0x61 the home; the filter record: +8 / +0xc the two mask words, +0x10 the target case
  0 .. 7, +0x14 bit 1 with the four +0x1c ids, +0x40 / +0x44 / +0x48 the relation range. The bar chart table
  0x91ab0c .. 0x91ac4c: 5 dwords per entry (the max dword +0, the float +4); 0x4764f0 (x, y, ...).
- layout 2026-10-24 (worktree, twelfth): the trial / interrogation stretch of unk_58bbd0.c declares
  0x4c4ac0 with a raw 4-byte float third argument (the cm.h int view is wrong; the call in 0x5cfb80 goes
  through `((void (*)(int, int, float))0x4c4ac0)`); 0x5b2180 takes a fifth argument (the request records
  are 0x14 dwords) and its definition in the file now has param_5; ai_Unk42ec20 is declared `float` in
  the file (as in unk_575860.c), which changes the byte diffs of the three draft callers 0x598170 /
  0x5a5530 / 0x5a5910 (none matched). The request record 0x5b2180 returns: the dword at +1 is the
  message +0x58 field and the 0x4c3c00 4th argument, +0x27 the owner word. Character records 0x770b80 +
  i * 0x21c: +9 the sex / flag byte compared against 0x6f25b8, +0x1c5 a packed byte read as
  `(x & 1) + (x >> 1)`, +0x16c the owner / building pointer. The trial messages: the type 0x82 record
  (+0x58 / +0x59 bytes, +0x5c / +0x60 the float bits 0xbe2aaaab / 0xbeaaaaab, +0x64 = 0x18), the types
  0x76 / 0x77 (+0x6e = 0x177 / 0x178), 0x53dda0's shape (the character + 4, -1, text, 0, 0x5fb, 0) and
  0x505ca0 (n, out) = a shuffled 0 .. n-1 index list (the f3gm.c definition).
