# net.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Worker "net": 0x41f000..0x426000 [net]

### TU boundaries found (inferred from content, callers and string tags)
| range | file | content |
|-------|------|---------|
| 0x41ebc0..0x4206d0 | unk_41ebc0.c (from 0x41f000 here) | surface scale/convert; 0x41ebc0/0x41ec50/0x41ee70 (8/16/24-bit box filters, dd_vesa worker's range) are called from surf_Unk420140 and very likely open this TU |
| 0x4206d0..0x4214a0 | unk_4206d0.c | octree color quantizer |
| 0x4214a0..0x4223c0 | (no source) | error handler / log ("Error-Handler", "_error.log"), left without source |
| 0x4223c0..0x422c90 | unk_4223c0.c | tracked malloc ("m_alloc_debug") + pools ("m:pool_...") |
| 0x422c90..0x423770 | unk_422c90.c | DirectShow movie playback ("MOVIEWNDCLASS"); 0x422c90 is the WndProc (not a Ghidra function) |
| 0x423770..0x424510 | msx.c | D:\develop\units\Msx\msx.c; 0x423770 is the update thread (not a Ghidra function) |
| 0x424510..0x424f30 | net.c | D:\develop\units\net\net.c |
| 0x424f30..0x425bc0 | unk_424f30.c | A* path search |
| 0x425bc0..(0x427390+) | unk_425bc0.c | sample library ("sb_Init"); continues past 0x426000 |

### Nonmatching (attempts)
- match 2026-10-22, nothing new: net_Recv (rxlen / rx loaded and added in a different order: statement swaps, `l` temp for the new
  rxlen, p temp for rx all same or worse); net_Connect unchanged.
- net_Connect: 2 asm lines; `or eax,-1` scheduled before the pops and the host==NULL return jumps into the
  failure epilogue. Tried int return type, return -1/INVALID_SOCKET/net_Conn.s, s = INVALID before cleanup.
- net_Recv: early-return/merge fixed with `goto ret1`; left: the received packet pointer is reloaded into
  eax after the byte counters (ours keeps ecx), sequence-number branch registers. Tried branch reorders, goto
  forms, seq/branch nesting.
- msx_Update: only the induction pointer base differs (orig &t->volume, ours &t->fadepos). Tried index vs
  pointer loop, direct msx_Tracks[i] access, locals for fadepos/volume.
- mov_Open: register allocation (orig keeps 0 in esi as `id`, file in ebx); structure (id = 0 / return id /
  fail label after the final return) is right.
- mov_Find / mov_FindWnd: orig materializes NULL in ecx before the list test (`xor ecx,ecx ... mov eax,ecx`);
  ~12 loop/return spellings tried.
- pool_Next: inner slot-scan loop is not rotated in the original and slot/size registers are swapped.
- qnt_InitTables (byte-register partial ops), qnt_Build (addressing operand order), qnt_MakePalette (load
  order), qnt_Nearest (b/g register swap), qnt_Map (big, error diffusion; same size).
- surf_Down32, surf_Up16/24/32 (bilinear; ours smaller), surf_Unk420140 / surf_Unk4205c0 (esi/edi swap of
  pal8 flag and width).
- path_Search1/2, path_Expand, path_Reverse, sb_Update: first drafts from the pseudocode.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (leaked): D:\develop\units\net\net.c
- Boundaries: separated (plausible): leaked; end next to path.c (plausible)
- Range then: 0x424510..0x424e70

## Open work
- net.c (match-tu 2026-10-23, 2nd run): net_Connect 2 lines / net_Recv 70 lines unchanged. Rungs: respell (14 / 31 variants, no gain), idioms none, siblings none. net_Recv c2prio: `_net_RecvBytes` has two webs (first recv block, tail), both prio 8, both printed without a register although the first block compiles with ebx; the tail web is the one that ends in ecx (scratch). Block 11 (lines 257-259, the tail adds + compare) has P=4. Lead: raise the tail web's priority with a REAL extra reference of net_RecvBytes or rxlen in block 11 (code-free references fold: see di_input.md), or find why the first web is allocated and the second not (c2prio --ir on both blocks). net_Connect: the 2 lines are which epilogue the host == NULL return merges into (ours the failure one after its `or eax,-1`, the original the success one); the layout orders tried are in the entries below; lead: a form where the failure epilogue precedes the success return in the IR (e.g. the loop's success `return s` as the last statement with the failure path above it under a label).
- match 2026-10-23 (8th), no gain: net_Recv tail with all valid orders of `rb = net_RecvBytes + n; l = rxlen + n; rx = net_Conn.rx; stores` (5 statements, 60 variants with two compare spellings): best 91 B / 81 lines (same family as the recorded 93 B). The original's second block uses ebx / cx for the sums exactly like the first block, ours ecx / dx: the sums are callee-saved candidates in the original (live across the `rx` load that precedes both stores).
- match 2026-10-23 (5th): net_Recv tail with a `NETPKT *rx` local and every spelling of the compare and of the later `net_Conn.rx->` uses (798 variants): no gain; the unsigned short sum temp (93 B / 81 lines) stays the closest in bytes. The original reloads `net_Conn.rx` into eax after every sprintf call and keeps the RecvBytes sum in ebx.
- match 2026-10-23 (4th): net_Recv tail, no gain: `rx` local (NETPKT *) taken before the adds (either store order, 143 B), `(unsigned short)` compare cast, `rx->len != (rxlen += n)`, a `unsigned short l = rxlen + n` temp stored once (93 B but 81 lines: worse shape, the sums use lea). The first recv block (same `RecvBytes += n; rxlen += n`) already matches with ebx / cx, so only the tail's placement of the `rx` load (original: after the adds, before both stores, into eax) differs.
- match 2026-10-23 (3rd): net_Recv: the original keeps the RecvBytes sum in ebx (a callee-saved register: probably the net_RecvBytes web as an allocation candidate, see uv run c2prio) and the rxlen sum in cx, and loads rx into eax before both stores. Tail forms without change (143 / 148 B): `(rxlen += n) != rx->len`, `rx->len != rxlen` (both store orders), `x = x + n`, `(short)n` casts. Next: c2prio on net_Recv (which web / temp gets ebx).
- match 2026-10-23 (3rd): net_Connect cause narrowed: the 8 B are only the position of `or eax,-1` in the failure epilogue, and it follows from the host == NULL jump: ours jumps into the failure epilogue just after its `or eax,-1` (eax is known -1 from the INVALID_SOCKET store), so the `or` cannot be scheduled into the pops; the original jumps to the success epilogue (after `mov eax,esi`), leaving the failure epilogue label-free. Tried without change: `s = INVALID_SOCKET; net_Conn.s = s;` (+ `return s` on NULL: 523 B), `net_Conn.s = s = ...`, NULL return `return net_Conn.s` / `goto done` with success `return net_Conn.s = s` / `net_Conn.s = s; return net_Conn.s` / `goto done` and `done: return net_Conn.s;` (24 forms), the success code moved to the end of the function behind `goto connected` (same layout as now).
- match 2026-10-23 (2nd): net_Recv no gain: the original evaluates `rx` (eax, reusing n's register) right after the two adds and before both counter stores, then compares the register sum `cx` with `[eax+1]`. Tried perm of the two `+=` and the compare, `(rxlen += n) != rx->len`, `rx->len != rxlen`, `x = x + n` spellings (143 B at best). net_Connect: same 8 B (original: pops, then `or eax,-1`, then `pop ebx`).
- match 2026-10-23: net_Connect: the original has two epilogues: host == NULL jumps to `pop edi/esi/ebp/ebx; add esp; ret` at the end of the function (eax is still -1 from the `net_Conn.s = INVALID_SOCKET` store), the failure path has its own `pop pop pop; or eax,-1; pop ebx`. Ours shares one (the NULL return jumps into the failure epilogue, `or eax,-1` before the pops). Tried: whole body nested in `if (host != NULL)` with `return INVALID_SOCKET` / `return net_Conn.s`, a `ret` local, `goto done` (all 8 B or worse).
- match 2026-10-22 (2nd pass): net_Recv 148 -> 143 B (`net_Conn.rxlen += n; net_RecvBytes += n;` swapped in the tail; the original loads RecvBytes, rxlen, then rx and adds in ebx / cx). Still nonmatching: the original keeps the packet pointer in eax from the load after the adds (ours: ecx loaded before them); a local `c = net_Conn.rx` assigned after the adds, before the first test (third form: 303 B worse), `x = x + n` forms, `(net_Conn.rxlen += n) != ...`, compare operand swap: no change. net_Connect unchanged.


## match-base 2026-10-23 (match worker)
- net_Error, net_ClearStats, net_Count, net_PrintStats, net_Close: descriptions only (byte + readable).
- net_Connect: behaviour + readable. Q4: `*(u_long *)he->h_addr_list[0]` -> `memcpy(&addr.sin_addr, he->h_addr_list[0], sizeof(addr.sin_addr))` is byte-neutral (the same single load); the 8 order hunks stay (see Open work).
- net_Recv: readable, escalated for behaviour. Its unnamed pieces are now named: net_Unka687e0 -> cm_LastSyncCount (cm.h calls 0xa687e0 "cm_LastSyncCount"; cm.c sets it from the last Sync message, type 0x20 / data[0] 0x0e) and NETPKT.unk04 -> idx (cm.h's G17MSG view has idx at 0x04, "its slot in the send ring"; -1 = not sequenced). Reading the Ghidra pseudocode 0x424c80 showed no meaning difference (the seq bookkeeping assigns the same values in every branch); the code hunks are the register allocation around `rxlen += n; net_RecvBytes += n;` (the original keeps the RecvBytes sum in ebx, the rxlen sum in cx and reloads rx into eax), the block order of the three sprintf branches, the dword clear of rxlen (ours stores the word) and 7 tail nops. The leads in Open work apply.
