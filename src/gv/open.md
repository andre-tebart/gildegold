# gv.c (new, layout 2026-10-22)

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** first; below it the findings of each session, one section per author.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): the graveyard module (gv_) with avatars and effect slots
- Boundaries: open: new unit (layout 2026-10-22 session 16, name from the tags "gv_AllocGraveyardField" / "gv_Alloc:_all_graves" / "gv_Alloc:_all_zombies" at 0x61120c..0x611240, renamed from unk_4ed170.c): the avatar table 0x60f310 (32 records of 0xf4 bytes, "neVmMaven"), the effect slots 0x6f1008 (8 of 0x170 bytes; "kugel" objects, "Explosion" 0x4ee360), the graveyard building (kind 0x1d: records +0x65, lists of graves / zombies, tables 0x5ee054..0x5ee0b4, the 0x1d-record table 0x611258, the 0x2d-byte records 0x91f700 and pairs 0x91b6e0, "gs_AddBeweis" 0x611698). End found (session 17): the order handlers start at 0x4f4e20 (table 0x611768 lists them, 0x4f4e20 .. 0x4f8140), and 0x4f5d50 / 0x4f5f00 match only when 0x4f2870 is not defined above them in the same file (VC6 knows a callee defined earlier in the TU does not clobber m[0]), so the handlers moved to ord.c. All functions drafted (layout 2026-10-23: 0x4ef090, 0x4f2330 / 2440 / 24a0, 0x4f30a0 .. 0x4f3580, 0x4f41b0, 0x4f4460, 0x4f48f0 added, ROUGH; 0x4f2440, 0x4f3580, 0x4f48f0 match). Open because it is incomplete and the effect slots / avatars / graveyard may be several modules sharing one .data block
- Range then: 0x4ed170..0x4f4e20
- Was: tag "gv_"

## Open work
- Drafted in layout 2026-10-23 (ROUGH, all compile; see the section below): 0x4ef090, 0x4f2330 / 0x4f2440 / 0x4f24a0, 0x4f30a0 / 0x4f31d0 / 0x4f3270 / 0x4f3580, 0x4f41b0, 0x4f4460, 0x4f48f0. Matched: 0x4f2440, 0x4f3580, 0x4f48f0. Near / open: 0x4f2330 and 0x4f24a0 (the clamp of x into 10000 .. 1e6 / 1e5 compiles to two `fcomp` of the same constant: three ternary shapes tried, none gives the original; the rest is the same), 0x4f30a0 / 0x4f31d0 / 0x4f3270 (the by-value record), 0x4f41b0 (the 0x2010-byte frame), 0x4f4460 (message fields only roughly placed).
- Rough drafts (structure only, many floats / __ftol arguments read from Ghidra): 0x4edf80, 0x4ee360 (explosion), 0x4ef9c0, 0x4efc10, 0x4eff50, 0x4f0240, 0x4f0490, 0x4f0770, 0x4f0ed0, 0x4f1200, 0x4f1de0, 0x4f28d0, 0x4f2b30, 0x4f2e30, 0x4f3a30 / 3b40 / 3c50 / 3dd0, 0x4f4970 (level-up), 0x4f4d30.
- Near misses: 0x4ed170 (16 B: the `fstp` placement in the float loop), 0x4ef260 (1 B, identical instructions), 0x4f2660 (9 B, one more fmul), 0x4f0430 (9 B), 0x4f0e80 (8 B), 0x4f01d0 (11 B), 0x4f2540 / 0x4f25d0 (100 B), 0x4f2720. Data not defined in C: the avatar table 0x60f310 (unk_Avatars), the 0x1d-record table 0x611258 (unk_Tab611258), the tables 0x5ee054 .. 0x5ee0f8, the bss.

## layout 2026-10-22 session 16
- Layout: .data block 0x60f310 (avatar table, 32 x 0xf4: +0 id (a 4-char tag as int), +4 / +0x14 names, +0x2c.. floats (set to the one before * 0.001 by 0x4ed170), +0xf2 used byte), then strings 0x611190 ("Avatar >%s< not found in savegame" / "Inconsisten Avatar count"), "kugel" 0x611204, "gv_Alloc:_all_zombies" 0x61120c, "gv_Alloc:_all_graves" 0x611224, "gv_AllocGraveyardField" 0x61123c, the 0x1d-record table 0x611258 (0x24 bytes each; byte +0 / +1 / +0x1c / +0x1d select texts and the grave kinds), "gs_AddBeweis" 0x611698.
- Effect slots 0x6f1008 (8 x 0x170): +0 id, +4 flags (1 / 2 / 4), +8 / +0xc / +0x10 floats (1.0, refilled by dt * 0.1 / 60 in 0x4edf80), +0x18 + j * 0x68 three parts of 0x68 bytes (record copied by 0x4eee00), +0x74 + j * 0x68 part state (0 idle, 1 running, 2 / 3 done), +0x150 + 4j part busy, +0x15c.. three kugel objects, +0x168 callback (default 0x4ed3f0), +0x16c its argument (0x6f0e48 + 0x38 * n). 0x4eeb70 sends the parts to the server (0x4c6260).
- Graveyard (building kind 0x1d, +0x65 a 0x24-byte record): +0 object id, +4 kind 1 .. 3 (building byte +0x247), +8 / +0xc / +0x10 / +0x14 the entry lists (0x14-byte entries: list number, id, short, short, byte +0xc, pointer +0x10 to a 0x44 << list grave record), +0x18 .. +0x1b level flags, +0x1c the entries, +0x20 the 0x44-byte zombie records. 0x4efc10 builds it (8 / 4, 12 / 6 / 2, 16 / 8 / 4 / 2 entries), 0x4eff50 grows it, 0x4f17f0 / 0x4f1aa0 save / load, 0x4ef7c0 iterates a list with a callback (0x4ef710 / 740 / 760 / 770 / 780 are the callbacks).
- Order messages go through 0x4c60b0 (type 0 .. 6: 0x4f01d0, 0x4f0430, 0x4f0700, 0x4f0900, 0x4f0a90, 0x4f0c20, 0x4f0d70) and are handled by 0x4f0240 / 0x4f0490 / 0x4f0770 / 0x4f0970 / 0x4f0af0 / 0x4f0c80 / 0x4f0dd0 with the common reasons -1 .. -7 (no object / own object, not a graveyard, no record, no list, no character, entry mismatch).
- Matched: 0x4ed1e0 (`if (n > 0)` for the unsigned count), 0x4ede70, 0x4eded0, 0x4eea70, 0x4eedd0, 0x4eeee0, 0x4ef330, 0x4ef440, 0x4ef4f0, 0x4ef710 .. 0x4ef780, 0x4ef900, 0x4f0180, 0x4f0dd0, 0x4f15e0, 0x4f16f0, 0x4f17f0, 0x4f1aa0 (`z += 0x14` before `i++`), 0x4f22a0, 0x4f36e0, 0x4f39b0, 0x4f4d10.
- Later in the session: matched 0x4f0700, 0x4f0c20, 0x4f0d70, 0x4f2da0 (`if (v > best)`), 0x4f3780 (`i >= 0x800`), 0x4f5070, 0x4f50f0. 0x4f4970 is a level-up check (threshold table 0x5ee0f8, message 0x69 type 9) and 0x4f4d10 loops it over the 768 characters; 0x4f4e20 .. are order handlers of another module (tus.md).

## layout 2026-10-22 session 17
- The unit ends at 0x4f4e20: the handler table 0x611768 follows gv's strings, and 0x4f5d50 / 0x4f5f00 (callee 0x4f2870 of gv.c) match only with the callee outside their file (see notes/vc6-matching.md). The gv.c functions with an address below 0x4f4e20 that had been appended after the handlers (0x4f1de0, 0x4f2750, 0x4f27d0, 0x4f2e30) stay in gv.c at its end (not in address order).

## layout 2026-10-23 session 4
- 0x4f2330 (id, a, kind, flag, out): x = 0x5085b0 (0x50a640 (c id), 0x6f1bc5) clamped to 10000 .. 1e6, v = log10 (x)^2 * 100 * (0.25 / 0.5 / 0.75 by kind 0 / 1 / 2) (flag 0: also * ((a & 0xff) / 255 * 0.7 + 0.3) * 0.75), returns r = 0x5085f0 (v, 0x6f1bc5) and *out = r * 0x5ee034[kind] (0.8 / 0.7 / 0.6). 0x4f2440 (c, k1, k2, out) is the difference of two such costs (matched). 0x4f24a0 = 0x5085f0 (0.1 * x / (log10 x + 1)) for x clamped to 10000 .. 1e5. The call in 0x4f1de0 passes (c, byte +0xc of the entry, entry id, 0, &out).
- The text builders 0x4f30a0 / 0x4f31d0 / 0x4f3270 take a 0x2d-byte record (UNKREC in gv.h, passed by value: a 0x30-byte stack slot copied with rep movs + movsb) and the mode int after it: +4 an int value shown by address, +0x12 the character id, +0x1c the type byte (2 .. 8 / 0xe, 0x10 / 0x11 .. 0x1c), +0x1d / +0x21 ints (text index, object id); texts 0x13xx .. 0x14xx (+ 1 for mode < 2) through 0x5176d0. 0x4f3580 dispatches by the type byte (matched).
- 0x4f41b0 (c, max, ids, cnt): of the 512 records at 0x91f700 (0x2d bytes; +0x25 != 0, +0x16 the accused character id != c) whose pair (c id, record id) is not in 0x91b6e0 the accused characters are counted (up to 0x300 in a local table); returns the one with the most records and their ids.
- 0x4f4460 (c, action 0 .. 9, jump table 0x4f48c8): 0 .. 3 messages 0x5c / 0x5b (value 3 / 0.03 * 0x50a640), 4 / 6 / 8 0x4c5fa0 (id, 1 / 2 / 3, 0), 5 0x4e6680, 7 sets +0xd of the character, 9 a broadcast to the players (0x4e6030); 0x4f48f0 builds the action text (matched with a switch).
- gv.h: ai_Unk50a640 is now `int (unsigned short)` (it was `float ()`, wrong: the callers `fild` its result); the new K&R declarations live in gv.c (gv.h is shared with ord.c / f3gm.c / ls.c / f3init.c: declarations there conflicted with their own).

## match-base 2026-10-24 (base-1 worker, shard 1/5)

- unk_Unk4ed1e0 -> gv_WriteAvatars (the avatar table writer: count 0x20, then per record the id
  and the used byte at +0xf2). Its UNK_FW cast-macro calls are vfs_fwrite calls with the gv.h
  prototype now (bytes unchanged, byte + readable). 0x4ed270 (the reader, same UNK_FR macro) is
  still a draft: the same change there plus ROUGH / Q5 and the "Inconsisten Avatar count" path.
- Session 2, later batch: the Q1-only byte functions named: gv_EntrySameId (0x4ef740),
  gv_EntryIsFree (0x4ef760), gv_EntryIsUsed (0x4ef770), gv_FreeGraveyard (0x4f0180),
  gv_AllocAvatar (0x4ed170) and gv_ReserveEffect (0x4eeaa0,
  behaviour: the guard is first >= -2 - the original's jl - and the bound is gv_Effects +
  8 * 0x170). gv_EntryHasId (0x4ef780), gv_LoadGraveyard (0x4ef900, also clears the ai_Chars
  Q2), gv_FindEffect (0x4eded0); unk_Tab5ee054 -> gv_EntryCounts, unk_Unk4ef7c0 -> gv_FindEntry.
  ai_Unk7e3f30 -> ai_BTypes and ai_Unk91aaf0 -> ai_Goods (ai.h's names; gv.c and the other
  non-ai.h units keep char * views, baseline type debt) cleared Q2 in gv.c / f3gm.c.

## match-base 2026-10-24 (base-1 worker, shard 1/6)

- Renamed from the save game's reading (ls.c): unk_Unk4efc10 -> gv_AllocGraveyard and unk_Unk4f1aa0 ->
  gv_ReadGraveyard (its own address comment: "allocates / reads the graveyard record of the object o", the
  mem_Alloc tag "gv_AllocGraveyardField"); ai_Unk6f1bc0 -> ai_CharCount (18 -> 19 refs over 6 files, the
  number of characters in use of ai_Chars[0x300], ahm.h's own comment).

## match-base 2026-10-25 (base-0 worker, shard 0/6)

- 0x447190 call sites renamed ai_Unk447190 -> d3if_Delete; the 0x4f2720 definition and calls renamed unk_Unk4f2720 -> gv_GetTableRecord (it copies table record i of 0x611258 to out); byte unchanged.

## match-base 2026-10-25 (base-4 worker)
- Reference-only renames (no code change; every use updated, build/link/ledger clean): ai_Unk4c3ae0 -> cm_GetSlotState (0x4c3ae0) and ai_Unk4767d0 / ai_Unk476860 -> d2_StartFade / d2_ReleaseFade (0x4767d0 / 0x476860, the old ai_ aliases folded into the d2 names). See notes/units/unk_5d33b0.md for the whole rename list.

- match-base 2026-10-25 (base-1 worker, last item): ai_StartBuildingWork (0x483d30, ai.c) was raised to
  behaviour + Q6; its callee 0x4ef6b0 was renamed to gv_EntryIndex here (gv.h/gv.c definition and every
  call). The 0x4f22a0 name was NOT unified: the AI caller uses the result as an int * (dereferences it)
  while this unit's draft returns int, so renaming both to one name makes the two headers disagree (lint
  error); the AI-side name ai_Unk4f22a0 was restored.

## play 2026-10-28 (the tutorial crash)
- `gv_ClearGraveyard` (and the two save/load loops at 0x4f28d0 / 0x4f2b30) ended their 0x5a00-byte
  record walk on `&ai_Unk925108 + 0xa` / `&ai_Unk925100`. In the original those are 0x5a08 bytes past
  `gv_Graveyard`; the full link puts them ~0x81a8 past it (the two are separate objects' blocks), so
  the clear wrote ~700 KB of -1/0 over the data section - `ai_GameFlags`, `init_PlayerProfile` and
  `net_Conn` included (`net_Conn.closed` is aliased to `ai_Unk6f1bd4`), which made every later message
  send return -1 and `main_Unk53b840`'s `strncpy` write to NULL. The bounds are now array-relative
  (`gv_Graveyard + 0x5a12` / `+ 0x5a00`), same values as the original's literals.
