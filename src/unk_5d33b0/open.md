# unk_5d33b0.c -- the screen / front-end module

## Unit
0x5d33b0 .. 0x5dd3e0 (the end of the game code; opened by layout 2026-10-24 session 13, drafted to its end by
session 14). The last function is 0x5dd100 (ret 0x5dd3d6, nop fill to 0x5dd3e0); 0x5dd3e0 .. 0x5de450
(4208 B) is a DATA table, not code (see the session-14 section). The state lives in the source header
tags.

- **Start (plausible)**: 0x5d33b0 is the first function of the screen-file family (the extension pick over
  vfs_FindDir, the "%s_%s" 0x62d7d8 entries). The previous unit (unk_58bbd0.c) ends with the trial / action
  callbacks 0x5d0f20 .. 0x5d3270 (records of the table 0x62c420, no literals of their own); the previous
  unit's last literal is "special_uni" 0x62d6bc and 0x5d33b0's first is 0x62d7d8 (+0x11c, data order D8).
  No leaked name for this module.
- The literals of 0x5d33b0 .. climb 0x62d7d8 ("%s_%s") .. 0x62de6c in function order (the graphics keys
  "texture_scale" 0x62dae8 .. the texts 0x62de60 .. of 0x5dbc10 / 0x5dbc20 / 0x5dbc30 / 0x5dbed0), one
  .data block; no boundary found inside the stretch so far.
- The functions are reached from main.c (0x531690 the frame, 0x531c50, 0x532160, 0x533850 the menu
  screens, 0x53bb20 the module init 0x5d7320), from the frame loop 0x5309a0 and through the .data
  callback slot 0x5eee34 (in .rdata).

## Functions
- 0x5d33b0 the screen file list (256 B): vfs_FindDir (param_1, vfs_Root 0x6333fc); the entries of the
  directory whose extension equals param_2 (case-insensitive) go into param_3 + 9 with a 0x210 stride:
  +0 the name, +0x100 the "%s_%s" form (0x62d7d8) with the extension cut. Returns the count. Drafted
  (61/256); the differences are the prologue / loop shape.

## Open work
- Every game-code function of the unit is drafted (0x5d33b0 .. 0x5dd100, 63 functions, contiguous; the
  ledger reports no undrafted byte inside the unit).
- **The 0x5dd3e0 .. 0x5de450 data tail**: not code (a registration table); the ledger cannot claim it
  (see the session-14 section). It is the only undrafted region of the game code.
- The older ROUGH drafts of the save / load and options windows are still stubs (0x5d3ab0, 0x5d4080,
  0x5d42b0, 0x5d45d0, 0x5d4920, 0x5d4c70, 0x5d5640, 0x5d5c40, 0x5d5f10, 0x5d5fd0, 0x5d6580, 0x5d6950)
  and the new big window bodies (0x5d85e0, 0x5d8c90, 0x5d90e0, 0x5d95a0, 0x5d9890, 0x5d9de0,
  0x5da030, 0x5db110, 0x5dd100) are ROUGH skeleton / stubs with their calls and constants recorded.
  0x5d6f40 matched first (the d2_Objs pointer form).

## layout 2026-10-24 (session 13)
Opened the unit and drafted 0x5d33b0. Nothing matched yet.

## layout 2026-10-24 (session 13, continued)
Drafted in address order after 0x5d33b0: 0x5d34b0 (the "menu\fileselector" window: the list of 0x5d33b0,
the 0x210-stride controls, the pump 0x5309a0, the click selection and the sprintf "%s\\%s%s" 0x62d7e0;
630/800), 0x5d3810 (the misc\Messagebox window family: Autosize / BIG / VERY_BIG / NONE_PERGA, the
0x717c6 pump flags, the 500 ms timeout; 403/512), 0x5d3a00 (the save slot writer: AUTOSAVE 0x62451c /
QUICKSAVE 0x624528, the two 0x220-byte slots, the 0x84-dword copy; 132/192) and 0x5d44c0 (the edit field
dialog 0x46a110, the key 0x1c; 166/272). ROUGH stubs with the flow in the comment: 0x5d3ab0 (the 16-slot
save / load window, ".SAV" 0x62d7fc, the texts 0x1cc5 / 0x1cc6 / 0x1cc7, the 0xa0 x 0x78 picture
0x766320), 0x5d4080 / 0x5d42b0 / 0x5d45d0 / 0x5d4920 (the save / load windows: the setup calls and the
16-slot click walk, the arguments are lost in the Ghidra pseudocode) and 0x5d4c70 (the options INI
writer: 50 keys texture_scale .. opponent_g into the profile 0x75e480). The unit now runs
0x5d33b0 .. 0x5d5640.

**Open work**:
- 0x5d4080 / 0x5d42b0 / 0x5d45d0 / 0x5d4920 / 0x5d4c70 / 0x5d3ab0 need the real bodies; the arguments
  of the first four are lost in build/ghidra and must be read from uv run disasm.
- The Ghidra pseudocode of the next starts is in the main checkout's build/ghidra (5d5640.c .. 5dd100.c);
  0x5dd3e0 has no procedure (uv run disasm).

**layout findings**: the save / load family's slot record is 0x220 bytes (0x84 dwords at +0x10, the name
at +9, the picture id at +8, the kind byte at +0); the control table 0xf8f080 + id * 0x2ec (visible +0x4c,
the word +0x14, the picture +0x1c0 = 3) and the screen records 0xf8f120 + id * 0xf1 / 0xf8f38c + id * 0x3c4
(the selected id 0xf8aad0) repeat in every window of the family.

## layout 2026-10-24 (session 13, second continuation)
The options module 0x5d5640 .. 0x5d6f40: 0x5d5640 the options reader (GetPrivateProfileIntA, the twin of
0x5d4c70; stub), 0x5d5c40 the shadow detail setup (stub), 0x5d5e90 the game speed factor (**drafted
95/128**: the double arithmetic and the float 0x5f1290), 0x5d5f10 the volume setters (stub: the two lost
__ftol arguments), 0x5d5fd0 / 0x5d6580 / 0x5d6950 the three option screens "menu\options_gfx" 0x62daf8,
"menu\options_sfx" 0x62db1c and "menu\options_game" 0x62db30 (stubs with their call sets: the window
family 0x4741f0 / 0x475870 / 0x475850 / 0x475f70 / 0x52ea30 / 0x476f80 / 0x515960, the controls 0x46bc70 /
0x46bca0 / 0x46bdd0, the pump 0x5309a0; on apply 0x5d4c70 + 0x5d5640 / 0x5d5f10 / 0x5d5e90). The unit now
runs 0x5d33b0 .. 0x5d6f40.

**Open work**: 0x5d6f40 (84 B, a list control builder) is the next start; the remaining stretch to
0x5de450 holds 0x5d6fa0 / 0x5d7080 (the black-screen fades: 0x4767d0 / 0x476860 / 0x46e990 / 0x52e600 /
0x522100 / 0x524010, the screen rect 0x6237d0 / 0x6248a4 ..), 0x5d7160 (the screenshot "!f3:ScreenShot"
0x62db44), 0x5d7430 (the quicksave "Gamedata\Saves\Quicksave.SAV" 0x62db54 / "Quicksave" 0x62db74),
0x5d7530 (the network start "Gamedata\network\%s.SRV" 0x62db80 / "Gamedata/cities/standard.scr"
0x62db98), 0x5d7f10 ("menu\messagefilter" 0x62dbc8) and the contact / gypsy screens
("contact_PLAUSCH" 0x62dc14, "Locations\zigeuner\blutrache" 0x62dc68, "_WAHRSAGERIN_FLUCH_0%d"
0x62dc88, "RAUSWURF" 0x62dca0).

## layout 2026-10-24 (session 14, worktree)
Drafted the last game-code stretch 0x5d6f40 .. 0x5dd100 (45 functions; the unit now runs
0x5d33b0 .. 0x5dd3e0, 63 functions, contiguous - no undrafted byte inside). The functions:
- the menu / screen block: 0x5d6f40 (the list control builder: 0x46bf10, the control record +0x5c = 1,
  0x46c260 (id, 300)), 0x5d6fa0 / 0x5d7080 (the black fades: 0x4767d0 "BLACK", the screen rectangle
  0xfb9060 .. 0xfb906c, the engine resets 0x52e600 / 0x46e990 / 0x46cfd0 / 0x522100 / 0x524010,
  0x760638 = 0x24087), 0x5d7160 (the screenshot into the 0x78 x 0xa0 image 0x766320: the
   scaled 0x140 x 0xf0 sample of the screen record 0x6ed924, the 0x420140 blit), 0x5d7320 (the loading
  screen "Gamedata/cities/standard.scr" RGB -> 0x766320 via dd_RGB), 0x5d73e0 (the quicksave player
  predicate over 0x770b80 + i * 0x21c, the kinds 6 / 7), 0x5d7430 (the quicksave: 0x5190b0
  "Gamedata\Saves\Quicksave.SAV", the network path 0x4c4db0 (0xf)), 0x5d7530 (the network start: the
  slot array 0x76fa20, the .SRV / .SAV writes, 0x75e640), 0x5d7770 (**the options menu "menu\options"**:
  the seven buttons 0x5d6f40 with the texts 0x1441330 .. 0x1441348, the radio group 0x46c490, the
  clicks to 0x5d4080 / 0x5d4920 / 0x5d45d0 / 0x5d6950 / 0x5d5fd0 / 0x5d6580), 0x5d7ba0 (the progress
  bar loop 0x472390 / 0x475730, the frame skip from the clock 0x62378c), 0x5d7e90 (the pause loop,
  "Pause") and 0x5d7f10 (the message filter "menu\messagefilter": nine rows 0x4731d0, he_Unk575640 /
  he_Unk575710).
- the gypsy / tailor block: 0x5d8080 (**a Ghidra-missed start**, the tailor chat window
  "locations\schneider\plausch" 0x62dbf8 registered by 0x5d8360: the participants 0x4f41b0, the talks
  0x53dda0 / 0x5176d0 / 0x4c4a60 / 0x4c5f60), 0x5d8360 (the blood-revenge prompt), 0x5d8420 (the
  production building listener: contact_PRODUKTION_SCHNEIDER / ROHSTOFFE_SUCHEN_SCHNEIDER / PLAUSCH /
  LAGER / TRANSPORT / PERSONALBUCH / MEISTERBRIEF), 0x5d85e0 (the blood revenge window
  "Locations\zigeuner\blutrache"), 0x5d8c90 (the curse window, "_WAHRSAGERIN_FLUCH_0%d"), 0x5d90e0
  (the curse target picker), 0x5d95a0 (the homage window), 0x5d9890 (the card game window), 0x5d9a80 /
  0x5d9b20 / 0x5d9b90 / 0x5d9c30 (the four contact listeners) and the poker family 0x5d9de0 / 0x5d9f50 /
  0x5da010 / 0x5da030 / 0x5dadf0 / 0x5db110 ("Locations\zigeuner\poker / poker2").
- the effect / explosion math block: 0x5dbc10 / 0x5dbc20 (the default effect record and its scale),
  0x5dbc30, 0x5dbed0, 0x5dbfc0 / 0x5dc0c0 (the effect file writer / reader), 0x5dc1a0 (the sphere volume
  record), 0x5dc210 (the position share), 0x5dc240 (the emitter record), 0x5dc3a0 (the particle step),
  0x5dc4a0 / 0x5dc9d0 / 0x5dcf00 (the curve samples over 0x45f170), 0x5dd090 (the 0 .. 60 / 60 clamp) and
  0x5dd100 (the trajectory record: d3_Normalize, d3_VecAngle2).
Matched this session (3): **0x5d6f40** (the d2_Objs pointer form: `U_I(U_I(0xf8f080) + ..)`, not the absolute
address), **0x5da010** (void, no `return 0`) and **0x5d7e90** (0x6f25ac is a byte: `U_B`). Near-misses:
0x5d7080 (27/224: the four rect loads are hoisted above the stores in the original), 0x5d6fa0 (38/224: the
same two loads), 0x5d7320 (59/192: the prologue / epilogue and the free order), 0x5d73e0 (51/80: `eax = 1`
set at entry before the flag test), 0x5d9a80 / 0x5d9b20 / 0x5d9b90 (76/112, 37/80, 86/144: the original
keeps the building in a callee-saved register (esi / ebx) across the contact calls and reads the click
0x6f22e4 once), 0x5d8360 (140/160), and the listener signature facts: 0x5d9a80 / 0x5d9b20 / 0x5d9b90 /
0x5d8360 are void.

**The end of the game code**: the last function is 0x5dd100 (ret at 0x5dd3d6, nop fill to 0x5dd3e0),
so the unit ends at 0x5dd3e0. 0x5dd3e0 .. 0x5de450 (4208 B) is library DATA, not game code: the
DIOBJECTDATAFORMAT arrays of dinput.lib's c_dfDIKeyboard (256 x {&GUID_Key 0x5eee70, i, 0x8000000c | i << 8,
0}) and c_dfDIMouse ({&GUID_XAxis, 0}, {&GUID_YAxis, 4}, {&GUID_ZAxis, 8}, 4 buttons), byte-equal to the
dinput.lib members of the DX 6.1 SDK with the GUID relocations masked. The game_range.toml range ends at
0x5dd3df since then, so the unit's end is proven (R7).

## layout findings (session 14)
- The 0x5d33b0 module is the game's **front end**: file selector, save / load windows, options, the
  options menu, the pause, the message filter, the gypsy / tailor contact windows, the poker windows and
  the effect math of the ending.
- The building listener pattern repeats in 0x5d8420 / 0x5d9a80 / 0x5d9b20 / 0x5d9b90 / 0x5d9c30:
  0x52e190, the pump 0x5309a0 (self, 0x67fff), the contacts 0x52e1c0 (name, id, type) on the flag
  0x6f231c bits 0x200 / 0x400, the click 0x6f22e4 dispatch and the closing 0x58f510 / 0x52e190 pair.
- The control record 0xf8f080 + id * 0x2ec repeats: +0x48 / +0x4c visible, +0x50, +0x1c0 = 3,
  +0x1c4 / +0x1cc / +0x1d0 the text box, +0x2e8 the value; the screen record 0xf8f120 + id * 0xf1 and
  the counter 0xf8f368 + id * 0x3c4.
- The blit / surface record (SCRSURF in the source) is 0x64 bytes: +8 height, +0xc width, +0x10 pitch,
  +0x24 the bitmap, +0x54 = 0x10, +0x58 / +0x5c / +0x60 the screen globals 0x1464fa4 / 0x1464fb0 /
  0x146359c. 0x420140 scales src into dst.

## match-base session 2026-10-24 (base-4 worker, shard 4/5)

- **0x5d33b0 -> ai_ListFiles, byte + readable.** The listing loop must be spelled as
  `i = 0; if (i < dir->nentries) { out = records + 9; do { ... i++; } while (i < dir->nentries); }`:
  the explicit `i = 0` before `out = ...` makes i's live range overlap param_3's, so VC6 gives i its
  own frame slot; with a plain `for (i = 0; ...)` the packer put i into the dead param_3 argument slot
  (frame 0x24 instead of the original 0x28) and 62 code hunks remained. `sprintf` replaces the K&R
  ai_Unk5de652 at the call site.
- **0x5d44c0 -> ai_EditText, behaviour + readable.** Meaning fixes: the key 0x1468410 is a byte
  (`kb_Last`), the picture fields at 0xfa6030 / 0xfa617c are **arrays** (Ghidra `&DAT` vs `DAT`:
  `U_I(U_I(0xfa6030)+x)` wrote through the array's first dword instead of into the array), and the
  copy loop uses a separate destination pointer (`dst = text; while ((*dst++ = *src++))`) - without
  it 17 code hunks remained, with it 0.
- **Escalated (deep)**: 0x5d34b0 (meaning matches; 246 code hunks are regalloc win ebp/ebx, slot order
  and VC6's deferred push cleanup), 0x5d3810 (meaning matches; frame 0x0c vs 0x10, regalloc), 0x5d3a00
  (meaning matches; branch layout / regalloc), and the ROUGH STUBs with no body: 0x5d3ab0, 0x5d4080,
  0x5d42b0, 0x5d45d0, 0x5d4920, 0x5d4c70, 0x5d5640, 0x5d5c40. ai_Unk5d34b0 / 5d3810 / 5d3a00 carry
  the @deep reason in their address comment.
- **0x5d5e90 in progress (draft, 71 -> 26 code hunks, kept)**: the byte test 0x6f23f8 is `U_B`,
  0x60bd84 converts as **unsigned** (fild QWORD with a zero high dword), the else multiplier 0.00625
  is a **float** constant (`fmul DWORD`; a double constant gives fmul QWORD), the return is void and
  `+ -0x40` (the original emits `add edx,0xffffffc0`). Left: the source branch order - the original
  tests `test cl,4; je` into the 0.00625 branch, so the 0x60bd84 branch is the if-true one; write
  `if ((U_B(0x6f23f8) & 4) != 0)` first. 0x6f3608 stays signed (fild DWORD).
- **Renames (project-wide)**: d2_Unk475cb0 -> d2_SetObjValue, d2_Unk475f40 -> d2_SetObjMax,
  d2_Unk4760b0 -> d2_SetObjFont, d2_Unk478320 -> d2_FocusInput (the merge kept base-2's name over d2_SetFocus), ai_Unk760638 -> sv_FrameMask,
  ai_Unk6f22a0 -> sv_PopRequest, ai_Unk14673a0 -> di_MouseEvent, ai_Unk5d44c0 -> ai_EditText.
- **Type debt**: d2.h cannot be included in this unit (it declares `void *vfs_Root` /
  `void *vfs_Tfopen`, vfs.h `VFSDIR *` / `VFSFILE *`); unk_5d33b0.c therefore declares local views
  (`D2OBJVIEW`, `D2INPUTVIEW`). The view struct must be padded to the **full** d2.h size (D2OBJ is
  0x2ec, not 0x7c) or `d2_Objs[id]` scales by the short size and reads the wrong record - it compiles
  and matches nothing, silently. The 2-parameter d2_SetObjValue call site is tagged `@original`
  (the 5-parameter definition; this site pushes 2, 0x5d4500).

## match-base session 2026-10-25 (base-4 worker, shard 4/6)

Unit debt: 50 -> 43 functions below behaviour + readable (7 raised, all byte + readable). New unit
header `include/unk_5d33b0.h` (the option globals and the unit's views; it grows with the unit).

- **0x5d5e90 -> ai_ApplyOptions, byte + readable.** Meaning fixes: the network branch is the if-true one
  (`if ((ai_GameFlags & 4) != 0)` first - the original tests `test cl,4; je` into the 0.00625 branch);
  the game branch multiplies in **float** (`fmul DWORD`, `(float)ai_OptionSpeed * 0.00625f`) and adds
  the double 0.25. The last code hunk was the constant encoding: `U_I(0x6f3610) + -0x40` compiles to
  `sub edx,0x40`, the named extern `ai_OptionMouseSpeed - 0x40` to the original's `add edx,0xffffffc0`
  (lesson in notes/vc6-matching.md) - the typed externs were both the Q3/Q4 fix and the byte fix.
  Renames: ai_Unk5d5e90 -> ai_ApplyOptions, ai_Unk6f23f8 -> ai_GameFlags, ai_Unk60bd84 ->
  cm_SpeedLevel, ai_Unk5f1290 -> cm_SpeedFactor, ai_Unk623814 -> sv_CameraStep. di_input.h now declares
  di_SwapLR (it was only in di_input.c).
- **0x5d5f10 -> ai_ApplyVolumes, byte + readable.** The "Sound" option record 0x6f3600 is read as
  `SNDOPTIONBYTES` with **unsigned char bitfields**: the original loads the master+sfx dword whole and
  extracts the sfx byte with `mov dl, ah`; plain `unsigned char` fields emit two byte loads and leave
  code hunks. Values: master = master_vol/127, sb_SetMasterVolume(sfx * master), msx_SetVolume(msx *
  master), msx_SetAmbVolume(sfx * master), snd_VoiceVolume = speech/127, snd_MusicVolume = (msx_freq &
  0xff) * 0.25. The unit includes f3snd.h (snd_VoiceVolume / snd_MusicVolume / the snd_* prototypes);
  msx.h cannot be included (it and f3snd.h redefine msx_Play), so msx_SetVolume / msx_SetAmbVolume are
  repeated in the unit header. sb_SetMasterVolume has no header yet (sblib.c), repeated there too.
- **0x5d6f40 -> ai_MenuButton, byte + readable.** The twin of main_Unk5337f0 (row stride 0x2b vs
  0x2e). Renames: d2_Unk46bf10 -> d2_CreateControl, D2OBJ.unk5c -> centered (d2.h's own comment says
  "centred (d2_CenterObj)"); the unit's D2OBJVIEW gained the field. `unk5c` is a Q2 placeholder, so the
  field name was the readability fix; d2.h still cannot be included (see the type debt section).
- **0x5dbc10 -> ai_GetDefaultEffect, byte + readable.** Returns ai_DefaultEffect (0x62de60: empty
  name, scale 1.0, lifetime 25.0, then the entry's ids / callbacks).
- **0x5dc1a0 -> ai_SphereVolume, byte + readable** (radius, size, out; params and the local named).
- **0x5da010 -> ai_ClosePokerWindow, byte + readable.** d2_Unk4758f0 -> d2_CloseView (43 refs, 10
  files); the poker view handle 0x62dda0 is ai_PokerWindow. The rename moved the lint baseline's X
  lines (uv run lint --update-baseline; 0 new, 0 fixed afterwards).
- **0x5d7e90 -> ai_PauseGame, byte + readable.** sv_Unk52e330 -> sv_SetStatusText (15 refs),
  ai_Unk6333e0 -> tb_Paused (tb.h already had that alias); ai_GameState (0x6f25ac) is declared `char`
  in the unit header to agree with f3snd.h - cm.h / f3init.c / ls.c have `unsigned char` (type debt,
  the byte code is the same for the stores here).
- Still next in the unit: 0x5d5fd0 (the graphics options screen, no body); the ROUGH STUBs and the big
  window bodies are base meaning work - write them from the disassembly, growing each stub in pieces.

## match-base 2026-10-25 (match worker, base-4)

Raised to **byte + readable**, each with one commit:
- **0x5d6fa0 -> ai_LeaveGameScreen** and **0x5d7080 -> ai_ResumeGameScreen** (the black fade pair).
  Renames: d2_Unk4767d0 -> d2_StartFade (0x4767d0, plus the ai_Unk4767d0 alias of main.c / f3init.c /
  sv.c / cm.c / unk_575860.c folded into it), d2_Unk476860 -> d2_ReleaseFade (d2_FreeFade was still a Q2
  placeholder: "eeFade" is six hex letters), sv_Unk522100 -> sv_SetPanelEnabled (1 enables, 0 disables:
  0x475aa0 sets disabled = (on == 0); the old comment had it backwards), sv_Unk52e600 ->
  sv_DisableVisibleWidgets, sv_Unk524010 -> sv_RedrawScreen. The typed reads use d2_ScreenW/H,
  d2_DirtyRectLeft/Top/Right/Bottom (0xfb9060.., d2.h) and the D2FADEVIEW flags byte (the original tests
  `test BYTE PTR [edi],4`, a dword field would not match). Data: ai_GameOver 0x6f259c (0x5d7080's
  "network game" reading is wrong: main.c sets it when the game is left and clears it at 0x537de0 start),
  ai_FrameWait 0x6f22bc (sv_RunFrame counts it down), sv_ViewRectLeft/Top/Right/Bottom 0x6248a4..0x6248b0
  (the game view rectangle 0x43b950 uses).
- **0x5d7320 -> ai_LoadScreenImage** (void function: the h==0 path returns through vfs_Tfopen's eax and
  the success path tail-calls vfs_fclose; i is initialised before the open, so `int i = 0` at the top).
  Reads ai_ScreenImage 0x766320 (ls.c's extern renamed from ai_Unk766320); the mem_Alloc tag is
  "ls:tmp" 0x623654, not "ls_tmp" (the link gate caught it).
- **0x5d7430 -> ai_Quicksave, IN PROGRESS at TIME UP** (73 code hunks at HEAD -> 2): void function, the
  network branch written first (if/else order decides the je/jne layout), U_B(0x6f1c38) = 1 is a byte
  store. The last hunk is `jl X` (ours) vs `js X` (original) on the sign test of 0x6f23f8; `uv run spell`'s
  cmp-test prescriptions tried: `>= 0` (jl), `< 0` early return (jl), `> -1` (cmp al,0xff; jle - worse).
  Open for the next base session: that spelling, Q3 (0x14414a8 / 0x14414a4 are runtime text-pointer
  slots in .data's BSS region, 0x6f1c38 has only ls.c's ai_Unk6f1c38), Q4 (`*(int *)rec = 1`, the
  0x7c-byte message record's +0 kind has no field name).
- **0x5d73e0 -> ai_HasQuicksavePlayer -> match-deep**: 41 code hunks, 37 vs 35 instructions; the record
  walk and tests match (p[6] > 0 is the unsigned jbe form), what is left is VC6 register allocation
  (al/eax vs cl/ecx, the hoisted -1 in edx vs the imm8 -1) - `uv run spell` matched no menu rule. Q4's single
  raw access is the dword at character +0x20a (ai.h's AICHAR pads 0x1e8..0x210; only ls.c's raw save
  slice reads it) - no evidence for a field name (Q2 would flag unk20a).
- Callee renames done with this session's evidence: ai_Unk4c3ae0 -> cm_GetSlotState (the state of a
  message slot, -1 none), unk_Unk5190b0 -> ls_SaveGame (the save writer), ai_Unk5d7160 -> ai_Screenshot.

## match-base 2026-10-25 (base-4 worker, shard 4/6, second session)

Unit debt: 39 -> 29 functions below behaviour + readable at the base level (6 raised byte + readable,
4 escalated to deep). All gates green at wrap-up: build 2554/4664, link 0 conflicts, ledger 0 errors,
lint 0 new X, 0 grade regressions.

- **0x5d7430 -> ai_Quicksave, byte + readable** (the last 2 hunks of the previous session). The
  sign test of ai_GameFlags is **bit 7**, not `< 0`: `if ((flags & 0x80) != 0)` on a signed `char`
  local compiles to `test al,al; js`, the `< 0` family always stays `jl` (scratch build of the
  variants: `>= 0`, `(int)flags < 0`, `(signed char)` all `jl`; `(flags & 0x80) != 0` is the only
  `js`). Readability: `CMQUICKSAVE` (0x7c B; `slot` +0, `name` +4), the two text slots
  ai_QuickSaveDisabledText 0x14414a8 / ai_QuickSaveNoPlayerText 0x14414a4, ls_SaveSlot 0x6f1c38
  (ls.c writes it into the header; 0 autosave, 1 quicksave, the network slot - renamed from
  ai_Unk6f1c38 in ls.c), prototypes ls_SaveGame / cm_GetSlotState in the unit header (ls.c has no
  header - type debt), the cm_SendExSysMessage int-returning call cast tagged @original (the
  callee is void, its body returns 0x4c39c0's eax).
- **0x5d9a80 -> ai_GypsyContact, 0x5d9b20 -> ai_CurseContact, 0x5d9b90 -> ai_TheatreContact: all
  byte + readable.** The old "the original keeps the building in a callee-saved register across
  the plate calls and reads the click once" diffs were **missing calls**: VC6 drops a call to a
  function whose body it has already seen when the body has no side effect (`stub { return 0; }`),
  and the listeners sat below the 0x5d85e0 .. 0x5d9890 stubs. Moving the three listeners above
  those stubs (out of address order, marked with a note; move them back once the stubs have their
  real bodies) makes all three match byte for byte. Scratch check: an empty callee defined above
  its caller loses its call, the same callee below keeps it - the rule is body visibility, not the
  address. Readability: parameters / locals by role, typed externs ai_BuildingPerms 0x6f231c (the
  0x503530 permission bits of the current building), ai_ClickedObject 0x6f22e4 (cm.h), the five
  contact text slots 0x144a884 / 0x144a9d8 / 0x144a8c0 / 0x144a98c / 0x144aa9c, prototypes of the
  opened windows in the unit header. Renames: ai_Unk5d85e0 -> ai_BloodRevengeWindow, ai_Unk5d95a0
  -> ai_HomageWindow, ai_Unk5d8c90 -> ai_CurseWindow, ai_Unk5d90e0 -> ai_CurseTargetWindow,
  ai_Unk5da030 -> ai_PokerGame (now `void` parameters: its only caller pushes none),
  sv_Unk52e190 + its alias ai_Unk52e190 -> sv_ReleaseNamePlates, sv_Unk52e1c0 ->
  sv_RegisterNamePlate (the name main.c / unk_575860.c already used), ai_Unk6f231c ->
  ai_BuildingPerms (only sv.c's extern held the name; the 84 raw uses stay raw in other units).
  The K&R declarations of sv_ReleaseNamePlates in unk_575860.c / unk_58bbd0.c had to become
  `void ...(void)` (the folded alias returned int; lint X otherwise).
- **0x5dbc20 -> ai_GetDefaultEffectScale, byte + readable.** The original TU called the float clamp
  0x5dd090 through an int K&R declaration: it pushes the default effect's scale bits as an int
  (`mov eax,ds:0x62de6c; push eax`), returns the callee's st(0) (float - its callers in
  unk_575860.c use `(int)ai_Unk5dbc20(1)` and declare it `float f();`), and its cleanup merges the
  argument pop into `pop ecx; ret`. Spelling: `float f(void)` + the call cast to
  `int (__cdecl *)()` (scratch-verified: the cast form emits exactly these bytes, a K&R float
  declaration adds `fstp st(0)`), tagged @original on the call line (lint wants the tag there).
  Renames: ai_Unk5dbc20 -> ai_GetDefaultEffectScale, ai_Unk5dd090 -> ai_NormalizeEffectScale (the
  0 .. 60 -> 0 .. 1 clamp); the bits extern ai_DefaultEffectScaleBits 0x62de6c.
- **0x5dc210 -> ai_PositionShare, byte + readable.** The original divides `fld value; fdiv divisor`
  where the U_F form (and the parenthesised struct field `(x / (rec->reach))`) makes VC6 emit
  `fld divisor; fdivr value`. The spelling that matches is `a / rec->reach * 0.1f` - a typed field
  access without parentheses (scratch-verified: `a / r->div` fdiv, `a / (r->div)` fdivr). The
  record is the SPHEREVOLUME view (radius / size / surface / volume / reach) that 0x5dc1a0 fills;
  am.c passes its int par[5] as int *in. Void. Rename ai_Unk5dc210 -> ai_PositionShare in am.c/am.h.
- **0x5dd090 -> ai_NormalizeEffectScale -> match-deep** (56 -> 34 code hunks, readable). The negated
  compare `!(x < 60.0f)` is the original's `test ah,0x5; jp` (the positive `x >= 60` forms emit
  `and eax,0x100`), the divisions are float (a double `/ 60.0` becomes an fmul of the reciprocal),
  and `v <= 0.0f` is the second test's spelling. Left: the original keeps the selected value on
  the FPU stack across `fcomp 0.0`, tests C0|C3 (`and eax,0x4100; jne`) and puts the return-0 block
  out of line, ours folds the select and uses `test ah,0x41; jp` - FPU / branch scheduling, deep.
- **Escalated with no body (readability done, empty stubs): 0x5d5fd0 -> ai_OptionsGfxScreen,
  0x5d6580 -> ai_OptionsSfxScreen, 0x5d6950 -> ai_OptionsGameScreen** (1454 / 974 / 1514 B
  originals; the call sets are recorded in the comments, the bodies need the disassembly statement
  by statement). Call sites renamed in main.c too.
- Next base item in the unit: 0x5d7160 ai_Screenshot (232 code hunks). Still far in the unit:
  0x5d8420 (34 hunks: init order of the seven plate ids and VC6's merged stack cleanup at the
  dispatch join - the original shares one `add esp,8` at 0x5d85a7 for the two-argument branches,
  ours cleans up per branch), 0x5dbed0 (a stub with fsin/table math), the ROUGH-free stubs of the
  gypsy/poker/effect stretch (0x5dc3a0 .. 0x5dd100) and the big window bodies.

## match-base 2026-10-25 (base-4 worker, shard 4/6, third session)

- **0x5d7160 ai_Screenshot -> behaviour + readable.** The original's shape: ONE float step
  (`(float)d2_ScreenW * 0.003125f`, the literal is the compiler constant 0x5eedf0) is advanced over
  BOTH the 0x140 columns and the 0xf0 rows (x and y accumulators; the row offset is
  `(int)yf * pitch`, computed in the outer loop); the row / column converts are the __ftol helper
  0x5de518; `d2_ScreenSurf->bits` is re-read every iteration (a store through buf may change the
  global) while the pitch load is hoisted; buf[i++] is a WORD store buf + 2*i - 2. The two
  DDSURFACEDESC2 descs (h/w/pitch 0xf0/0x140/0x280 and 0x78/0xa0/0x140, lpSurface buf /
  ai_ScreenImage, 16 bpp; the source desc also takes dd_RMask / dd_GMask / dd_BMask) are cleared
  with the D3_ZERO inline-asm macro, not memset: the inline asm is what forces the ebp frame
  pointer (/Oy) - a memset draft compiles ebp-less. The function is void (no `xor eax,eax` tail).
  Left: order / fpu only. Scaffolding: D2SURFVIEW (pitch +0x10, bits +0x1c) because d2.h cannot be
  included here (its vfs_Tfopen / vfs_Root clash with vfs.h); prototypes for d2_LockSurface /
  d2_UnlockSurface / mem_Alloc / mem_Free / dd_RMask / dd_GMask / dd_BMask; includes ddraw.h,
  surf.h (surf_ScaleBlit), d3.h (D3_ZERO).
- **0x5d7530 ai_SaveNetworkGame -> readable, escalated to match-deep (1 code hunk).** The network
  save: clears the 8 acknowledgement slots cm_SaveAcks 0x76fa20 (8-byte stride, [i][0] = netId,
  [i][1] = save hash; message case 16 in cm.c fills the first free), counts the player records
  (do-while from ai_Chars + 2, kind 6/7, id != 0xffff) into the byte ai_NumPlayers, copies the
  requested name into ls_SavePath (the 0x60-byte path field of the save header), writes the .SAV
  with "wb" and header flag 5, reopens it "rb", hashes the header's first 0x78 bytes (util_Hash),
  sends system message 0x10 with [0] = the local player's netId and [1] = the hash (0x7c bytes =
  0x1f dwords), waits until as many ack slots are filled as there are players, then writes the .SRV
  (flag 6) and .SAV (flag 5) saves and clears the status line. rec[0] must be written before rec[1]
  (only then the scheduling matches). Escalation: the original stores ai_NumPlayers = 0 before the
  record walk, our VC6 removes that dead store; tried a volatile declaration (keeps the store but
  turns the wait-loop compare into mov/and instead of xor/mov), separate and chained init, for vs
  do-while, the wait-loop compare spellings. Rest is instruction-equal up to registers plus the
  fill tail. See the @deep reason.
- **ai_Unk5d7770 (0x5d7770) lead:** the original's __chkstk size is 0x981c and the 0x2580-dword
  image copy targets frame+0x21c, so the original has 0x21c bytes of locals where ours (0x961c,
  image at esp+0x28) have about 0x40. The extra ~0x200 are probably the buffers / records the
  window calls (0x5d4080, 0x5d45d0, 0x5d4920, 0x5d3810 ...) take - their arguments are lost in
  Ghidra. Read the first 0x21c bytes of locals from uv run disasm before the body rewrite.
- Renames (project-wide, all references): ai_Unk76fa20 -> cm_SaveAcks, ai_Unk6f25c8 ->
  cm_RequestedSaveSlot, ai_Unk6f25d0 -> cm_RequestedSaveName, ai_Unk75e640 -> ls_SavePath,
  ai_Unk5de652 -> crt_Sprintf, sv_Unk52fcf0 -> sv_BroadcastText, unk_Unk5194c0 ->
  ls_WriteSaveHeader, unk_Unk51cee0 -> ls_ReadSaveHeader, ai_Unk5d7530 -> ai_SaveNetworkGame.
  crt_Sprintf's K&R declarations became one prototype in every caller (lint X otherwise); in
  unk_58bbd0.c the prototype had to move above its first use.
- **0x5d7770 ai_Unk5d7770 (in progress):** fixed one meaning error found while reading: the
  original calls d2_Unk475850 with a second argument 2 (`push 0x2` at 0x5d77bc), the draft passed
  only the window. Byte count unchanged (712 differing bytes, one asm line fewer), the function
  stays draft; the full body needs the rewrite described in the lead above (the 0x21c of locals,
  the `&image[1]` argument of the save / load windows 0x5d45d0 / 0x5d4920, the click dispatch
  order 0x5fe710 == id0 / id1 / id3 / id4 / id5 / id7 and the confirmation id).

## match-base 2026-10-25 (base-4 worker, shard 4/6, fourth session)

Unit debt: 29 -> 27 functions below behaviour + readable at the base level (1 byte + readable, 1 in progress).

- **0x5d7770 -> ai_OptionsMenu, byte + readable.** The body from uv run disasm 0x5d7770: the missing
  0x200-byte confirmation text buffer (the save / load windows 0x5d45d0 / 0x5d4920 and the box 0x5d3810
  take it, the frame is 0x981c with the image at +0x21c of the locals), the click chain order id0, id1,
  **id5** (leave the game, the confirmation text 0x1ca4), **id2** (game options 0x5d6950), id3 (gfx
  0x5d5fd0), id4 (sfx 0x5d6580), id7 - the creation order is NOT the compare order; the save / load pair
  by the network flag (0x5d45d0 when bit 2 is clear); d2_CloseView(wnd) at the end (the draft dropped the
  argument); msx_SetAmbVolume(sfxVolume) restores the ambient volume; no 0x576020 call on the exit path;
  void (the tail call 0x576030 is the last statement). The last hunk (the 0x6f25ae store scheduling to
  just before the id2 call) disappeared with the typed globals below.
  **Order trap (VC6)**: the calls to the empty / `return 0;` stubs whose bodies were visible above the
  caller vanished from the code (VC6 drops a call whose result is unused to a side-effect-free body it
  has already seen). The three option screens and the two save / load stubs were moved below 0x5d7770
  (out of address order, order note in the .c); move them back once they have bodies. A prototype does
  not re-enable the call - only body invisibility does.
  Readability: the unit header now declares the menu's globals (d2_LastWindow 0x6ed938, ai_GameOver,
  ai_LeaveGame 0x6f25a0, sv_MenuOpen 0x6f25ae, rt_Clicked 0x1467398, d2_PointerObj 0x5fe710,
  snd_AmbientOnByte 0x6f0b60, the seven text slots ai_TextLoadGame .. ai_TextBack 0x1441330 .. 0x1441348)
  and the prototypes of its callees. **snd_AmbientOn.on (the char member of the G16AMBIENTON union at
  0x6f0b60) changes the whole function's register allocation** (ebp pushed in the prologue, id slots
  shift) - 'saved = ...' must read the unsigned-char alias (snd_AmbientOnByte), not the union member.
- **0x5d7ba0 ai_Unk5d7ba0, IN PROGRESS at TIME UP (draft, 205 code hunks).** Read from uv run disasm
  0x5d7ba0: the draft's statements match the original's flow (the fade 0x4767d0 / 0x476860 pair, the
  screen switch 0x46e990 / 0x46cfd0, the bar 0x472390 + 0x475730, the text 0x515960 (0x2004), the loops).
  Leads for the next session: (a) the screen dimensions are **shorts** (d2_ScreenH 0xfb9070 /
  d2_ScreenW 0xfb9072, movsx WORD in the original; the draft's U_I reads ints), (b) the progress
  comparison is `contentHeight <= (short)[+0xa] * 1.5 + scrollY` (the original's test ah,0x5 / jp = not
  greater and not unordered; the draft's `<`), (c) the step chain is three compares (x < 80 -> x < 30 ->
  x <= 30) = `if (x >= 80) step = 4; else if (x >= 30) step = 2; else step = 1;` per loop, (d) the
  original stores the screen record pointer into 0x6ed980 (d2_LastWindowPtr, `mov ds:0x6ed980,ecx`)
  inside the loop before the frames % step test and reads the +0x248 / +0xa / +0x244 fields through
  it - the draft reads U_I(0x6ed980) + offsets and misses the store. The D2WIN record (0x3c4) is
  0xf8f120 + bar * 0x3c4 = d2_Wins[bar] (+0x244 contentHeight, +0x248 scrollY, +0x0a h).
- **0x5d7ba0 ai_Unk5d7ba0, follow-up 2026-10-25 (match-base): 205 -> 68 code hunks, still draft.** All four
  leads above are applied and each was a real gain: (a) `*(short *)0xfb9070` at d2_CreateWindow and the
  negation (movsx); (b) the progress comparison written `1.5 * (short)[rec+0xa] + rec[0x248] >
  (double)rec[0x244]` - the operand order matters, the flipped spelling gives the `test ah,0x5; jp` form;
  (c) the step chain exactly as recorded (`>= 80 -> >= 30 -> else 1`, both loops); (d) the record pointer
  `rec = (int *)0xf8f120 + bar * 0xf1` (int-index = 0x3c4 bytes) stored with `U_I(0x6ed980) = (int)rec`
  before the frames % step test, the counter `rec[0x92] = rec[0x92] + 1` (first loop) / `rec[0x92]++`
  (second), the compare reading through the reloaded global. The function has **no return value** (the
  original has no `xor eax,eax`: it is `void`, `return 0` removed - 1 hunk). Reverted no-gain tries: the
  `tri->x = x = ...`-style double assignment, dropping the `pump` variable (122 hunks), `>= 30` mixed with
  `< 30` (73), the second loop's step chain in `<` polarity (71). Left: the first-loop counter is
  load/inc/store in the original, `inc [mem]` here; and the whole block layout around the bar/frames
  register pair (ours bar = esi / frames = ebp, the original the other way round) - register allocation.
  The six screen-size reads are now `d2_ScreenH` / `d2_ScreenW`, and `sv_PopRequest` (0x6f22a0),
  `d2_CutsceneActive` (0x6ed9d4), `di_MouseEvent` (0x14673a0), `kb_Last` (0x1468410) replace their U_I /
  U_B literals (all byte-neutral).
  **The record is a d2.h D2WIN**: 0xf8f120 = `D2WIN d2_Wins[0x61]`, so `rec = &d2_Wins[bar]` (0x3c4
  bytes, the draft's `(int *)0xf8f120 + bar * 0xf1`), and the fields are d2.h's own `win->h` (0x0a, short),
  `win->contentHeight` (0x244) and `win->scrollY` (0x248); 0x6ed980 = `d2_LastWindowPtr` (D2WIN *). Typing
  them needs a local D2WINVIEW in this unit (d2.h cannot be included, see the type debt) and a name for
  0x6ed980 that does not clash with d2.h's D2WIN * - next session's first readability step. Q1 is the block:
  ai_Unk5d7ba0 has no evidenced name (the strings it touches are "BLACK" only).

## match-base 2026-10-25 (base-4 worker, shard 4/6, second session)
- Rename `ai_Unk6ed938 -> d2_LastWindow` (131 references in cm.h, cm.c, f3gm.c, io_text.c, main.c, sv.c,
  unk_575860.c; d2.h's own name for 0x6ed938, "window created last"; byte-neutral, check 2567). The old
  placeholder stays as the data.csv alias list entry only if another name is added later.
- ai_Unk5d7ba0: 205 -> 68 code hunks (details in the follow-up bullet above); the record is a d2.h D2WIN
  (d2_Wins[bar]). Still draft, in progress; Q1 (no evidenced name) is the readability block.

## match-base 2026-10-25 (base-4 worker, shard 4/6, third session)
- **0x5d7ba0 -> ai_CreditsScreen, BYTE + readable.** Q1 evidence: the caller is the credits button of the
  main menu (main.c 0x533850, `bcred`, main.c's own comment "credits 0x5d7ba0", followed by
  "ZumGutenEnd.mp3"). The byte fixes, each one hypothesis: (1) the frame rate chain is **three**
  comparisons in the original - `if (x >= 80) step = 4; else if (x >= 30) step = 2; else if (x < 30)
  step = 1;` (the redundant third test is `fnstsw ax; test ah,5; jp`: MSVC's `<` spelling; the plain
  `else` of the draft gave two); (2) the first loop's increment is written through the **global**
  `d2_LastWindowPtr->scrollY = d2_LastWindowPtr->scrollY + 1` (load/inc/store; through the local
  `win->scrollY = ... + 1` VC6 emits `inc [mem]`, and through the global the write forces the reload of
  0x6ed980 that the original has), while the second loop's is `d2_LastWindowPtr->scrollY++` (`inc [mem]`
  in the original too - the two loops really are spelled differently); (3) `d2_WinOpenPic` /
  `d2_CreateWindow` need the **int** caller prototype (the original pushes ints: `push eax/ebx/edx/8`),
  the short K&R definition would make VC6 push the low word only (`mov cx` vs `movsx ecx`).
  New: `D2WINVIEW` (0x3c4, h 0x0a / contentHeight 0x244 / scrollY 0x248) with `d2_Wins` and
  `d2_LastWindowPtr`; `sv_FramesPerTick` (0x62378c, the frame counter 0x623790 as a float).
- **0x5d7f10 -> ai_MessageFilterScreen, readable, escalated (@deep).** Renames (project-wide):
  `unk_Unk515960` / `ai_Unk515960 -> txt_Print` (858 refs), `ai_Unk62378c -> sv_FramesPerTick`,
  `ai_Unk475600 -> d2_CentreViewWindows`, `ai_Unk4758f0 -> d2_CloseView` (the TUs' K&R declarations
  unified to the d2.h `void (int view)` prototypes: he.c needed the prototype moved above its first
  call), `he_Unk575640 -> he_IsMessageKindFiltered`, `he_Unk575710 -> he_SetMessageKindFiltered`.
  D2OBJVIEW gained checkable / interactive / picW / captionX / captionY and the row captions
  0x1441500 are `ai_MessageFilterTexts[]`. Escalation reason in the function's `@deep`: the meaning
  matches Ghidra line by line; the 77 code hunks are VC6's induction-variable strength reduction
  (ours increments IVs for `i * 0x1c + 10` / `i + 0x605`, the original recomputes the lea chain per
  iteration) and the stack frame (0x2c vs 0x44; the original spills `wnd` and re-reads it before
  d2_CloseView); `uv run spell`: no menu rule, the three reload leads are that same slot allocation.
- Renames this session (besides the above): `ai_Unk5d7ba0 -> ai_CreditsScreen`,
  `ai_Unk5d7f10 -> ai_MessageFilterScreen`. Names.csv grew the D2WINVIEW block and the new externs.

### ai_Unk5d8080 (0x5d8080, tailor chat window): leads read from the disassembly this session
Read against 0x5d8080..0x5d8349 (uv run disasm), the draft's *meaning* is wrong in three places and
its locals do not fit the original's frame:
- **Frame**: original `sub esp,0x41c`; the draft has `char buf[0x400]; char buf2[0x400]; int parts[0x24]`
  -> 0x898/0x89c. Layout read from the instruction offsets: `count` at frame+0x10 (the `&count` argument
  of 0x4f41b0 at 0x5d80ba is this slot), `obj` (the d2_GetWinObj result, 0x475b80 call at 0x5d8181) at
  +0x14, `found` at +0x18, the participant array at +0x1c (loop read `[esp+edi*4+0x1c]` at 0x5d8217), the
  txt_Format buffer at +0x3c (0x5d8288), the second formatted string at +0x48 (0x5d82aa) - so the two
  strings are short buffers near each other, not two 0x400 arrays. 0x41c - 0x1c = 0x400 left for the
  participant array (256 dwords) - i.e. the query writes into one big int array.
- **The count cap is inverted**: at 0x5d8102-0x5d810b the original does `eax = limit; inc eax; if
  (count >= eax) count = eax`, i.e. count = min(count, limit + 1). The draft's `limit++; if (count <
  limit) count = limit;` is the opposite (max).
- **The click test** at 0x5d81e9: `if (d2_PointerObj == -1) skip; if (d2_PointerObj != obj) skip;` - the
  draft compares the clicked object with 0 and throws the d2_GetWinObj result away.
- Tried: `if (count > limit) count = limit + 1;`, `obj = d2_GetWinObj(...)` + `click == obj` (build clean,
  unitdiff 0x5d8080 559 -> 584 bytes = WORSE); reverted. Fix the locals/frame first, then these.
- Further renames this session (byte-neutral, all units build): `ai_Unk5d8080 -> ai_TailorChatWindow`
  (the tailor chat, form "locations\schneider\plausch"), `ai_Unk4c4a60 -> cm_SendSetField` (the canonical
  cm.c name; the K&R declaration here became the 5-parameter prototype), `gm_Unk5092e0` /
  `ai_Unk5092e0 -> gm_BuildingSkill` (f3gm.c's building-slot sum, f3gm.c's own comments call it "the
  0x5092e0 skill of b"), `he_Unk53dda0 -> he_SendEventText` (he.c: message 0x11, event text; the name
  the hmt.c note already used). 0x6f2308 is `ai_PlayerBuilding` (data.csv) and 0x770b80 `ai_Chars` -
  the tailor chat still reads them through U_I, so its Q3/Q4 stay open.
- ai_TailorChatWindow (0x5d8080) stays **draft** (in progress): Q1 done (name), Q2 still lists its
  unnamed callees (ai_Unk4c4ed0 message 0x24, ai_Unk4c5f60 message 0x5a, unk_Unk4f41b0 the accuser
  query, ai_Unk475b80/0x475ff0 d2_GetWinObj/d2_GetViewWin which cannot take a prototype here while
  0x5d34b0 calls 0x475ff0 with 6 arguments), Q3/Q4 (the ai_Chars / ai_PlayerBuilding raw reads), Q5
  (the ROUGH comment stays until the body is re-read). The frame and the three meaning bugs are under
  the leads above; the next session fixes those first.

## match-base 2026-10-26 (base-4 worker, shard 4/10)
- **0x5d8080 ai_TailorChatWindow: behaviour + readable.** Meaning read from 0x5d8080..0x5d835c: the locals are
  `int ids[4]` + `char buf[0x400]` (frame 0x41c; the two event texts are written at buf and buf + 0x10; the
  query's max is 4 and the cap only lowers count, so ids[4] holds them); the cap is
  `if (count >= limit + 1) count = limit + 1;` (min, not the draft's max); the printed word and count are the
  request's (`req->id`, `count`), not the character record's; the window object is
  `obj = d2_GetWinObj(wnd, 2, io_PrintText("%ia[%s]$N", 0x422d));` and the following text is
  `io_PrintText("%in[%s]", 0x422e)`; the click test is `d2_ClickedId != -1 && obj == d2_PointerObj`; the
  ai_Chars[ai_PlayerId] reads in the loop, the two message calls and the news are spelled fresh (the original
  recomputes them per use - only cm_SendSetField uses the cached rec); the function is void. Readability: AIREQ /
  AICHARVIEW (with flags1cc) views, the three format / news texts as string literals (`uv run link` locates them by
  content), d2_ClickedId in the unit header, prototypes for the callees (d2_GetViewWin only from here: 0x5d34b0's
  six-argument call keeps the K&R declaration, tagged @original).
- **0x5d8360 ai_TailorContact: byte + readable.** flags1cc bit 0x20000 -> message box + return; else
  sv_CheckMoney(character, 1); then a D3_ZEROed 0x50-byte request (+0 = 1, +4 = 0x400, +0x10 = 6) through
  ai_AmountMenu with the callback ai_TailorChatWindow; then sv_CameraFollow. **The zeroing is D3_ZERO, not
  memset**: memset(&req, 0, sizeof req) folds to `mov ecx,0x14; rep stosd` in an esp frame, D3_ZERO (d3.h)
  reproduces the original's ebp frame with the two argument temps (the size at ebp-4, the pointer at ebp-8).
  The buffer is 0x50 bytes (0x14 dwords, the same record 0x5d8c90 uses). New externs ai_TailorBlockedText
  0x144aa60 / ai_TailorAmountText 0x144aa68 in the unit header.
- **0x5d8420 ai_ProductionContact: behaviour (readability open).** Fixed: void (no `return 0`; the original
  tail-jumps to sv_ReleaseNamePlates), the declaration / init order lager, schn, trans, personal, meister, roh,
  plausch (the original's register and dispatch order), and the PLAUSCH branch calls
  `ai_TailorContact(param_1, param_2)` (the original pushes both arguments; the callee ignores them, so its
  definition declares them unread). Only `order` hunks remain in B. Open readability (next session): rename
  param_1 / param_2, replace U_I(0x6f231c) / U_I(0x6f22e4) by ai_BuildingPerms / ai_ClickedObject (both already in
  the unit header) and declare the seven contact text slots 0x144aa84, 0x14401b4, 0x144aa58, 0x1441f90,
  0x1441fa0, 0x143c9f0, 0x143cbc8 (the contact captions; no symbol yet).
- Renames this session (project-wide, byte-neutral, every use updated): unk_Unk4f41b0 / ai_Unk4f41b0 ->
  gv_FindAccused, ai_Unk4c4ed0 -> cm_SendProofExecuted, ai_Unk4c5f60 ->
  cm_SendMoneyDelta, ai_Unk5d8360 -> ai_TailorContact, ai_Unk5c1130 -> ai_AmountMenu, sv_Unk520f90 ->
  sv_CheckMoney, sv_Unk52d7a0 -> sv_CameraFollow, ai_Unk5d8420 -> ai_ProductionContact, ai_Unk58bd60 ->
  ai_ProductionWindow, ai_Unk595480 -> ai_TradeSearchWindow, ai_Unk5aab80 -> ai_PersonnelBook, ai_Unk5b26b0 ->
  ai_StorageWindow, ai_Unk5c3200 -> ai_MasterLetterWindow.

## match-base 2026-10-26 (base-4 worker)
- **0x5d8420 ai_ProductionContact: behaviour + readable** (was behaviour + draft). The seven contact text
  slots are now named externs in include/unk_5d33b0.h (ai_ContactTextProductionTailor 0x144aa84,
  ai_ContactTextRawMaterialSearch 0x14401b4, ai_ContactTextChat 0x144aa58, ai_ContactTextStorage 0x1441f90,
  ai_ContactTextTransport 0x1441fa0, ai_ContactTextPersonnelBook 0x143c9f0, ai_ContactTextMasterLetter
  0x143cbc8); the params are building / arg, ai_BuildingPerms / ai_ClickedObject replace the raw reads, and
  the building +0x5d (AIBLD.objList) is read through the packed AIBLDVIEW. The view **must** be
  `#pragma pack(push, 1)`: an unpacked struct aligns the int to +0x60 (the hunk read `[eax+0x60]` against the
  original's `[eax+0x5d]`). The function keeps only order hunks in B.
- The same seven addresses are reused by the perfumery listener (0x595ed0, unk_58bbd0.c) whose header now
  declares the four shared slots too.

## match-base 2026-10-26 (base-4 worker, shard 4/6, fifth session)

Unit debt: 27 -> 21 functions below behaviour + readable at the base level (3 byte + readable /
behaviour, 3 escalated with their bodies written). Gates at wrap-up: build 2611/4664 (was 2610),
link 0 conflicts, ledger 0 errors, lint 0 X, 0 grade regressions, unitdiff no WORSE.

- **0x5d3ab0 -> ai_FillSaveSlots, body written, readable, -> match-deep.** The whole 1488-byte body from
  tools/disasm: the 0x2100-byte slot clear, the -1 obj/win init, ai_ListFiles (dir, ".SAV"), the header
  read (ls_ReadSaveHeader + the flags bit 2 skip), ai_RegisterSaveSlot, the child window (win->w - 0x10 /
  -0xa0 / -0xb0, font 0x54, mt 0), the title text (d2_CreateWinText(-2) + the strcpy loop), the
  ls_Version >= 0x10028 border clear of ai_ScreenImage and d2_WinOpenRaw, the two io_PrintText calls
  (0x1cc5: name, year + 0x578, hour, minute % 60; 0x1cc6: the three text-id offsets 0x114 / 0x130 /
  0x253 + a saved byte, the scenario, the money string "%i%c", ai_DifficultyTexts) and the second pass
  (memset + the empty-slot name 0x14414e0 + 0x1cc7). Meaning fix found while reading: the compact-text
  select is `(compact ? 0x28 : 0) + 0x253`, not the inverse (Ghidra's `-(cond) & 0x28`), worth 100+
  hunks. Left: 121 differing instructions in 5 regions of register allocation / lea folding / the
  pending-push cleanup (the @deep reason lists them). New: SAVEHEADERVIEW (0xfc, ls.c's writer layout),
  SAVESLOTVIEW (0x220), SAVEREQUEST (0x210), ai_DifficultyTexts 0x14413f0, ai_EmptySlotNameFormat
  0x14414e0.
- **0x5d4080 -> ai_LoadGameWindow, behaviour + readable; 0x5d42b0 -> ai_JoinGameWindow, behaviour +
  readable.** Both from their disassembly: the shared setup (form "menu\loadgame_new", the view placement,
  sv_FitForm, the pointer sprite, the caption 0x1c57 / 0x1c58, the 6-argument d2_GetViewWin(win, 1, 0x82)
  + the 4-argument slider), the pump loop, the 16-slot hit test (caption object / the slot's window /
  a control inside it) and the two different actions: the load window writes the .SAV path into
  ls_SavePath and sets ai_GameFlags = 10 (the dword view ai_GameFlagsWord; the byte view would be a byte
  store); the network picker (0x5d42b0, dir "gamedata/network", fill mode 1) copies the slot name into
  ls_SavePath for main_Unk532a70. ai_FillSaveSlots needs a **sixth parameter** (the original call pushes
  one that 0x5d3ab0 never reads) - without it the frame, the cleanup and the setup block never line up.
  Left: the `add esp,8` the original emits after d2_GetViewWin (grade behaviour: the same instructions
  moved) and the `push 0x101` order.
- **0x5d45d0 -> ai_SaveOptionWindow, body written, readable, -> match-deep.** The local save: the two
  autosave / quicksave slots disabled, the slot walk 2 .. 15, the name editor ai_EditText on the slot
  title, the exists check of "Gamedata\Saves\<name>.SAV", the overwrite box 0x14414ec, the file name
  "Gamedata/saves/GILDE_SAVEGAME_%i.SAV", the 0x9600-byte image memcpy into ai_ScreenImage, ls_SaveSlot,
  sv_OpenMessageBox 0x14414e8, ls_SaveGame, the pop request. The 19 code hunks are the pending-push
  cleanup split (the @deep reason lists the tries: the exists = 0 + if spelling 36 -> 21 hunks, its
  placement 21 -> 19).
- **0x5d4920 -> ai_SaveNameWindow, body written, readable, -> match-deep.** The network save: the twin
  with the one disabled slot, dir "gamedata/network", the CMQUICKSAVE 0xf request through
  cm_SendExSysMessage (or the status text 0x14414a4 when ai_HasQuicksavePlayer finds no player) and
  sv_PopRequest = 1. Same cleanup-split root (17 code hunks).
- **0x5d4c70 -> ai_SaveOptions, BYTE + readable.** The 52 INI keys transcribed from the Ghidra output
  (the section strings "Gfx" / "Sound" / "Game", the profile 0x75e480): each dword holds four option
  bytes (OPTIONBYTES union views), the 12 screen tuning floats come out as `(int)(x * 100.0f)`, the town
  name is written as it is, the seven opponents are ints. The one hunk left after the first build was
  panel_help's missing `& 0xff`. New symbols: ai_GfxDetail 0x6f35c4, ai_GfxFloor 0x6f35c8, ai_GfxSet
  0x6f35c0, ai_GfxFog 0x6f35cc, ai_GfxResolution 0x6f2f94, ai_ScreenTuning 0x6f35d0, ai_GameSwitches
  0x6f3618, ai_GamePanel 0x6f3668, ai_OptionScrollSpeed 0x6f3614, ai_TownName 0x6f361c, ai_OptionHistorie
  0x6f365c, ai_OptionMission 0x6f3660, ai_OptionNetMission 0x6f3664, ai_OptionPanelHelp 0x6f366c,
  ai_Opponents 0x75e160, ai_OptionsFile 0x75e480; crt_Itoa 0x5e56c4 (the library's itoa); the
  SNDOPTIONBYTES union gained msxFreqByte.
- **0x5d5640 -> ai_LoadOptions, body written, readable, -> match-deep.** The reader twin: every
  GetPrivateProfileIntA with its default (character_detail 1, brightness a 0x32, contrast / gamma 100,
  nachtwaechter / show_cursor_txt / show_geb_info 1, mission / net_mission 1, the panel options 1, the
  opponents 2), the floats * 0.01f, the town string ("Augsburg"), invert_mouse forced to 0 and the cur_res
  mode table (ai_ScreenModes 0x62d7b8 -> ai_ScreenModeW / H 0x62d7d0 / 0x62d7d4). Two roots left: the
  table index (a typed struct index gives [eax*8+base], the original's char offset gives shl eax,3;
  the char-offset spelling is a Q4 cast) and the store scheduling.
- Next base item in the unit: 0x5d5c40 (the shadow detail setup).

## match-base 2026-10-28 (solo session on main)

- **0x5d5c40 -> ai_ApplyGfxOptions, behaviour + readable** (a 16-byte ROUGH stub; 176 -> 98 differing
  instructions). Body rewritten against 0x5d5c40..0x5d5e8b: shadow detail (`ai_GfxDetail.byte[3]`) ->
  `ch_ShadowEnabled` + the 0x20 / 0x40 texture size + the changed flag; a changed size resets / frees the
  object shadows (mask 0x40), clamps `d3s_TexSize` to `d3s_MaxTexSize` (unsigned) and calls
  `d3_UpdateAll(0)`; `init_SetDetailLevel(ai_MapRefs, 0)` (data_views' macro); the LOD handling
  (`byte[2]`) picks `d3io_Lod.all` (2 / 4), deselects the scene, reloads the meshes (0x40, 3, 0) and
  reselects; `d3_ApplyGraphicsOptions(floor byte, 0, 2 - texture_scale, changed, 0)`; the fog scale
  `1.0 - fog * 0.01` with **double** constants only while neither ai_PlayerBuilding nor
  ai_FallbackBuilding is set; the town-scene character bodies get the shadow bit (0x211 bit 3) with the
  xor idiom `b ^= ((ch_ShadowEnabled << 3) ^ b) & 8`; `ch_SetLowPoly(character_detail)`;
  the camera limit pair of the camera_limits byte (table 0x6237d0, six pairs, the pair i and i+3) into
  sv_CameraHeightNear .. sv_CameraAngleFar; `sv_PlaceCamera` when ai_GameState; `d3fl_SetMips` on a floor
  change; then select the scene, ai_CheckBuildSites, d3_SetRLights(detail != 0), reselect.
- Views/types added: INIT_CHARACTER (+ ch_List), the INIT_LOD **union** (the whole-dword d3io_Lod: the
  original compares / stores the dword, a bitfield member gave the and/xor chain), prototypes for the
  13 callees.
- Left for match-deep (tag): the camera limit copy - ours does float copies (fld/fstp) from the table
  reached as (sv_CameraHeightNear - 0x30) and the original copies four dwords from a **table symbol the C
  does not own** (0x6237d0 has no name / definition; defining that data first is the clean fix), plus the
  changed / lod / scene register-slot rotation.
- Renames (project-wide): ch_Unk5f122c -> ch_ShadowEnabled, sv_Unk525dd0 -> sv_PlaceCamera,
  ai_Unk623800 / 04 / 08 / 0c -> sv_CameraHeightNear / sv_CameraAngleNear / sv_CameraHeightFar /
  sv_CameraAngleFar, ai_Unk6f22d4 / d8 -> sv_CameraHeightFactor / sv_CameraAngleFactor.
- Lesson: a **union view of a bitfield** is the source form when the original moves the whole dword; the
  bitfield member spelling makes VC6 emit `and` / `xor` around the store.

## play 2026-10-29 (the missing camera limit table)

- The "table symbol the C does not own" is defined now: `sv_CameraLimits[12]` in sv.c at 0x6237d0, directly
  before sv_CameraHeightNear (0x623800). ai_ApplyGfxOptions uses `lim = sv_CameraLimits;` instead of
  `(float *)&sv_CameraHeightNear - 12` - 10 fewer differing instructions (88 asm, 608 B).
- While the table was missing, the copy read the linker's neighbours of sv_CameraHeightNear: the tutorial's
  camera pitch became 8.2e20, the camera matrix garbage, every terrain tile was culled and the game screen
  showed only the fog colour. The six pairs are (1200, -0.9), (650, -0.471238911), (190, -0.104719758) for
  the near view and (1200, -0.9), (1800, -1.08210409), (2400, -1.04719758) for the far one; the values are
  byte-identical to the original's 0x6237d0..0x6237ff.

## play 2026-10-29 (ai_CameraStep stays an int; the type debt is gone)

`ai_CameraStep` (0x623814) was the last "sv.c reads the same address as float" debt. The original treats it as
an int everywhere: 0x526358 / 0x528141 / 0x52b0d8 / 0x599f2c / 0x59a29c load the dword and push it as the step
argument, 0x527a50 / 0x528094 / 0x5280d7 use `fild` for the float comparisons. sv.c now declares
`extern int ai_CameraStep` (its address-only `unsigned char` declaration at the top was the conflict), passes
the int to d3t_PlaceObject(V) and casts at the two float uses. Reading it as float gave the denormal 4.2e-44,
i.e. a camera step of 0; the tutorial's camera focus then never moved. The header comment no longer calls it
a type debt.
