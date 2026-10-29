# sv.c (new, layout 2026-10-23 session 4)

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** first; below it the findings of each session, one section per author.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): screen layout, message boxes, camera, info panels, group selection, book, chat, frame loop
- Boundaries: open (layout 2026-10-23, 5th session): unit start unchanged (tag "sv_", see session 4). Drafted this session: 0x522f10, the group / selection code 0x52a050 .. 0x52c4e0, the plates / hint / rubber band 0x52cef0 .. 0x52f2c0, the book 0x52f360 .. 0x52f710, the scroll 0x52fa10, the network sync 0x52fc10 .. 0x52fe00, the chat console 0x52fe10, the weather 0x5301a0 / 0x530670, the frame loop "MASTER" 0x5309a0 (the callback stack 0x75f780, strings "MASTER(): Function already registrated!" 0x624534) and its callback 0x5315c0. End: 0x531670 .. is the program level code (window class "Die Gilde", the tag "main_"): split into main.c, so sv.c ends at 0x531670 (the last sv string is 0x624534). Still open: the unit end (0x531670) and start (0x520ec0) rest on the .data order only and many drafts are ROUGH (see notes/units/sv.md)
- Range then: 0x520ec0..0x5315c0 (all drafted; layout 2026-10-23 session 10 added the gaps 0x521c10, 0x52bbc0, 0x52bec0)
- Was: tag "sv_"

## Open work
- Gaps filled layout 2026-10-23 session 10 (ROUGH): 0x521c10 (the scene load progress callback, a Ghidra miss inside 0x521ba0's span; 608 B), 0x52bbc0 (moves the selected characters into a group, 784 / 768 B), 0x52bec0 (mouse release of the selection, 1376 / 1376 B: the __ftol operands decoded: off[9][2] * (50.0 / cell size) + c).- All functions of the unit up to the frame loop have source (0x520ec0 .. 0x5315c0); the unit ends at 0x531670 (main.c starts there). Not separated yet: the end rests on .data order only and many drafts are ROUGH (below).
- Matched (layout 2026-10-23, sessions 4 + 5): 0x520ec0, 0x520ee0, 0x520f10, 0x520f20, 0x520f90, 0x520fe0, 0x521040, 0x522100, 0x522280, 0x5223c0, 0x522ec0, 0x524010, 0x5241b0, 0x5241d0, 0x524900, 0x525440, 0x525a80, 0x5282f0, 0x528660, 0x528770, 0x528800, 0x529fb0, 0x52a040, 0x52ae40, 0x52b490, 0x52bb70, 0x52c420, 0x52c4a0, 0x52e050, 0x52e170, 0x52e190, 0x52e330, 0x52ea30, 0x52eed0, 0x52f340, 0x52f530, 0x52f9f0, 0x52fa00, 0x52fa10, 0x52fa90, 0x52fad0, 0x52fd80, 0x52fe00, 0x5315c0 (0x530100 is 2 bytes off: asmdiff shows identical instructions, check the `cmp` encodings).
- Near misses (session 5): 0x52e0c0 (the compiler hoists the two screen-centre loads; the original has the `fld` before the flag test), 0x52fcf0 (the key mask kept in edi), 0x52fb50 (`>= 0` loop shape), 0x52c4e0 (the original hoists 0x6333cc into esi), 0x521e70 (11 B, session 4), 0x5210a0, 0x525490, 0x528290 (session 4).
- ROUGH drafts of session 5 (the global accesses are absolute-address macros SV_I / SV_S ..; convert to named externs first, 0x52fa10 / 0x52fa90 matched that way): 0x522f10 (city / time panel, size equal 4352), 0x528180, 0x52a050, 0x52a680, 0x52adb0 .. 0x52b3c0, 0x52ae70, 0x52b500 (the extras of the group: names "bewohner", "besucher", "meister", "pause", "faulenzen"), 0x52c5c0 (speech bubbles), 0x52cef0 / 0x52d010 / 0x52d1e0 / 0x52d2a0 (plates), 0x52d480 / 0x52d7a0 / 0x52d940 (view arrows, camera follow, click handler), 0x52e1c0, 0x52e350 / 0x52e4b0 (button rows, varargs), 0x52e600 .. 0x52eb50 (forms, bars: the float operands of the tile counts are LOST in the pseudocode: placeholders (int)0.0f), 0x52ee50 / 0x52eee0 / 0x52f0d0 / 0x52f2c0 (rubber band selection: the screen position floats lost), the book 0x52f360 .. 0x52f710, 0x52fc10 .. 0x52fe10 (network sync, chat console), 0x5301a0 / 0x530670 (weather: the emitter floats lost), 0x5307f0, 0x5309a0 (the frame loop "MASTER": the callback stack 0x75f780 (8 B per entry, depth 0x6f229c), the mask bits in the comment).
- Earlier ROUGH: the message boxes 0x521150 / 0x521410 / 0x5216a0, the camera code 0x525dd0 .. 0x5263a0, 0x526cf0, 0x527640 .. 0x527e90, the panels 0x528860 .. 0x529380, 0x524260 / 0x524990, 0x525220 / 0x525510 / 0x525ad0.
- Types: the widget table 0xf8f080 (0x2ec-byte records, accessed through SV_W / SV_WI macros: +0x4c shown, +0x5c, +0x14 / +0x16 size, +0x1c0 kind flags (0x43 = shown button, 3 = plain), +0x74 picture / font, +0x78 frame, +0xbc text, +0x1e0, +0x1e4 progress float, +0x2e8 the character pointer), 0xf8f128 the form records (0x3c4 bytes: +0x6 / +0x4 size, +0x39c), 0x75ec80 the role table (0x20 bytes: id, name[0x10], flag, +0x18, +0x1c), 0x761fc0 the speech bubble table (0x40 entries of 0x10c: +4 time, +8 object, +0xc text), 0x760640 the plate table (0x20 of 200 bytes), 0x7603a0 the group table (0x20 character pointers).

## layout 2026-10-23 session 4
- Boundary: 0x520ec0 is the first function of sv.c (a 32-byte callback `fild [0x623790]; mov [0x623790], 0; fstp [0x62378c]` registered with 0x42e970 (cb, 0x47) by 0x520ee0 and unregistered by 0x520f10); ls.c's last function 0x520b80 ends with its ret at 0x520eb6.
- Message boxes: forms "misc\\Messagebox" / "_Autosize" (flag 2) / "_BIG" (0x10) / "_VERY_BIG" (0x20) / "_NONE_PERGA" (0x100), flags 4 (auto close after 500 ms), 8 (all message kinds, mask 0x717c6), 0x40, 0x80 (countdown bar); 0x521150 returns the button + 1, 0x5216a0 1 / 0, 0x521410 the green box with a title text.
- Camera: the camera object 0xfc2dec (+0x4c position, +0x84 direction, +0x18c matrix), zoom 0x6f22d4 / 0x6f22d8 (0 .. 1) blended between the height pairs 0x623800 .. 0x623808 and the angle pair 0x623804 .. 0x62380c; presets at +0x5c / +0xb4 / +0xcc / +0xe4 / +0xfc (position + direction); the player's saved camera is the character record 0x506430 (+0x88 .. +0xa4). 0x525dd0 / 0x525ee0 / 0x525ff0 / 0x5263a0 / 0x526cf0 / 0x527640 are the per-frame control, 0x527840 / 0x527bb0 / 0x527e90 focus an object.
- Day / weather: 0x525a00 fills the phase table 0x623ba9 (6 rows of 10 bytes) from 0x5eeab8; 0x525ad0 maps the clock to 0 .. 600 and calls 0x449de0 (day, fraction, 1.0, changed); 0x525510 rerolls the 24 cloud counters 0x760488 and creates the sky objects "Sky_Schoen_01 / 02", "sky_dunkel_01".
- Groups: the group record (+0x8c 8 member ids, +0x14 / +0x18 counts) is maintained by 0x524100 / 0x524160 / 0x5241b0 / 0x5241d0; 0x524260 moves a character between groups (messages 0x33 / 0x39 with the tag "sv_ChangePlayerHE" 0x624120).

## layout 2026-10-23 session 5
- Boundary: sv.c ends at 0x531670 (see notes/layout.md); 0x531670 .. is main.c. The frame loop 0x5309a0 is "MASTER()" (the callback 0x5315c0 sets the time of day from the game speed 0x6f3608; the screens pass their own function as the registration key: the stack 0x75f780 holds the keys, 0x6f22a4 the base depth, 0x6f22a0 the pending pop count).
- Ghidra merged functions: 0x52cef0 (+0x52d010, a 0x1d0-byte callback of 0x438e60 drawing the gate names), 0x5309a0 (+0x5315c0), 0x52c420 / 0x52c4a0 (a 0x40-byte function the backlog counted into 0x52c4e0).
- The sv_ tag strings: "sv_NetworkSync" 0x6244a8 (0x52fc10), "sv_HandleObjects(): Could not find QuickJump.ContactName: %s" 0x6242c4 (0x52ae70, the quick jump to a contact), "MASTER(): Function already registrated!" 0x624534.
- 0x52b500 builds script objects for the characters of a group: the script path "x:\engine\gfx\scripts\locations\<class>\<name><n>_<class>.esc" (n = first free number among the objects already using the name, found through 0x5de6b0 = strstr).

## match-base 2026-10-25 (base-0 worker, shard 0/6)

- 0x447190 call sites renamed ai_Unk447190 -> d3if_Delete (two local K&R lines dropped, gv.h declares it); byte unchanged.

## match-base 2026-10-25 (base-4 worker)
- sv_Unk522100 -> sv_SetPanelEnabled (comment corrected: n != 0 enables, 0 disables, 0x475aa0 sets disabled = (on == 0)), sv_Unk52e600 -> sv_DisableVisibleWidgets, sv_Unk524010 -> sv_RedrawScreen; ai_Unk6f22bc -> ai_FrameWait, ai_Unk6248a4/a8/ac/b0 -> sv_ViewRectLeft/Top/Right/Bottom.

## match-base 2026-10-25 (base-5 worker, shard 5/6)

- Reference-only renames (every use updated, build/link/ledger clean): sv_Unk521c10 -> sv_SceneLoadProgress
  (0x521c10, the scene-load progress callback), sv_Unk5315c0 -> sv_UpdateClock (0x5315c0, the wait-loop clock
  callback), ai_Unk75e8c0 -> sv_PanelFlag (0x75e8c0, the panel flag).

## match-base 2026-10-26 (base-0 worker, shard 0/10)

Check count 2573 -> 2574/4664; grade regressions 0; sv.c 39855 -> 39657 differing bytes; byte + readable
1324 -> 1325, readable 1554 -> 1555.
- **sv_CheckStock** (0x5210a0, was sv_Unk5210a0) byte + readable: the three `txt_Format` calls of the source
  tail-merge into one call in the original, so the text id is a constant pushed per branch (0x76 for the
  prototype 0x142, 0x77 for 0x115, else 0x75); the `id`-in-a-variable spelling produced the `mov eax,0x76`
  form. `sv_MessageBox` got its prototype, which cleared the Q7 of sv.c's own message-box callers.
- **sv_MessageBox** (0x521150) -> match-deep (@deep in its address comment): 212 -> 133 code hunks. The
  statement set is complete (form call per branch, `timed`/`quick`/`n` locals, `bar = -1` before
  `if (timed) bar = d2_WinOpenSlider(..)`, the `if (sv_RunFrame(..)) { .. do { } while (sv_RunFrame(..)); }`
  do-while, the widget loop as the twin 0x4e5470's do-while, `unsigned t0` for the unsigned timeout compare);
  what is left is the allocation cascade (widget-table base ebx vs edx -> flags param slot live vs dead ->
  form/bar slots, bar in esi vs a slot). Readability: Q2 clears except ai_Unkf8aad0, Q4 7 raw accesses need
  D2WIN / D2OBJ views (d2.h cannot be included in sv.c: vfs_Root / vfs_Tfopen clash with vfs.h), Q7
  d2_SetObjValue is K&R (2..5 argument callers).
- **sv_MessageBoxGreen** (0x521410) -> match-deep: 73 code hunks, the residue is the constant 1 held in ebx
  and `bar = -1` in its own frame slot with form in ebp (the hoist tried alone made it 267, reverted);
  Q4 6 raw accesses, Q7 d2_SetObjValue. Q2 clear.
- Readability work that also serves the rest of the unit: the d2 message-box / radio / slider API is named
  after d2.h at every call site (d2_OpenForm, d2_SetForm, d2_RadioAdd, d2_RadioSelect, d2_RaiseViewWindows,
  d2_WinOpenSlider, d2_RadioSel, d2_ClickedId), d2_LastWindowPtr (0x6ed980) and d2_Objs (0xf8f080) are
  declared char* with an @original tag like rt.h (d2.h's D2WIN* / D2OBJ*), kb_Last (0x1468410, di_input.c's
  unsigned char), d2_SetObjValue (0x475cb0), sv_FitForm (0x52ea30), sv_MessageBoxOpen (0x6f2350),
  sv_MessageBoxGreenOpen (0x6f2351). lint went from X 13 conflicts / 17 pairs to X 0 / 0.
- TRIED AND REVERTED: `ai_Unk6333cc -> d3_Time` (volatile unsigned long) broke sv_Unk5263a0 (1365 -> 1458
  differing bytes: the clock's type decides its code) - reverted to the int alias, the d3_Time unification is
  a shared-header decision for match-deep.
- NOT attempted (leads): 0xf8aad0 has no name with evidence (gfx.h's d2_Unk8aad0 is a placeholder; the role is
  the window under the pointer copied from d2_PointerWin in d2_MouseUpdate).

## match-base 2026-10-25 (base-0 worker, shard 0/6, session 6)

- Shard 0/6: finished amt.c's base queue (see notes/units/amt.md) and took sv.c's next item: sv_MessageBoxYesNo
  (0x5216a0) -> match-deep, reason in its @deep tag; readable fails only Q4 / Q7, the documented d2 view debt of
  the message-box family.
- The form-name chain is one d2_OpenForm call per arm (`if ((flags & 2) != 0) form = d2_OpenForm(0, 0,
  "misc\Messagebox_Autosize"); else if 0x10 ... else if 0x20 ... else ...`): the original pushes the string in
  each arm and shares the `push 0 / push 0 / call` tail. 246 -> 114 code hunks.
- The clock 0x6333cc is used as `d3_Time` in sv.c now (d3.h is included; `extern int ai_Unk6333cc` dropped from
  sv.c), whose volatile re-reads match the original. ai_Unk6f2352 is sv_MessageBoxYesNoOpen (the non-reentrancy
  flag, twin of sv_MessageBoxOpen 0x6f2350 / sv_MessageBoxGreenOpen 0x6f2351).
- Left in the tag: the shared 0x40 constant (ours hoists it into edi and reuses edi for 1; the original uses
  immediates and never touches edi), the bar / t0 slot pair (swapped: frame+0x10 / +0xc) and the push edi frame
  shift; bar in esi vs a slot. The other units still use the ai_Unk6333cc alias - not renamed (not this shard's
  units).
- sv_OpenMessageBox (was sv_Unk521900, 0x521900) byte + readable: the form name is one d2_OpenForm call per arm
  (`if ((flags & 0x10) != 0) form = d2_OpenForm(0, 0, "misc\Messagebox_BIG"); else if (0x20) ... else ...`; the
  original pushes the string in each arm and tail-merges the `push 0 / push 0 / call`), which was the whole diff.
- The message-box form strings were written `"misc\Messagebox"` (an invalid `\M` escape): MSVC drops the
  backslash, so the string data was "miscMessagebox" while the original's is "misc\Messagebox" - the functions
  still matched (the relocation is masked) but `link --nonmatching` reported the three `??_C@...` string symbols
  as data conflicts. All 12 code literals in sv.c are escaped now (`\`); the link is clean. Check the other
  units for the same invalid escape (a data conflict in build/link/report.txt).
- sv_Unk521960 (0x521960, the info-panel object selection) is in progress in the base queue: Q3/Q5/Q6/Q7 pass
  (init_SceneDirty for 0xfd7f75, init_LoadObjectScene for 0x50d7f0, prototypes for gm_IsHomeType / ai_Unk5024f0 /
  ai_Unk511dc0), the panel data symbols are named sv_PanelObject (0x75e8c4) / sv_PanelRecord (0x75e8c8) /
  sv_PanelText (0x75e8d4), and the comment says what it does. Left for the next session: Q2 on ai_Unk6f229c /
  ai_Unk6f22a4 (read only in sv.c, as a screen-stack index - `SV_I(0x75f780 + SV_I(0x6f229c) * 8)` - and as the
  frame difference; nothing in the decompiled code writes them) and on ai_Unk6f2310 (the record shown, "its room"
  in f3snd.h) / ai_Unk5024f0 / ai_Unk591520, plus Q4 on the building / character records. 256 code hunks, far.
- Renames (project-wide, every use): unk_Unk511dc0 + ai_Unk511dc0 -> init_ObjectSceneUniverse (0x511dc0, the
  scene universe of an object prototype, the sibling of init_BuildingSceneUniverse; 19 uses in cm.c / f3init.c /
  f3gm.c / he.c / ls.c / sv.c), ai_Unkfd7f75 -> init_SceneDirty (0xfd7f75, the dirty flag byte of each scene
  universe; cm.c / f3init.c / sv.c).

## match-base 2026-10-26 (base-3 worker, shard 3/6)

sv.c includes d3_light.h; the ai_Unk453280 K&R prototype is gone and the call uses d3_RelightObject. No byte change.

## match-base 2026-10-26 (base-0 worker, shard 0/6, session 8)

Check count 2610/4664 (unchanged: no byte gained, none lost); grade regressions 0; sv.c 39694 -> 39588 differing
bytes (unitdiff --against, no WORSE); readable 1757 -> 1776, byte + readable 1479 -> 1494 overall.
- **sv_SelectPanelObject** (0x521960, was sv_Unk521960) -> match-deep (@deep in its comment): 256 -> 202 code
  hunks, readable. The whole first hunk was a local: the original does NOT copy sv_PanelRecord into a `rec`
  local - it reloads the global at every use (four `mov eax,ds:sv_PanelRecord` sites). `cur = 0; if (b) cur = b;
  else if (f) cur = f;` reproduces the original's shared conditional assign (cmp eax,ebp / jne /
  mov eax,[ai_FallbackBuilding] / cmp / je / mov edi,eax). `*sv_PanelRecord` is read as unsigned short (the
  original's `xor edx,edx / mov dx` into init_LoadObjectScene). Residue is one allocation cascade over four
  saved registers (o=esi, cur=edi, 0=ebp, 1=ebx in the original, ours folds cur into ebp and 1 into immediates).
- **sv_DrawPanelPointer** (0x521ba0) -> match-deep: 65 -> 19 code hunks, readable. The width was half the diff:
  d2_PointerLook (0x6ed9d0) is a *short* - the original does `mov ax,ds:X / cmp ax,1`; and the shape is one
  if/else-if chain in the original's test order 1 / 3 / 6 / 8 / 7. Left: the original keeps x = 0 (edx) and y = 0
  (ecx) live from entry and pushes them, VC6 folds ours to immediate pushes (constant fold, a per-function
  peephole).
- **sv_RemoveGroupMember** (0x524160) -> match-deep: 11 code hunks, readable (SVGROUP.member, SVCHARREC.netId).
  Left: the original materialises the character base (`lea ecx,[eax*4+0x770b80]`) and never loads through it,
  and computes p = g+0x8c after `i = 0`.
- byte + readable this session: sv_UpdateFramesPerTick (0x520ec0, was sv_Unk520ec0), sv_InitTick (0x520ee0),
  sv_UnregisterTick (0x520f10), sv_ResetScreenClip (0x520f20), sv_CheckMoney (0x520f90), sv_CheckCharacterFlag
  (0x520fe0, was sv_Unk520fe0), sv_CheckGoodAmount (0x521040, was sv_Unk521040), sv_SetResearchClass (0x522ec0),
  sv_UsedGroupSlots (0x5241b0), sv_ClosePlayerBar (0x525440), sv_ClockSeason (0x525480), sv_ReleaseBubble
  (0x52c4a0), sv_ClearNamePlates (0x52e170), sv_ReleaseNamePlates (0x52e190), sv_SetStatusText (0x52e330).
- Records / tables as views in sv.c (ai.h cannot be included: its ai_Time / ai_Chars / ai_BTypes / ai_Blds
  declarations clash with gv.h's char views): SVOBJREC (AIBLD: kind, sceneRec +0x29, objList +0x5d, ownerId
  +0x65), SVCHARREC (AICHAR: netId, inventory +0x178, money +0x194, flag1b1), SVGROUP (member[8] +0x8c),
  SVPLATE (200 B, obj +0), SVPLATEOBJ (+0x224 plate). Watch the padding: a view that forgets it makes the
  byte check fail (and one that mis-sizes an array writes the wrong slot: SVBUBBLE with 12 instead of 0x10c
  bytes).
- Frame / panel globals named (all cross-unit evidence): sv_FrameDepth (0x6f229c), sv_FrameBase (0x6f22a4),
  sv_PanelShown (0x6f2310), sv_PanelForm (0x623860), sv_PanelPic (0x623854), sv_PlayerBarForm (0x62385c),
  sv_SelectionFlags (0x75e920, the mirrored selection flags read as bytes at sv.c 4631), sv_ResearchClass
  (0x6f233c), sv_TickFrames (0x623790), sv_BubbleObjects (0x761fc8, the object field of the 0x40 x 0x10c
  bubble records - named at +8, NOT at the table base 0x761fc0: a symbol referenced only at base+8 makes
  `uv run link` report "matched sites disagree"), sv_NamePlates (0x760640), sv_StatusText (0x761f40), sv_StatusTime
  (0x6237c4). d2_PointerLook is a short.
- Its own prototypes: sv_Bubbles was dropped - use sv_BubbleObjects (above).
- Open: sv_SceneLoadProgress (0x521c10) is `next`; its readability is blocked on two floats with no evidenced
  role anywhere (0xfe73c8, 0xfe73a8: only sv.c reads them, the bar's y = f * 0.5 + f - 205.0; no writer in the
  sources, no Ghidra reference), the rest (Q3/Q5/Q7, the d2 call names) is mechanical. sv_Unk521e70
  (behaviour, draft) needs Q2 on the widget / viewport globals (0x623840 / 0x623850 / 0x6238a4, 0xfb9060..,
  0x6f0454 which shp.h calls d2_TransparentColour) and on sv_Unk522f10.

## play 2026-10-29 (the city view was blue: the camera limit table was missing)

- The tutorial's main screen drew the fog colour where the 3D city should be. `sv_PlaceCamera` computed the
  pitch as ~8e20: `ai_ApplyGfxOptions` (unk_5d33b0.c) copies the camera_limits pair out of the table at
  0x6237d0, which no C definition owned, so `(float *)&sv_CameraHeightNear - 12` read whatever the linker had
  put before the globals (a 1-byte 'r' and 0x622e435f) into sv_CameraHeightNear .. sv_CameraAngleFar.
  `d3_MatFromAngles` then built a garbage camera matrix, every terrain tile was frustum-culled
  (`d3fl_UpdateTiles` -> clip 0x40) and only the D3D clear / fog colour was left (52 object triangles,
  d3_MinZ / d3_MaxZ garbage from the uninitialised tile z ranges).
- `sv_CameraLimits[12]` is defined in sv.c directly before `sv_CameraHeightNear` (0x6237d0, the original's 48
  bytes: 1200 / -0.9, 650 / -0.471238911, 190 / -0.104719758, then the far pairs 1200 / -0.9,
  1800 / -1.08210409, 2400 / -1.04719758 - every literal compiles to the original's bytes) and
  `ai_ApplyGfxOptions` takes `lim = sv_CameraLimits` (was `&sv_CameraHeightNear - 12`, 10 fewer differing
  instructions). The item is 0x6237d0..0x6237ff; the next original item is sv_CameraHeightNear at 0x623800.

## play 2026-10-29/30 (`rot[12]` was a 16-byte stack overflow in the camera-focus family)

- `sv_Unk527840` (0x527840), `sv_Unk527bb0` (0x527bb0) and `sv_Unk527e90` (0x527e90) declared
  `float rot[12]`, but `d3_MatTranspose(camera + 0x18c, rot)` writes a `D3MATRIX` (0x40 bytes). The 16-byte
  overflow overwrote the saved frame; the first call of the new `ai_GameFrame` panel path returned to address
  0 (`read of 0`, eip 0). `rot[16]` now; the three frames match the original better (debt -12 / -6 / -5 asm).
- `sv_Unk5229b0` (0x5229b0) blitted `ai_Unk6f22c8` with `d2_BlitPartMode(surf)`, one argument, while the
  original (0x522b0c..0x522b3d) passes nine: `(0, 0, surf[4], surf[8], surf, d2_ScreenW - 0x6e, 0x5b,
  d2_ScreenSurf, 1)`. The garbage parameters made `src` 2 and `d2_BlitPartMode` read [2 + 0x28] as soon as the
  panel was drawn with a selection. Now the player's background strip blits (208 -> 240 bytes, +24 debt).

## play 2026-10-29 (the tutorial camera flew to 1e9: two int/float type errors)

The reported "Weiter in 'Einführung ins Tutorial', then the 3D scene turns blue when 'Kapitel 1' opens"
was the camera, not the scene: the fog/clear colour was all that was left after every tile was culled.

- **The stored view was passed as int.** `sv_Unk528140` calls `d3t_PlaceObject(camera, ai_Unk75fb80, ...)`,
  but ai_Unk75fb80 / 84 / 88 / 90 / 94 / 98 are `int` cells holding the **bit patterns** of the float
  position / direction. The prototype takes floats, so the compiler converted the ints (`0xc3ccd9c2`
  = -1028326974) to floats instead of reinterpreting them: the camera target became about -1.0e9 and
  the path animation moved the camera there (d3anim_StepObject wrote -1.0e9, then -1.5e9, ... per frame).
  The call now passes `*(float *)&ai_Unk75fb80` etc.; `sv_Unk528140` is byte-identical to the original
  (96 -> 64 differing bytes, then matched), which confirms the float reinterpretation. Trigger: the
  tutorial's panel step focuses the market (sv_Unk527840), then sv_CameraFollow sees the stored view and
  calls sv_Unk528140; the market panel stayed correct while the world was culled blue.
- **`ai_CameraStep` was read as float.** 0x623814 is `int ai_CameraStep = 50` (the original does
  `mov eax,ds:0x623814`, `/3/` = `lea eax,[eax+eax*2]`, or `fild` where it wants a float). sv.c declared
  `float sv_CameraStep`, so the int bits (0x00000032) read as 4.2e-44 and the camera step / duration was 0.
  sv.c uses the int symbol now (`ai_CameraStep`) and converts with a cast at the two float sites
  (sv_Unk527840: `(int)(float)(ai_CameraStep * 3)`, sv_Unk527e90's distance test).

Verified with `uv run play --start tutorial`: the dialog's Weiter is auto-answered as before, the camera
reaches the market and the 3D city renders (screenshots), 50 s run exits 0 without a crash.

## play 2026-10-29 (the right-drag crash: the day table wrote into the drag view id)

Holding the right mouse button and moving (the camera step) crashed in `d2_CloseView(view)` with a view id
`0x06ffffff`. A hardware watch on `ai_Unk62a8cc` named the writer: `sv_Unk525a00` (the day phase fill), whose
first `p[-1] = h` writes the hour one byte **before** `ai_Unk623ba9`. In the original that byte is 0x623ba8,
the tail of the preceding table; in the full link the object order put `ai_Unk62a8cc` (the drag window id)
immediately before `ai_Unk623ba9`, so the hour 6 landed in its high byte (0x06ffffff) and the next drag close
computed `d2_Views + 0x06ffffff * 0x2ac`.

`sv_Unk623ba8` (ALIGN1 char, the original's 0x04) is defined directly before the `ai_Unk623ba9` block now and
absorbs the write: `ai_Unk62a8cc` stays -1 and the view id is intact. The right-drag was re-run twice
(camera rotated, hint still up, game alive). The 6 s / EIP 0 startup crash seen twice while bisecting is
separate and intermittent (stack shows evt_ImportCommand during loading), not this path.

## play 2026-10-29 (the border-scroll crash: `d3_MatTranspose` into `right[3]`)

Moving the mouse to the right screen border crashed at EIP 0 with the stack full of camera matrix floats and
a stale `evt_ImportCommand` chain. The cause was another too-small matrix local: `sv_Unk5263a0` (the border
scroll) has `float right[3]`, but `d3_MatTranspose(camera + 0x18c, right)` writes a `D3MATRIX` (0x40 bytes).
The 52-byte overflow ran over the saved frame; the return address / EIP became 0. The same mistake was in
`sv_Unk526cf0` (the right-drag camera control) and `ai_Unk57f0e0` (the cannon camera, `t[4]`); all three are
16 floats (`D3MATRIX`) now. Verified: border scroll and right-drag in one run both keep the game alive and
the camera moves (screenshot); a 75 s run exits 0.

## play 2026-10-29 (the camera panned up/down but not left/right)

`sv_Unk5263a0` and `sv_Unk526cf0` computed the screen movement (`v[0]` = x, `v[1]` = y) with the **fallback**
branch only: `d3_VecAngle2` + `sin`/`cos`. The original uses that branch only when the camera's forward
projection is shorter than 0.1 (looking straight down); the normal branch is the matrix multiply at
0x526acf / 0x527388:

    fwd[0] = right[0] * v[0] + right[4] * v[1];
    fwd[2] = right[2] * v[0] + right[6] * v[1];

With `sin`/`cos` the horizontal component landed on the wrong axis, so only up/down moved. The normal branch
is written now (the sin/cos stays as the `else`); `right[8]` / `right[10]` are still the forward used for the
0.1 length test. `sv_Unk5263a0` / `sv_Unk526cf0` stay draft (debt +6 asm / +46 B and +6 / +55 B).
