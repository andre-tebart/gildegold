# unk_589f10.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the findings of each session, one section per author.

## Unit
The network lobby (layout 2026-10-23 session 21). Range 0x589f10 .. 0x58bbd0, name unknown (the screen is the
form "menu\netlobby"; `@named_by -`).
- Start `proven` (data order D8 + code seam): the previous unit (unk_575860.c) ends with its literal
  "gamedata/screenshots/gilde%04i.bmp" 0x62aa58 (0x589bc0); the four dwords 0x62aa7c .. 0x62aa88 (-1, 0, -1, -1)
  are the globals of a new .data block and its first literal "menu\netlobby" 0x62aa8c is used by 0x589f10. Code
  seam: 0x589e50 ends at 0x589f07 with nop fill 0x589f08 .. 0x589f0f; 0x589f10 is called from main.c 0x53bb20.
- End: the literals climb 0x62aa8c ("menu\netlobby"), 0x62aa9c ("Beruf"), 0x62aaa4 ("%s: %d"), 0x62aaac
  ("%s: %s"), 0x62aab4 ("%s %s %s "), 0x62aac0, 0x62aad0 through 0x58ba70; the next run starts with
  "Handel\Handel_PRODUKTION" 0x62b038 (0x58bd60, a big +0x568 jump). 0x58bbd0 (the table init called by
  0x58bd60) is the first function of that next unit (plausible).
- Globals: the player record 0x6f5e20 .. 0x6f5e76 (the name +0x12, family name +0x22, city / emblem +0xc,
  the gender word +0x10 with the religion at +0x11, the profession +0xb, the chosen player 0x6f5e78, the
  player list 0x6f267c, the selected index 0x6f5ea0, the selected id 0x6f5e00), the window ids 0x62aa7c /
  0x62aa80 / 0x62aa84 / 0x62aa88, the profile 0x75e480 with the last choice 0x75e5a0 .. 0x75e5ca, the screen
  0x6ed938, the window table 0xf8f080 (0x2ec bytes per window) and the control table 0xfa5aac / 0xfa6030.

## Open work
- 0x589f10 ROUGH (2595 B): the whole structure (the profile read / write, the sub window builders, the frame
  loop, the click cases on 0x62aa7c / 0x62aa80 / 0x62aa84 and the player list) is written from the Ghidra
  pseudocode; the store order, the local frame (0x250) and the sprintf / text local assignment are not verified.
  Mine compiles to 2448 B (original 2595 B).
- 0x58aa30 (368 B), 0x58aba0 (384 B), 0x58ada0 (432 B): drafted, nonmatching (248 / 324 / 357 B differ). The
  two identical branches of the city-name copy in 0x58aa30 (0x9364dc == 0x3f) are written once; the
  strcat in 0x58ada0 is a hand loop in the original (rep movs). The text helpers 0x5de652 (sprintf) and
  0x473650 / 0x4731d0 (controls) need their real prototypes (0x473650 returns the control id, its second
  argument is a short y).
- 0x58ad20 (128 B): nonmatching 64 B, the entry scan of the player list 0x6f267c[1 .. 8] (0x4cd820).
- 0x58af50 (2352 B, the choose-player window) and 0x58b880 (496 B, the choose-profession window) and
  0x58ba70 (352 B, the chat lines): drafted from the Ghidra pseudocode, ROUGH.
- 0x58bbd0 (400 B, the production window's table init) belongs to the next unit (see Unit).

## layout 2026-10-23 (session 21)
Found the unit boundary in the course of the frontier work: the drag-and-drop family of unk_575860.c ends with
the screenshot helper 0x589bc0 and the speed / slot keys; 0x589f10 opens the network lobby, a module of its own
(the .data block 0x62aa7c .. proves it). Drafted 0x589f10 (ROUGH) and 0x58a940 / 0x58a9d0 (both matched first
try), 0x58aa30, 0x58aba0, 0x58ad20, 0x58ada0, 0x58af50, 0x58b880 and 0x58ba70 (drafts).
The Ghidra exports of this stretch were missing (no build/ghidra/<addr>.c); the pseudocode came from REA
`batch_decompile` of 0x589f10, 0x58a940, 0x58a9d0, 0x58aa30, 0x58aba0, 0x58ad20, 0x58ada0, 0x58af50, 0x58b880,
0x58ba70, 0x58bbd0, 0x58bd60 and 0x58c680 (the last two are the next unit).

## match-base 2026-10-24 (match worker)
All 10 functions escalated to match-deep (`@deep` in their address comments) - the easy means cannot raise
any of them:
- Q1 blocks every function: the unit is unnamed (`@named_by -`), so the module prefix and the function names
  would be invention; the header prototypes only give roles ("the lobby's buttons", "the chat lines").
- Q2/Q3/Q4 in every function: the lobby's globals and the control API (0x475f70 select control, 0x52e350 /
  0x52e860, 0x4731d0 / 0x473650 text) are placeholder callees, 8..63 address literals per function and the
  U_I / U_B / U_W macros (Q4).
- The 8 drafts need structural byte work: code hunks 28 (0x58ad20) .. 1020 (0x589f10); 0x589f10 is 2448 B
  against the original's 2595 B and the original keeps an ebp frame over its 0x250 local frame (our build
  omits it); 0x58ada0's strcat is a rep movs hand loop; 0x58aa30 writes the two identical city-name
  branches (0x9364dc == 0x3f) once; ROUGH / SKELETON in 0x589f10 and 0x58af50.


## match-base 2026-10-25 (base-4 worker)
- Reference-only renames (no code change; every use updated, build/link/ledger clean): ai_Unk4c3ae0 -> cm_GetSlotState (0x4c3ae0) and ai_Unk4767d0 / ai_Unk476860 -> d2_StartFade / d2_ReleaseFade (0x4767d0 / 0x476860, the old ai_ aliases folded into the d2 names). See notes/units/unk_5d33b0.md for the whole rename list.
