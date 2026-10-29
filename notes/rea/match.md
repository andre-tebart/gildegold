# Match sessions (skills: match-base, match-deep)

Solo sessions that bring the functions of separated units (`uv run ledger units`: `separated (proven)` /
`separated (plausible)`) closer to byte identity, one unit at a time. Per-unit findings, near-misses and what was tried go to
`notes/units/<unit>.md`; this file holds only where the next session starts.

The queue is computed, not kept here. Since 2026-10-23 every function carries two grades (`uv run ledger grade
--help`: behaviour `draft / behaviour / byte`, readability `draft / readable`, recorded in the working state
`build/state/names.csv` and gated against the committed `state_baseline/names.csv`) and the
work is split in two levels (user decision 2026-10-23): **match-base** raises functions to behaviour + readable
with reading and meaning fixes only (`uv run ledger grade next`: the base queue, behaviour drafts before the
readability work of functions already at behaviour / byte, units with the fewest such functions first); a
function the easy means do not raise is handed over with `uv run ledger grade escalate NAME
"why"` (`@deep why` in its address comment, column level = deep); **match-deep** brings the escalated functions
and the byte work of units whose base work is done to byte identity with the whole toolbox, keeps / makes them
readable, and owns the unit's data blocks (`uv run ledger grade next --deep`: units by total differing instructions,
smallest first, closest function first). A unit is part of a queue while the ledger computes it separated (both
boundaries at least plausible, nothing undrafted inside), and leaves it when fully byte + readable or open.
A `WORSE` line normally reverts the edit; a meaning correction that must stay (a wrong argument count / type
that the register / slot cascade amplifies) keeps its source, is recorded in the unit note and its escalation,
and resets the local best reference with `uv run ledger grade accept NAME "why"`.

## Where to continue
A match-base session takes `uv run ledger grade next`; a match-deep session `uv run ledger grade next
--deep`. A function whose leads are exhausted is marked `@stuck why` (`uv run ledger grade stuck NAME "why"`, column
level = stuck); the deep queue keeps it but moves it behind every `@deep` function, and `--queue` / `unitdiff
--queue` show the fully stuck units apart. A new idea re-promotes it with `uv run ledger grade escalate NAME "why"`.
`uv run ledger check` refuses a `@stuck` function that is not under **Open work** in its note, and a
stale `@deep` / `@stuck` on a byte + readable function. Leads for a stuck function live in the unit's note.

## match-base session 2026-10-23 (match worker)

Units worked: path.c (readable, 4 escalations), txt.c (2 readable, 2 escalations), tb.c (6 readable, 1
escalation), zlib/inflate.c (unit byte + readable), d3_io_fl.c (4 renames, 2 readable, 3 escalations),
d3_quad.c (pos / flags field renames, 5 readable, 2 escalations), unk_4591a0.c (4 function renames + 1
macro rename, 4 escalations). Next session: `uv run ledger grade next`.

## match-base session 2026-10-24 (match worker, 2nd)

Units worked: d3_par.c (unit byte + readable, 12/12, 7 functions raised), mov.c (mov_WndProc + 8 more
readable, mov_Find / mov_FindWnd byte, mov_Open escalated), anm.c (anm_Alloc / anm_Free named and
readable). Next session: `uv run ledger grade next`.

Open regression, NOT from this session: ai_Unk4a13e0 (am.c, 0x4a13e0) fell behaviour -> draft with
257bad90's L3 change `ai_Unk503ee0(&x, ...)` (+ the am.h prototype). The original caller at 0x4a13e0
pushes the hidden result pointer and then copies 6 bytes from eax after the call (the by-value / returned
pointer form the commit removed); the callee gm_Unk503ee0 (0x503ee0) itself does not match (34/64), so the
out-pointer premise is unproven. It was invisible until am.c was recompiled (stale object in build/). The
main owner has to settle the callee's real signature; the source change was left as committed.

## match-base session 2026-10-24 (match worker, 3rd)

Unit anm.c: named the whole animal module (7 function renames + 4 rd.c helpers), raised anm_NewAnimal and
anm_RemoveAnimal to byte + readable; the other 9 functions are byte and escalated with Q4 / Q2 reasons (the
typed-field fix needs ch.h in anm.c, which rd.c's char* d3_DefaultScene view and the d3.h / ch.h / rd.h
conflicts block - details in notes/units/anm.md). Next session: `uv run ledger grade next` (anm.c is out of the
base queue; the Q4 work is with match-deep).

## match-base session 2026-10-24 (match worker, 3rd) - units worked

anm.c: named the whole animal module (see notes/units/anm.md), anm_NewAnimal / anm_RemoveAnimal readable, the
other 9 escalated (Q4 typed fields need ch.h in anm.c, blocked by rd.c's char* d3_DefaultScene; Q2 on two
unnamed D3OBJECT/D3LOPS fields).
dd_vesa.c: the unit's base queue is empty now (d3_InitKeyCodes, dd_Exit, d3d_GetDeviceInfo, dd_DlgProc,
dd_DeviceDialog, dd_CheckModes, dd_LockSurface, dd_FillSurface, dd_SplitRGB, dd_ReleaseSurface,
dd_UnlockSurface, dd_Line byte + readable; d3d_EnumDeviceCallback and dd_SetMode -> match-deep). The Q2 names
came from D3DCARDINFO (d3_d3d.h) and the existing dd_ZCleared alias.
d3_light.c: 6 functions named + readable, 2 escalated (see notes/units/d3_light.md); D3MESH.unk96d -> flags
and unk958 -> lightScale.
Next session: `uv run ledger grade next` (d3_light.c next: d3_Unk452fa0).

## match-base session 2026-10-24 (match worker, 4th)

d3_light.c: the 6 remaining byte functions are escalated (Q1/Q2/Q4, see notes/units/d3_light.md); the unit's
base queue is empty. d3_snd.c: 15 functions to byte + readable (Q6 descriptions, the sb_* callee names and the
SBSAMPLE overlay fields), d3sndw_Update -> match-deep. msx.c: 5 functions to byte + readable.
Next session: `uv run ledger grade next` (msx.c next: msx_Stop).

## match-base session 2026-10-24 (match worker, 5th)

msx.c: 10 byte functions to readable (Q6), msx_Update escalated (IV base &t->volume), unit 18/19 byte and
its base queue empty. d3_io_a.c: 13 AGI handler descriptions + AGIPOLY.unk24 -> tex, the two unnamed
handlers escalated, unit 25/25 byte. d3_anim.c: d3anim_ApplyMotionTo (behaviour + readable, D3ANIMSLOT
lastFrame/lastSub), d3anim_MapBones (behaviour + readable), d3anim_NextFrame (byte + readable),
d3anim_ApplyRootMotion / d3anim_FreePath named, 9 functions escalated (deep drafts and the two shared-field
blockers unk084 / D3_OBJPATH, see notes/units/d3_anim.md). Reference updates in ch.c, d3_engine.c,
d3_io_t.c, d3_tools.c, ecmd.c.
Next session: `uv run ledger grade next` (d3_anim.c next: d3_Unk435550; then d3_Unk435740, d3anim_Unk435810,
d3anim_Unk435870, d3anim_SetFrame).

## match-base session 2026-10-24 (match worker, 6th)

d3_sm.c: D3OBJECT.unk1cc -> info and unk212 -> stateFlags project-wide (the cleared Q2 leads), d3sm_BoundsCB
byte + readable, d3sm_ShadowCB readable (behaviour deep). d3_quad.c: d3_Unk439870 -> d3_UpdateWorldMatrix,
d3_Unk439650 -> d3_ClipBox, d3q_GetBounds + d3q_Mark byte + readable; unit 11/11 byte, base queue empty.
d3_anim.c: the D3ANIMSLOT/D3ANIMDATA fields named (nextFrame, subFrame, attach*, attached, fade*, timeout,
speed, frameTime, first/last/relative), d3_Unk431ae0 -> d3anim_PoseBones, d3_Unk435550 -> d3anim_StartAnim,
0x435810 -> d3anim_PauseAnim, 0x435870 -> d3anim_PlayAnim; four functions byte + readable, base queue empty.
d3_light.c: D3OBJECT.unk220 -> kind, D3LOPS.unk6c -> originY, d3_Unk452ef0 -> d3_LightPrepare,
d3_Unk43a000 -> d3_SelectSurf, d3_Unk43d7c0 -> d3_TransformSurf; d3_LightApply + d3_LightPrepare byte +
readable. Next session: `uv run ledger grade next` (d3_light.c next: d3_Unk452fa0).

## match-base session 2026-10-24 (match worker, 7th)

d3_light.c: the base queue is empty. Raised to byte + readable: d3_SetVertexColors (0x452fa0),
d3_LightCollectCB (0x453110), d3_RelightObject (0x453280), d3_RelightObjects (0x453580), d3_RelightObjectFrame
(0x4535b0, now takes the D3ANIMFRAME and reads its radius, not a D3MATRIX). Escalated: d3_LightInvalidate
(0x4538a0, Q2 0x633438 - no writer in the exe, condition unproven). Project-wide renames: D3OBJECT.unk211 ->
objectFlags, D3PTRI.unk26 -> drawFlags, d3_Unk5fcc78 -> d3_AmbientColor, D3MESH.unk95c -> lightGeneration,
d3_Unk633738 -> d3_Terrain, d3_Unk633b14 -> d3_LightsChanged (that raised d3s_LightCB, d3s_FreeLightCB and
d3s_ResetShadows to readable as a side effect).
Blocked type debt: typed union/field views in D3PTRI/D3TEXTURE/D3PVERT flip the instruction order of
d3_CloneObject and d3_Unk4550e0; d3_light.c uses cast views of its own header. Lesson in
notes/vc6-matching.md, details in notes/units/d3_light.md.
Started rd.c: rd_DoorPos (0x4bc2e0, byte) needs Q2 only; its callees are named in f3gm.c / d3_interface.c (leads
in notes/units/rd.md). Next session: `uv run ledger grade next` (rd.c next: rd_DoorPos).

## match-base session 2026-10-24 (base-0 worker, shard 0/2)

Shard 0/2. d3s.c: 11 functions to byte + readable (the edge / span / cache and shadow-system helpers, all
named; d3_InitShadows' Q2 cleared through d3d_UsedCard.info.maxw and the shadow size globals) and 3 Q6
descriptions; the rest of the unit (d3s_DrawSpans, d3s_ShadowTriangle, d3s_ShadowPoly, d3s_TerrainShadowMesh,
d3s_MeshFromMap, d3s_UpdateObjectAt, d3s_Update, d3s_Project, d3s_AddShadow) escalated with @deep reasons.
Meaning fixes found on the way: the block scan reads `.lod` (not `.hi`; D3SMBLOCK gained lod), Project's rect
ends round as `+ 1.0` (double) inside the cast, Update's `type >= 5` and double `0.0`. Only d3s_GetGround is
left in the unit's base queue (in progress: parent test + init order fixed, 6841 -> 6712 bytes, one
branch-polarity hunk). d3_sm.c: the 0x45a2e0 call now uses d3s_ShadowTriangle (declaration and call had a
d3_ prefix, the definition d3s_). Next session: `uv run ledger grade next` (d3s.c next: d3s_GetGround; then vfs.c).

## match-base session 2026-10-24 (base-1 worker, shard 1/2)

Unit rd.c: base queue empty. rd_DoorPos byte + readable (f3gm.h + G15BLD, d3_FindObject449690 -> d3_FindObject
and the finder variants renamed), 19 functions readability-raised (the whole veg_* family, the road helpers),
8 escalated to match-deep (rd_PointsNear, veg_FindType, veg_SetPlant, veg_LoadModel, rd_BuildRoad,
rd_PathToRoad, rd_AllBauplatzToSupermap, rd_FillQuad - reasons in notes/units/rd.md and the @deep tags).
Next session: `uv run ledger grade next` (sblib.c next: sb_Init).

## match-base session 2026-10-24 (base-0 worker, session 2)

Units worked: d3s.c (d3s_GetGround byte + readable, unit's base queue empty), sblib.c (sb_Init, sb_Exit,
sb_Update + 20 Q6 descriptions, sb_UnloadSampleBank behaviour + readable; sb_Play -> match-deep; base queue
empty), vfs.c (20 readability-raised: vfs_strcmp, vfs_UpperCase, vfs_ToSlashes, vfs_ToBackslashes,
vfs_AddFilePath, vfs_FindFile, vfs_DirPath, vfs_AddDir, vfs_AddZip, vfs_MakeDirs, vfs_FinishedFiles, vfs_GetPath,
vfs_fread, vfs_fgetc, vfs_Init, vfs_Exit + the four typed comparators; five register/slot functions ->
match-deep; base queue empty), d3_interface.c (d3_CreateObject renamed, D3SURF.unk14 -> texHandles,
d3_Unk44cbb0 -> d3_AttachMesh byte + readable; d3_CreateObject -> match-deep on the d3io_LoadObject header
clash). Project-wide renames in the report. Lesson: a struct member-list change in a shared header (anonymous
union in D3MESH, a field in D3MATERIAL) flips d3_Unk4550e0 by 8 bytes; cast views in the unit's own header
instead (notes/vc6-matching.md).
Next session: `uv run ledger grade next` (d3_interface.c next: d3if_SelectTextureSet).

## match-base session 2026-10-24 (base-1 worker, shard 1/5)

Shard 1/5. chc.c: 43 of 44 functions byte + readable (all the Q6 descriptions and the Q4 macros
replaced by typed views in ch.h); chc_TakeObjectScript -> match-deep (Q4 D3MATERIAL_NAME104, the
d3.h union flips d3_Unk4550e0). Project-wide renames: CHARACTER.unk02c -> building, unk030 ->
group, unk124 -> transport, flags.unk9 -> lowPoly, D3OBJECT.unk084 -> rotation,
d3_Unk448ac0 -> d3_YawTo, d3_Unk453d60 -> d3_UnwrapAngles, ai_Unk7e3f34 -> ai_Objects; the d3_anim
@deep of d3anim_ApplyRootMotion cleared (byte + readable now).
ls.c: ls_LanguageOf, ls_GetLanguage and ls_WriteObjects raised; 7 of the layout drafts escalated
(shape only, reasons in notes/units/ls.md).
Next session: `uv run ledger grade next` (ls.c next: unk_Unk51a4b0).
## match-base session 2026-10-24 (base-4 worker, shard 4/5)

Shard 4/5. hmt.c: 12 functions to byte + readable (the table helpers, the election state and the price helper; the
record table read out of .rdata: kind 0 talent / 1 profession / 2 historical event, textSet selects the text block) and
4 escalations (RandomTextOffset, RecordTextId, Init, Load - register allocation / dead load, details in
notes/units/hmt.md). The worth helpers were one function under two or three spellings (ai_Unk/gm_Unk/unk_Unk):
gm_GetGoodsValue / gm_GetWorth / gm_FindRichest, 208 references (readable count +57 in the other units). sfx.c: unit
byte + readable, all 43 Q6 descriptions on the prototypes in sfx.h and main.c includes sfx.h. evtc.c, mov.c, surf.c:
unit readability (the last foreign declarations fixed: evt.c includes evtc.h, main.c includes mov.h, surf.h owns
surf_Blit / surf_ScaleBlit). bmp.c's 6 foreign declarations need a vfs declaration cascade (d3_fl.h / d3_int_io.h
declare vfs_Root / vfs_FindDir / vfs_Tfopen differently from vfs.h) - not attempted, the batch was reverted.
Next session: `uv run ledger grade next` (hmt.c next: ai_Unk4ac2f0; then its behaviour drafts).

## match-base session 2026-10-24 (base-0 worker, shard 0/5, session 2)

Shard 0/5. d3_interface.c: project-wide renames D3SCENE.unk3d4 -> loaded, unk3d5 -> dirty (f3init.c marks
reserved universes "dirty" and clears the flag after the reload; d3_int_io.c sets loaded after a scene file
load; d3t_FindFreeScene skips both), the D3IFSCENE mirrors, unk3d8 -> sceneFlags (0x63349c d3_SceneFlags),
unk3dc -> hardware (0x633450 d3_Hardware), unkb0 -> hmap (D3SCENE.hmap), unkb4 -> mapSize (0x80/0x40, the
d3sm_Create size; ch.c's coordinate bound) and d3_Unk43ff90 -> d3fl_FreeMap (0x43ff90, d3_fl.c).
d3if_ClearScene raised byte + readable. Escalated: d3if_Reset and d3if_InitScene (Q2 0x5fb5c0 / 0x5fb5c4,
the D3IFSCENE unk3cc / unk3d0 pair: write-only - a .text scan of the whole exe finds only the stores in
d3if_Reset and the save/restore in d3if_SelectScene, no reader, no pointer in .data, .data initials 4 / 0xf),
d3if_SelectScene (same pair + 12 further unnamed globals, list in the @deep tag).
VC6 lesson: the prototype of d3fl_FreeMap added to include/d3_fl.h flipped d3_tools.c's d3t_UpdateSunRays from
byte to 125 differing bytes (fild + fsubp vs the original's fisub); the declaration local to d3_interface.c
keeps it matched (see notes/vc6-matching.md). A clean rebuild (rm -rf build/obj) is needed to see it.
Also raised byte + readable: d3if_MoveObject (d3_Unk43dbc0 -> d3_InflateObject, the "d3:InflateObject(...)"
allocation tags), d3if_LoadSprite (d3if_Unkfe798c -> d3d_UsedCard.info.caps, the byte read at 0xfe798c =
0xfe7680 + 0x30c; D3IFTEXFLAGS is a union with the raw D3TEXFLAGS), d3if_DrawSprite (0xffffff -> D3IF_RGB_MASK),
d3if_ScreenBox (behaviour + readable; D3MESH.unk048 -> cameraSpaceMatrix and unk008 -> worldMatrix, both the
0x40-byte D3MATRIX; the old "bounding box" comments were wrong). D3MESH field renames also raised d3_GetBounds
readability as a side effect; ch.c / d3_engine.c / d3_mir.c update sites compile unchanged.
Next session: `uv run ledger grade next` (d3_interface.c next: d3if_CreateCameras, Q2 d3_Unk633438).
## match-base session 2026-10-24 (base-1 worker, shard 1/5, session 2)

Shard 1/5. ls.c: ls_ActionIndex (0x51ad30), ls_ActionAt (0x51ad60), ls_WriteAvatars (0x51bdb0)
byte + readable; ls_ReadRelations (0x51f650), ls_WriteRelations (0x51b890) behaviour + readable.
Project-wide renames: ai_Unk7e5360 -> ai_Relations, ai_Unk889f80 -> ai_RelationMemory (the two
0x300 x 0x300 relation matrices, cm.c 0x4c8240), ai_Unk623650 -> ls_Version (the save version);
0x631050 is ch.h's ch_Queue (CHACTION*), ls.c uses the ls_ActionQueue alias because ch.h cannot
be included there (gv.h's K&R d3sm_GetVertex collides; ord.c calls through it). gv.c:
unk_Unk4ed1e0 -> gv_WriteAvatars, byte + readable (UNK_FW -> vfs_fwrite, bytes unchanged).
Next session: uv run ledger grade next (ls.c next: unk_Unk51a4b0; then unk_Unk51ad80, unk_Unk51aff0).
- Later in the same session: ai_Unk770b80 -> ai_Chars project-wide (348 refs, 16 files; the
  four char-view headers are type debt in lint_baseline.txt), ai_Unk7e3f30 -> ai_BTypes,
  ai_Unk91aaf0 -> ai_Goods, d3t_Unk6310e8 / ai_Unk6310e8 -> d3t_CamMove, ai_Unk47dd70 ->
  ai_WorksForMaster. gv.c: gv_AllocAvatar, gv_FindEffect, gv_ReserveEffect (behaviour),
  gv_EntrySameId / gv_EntryIsFree / gv_EntryIsUsed / gv_FreeGraveyard / gv_EntryHasId /
  gv_LoadGraveyard; f3gm.c: gm_RemoveObjectFromBuilding, gm_Opinion, gm_HealthFactor; ls.c:
  ls_ClearObjectFlag. The base queue of ls.c is 38 -> 32 functions; readable total 911 -> 938.
  Next session: uv run ledger grade next (ls.c next: unk_Unk51a4b0).

## match-base session 2026-10-24 (base-2 worker, shard 2/5, session 3)

Shard 2/5, f3snd.c 14/27 -> 16/27 matched. Byte + readable: snd_PlaySpecialTrack (0x4c23f0),
snd_EndSpecialTrack (0x4c2480), snd_VoiceVariant (0x4c24e0), snd_PlayVoice (0x4c2760),
snd_LoadVoiceBank (0x4c2830), snd_UnloadBuildingVoices (0x4c2c00), snd_PlayWorkerClick (0x4c2c70, was
draft: int random, unsigned short record, the G16CHARCLICK view) and snd_LoadBuildingVoices (0x4c28a0,
was draft: the wrong voice-bank literals fixed, GEHILFEN / ALCHIMIST / HANDWERK). snd_FindVoiceSample
(0x4c2510) -> match-deep (codegen only; lead the real AICHAR view at 0x770b80). snd_QueueVoice became
readable as a side effect. Project-wide renames: the special-track pair and its music/ambient globals,
the voice helpers (snd_FindVoiceSample / snd_PlayVoice / snd_LoadVoiceBank / snd_VoiceVariant), the four
voice-bank handles, ai_Unk631090 -> ch_EmptyName (135 refs, 13 files). Lessons in notes/vc6-matching.md.
Next session: uv run ledger grade next (f3snd.c next: ai_Unk4c2ce0).
## match-base session 2026-10-24 (base-4 worker, shard 4/5, session 3)

Shard 4/5. hmt.c: the unit's base queue is empty. hmt_Update -> match-deep (byte, Q2 only: the three callees are
identified - gm_FindChar / io_FormatText / he_SendEventText - but no spelling is canonical: f3gm.c defines
short *gm_FindChar, ai.h declares AICHAR *, ahm.h / amt.h / cm.h char *, gv.h K&R; he.c calls 0x53dda0 once
with 5 args and cm.c twice with 3 against the 6-param definition, so those sites need the original's pushes as
@original evidence). hmt_Election (175 code hunks, register/frame shape), hmt_Unk4ad580 (unsigned
hmt_PendingElection fixed: the original compares jae; 52 -> 50 code hunks; the rest is the n > 3 / cmp 4 spelling,
the fcomp operand order and the AIMSG store order), hmt_Unk4ad7a0 (loop exit `i != -1` spelling, the wait loop
rotation, fmul/fimul order) and hmt_Create (the character scan's i was unsigned short; the original strength-
reduces an int counter to a pointer walk, int i gave 43 -> 17 code hunks; the rest is the ?: vs if/else shape of
the hmt_Find argument) were escalated.
unk_58bbd0.c (the shard's last unit, 480 base items): ai_Unk58bbd0 (the two grid loops compute % 4 and / 4 with
the signed idioms, not & 3 / >> 2; 242 -> 231 code hunks; the rest is jl vs jb pointer compares and rep stos vs
unrolled stores), ai_Unk58e130 (the 68-dword kind table is a local automatic array - the original does
sub esp,0x110 and initialises every entry on the stack; 255 -> 186 code hunks and the unit's .rdata debt drops by
272 B) and the ROUGH skeletons ai_Unk58bd60, ai_Unk58c680, ai_Unk58cc10, ai_Unk58d720, ai_Unk58dd00,
ai_Unk58e440, ai_Unk58e6a0, ai_Unk58ee30 escalated. ai_Unk58ee30 looked like the same local-table case but the
change made it WORSE (445 -> 470 code hunks, reverted): its table stays static, the 0x114 frame is another local.
Next session: `uv run ledger grade next`.
- Project-wide renames this session: ai_Unk5309a0 / sv_Unk5309a0 -> sv_RunFrame (0x5309a0, "one frame of the
  game screen loop", 443 refs in 16 files; the four declarations - K&R, void *fn, void (*)(void) - collapsed to
  int sv_RunFrame(int cb, unsigned mask), 4 lint baseline lines dropped, C calls 11914 -> 11804) and ai_Unk4c4b70
  -> cm_SendLongMessage (0x4c4b70, the 0xf8-byte AIMSG as message 0x1c, 230 refs in 21 files; the callers use the
  result, so the canonical prototype returns int; 8 baseline lines dropped). ai_Unk58f620 -> ai_PumpFrames (the
  build screen's frame pump, byte + readable, the unit's first base item raised this session).
  Matrix: readable 959 -> 960 (byte + readable 859 -> 860).
- ai_BuildingName (0x58f640): the building-name table walk steps 0x50 bytes (15 groups of name[0x20] + 12 ids),
  not 0x14; 4 -> 2 code hunks. The typed AIBUILDINGREC view cleared Q3 but cost 18 code hunks (reverted).
Next session: `uv run ledger grade next` (unk_58bbd0.c next: ai_Unk58f510; then ai_BuildingName, ai_Unk58f6a0,
ai_FindBauplaetze, ai_Unk58f920).

## match-base session 2026-10-24 (base-0 worker, shard 0/6)

Unit d3_fl.c (19 -> 11 below base). Raised to byte + readable: d3fl_TileLod, d3fl_LightCB. Escalated to
match-deep with reasons: d3fl_SetDetail (n in eax vs edx), d3fl_Alloc / d3fl_GetTexture (TSTEXTURE.unk58
0x58 has no reader anywhere), d3fl_PrepareWater (ramp retest / byte-register switch / slots), d3fl_MakeDivide
(register allocation + the <<6 distribution), d3fl_MakeLight (FPU order, frame, divide-edge groups). Renames:
d3fl_DetailShift, d3fl_DetailSizeShift, d3fl_LightMargin, D3FLMAP.lodDist2/lodDist4/negScale,
D3FLTILE.lodDistBias/centerDist/lodDelay/nextLod. d3_fl.c includes d3_light.h now; the anonymous union at
D3OBJECT/D3OBJX 0x90 flips d3_CloneObject and was reverted (notes/vc6-matching.md). Next session:
`uv run ledger grade next` (d3fl_InitTile; then FreeTiles, LoadMap, UpdateWater, DrawWater, UpdateTiles, Draw,
Prerender, map_Pick, map_PickScreen, d3fl_FarDist in d3_fl.c).
## match-base session 2026-10-24 (base-1 worker, shard 1/6)

Shard 1/6, unit ls.c (queue position 1). ls.c 26/48 -> 15/48 below behaviour + readable; check count
2510 -> 2511. Raised: ls_WriteHistory (0x51c450, behaviour + readable), ls_ReadObjects (0x51d3d0, byte +
readable; the index-form record loop), ls_HookBuildingCB (0x51d4f0, byte, Q4 escalated). Escalated to
match-deep: 0x51b910, 0x51bdc0, 0x51c2f0, 0x51c560, 0x51c870, 0x51c950, 0x51cee0, 0x51d2d0. Details and the
rename list in notes/units/ls.md; the return-block and induction-variable lessons in notes/vc6-matching.md.
Next session: `uv run ledger grade next` (ls.c next: unk_Unk51d530-related items are done; the next base item is
0x51d550).
## match-base session 2026-10-24 (base-2 worker, shard 2/6, session 4)

Units worked: f3snd.c (base queue empty: snd_PlaySelectedClick byte + readable; snd_StartMarketAmbience /
snd_StopMarketAmbience byte + readable; snd_PlayFavourComment and ai_Unk4c2f30 -> match-deep), shp.c
(d2_SetColourFormat / d2_FreeLightTable / d2_SetLineTable byte + readable; d2_MakeLightTable and
d2_Unk479f40 -> match-deep). Project-wide renames: ai_Unk770d08 -> ai_Selected, ai_Unk6f0b80 ->
snd_AmbientTrack, ai_Unk6f1bc8 -> ai_MarketBuilding, ai_Unk4c2ce0 -> snd_PlaySelectedClick, ai_Unk4c2d20 ->
snd_PlayFavourComment, ai_Unk4c30e0 / ai_Unk4c3140 -> snd_StartMarketAmbience / snd_StopMarketAmbience;
d2_Unk479d00 -> d2_MakeLightTable, d2_Unk479df0 -> d2_SetColourFormat, d2_Unk479ee0 -> d2_FreeLightTable,
d2_Unk479f10 -> d2_SetLineTable, d2_Unk6f0450 -> d2_ColourFormatSet, d2_Unkf8a11c -> d2_HighBitMask,
d2_Unkf8a630 -> d2_LightTableExternal, d2_Unkf8a640 -> d2_HighBitMask32, d2_Unkf8a64c -> d2_PaletteDepth.
f3snd.h: ai_Unk6f25b8 is unsigned short, ai_Unk7603a0 unsigned short *[0x20], new packed G16BLDVIEW.
Matrix: readable 1005 -> 1012 (byte + readable 903 -> 909); check 2510/4663 -> 2511/4663.
Next session: `uv run ledger grade next` (shp.c next: d2_Unk47a460; then d2_Unk47a7d0 and the readability of
d2_Unk47c0e0 / the bank functions).
## match-base session 2026-10-24 (base-5 worker, shard 5/6)

Shard 5/6, rt.c only. 17 functions to byte + readable: the route point list helpers (rt_AddPoint, rt_RemovePoint,
rt_FreePoints, rt_CopyPoints, rt_PointIndex, rt_PointAt, rt_PointCount), the mark helpers (rt_ClearMarkGroup,
rt_BuildMarkGroup, rt_MarkValue, rt_ReadMarkValues, rt_UpdateMarkPics, rt_FindMark, rt_RemoveMark) and the control
record (rt_RemoveControls, rt_InitControls); RTPOINT fields cartId / buildingId / loadSlots / unloadSlots, the rt
globals rt_Marks / rt_MarkCount / rt_PickedBuilding / rt_EditRoute / rt_MarkGroup, d2_Unk475980 -> d2_GetObjValue,
d2_Unk46e710 -> d2_RemoveObj. Escalated: rt_SetMarkPic (Q4, rt.h type debt), rt_Unk4bdb30 (slots), rt_Unk4bdcd0
(reg/order shift), rt_Unk4be180 (prologue regalloc), rt_HighlightRoute, rt_Unk4be3b0 / rt_Unk4bf080 / rt_Unk4be940
/ rt_Unk4bf5b0 / rt_Unk4bfc60 (frames/local layout), rt_SaveRoutes, rt_LoadRoutes, and rt_UnloadAtPoint for its
readability (Q2/Q4). rt_UnloadAtPoint is BYTE (check 2510 -> 2511): the price factors 1.1f (0x5ecee8) and
*(float *)(b + 0x49), `pl` is an int, the budget compares are written from the money side - details in
notes/units/rt.md. rt.c's base queue is empty. Next session: `uv run ledger grade next` (gfx.c).

## match-base session 2026-10-24 (base-3 worker, shard 3/6, second pass)

d3_engine.c: 6 functions to byte + readable (d3_DeflateObject, d3_DeflateScene, d3_InflateScene,
d3_ToggleRLightCB, d3_SetRLights, d3_EndFrame; d3_UpdateAll lost its @deep through the same renames), 2 escalated
(d3_InflateObject - whole-function register allocation; d3_Render - Q2 write-only stats), d3_Animate started and
reverted (stays in the queue). Renames: d3_Unk43da00 -> d3_DeflateObject, d3_Unk43db30 -> d3_DeflateScene,
d3_Unk43deb0 -> d3_InflateScene, d3_Unk441db0 -> d3fl_FreeTiles, d3_Unk441ea0 -> d3fl_Inflate,
d3_Unk43e110 -> d3_ToggleRLightCB, d3_Unk43e180 -> d3_SetRLights, d3_Unk633df8 -> d3sm_Shadow,
d3_Unk45e620 -> d3sm_Build, d3_Unk633448 -> d3s_Object, d3_Unkfb9220 -> d3s_NumVerts,
d3_Unkfb925c -> d3s_NumUVs, d3_Unkfe7a88 -> tb_Delay, D3OBJX.unk040 -> offTime. Next session: `uv run ledger grade next` (d3_Animate).

## match-base session 2026-10-24 (base-0 worker, shard 0/5)

d3_tools.c: d3t_GetBoundingSphere behaviour + readable (volatile store / plain compare union view; only the fsqrt
order left); byte + readable: d3t_InflateObjectTree, d3t_CreateRain, d3t_DeleteRain, d3t_DrawRain, d3t_DrawSnow;
escalated d3t_MoveObjectPath (i/k register allocation), d3t_CreateSnow / d3t_FreeSnow / d3t_SetSnow (Q2 fields with
no reader). Reference renames in ecmd.c / ecmd.h, d3_engine.c / d3.h, ts_texture.c. Next session:
`uv run ledger grade next` (d3_tools.c next: d3t_EmitterCB).
## match-base session 2026-10-24 (base-2 worker, shard 2/5)

Shard 2/5. shp.c: the unit's base queue is empty (22 -> 0). d2_ShpBlit (0x47bd70) matched byte (the head hunk
is the clip sum: a local `w = s->w` before the if, lesson in notes/vc6-matching.md) and 15 more functions are
byte + readable through the naming (the whole conversion / bank / mode-draw family, the two asm clip blitters
with their labels and D2BANK.w / h). d2_ShpConvert24To16 got its unsigned guard fix (0 < src[1]). Escalated:
d2_ShpConvert8To16, d2_ShpConvert24To16, d2_BankConvertTo16, d2_StartAnim, d2_DrawShapeScaled. gfx.c:
d2_UpdateInput (0x477ed0), d2_FocusInput, d2_AddHitRect, d2_RemoveHitRect, d2_GetBmpSize byte + readable, the
pointer / drag / hit table names; d2_MouseUpdate -> match-deep.
Project-wide renames: the shp.c and gfx.c lists in the session report (d2_ names; D2INPUT text ids, D2BANK.w/h).
Next session: `uv run ledger grade next` (gfx.c next: d2_Unk478a00; then unk_478c50, unk_478cc0, unk_479150...).

## match-base session 2026-10-25 (base-2 worker, shard 2/6)

Shard 2/6, unit ahm.c: 17/46 -> 23/46 matched, 12 functions raised (9 byte + readable, 2 behaviour +
readable; ahm_FillSteps readable and escalated with the table / step pointer scheduling reason).
Typed views AHMREC / AHMOBJ / AHMCHAR / AHMSEED in ahm.h, the walk state and prototype-table globals
named, the 16-bit util_Random view as a local @original declaration, two stale lint_baseline lines
dropped. Check 2523 -> 2529; readable 1193 -> 1205 (byte + readable 1050 -> 1059). Details in
notes/units/ahm.md.
Next session: `uv run ledger grade next` (ahm.c next: ahm_Unk4eaf50).
## match-base session 2026-10-25 (base-4 worker, shard 4/6)

Shard 4/6, unit unk_5d33b0.c only (the unit's base debt 50 -> 43). 7 functions raised, all byte +
readable: ai_ApplyOptions (0x5d5e90; the branch order, the float multiply, and the last hunk - a named
extern encodes `- 0x40` as `add x,0xffffffc0`, a dereferenced literal as `sub x,0x40`),
ai_ApplyVolumes (0x5d5f10; the Sound option record 0x6f3600 as unsigned char bitfields reproduces the
dword load + `mov dl,ah`), ai_MenuButton (0x5d6f40), ai_GetDefaultEffect (0x5dbc10), ai_SphereVolume
(0x5dc1a0), ai_ClosePokerWindow (0x5da010) and ai_PauseGame (0x5d7e90). Check count 2523 -> 2525.
New include/unk_5d33b0.h (the option globals and the unit's views); the unit includes di_input.h,
f3snd.h and tb.h. d2.h still cannot be included in the unit (its vfs_Root / vfs_Tfopen clash with
vfs.h), so its two prototypes are repeated locally. Renames in the session report below.
Next session: `uv run ledger grade next` (unk_5d33b0.c next: ai_Unk5d5fd0, the graphics options screen, no
body yet; the missing bodies are base meaning work - write them from the disassembly, growing each stub in pieces).
## match-base session 2026-10-25 (base-5 worker, shard 5/6)

Shard 5/6, gfx.c: 9 functions to readable (d2_ReadBmpPixels, d2_LoadBmpSurface, d2_ConvertRgbTo16,
d2_Convert16ToRgb, d2_MakeSurface, d2_GetSurfaceDesc, d2_BlitPartMode, d2_BlitPartToDD; d2_FreeSurface and
d2_BlitPart as side effects). Escalated: the +1 row-pointer fold pair plus d2_Convert16ToRgb, d2_MakeSurface
(the ddsCaps char cast), d2_BlitPartMode (register roles / clip scheduling), d2_BlitPartToDD (RECT store
scheme). D2SURF.external / D2SURF.manualBlit named; DDBLT_WAIT / DDBLT_KEYSRC replace the Q3 literals.
d2.c: d2_Unk14661a0 -> dd_PitchPixels (2 uses), its d2.h duplicate extern removed. Matrix: behaviour +
readable 1091 -> 1095, readable 1193 -> 1203, check 2523/4664 unchanged.
Next session: `uv run ledger grade next` (gfx.c next: d2_Unk479610; then d2_Unk4798f0, d2_Unk479d00, d2_MouseUpdate).

## match-base session 2026-10-25 (base-0 worker, shard 0/6, session 2)

Shard 0/6, unit amt.c (queue position 1). Check count 2539 -> 2542; grade regressions 0. Raised byte + readable:
amt_CheckApplication (0x4e6360), amt_SendOfficeFree (0x4e65e0), amt_SendOfficeCandidates (0x4e6600),
amt_SendOfficeOwner (0x4e6650), amt_SendOfficeMessage (0x4e6680), amt_CheckOfficeMessage (0x4e6710).
Escalated to match-deep with reasons in their @deep tags: amt_ApplyForOffice (0x4e6180, allocation / block order /
deferred add esp / 0xff store form; the meaning was read against the whole original) and amt_AcceptApplication
(0x4e6430, prologue push edi, kind local slot, compare registers). Renames: ai_Unk4c4540 -> cm_BeginGroup,
ai_Unk4c4580 -> cm_EndGroup, ai_Unk4c58a0 -> cm_SendApplication, ai_Unk505df0 -> gm_FindChar (602 refs, 24 files),
ai_Unk4c58e0 -> cm_SendOfficeRecord; the unit's functions named as above. Details in notes/units/amt.md and
notes/units/cm.md; the loop/byte-compare/frame lessons in notes/vc6-matching.md.
Next session: `uv run ledger grade next` (amt.c next: amt_Unk4e67a0).
## match-base session 2026-10-25 (base-1 worker, shard 1/6, session 2)

Shard 1/6. ai.c: ai_Unk47ddf0 -> match-deep (allocation / scheduling: best / bestf stack slots vs edi, the
dead-parameter row offset, the out-of-line return tails; the logic is complete and every hunk was read; the
`uv run spell` dup-tail / reload leads tried, details in notes/units/ai.md). Project-wide renames:
ai_Unk505df0 -> gm_FindChar (602 refs), ai_Unk4c3c00 -> cm_AddFieldDelta (174 refs). Check count unchanged
(2539 / 4664). Next session: `uv run ledger grade next` (ai.c next: ai_Unk47e360, byte, Q1/Q2 only; then the unit's other
byte functions; the ai.c base queue is 47 of 55).
- Later in the session: ai.c ai_Unk47e360 (0x47e360, byte) -> match-deep for the three readability gaps
  (Q1 name, AICHAR.unk008 / unk1b1, the 0x7d5f82 bound; details in notes/units/ai.md) and the companion
  naming: ai_FindPartner, ai_ChooseAction, ai_ExecuteAction, ai_FillNearRing, cm_SendMessage3d,
  ai_BuildingTypeMasks, ai_Partner, ai_ActionFlags, AICHAR.act / mask, AITIME.hour. Check count unchanged
  (2539 / 4664).
- End of the session: ai.c raised to byte + readable - ai_Unk47e840 -> ai_IsLocalWorker, ai_Unk47e8b0 ->
  ai_IsLocalJob, ai_Unk47e940 -> ai_ResetJob, ai_Unk47ec20 -> ai_PlanSlots; escalated ai_Unk47ddf0 (allocation),
  ai_Unk47e360 / ai_UpdateCharacters (Q2 unk008 / unk1b1 / unk024), ai_Unk47e990 (the seven unnamed master
  evaluations). AISLOT make / pending / capacity named; gm_ItemPrice / gm_GoodsPrice / gm_ListAmount /
  gm_ListStock named and their K&R declarations in f3gm.c (x2) and unk_58bbd0.c turned into prototypes, so
  file 0x5087f0 called gm_ItemPrice has one type everywhere (unsigned char n). Matrix: byte + readable
  1125 -> 1130 (behaviour + readable 1168 -> 1173), check 2539 / 4664 unchanged, ai.c 29/55 matched with
  37618 differing bytes (unchanged). shard 1/6 queue next: ai.c 39 of 55 below base.
## match-base session 2026-10-25 (base-5 worker, shard 5/6, 2nd)

Shard 5/6. gfx.c: the unit's base queue is empty - d2_BlitPartFromDD, d2_ClearSurface, d2_CopySurface
(D2SURF.unk00 -> makeFlags), d2_ClearRect, d2_PutPixelRGB and d2_SetSurfaceClip byte + readable, plus the
C++-free Q3s; d2_GetPixelColour (register permutation) and d2_DrawRectOutline (edi/ebp tie-break) escalated.
ecmd.c: 60 of 61 to readable (the Q6 description of every command, the Q2 callee / field names, Q4
flags.all / D3EMITTERCFG.dir, Q7 par_Delete); ocmd_SetCameraToDummy escalated (Q2 D3OBJECT union). The
ecmd.h / cm.h edits recompiled six other units and `uv run build` recorded 8 stale-object draft -> byte
rows there (see notes/units/ecmd.md). Matrix: byte + readable 1125 -> 1191, readable 1284 -> 1352,
check 2539/4664 unchanged. Next session: `uv run ledger grade next` (next unit f3init.c; gfx.c / ecmd.c have
only their unit-header canonicalisation left).

## match-base session 2026-10-25 (base-0 worker, shard 0/6, session 3)

Shard 0/6, unit amt.c (queue position 1). Check count 2548 -> 2549; grade regressions 0. Raised byte + readable:
amt_SetEventOwner (0x4e6b40), amt_SetPersonOwner (0x4e6b70), amt_SendOfficeSwap (0x4e6ba0, was draft) and
amt_ExecuteSwapMessage (0x4e6c50, zero differing instructions; the office-search idiom plus the two record pointers
p1 / p2 - lesson in notes/vc6-matching.md). Escalated to match-deep with reasons in their @deep tags:
amt_ExecuteOfficeMessage (0x4e67a0, message 0x45: out-of-line block order + register roles; Q2 ai_Unk5a8cb0 has no
evidenced name), amt_ClearOffices (0x4e6d80) and amt_ClearOfficesIfAll (0x4e6e20) (induction base &x14 vs &owner,
hoisted -1 register), amt_GetFirstOffice (0x4e6ec0) and amt_GetOffice (0x4e6f30) (one branch's block placement: the
original's jl + inline return 0), amt_GetColumnOffices (0x4e6fc0) (callee-saved push placement only). In progress at
TIME UP: amt_GetColumnOfficesOwned (0x4e7040), readable, 118 -> 103 code hunks, left in the base queue (not escalated).
Typed views: AMTCHARVIEW officeKind / prevOffice / appliedOffice / secondOffice / money and the 0x21c size; AMTSET.idx
unsigned char. Renames: ai_Unk5a9330 -> ai_SendSwapText, ai_Unk4c5fe0 -> cm_SendTradeMessage, ai_Unk770ae4 ->
ai_TimeHour (project-wide). Unit amt.c 25/64 -> 26/64 matched, 12149 -> 11630 differing bytes.
Next session: `uv run ledger grade next` (amt.c next: amt_GetColumnOfficesOwned).

## match-base session 2026-10-25 (base-0 worker, shard 0/6, session 4)

Shard 0/6, unit amt.c. Check count 2560/4664 unchanged; grade regressions 0; unit differing bytes 11630 -> 11511;
readable 1429 -> 1435 (byte + readable 1240 -> 1243). Raised byte + readable: amt_GetColumn7Offices (was
amt_Unk4e70f0, the column-7 offices for the yearly check), amt_GetOfficeHolder (was amt_Unk4e7510, prototype added
to amt.h), amt_GetSuperiorOfficeHolder (was amt_Unk4e7550, the election's reference office holder). Escalated to
match-deep with @deep reasons: amt_GetColumnOfficesOwned (the break spelling fixed the push placement; the loop-entry
test, the out home-slot reload and the rep movs allocation remain), amt_GetOfficeCandidates (was amt_Unk4e7150; the
meaning read whole, 368 -> 352 differing bytes with n <= 0, the continue-form rate test and the pointer walk),
amt_GetOfficeCandidatesFull (was amt_Unk4e7320; 384 -> 359) and amt_CanTakeOffice (was amt_Unk4e75c0; the esi/edi
hoist of b.level and the a/b frame base remain). In progress at TIME UP, still in the base queue: amt_Unk4e76b0
(0x4e76b0), restructured as the original's early return + `n >= 6` (167 -> 116 differing bytes, 183 -> 56 asm lines;
next: the out[] store form and the name). Renames in the session report; lessons in notes/vc6-matching.md.
Next session: `uv run ledger grade next` (amt.c next: amt_Unk4e76b0; then the unit's byte Q1 chain, 0x4e7770 onwards).

## match-base session 2026-10-25 (base-3 worker, shard 3/6, session 2)

Shard 3/6, unit d2.c only: the unit's base queue is EMPTY (24 -> 0). 18 functions raised to byte + readable
(the d2 view-texture pair 0x474ca0 / 474d10 / 474d90, the view enable / placement family 475590 / 475730 /
475850 / 475870, the paintbox family 476220 d2_Plot / 476710 / 476760, the fade family 4768a0 / 476b40,
476b10 / 476110 / 476b70, 475e40 / 475f40 / 475980), 4 raised to readable only (4756b0, 474e80, 4764f0
d2_PlotLine, 4767d0 d2_StartFade, all behaviour draft), 6 escalated to match-deep with @deep reasons
(d2_CloseView, d2_GetObjPicSize Q4-blocked; d2_RedrawView, d2_PlotLine far; d2_PlaceWindow, d2_StartFade
scheduling). Check 2564/4664 unchanged; ledger check --against 0 regressions; readable 1463 -> 1484, byte + readable
1261 -> 1279. VC6 lesson: an equal-block struct-order store sequence (f->x, f->y, f->w, f->h) decides which
parameter loads VC6 hoists (d2_StartFade 21 -> 9 differing instructions). Renames in the session report.
Side work: the tile qnt.c Q7 - qnt_Quantize moved out of include/bmp.h (bmp.c includes qnt.h instead),
bmp.c still 3/3 byte.
Next session: `uv run ledger grade next` (d2.c's base queue is empty; the shard's next units are parked mem.c /
di_input.c, and d2.c's unit readability waits for the d2.h canonicalisation).

## match-base session 2026-10-25 (base-2 worker, shard 2/6, 6th)

Unit ahm.c (base queue 4 -> 3). Check 2567/4664 unchanged; grade regressions 0; unit 33/46 byte,
2414 differing bytes (unchanged).
- ahm_CreateAlchemist (0x4ec500, was ahm_Unk4ec500) -> match-deep: 196 differing instructions, the
  statement set complete, allocation from the first instruction (zero register / -1 / frame base / store
  schedule); `uv run spell` menu assessed, no meaning-class lead left (details in the @deep tag and
  notes/units/ahm.md). Its Q2 gap (ai_Unk144a7a8 / ai_Unk144a80c, the two character-name tables in BSS,
  no evidence) and Q4 gap (54 raw accesses; the unnamed record offsets) are recorded there.
- ahm_AddValue (0x4ecdb0): the clamp rewritten to the `!(f > 0.0f)` / `!(f < 1.0f)` form, 44 -> 41
  differing bytes, 25 -> 12 code hunks, left in the base queue (readable, behaviour draft; the remaining
  hunks are the test schedule). Three other spellings tried and reverted (list in the unit note).
- Also escalated: ahm_WriteRecord (0x4ecf10, the unsigned `<= 0` fwrite checks applied, 135 -> 133) and
  ahm_ReadRecord (0x4ed040, the shared return-0 tail / prologue). ahm.c's base queue is empty; the ahm_*
  record family is with match-deep.
- Renames: ahm_Unk4ec500 -> ahm_CreateAlchemist, ai_Unk4fb890 -> gm_ChooseHeadIcon, ai_Unk4fbd10 ->
  gm_PlaceOnStage, ai_Unk4ff190 -> gm_RefreshGoodsLists (their ahm.h / cm.h / gv.h K&R lines are
  prototypes now; cm.h keeps the int return @original). cm.c's handler ai_Unk4ccf90 stayed byte.
- ahm.c's unit Q7 debt: unk_575860.c still declares ahm_SendCreateAlchemist locally (not touched, other
  shard's file); ahm_AddPlace is byte and fails Q4 only (1 raw `*(int *)(sp + 2)` - the gm_Query record's
  +2 id has no typed view yet).
Next session: `uv run ledger grade next` (unk_4b39f0.c next: ai_Unk4b3bd0 / ai_Unk4b3c10 / ai_Unk4b3c40 are byte and
need names with evidence; the rest of the unit's drafts need the same).
- unk_4b39f0.c (the shard's next unit): readability-led, six functions byte + readable through the mission
  unit's names alone (ai_ClearMissions, ai_MissionRandom, ai_FindOwnerMission, ai_GetMissionType,
  ai_HasMission, ai_CountMissionTypes; ai_Missions / ai_MissionSeed / ai_MissionMode / ai_MissionTypes and
  the helper renames project-wide, reference-only in cm.c / f3init.c / main.c / f3gm.c / unk_58bbd0.c).
  ai_FindMissionType -> match-deep (6 differing instructions, the loop-end constant in ecx).
  ai_NewMission (0x4b3ca0) -> BYTE (check 2567 -> 2568): the type/owner store order in both blocks;
  escalated on Q2 only (G13MIS.unk20, no evidence). ahm.c's base queue is empty; ahm_AddPlace's Q4 is
  the only close base work left there.
## match-base session 2026-10-25 (base-3 worker, shard 3/6, session 3)

Shard 3/6. Check count 2567/4664 unchanged; `uv run ledger check --against` 0 regressions. di_input.c: di_UpdateButtons
(0x410ac0) -> match-deep (the single code hunk read: the queue-scan tail's register choice, meaning right;
readability raised to readable with the DIMOUSEST volatile anonymous-union view for the 4 Q4 casts, bytes
identical; the for-init and comma scan forms re-tried, both 458 B / 180 asm lines WORSE, and the guard comma
form 458 / 176 - the rest is the c2prio register-priority shift). The unit's 15 Q6 descriptions added. mem.c: 6 Q6 descriptions (all 11
functions readable). Renames: none.
- Tried and reverted: ai_Unk4582d0 -> d3q_Create in f3init.c (same address, the call shape matches
  include/d3_quad.h's prototype). The K&R declaration `int d3q_Create();` then collides with the header
  prototype (lint: "declared differently here and elsewhere", not in the baseline), so the rename only works
  together with the declaration -> include replacement in f3init.c.
- The shard's base queue is now EMPTY. What is left at the base level: (1) unit-header canonicalisation of
  the 9 parked units (di_input.c 22, mem.c 54, evt.c 41, txt.c 10, d3_engine.c 77, d3_light.c 17,
  d3_quad.c 6, d2.c 405 foreign declarations; qnt.c has 0 and waits on the no-evidence Q2 of qnt_Free);
  each fix is a declaration -> include replacement in another unit's .c/header, not a rename. (2) the names
  queue (17 Q-only functions of the shard), every one with a recorded no-evidence reason: d3_Unk633438,
  d3_Unkfc2df0 / d3_Unkfc2df4, D3CLIPINFO.unk04, D2INPUT.unk144, D2WIN.unk270 / unk268 / unk3bc, the
  D2WO / D2VO byte-offset macros (Q4, byte-blocked - see notes/units/d2.md).
Next session: `uv run ledger grade next` (empty); the next base work of the shard is the unit-header
canonicalisation, d3_quad.c first (6 declarations: f3init.c x3, he.c, sv.c, cm.h).
- d3_quad.c canonicalisation, the exact sites (checked 2026-10-25): src/game/f3init.c:117 d3q_RemoveObject,
  :121 d3q_FreeNode, :202 `int ai_Unk4582d0();` (0x4582d0 = d3q_Create, K&R - rename + prototype), he.c:14315
  d3q_RemoveObject, sv.c:2852 d3q_RemoveObject, include/cm.h:440 d3q_RemoveObject. The three .c files include
  gv.h / he.h only; d3_quad.h needs the D3OBJECT / D3VECTOR4 / D3BOXVERT typedefs (d3.h) before it, and cm.h has
  no includes at all - check the include order and the full ledger check after each file (a prototype replacing a
  K&R declaration can flip a caller's bytes, see notes/vc6-matching.md).

## match-base session 2026-10-25 (base-2 worker, shard 2/6, 7th)

unk_4b39f0.c: ai_PlayerHeldBuildings BYTE (the condition sums G13STAT.same, not n: 352/352 bytes);
ai_ChooseMission, ai_HasThreeStockedBuildings and ai_MissionSuccessDialog raised to readable and escalated to
match-deep (reasons in the @deep tags); ai_Unk4b58b0 in progress (the constant 1 kept in ebx across the
dialog). Check 2569 -> 2570; readable 1538 -> 1542; unit 49/59 -> 50/59 matched, 1721 -> 1486 differing bytes.
The text engine 0x515960 is io_PrintText now (865 refs), ai_Unk503fa0 / gm_Unk503fa0 -> gm_BuildingTypeOfJob,
ai_Unk4b3e70 -> ai_ChooseMissionType, ai_Unk4b5270 -> ai_PlayerShare, ai_Unk4273c0 -> sb_IsInit,
G13STAT.unk00 -> same, G13TYPE.unk14 -> goal, G13TYPE.unk10 -> speech.
Next session: `uv run ledger grade next` (unk_4b39f0.c next: ai_Unk4b58b0).
## match-base 2026-10-25 (base-4 worker, shard 4/6, third session)

Unit unk_5d33b0.c: 26 -> 24 base items. **ai_CreditsScreen** (0x5d7ba0) byte + readable (the credits
button's scroll screen; three-way frame-rate test, the two loops' increments spelled differently, the
int caller prototype of d2_CreateWindow / d2_WinOpenPic). **ai_MessageFilterScreen** (0x5d7f10) readable
and escalated: the remaining code hunks are VC6's induction-variable strength reduction and the frame
layout (reason in its @deep). Renames of the unit's callees: txt_Print (858 refs), d2_CentreViewWindows,
d2_CloseView, he_IsMessageKindFiltered, he_SetMessageKindFiltered, sv_FramesPerTick. Next session:
`uv run ledger grade next` (unk_5d33b0.c next: ai_Unk5d8080).

Follow-up renames of the same session (byte-neutral): ai_Unk5d8080 -> ai_TailorChatWindow,
ai_Unk4c4a60 -> cm_SendSetField, gm_Unk5092e0 / ai_Unk5092e0 -> gm_BuildingSkill,
he_Unk53dda0 -> he_SendEventText. Check count 2569 -> 2570; 0x5d8080 left in progress (draft) with
its frame / meaning leads recorded in the unit note.

## match-base 2026-10-26 (base-0 worker, shard 0/10)

Shard 0/10, unit sv.c (queue position 1). Check count 2573/4664 -> 2574/4664; `uv run ledger check --against` 0
regressions; sv.c 39855 -> 39657 differing bytes; byte + readable 1324 -> 1325, readable 1554 -> 1555.
- sv_CheckStock (0x5210a0, was sv_Unk5210a0) BYTE + READABLE: the text id is a constant per branch call (VC6
  tail-merges the single call); sv_MessageBox's prototype added (Q7 of its callers in sv.c).
- sv_MessageBox (0x521150) -> match-deep after 212 -> 133 code hunks; sv_MessageBoxGreen (0x521410) ->
  match-deep (73 hunks). Both are the allocation cascade + the D2WIN/D2OBJ typed views + the K&R
  d2_SetObjValue; reasons in their @deep tags and notes/units/sv.md.
- Project-wide naming (the d2 message-box API, byte-neutral): ai_Unk4741f0 -> d2_OpenForm, ai_Unk475f70 ->
  d2_SetForm, ai_Unk46c560 -> d2_RadioAdd, ai_Unk46c630 -> d2_RadioSelect, ai_Unk4741a0 ->
  d2_RaiseViewWindows, ai_Unk46a960 -> d2_WinOpenSlider, ai_Unkfa5aa8 -> d2_RadioSel, ai_Unkf8a668 ->
  d2_ClickedId, ai_Unk1468410 -> kb_Last, ai_Unk475cb0 -> d2_SetObjValue, ai_Unk52ea30 -> sv_FitForm,
  ai_Unk6ed980 -> d2_LastWindowPtr, ai_Unkf8f080 -> d2_Objs, sv_Unk6f2350/51 -> sv_MessageBoxOpen /
  sv_MessageBoxGreenOpen. The void-returning ones needed their `int f();` lines turned into prototypes (the
  names met cross-TU conflicts); lint X 13/17 pairs -> 0/0.
- Reverted, not committed: the `ai_Unk6333cc -> d3_Time` rename (see notes/vc6-matching.md).
Next session: `uv run ledger grade next` (sv.c next: sv_MessageBoxYesNo 0x5216a0, then the unit's other drafts).
## match-base 2026-10-26 (base-3 worker, shard 3/10)

Shard 3/10 owns d3_quad.c, mem.c, qnt.c, sndc.c, txt.c, zlib/crc32.c, zlib/gzio.c (72 functions); the base
queue is empty - d3_quad / sndc / crc32 / gzio are byte + readable, the other three have only deep-queue
functions. The seven (eight with qnt_Free) below-byte functions were re-read against their sources and the
@deep reasons now carry the current `uv run ledger grade` classification and unitdiff numbers; pool_Next and qnt_Nearest
got their reasons (byte work only). One new spelling (txt_ParseRandomText's reversed if polarity) is WORSE;
qnt_Build's for / while / do / fresh-variable loop shapes compile identically. mem.h gained the canonical
mem_Alloc / mem_Free prototypes; the 54 foreign declarations still need d3_engine.c (shard 7/10) re-matched
first, so the sweep is not mechanical yet. No renames. Next session: uv run ledger grade next (empty) - the base
work of the shard is the mem sweep once d3_engine.c's d3_CloneObject is byte under mem.h's TUs content.
## match-base 2026-10-26 (base worker, main checkout)

Check 2593 -> 2596/4664; byte + readable 1376 (unchanged readability total 1619; the renames cut Q2 debt only).
Units: unk_4b39f0.c (two escalations), unk_4b6480.c (56/66 -> 59/66, 1893 -> 1414 differing bytes).
- byte: ai_Unk4b82c0 (the `(c ? 0x230 : 0x20d) + title` ternary spelling - the flat `(c ? 0x23 : 0) + 0x20d +
  title` is one lea), ai_Unk4b9f30 (same idiom), ai_Unk4ba840 (the guards as one nested if with a shared
  `return 0`).
- near-miss gains: ai_Unk4b84b0 54 -> 11 (then escalated for the util_Random argument-push schedule),
  ai_Unk4b9cd0 56 -> 42 (`buf[0x1f0]` -> `0x200`, the swapped guard `if (util_Random(0x32) > f) { ... }`).
- escalations: ai_MissionCompleteDialog (h spilled to a slot, frame 0x194 vs 0x190), ai_MissionResultDialog
  (one ebp/ebx colour swap), ai_Unk4b84b0 (argument-push scheduling).
- renames (byte-neutral, evidence in the definitions' comments): ai_Unk4fca40 / unk_Unk4fca40 -> gm_AddHealth
  (0x4fca40), ai_Unk50c550 / unk_Unk50c550 -> gm_Liking (0x50c550), ai_Unk4b8c80 -> ai_MsgJobTime (0x4b8c80).
- lesson: "nested if + one return" and "the ternary of the two sums" are cheap base wins; register/slot spills,
  colour swaps and argument-push scheduling are the match-deep boundary.
Next session: `uv run ledger grade next` (ai_Unk4b9cd0's AIMSG store schedule, or the next unit's drafts).

## match-base 2026-10-26 (base worker, main checkout, second session)

Check 2596 -> 2599/4664 (byte + readable 1376 -> 1378, readable 1619 -> 1621); `uv run ledger check --against` 0
regressions; unk_4b6480.c 59/66 -> 61/66 matched and 1414 -> 750 differing bytes, unk_58bbd0.c 121798 ->
121775. Unit unk_4b6480.c: base level 53 -> 48.
- ai_InventoryHas (0x5ba6b0, was ai_Unk5ba6b0) **byte**: the ROUGH stub's missing body (and its wrong gm_Query
  arguments) written from the disassembly; this also clears the Q2 blocker of 0x4b6fd0 / 0x4b7430. Its role
  name: the callers call the ids skills and the object is the character's inventory item.
- ai_MsgLoseGood (0x4bacc0) **byte + readable**: the eligibility byte was the good's `type`, not byte 9.
- ai_Unk4bb250 **byte** (the 10-byte calendar record; the frame was 8 short), -> match-deep for Q1/Q2/Q4.
- ai_Unk4ba0c0 **behaviour** (FP compare operand order, shared 0x400 exit), -> match-deep for Q1 + the AIMSG
  text-id view.
- Readable + handed over: ai_MsgWorkerStop (0x4b9cd0, scheduling), ai_Unk4ba630 (allocation: k in ebp).
- Renames: ai_Unk5ba6b0 -> ai_InventoryHas, ai_Unk53dee0 / he_Unk53dee0 -> he_MessageBoxQuickjump (its own
  error string names it), ai_Unk4b9cd0 -> ai_MsgWorkerStop, ai_Unk4bacc0 -> ai_MsgLoseGood.
- Lesson: the frame / offset difference of a far draft is often ONE local's type (0x4bb250's 10-byte calendar
  record, 0x4ba0c0's FP temp): read the out-parameters' callee before rewriting statements. Next session:
  `uv run ledger grade next` (unk_4b6480.c next: ai_Unk4b9f30, then the unit's other drafts).

## match-base 2026-10-26 (base-3 worker, shard 3/6)

Shard 3/6 owns d2.c, mem.c, d3_engine.c, d3_light.c, qnt.c. The base queue is empty (0 draft units, 0 readability
units; the 13 Q-only names items are all `@deep` with recorded evidence gaps), so the session took the Q4
raw-access class, where a typed spelling can be byte-neutral: d2.c's D2WO / D2VO macros became the pointer views
D2WOP / D2VOP (`((T *)(base + off))->field` is the same AST as `(*(T *)(base + off)).field`), which made
d2_CreateScrollButtons, d2_ScrollWindowBy and d2_CloseView byte + readable; d2_GetObjPicSize got a local
`int *frameOffsets` (byte + readable); D2OBJ.unk1e4 -> fillFraction (drawn fill percentage) raised d2_DrawInput to
readable; D2WIN.unk270 -> backgroundState (background save/restore state) raised d2_DrawWindowBackground to byte +
readable and d2_DrawWindow to readable. 7 readability rises, 5 @deep tags cleared: byte + readable 1479 -> 1484,
readable 1757 -> 1764, behaviour + readable 1540 -> 1545, check 2610/4664 unchanged, `ledger check --against` 0
regressions, ledger check 0 errors, no unitdiff WORSE.
Blocked, both now with the exact evidence: d3_light.c's include/d3.h declaration (removing it - or routing it
through a tail include of d3_light.h - flips d3_mir.c's byte d3_MirDrawObject) and its d3_anim.c local prototype
(the include is WORSE for d3anim_CreatePath / d3anim_CreateMorphAnim); mem.c's 54 foreign declarations (same
class, d3_CloneObject). d2.c's 413 and d3_engine.c's 78 need their canonical headers self-contained first
(d2.h has no includes; d3_engine.h's void* callee views). Next session: `uv run ledger grade next` (empty for the
shard's units) - the shard's remaining base work is those header cascades, each gated on another shard's byte
functions, or new evidence for the 13 Q2 gaps.
- Clean-rebuild check (rm -rf build/obj, full build): 2610/4664 again, no REGRESSED row, link 0 differing bytes,
  12 baseline differences (the 7 readability rises + 5 @deep clears), so the session's numbers are not stale-object
  artefacts.
