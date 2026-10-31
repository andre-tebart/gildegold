# main.c (new, layout 2026-10-23 session 5)

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** first; below it the findings of each session, one section per author.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): program start / stop, main menu, network dialogs, character creation, round loop
- Boundaries: open (layout 2026-10-23, 5th + 6th session): new unit, split from sv.c: tag "main_" ("main_OpenEngine(): could not open txt-engine!" 0x624a8c, "main_Menue:Could not load 3D-scene..." 0x624c9c, "main_PreloadSound(): .." 0x6252ac, "main_RundenBeginn(): init round no %i" 0x625388). Evidence: .data: sv's last string is "MASTER(): .." 0x624534, the next block starts with "Die Gilde" 0x62493c (window class, mutex), the version string 0x624948 and the month table 0x62496c; the code is the process start / stop (0x531670, 0x5316d0, 0x531ba0), the window / engine start 0x531c50 / 0x532160 / 0x5324f0, the network lobby dialogs 0x5328f0 .. 0x5337f0, the main menu 0x533850, the character creation 0x534040 .. 0x537040, the loading screen 0x537980 .. 0x537be0, the game start / end 0x537de0 / 0x537f40, the round loop 0x5384e0 .. 0x53bb20 (day change 0x5384e0, round intro 0x539870, player slot update 0x53a250 / 0x53a450 / 0x53a520, the start couple 0x53b840, the round start 0x53b2e0, the game loop 0x53bb20), the intro / outro movies 0x53c9b0 / 0x53cb40 and the program entry (WinMain, game.INI) 0x53ccf0. End: 0x53d426 (the end of 0x53ccf0); the next unit he.c starts at 0x53d430 (tag "he_", first string 0x626680, its globals 0x625718 / 0x62571c follow the INI strings 0x6256fc). Open: the start (0x52fb80 the message pump is in sv.c), 5 undrafted functions (pseudocode exported to build/ghidra/*.c; 0x532da0 needs the disassembly for its call arguments), many ROUGH drafts
- Range then: 0x531670..0x53d426 (every function drafted: the gaps 0x532da0, 0x5359f0, 0x5361d0, 0x536980, 0x537230 and the Ghidra miss 0x537b60 filled layout 2026-10-23 session 10; still open: the drafts are not all in address order)
- Was: tag "main_"

## Open work
- **match-base 2026-10-31 (2nd session, 0x537980 .. 0x53a520)**:
  byte: main_LoadingScreen (byte + readable), main_EndGame 0x537f40, main_RundenBeginn 0x538140, main_UpdateFigures 0x53a450 (Q7 / Q2 left, below).
  Q7 left on those: main_EndGame calls snd_ClearQueue (f3snd.h clashes sb_PlaySample with gfx.h), hst_ResetPlayerTexts / ai_ClearMissions (hst.h /
  unk_4b39f0.h clash on gm_Unk503ee0, am.h too), d3sky_DeleteLayer (adding it to d3_sky.h drops d3_CloneObject byte -> shape: declaration-set
  tie-break); main_RundenBeginn calls gm_TitleJob (a prototype with its unsigned char result changes the bytes: the original called it without one);
  main_UpdateFigures: AICHAR.unk1b1 has no evidenced name (ai.c note), the float at +0x1c (0x1c in AICHAR.pad009) is a raw access until named.
  Escalated (deep): 0x5384e0 main_Unk5384e0 (4.1 KB round / day change: top half read, the season in ebx vs a spill and -1 CSE shift the frame by 4;
  the rest after +0x5c7 not yet reconciled), 0x539620 main_ReplaceFigure (VC6 cross-jumps the two identical 'ai_GameOver = 1; return' blocks the
  original keeps apart), 0x539870 main_RoundReport (row loops rebuilt, slot order of the five running sums / row list at +0x8c), 0x53a250
  main_UpdateFigure (kind 7 is retested at 0x53a3fc in the original: jump threading; i on the stack, base in edi, &ai_Chars[i] in esi).
  Findings: the clock copy in main is AHMCLOCK (3 ints + a short: 'the original copies it so'), the player id is recomputed as (ai_PlayerId & 0xffff)
  at each use (MAIN_PID), 0x6f2598 is main_Round, 0x6f25c4 main_UdpOpen, 0x6f2408 .. 0x6f2414 the sample banks of thunder / books / rooms / transport
  (snd_ThunderBank etc., main.h), d3_SkyLayers are 6 slots up to sv_FrameMask 0x760638 (sv.h view macro), he_AddApEvent / he_SumApEvents the AP event
  list at 0x758ca0 (0x50 bytes each); gm_AddHealth takes the id as a word at main_UpdateFigures (cast macro main_AddHealthS).
- **match-base 2026-10-31 left for the deep queue** (the character creation dialogs, bodies read against the original; calls,
  arguments and constants match):
  0x534b30 main_ChooseModel (715 ins, most of it the 34-string table init in another schedule, verified identical by simulating both
  sides' stores): VC6 factors `d2_ScreenScale * 150.0f + d2_ScreenScale * 330.0f` into one multiply, the original keeps two products
  (tried literals of every type, reversed operands, casts, static const x0 / y0; only a second symbol for the scale or `+ 0.0f` keeps
  them, neither has evidence); allocation and one frame slot (0x4a0 vs 0x49c).
  0x5359f0 main_ChooseFamily (124): allocation only (ebp saved in the prologue holding f, one more slot: frame 0x16c vs 0x168).
  0x5361d0 main_ChoosePlayer (184): allocation (-1 in ebp, 1 in ecx) and a dead load of flag in stage 5 (`mov eax,[flag]` when
  ai_MissionMode != -1, then ret = 1) that VC6 removes for every spelling tried.
  0x536980 main_ChooseHistory (112): gm_Unk503ee0 returns its 6-byte record by value in the original (hidden pointer; the caller
  copies it to a temp, then 5 bytes into init_PlayerProfile.talents): needs f3gm's definition, am.h and unk_575860's prototype
  changed together; the rest allocation (again in ebx, ret in ebp).
  0x537040 main_ChooseOpponents (24): allocation of n / y around the first loop.
  0x537230 main_ChooseCity (32, size equal): cityobj / buf4 slots swapped (declaration order has no effect), the tower position stores
  scheduled around the d3_CreateObject pushes.
- **Q7 open** for the creation dialogs: ch_* (ch.h does not include into main), gm_TitleGroup has no prototype in f3gm.h, d2.h's
  d2_SetObjValue() has no parameter list, ai_ListFiles / ai_Unk59a2c0 / ai_Unk4e2bf0 / ai_Unk4e2c70 / ai_Unk4b1f90 / ai_ChooseMission
  / ai_Unk58b880 undeclared in main.
- **match-base 2026-10-30 (fifth session) left for the deep queue** (bodies read against the original; calls and constants match):
  0x532a70 main_JoinNetworkGame (36 ins, readable: ours loads ls_SavePath.cur early into ax and stores net.x first, so ai_GameFlags is or-ed
  in eax (`or al,0x50`) where the original uses ecx; ours hoists the hdr.char164 load above the first rep movsd; char0c / char54 a pure
  eax->ecx->edx rotation), 0x532da0 main_SearchNetworkGame (347 ins, readable: frame 0x988 vs 0x990; the original keeps list in a slot,
  the ent row pointer in [esp+0x18], the name copy as src + (name - &recs[i].name) stepped by -0x80, ours in registers / two pointers),
  0x533850 main_Menue (265 ins: the original keeps 0 in ebx for the whole function and pushes literal 1s, ours keeps 1 in edi, so the three
  msx_Play cases are not cross-jumped; the original saves edi late (push at 0x53393f) and caches GetPrivateProfileStringA in esi, putting
  f in [ebp-0x3c]; frame 0x50c vs 0x508).
- **Q7 open** (header conflicts, not settled): main_Menue's hmt_Reset / ai_TutorialWindow / ai_SetupTutorialPlayer (hmt.h and
  unk_575860.h pull declarations that make main's calls of cm_SendExSysMessage / cm_SendCreateCharacter / gm_Unk503ee0 assign void
  results: 8 `other` conflicts in `dcp headers conflicts main hmt.h`); main_GiveStartBuildings' cm_BeginSendGroup / cm_EndSendGroup /
  cm_SendGroupState (cm.h: sb_PlaySample clash + the same 8). Settling them = correcting those callers' result use first.
- main_ChooseTalents is shape: two scheduling pairs (the 0xb-dword struct copy before `r = 0` / `mov ebx,1` in ours, after in the
  original; `again = 1` vs `i = 0` at the arrow loop). Moving `r = 0` after the memset makes it worse (r stored from eax).
- data: 0x9367a0 is CMJOIN (cm_types.h), viewed as cm_JoinChar in main's head; its definition is still inside cm.c's
  ai_Unk9364e0[752] (8 records of 0x57 = 0x2b8, then 8 bytes, then this 0x30 block).
- **headers (2026-10-30, typing pass 2)**: head.c includes txt_views.h, main.h, he.h, sfx.h, f3gm.h, mov.h, txt.h, di_input.h, evt.h, d3.h, d3_light.h, d3_engine.h, data_views.h, d2.h, unk_58bbd0.h, ecmd.h, ts_texture.h, unk_5d33b0.h, ai.h, gfx.h, dd_vesa.h, d3_interface.h, d3_int_io.h, err.h, tr.h, vfs.h, tb.h, msx.h, evtc.h, sndc.h, net.h, ls.h, cm_types.h, f3init_types.h, ch_types.h, f3snd_types.h, time_view.h, f3init.h, sv.h, mem.h, d3_d3d.h, io_text.h, d3_snd.h, d3_tools.h, ahm.h, gv.h; keeps 4 cast / view macros with @original byte evidence (the address and the matched caller are in each comment): d2_PlaceMainWindowF, d2_MoveObjS, util_RandomInt, cm_SendExSellObjectI; views / casts without byte evidence of their own (weak definitions or drafts' spellings, notes/headers.md debt item 1): d2_SetObjValue4, ai_GoodsT, ai_BldsT, ai_BTypesT, ai_CharsT, ai_TimeT.
- **headers (2026-10-30)**: retried with `dcp headers conflicts main ..` (typing pass 2): mem.h, d3_d3d.h, io_text.h, d3_snd.h, d3_tools.h, ahm.h, gv.h are included now, and ls.h, f3init.h, sv.h (new headers). Still unsettled, with what is in the way: ch.h (2 header errors in main's set: D3ANIMSLOT without its struct tag, PATHOUT unknown), chc.h (needs ch.h), hmt.h / amt.h / hst.h (main's drafts use the results of void functions: gm_Unk503ee0 at 0x536980 / 0x53ccf0, cm_SendExSysMessage at 0x5384e0, cm_SendCreateCharacter at 0x53b840 - meaning work for those drafts), cm.h (a clash and 8 such calls), sblib.h (sb_PlaySample: sblib.h and gfx.h disagree), unk_575860.h (ai_ClearTurnTable has no prototype there).
- **head typing pass (2026-10-30)**: head.c now includes d2.h, d3_engine.h, err.h, tr.h, vfs.h, tb.h, msx.h, evtc.h, sndc.h, net.h, gfx.h, dd_vesa.h, d3_interface.h, d3_int_io.h, ai.h, unk_5d33b0.h, unk_58bbd0.h, ecmd.h, ts_texture.h; the views D2VIEWV / D2WINV / D2SURF / D2FADE / VIDCFG / MAINCLOCK are gone. Left:
  - head: include/ch.h, ch_Open / ch_Close / ch_SndLib / ch_Motions / d3t_CamMove unsettled (ch.h redefines D3OBJBITS and d3anim_MapBones against d3_engine.h); include/chc.h (chc_Register) needs ch.h.
  - head: include/hmt.h, hmt_CreateBinaryFile / hmt_Init / hmt_Exit unsettled (hmt.h pulls am.h, whose ai_Time / ai_Chars / ai_BTypes views conflict with ai.h); the same for include/ahm.h (ahm_LoadPrototypes / FreePrototypes: AHMCLOCK ai_Time, char ai_Chars[][0x21c]), include/amt.h (amt_CheckOffices), include/gv.h (gv_FreeEffectObjects, unk_ResetEffectSlots), include/cm.h (ai_InitStates, ai_BuildFloatTable, ai_ResetMessages) and include/hst.h (hst_ResetLists, via am.h; am.h's gm_Unk503ee0 is also a guess: it returns its 6-byte record in eax, struct-return style, main's callers use the result at 0x536cb5).
  - head: include/unk_575860.h, ai_ClearTurnTable unsettled (adding its prototype to unk_575860.h moves that unit's drafts ai_Unk579570 / ai_Unk57aeb0 by 5..36 ins, a declaration-set allocation effect).
  - No header yet (prototypes stay local): sblib (sb_Init / SetPath / Exit: sblib.h clashes, see above). sv, f3init, io_text, d3_snd, d3_tools have headers now and main includes them; ls.h holds NETHDR / LSSAVE (ls_SavePath).
  - Calls through casts with @original evidence: d2_MoveObjS (definition reads shorts; d2.h keeps int for d2_CreatePicObj), util_RandomInt (tr.c's int result: 0x53b8c9), d3io_NoopCallback with no argument, d2_SetObjValue4.
  - main_Unk539870 flips between 593 and 595 differing instructions with unrelated declaration changes (two slots of `int a[3]` swap); it ended at 593.
  - data: 0x936460 is cm's cm_NetGame (CMNETGAME, cm_types.h): the 0x80-byte network game record (+0x10 the NETHDR, +0x36 its player count, +0x7c the mode 0x3f / 0x7f); main's gap blocks of it are gone (baa83bb).
  - data: 0x75e1c0 main_NetHost / 0x75e360 main_NetSaveName, owner main or unk_5d33b0 (only main's functions use them, the neighbours ai_Opponents 0x75e160 / ai_OptionsFile 0x75e480 are defined in unk_5d33b0); typed in place in the gap block. main_NetSaveName's size (161, the gap's) is open: 0x75e401 is only 0x53b840's copy-loop base constant.
- **match-base 2026-10-30 (fourth session) left for the deep queue** (bodies complete, all readable unless noted):
  0x531780 main_WindowProc (instruction-identical, hunks=0; the grade still reads draft because the original's function extent holds the 37-byte switch tail: fill + 5-entry jump table + index table at 0x5319ce..0x5319f3, which our symbol does not cover - same artifact as ai_PokerGame),
  0x531a00 main_BuildVersionString (13 ins: the token copy loop; the original hoists `lea esi,[tok]` after the first strtok's `je` and copies with src in ecx / `add esi,0x10` before the loop, ours uses the `sub edx,eax` dst-src idiom; date is char[0x100]),
  0x531c50 main_OpenScreen (226 ins: exstyle spilled to [esp+0x10] and GetWindowLongA cached in ebp in the original; the resolution fallback chain is laid out inline in the original, ours (gotos) in another block order; the video record is VIDCFG = D3INITCFG with `triple` a dword),
  0x532160 main_OpenEngine (94 ins: one cause - the original is an ebp-frame function because its memset(init_PlayerProfile, 0, 0x8c) uses the generic alignment-fixup expansion with a [ebp-4] count; ours emits the plain rep stosd and an esp frame),
  0x5324f0 main_CreateWindow (181 ins: exs lives in ebx in the original, in a stack slot in ours - frame 0xcc vs 0xc8; declaration order has no effect),
  0x5327e0 main_PlayerCountDialog (one 14-ins hunk: the original pre-stores 2 into ai_NumPlayers (`mov byte [x],2`) before the `jb`, then reloads the byte as dword + `and eax,0xff` for the d2_SetObjValue argument).
  Raised: 0x532700 main_StartCityGame (byte + readable), 0x5328f0 main_HostNetworkGame (shape + readable; the `ai_ModeMask = 0x3f` store sits before the viewport argument setup in ours, after it in the original).
- Drafted layout 2026-10-23 session 10 (all ROUGH, nonmatching, sizes close): 0x532da0 (network game list, 1712 / 1744 B), 0x5359f0 (family choice, 2096 / 2016), 0x5361d0 (network player setup, 1936 / 1968), 0x536980 (history choice + start menu pages, 1056 / 1024), 0x537230 (city choice, 1824 / 1792) and the Ghidra miss 0x537b60 (loading progress callback, matched). No undrafted function is left in the range. Everything from 0x5384e0 to 0x53d426 has a draft now (layout 2026-10-23 session 6); the unit ends at 0x53d426 (he.c starts at 0x53d430, see notes/tus.md).
- Not in address order: the drafts are appended in the order they were written (0x531c50 .. 0x538140, then 0x5384e0 .. 0x53ccf0 in order); sort when the gaps are filled.
- Matched: 0x531670, 0x531690, 0x5316d0, 0x531750, 0x531b50, 0x531b80, 0x531ba0, 0x531c20, 0x5337f0, 0x537930, 0x537af0, 0x537be0, 0x537c70, 0x537db0, 0x53b830, 0x53c9b0. Near misses: 0x5344f0 (10 B: the `fstp` of the heading float is scheduled after the next load), 0x537de0 (49 B: constant loads, named externs would help), 0x531a00 (the original builds the month table on the stack), 0x53a520 (3504 of 3520 B) and 0x53bb20 (3680 of 3728 B) are close in size. 0x531780 (287/640: dispatch + surface pointer forms, see session 8).
- ROUGH: 0x531c50 (the video setup record of 24 ints and the window sizing), 0x532160 (the engine open), 0x5324f0 (the window class and window), 0x532a70 (the saved network game header fields: offsets in the header copy are read from the Ghidra locals), 0x533850 (the main menu), 0x534040, 0x534610 / 0x534b30 (the talent / model choice: floats lost), 0x537040, 0x537980, 0x537f40, 0x538140, 0x5384e0 (day change), 0x539620 / 0x539870, 0x53a250 / 0x53a450 / 0x53a520 (the __ftol operands are guesses), 0x53b840, 0x53b2e0, 0x53bb20 (the game loop: the pl struct of 0x4c4fa0 is approximated), 0x53cb40, 0x53ccf0.
- Types: all by absolute address (U_I / SV_WI2 / PL macros), the callees K&R implicit; the video record is `int st[24]` (see 0x531c50), the 0x75e6e0 / 0x936470 game record is 26 ints + a short (0x6a bytes), the speech / sound table 0x75df20 (0x80 sound bank handles). Callees now drafted in he.c (0x53d9f0, 0x53e400 ..) are still called as ai_Unk<addr> from main.c; main.c includes he.h.

## layout 2026-10-23 session 5
- Split from sv.c: see notes/tus.md (tag "main_": main_OpenEngine / main_Menue / main_PreloadSound / main_RundenBeginn).
- 0x531a00 builds the version string: the compile date literal "Oct  1 2003" is parsed with strtok and compared with a month table, the result is unused; the output is always "Europa 1400 - Gold Edition - 2.06" (0x624948); 0x532160 shows "$ADie Gilde - Gold Edition - Version 2.05$X" on the loading screen.
- Menu structure: 0x533850 builds 9 buttons through 0x5337f0 (row n at y = n * 0x2b + 10, width 300): new game 0x537930 (-> 0x537230 the player setup), tutorial, load 0x5d4080, network 0x5335b0 (host 0x5328f0 / load 0x532da0 / join 0x533470 -> 0x532a70 or 0x532da0 (1) / address 0x532c30), options 0x5d5fd0 / 0x5d6580 / 0x5d6950, credits 0x5d7ba0 (music "cd2\ZumGutenEnd.mp3"), quit.

## layout 2026-10-23 session 6
- Drafted 0x5384e0 .. 0x53ccf0 (the round loop): 0x53bb20 is the dialog callback of the process level (called with 0 from 0x53ccf0 until 0x6f25a0 is clear; also the callback of 0x5309a0 with mask 0x20007): start-up by the mode flags 0x6f23f8 (1 new game, 2 load game, 4 network, 8, 0x10 plug-in dll "Init", 0x40, 0x80), the loading of the game, the autosave (`<gfx path>\gamedata\saves\autosave.sav` copied to .bak, then 0x5190b0 "Gamedata\\Saves\\Autosave.SAV"), then per player the round: 0x5384e0 (day change), 0x53d9f0 (event bar), 0x539870 (round intro), 0x53b2e0 (round start).
- 0x53ccf0 is the program entry (WinMain-like): game.INI keys Network (Server, Host, Port), Gfx (TextureDivider, cur_res), Sound (msx, ambient, sfx, weather), General (Bildmodus FULLSCREEN / DIRECTWINDOW, GfxPath, GamePath, MoviePath, Language, show_intro, Stadt); command line options STADT=" BERUF=" IP=" PORT=" MO_MONEY FULLSCREEN DIRECTWINDOW; with 0x6f2434 (test start) the figure set-up with the names "Tobias" / "Severin" and the model "patrizier_MANN2".
- 0x53c9b0 (matched) plays the logo movies, 0x53cb40 the outro (`%soutro.mpg`).

## layout 2026-10-23 session 9
- 0x531780 (the window procedure, a Ghidra miss) drafted from the disassembly: `switch (msg)` with cases 2 / 0x10 -> PostQuitMessage(0); 0xf -> ValidateRect; 0x14 -> ValidateRect + if both surfaces set, the blit 0x479480; 0x1c (WM_ACTIVATEAPP): activate (the 0x41bdd0 / 0x4370d0 restore loops, 0x468e30, the 0x479020 / 0x46d3d0 / 0x4790e0 / 0x41e390 blit path, sfx_ReacquireAll 0x42bb00(hwnd, 0x7e7), 0x6333e0 = 0, di_AcquireMouse / di_AcquireKeyboard 0x410710 / 0x410740) / deactivate (0x6f2584 gate, SetWindowPos HWND_BOTTOM); 0x20a -> ValidateRect, return 1; 0x218 -> wparam == 0 returns 0x424d5144 ("DQMB"); 0x7e7 -> sfx_Reacquire 0x42bab0. VC6 lowers the sparse cases to a `sub 0x20a / sub 0xe / sub 0x5cf` chain reached by the `ja` of the 0x1c tree root: write them as real cases, not default handles. IAT: PostQuitMessage 0x5eb21c, ValidateRect 0x5eb220, SetWindowPos 0x5eb23c, DefWindowProcA 0x5eb24c.
- 0x531780 near-miss 287/640, first diff at +0xe: the surface pointers as p / q locals is the best form so far (287/640); re-reading U_I(0x6ed924) / U_I(0x6ed92c) at each use is worse (438/656); the original CSEs the loads into eax / edx per expression. Untried: the deactivate branch as `if (U_I(0x6f2584) != 1) { .. }` with one shared `return 0` (ours has an early `return 0`, the original shares the epilogue); the `mov esi, 1` schedule at 0x5318dc.
- 0x532da0 (the network game list dialog, 1744 B): pseudocode exported to build/ghidra/532da0.c but Ghidra lost ALL call arguments; the frame is huge (0x990+). It walks the listed games (`*piVar8 == 0x3f / 0x7f` record codes) and on join writes the host / port to game.INI (WritePrivateProfileStringA, 0x75e1c0 / 0x75e480) and returns 1 on success. Disassemble before drafting.

## layout 2026-10-23 session 10
- 0x537230 (second parameter = host flag, first unused) lists gamedata/cities/*.CTY (or .NET for a host): records of 0x210 bytes (path at +0x109, up to 64), header +5 name, +9 object name, +0x34 player limit; the default pick is the object of the 5th city (cityobj[4], an original quirk). Its stack arrays sit at fixed offsets of the 0x8984 frame (see the header comment).
- 0x5359f0 / 0x536d80 are called with a dummy argument 0 by 0x536980 (cdecl, ignored): 0x5359f0 takes `int unused`, 0x536d80 is called through a cast. 0x5361d0's first parameter is the second argument of 0x5309a0 (the mask), not the callback.
- 0x537b60 = the progress callback (kind, cur, total): kind 0 -> 10 + cur * 70 / (2 * total) percent, kind 1 -> 45 + ..; it hides inside the Ghidra span of 0x537af0 (found by the aligned-start scan: 16-aligned address after `ret` + nop padding with a prologue-like first byte).

## match-base 2026-10-24 (base-4 worker, shard 4/5)
- Includes sfx.h (+ mov.h after <objbase.h> / <strmif.h> / <control.h>) instead of the local K&R declarations of
  ai_Unk42b750 / 42bb00 / 42bab0 / 42ba70 (= sfx_Shutdown / ReacquireAll / Reacquire / ReleaseAll) and ai_Unk422d90
  (= mov_Exit); the call sites use the sfx_ / mov_ names now. No position changed (main.c 17/58 before and after).

## match-base 2026-10-25 (base-0 worker, shard 0/6)

- 0x447190 call sites renamed ai_Unk447190 -> d3if_Delete (declared with the full prototype); byte unchanged.

## match-base 2026-10-25 (base-4 worker)
- Reference-only renames (no code change; every use updated, build/link/ledger clean): ai_Unk4c3ae0 -> cm_GetSlotState (0x4c3ae0) and ai_Unk4767d0 / ai_Unk476860 -> d2_StartFade / d2_ReleaseFade (0x4767d0 / 0x476860, the old ai_ aliases folded into the d2 names). See notes/units/unk_5d33b0.md for the whole rename list.

## match-base 2026-10-26 (main.c in address order, not the base queue)

User direction for this session: walk the functions of main.c in address order instead of `uv run ledger grade next`.
Check count 2593/4664 before and after; `uv run ledger check --against` 0 regressions; lint E 0 / X 0. Four functions to
**byte + readable** (main.c readable 0 -> 4, byte + readable 17 -> 21; unitdiff 17/58 matched, 34372 differing
bytes - unchanged):
- 0x531670 -> main_DestroyWindow (main.h now owns `main_Window` 0x6f2588 and `main_Instance` 0x75e140).
- 0x531b50 -> main_CreateMutex, 0x531b80 -> main_SetScreenSaver (no callees; Q1 only).
- 0x531c20 -> main_ShutdownEngine: the K&R lines became prototypes with the canonical names tb_Exit / vfs_Exit /
  tr_shutdown / mem_Exit / err_Exit (declared locally as type debt; main.c still does not include those
  headers - the includes drag in the data declarations that main.c repeats as U_I addresses).
- main_Instance stays `void *` in main.h: ahm.h / am.h / hmt.h declare it `void *` and lint X flagged the
  HINSTANCE spelling (f3init.c's HINSTANCE file-scope line is not seen by lint). The use casts to HINSTANCE.
- main.c was not valid UTF-8: one 0xfc byte in "Meisterpr" 0xfc "fung" (0x53bb20). Written as a two-literal hex
  escape (identical bytes, the escape form "\xfcfung" would read as \xfcf); the file is ASCII now, so the Edit
  tool works on it.
Next in address order: 0x531690 (needs ai_Unk5d4c70 = the options writer, 0x6f258c named, and the d2_Exit /
d3if_SelectScene / d3t_FreeSnowMipmaps / d3_Shutdown names adopted - all canonical already), then 0x531750
(0x75e62c = dwPlatformId != 1, 0x6f25bc = the screen saver was off), 0x531780 (draft, 86 code hunks).

## match-base 2026-10-26 (2nd, main.c in address order)

Check 2593/4664 unchanged; `uv run ledger check --against` 0 regressions; lint E 0 / X 0; unitdiff 17/58 matched,
34372 -> 34360 differing bytes (41 functions, no WORSE). Four more to **byte + readable** (main.c readable
4 -> 8): 0x531690 -> main_ShutdownScreen, 0x5316d0 -> main_ShutdownModules, 0x531750 -> main_Exit,
0x531ba0 -> main_Init.
- Names of the shutdown chain, each from its own definition's role: ai_Unk5d4c70 -> ai_SaveOptions (the INI
  options writer, unk_5d33b0.c), unk_Unk4fdfb0 -> gm_FreeIconNames (f3gm.c), sv_Unk520f10 ->
  sv_UnregisterTick (sv.c), he_Unk575600 -> he_SaveMessageFilter (he.c), ai_Unk5a8920 -> ai_FreeSettings
  (unk_58bbd0.c), unk_Unk4ede70 -> gv_FreeEffectObjects (gv.c/gv.h), ai_Unk4c3560 -> ai_ResetMessages (cm.c/cm.h,
  "0x4c3480 without the start" = the message-record lists reset), main_Unk537db0 -> main_FreeSoundBanks,
  d3_Unk44ee40 -> d3io_NoopCallback (22 refs in 8 files: the empty default callback).
- main.c declarations are prototypes now with the canonical names (d2_Exit, d3if_SelectScene from cm.h,
  d3t_FreeSnowMipmaps, d3_Shutdown, sb_Exit, d3snd_Exit, d3sndw_Exit, msx_Exit, gm_Free, ahm_FreePrototypes,
  hmt_Exit, gv_FreeEffectObjects, ch_Close, err_Init, mem_Init, tr_startup, vfs_Init, tb_Init) - still
  declared locally, the includes are the remaining type debt (d2.h/gfx.h would also give d2_CutsceneActive).
- New main globals in main.h: main_ScreenOpen (0x6f258c: set in 0x532160, cleared in 0x531690, gates the
  WM_ACTIVATEAPP restore; the original stores a dword in 0x532160 and a byte here), main_PlatformNT (0x75e62c =
  OSVERSIONINFO.dwPlatformId != 1, set in 0x53ccf0) and main_ScreenSaverOff (0x6f25bc: has no writer in the
  sources yet, only the two exit reads).
- d3io_NoopCallback is declared K&R in main.c (`void f();`): the original calls the empty 0x44ee40 with no
  argument at 8 sites, so a prototype that takes `void *t` cannot be used there; 0x531ba0 only takes its
  address, so it passes Q7.
- err_Init is called as err_Init((int)main_Exit, d3io_NoopCallback, main_Window, 0x10): err.h's prototype names
  the first parameter `int started` (err_Started is only tested for truthiness), the original passes the exit
  handler's address there.
Next in address order: 0x531780 (draft, 86 code hunks; the note's leads: sparse-case lowering, the shared
return, the mov esi,1 schedule), then 0x531a00 (draft, 131 code hunks, the stack month table), 0x531c50.
- 0x531780 tried this session and reverted (WORSE): the deactivate branch as one `if (U_I(0x6f2584) != 1) { ... }`
  with the shared `return 0` epilogue - 287 -> 296 differing bytes, 54 -> 57 asm lines. The early `return 0`
  form stays. Still open: the surface pair in esi/edi (the original loads 0x6ed92c / 0x6ed924 once per branch and
  dereferences [esi+4] / [esi+8]; our best is the p/q locals, 287/640).

## match-base 2026-10-26 (base session 2)

- amt_CheckOffices() (0x4e9260, the round-start office check) was called as `ai_Unk4e9260()` with no
  declaration (an implicit int call, a link break once the symbol was renamed). main.c now declares
  `void amt_CheckOffices(void);` with the other foreign prototypes at the top of the file, as the ai_ owners
  there are; including amt.h instead conflicts with main.c's `r6 = cm_SendCreateCharacter(...)` uses (amt.h
  carries the void declaration of 0x4c4650). No byte change (17/58 matched, 34360 differing bytes before and
  after).

## match-base 2026-10-26 (base-3 worker, shard 3/6)

main.c includes <d3d.h>, d3.h and d3_light.h; its ai_Unk453280 call is d3_RelightObject now (cm.h lost the alias). No byte change (unitdiff main.c no WORSE).

## play 2026-10-28 (the tutorial: the chapter callback table, the start hook)

- **full_data_6291a0** held the five chapter callbacks (`ai_Unk578180`'s table) as the original's raw
  addresses (0x578570, 0x578e40, 0x579570, 0x57aa80, 0x57aeb0). The full link places those functions
  elsewhere, so the tutorial died at the first callback (0x578570 = the middle of another function). Split into
  `int full_data_6291a0[5] = { (int)&ai_Unk578570, (int)&ai_Unk578e40, (int)&ai_Unk579570,
  (int)&ai_Unk57aa80, (int)&ai_Unk57aeb0 }` (0x6291a0) and `GAPDATA full_data_6291b4[336]` (0x6291b4, the
  state-name pointer table and its strings; read by nothing yet, still the original's bytes).
- **REA_START waits one pass** (`rea_delay`) before taking the menu entry. The hook used to take it on the
  zero-th pass, before the menu's own `sv_RunFrame` had registered `main_Unk533850`; the pop request the entry
  sets was therefore not consumed (`sv_RunFrame` returns 0 without decrementing when the callback is not at the
  current depth) and leaked into the game loop, which exited after its first frame. A real click always comes
  after the registration.

## match-base 2026-10-30 (fifth session: the network menus, the main menu, the start buildings, the talents)
- Raised: 0x533470 main_ChooseNetworkGame and 0x5335b0 main_NetworkMenu byte + readable (the loop is if (sv_RunFrame) do .. while
  (sv_RunFrame); each button disables the view, calls, raises pop / r, enables again; VC6 merges the tails), 0x534040
  main_GiveStartBuildings byte (Q7 open), 0x534610 main_ChooseTalents shape + readable.
- The network game: 0x532a70 joins the saved game main_NetSaveName from its LSHDR (ls_types.h: the save header, 0xfc bytes, the fields
  from ls_WriteSaveHeader), NETHDR's tail is the save name + the party table, cm_NetGame.hdr is a struct copy; 0x532da0 lists the LAN
  hosts (UDPSERVER, its info.game is a NETHDR; byte 0x27 `joined` disables a full new game). The three menus: main_NetworkMenu (host /
  search / client) -> main_ChooseNetworkGame (saved game / search).
- main_Menue (the name its error text leaks): the full screen d3_SetViewport after the two scene loads was missing; the profile clear is
  D3_ZERO_AT (inline asm: the ebp frame); d2_LastWindow is read once for the 9 buttons. main_MenuSong / main_DevMenu / main_QuitProgram
  are typed in main's bss.
- The character creation: main_Family (MAINFAMILY, main.h) = the six profession groups (main_Unk5359f0) + the five talents; the parents'
  groups give +63 to their talent, an arrow moves 21 points, the pool is 0x7e, the bar is sv_ValueBar (value / 21 half tiles: an int,
  sv.h said float); the tools by the parents' groups go to init_PlayerProfile.tool1 / tool2.

## match-base 2026-10-31
- INIT_PLAYER is 0x8c bytes now: mission 0x4c (ai_ChooseMission's result), talents[5] 0x50 (copied as an int and a byte), model[0x33]
  0x55 (the model name the chooser writes), modelId 0x88 (unk_Unk4fb770(model) + 0x639); the gap blocks 0x75e5ec .. 0x75e628 are gone.
- main_MessageBox 0x624938 (int, -1): the message box the history choice opens over the creation and main_ChooseFamily closes.
- The creation dialogs share one shape the drafts missed: `if (sv_RunFrame(cb, mask)) do { .. } while (sv_RunFrame(cb, mask));` with the
  clean-up after the loop, and button tests the original writes as plain ifs that VC6 jump-threads (a goto in the draft is usually such a
  threaded `redraw = 1` / `if (ret == 0)`). main_CharacterIntro (0x536d80, takes an unused int) and main_ChooseDifficulty (0x536e80)
  are byte.
- main_ChooseModel: the button positions are 800-wide screen units times d2_ScreenScale (next 150 + 330 / 370, previous 330 / 370, ok
  70 + 330 / 45 + 370); the effect script gets (x, y + 0x14, z, 0x28, 0x5a, ""); the draft lost `shown = idx`.
- main_ChooseOpponents: the row height is 10.0f / n * 24.0f.
