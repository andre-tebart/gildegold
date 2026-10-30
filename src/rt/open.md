# rt.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the findings of each session, one section per author.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (tag): cart routes: route point lists (0x40 bytes, at most 8), the marks on the town map, the routing dialogs (choose good, edit point, settings), the route editor 0x4bf5b0, loading / unloading at a point, save / load
- Boundaries: separated (proven): tag "rt_point" / "rt_load" (mem_Alloc). .data: a -1 global 0x602a90 (radio group) heads its block right after rd.c's literals, then "rt_point", "misc\routing_*", "$Z%2N3$A%1G", "rt_load" 0x602b00; f3snd.c's block follows (0x602b08: 1.0f, "Boehe_*"). All drafted (layout 2026-10-22 session 8)
- Range then: 0x4bd950..0x4c01b0

## Open work
- **headers (2026-10-30, typing pass 2)**: head.c includes d2_types.h, txt_views.h, rt.h, di_input.h, data_views.h, d3_types.h; keeps 2 cast / view macros with @original byte evidence (the address and the matched caller are in each comment): d2_SetObjValue4, cm_SendExSellObjectI; views / casts without byte evidence of their own (weak definitions or drafts' spellings, notes/headers.md debt item 1): d2_ObjsC.
- Near-misses: 0x4bff40 (96 B: save loop), 0x4bfa40 (133 B), 0x4bdb30 (132 B, behaviour: stack slots).
- Far drafts (Ghidra lost the frames of 0x4be940 and 0x4bf5b0; drafted from the disassembly): 0x4bdcd0, 0x4be180,
  0x4be3b0, 0x4be790, 0x4be940 (original size), 0x4bf080, 0x4bf440, 0x4bf5b0 (map window struct as `int m[0x9ea]`),
  0x4bfc60, 0x4c0040.
- 0x4be3b0 (new, match-base 2026-10-24): our frame is 0x1d0, the original's 0x2e4 (0x114 = 69 ints more locals)
  and a branch reads another offset: ours pushes `[eax+0x5d]` on one arm where the original pushes `[eax+0x14]`
  and reads `[ecx+0x5d]` on the other -> the stock list is built from a different source in one arm; the local
  arrays are too small. Frame + branch need a real reread (match-deep queue).
- Unsure: 0x4bf5b0 / 0x4bf080 pass a byte v to 0x4c3d90 (0x4bf5b0: 0 from ebx), 0x4bfa40 prices with
  ai_Unk6f1bc5 in both arms (Ghidra), 0x4be180's loop exit condition.
- Type debt: rt.h's views (notes/layout.md, session 8). rt.h repeats d2/ai declarations with other signatures
  than d2.h/ai.h (d2_Unk4731d0 int vs short), so no TU can include both; the rt_SetMarkPic Q4 (D2OBJ.pic) needs
  that cleanup.
- Header cleanup for the unit's readability: cm.h:323 rt_PointCount now matches rt.h; cm.h:523/524 still declare
  the functions of 0x4bd9e0 / 0x4bda00 as ai_Unk4bd9e0 / ai_Unk4bda00 (cm.c, f3gm.c call them), f3gm.c:3386
  repeats ai_Unk4bd9e0, ls.c:75 ai_Unk4bff40, sv.c:3065 ai_Unk4bf5b0 -> use rt.h (rt_FreePoints, rt_CopyPoints).

## layout 2026-10-22 (session 8)
- RTPOINT (0x40, "rt_point"): unk00 the cart id, unk04 the building id, unk08[6] three load slots {good, amount},
  unk20[6] three unload slots, prev / next. Carts hold their route at +0x29, the current point at +0x2d.
- RTMARK rt_Unkf812a0[8] {d2 picture, point}; rt_Unk602a90 their radio group (.data -1); rt_Unk6f0750 marks used,
  rt_Unk6f0758 the route being edited, rt_Unk6f0754 the building picked on the map.
- Loops over rt_Unkf812a0 by pointer with a signed end compare: `(int)m < (int)(rt_Unkf812a0 + 8)` (0x4bdae0,
  0x4bdca0, 0x4bf3b0 matched).
- 0x4bd950: `do { q = p; i++; p = p->next; } while (p != 0); if (i >= 8) return 0;` (the zero is the loop's p).

## match-base 2026-10-24 (base-5 worker, shard 5/6)
- Named and made readable (all byte the whole time, 17 functions now byte + readable): the route point list
  helpers rt_AddPoint / rt_RemovePoint / rt_FreePoints / rt_CopyPoints / rt_PointIndex / rt_PointAt /
  rt_PointCount, the mark helpers rt_ClearMarkGroup / rt_BuildMarkGroup / rt_MarkValue / rt_ReadMarkValues /
  rt_UpdateMarkPics / rt_FindMark / rt_RemoveMark, the control record rt_RemoveControls / rt_InitControls.
  rt_MarkValue (0x4bdc90) is a tail-call getter (`jmp d2_GetObjValue(0x475980)`); keep the return if it is ever
  given one (the callers ignore it).
- RTPOINT fields named from use: unk00 -> cartId (ai_Unk4fe980 cart lookup, the save id), unk04 -> buildingId
  (ai_Unk502020, -1 when new), unk08/unk20 -> loadSlots / unloadSlots (three {good, amount} pairs, see 0x4be940).
- Globals: rt_Unkf812a0 -> rt_Marks, rt_Unk6f0750 -> rt_MarkCount, rt_Unk6f0754 -> rt_PickedBuilding,
  rt_Unk6f0758 -> rt_EditRoute, rt_Unk602a90 -> rt_MarkGroup.
- Callee renames: d2_Unk475980 -> d2_GetObjValue (object value: input text/value, slider value), d2_Unk46e710 ->
  d2_RemoveObj ("removes the object", d2.c's own comment).
- Meaning fixes: rt_UnloadAtPoint (0x4bfa40) is now BYTE (check 2510 -> 2511). The price is multiplied: the
  market arm (type 10) and the warehouse arm (0x1c, in the load 0x4bfc60) by the float at 0x5ecee8 (1.1f,
  `fmul ds:0x5ecee8`), the other arm by `*(float *)(b + 0x49)` (a building price factor, inside AIBLD's pad3d);
  the draft had both / three arms identical. `pl` is an int, not an unsigned char (the original pushes it
  unmasked). The comparisons are written from the money side (`ai_Unk509ee0(o, pl) >= price * n`,
  `n >= ai_Unk50aaf0(...)`): that swapped the compare operands to the original's order and took 4bfa40 to byte.
  The same price factors are in rt_LoadRoutes's draft (0x4bfc60: 120 -> 67 code hunks).
- Named: 0x4bf440 -> rt_HighlightRoute, 0x4bfa40 -> rt_UnloadAtPoint, 0x4bff40 -> rt_SaveRoutes,
  0x4c0040 -> rt_LoadRoutes.
- Escalated: rt_SetMarkPic (0x4bf360, Q4 only: D2OBJ.pic through the char* d2_Objs view), rt_Unk4bdb30
  (behaviour: the statements are all there, the hunks are stack slots - frame 0x20 vs 0x28 and a different
  local layout), rt_Unk4bdcd0 (behaviour: one stream shifted 0xf, register allocation and argument spills),
  rt_Unk4be180 (behaviour: prologue register choice only; `int n = 0;` declaration initializers tried, no change),
  rt_HighlightRoute (scheduling + Q4), rt_Unk4be3b0 / rt_Unk4bf5b0 / rt_Unk4bf080 / rt_Unk4be940 / rt_Unk4bfc60
  (frames and local layout), rt_SaveRoutes (the five vfs_fwrite tests: original tests `> 0` with `ja` to the body,
  ours `!` with `je` to the return), rt_LoadRoutes (168 hunks, frame 0xc vs 0x8 and register allocation),
  rt_UnloadAtPoint (readability: Q2 stock/cart API names, Q4 typed views - byte otherwise).
- cm.h's rt_PointCount view now matches rt.h (`struct RTPOINT *`); lint baseline updated.
