# ahm.c (new, layout 2026-10-22)

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** first; below it the findings of each session, one section per author.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): alchemist's building: place records, prototype tables from alchemy_data.ini / alchemy.dfn, walk state, the order messages, character creation, save / load of the record
- Boundaries: separated (plausible): the alchemist's building (layout 2026-10-22 session 16): every function of the range has a draft (0x4eaa50 .. 0x4ed040, 0x4ec500 .. 0x4ed040 new). Start by subject (the building kind 0x1e accessors); its .data block starts at 0x60f1b0 (8-int table, first user 0x4ead20), then the strings "RCPID" .. "alchemy_data.ini" and the tag "ahm_AllocAhmInfo" (0x60f2f8); bss 0x6f0dc8..0x6f0e34. End 0x4ed170: the table 0x60f310 (the avatars, "neVmMaven") starts the next .data block, whose strings (0x611190 "Avatar >%s< ..", "kugel", "gv_Alloc..") are gv.c's, not ahm's (.data order only; no leaked name)
- Range then: 0x4eaa50..0x4ed170
- Was: tag "ahm_"

## Open work
- **headers (2026-10-30, typing pass 2)**: head.c includes txt_views.h, ahm.h, f3gm.h, net.h, vfs.h; keeps 3 cast / view macros with @original byte evidence (the address and the matched caller are in each comment): ai_TimeK, cm_SendExSetHEI, vfs_freadU.
- Complete: 0x4eaa50 .. 0x4ed040 all have drafts (the unit ends at 0x4ed170, see tus.md).
- Rough: 0x4eb4c0 (INI loader, the clamps), 0x4ec320 / 0x4ec3d0 (float chains), 0x4eb180, 0x4ec500 (character creation, 760 / 1072 B off; a copy of the shape of the other creation functions). Near misses: 0x4ec940 / 0x4ecc30 (10 B: the three-way clamp 0 / v / 1.0
  is emitted in the order v, 0, 1.0 by the original; nested `if (v > 0) { if (v < 1) .. }` gives 1.0, v?, 0), 0x4ecd10 (14 B, register allocation p / rec swapped),
  0x4ecf10 / 0x4ed040 (the callee results are unsigned: `ja` after `test eax,eax`; the original keeps four saved registers).
  0x4eac60 is with match-deep (table/step pointer scheduling, see the 2026-10-25 section); 0x4eaa50, 0x4eaad0, 0x4eab20, 0x4eabd0, 0x4eabe0, 0x4eac20 are byte / behaviour + readable now.
- Data not defined in C: the 8-int table 0x60f1b0 (0, 1, 3, 7, 0xb, 0xd, 0x11 ..), the strings, the bss 0x6f0dc8 .. 0x6f0e34.

## match-base 2026-10-26 (base-8)
- 0x4ecb30 ahm_AddPlace **byte + readable** (its only failure was Q4): the object id gm_Query returns is read
  through the new ahm.h view `AHMGOOD {type +0, objId +2}` (AIOBJ.objId, ai.h - ahm.h does not include ai.h);
  `char *sp` became `AHMGOOD *sp`, the cast sits in the initialiser, and all 55 instructions stayed identical.
  The same raw read is still in the 0x4ec4xx function (`gm_AddObjekt(*(int *)(sp + 2), p[5], 2)`).

## layout 2026-10-22 session 15
- The object (building kind 0x1e) has a record at +0x65 (0x3c bytes, "ahm_AllocAhmInfo"): +0 id, +4 float, +8 .. +0xb four shares (byte / 252), +0xc 6 places of (id, value). A character's +0x16c is its object.
- Prototype tables: players 0x6f0e2c (12 B: 5 attribute bytes, id at +8), recipes 0x6f0e34 (12 B: id, two floats), loaded from alchemy_data.ini (0x4eb4c0) or "/gamedata/ai/alchemy.dfn" (0x4eba80, magic 0x61486d52; 0x4ebd60 writes it). 0x4ebf50 skips the "ob_" prefix (`name + 3`).
- 0x4eabd0 is a plain forwarding function (`return f(c, id);` gives `jmp`). The walk generator 0x4eac60 / 0x4ead20 uses a 25 x 25 grid and the table 0x60f1b0.
- Matched: 0x4eaa90, 0x4eab20, 0x4eabd0, 0x4eaf50, 0x4eafa0, 0x4eb130, 0x4ebf50, 0x4ebf60, 0x4ec2d0.

## layout 2026-10-22 session 16
- Matched: 0x4eca00 (`(char)(int)(f * 252.0f)` per share), 0x4ecaa0, 0x4ecb30 / 0x4ecbc0 (do-while with `i++; q += 2;` in that order), 0x4ecd50, 0x4ece10 (message tag stored before the +4 id), 0x4ece60. The messages sent to 0x4c61b0 / 0x4c6230 are `int m[6]` zeroed with memset (rep stos).
- Order messages: tags 'SsCa' 0x4ec940, 'Rdaa' 0x4ecaa0 (id and +8 of the table entry), 'cRSa' 0x4ecc30, 'SmSa' 0x4ecd50, 'ErFi' 0x4ece10; the record handlers 0x4eca00 (set a share), 0x4ecb30 / 0x4ecbc0 (add / remove a place), 0x4ecd10 (set a place's value), 0x4ecdb0 (add to the float at +4, clamped 0 .. 1), 0x4ece60 (delete the alchemist).
- 0x4ec500 creates the alchemist character: first free slot of the 768 (or a kind 0xf one), relations random -0x20 .. 0x1f in both directions (the loop writes ai_Unk7e5360 / ai_Unk889f80 by index), record at +0x65 of the object ("ahm_AllocAhmInfo", 0x3c bytes) filled from the order record.

## match-base 2026-10-24 (base-1 worker, shard 1/6)

- ahm_Unk4ed040 -> ahm_ReadRecord (the reader of the alchemist record at +0x65, called by the save game
  ls.c 0x51d550; its own address comment). ls.c keeps a local prototype: ahm.h's `const char *tag` for
  mem_Alloc conflicts with gv.h's `char *tag` when both are included.

## match-base 2026-10-25 (base-2 worker, shard 2/6)

The unit is 17/46 -> 23/46 byte matched; 12 functions raised to readable, one escalated.

- byte + readable: ahm_OwnerChar (0x4eaa50), ahm_FindSlot (0x4eabe0), ahm_HasFreeSlot (0x4eac20),
  ahm_ShufflePlayers (0x4ead20), ahm_ShuffleTable (0x4eadc0), ahm_RandomRecipe (0x4eaea0).
- already byte, now readable: ahm_UsedSlots (0x4eaa90), ahm_Share (0x4eab20), ahm_SlotOf (0x4eabd0).
- behaviour + readable: ahm_HasRecord (0x4eaad0), ahm_FindRecipe (0x4eae60).
- ahm_FillSteps (0x4eac60): readable, escalated to match-deep. The table pointer and the step pointer
  are allocated / scheduled differently (54 code hunks); every statement and offset matches. Leads
  tried: loop end as ai_WalkSteps+3 (WORSE, 54 -> 72 code hunks, reverted), (int)&ai_PlayerPrototypeCount
  (same bytes, Q3-clean), the compare from the index side (62 -> 54), the menu reload lead (drop the g
  local, re-read ai_PlayerPrototypes[i]: same), the branch-polarity lead. Details in the @deep tag.
- Meaning fixes: +0x65 of the object is the record *pointer*; ahm_OwnerChar passes the record's id (+0),
  not the pointer (the tail call loads `mov ecx,[edx]` first). ahm_HasFreeSlot's limit is
  `(int)(ahm_Share(c, 2) * 6.0f)` (the fmul constant 0x5ebb1c is 6.0 = the six places), compared from
  the limit side (`limit > n` -> cmp eax,edi / setg). ahm_ShuffleTable's count is read as a word, i/j are
  int and the loop is a for loop; ahm_RandomRecipe's step loop is a while with k-- before the step.
- Typed views in include/ahm.h: AHMREC (the 0x3c record), AHMOBJ (kind + rec at +0x65, pack 1), AHMCHAR
  (+0x16c obj) and AHMSEED (int seed / unsigned short seed16). The AHM_REC raw-cast macro is gone.
- util_Random is declared locally in ahm.c as `unsigned short(short)` with @original: 0x42ec00 masks
  its argument (test si,si; and esi,0xffff) and the original caller loads the count with
  `mov cx,WORD PTR ds:0x6f0e30`. ai.h keeps the int view; a header-wide change would add six lint
  conflicts. The two stale `ai_Unk5de62a` lines were dropped from lint_baseline.txt (ahm_ShufflePlayers
  calls srand()).
- Globals named (evidence: the 25x25 grid walk and the "ahm_AllocPlayerPrototypes" /
  "ahm_AllocRecipePrototypes" tags): ai_WalkX / ai_WalkDX / ai_WalkY / ai_WalkDY / ai_WalkIndex /
  ai_WalkSeed / ai_WalkSteps (0x6f0dc8 .. 0x6f0e28), ai_PlayerPrototypeCount / ai_PlayerPrototypes
  (0x6f0e28 / 0x6f0e2c), ai_RecipePrototypeCount / ai_RecipePrototypes (0x6f0e30 / 0x6f0e34).
- Next base item: ahm_Unk4eaf50 (0x4eaf50, byte, Q1 only).

## match-base 2026-10-25 (base-2 worker, shard 2/6, 2nd)

The unit is 23/46 -> 26/46 byte matched (check 2539 -> 2542); 13 functions to byte + readable, one to
behaviour + readable, four to readable with behaviour still draft.

- byte + readable: ahm_ResearchTime (0x4eaf50), ahm_ResearchMinutes (0x4eafa0), ahm_ShareProgress
  (0x4eb0b0), ahm_PlaceFactor (0x4eb130), ahm_FreePrototypes (0x4ebf60), ahm_WalkStep (0x4ec2d0),
  ahm_ClearStep (0x4ec2f0), ahm_LoadPrototypes (0x4eb480), ahm_SetShare (0x4eca00), ahm_SendAddPlace
  (0x4ecaa0), ahm_SetPlaceValue (0x4ecd10), ahm_SendAddValue (0x4ecd50), ahm_SendDelete (0x4ece10).
- behaviour + readable: ahm_ResearchProgress (0x4eafd0) - its draft was the 16-byte object padding:
  the function was 225 bytes and `uv run build` compared 240, i.e. against ahm_ShareProgress's first bytes;
  min as `n < t ? n : t` (one store) brought it to 224 = the original's size.
- readable, behaviour still draft: ahm_SendSetShare (0x4ec940) and ahm_SendSetPlaceValue (0x4ecc30)
  (10 bytes: the 0.0 / 1.0 clamp blocks in the other layout order - the else-form, the inverted form
  and the `if (!(v > 0))` form were all tried, all 10 or worse; the 0.0 block must be the one laid out
  first), ahm_AddValue (0x4ecdb0, 53 -> 44 bytes: the first test `!(f > 0.0f)` is the original's
  and/jne, which catches NaN; `uv run spell` `dup-tail`), ahm_ResearchState (0x4eb180, 191 code hunks, the
  goto-chain clamp of the done share).

Meaning found on the way:
- The 0x4eaf50 family takes a *recipe prototype* (AHMGRID: id + two float attribs), not an AHMSLOT:
  the research time is 14400 * (2 - ATTR0) tenths of minutes (0x3840 = the 1440 minutes of a day times
  ten), reduced by up to half by share 0; ahm_ResearchMinutes = /10 is the amount the research event
  0x95 (0x56d2d0) counts down by gm_MinutesBetween (0x4fe280). ahm_ResearchProgress / ahm_ShareProgress
  return the progress made in *cap minutes and write the minutes used back.
- The five order commands of the alchemist record are the type 100 messages cm.c's 0x4cd010
  dispatches on their four-letter tag: 'SsCa' 0x4eca00 (set a share), 'Rdaa' 0x4ecb30 (add a place),
  'Rdsa' 0x4ecbc0 (remove a place), 'cRSa' 0x4ecd10 (set a place's value), 'SmSa' 0x4ecdb0 (add to the
  record's value), 'ErFi' 0x4ece60 (delete the alchemist); the senders are 0x4ec940 / 0x4ecaa0 /
  0x4ecc30 / 0x4ecd50 / 0x4ece10. 0x4cd010 is where the tag -> handler mapping is proven.
- 0x4c61b0 is the type 100 message poster (cm_SendCommand); 0x4ec61b0's 0x18-byte data is AHMORDER.
- 0x4eb480 loads the prototype tables: mode 0 from alchemy.dfn (ahm_LoadDfn), else from
  alchemy_data.ini (ahm_LoadIni) and then written back (ahm_SaveDfn); called once at start
  (main.c 0x532407 with the global 0x6f2468).
- 0x4eb180 is the research state of an object (events 0x95 .. 0x97: the done share and the id);
  0x4eb410 re-stamps the due clock of those events and re-posts them (-2).

Typed views added in include/ahm.h: AHMORDER + the AHM_CMD_* tags, AHMCHAR.id (+4), AHMOBJ.id (+1,
AIBLD.id), AHMRESEARCH (+0xb0 .. +0xbc).

Open work (next session):
- ahm_RescheduleResearch (0x4eb410): Q4 - the 14-byte clock copy is four stores 4/4/4/2 bytes into
  +0x52 / +0x56 / +0x5a / +0x5e, so a typed clock needs that field split (ai.h's AITIME splits at
  +6/+0xa and would change the copy); also ai_Unk4c4c60 / ai_Unk770ae0 (Q2).
- ahm_AddPlace (0x4ecb30), ahm_RemovePlace (0x4ecbc0), ahm_DeleteAlchemist (0x4ece60), ahm_ObjectId
  (0x4ebf50): Q2 needs ai_Unk4ffe90 / ai_Unk4ffff0 / ai_Unk4ff190 / ai_Unk500970 / ai_Unk505df0 /
  ai_Unk4c4c60 / ai_Unk1460ae0 / ai_Unk61a654 / ai_Unk4fe900 (gm_FindObjectProto, already named in
  f3gm.c: ahm.c still declares it locally as ai_Unk4fe900) and 0x4ebfb0 / 0x4ec140 / 0x4ec320 /
  0x4ec3d0 / 0x4ec490 / 0x4ec500 / 0x4ecf10 / ahm_ReadRecord (all draft).
- Q7: the unit's header is not canonical - he.c (18), unk_575860.c (9), cm.h (8), ls.c (3), f3gm.c (2),
  main.c / sv.c (1 each) declare ahm.c's functions locally (ai_Unk4eaa50, ai_Unk4eab20, ...). Not
  touched here (other workers' .c files); the ahm.h prototypes the canonical names.
- Big drafts: ahm_LoadIni 0x4eb4c0 (618 code hunks), ahm_LoadDfn 0x4eba80 (228), ahm_SaveDfn 0x4ebd60
  (218), ahm_Unk4ebfb0 (205), ahm_Unk4ec140 (198), ahm_Unk4ec500 (405), ahm_Unk4ecf10 (189),
  ahm_ReadRecord (150), ahm_ResearchState (191).

## match-base 2026-10-25 (base-2 worker, shard 2/6, 3rd)

The unit is 27/46 -> 28/46 byte matched (check 2548 -> 2550). Two functions to byte + readable, one
escalated, one readable with the meaning fixed and escalated when only the block order was left.

- byte + readable: ahm_RescheduleResearch (0x4eb410), ahm_LoadDfn (0x4eba80).
- readable, escalated: ahm_LoadIni (0x4eb4c0, 492 -> 49 differing instructions; the two hunks left are
  the cleanup block's place), ahm_ResearchState (0x4eb180, the clamp value v on the FPU vs a stack temp).
  Both carry the worked-out findings in their @deep tags.
- AHMGRID is now a union with three views: `plr` (attr[5] + id at +8, the table 0x6f0e2c), `rcp` (id + two
  0 .. 1 floats at +4 / +8, 0x6f0e34) and `w[3]` (the words ahm_FillSteps copies). Storing through the typed
  fields instead of `char *e = base + i * 12` is what the original does (it reloads the table base per store,
  `mov ecx,ds:0x6f0e2c; mov [edi+ecx*1+4],al`) and it removes the address-taken float temp (ahm_LoadIni's
  frame 0x304 = the original's, was 0x308).
- Clamps read off the original: ahm_ClampAttr zeroes 0 and below (`v & ((v <= 0) - 1)`) and caps at
  `v >= 0xfc`; ATTR4 is 0x12 .. 0x1e (`<= 0x12` / `>= 0x1e`); ahm_ClampShare is min(max(atof(s) / 100, 0), 1)
  computed in DOUBLE (QWORD fmul / fcomp) with atof re-evaluated at every use - min/max macros, four atof
  calls and a QWORD store per check; only the store into the float field narrows.
- ahm_RescheduleResearch: the kind test is 0x95 <= kind && kind <= 0x97, the due clock at +0x52 is a 14-byte
  AITIME copy (AHMCLOCK, pack 2: stores 4/4/4/2). 0x4c4c60 sends message 0x1d for the job/event (its
  handler's own string is "cm_ExSetHE"), hence cm_SendExSetHE.
- ahm_LoadDfn: the counts are 1 .. 0x64, the entries are read straight into the union fields, and the read
  helper is vfs_fread seen through an unsigned view (the call sites cut `test eax,eax; jbe`, i.e. `<= 0`).
- Type debt in include/ahm.h (@original tags): vfs_fread is declared unsigned and vfs_Tfopen takes
  `struct VFSDIR *` (vfs_Root), against vfs.h's int views. ahm.h is only included by ahm.c.
- Next base item: ahm_SaveDfn (0x4ebd60, 218 code hunks - the writer of the same dfn format; it already
  improved 129 asm lines from the LoadDfn changes).

## match-base 2026-10-25 (base-2 worker, shard 2/6, 4th)

The unit is 29/46 -> 32/46 byte matched (check 2561 -> 2564); five functions raised (four to byte +
readable, one to behaviour + readable + readability), two escalated (one later cleared).

- byte + readable: ahm_SaveDfn (0x4ebd60), ahm_SaveWalk (0x4ebfb0), ahm_LoadWalk (0x4ec140),
  ahm_ObjectValue (0x4ec3d0).
- byte, now readable: ahm_ObjectId (0x4ebf50; only Q2/Q7 were open).
- behaviour + readable, escalated: ahm_StepValue (0x4ec320 - the FPU scheduling of the four squares,
  see its @deep tag).
- ahm_SaveWalk was escalated mid-session (the induction-variable base) and cleared when the index form
  matched; the @deep tag is gone.
- Names: ahm_Unk4ebfb0 -> ahm_SaveWalk (the save game 0x5190b0 calls it), ahm_Unk4ec140 -> ahm_LoadWalk
  (the loader 0x51c950 from version 0x10049), ahm_Unk4ec320 -> ahm_StepValue (its result goes to the
  text builder 0x5176d0 from the tutorial windows 0x57bf00 / 0x57cf90), ahm_Unk4ec3d0 -> ahm_ObjectValue
  (the same formula for a whole building; used by 0x57cf90 / 0x57de60), ai_Unk4fe900 ->
  gm_FindObjectProto (f3gm.c's own name, prototype moved to f3gm.h), ai_Unk51c920 -> ls_Read (ls.c's
  reader, 0x51c920; its seven declarations disagreed on the file argument and now all use ls.c's
  `int ls_Read(void *buf, int size, int count, int f)`, so the Q7 canonicalisation of ls.c is still open
  but the cross-TU conflict is gone).
- AHMSTEP is now `{ int on; AHMGRID grid; int x, y; }`: the step's 12 bytes at +4 are a copy of a grid
  entry (ahm_FillSteps copies its three words; the save writes its player view = five attribute bytes +
  the id). ahm_SaveWalk / ahm_LoadWalk write/read through `ai_WalkSteps[i].grid.plr.*`.
- Meaning found: ahm_StepValue's recipe values are a second sum ADDED to the squared attributes (the
  0.0 at 0x5eb788 is the value the branch replaces), not a replacement; ahm_ObjectValue's two products
  are separate statements with the recipe sum first.
- VC6 lessons (also in notes/vc6-matching.md): the walk save/load loops only match in the direct index
  form `ai_WalkSteps[i]` (every pointer spelling put the induction variable at step + 0x14 instead of the
  original's + 0x10); `v = sum * 500.0f + sum2 * 500.0f` is factored into (sum + sum2) * 500 by VC6, so
  ahm_ObjectValue needs the two products as separate statements; the unsigned `<= 0` fwrite/fread checks
  with statement-level early returns give the original's inline `pop edi; pop esi; ret` epilogues.
- Next base item: ahm_Unk4ec490 (0x4ec490, the 0x24-byte message 0x4c6170 of a step; in progress: the
  draft stores the same fields in the original's order, the register allocation differs - the original
  keeps the id in eax, ours hoists p[4] into edi and saves it).

## match-base 2026-10-25 (base-2 worker, shard 2/6, 5th)

The unit is 32/46 -> 33/46 byte matched (check 2564 -> 2565); one function raised draft -> byte + readable,
one already at byte and now readable, two close ones escalated.

- byte + readable: ahm_RemovePlace (0x4ecbc0) - the two place stores go through `AHMREC.slots[i]` and the
  object list through the new `AHMOBJ.objs` (+0x5d) field; both are byte-identical (the typed store of the
  float `0.0f` is still one `mov DWORD PTR [..],0`). Rename ai_Unk4ffff0 -> gm_RemoveObject: f3gm.c already
  names 0x4ffff0 so (its comment at its definition) and the ahm/cm/ai declarations were the stale views;
  f3gm.c's own K&R line 2440 was renamed too.
- byte + readable: ahm_SendCreateAlchemist (0x4ec490, was ahm_Unk4ec490). The original did NOT cache the
  prototype's id across the ahm_FindRecipe call: it loads `[esi+0xc]` for the argument, then reloads the
  three prototype words through a materialized `lea ecx,[esi+4]`. Both facts come from ONE source form:
  the message is a struct (AHMCREATEMSG, 0x24 bytes = 9 ints) and the prototype is copied with the union
  assignment `m.proto = s->grid` - a 12-byte aggregate copy makes VC6 compute the source address into a
  register (lea) and re-read it, while the call's `s->grid.plr.id` stays a direct load; two separate
  member stores or an int pointer local (`int *w = p + 1`) let VC6 CSE the id into edi across the call
  (push edi / pop edi, one extra instruction and a different store schedule). The message's first two ints
  (seed, charId) are never written here - the handler 0x4ccf90 writes/syncs them.
- ahm_AddPlace (0x4ecb30) readability advanced, byte kept: the order goes through AHMORDER (its +0x14, now
  `nextObjectId`, is the id counter a networked client continues from - the function gives it to
  ai_NextObjectId when the socket is not INVALID_SOCKET), the place stores through `AHMREC.slots[i]`, the
  object list through `AHMOBJ.objs`; ahm.c includes net.h for `net_Conn.s` (0x1460ae0 is net_Conn, the field
  hst.c's local view calls `s`; the socket comparison is the original's `!= -1`). Q4 is down to 1 and Q2 to
  1: both need 0x4ff190. What 0x4ff190 is (read 0x4ff190..0x4ff443): it takes one argument it never reads,
  queries the market building (gm_BuildQuery = 0x501190, list {0x92 .. 0x97, 0x2db, 0}) into ai_MarketBuilding
  0x6f1bc8, clears the 0x55-entry table 0x91aaf0 (each entry +4 of 0x80), walks the queried buildings' object
  lists (+0x5d / +0x14) and the own player's trade slots, creates the missing objects with 0x500190 and
  stores a count in the byte +0x1c of an object of kind 4 / 0x11. It is too big to name from its head alone
  - it needs its own reading. ai_Unk4ff190's declaration in ahm.h is still K&R (Q7).
- ahm_WriteRecord (0x4ecf10, was ahm_Unk4ecf10; ls.c's ai_Unk4ecf10 alias too) and ahm_ReadRecord
  (0x4ed040) are readable now (behaviour still draft, 189 / 150 code hunks - match-deep): the
  `AHM_FW` / `AHM_FR` macros (`((unsigned (*)())vfs_fwrite)`, the unprototyped function-pointer views) are
  gone - the callees have prototypes in ahm.h and every comparison is `== 0`, so the direct calls compile
  to the same bytes; the record pointer is `AHMOBJ.rec`. Rename ahm_Unk4ecf10 -> ahm_WriteRecord pairs with
  the existing ahm_ReadRecord (its own comment: writes the record 0x4ed040 reads).
- Two more stale views unified with the names their owner already gives them: ai_Unk500970 -> gm_AddObjekt
  (f3gm.c defines 0x500970 under that name; its comment "adds the amount n of the object prototype proto
  to the owner id") and ai_Unk61a654 -> ai_NextObjectId (f3gm.c's own comment "the next object id"; the
  counter behind the building and character ids).
- Meaning: the type 99 message (cm.c 0x4c6170, handler 0x4ccf90) makes the receiver create the alchemist
  character (ahm_Unk4ec500) of the building at data[2]; data[3..5] is the step's prototype entry, data[6]/[7]
  the name indexes (0x144a7a8 / 0x144a80c) and data[8] the recipe entry's +8.
- Renames: ahm_Unk4ec490 -> ahm_SendCreateAlchemist, ai_Unk4c6170 -> cm_SendCreateAlchemist (the only
  caller is 0x4ec490; the handler 0x4ccf90 creates the alchemist from it).
- Escalated to match-deep, both close (12 differing instructions each, no menu rule for the hunk): ahm_SendSetShare (0x4ec940)
  and ahm_SendSetPlaceValue (0x4ecc30). The only difference is the order of the two out-of-line 0.0 / 1.0
  return blocks (the original lays 0.0 out first) plus the block's lea register; the branch tests and all
  leading instructions match. Two more clamp spellings tried on SendSetShare, one build each: the 1.0 block
  inside the outer `if` with 0.0 after it (no `else` keyword - still the 1.0 block first, 12 as before) and
  the two-guard early-return form (worse: 30, `jnp` branch). Reasons in the @deep tags.
- Next base item: ahm_Unk4ec500 (0x4ec500, the creation; 332 / 330 instructions but 196 differing asm
  lines - the draft is semantically complete, the allocation differs from the top: the original zeroes ebx
  once and pushes it for gm_FindById's two 0 arguments and for the `o == 0` test, walks ai_Chars with a
  pointer (eax += 0x21c, bound 0x7d5f80) instead of an index, and keeps the object in eax first).

## match-base 2026-10-25 (base-2 worker, shard 2/6, 6th)

Check 2567/4664 unchanged, grade regressions 0; ahm.c 33/46 byte, 2414 differing bytes (unchanged).

- ahm_CreateAlchemist (0x4ec500, was ahm_Unk4ec500) -> match-deep. 196 differing instructions, but the
  whole statement / offset set was read against the original and is complete: the difference is allocation
  from the first instruction (a zero register ebx pushed for gm_FindById's two 0 arguments and reused for
  the byte / dword 0 stores, ours uses immediates and edi = 0; -1 in ebx vs eax; the character pointer
  frame base ebp vs eax; the ai_NextObjectId store schedule after the name strcpy and the util_Random
  block). `uv run spell` menu assessed: rep-stos (memset already rep stos on both sides), the three dup-tails
  (no arm to move), the reload at 4ec8ab (the [sp+0x14] head is re-read from memory on both sides), the two
  code hunks (scheduling, equal multiset). Q2 gap: ai_Unk144a7a8 / ai_Unk144a80c are the two character-name
  pointer tables in the exe's BSS (0x144a7a8 / 0x144a80c, filled at runtime; no writer, no string or data
  evidence for first / last name). Q4 gap: 54 raw accesses of the 0x21c record; ai.h's AICHAR names id, kind,
  netId, health, name, jobId, title, bld / bld2, money, flags1cc, mood3Rate, partner, unk215 .. unk217, mask,
  but +0x10 .. +0x20 (the 1000.0f / 12.0f start values), +0x50, +0x5c, +0x80, +0x9, +0xc, +0x18c, +0x190,
  +0x1f0, +0x1f4, +0x20c have no evidence, and the typed rewrite re-schedules the store block. Q1 / Q5 /
  Q6 / Q7 done. Renames: ahm_Unk4ec500 -> ahm_CreateAlchemist; ai_Unk4fb890 -> gm_ChooseHeadIcon (the head
  icon number of the character), ai_Unk4fbd10 -> gm_PlaceOnStage (puts the character on the stage at its
  building's door), ai_Unk4ff190 -> gm_RefreshGoodsLists (its own comment: refreshes the goods lists of the
  own player and stocks the market); their K&R lines in ahm.h / cm.h / gv.h are prototypes now, f3init.c
  keeps its K&R line (rename only). cm.h keeps `int ahm_CreateAlchemist(int *p)` with @original: the original
  handler 0x4ccf90 reads the id as an int (no `and eax,0xffff`), the definition returns unsigned short.
  Incident: the first build with the unsigned short prototype in cm.h turned ai_Unk4ccf90 from byte to
  behaviour by that one masked instruction; the @original line restores it (check 2567 again).
- ahm_AddValue (0x4ecdb0): the clamp rewritten to `if (!(f > 0.0f)) f = 0.0f; if (!(f < 1.0f)) f = 1.0f;
  r->value = f; return 1;` (the else-if form compiles to the same bytes) - 44 -> 41 differing bytes, 25 -> 12 code hunks. Tried and reverted (worse or
  equal): the nested store form (37 bytes but 15 asm lines), the `f <= 0 / f >= 1` clamp (44), the
  early-return form (39 / 18 lines). The remaining 12 hunks are the test schedule: the original emits the
  0.0 and/jne test and then the 1.0 test at the top, our form evaluates them at the L1 join. Left in the
  base queue (readable, behaviour draft). Lesson in notes/vc6-matching.md.
- Not touched, still open: 0x4eb180 / 0x4eb4c0 / 0x4eac60 / 0x4ec320 / 0x4ec940 / 0x4ecc30 (all @deep),
  ahm_AddPlace's Q4 (1 raw access) / Q7 (done now: gm_RefreshGoodsLists has a prototype), ahm_ReadRecord /
  ahm_WriteRecord / ahm_ResearchState behaviour.
- ahm_WriteRecord (0x4ecf10) -> match-deep: the four flat fwrite checks changed to the unsigned `<= 0`
  form (vfs_fwrite is declared unsigned; the original tests `ja` over the inline return) - 135 -> 133
  differing instructions; the same change on the nested +0x11 check and the while makes it far worse (191,
  reverted: the inverted nested block regroups the body). The rest is the prologue (the original keeps the
  record in ebx with ebp pushed, ours in esi) and the return-0 tail: the original inlines
  pop edi/pop esi/pop ebp/xor eax/pop ebx/ret after every failure, ours routes every return 0 to one
  shared out-of-line tail. @deep tag has the details.
- ahm_ReadRecord (0x4ed040) -> match-deep: the same shared-tail / prologue difference (the original inlines
  the return-0 epilogue and skips it with jne, ours jumps to one shared tail with je); the `!ls_Read(...)`
  spelling compiles to the same 136 differing instructions (not the cause). Body complete.
