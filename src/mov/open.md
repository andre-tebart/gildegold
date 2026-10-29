# mov.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Also about this unit
- notes/units/net.md, section [net]

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (guess): DirectShow movie playback, "MOVIEWNDCLASS"
- Boundaries: separated (plausible): coherent DirectShow playback ("MOVIEWNDCLASS"), own string block 0x5f7470.. between mem.c and msx.c
- Range then: 0x422c90..0x423740
- Was: unk_422c90.c (round 12)

## Open work
- match 2026-10-23 (4th): mov_Find 7 -> 5 asm lines (26 B) with `MOVIE *m = mov_List; if (m != NULL) { do { if (m->id == id) break; m = m->next; } while (m != NULL); return m; } return NULL;` (the loop-exit return then shares the found `ret` like the original). Left: only the hoisted `xor ecx,ecx` / `mov eax,ecx` for the empty-list return (ours `xor eax,eax` at the end). Also tried: 14 more forms (zero locals r / z with compares against them, `return r` / `return m` / `return 0` in the empty path, for(;;) with two `return NULL`): 25 B / 7 lines or worse. The same form on mov_FindWnd gives the same code (+1 B, same lines): not applied (its 17 lines are the original's 14 pad nops after the function).
- match 2026-10-23: mov_Find: 7 forms (early `if (!m) return 0` + do/while, return m after the loop, `return z` of a zero local, nested if, break + return m): all 25 B; c2prio: const#0 gets no register in ours (prio 3, only block 4 / 7 returns cost 1). mov_Open: first hunk is a register map shift (name ebx / zero esi / m ebp in the original, ours name ebp / zero ebx); `int id = 0` is a real stack store in ours, absent in the original.
- match 2026-10-22 (2nd pass): mov_Find / mov_FindWnd: the original is `mov eax,[mov_List]; xor ecx,ecx; test eax,eax; je Lret0 ... ret; Lret0: mov eax,ecx; ret`: only the empty-list path returns the zero register, the loop exit returns eax; not reproduced by r = NULL / do-while / for forms.
- match 2026-10-22: mov_Find: the original keeps a zero in ecx (`xor ecx,ecx` after loading mov_List, empty list returns it); `r = NULL` locals, do/while, while forms: no change.

## match-base 2026-10-24 (match worker)

- Readable now: mov_WndProc, mov_Init, mov_Exit, mov_Play, mov_GetEvent, mov_Close, mov_Alloc, mov_Free
  (Q6), mov_Release (MOVIE.unk134..unk144 -> iface[5]: released, never obtained by this file).
- mov_Find and mov_FindWnd **byte**: `MOVIE *m = mov_List, *found = NULL; if (m != NULL) { do { if (...)
  break; m = m->next; } while (m != NULL); found = m; } return found;`. The assignment AFTER the loop (not
  inside it) is what the earlier 14 forms missed: VC6 keeps the constant zero in ecx from entry (the
  empty-list return is `mov eax,ecx` in the original) and the found/exhausted paths return m through found.
- mov_Open -> match-deep: 460 code hunks, the whole register map shifted (MOVIE pointer ebp vs our ebx),
  frame 4 bytes larger, `int id = 0` is a stack store in ours and absent in the original.

## match-base 2026-10-24 (base-4 worker, shard 4/5)
- Unit readability now: main.c declared mov_Exit as ai_Unk422d90 (twice called); main.c includes mov.h
  (<objbase.h> / <strmif.h> / <control.h> first) and calls mov_Exit. No code changed (main.c 17/58 before
  and after). Behaviour stays draft (mov_Open is with match-deep).
