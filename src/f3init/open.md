# f3init.c (new, layout 2026-10-23 session 2)

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** first; below it the findings of each session, one section per author.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): game initialisation, scene loading, start objects
- Boundaries: separated (plausible) (layout 2026-10-23, 3rd session: every function of the range has a draft; end fixed at 0x5142e0 = io_text.c): name from the tag "f3_init:Gebaeude des Typs %s allokiert" (0x622ecc, used by the callback 0x50d550) and the error strings "init_LoadObjektScene:" / "init_LoadGebaeudeScene:" / "init_LocalData(): gm_RichterKatholisch==NULL" (0x622f20 / 0x622fa4 / 0x6231c4). Boundary to f3gm.c: the last f3gm string is "_NACHRICHTEN_HS_67" 0x61ade0 (0x50cad0); right after it (0x61adf8) come this unit's initialised globals (0x61adf8 detail level, 0x61ae00 handle, then the table of 19 records of 0x6c4 bytes at 0x61ae08 .. 0x622e94: the character animation names per building class, used by 0x512f30), then its strings from "KONTOR" 0x622e94 on (the first user is 0x50d100). Evidence: .data (one block f3gm -> f3init without overlap, the table is f3init's own: it is used only by f3init functions) and the tags; the first attempt joined it into f3gm.c (one .data block) and was undone: a "f3_gm:" and a "f3_init:" tag in one unit is unlikely. Content: scene object names to prototypes (0x50d100), the object / building scene loaders (0x50d7f0 / 0x50dc30, "scenes/*ob_%s.ed3" / "scenes/*gb_%s.ed3"), the start objects (0x50f0b0 .. 0x50f750), the network game start (0x510e50 not drafted, 0x5112d0), "init_LocalData" 0x511ad0, building names 0x511890, season textures 0x5128d0 (four copies in the original), scene callbacks (0x50d550, 0x50d6a0, 0x5126a0, 0x512e80), detail level 0x513590, comma lists 0x513970, the city INI 0x513a20, the town loaded 0x513740. Boundary to io_text.c at 0x5142e0: this unit's strings end with the city INI block (0x6233xx .. 0x623558, used by 0x513a20: "KONTOR" Import / Export, the sections "A - ALLGEMEIN" ..); the next block starts with "_STADTAUSWAHL_%s_INFO+0" 0x623564 (0x5142e0). Open: ROUGH drafts (see notes/units/f3init.md); data debt (0x61adf8 / 0x61ae00 / the 19-record table 0x61ae08 are externs)
- Range then: 0x50d100..0x5142e0 (all drafted, address order)
- Was: tag "f3_init:"

## Open work
- **headers (2026-10-30, typing pass 2)**: head.c includes txt_views.h, gv.h, d3.h, d3_quad.h, txt.h, evt.h, d3_light.h, net.h, ai_types.h, ch_types.h, data_views.h, d3_snd.h, sblib.h, unk_5d33b0_types.h, f3init_types.h, rd_types.h, ls.h, f3init.h; keeps 1 cast / view macros with @original byte evidence (the address and the matched caller are in each comment): util_RandomU; views / casts without byte evidence of their own (weak definitions or drafts' spellings, notes/headers.md debt item 1): d3snd_AttachI, ai_PlayerCharC.
- Everything in the range 0x50d100 .. 0x5142e0 has a draft now (layout 2026-10-23 3rd session: 0x50e690, 0x50f910, 0x510e50, 0x511f20, 0x513740, 0x513a20 are new, ROUGH except 0x513740 which matches). The unit ends at 0x5142e0: the text builders and the text engine are io_text.c (see notes/tus.md).
- ROUGH drafts (lost operands, restructured): 0x50e690 (the float operands of the random calls are lost; 2528 vs 2592 B), 0x50f910 (3264 vs 3616 B), 0x510e50 (1168 vs 1152 B), 0x511f20 (1136 vs 1450 B: the record walks are summarised), 0x513a20 (the city INI loader, 2288 B), 0x50d7f0, 0x50dc30 (the two scene loaders are near copies), 0x50e0d0, 0x50e330, 0x50e5a0, 0x50f0b0, 0x50f340, 0x50f570, 0x50f750, 0x510770, 0x510ad0, 0x511400, 0x511890, 0x5112d0, 0x5124f0, 0x5128d0 (written as one loop; the original has four near copies and is 1296 B, ours 688), 0x513050, 0x513400, 0x513590, 0x50d550.
- Data debt: the unit's initialised globals are not defined in C yet: 0x61adf8 (detail level 4 / 2 / 1, used by 0x513590) and 0x61ae00 (a handle, -1) and the table of 19 records of 0x6c4 bytes at 0x61ae08 (an int kind, then 16 animation names of 0x30 bytes: used by 0x512f30); they are declared as externs. Strings: the unit's block runs from "KONTOR" 0x622e94 to at least 0x6235a8.
- Missed function starts (callbacks): all drafted (0x50d550, 0x50d6a0, 0x5126a0, 0x512e80, 0x511f20).
- Names: the unit is called f3init.c after the tag "f3_init:" (0x622ecc); functions are unk_Unk<addr> (no real names yet).

## layout 2026-10-23 (session 2)
- Found as a unit of its own after the first attempt joined it into f3gm.c: see notes/tus.md (the .data evidence and the two different tags).
- Scene objects: names "ob_NAME[_A..H]" / "gb_NAME[_A..H]" (0x50d100: the variant suffix is cut off, the name is looked up in the object prototypes (0x91aaf0, 0x41 bytes each, name at +1) or the building prototypes (0x7e3f30, 0x24d bytes each, name at +1); the result 0x1000000 | n / 0x2000000 | n at +0x48 of the scene object); the scene objects have +0x1f8 parent, +0x200 owner record, +0x218 kind, +0x220 state, +0x224 a flag. The scene universes (0x411bb0 finds a free one) are kept per object prototype (0x76faa0, 0x2dc bytes) and per building class (0x76fa60, 0x22 bytes), the records of the universes are 0x3e0 bytes at 0xfd7ba0 (name) with a dirty flag at 0xfd7f75.
- A `strcmp`-like inline loop in Ghidra (2 bytes per iteration) is the VC6 strcmp intrinsic; a `repe cmpsb` with ecx = 3 is strcmp / memcmp of a short literal.
- Matched: 0x511600 (needs `>= 5`), 0x511dc0 (`strcpy`), 0x512de0 (a callee with a `char` third parameter), 0x5115b0 (a `char` return), 0x5124d0, 0x510730, 0x510e20 and a few more (see names.csv).

## layout 2026-10-23 (session 3)
- 0x513740 matches (needs `int ai_Unk6f35cc`: a dword, `& 0xff` in the expression, not a byte). 0x513a20 reads a city INI ("%s%s/%s.ini" of the gfx dir + "gamedata/cities"): the sections "A - ALLGEMEIN" (Stadtname, KartenPosition, Glaube, HistorieStart / Ende, Waehrung (an object name, 3 characters skipped), Land, Sprache, KartenOffset, MaxPlayer, NachbarStadt: the neighbour cities are loaded recursively into the slots 1 ..), "A - EINWOHNER" / "A - PRUNK" (Einwohner_%i / Prunk_%i pairs), "B - WETTER", "C - PRIVILEGIEN", "E - UMLAND" (Umland%i), "F - GESETZE" (six law lists of 12), "KONTOR" (Import / Export good lists) into the town slot table 0x7e3f40 (0x2f4 bytes each; offsets in the comment).
- 0x510e50: the network game start (host: loads "gamedata/cities/AUGSBURG.NET" by default, 0x520a30 sends the save, the client waits for 0x6f2594 bits 1 / 4); returns 1 = the city could not be loaded, 2 = 0x520b80 failed.
- 0x50f910 hands each player character its first building of the job (0x50f750) or builds one from the prototype list (0x58f640 / 0x58f7a0), fills the market buildings (the class table cls[]: 0 nothing, 1 one or two persons, 2 / 3 special) and lets all characters of the kinds 2 .. 7 greet each other (0x4c4b00 both ways).
- unk_Unk5142e0 / 514360 moved to io_text.c (the unit ends at 0x5142e0).

## match-base 2026-10-24 (base-4 worker, shard 4/6)

- Reference-only: ai_Unk58fd90 -> ai_ObjectAngle in 0x50f910's vecB / list200 site (the call still goes
  through an implicit declaration there; adding a gv.h prototype would be the unit's own cleanup).
  No f3init.c code changed.

## match-base 2026-10-25 (base-0 worker, shard 0/6)

- 0x447190 call sites renamed ai_Unk447190 -> d3if_Delete (local K&R line replaced by the full prototype); byte unchanged.

## match-base 2026-10-25 (base-4 worker)
- Reference-only renames (no code change; every use updated, build/link/ledger clean): ai_Unk4c3ae0 -> cm_GetSlotState (0x4c3ae0) and ai_Unk4767d0 / ai_Unk476860 -> d2_StartFade / d2_ReleaseFade (0x4767d0 / 0x476860, the old ai_ aliases folded into the d2 names). See notes/units/unk_5d33b0.md for the whole rename list.

## match-base 2026-10-25 (base-5 worker, shard 5/6, 2nd)

Shard 5/6, unit f3init.c (base queue 62 -> 53 functions below base). Check count 2548 -> 2551. Grade
regressions 0.
- Raised byte + readable: init_SceneObjectDestroyCB (0x50d350; he_RemoveGfxInfos + d3q_RemoveObject +
  d3_QuadTree named project-wide), init_ResetSceneUniverses (0x50d460; **two memsets** - see the VC6
  lesson - and the char return 0), init_FreeScenes (0x50d490; the height maps of the 64 scenes, bound as
  INIT_SCENE_HEIGHTMAP_END - a symbol in the compare made the masked field differ, the macro keeps the
  immediate), init_LoadTownScene (0x50d6d0; the char-returning d3_LoadScene prototype turned test eax
  into test al and dropped the xor al,al).
- Raised behaviour + readable: init_SceneNameToPrototype (0x50d100; the KONTOR store is ONE dword store
  0x2000047, not four char stores - VC6 merges the byte stores when they are written as chars),
  init_InheritOwner (0x50d6a0; SCENEOBJ.parent / .owner typed).
- Escalated: init_HookObject (0x50d380, byte; Q4 8 raw accesses - D3OBJECT +0x200 has no field in d3.h,
  b is an AIBLD from the un-includable ai.h; the same blocker ls.c 0x51d4f0 recorded), init_AllocSceneBuilding
  (0x50d550, far draft 90 code hunks, all read; `uv run spell` reload / dup-tail tried, no change; Q4 same
  blocker), init_AdjustSceneLights (0x50d730, far 61/59, no menu rule; Q4 needs named D3OBJECT.unk05c /
  0x1e8 / light-record fields).
- Facts found: the "scene objects" of this unit are D3OBJECTs (name 0x00..0x4b, the prototype reference is
  written into the tail of name at +0x48: 0x1000000 | object index / 0x2000000 | building index; +0x1ec
  material ops[0] = the destroy callback; +0x1f8 parent, +0x200 owner record, +0x218 type, +0x220 kind,
  +0x224 mask). The CHARACTERs of the 0x1468ae0 walk in 0x50d7f0 are a different list (scene at +0x88).
  init_SceneHeightMaps (0xfd7c50) is d3_Scenes[0].hmap: 64 height map pointers 0x3e0 apart, ending at
  0xfe7450 = d3_Scenes + 64 * 0x3e0 + 0xb0.
- Type debt: f3init.c gets a local `SCENEOBJ` view (pad + proto (+0x48), parent, owner, type, kind, mask);
  it still cannot include ch.h / d3.h / ai.h. Other units' functions are declared locally with their
  canonical names and prototypes (d3if_SelectScene, d3if_Reset, d3q_FreeNode, d3sm_Destroy, d3if_ShowTree,
  d3_LoadScene, d3_ForEachObject2) - the shared headers would drag the gv.h type conflicts in. 0x438d40
  still has no name (a d3_ForEachObject2 variant with a list argument); 0x501580 (gm.c) needs its Q4 work
  in ai.h.
- Open: 0x50d7f0 is still draft (214 code hunks). Tried and reverted (both made it WORSE): `int changed`
  instead of char (214 -> 217), `ai_FrameWait <= 0` instead of `< 1` (217 -> 218). The original stores
  DWORD 0/1 at [esp+0x10] where ours stores a BYTE at [esp+0x13], but the frame shift costs more than it
  gains - a match-deep frame job. 0x50dc30 is the near copy of it (231 code hunks).

## match-base 2026-10-25 (base-5 worker, shard 5/6, 3rd)

Shard 5/6, unit f3init.c. Check 2560/4664 unchanged; no grade fell.
- init_LoadObjectScene (0x50d7f0): draft -> readable, then escalated to match-deep (reason in its @deep
  tag). Body rewritten with the canonical engine names / full prototypes (declared locally: the unit cannot
  include ch.h / d3.h - d3.h needs the DirectX DDPIXELFORMAT and ch.h pulls d3_sm.h, whose prototype
  collides with gv.h's K&R d3sm_GetVertex): d3_CurrentScene, d3_Terrain, d3_FindObject, d3_VecEqual,
  d3_PointToWorld, d3_Origin, d2_ClearScreen, d2_ScreenW / d2_ScreenH, ch_GetSceneHMap, ch_SetPos,
  ch_PlaceOnGround, ch_FindDummy, d3t_FindFreeScene, tb_SetFunctionOff / tb_IsFunctionOff, err_Message,
  d3sm_Build, sv_SceneLoadProgress, sv_UpdateClock. The SCENEOBJ view gained scene (0x88) and transport
  (0x124); new INIT_D3SCENE view (hmap 0xb0, dirty 0x3d5) and a local D3VECTOR. d3_Origin replaces the
  0x5ebba4 literal; the walk is ch_List (0x1468ae0) to ch_List + 0x300, compared as int (the original's
  `jl`; a pointer compare is `jb`).
- Last-bytes gains kept: `!(ai_FrameWait > 0)` (the compare was `cmp [x],1; jge`, now `mov eax; test;
  jg`; 214 -> 201 code hunks) and the inner if/else polarity swap of the position / dummy block (the
  original's then-block is out of line; 201 -> 181). Reverted (worse): `int changed` (the dword store is
  the original's, but the frame shift costs code hunks: 181 -> 198; also 214 -> 218 before) and a separate
  0xfd7f75 dirty-flag view (no change). Final: 181 code hunks, 550/928 bytes, 138 asm lines; the rest is
  allocation / scheduling (details in the @deep tag).
- init_LoadBuildingScene (0x50dc30, the near copy): draft -> readable and escalated (reason in its @deep
  tag). Same rewrite (canonical names, INIT_D3SCENE, ch_List walk, tmp union); gains: `ai_FrameWait <= 0`
  in the new-slot branch (the original's `test; jle`; 220 -> 215 code hunks) and the same if/else polarity
  swap (215 -> 195; 1184 -> 750/1184 bytes, 156 -> 137 asm lines). `int changed` reverted again (the dword
  store is the original's, the frame shift costs hunks).
- init_BuildingKindName (0x50dba0): draft -> readable, escalated (close: 47/47 instructions, one
  12-instruction hunk - the inline strcpy form, the original folds the destination base with `sub edx,eax`;
  the direct expression in the call changed nothing; no `uv run spell` rule).
- init_SetDetailLevel (0x513590) renamed from unk_Unk513590 (its own Q2 still fails on ai_Unk633738).
- unk_Unk50e0d0 (0x50e0d0, the game opening) IN PROGRESS, still in the base queue: behaviour 160 -> 27 code
  hunks (196/200 instructions, 24 differing), readability still draft. Meaning fixes found (all kept):
  `gm_PlayerFactor((util_Random(0x122) & 0xffff) + 10, ai_OwnPlayer)` inlines the random call (the original
  evaluates the second argument first; 160 -> 62); the two 8-record loops call the real 8-argument
  cm_SendCreateCharacter - `(0, -1, -1, util_Random(8) + 0x10, -1, (char)util_Random(3) + 4, 0, 0)` and
  `(0xb, -1, -1, util_Random(8) + 0x10, -1, 0, 0x13, 0)` (kind 0 / 0xb, the 6th argument is the (char)
  random(3) + 4 of the discarded `k`); the cast must use the canonical parameter types (unsigned char /
  unsigned short), not int (the byte adds: 53 -> 27). gm_Unk5085f0 -> gm_PlayerFactor with its canonical
  `(int, unsigned char)` prototype (the unsigned char parameter removes the zero-extension: 56 -> 53).
  Left: `uv run spell` slots 50e10e (w1 / w2 frame slots), the reload 50e1a0 (w1 / w2 spilled to the stack where
  the original keeps them in esi / edi across ai_Unk4c3b10) and the `push edi` placement - allocation.
  Readability still to do: the cm_* network queue helpers (0x4c3b30 / 0x4c3b40 / 0x4c3b50 / 0x4c3680 /
  0x4c3fd0 / 0x4c4190 / 0x4c4880), ai_Unk5de634, ai_Unka68d2c, and the ROUGH marker.
- Renames: sv_Unk521c10 -> sv_SceneLoadProgress, sv_Unk5315c0 -> sv_UpdateClock, ch_Unk404a80 ->
  ch_PlaceOnGround (param unk -> force, Q6 moved to ch.h), unk_Unk513590 -> init_SetDetailLevel,
  ai_Unk75e8c0 -> sv_PanelFlag (the panel flag, sv.c's 0x75e8c0).
- Type debt: the D3VECTOR / INIT_D3SCENE views and the local canonical prototypes live in f3init.c; the
  CHARACTER arguments are cast through the SCENEOBJ view. The 0x38 field of CHARACTER (a D3VECTOR) still
  has no evidenced name, the body uses a commented cast.

## match-base 2026-10-25 (base-5 worker, shard 5/6, 4th)

Shard 5/6, unit f3init.c. Check 2564/4664 unchanged; no grade fell; unit differing bytes 15410 -> 15397.
- init_StartNewGame (0x50e0d0, was unk_Unk50e0d0): readable, escalated. Behaviour stays 27 code hunks, all read
  against the original 0x50e0d0..0x50e270: the meaning is equal (the first 16 character records with a start
  purse through the field block 0x1ac, the two judges kind 0xc and their religion fields, the 8 + 8 start
  characters, the four send groups). Left is allocation: the original keeps the two judge send slots in esi /
  edi across the wait group (its `push edi` sits mid-function at 0x50e143), ours spills the first to the stack
  and keeps the second in esi; the `uv run spell` slots / reload leads do not move it (the values are already int
  locals held across the calls). Q4 cleared with the INIT_CHAR view (id / kind / netId, 0x21c bytes); Q3 through
  INIT_START_CHARACTERS_END (0x772d40 = 0x770b80 + 16 * 0x21c); INIT_FIELD_MONEY / INIT_FIELD_RELIGION for the
  two field numbers; the `int (*)(8 args)` casts stay because cm.h's cm_SendCreateCharacter is void although
  the sender leaves its send slot in eax (the callers read it).
- 0x50e330 (was unk_Unk50e330, now init_CreateOpponents) IN PROGRESS, still in the base queue (not
  escalated): 142 -> 108 differing
  instructions, 389 -> 405 differing bytes. Meaning fixes, all read against the original:
  the inner skip loop is a do/while over `list[idx]` with its own counter (m reloaded each outer step) and its
  own `break` - the old draft used `continue` with a `cnt` that was never reset, so the structure was wrong;
  the +1/+3 index step is one hoisted local `step = sw != 0 ? 1 : 3` (the original computes it once, right
  after util_Random(2), and keeps it in edi); the byte change is clamped at BOTH ends - the upper clamp goes
  through a float compare against 252.0f written with the constant on the left (`252.0f < v ? 252.0f : v`,
  which is what makes VC6 emit `fld 252.0 / fcomp st(1)` instead of `fcom [252.0]`), the lower one is the
  `v < 0 ? 0 : v` idiom (the setl/dec/and guard after __ftol). Left: allocation - the original has m in ebp
  and prm in a stack slot ([esp+0x10], stored and reloaded), ours the reverse, and the tab / ids arrays sit
  0x20 lower in our frame (our ids starts where the original's tab[0] is; swapping the two declarations does
  not move them). Readability: Q1 init_CreateOpponents and Q4 cleared (the INIT_CHAR view gained skillBytes[5]
  at 0x80 and the loop reads pc->netId / pc->skillBytes[j]); the role is confirmed by the unit's own caller
  0x511400 (at 0x511547: `init_CreateOpponents(player slots - ai_NumPlayers, 8, tab)` with the free
  prototypes; init_StartNewGame is called at 0x511423 of the same function) and by
  0x75e160 = the seven option globals opponent_a .. opponent_g (0x5d4c70 / 0x5d5640 / 0x537040), renamed
  opponent_a. Q2 left on one global only: ai_Unk6f2440 (0x6f2440) - nonzero gives the first kind 6 character
  the starter buildings (0x511c70) and adds a random 5000 .. 9999 payment here; no other reader and no string
  names it, so it needs match-deep or a new lead.
- Renames this session (project-wide): cm_* network helpers ai_Unk4c3680 -> cm_FlushSendQueue, ai_Unk4c3b10 ->
  cm_GetSlotValue, ai_Unk4c3b30 -> cm_BeginSendGroup, ai_Unk4c3b40 -> cm_EndSendGroup, ai_Unk4c3b50 ->
  cm_SendGroupState, ai_Unk4c3fd0 -> cm_ReceiveMessages, ai_Unk4c4190 -> cm_ExecCommands, ai_Unk4c4880 ->
  cm_SendExSellObject (its handler 0x4c70b0 is "cm_ExSellObjekt"), ai_Unka68d2c -> cm_FieldObject (cm.h: "the
  object of the field block"), ai_Unk4c3d90 -> cm_AddFieldValue (cm.c "as 0x4c3d00, value by value"),
  ai_Unk4c49e0 -> cm_SendFieldValues (message 0x17, the value block), ai_Unk5de634 -> rand (the CRT rand: the
  LCG 0x343fd / 0x269ec3 with the seed 0x62e108, tr.c already calls rand()), unk_Unk50e0d0 ->
  init_StartNewGame.
- 0x50e5a0 (was unk_Unk50e5a0, now init_PlaceOffices): readable, escalated; 75 -> 3 differing asm lines.
  Its caller is 0x50e690 at 0x50e8a6: `init_PlaceOffices(0x1e - i, out + i, ids + i)` with out at the frame
  0x288 and ids at 0x168 - the out array there is only 0x38 bytes, so either the count or the array bound of
  that call is the next lead for 0x50e690 (its frame is 0x284 in ours against 0x2c0 = 60 bytes missing).
  Meaning / type fixes, all read against the original 0x50e5a0..0x50e690: `rec` is 0x18 bytes (amt.h's AMTREC,
  the amt_GetOffice output) - the old 0x20 made the whole frame 8 bytes too big; the loop ends with
  `while (--n != 0)` (the original reuses the dec's flags: `dec ebp; mov esi,edx; jne`, an extra `test` before);
  `out` / `ids` are the 3rd / 2nd argument (the original computes `ids - out` once); the second argument of
  amt_GetOffice is a byte (the original does `mov cl,[...]; push ecx`, no zero-extension - the parameter is
  amt.h's unsigned char); the return type is 16-bit (`unsigned short`: the original leaves eax dirty, no
  `and eax,0xffff` at the ret). Left: the random slot index - the original stores it 16-bit (`mov si,ax`) and
  zero-extends once before the loop, ours uses `mov esi,eax`; four spellings tried, all reverted (see the
  @deep tag). Q4 through the new INIT_OFFICE view (kindWord at 0x08: the original loads the dword, so
  AMTREC.kind's byte field would not reproduce it).
- Renames this session, continued: ai_Unk4e6f30 -> amt_GetOffice and ai_Unk4e7c40 -> amt_GetKindLevel (both
  already named in amt.c; f3init.c used the placeholder spellings). The amt_GetOffice rename exposes the three
  incompatible views (amt.h `unsigned char, AMTREC *`, cm.h K&R, unk_4a7140.h `int, void *`): the four
  (symbol, file) pairs are recorded in config/symbols/lint_baseline.txt (the first entries after the campaign);
  f3init.c now uses amt.h's parameter type with unk_4a7140.h's void * view, which reproduces the byte load.
  0x75e160 -> opponent_a (see above).

## match-base 2026-10-25 (base-5 worker, shard 5/6, 5th)

Shard 5/6, unit f3init.c. Check 2567/4664 unchanged; no grade fell; lint baseline +6 (type-debt views).
- init_CreateStartCharacters (0x50e690, was unk_Unk50e690): draft -> readable, then escalated (reason in its
  @deep tag). The whole original 0x50e690..0x50f0a1 was read; the meaning fixes (msg5 / purse / msg4 FP
  expressions, the 4-argument cm_SendRelations, gm_TitleNumber(gm_TitleGroup(job), 1), the two-branch 0x1cc
  block, the two scan breaks, the canonical cm_SendCreateCharacter casts) took 686 -> 568 differing
  instructions. Readability: INIT_CHAR gained female (0x09), age (0x0a), partner (0x5c), jobId (0x164),
  officeKind (0x166); INIT_OFFICE gained idWord (0x00); new INIT_KIND (AMTKIND's level at +8); the three
  amt_GetFirstOffice out records are three INIT_OFFICE locals (the original has three 0x18-byte arrays);
  INIT_CHARACTERS_END for the ai_Chars walk end. The frame is now 0x2cc vs the original's 0x2c0 (12 bytes).
- Facts: the 4th cm_SendCreateCharacter argument is the new character's AGE (the pair loop sends v->age - 18),
  not 0x10-ish constants only; t[0..29] are the ages (16 .. 37), t[0x1e..0x27] the job counts and
  t[0x28..0x31] the start money by the office kind's level (amt_GetKind -> AMTKIND.level at +8);
  0x4c47e0 MoveGoods sends the money; field 0xd gets random(2) + t[0x1e+level]; field 0x84 the msg5 float;
  field 0x1ac a random 1 .. 18 % of the purse (gm_PlayerFactor result) when the kind byte is 3; the
  office kind (0x166) 0x18 / 0x1a / 0x1b selects flags 0x80, 0x16 / 0x17 / 0x19 flags 0x40, 0x19 / 0x1a /
  0x1b the first office of kind 0x23 / 0x24 / 0x25.
- Renames: unk_Unk50e690 -> init_CreateStartCharacters, unk_Unk4fc6f0 -> gm_AgeCharacter (f3gm.c).
  Type debt: the ai_WorksForMaster(char *) / amt_GetKind(void *) / amt_GetFirstOffice(void *) views are in
  config/symbols/lint_baseline.txt (the amt.h / ai.c / hmt.h views differ).

## match-base 2026-10-25 (base-5 worker, shard 5/6, 6th)

Shard 5/6, unit f3init.c. Check 2569/4664 unchanged; ledger check --against 0 regressions; lint 0 new X; unit
differing bytes 14825 -> 14766; readable 13 -> 16 of 62.
- init_StockMarketStalls (0x50f340, was unk_Unk50f340): readable, escalated. The original 0x50f340..0x50f54f
  was read whole: the two markets' stalls (0x1db sale / 0x1dc purchase) get their city's import / export
  lists (ai_PlayerSlots + 0x2ac / 0x2cc, 16 words), the count, the capacity 4 and both send directions. The
  meaning fix: the third cm_SendExSellObject argument is an int (the original zero-extends the unsigned short
  goods id, `and eax,0xffff` at 0x50f42c) - f3init.c's local declaration went short -> int and got an
  @original tag (evidence: that call site; cm.h / unk_575860.c / unk_58bbd0.c keep the short view). 28 -> 12
  code hunks. Left (match-deep): the count loops' lea scratch register (ours ecx, original eax) and the
  counter init's position - allocation.
- init_Betrieb (0x50f570, was unk_Unk50f570): the error text "init_Betrieb(): costs are zero" (0x622fe0) is
  the function's own name. Meaning fixes: the cost sum weights each recipe's price by its weight (fimul of
  rw[-4]), the stall clamp is `if (amount < stock) n = amount; else n = stock` (two gm_ListStock calls), the
  recipe walk is `while (o != 0 && name[0] == '%')` then `if (o == 0) return`, `n <= 1` and the plain int
  division. The recipe words are unsigned short and compared as `!= 0xffff` (a 16-bit `cmp ax,0xffff`;
  `!= -1` gave a 32-bit compare). 186 -> 52 code hunks / 152 -> 38 differing instructions. Left (match-deep):
  the sum / i register swap (ours sum=ebx i=ebp, the original the reverse) and the error tail's placement.
- init_OpenBuilding (0x50f750, was unk_Unk50f750): readable, escalated; the only hunk left is the stack
  cleanup merge (ours add esp,0x14 after init_Betrieb, the original add esp,0x10 before the push + 4 after).
  139 -> 33 differing bytes. p[0x230] is an unsigned char, ai_Chars[b->owner][2] is indexed directly.
- Renames (project-wide): ai_Unk501d80 -> ai_BuildingCategory (the class 0..9 of ai_BTypes[type].kind, table
  0x501e14; ai.h prototype documented), ai_Unk4c51b0 -> cm_RequestChrMoveToUniverse (cm.c's own error text),
  unk_Unk4f15e0 / ai_Unk4f15e0 -> gv_SendGrave, ai_Unk501b50 -> gm_BuildingMainObject (f3gm.c's name),
  unk_Unk50f340 -> init_StockMarketStalls, unk_Unk50f570 -> init_Betrieb, unk_Unk50f750 -> init_OpenBuilding.
- Typed views: INIT_BLD gained owner (0x27, the character id) and player (0x65, the city index - the KONTOR
  office slot of 0x50d550 and every ai_PlayerSlots lookup); INIT_OBJ gained list (0x14). ai_Unk7e41ec
  (0x7e41ec = ai_PlayerSlots + 0x2ac) is gone, the slot's goods lists are written as
  `(unsigned short *)(ai_PlayerSlots[n] + 0x2ac / 0x2cc)`.
- Open: unk_Unk50f910 (0x50f910, the opening of a whole town) is IN PROGRESS, still in the base queue. Read so
  far against 0x50f910..0x50fc60: the class table is ONE local array initialised from immediates (there is no
  separate tab copy - `memcpy(cls, tab)` removed, 789 -> 761 differing lines), the character scan walks
  `(char *)ai_Chars + 2` and puts `p - 2` into pl[] (bound = INIT_CHARACTERS_END + 2), cnt[] is 0x22 ints,
  pl[8], ch2[16]. The frame is 0x40c vs the original's 0x404 (the local set still differs). NOT done: the
  fallback block has more buffers than the draft knows (vecA at esp+0x6c, vecB at 0x4c, and ai_Unk4c5ae0 gets
  esp+0x80 / esp+0x54 - two further arrays the draft merges into vecA / vecB), and the name / Q4 of the whole
  function. Next base session continues here.

## match-base 2026-10-26 (base-2 worker)
- Reference-only rename (no code change; build/link/ledger clean): ai_Unk4b3c10 -> ai_NewRandomMission
  (0x4b3c10, the call at line 2250) and ai_Unk4b5310 -> ai_ShowMissions (0x4b5310). Both defined in
  unk_4b39f0.c; see notes/units/unk_4b39f0.md.

## match-base 2026-10-26 (base-5 worker, shard 5/10)

Shard 5/10, unit f3init.c, function init_OpenTown (0x50f910, was unk_Unk50f910; the unit's first base item).
Check 2573/4664 unchanged; ledger check --against 0 regressions; lint 0 X. Differing instructions 761 -> 759
(2717 -> 2769 differing bytes: the fixes replace short byte compares with longer zero-extends and push
immediates - the gate's asm-line metric improved, the byte count rose).

Kept meaning fixes (all read against the original 0x50f910..0x51072f):
- The player-character scan stops at `ncl >= 8` (the original's `cmp esi,8; jge`), the class loop runs
  `i = 0xb .. 0xd` (`cmp ebp,0xd; jle`, not `< 0xe`), and the job match compares as a byte
  (`cl[0x223 + j] == (unsigned char)c[0x164]` - with mixed char/unsigned char VC6 promoted both sides and
  emitted a movsx + 32-bit cmp instead of the original's byte cmp).
- `cl` is an `unsigned char *` (ai_BTypes' record bytes): the original zero-extends every one of the
  +0x22f / +0x223 / +0x230 reads (`xor/mov cl`), the old `char *` sign-extended (movsx). The `cm_SendMessage3d`
  arguments now go through cl[...] instead of re-indexing ai_BTypes.
- The first long message (AIMSG 0xf8 at the cnt slot): the original does NOT memset it. It writes type 0x1b
  (+4), to = ai_PlayerChar+4 (+8), fromId -1 (+0xc), flags 2 (+0x36), arg[0] = *(b+1) (+0x58) and
  arg[1] = gm_EquipmentValue(b, (char)b[0x5c]) (+0x5c). The ROUGH memset+send is gone.
- The mission-mode message (type 0x4c) also fills fromId (+0xc = the same netId), nIds 0 (+0x10), unk14 0
  (+0x14), the 14-byte game clock ai_Time 0x770ae0 at +0x28..+0x35 (3 dwords + 1 word) and flags 1 (+0x36)
  after its 0xf8 memset.
- The greeting loop: `cm_SendRelations` takes FOUR arguments (the pre-pushed 0 at 0x510672 is the 4th
  argument: VC6 hoists the constant above the util_Random call and the 12-byte cleanup leaves it for the
  callee) and the sv_PumpNet result is discarded (the original does not store it).
- The function is `void` (caller 0x511550 ignores eax, the original's epilogue loads none) and the stale
  K&R `int ai_Unk50f910()` declaration is gone.
Readability: Q1 init_OpenTown (the role: the new town's buildings are opened - caller 0x511400 is the
new-game setup), Q6 rewritten (no ROUGH), Q3 through INIT_CHARACTERS_END + 2 and the new INIT_LAST_CHARACTER
(0x7d5d64 = the last record's start; the greeting loop's bound is its +2 = 0x7d5d66), and one Q2 callee
resolved: ai_Unk458a20 -> d3_PointToWorld (the canonical d3.h prototype, arguments cast; byte-neutral:
759 asm lines before and after).
Tried and REVERTED (worse under unitdiff): the 4-argument cm_SendRelations call as written (2 codegen
variants: 762-763 asm lines, the constants push order differs from the original's scheduling - it needs a
match-deep round together with the walker base), and the greeting walker base `(char *)ai_Chars + 2` with
p[0] / *(int *)(p+2) (762; the same scheduling story). The 3-argument cast form stays until then.
Last-bytes pass (`uv run spell`): the menu is rep-stos (memset forms - the three memsets already match), reload
entries 50fa18 / 50fa62 / 50fb14 / 50fd78 / 5103a1 / 5105db (the values are int locals held across calls,
no spellings left), dup-tail 50fc02 / 50fc9b / 50fcc4 / 50fd36 / 50fee9 / 50ff1c / 5100d6 / 51060d / 510686
(the original has two tails, the source writes both arms; polarity variants tried in the prior session),
width 50fdbc / 51043e (checked: both sides read the same 16-bit b+0x25 / money slot), push-pop 51015c
(early-return guard does not apply: the original keeps the same register), code 5100b0 / 5101b8 / 510293 /
5102c6 / 5103cb (no menu rule; read, the statements are equal - the stores differ in order / register).
What is left is allocation / frame shape: the frame is 0x40c vs 0x404 (8 bytes; every local shifted), the
long-message buffer at 0x7c (ours puts it elsewhere), the h/f/gm_BuildQuery constant pushed from edi instead
of as an immediate 0, and the cls / 0xb..0xd blocks' store order. match-deep: frame job. The readability
gaps left are the Q2 callee list (ai_Unk4c54a0, ai_Unk4c5540, ai_Unk4c5ae0, ai_Unk4f8ed0, ai_Unk501f60,
ai_Unk504290, ai_Unk509d30, ai_Unk537af0, ai_Unk50e690, ai_Unk53efe0 / 53f070 / 53f100 / 53f190,
ai_Unk624564, ai_Unk42ec00 = util_Random) and the 46 Q4 raw accesses (they want local typed views of the
AIMSG fields and the building/character records; the unit still cannot include ai.h / ch.h - gv.h conflict).
The function stays in the base queue: the readability work is base work, not an escalation reason.

### init_OpenTown continued (same session, base-5 shard 5/10)

After the Q4 typed views the function reached **draft + readable** (Q1..Q7 pass) and was ESCALATED to
match-deep (@deep; reason in the address comment: 751 differing instructions, frame 0x40c vs 0x404 - the
allocation / frame job). Differing instructions over the session: 761 -> 751; check 2573/4664 unchanged; no
grade fell.
Q4 clearing (all byte-neutral except the last bullet, which gained 2): INIT_BLD gained masterId (0x25) and
level (0x5c, unsigned char - the original reads it as a byte and the worth 0x509b10 is weighted by it);
INIT_CHAR / INIT_BLD casts replaced the *(short*)/(int*) accesses (the building scan's `*(short *)c` was a
movsx where the original zero-extends - now INIT_CHAR.id); new INIT_AIMSG (0xf8, type 0x04, to 0x08, fromId
0x0c, nIds 0x10, reserved14 0x14, INIT_TIME time 0x28, flags 0x36, arg[11] 0x58) and INIT_TIME (14 bytes,
packed) views; the mission message copies the clock as `m2.time = *clock` (the original's 3 dwords + 1 word).
The one un-evidenced global 0x624564 is now init_AiTown: .data initial 1, no writer in the exe, it gates the
class-1 stocking and the computer players' mail (the role is the evidence); if a later session finds a
writer, rename after that.
Renames this session (project-wide, all references): ai_Unk4c54a0 -> cm_SendOwnership (message 0x38: the
building changes its person from data[4] to data[8], handler 0x4caaa0), ai_Unk4c5540 -> cm_SendBoss (0x39:
the building gets the character as its boss), ai_Unk4c5ae0 -> cm_RequestCreateGeb (cm.h's own name),
unk_Unk4f8ed0 / ai_Unk4f8ed0 -> ord_UpdateStatistics, ai_Unk501f60 -> gm_NearestFreeBuilding (the f3gm.c
definition's name), gm_Unk504290 / ai_Unk504290 -> gm_FirstTitleNumber, gm_Unk509d30 / ai_Unk509d30 ->
gm_BuildingKindValue (a readability side effect: the function itself became readable), main_Unk537af0 /
ai_Unk537af0 -> main_SetLoadProgress, he_Unk53efe0 / 53f070 / 53f100 / 53f190 -> he_PostEvent6d / 6b / 7a /
2b, ai_Unk50e690 -> init_CreateStartCharacters (the call keeps its pushed 0 through a cast, @original
evidence 0x510494), ai_Unk624564 -> init_AiTown (data), ai_Unk42ec00 -> util_Random (f3init.c only; the
project-wide spelling is still cm.h's, a separate rename). Local prototypes added in f3init.c for the cm /
he / f3gm / main / unk_58bbd0 callees (Q7).

### unk_Unk510770 started (same session, base-5 shard 5/10)

0x510770 (the group-scene loader, still in the base queue, IN PROGRESS): the name buffer `nm` is 0x40 bytes,
not 0x80 (the original's frame is 0x8c4 = list[0x221] 0x884 + nm 0x40; the parameter sits at [esp+0x8cc]),
194 -> 179 differing instructions. Left for the next session: the bounding-box block writes through the
GLOBAL expression `((float *)(ai_Bauplaetze + ai_NBauplaetze * 0x60))[...]`, not through a local pointer -
the original reloads 0xf812e0 / 0xf812e4 (`mov eax,ds:X` twice) for every store and every compare (6
instructions per store, ours 2 with `bb` in ecx); the min-side compares are written with the fields on the
left (`pf[-2] < bb[0x10]`), which is what makes the original `fld pf; fld dest; fcompp`. Then Q1-Q7 (the
whole function is a ROUGH draft, ai_Unk43c570 / 43ca20 / 449960 / 43b790 / 43b860 / 438d40 / 454510 are the
unnamed engine callees).

## match-base 2026-10-26 (base-3 worker, shard 3/6)

f3init.c includes d3_light.h; the ai_Unk4538a0 K&R prototype is gone and the call uses d3_LightInvalidate. No byte change.

## match-base 2026-10-26 (base-5 worker, shard 5/6, 7th)

Shard 5/6, unit f3init.c. Check 2603/4664 unchanged; ledger check --against 0 regressions; ledger check 0 errors; lint 0 X;
unit readable 16 -> 21 of 62; grade behaviour + readable 1449 -> 1454 (byte + readable 1394 -> 1397);
unit differing bytes 15397 at the session start -> 14256.
- init_LoadSiteScene (0x510770, was unk_Unk510770): behaviour + readable. Findings read against the whole
  original 0x510770..0x510ad0:
  * the "list" is INIT_SITELIST { int n; D3OBJECT *obj[0x200]; char key[0x80]; } and the strcpy destination is
    list.key: the original's `lea edx,[esp+0x844]` is computed BEFORE the pushes, so the key is at esp+0x854 =
    list + 0x804 - the same bytes the collector searches (r + 0x201 ints). A separate `char nm[0x40]` (the old
    draft) was wrong.
  * the six box stores and the six min/max statements go through ai_Bauplaetze / ai_NBauplaetze *directly*:
    a store through the global pointer makes VC6 reload both globals for every following statement (6
    instructions per store, `mov eax,ds:count; lea edx,[eax+eax*2]; mov eax,ds:arr; shl edx,5; mov [ecx+eax+off]`).
    A cached `float *bb` (the old draft) gives 2. The min/max are the project's min() / max() macros with the
    corner value first: `min(v[-2], ai_Bauplaetze[n].x0)` gives the `fld corner; fld dest; fcompp; and eax,0x4100;
    jne; fld corner; jmp; fld dest; fstp dest` select form, `x < corner ? ... : ...` gives `fcomp mem` (worse).
  * d3_MatIdentity(&cam) + `((D3MESHOPS *)o->info->mesh)->GetBox(o, o->info, &cam)`: the second lea is [esp+0x14]
    because the d3_MatIdentity push is not cleaned up yet (add esp,0x18 later); it is the same matrix. The 8
    corners are a D3BOXVERT[8] (0x50 each), the loop walks a float pointer at box + 0x16 floats (+= 0x14 floats).
  * the site search is the original's odd do/while with `if (i < ai_NBauplaetze) goto place; break;` (the draft
    had it right, my first cleanup into a for loop cost 15 hunks), and the place loop is
    d3_SpawnObject / d3_LinkTop / d3_SetParent / d3_SetObjectPos / d3_SetObjectRot / d3if_Delete with the child
    walk reading p->next before the call.
  Remaining: fpu 26, order 6, fill 4 - allocation only.
- init_CollectSiteObject (0x510730, was unk_Unk510730): byte + readable (renamed only; the body is the same).
- init_InitTown (0x510ad0, was unk_Unk510ad0): behaviour + readable. Meaning fixes: the bank character's name is
  `strcpy(ch->name, "STADT")` - the draft's `*(int *)(p + 0x18) = 0x54; p[0x1a] = 0;` was the inlined copy of the
  6-byte literal ("STAD", then the word "T\0"), the code at 0x510c5f reads the literal from 0x623000; the netId is
  INIT_CHAR.netId (0x04), the character id INIT_CHAR.id (a word, zero-extended); the shadow map is
  `d3_DefaultScene->hmap = (struct D3HMAP *)d3sm_Create(d3_Terrain, 0x80 / 0x100, 2)` and the terrain's size is a
  new INIT_TERRAIN view (d3_sm.h's D3TERRAIN.size, d3_sm.h is not includable: gv.h's d3sm_GetVertex).
  Q3: INIT_PLAYER_SLOTS_END + the new INIT_SLOT_CURRENCY_END (0x7e4b64) for the 3-slot currency walk
  (init_SlotCurrency at 0x7e4288 = ai_PlayerSlots[i] + 0x54). Remaining: regs 12.
- init_StartNetworkGame (0x510e50, was unk_Unk510e50): IN PROGRESS, still in the base queue. 293 -> 183 code
  hunks (order 16, slots 10). Done this session:
  * the mission-mode tail condition is `ai_MissionMode > -1 && ai_MissionMode <= 5` (the draft's `>= 0 && < 6`
    compiles to test/jl + cmp 6/jge; the original is cmp 0xff/jle + cmp 5/jg): 293 -> 186 code hunks.
  * Q3: INIT_PLAYER_SLOTS_END; Q4: the slot handle is an INIT_CHAR * (ch->netId at +4); Q7: prototypes for
    d3t_FindFreeScene, d2_ShowPicture, d2_DrawAll, d2_ClearSurface, gm_RandomMarketShake (an int parameter the
    definition in f3gm.c lacks - @original), ls_SendGame, ai_NewRandomMission, ai_ShowMissions,
    cm_SetSpeedLevel, cm_SendMessage0c (int return, cm.c defines void - @original), sv_NetworkSyncWait,
    ls_ReceiveGame, init_SetupNewGame, init_FillMarkets, d2_SetViewEnabled.
  * names: init_StartNetworkGame, init_ShowStatus (0x510e20), init_SetupNewGame (0x511400), init_LocalData
    (0x511ad0, its own error string), init_FillMarkets (0x511bc0), cm_SendMessage0c, cm_SetSpeedLevel,
    sv_NetworkSyncWait (0x52fc10), ls_ReceiveGame (0x520b80), cm_SendMessage10 (0x4c45c0).
  * the status texts are init_StatusFormat / init_StatusSendGame / init_StatusReceiveGame / init_StatusSetupGame /
    init_StatusLocalData (0x14414a0 / 90 / 94 / 98 / 9c) and the panel form init_StatusForm (0x75df00); the two
    names are init_PlayerFirstName / init_PlayerFamilyName (0x75e5aa / 0x75e5ca).
  Open (next base session): Q2 on the five registration values 0x6f2444 and 0x75e5a2 / a3 / a4 / a8 - no writer
  and no string names them, the only evidence is the argument list of cm_SendMessage0c; the frame is 0x310 vs the
  original's 0x384 (0x74 = 116 bytes of missing stack staging below ini at esp+0x8c, every local shifted), the
  remaining 183 code hunks are read only in part. The `strcpy(p, "\game.INI")` literal is wrong in the draft
  (`"\game.INI"` is a 9-byte string, the original copies 10 bytes: dword + dword + word) - fixing the literal to a
  real backslash was tried last and the sed did not apply (verify with asmdiff at 0x510f48..0x510f60).
  Meaning leads still open: the mission-money select (`ai_Unk6f2444 == 0 ? 2000 : 500000` compiles branchlessly
  with neg/sbb/and, the original branches: cmp eax,edi / je / push 0x7a120 / jmp / push 0x7d0), the
  cm_SendMessage0c argument loads (ours movsx / mov ax, the original movzx ax,byte + separate byte loads) and the
  arg staging slot [esp+0x10] vs [esp+0x18].
- Type debt: INIT_SITELIST, a local G15BK (rd.h is not includable in this unit: gv.h's d3sm_GetVertex collides -
  the include was tried and reverted), INIT_TERRAIN, the min/max macros (ai.h cannot be included).
- Renames this session (project-wide): see the report; the reference-only ones touched cm.c, ls.c, f3gm.c, gv.c,
  amt.c, sv.c, hst.c, main.c and unk_58bbd0.c with build/lint/unitdiff/grade clean.

## match-base 2026-10-26 (base-5 worker, shard 5/6, 8th)

Shard 5/6, unit f3init.c. Check 2610/4664 before and after; `ledger check --against` 0 regressions; lint 0 X; ledger 0
errors; unit differing bytes 14256 -> 14132.
- init_StartNetworkGame (0x510e50, was unk_Unk510e50): behaviour + readable, then escalated (reason in its
  @deep tag: the frame 0x310 vs 0x384 = 0x74 bytes of locals missing between the scalars and ini that no
  statement reads, the strrchr stack-cleanup scheduling, and the branchy start-money select). Meaning fix found:
  the game.INI literal is `"\game.INI"` (10 bytes: the original copies dword + dword + word; the draft's
  `"\game.INI"` is 9 bytes and gave a byte store) - 183 -> 87 code hunks. Readability: the player profile is the
  new INIT_PLAYER view at 0x75e5a0 (status 0x02, job 0x03, coat 0x04 int, sex 0x08, faith 0x09, names
  0x0a / 0x2a; main_Unk5361d0 / main_Unk53b840 / 0x57bc30 write it), which also fixed the old
  `unsigned short + >> 8` pair for the two bytes.
- init_SetupNewGame (0x511400): behaviour + readable, escalated (frame: the original uses a frame pointer
  `push ebp / mov ebp,esp / ebp-relative locals` and ours does not - no statement accounts for it). Readability:
  net_Conn.s (0x1460ae0, -1 = no network game), ai_PlayerSlots[n][0x48] (the city's MaxPlayer, 0x513a20 writes
  it), srand (0x5de62a stores the LCG seed 0x62e108), init_OpenTown, INIT_CHARACTERS_END + 2 for 0x7d5f82,
  INIT_BLD.objList / INIT_OBJ.objId. Lesson: an **undefined** function cannot be renamed - the link keeps a
  renamed prototype unresolved ("1 functions not placed: unresolved _main_GiveStartHouse" for 0x534040, which
  main.c has not drafted); a placeholder name for an undrafted address is the only linkable spelling.
- init_TownBuildingHook (0x511700, was unk_Unk511700): readable + behaviour (47 code hunks / 68 bytes left),
  escalated. The return type is **char** (the original sets only al at 0x511763 / 0x511789): 50 -> 47 hunks.
  Q4 through SCENEOBJ.proto, Q3 through INIT_PROTO_MASK. Left: the two flag blocks swapped (the original keeps
  its `_TOR_` arm inline) and the arm registers; three spellings tried, all identical code.
- init_NameBuildings (0x511890, was unk_Unk511890): readable + behaviour (178 code hunks), escalated. **Frame
  finding: the original has TWO 0x200-byte error buffers**, one per "Building name too long" format (0x120..0x320
  for the arm that calls err_Message, 0x620..0x810 for the arm that drops the text). With the second buffer the
  frame is 0x810 and every anchor matches (one 0x20, names 0x320, the 0x124 / 0x12c sprintf pair, the dead
  format at 0x620). Left: the original pushes ebx/ebp/edi mid-function (0x5118ee) where ours pushes them in the
  prologue. Also: ai_Unk4222b0 -> err_Message calls (the unit already had the err.c prototype).
- init_LocalData (0x511ad0): readable + behaviour (102 code hunks), escalated. Q3 (INIT_PLAYER_SLOTS_END,
  INIT_CHARACTERS_END + 0xc, INIT_CHARACTERS_END) and Q7 with the real `void gm_ResetStock(void)` prototype - the
  prototype is also the behaviour fix of the tail: the original ends in a tail jump `jmp 0x50cc60`, which VC6
  only emits with the prototype. Left: the judge scan's registers (the original loads only ai_JudgeProtestant
  into edi before the loop, keeps n in edx and tests p[-10] in memory) and the kind 0xb walk's collector.
- init_FillMarkets (0x511bc0): byte + readable goal reached as **behaviour + readable** (fpu 21 + fill 8 only).
  The meaning was lost in the draft: the amount is `(util_RandomF() + 0.5)` for the goods kinds 0x17 / 0x25 and
  `(util_RandomF() + 0.2)` for the others, times the record's demand float at +0x20 (`fmul DWORD [esi+0x20]`
  then __ftol). The stock table is a **block per player** of 0x2a90 bytes with four header ints: `p * 0x2a80`
  (a plain record array) is wrong, the records start at block + 0x10 - the new INIT_STOCK / INIT_STOCKBLOCK
  views (fields item, amount, demand, mean, price from hst.c's G12STOCK and f3gm.c's 0x4ffa10).
- init_BuildingSceneUniverse (0x511e60): readable + behaviour (35 code hunks), escalated. Fixes: init_SceneName
  (was ai_Unkfd7ba0), the `t` local removed so `*b` is re-read where the original re-reads it, and `k` as
  unsigned with a `(unsigned char)` cast (the original: `xor ebx,ebx / mov bl,BYTE / mov esi,ebx` and a byte
  compare against 0xff): 144 -> 54 differing bytes. Left: the inlined strcpy's registers.
- unk_Unk511f20 (0x511f20): readability advanced (Q3 and Q7 cleared, three callees named), escalated - a
  1450-byte far draft (777 code hunks) whose record walks are summarised. Q2 / Q4 blockers recorded in the tag.
- Renames this session (project-wide): ai_Unk50b010 / gm_Unk50b010 -> gm_FirstBuilding (28 refs, am.h / am.c /
  cm.c / f3gm.c / hst.c / f3init.c), ai_Unk5de62a -> srand (f3init only, stdlib.h), ai_Unk43dbc0 ->
  d3_InflateObject (f3init; cm.c keeps the ai_ alias), ai_Unk4fe940 -> gm_IsBuildingKind, ai_Unk411eb0 ->
  d3t_GetBoundingSphere (f3init; cm.h / cm.c keep the ai_ alias - the shared-header view differs), ai_Unk534040
  dropped (see the lesson above), ai_Unk6f2444 -> main_MoneyCheat (the "MO_MONEY" command-line switch, main.c
  4975), ai_Unk6f2430 -> main_StartHouseCheat, ai_Unkfd7ba0 -> init_SceneName, ai_Unk7d6a70 ->
  init_StockBlocks (0x7d6a60 block view; hst.c / f3gm.c keep ai_Unk7d6a70 - their stride is the record array),
  unk_Unk511700 -> init_TownBuildingHook, unk_Unk511890 -> init_NameBuildings, unk_Unk5124f0 (not renamed).
- Type debt added: INIT_PLAYER (0x75e5a0), INIT_STOCK / INIT_STOCKBLOCK (0x7d6a60 / 0x7d6a70), net_Conn (0x1460ae0).
- unk_Unk5124f0 -> init_LoadShopScript (0x5124f0): Q1/Q3/Q7 cleared, escalated. The name is the function's own
  format string ("Einkauf_%s.esc"); INIT_WINDOWS_END = 0x760420 (one past the 0x20-entry window table 0x7603a0);
  init_ShopScript (was ai_Unk61ae00) is the handle of the running shopping script (started here, stopped by
  0x5136b0). Left: the window record w (0x7603a0 entry) and the character fields +0x28 / +0x30 are not read yet
  - the entry compared with ai_Unk50a810(b) but dereferenced as a pointer is the suspicious spot (Q4 / Q5 / 4 Q2).
- unk_Unk5126a0 -> init_SoundObjectHook (0x5126a0): readable + behaviour (8 code hunks left), escalated. The
  three callees were named from d3_snd.c / f3snd.h: ai_Unk417710 -> d3snd_Attach, ai_Unk417cf0 -> d3snd_Play,
  ai_Unk426030 -> sb_GetSample (declare it with d3_snd.c's own prototype or lint reports an X conflict);
  SCENEOBJ gained soundRange (0x68) next to mask (0x224). 48 -> 8 code hunks; left: the frame is 0x108 against
  0x104 (one local on the stack too many) and the esi / edi swap of loop / the sample pointer.
- Session end state: check 2610/4664 (unchanged), ledger check --against 0 regressions, ledger 0 errors, lint 0 X,
  baseline --check 29 differences, all rises (6 readability draft -> readable, 8 level -> deep, the renames
  below); unit differing bytes 14256 -> 14080. Next base session: `uv run ledger grade next` (f3init.c still has
  many drafts: the town scene 0x5124f0 .. 0x5128d0, 0x513050, 0x513a20, 0x5112d0, 0x511400 ..).
- Renames, continued: unk_Unk5124f0 -> init_LoadShopScript, ai_Unk61ae00 -> init_ShopScript, unk_Unk5126a0 ->
  init_SoundObjectHook, ai_Unk417710 -> d3snd_Attach, ai_Unk417cf0 -> d3snd_Play, ai_Unk426030 -> sb_GetSample.

## match-base 2026-10-27 (solo session on main)

- 0x5127c0 -> init_SoundObjectFreeHook: **byte** + readable. The call is `d3snd_Free(o->sound)`, not a no-argument
  call: 0x5127c0 frees the sound of a "sfx_" object (the address comment said "starts it"). SCENEOBJ gained
  `sound` (0x20c, D3OBJX.sound); 0x4179d0 is d3snd_Free (d3_engine.h), the K&R `ai_Unk4179d0()` prototype is gone.
- 0x5127f0 -> init_PlayTownSound: **byte** + readable. The call is the 7-argument `sb_PlaySample(0, name, 0, 0x3f,
  (int)(volume * init_SoundVolume), 1, delay)`: the original pushes the sample table, 0x3f, 0, the name, 0 and the
  volume `fild v1 / fmul ds:0x61adfc / __ftol`. 0x61adfc is the 0.7f global of this unit's .data (read from the exe:
  0x3333333f) - new extern `init_SoundVolume` (data debt: still not defined in C). 0x76fd80 / 0x6f1c34 ->
  init_TownSounds / init_TownSoundCount.
- 0x5128d0 -> init_SetSeasonTextures: readable, escalated (511 -> 82 differing instructions). The original is four
  near copies of scan + update, and it names d3_Terrain at every mention (the global is reloaded before each
  strstr / tex_FindFile call) instead of keeping a local; spelling it out was the 193 -> 82 step. 0x45f9f0 =
  tex_FindFile, 0x442210 = d3fl_ReplaceTexture, gm_ClockSeason prototyped. Left (allocation / loop shape): VC6
  rotates our `while (i < 0x200)` (i = 0) to a bottom i test, the original keeps `cmp esi,0x200 / jge` at the top
  with the found test at the bottom and an inline ret (512900..512945), x4; plus the [eax+esi+0x1a64] /
  [esi+eax+0x1a64] operand order in each update loop.
- 0x512e40 -> init_TextureSetHook: readable, escalated (9 -> 6 differing instructions). The 2nd argument is
  `mesh + 0xf4` (the draft dereferenced it), the return is char (`mov al,1`), SCENEOBJ.mesh (0x1ec) replaced the raw
  casts. ai_Unk446f30 -> d3if_SelectTextureSet with ecmd.c's `void *list` prototype (the D3SURF * form made lint
  report a cross-TU X); ai_Unk6f1c24 -> init_TextureSet (cm.h / cm.c; written from gm_ClockSeason). Left: the
  original's dead `cmp ecx,0x3` between `add edx,0xf4` and the byte load, and our byte load before the pushes.
- 0x512f30 -> init_SetCharacterAnimations: readable, escalated (97 asm, ours 272 vs 273 bytes). Meaning read whole
  (class lookup in the 19 records at 0x61ae08, the ai_Chars + 0x184 walk in 0x21c steps, the q[-6] / group test,
  the 18-argument ch_PreloadAnims). New INIT_ANIMSET view (0x6c4: kind + 36 name slots), INIT_CHAR.group (0x2c),
  INIT_BLD.id; 0x404200 -> ch_PreloadAnims (varargs prototype), 0x52b2a0 -> sv_CollectGroupCharacters (renamed in
  sv.c), the table end is `(char *)ai_Chars + 0x184 + 0x300 * 0x21c`. Left: b in ebx vs the original's ebp and the
  class-byte address in edi vs edx (prologue 512f30, loop 512f9e).
- Renames: ai_Unk61ae08 -> init_ClassAnims, ai_Unk6f1c24 -> init_TextureSet (cm.h), ai_Unk6f1c34 / ai_Unk76fd80 ->
  init_TownSoundCount / init_TownSounds, ai_Unk5127c0 / 0x5128d0 / 0x512e40 / 0x512f30 -> init_SoundObjectFreeHook /
  init_SetSeasonTextures / init_TextureSetHook / init_SetCharacterAnimations, ai_Unk446f30 -> d3if_SelectTextureSet
  (f3init.c), ai_Unk404200 -> ch_PreloadAnims, ai_Unk52b2a0 -> sv_CollectGroupCharacters (sv.c).

## match-base 2026-10-28 (solo session on main)

Check 2661/4664 -> 2662/4664 matched; ledger check 0 errors; lint 0 X. Unit f3init.c, 4 items:
- **0x513050 -> init_ShowBuildingScene, readable, -> match-deep** (122 -> 94 differing instructions).
  Meaning fixed against 0x513050..0x5133f6: the room record is read through a plain `unsigned short *o`
  whose proto is loaded once into `proto` (the draft's `char u` temp made VC6 sign-extend the table byte
  after it had merged the two `ai_MsgNoOp()` tails), the room record's own id is at +2
  (`((INIT_OBJ *)o)->objId`; the draft read +4), the ai_Chars model walk re-reads `*q` before each use
  (`q` = `&((INIT_CHAR *)ai_Chars)->model`, 0x21c steps), and both loop ends are signed int compares
  (the draft's `unsigned` spelling gave `jb`). Views: INIT_BLD gained `plants` (0x71), INIT_CHAR gained
  `bld` (0x16c) and `model` (0x184), SCENEOBJ gained `group` (0x2c) / `scriptGroup` (0x30), and the new
  **INIT_PANELCHAR** (packed, `next` at 0x3b) carries the info-panel character chain. `sv_PanelShown` is
  an `INIT_OBJ *` here. Left (allocation / scheduler shape, reason in the tag): VC6 merges the valid
  branch's `ai_MsgNoOp()` (a bare `ret` at 0x4b70f0) with the invalid branch's; `d3if_ShowTree(d,
  ai_GfxDetail != 0)` is `setne cl / push ecx` where the original materialises the char bool with
  test / jne + push 0 / push 1; the redundant `t->scene == d3_DefaultScene` re-test of the else arm is
  folded for every cast form tried.
- **0x513400 -> init_FinishBuildingScene, byte + readable.** The five contact_EINKAUF finds are sequential
  `if (d == 0) d = ...` lookups followed by one `if (d != 0)` block whose else is the class 5 shop script;
  that source form (not the nested draft with `goto rest`) is byte-identical. The second parameter is the
  room record (`unsigned short *o`, init_LoadShopScript's own type), not an id.
- **0x513590 -> init_SetDetailLevel, readable, -> match-deep** (81 -> 60). **init_Detail (0x61adf8) is an
  unsigned char** - the original stores `mov BYTE`, and ls.c 0x51d2d0 loads it as a byte too, so its
  `extern` is unsigned char now. The size is `d3fl_DetailScale << d3fl_DetailSizeShift` with no `& 0x1f`,
  and the final test compares the old level computed once (`oldlvl = (old >> 2) & 7`) with the new one
  (the draft's xor/test form was the compiler's take on recomputing it). **d3fl_SetShift has its canonical
  prototype** (`void d3fl_SetShift(D3FLMAP *, unsigned char, char)`) in cm.h / f3init.c; cm.c casts the map.
- **0x513a20 -> init_LoadCityINI, readability + -> match-deep.** Q2 (`unk_Unk513970 ->
  init_ParseNumberList`), Q5 (the ROUGH marker replaced by the far-draft state), Q7
  (`gm_FindBuildingKind`, 0x502060). **Q4 is open and needs the disassembly first:** the draft's six law
  lists (0x1dc, 0x20c, 0x22c, 0x25c, 0x274, 0x290) overlap (finLaw 0x20c + 0x30 crosses penalLaw 0x22c),
  so an INIT_CITY view built from the draft's offsets would cement a wrong layout.
- Renames (project-wide): ch_Unk403620 -> ch_PlaceTransport, unk_Unk4fb5a0 -> gm_SelectModelVariant,
  gm_Unk502e30 -> gm_ShowBuildingObjects, unk_Unk512de0 -> init_PlantTextureSetHook, unk_Unk512780 ->
  init_GateObjectHook, unk_Unk511f20 -> init_SyncSceneObjects, unk_Unk513400 -> init_FinishBuildingScene,
  unk_Unk512840 -> init_PlayGraveyardSound, unk_Unk512e80 -> init_BuildingTextureSetHook, unk_Unk513050 ->
  init_ShowBuildingScene, unk_Unk513970 -> init_ParseNumberList, ai_Unk76fdc0 -> ai_SceneBuilding,
  ai_Unk76fa84 -> ai_SceneObject, ai_Unk6334a5 / d3_Unk6334a5 -> d3_FogByDistance, ai_Unk5f1294 ->
  ch_CurrentGroup, ai_Unk5f8430 -> evt_NoGroup, ai_Unk5fbd04 -> d3o_FogScale, ai_Unk5fcc70 ->
  d3_LightKeyBudgetVisible, ai_Unk61adf8 -> init_Detail, sv_Unk525940 -> sv_UpdateSkyColors.
- VC6 lessons: (1) **a byte store proves the variable is a char** - `mov BYTE ds:X,4` in one function and
  a `mov DL,BYTE ds:X` in another rule out `int` even where a dword-sized slot exists. (2) **A packed
  struct member can stop a load CSE across an unrelated store**: reading the value once into a local
  (`(proto = *o)`) restored the original's `mov ax` / `and eax,0xffff` reuse. (3) The sequential
  `if (d == 0) d = find(...)` chain, not nested ifs, is what puts the not-found block after the found
  block (and can be the byte-identical form). (4) `int x; x = call; if (x != 0)` vs the declarator with
  the field read in the condition changes which block VC6 emits inline.

## play 2026-10-28
- `INIT_SCENE_HEIGHTMAP_END` was `&d3_AnimHead + 0x10` (correct only at the original addresses). The
  full link places `d3_AnimHead` 0x5400 further from `init_SceneHeightMaps`, so `init_FreeScenes`
  walked 341 height-map slots instead of 0x40 and freed scene data as height maps (`d3sm_Destroy` on
  string bytes). The macro is now `init_SceneHeightMaps + 0x40 * 0x3e0`; the code keeps its `byte`
  grade (both forms are relocations).
