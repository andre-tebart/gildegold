# dd_vesa.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (leaked): D:\develop\Units\dd_vesa\Dd_vesa.c
- Boundaries: separated (proven): layout 2026-10-22: unk_418ce0.c joined (.data: dd_vesa\'s -1 at 0x5f3c48 heads the block, then d3d_Error\'s texts 0x5f3c50..0x5f6298, then the Dd_vesa.c path 0x5f65e0; byte-checked); start after d3_snd.c\'s literals, end next to surf.c (plausible)
- Range then: 0x418ce0..0x41eb00
- Was: unk_418ce0.c joined layout 2026-10-22

## Open work
- Header cleanup: Dd_vesa.c globals/functions are declared both in d3_d3d.h (with D3DCARD) and dd_vesa.h;
  d3_d3d.c hides two wrong d3.h prototypes with `#define d3_Unk4385a0 d3_Unk4385a0_int` (d3_Unk4385a0,
  d3_Unk4387b0 take unsigned char; d3_ForEachObject returns char): fix d3.h and drop the renames.
  SURF_COPY (unk_41ebc0.h) vs D3_COPY. msx.h's snd_* sound prototypes are the sfx_* of unk_42b720.h (mapping in
  notes/units/evt.md). 0x4014d0 is bmp_Load (unk_401410.h) and util_LoadImage (d3_d3d.h). d3_engine.h declares the
  file_* helpers 0x4011a0..0x401240, which unk_401000.h names bio_rd_*/bio_wr_*.
- d3d_GetDeviceInfo 0x41c080: matched 2026-10-23 (928 B). Root cause of the old const-2/ebx cascade was
  statement 927's spelling: `dwShadeCaps & 0x5000 ? 1 : 0` compiles to `setne`, while the original's
  `(dwShadeCaps & ALPHAGOURAUDBLEND) || (dwShadeCaps & ALPHAFLATBLEND) ? 1 : 0` is merged by VC6 into one
  `test dh,0x50` but keeps the branchy 0/1 result in ebx (`xor ebx,ebx` / `mov ebx,edi`). That ebx temp
  is what keeps the const-2 web out of the TRICAPS block (so 919's shift gets ebx, no byte spill, 918
  tests memory). After that only the 934-936 order remained: 934, 936, 935 (`unk334 = 1` after the
  `unk30c_4` store). `tex` is a plain `int` again; the volatile was compensating for 927.
- d3d_EnumDeviceCallback 0x41c460 (match-tu tooling session 2026-10-23): still 279 B / 39 lines; what the slot
  maps say (uv run listing prints them: `__cn$NNN = 24`). Original: HW branch _cn 28 (ctx), _cd 24 (hel) then
  _cn 28, _cd 8 (guid); HEL branch _cn 20 (hw), _cd 28, _cn 20, _cd 28. Ours: HW _cn 24 / _cd 28, _cn 8 / _cd 12;
  HEL _cn 20 / _cd 28, _cn 20 / _cd 8. Declaration order and temp names change nothing (byte-identical), so the
  packer ignores symbol order. Scope experiments: a function-scope `int _cn` (macro assigns it) reproduces the
  whole HW branch (28 / 24 / 28 / 8) but keeps 28 in the HEL branch too (33 lines); a branch-scope `_cn` gives
  the HEL branch's 20 but 24 in HW; mixed per-branch scopes reach 31 lines (branch-scope `_cd` in HW +
  branch-scope `_cn` in HEL; or branch-scope `_cn` in HW + both temps branch-scope in HEL), src-through-temp
  (D3_COPY3) and both temps function-scope are far worse (57-235 lines). Not kept (needs per-function macro
  forms for 8 lines). Lead: the original looks like ONE `_cn` per branch and ONE `_cd` in the HEL branch but two
  `_cd`s in HW; whatever packs that is not scope alone, so try the D3_COPY argument shapes next (dst as a
  pointer local for one copy, `&d3d_Devices[d3d_NumDevices]` vs `.guid`), then c2prio's order.
- dd_SetMode 0x41d2e0 (same session): the `> 8` spelling matches the first 327 instructions byte for byte and
  the whole rest of the difference is one scratch-register swap: the constant-8 web and the constant-0 web have
  eax / ecx exchanged (original `mov eax,8` for the 24/32 stores and the compare, `xor ecx,ecx` for
  `dd_MemBottom = NULL` / `dd_BShift = 0`; ours ecx / eax), and the swap begins at `if (biHeight > 0)` in the
  GDI case (original `mov eax,[biHeight]; xor ecx,ecx`, ours ecx / eax): the scratch round-robin in the block
  after the D3_ZERO_AT tail. `>= 9` only hides it (58 vs 93 lines total) by keeping the compare out of the web.
  Next: change the IR order / temp count of that block (the biHeight test spelled on `yres` or on a local, the
  `dd_MemBottom` arms, the D3_ZERO_AT size expression) and read the rotation with uv run listing. uv run c2prio
  --ir on this function stops after block 31 (lines 1362-1407; the Frida dump is cut), so it cannot be used here.
- d3d_EnumDeviceCallback 0x41c460: 279/656 B, 39 asm lines. Remaining: the dead-parameter slots of the
  four D3_COPY temps (original: copy1 _cn=ctx[0x1c]/_cd=hel[0x18], copy2 _cn=ctx/_cd=guid[0x8], copy4
  _cd=ctx; ours differ) and the HEL entry (original copies depth eax->ecx and loads hel into eax; ours
  loads hel into ecx), plus the lea-chain registers. Tried this session: (depth & hel->..) order, a local
  for hel, a `bits` local, a d2 copy, `== 0` forms (all identical); a global D3_COPY temp-declaration swap
  changes nothing here and regresses d3d_EnumTextureFormatsCallback / d3d_EnumZBufferFormatsCallback /
  d3d_CreateDevice / par_CreateEmitter, so it was reverted; a local D3_COPY2 macro with `char *_cd` declared
  before `int _cn`, `sizeof(field)` spellings and an `||` chain of the three HEL tests (94 lines) also change
  nothing or are worse. New lead: d3d_GetDeviceInfo is defined above this function in the TU; VC6's callee
  register analysis makes the caller's allocation depend on the compiled callee, so match GetDeviceInfo first
  and re-check this one.
- dd_SetMode 0x41d2e0: 1121/2752 B, 58 asm lines (was 868 B / 93 asm lines). `bpp >= 9` instead of `bpp > 8`
  matches the first 328 instructions (the constant-8 web no longer takes ecx); the compare itself still
  differs (ours `cmp bpp,9 / jl`, original `mov eax,8 / cmp bpp,eax / jle`) and the remaining hunks
  (0x41d86a masks, 0x41db71 store order, 0x41dcd2 al=8 vs eax=8, 0x41dd33) follow the constant-8 web's
  register. Tried this session: operand order, nested ifs, chained `=` assignments, (void) call casts, a
  named `eight` local, the byte constants through a dword `r = 8`, `9 <= bpp`, `(unsigned)bpp > 8u`,
  `!(bpp <= 8)`, parenthesizations / term orders of the `dd_Unk14635a8` shift sum, and `(bpp >> 3)` /
  `xres * yres * bpp / 8` / `bpp / 8 * (xres * yres)` for the `D3_ZERO_AT` size (all no change or worse).
  Why the compare wants eax: the original's 24/32 mask branch ends `xor ebx,ebx; jmp 0x41d6f4`, i.e. it jumps
  back into the middle of the `bpp > 8` test and reuses the `mov eax,8` both sets; one dword constant-8 web
  spans the mask branch and the test. Our `mov al,8` there is a byte web, so the web never reaches eax; a
  spelling that forces a dword use in that branch (chained `=`, `r = 8`) was folded back. This is the real
  blocker, not the compare spelling.

## d3d_Error 0x418e90 (matched 2026-10-23)
- 12096 bytes (0x418e90..0x41bdd0, its switch tables included; the old draft was 11936 bytes because
  the dispatch tree was smaller). The original switch has 110 `DDERR_*` cases with messages (the complete
  MAKE_DDHRESULT list of ddraw.h plus the five E_/CO_E_ aliases) and 75 `D3DERR_*` cases (the complete
  list of d3d.h, 0x887602bc..0x88760826) whose bodies are empty.
- VC6 drops an empty case (`break;`, `return;`, a dead store, a `goto` to the join) before it lowers the
  switch. A folded `if (0) { }` body keeps the label in the compare tree and the jump tables and still
  emits no code; with all 75 labels the tree, the six jump tables and every body are byte-identical.
  The source lists them after the DDERR cases, value order, with a comment (see src/game/dd_vesa.c).

## dd_vesa worker notes (0x41be00..0x41f000) [dd_vesa]

### TU boundaries
- Dd_vesa.c starts at or before 0x41bdd0 (dd_Restore: RestoreAllSurfaces on dd_DD, called from dd_SetMode and
  dd_Flip; outside this worker's range, no source yet). Its .data starts at 0x5f3c48 (dd_LastDriver = -1) and
  0x6311f8 (the dialog's combo boxes) .. 0x631227; .bss 0x1463568..0x14661a4.
- Last Dd_vesa.c function: 0x41eb00 (dd_SetSurfaceName, assert line 2609). 0x41ebc0.. are surface scaling helpers
  called only by 0x420140 (surf_Unk420140), continuing past 0x41f000 (0x41f000 32-bit shrink, 0x41f220,
  0x41f2b0, ...). Put in src/game/unk_41ebc0.c; probably the start of the file that holds 0x420140.
  No string literals between 0x41eb00's and the "_error.log" TU (0x4214a0), so this is not proven.
- Ghidra missed two functions: 0x41c830 (dd_DlgProc) and 0x41d1c0 (dd_EnumVideoModesCallback).

### Nonmatching
- d3d_GetDeviceInfo (0x41c080): matched 2026-10-23, see Open work above (the tex/edi cascade was 927's spelling).
- d3d_EnumDeviceCallback (0x41c460): only the dead parameter slots chosen for the D3_COPY temps differ
  (original: size -> ctx slot, dst -> hel/guid slots; ours the other way) and one register pair in the HEL
  branch. Tried: macro temp declaration orders, model/depth declarations, int depth, duplicated tails
  (the duplicated tail fixed the end), operand orders.
- dd_SetMode (0x41d2e0, 2.8 KB): remaining are the `888 - RLoss*100 - GLoss*10 - BLoss` expression (the
  original computes -GLoss*10 first via neg; all spellings, unsigned constants and casts compile identically
  for us), and an eax/ecx role swap in the GDI branch (biHeight compare, the constant 8). Tried
  ~20 variants, see git log.

## Also about this unit
- notes/units/sblib.md, section [sblib]

## unk_418ce0.md (joined into dd_vesa.c, layout 2026-10-22)
## Also about this unit
- notes/units/sblib.md, section [sblib]

## match-base 2026-10-24 (match worker)

- d3_Unk418ce0 -> d3_InitKeyCodes (byte + readable): fills the 256-byte character -> key code table.
- dd_Unk631223 -> dd_ZCleared (dd_Exit byte + readable): the name d3_d3d.h already used for 0x631223
  (`d3d_ZBuffer && !dd_ZCleared` -> set 1); one alias less in data.csv.
- d3d_GetDeviceInfo byte + readable: D3DDEVINFO's ints 0x310..0x334 renamed minw / minh / maxw / maxh / zmode /
  trans_destblend / trans_srcblend / glow_destblend / glow_srcblend / maxMipFilter - the same 0x2c-byte block as
  D3DCARDINFO in d3_d3d.h, whose names carry the evidence (the bitfields unk30c_* / unk30d_* are that struct's
  caps / flags bytes and stay for now; `uv run ledger grade`'s Q2 does not flag the `_N` form).
- dd_DlgProc byte + readable: txt_Unk14483bc/c0/c4 -> txt_DlgTitle / txt_DlgLabel104 / txt_DlgLabel105 (the
  WM_INITDIALOG SetWindowText / SetDlgItemText(dlg, 104 / 105, ..) calls; the pointers are NULL in the exe's .data
  and set elsewhere, so the control ids are the only evidence).
- d3d_EnumDeviceCallback -> match-deep: the rest is exchanged scratch registers / dead-parameter slots (see the
  2026-10-24 reasons; no meaning error left).
