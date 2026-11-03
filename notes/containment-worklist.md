# Containment worklist (decompanion ab2c99c)

decompanion's new checks fail gildegold's gate with 103 errors. They are real defects of the sources, not of the
tool: each one makes the program depend on a link layout it does not declare. Fix them in match-base / layout
sessions; take one group at a time, and `dcp build` + `dcp grade NAME` after each edit (no byte match may fall).
`dcp asmdiff NAME` marks the lines (`M.contain: outside its object`), `dcp data map UNIT` lists the `room_ref`
addresses after a datum.

- **M.contain** (ill-formed): our reference `&X + N` lies outside X's declared size (sizeof). The compiler's
  own arithmetic is exempt (indexed operands, a loop pointer one element past either end).
- **D7** (ill-formed): a view reads past its container's declared size.
- **D9** (contradiction): the original's code reads the padding our object gives a datum.

Done already (same bytes, the relocation names the object the address lies in): 50 gap sums that landed on another
gap piece (`full_data_6f4f64 + 0x380` -> `full_data_6f52e4`), the three `&d2_Pics + 0x1ebd8 / 0x269d8 / 0x269da`
(he_Events + 0x119b0 / 0x197b0 / 0x197b2).

## How to fix each kind

- **Constants of another object spelled from a neighbour** (`*(float *)((unsigned)&sv_PhaseMinutes + 0x140)`, the
  target in unknown.data): define the datum at its address with its own name and type (in the unit whose data block
  holds it, in address order: D4), then use it. sv_PhaseMinutes' targets 0x5eebf8 / 0x5eec10 / 0x5eec50 /
  0x5eec58 / 0x5eed68 / 0x5eed6c lie in sv's .rdata after it; gm_MoodRanges + 0x7c8 (0x5ee928),
  gm_TagGebaeudePROT + 0x12c (0x61ab64), d3if_ParList + 0x318 (0xfb98f8), ai_Unk625820 + 0x1780..0x1798
  (0x626fa0..0x626fb8) likewise.
- **One object of the original declared as several** (`memset(&evt_Globals, 0, 12)` over evt_Globals /
  evt_NGlobals / evt_MaxGlobals; `memset(&d2_DirtyRectLeft, 0, 16)` over the four d2_DirtyRect ints;
  `d2_LastWindowPtr + 6 / 8 / 0xa` as shorts): rewriting the memset as single stores costs the byte match
  (evt_Init falls to draft). Declare the block as one struct or array of the original's size and make the old names
  views or fields of it (D8's advice).
- **Flag bytes behind a 1-byte container** (d3_MipSkip, d3_D3DActive, d3if_Unk6339db, ai_GameState, dd_GBits: the
  D7 views and the D9 padding reads are the same objects): the original's object is 4 bytes (or 3); declare the
  container as large (`unsigned char d3_MipSkip[4]` or a struct of the four flags) and keep the views inside it.
- **Tables a gap piece stands for** (`full_data_6f5320 + col * 0x58`, `full_data_6f4f60` rows of 0x38 with a field
  at +0x10, `gm_DefaultStock` read as 0x55 records of 0x80 bytes): declare the table at its size (gap pieces merged,
  gm_DefaultStock as the GMSTOCK array the original copies).

## M.contain, by object

- `ai_GameState` (1 bytes), 3 site(s) at +0x2 (operand): ai_OptionsMenu (src/unk_5d33b0.c), sv_RunFrame (src/sv.c)
- `ai_Unk625820` (3680 bytes), 8 site(s) at +0x1780, +0x1784, +0x1788, +0x178c, +0x1790, +0x1794, +0x1798 (operand): he_Unk550ee0 (src/he.c)
- `d2_DirtyRectLeft` (4 bytes), 2 site(s) at +0x8, +0xc (operand): d2_Open (src/d2.c)
- `d2_LastWindowPtr` (4 bytes), 21 site(s) at +0x6, +0x8, +0xa (operand): ai_Unk596ff0 (src/unk_58bbd0.c), ai_Unk598b80 (src/unk_58bbd0.c), ai_Unk598dc0 (src/unk_58bbd0.c), ai_Unk5992c0 (src/unk_58bbd0.c), ai_Unk5996b0 (src/unk_58bbd0.c), ai_Unk59b160 (src/unk_58bbd0.c), ai_Unk59be90 (src/unk_58bbd0.c), ai_Unk59c120 (src/unk_58bbd0.c), ai_Unk59c450 (src/unk_58bbd0.c), ai_Unk59c700 (src/unk_58bbd0.c), ai_Unk59d5e0 (src/unk_58bbd0.c), ai_Unk5a0220 (src/unk_58bbd0.c)
- `d3_MipSkip` (1 bytes), 13 site(s) at +0x2, +0x3 (operand): d3_BeginScene (src/d3_d3d.c), d3_SetBlendState (src/d3_d3d.c), d3d_CreateDevice (src/d3_d3d.c)
- `d3if_ParList` (788 bytes), 1 site(s) at +0x318 (operand): par_Init (src/d3_par.c)
- `evt_Globals` (4 bytes), 1 site(s) at +0x8 (operand): evt_Init (src/evt.c)
- `full_data_6f4f60` (4 bytes), 1 site(s) at +0x10 (immediate): ai_Unk5ae9a0 (src/unk_58bbd0.c)
- `full_data_6f5320` (4 bytes), 1 site(s) at +0x58 (immediate): ai_Unk5923c0 (src/unk_58bbd0.c)
- `gm_DefaultStock` (48 bytes), 2 site(s) at +0x2a80, +0x2ab0 (immediate): gm_AllocRaum (src/f3gm.c), gm_InitPlayerRecord (src/f3gm.c)
- `gm_MoodRanges` (1904 bytes), 1 site(s) at +0x7c8 (operand): he_Unk571c30 (src/he.c)
- `gm_TagGebaeudePROT` (24 bytes), 1 site(s) at +0x12c (immediate): he_Unk565940 (src/he.c)
- `sv_PhaseMinutes` (96 bytes), 32 site(s) at +0x140, +0x158, +0x198, +0x1a0, +0x2b0, +0x2b4 (operand): ai_EmbezzlementContract (src/unk_58bbd0.c), ai_EmbezzlementPrivilege (src/unk_58bbd0.c), ai_EspionagePayment (src/unk_58bbd0.c), ai_EspionageWindow (src/unk_58bbd0.c), ai_Unk575030 (src/he.c), ai_Unk5a53e0 (src/unk_58bbd0.c), ai_Unk5bea00 (src/unk_58bbd0.c), ai_Unk5d0320 (src/unk_58bbd0.c), ai_Unk5d0530 (src/unk_58bbd0.c), ai_Unk5d0b20 (src/unk_58bbd0.c), ai_Unk5d0c70 (src/unk_58bbd0.c), ai_Unk5d0f20 (src/unk_58bbd0.c), he_Unk550070 (src/he.c), he_Unk5506c0 (src/he.c), he_Unk550850 (src/he.c), he_Unk55a5f0 (src/he.c), he_Unk571000 (src/he.c), he_Unk571c30 (src/he.c)

## D7 / D9

- D9 ai_FloatOne at 0x5eb780 is 4 bytes, our object pads it to 0x5eb788; the original's code refers to 0x5eb784 (+0x4, 53 uses) in that padding: ai_FloatOne is larger (an array or a struct), or an object of its own is missing there (dcp data map) (src: src/cm.c:16701)
- D9 ai_CardBack at 0x5ed1bc is 4 bytes, our object pads it to 0x5ed1c4; the original's code refers to 0x5ed1c0 (+0x4, 1 use) in that padding: ai_CardBack is larger (an array or a struct), or an object of its own is missing there (dcp data map) (src: include/unk_5d33b0.h:263)
- D9 d3_MipSkip at 0x63342c is 1 bytes, our object pads it to 0x633430; the original's code refers to 0x63342d (+0x1, 6 uses), 0x63342e (+0x2, 9 uses), 0x63342f (+0x3, 4 uses) in that padding: d3_MipSkip is larger (an array or a struct), or an object of its own is missing there (dcp data map) (src: src/ts_texture.c:55)
- D9 d3_D3DActive at 0x63345c is 1 bytes, our object pads it to 0x633460; the original's code refers to 0x63345d (+0x1, 2 uses) in that padding: d3_D3DActive is larger (an array or a struct), or an object of its own is missing there (dcp data map) (src: src/d3_engine.c:2579)
- D9 d3if_Unk6339db at 0x6339db is 1 bytes, our object pads it to 0x6339df; the original's code refers to 0x6339dc (+0x1, 2 uses) in that padding: d3if_Unk6339db is larger (an array or a struct), or an object of its own is missing there (dcp data map) (src: src/f3init.c:314)
- D9 ai_GameState at 0x6f25ac is 1 bytes, our object pads it to 0x6f25af; the original's code refers to 0x6f25ad (+0x1, 2 uses), 0x6f25ae (+0x2, 3 uses) in that padding: ai_GameState is larger (an array or a struct), or an object of its own is missing there (dcp data map) (src: src/sv.c:7795)
- D9 dd_GBits at 0x1463580 is 1 bytes, our object pads it to 0x1463584; the original's code refers to 0x1463581 (+0x1, 7 uses) in that padding: dd_GBits is larger (an array or a struct), or an object of its own is missing there (dcp data map) (src: src/dd_vesa.c:807)
- D7 view d3d_ZEnabled at 0x63342d is d3_MipSkip +0x1, outside d3_MipSkip (1 bytes): the view reads whatever the linker puts after d3_MipSkip; declare d3_MipSkip as large as the original's object (a struct or an array) (src: include/data_views.h:104  b=src:src/ts_texture.c:55)
- D7 view d3d_balpha at 0x63342e is d3_MipSkip +0x2, outside d3_MipSkip (1 bytes): the view reads whatever the linker puts after d3_MipSkip; declare d3_MipSkip as large as the original's object (a struct or an array) (src: include/data_views.h:105  b=src:src/ts_texture.c:55)
- D7 view d3d_glow at 0x63342f is d3_MipSkip +0x3, outside d3_MipSkip (1 bytes): the view reads whatever the linker puts after d3_MipSkip; declare d3_MipSkip as large as the original's object (a struct or an array) (src: include/data_views.h:106  b=src:src/ts_texture.c:55)
- D7 view d3_NoTextures at 0x63345d is d3_D3DActive +0x1, outside d3_D3DActive (1 bytes): the view reads whatever the linker puts after d3_D3DActive; declare d3_D3DActive as large as the original's object (a struct or an array) (src: src/ecmd.c:502  b=src:src/d3_engine.c:2579)
- D7 view d3_Unk63345d at 0x63345d is d3_D3DActive +0x1, outside d3_D3DActive (1 bytes): the view reads whatever the linker puts after d3_D3DActive; declare d3_D3DActive as large as the original's object (a struct or an array) (src: include/d3_engine.h:572  b=src:src/d3_engine.c:2579)
- D7 view d3if_Unk6339dc at 0x6339dc is d3if_Unk6339db +0x1, outside d3if_Unk6339db (1 bytes): the view reads whatever the linker puts after d3if_Unk6339db; declare d3if_Unk6339db as large as the original's object (a struct or an array) (src: include/data_views.h:138  b=src:src/f3init.c:314)
- D7 view main_GameEnded at 0x6f25ad is ai_GameState +0x1, outside ai_GameState (1 bytes): the view reads whatever the linker puts after ai_GameState; declare ai_GameState as large as the original's object (a struct or an array) (src: src/main.c:3271  b=src:src/sv.c:7795)
- D7 view sv_MenuOpen at 0x6f25ae is ai_GameState +0x2, outside ai_GameState (1 bytes): the view reads whatever the linker puts after ai_GameState; declare ai_GameState as large as the original's object (a struct or an array) (src: include/data_views.h:109  b=src:src/sv.c:7795)
- D7 view dd_GLoss at 0x1463581 is dd_GBits +0x1, outside dd_GBits (1 bytes): the view reads whatever the linker puts after dd_GBits; declare dd_GBits as large as the original's object (a struct or an array) (src: include/data_views.h:115  b=src:src/dd_vesa.c:807)
