# Windowed-mode 32-bit surface bug (the 2D layer): fixed at the root

Historical record of the 32-bit surface bug, the `REA_FIX_32BPP` workaround (now removed) and the
`REA_FIX_16BPP` root fix that replaced it. The hybrid harness named below (`tools/run_hybrid.ps1`,
`uv run link --nonmatching`) was removed on 2026-10-28: build with `REA_FIX_16BPP=1 uv run build`,
`uv run link`, and run the resulting `build/link/GildeGold_full.exe`.

## Root fix (2026-10-27): REA_FIX_16BPP

The whole 32-bit blitter patch below was a workaround. The root cause is that in windowed mode
`dd_SetMode` (dd_vesa.c) creates the offscreen back buffer without DDSD_PIXELFORMAT, so DirectDraw gives
it the desktop's 32-bit format (fullscreen gets 16-bit from `SetDisplayMode`, which is why it works).

`REA_FIX_16BPP=1` (`/DREA_FIX_16BPP` in dd_vesa.c) fixes it at the root: the window-mode back buffer is
created with `DDSD_PIXELFORMAT | DDPF_RGB`, 16-bit 565 (0xf800/0x07e0/0x001f), matching what
`d2_MakeSurface` uses for its own 16-bit surfaces and what fullscreen gets. With a 16-bit back buffer the
original 16-bit blitters and the `dd_RGB` converters work unchanged, so `REA_FIX_32BPP` was removed.

Build/run: `uv run play` (the debug compile flags are always on there, and it reports a crash in
`build/crash/`); the historical hybrid recipe was `REA_FIX_16BPP=1 uv run build && uv run link
--nonmatching`, then `powershell -File tools/run_hybrid.ps1 -Exe build/link/GildeGold.exe -Seconds 12`.
Verified working
2026-10-27 (window mode reaches the menu and renders correctly; `REA_FIX_32BPP` and `REA_FIX_555` were
deleted from shp.h / shp.c / gfx.c / build.py).

## The removed workaround (REA_FIX_32BPP, state 2026-10-25)

Branch `worktree-fix32` (worktree `.claude/worktrees/fix32`). Removed 2026-10-27 in favour of the root
fix above. Kept below for the record.

## Goal

Fix the windowed-mode 2D rendering on 32-bit surfaces (the doubled / wrong-colour UI), gated by a
compile-time flag so the default build stays byte-identical to the original.

## The original bug (verified, fixed, user-confirmed colours are right)

- In windowed mode `dd_SetMode` (dd_vesa.c) creates the offscreen back buffer without DDSD_PIXELFORMAT
  -> it gets the desktop's 32-bit format (measured: back=1024x768x32 pitch=4096, front=2560x1440 desktop).
- The d2 shape blitters (shp.c, mostly inline asm) are 16-bit only: word stride = pitch*2 bytes = half a
  32-bit row, 2-byte writes -> the UI appears twice at half size in the top half, colours garbled.
- The 8->16 / 24->16 converters and the light table build 16-bit values with `dd_RGB`, which follows the
  32-bit surface masks and truncates the red channel away (0xGGBB).

## What the patch does (all under `#ifdef REA_FIX_32BPP`, off = byte-identical)

- include/shp.h: `REA_EXPAND(v)` (16-bit shape pixel -> 0x00RRGGBB), `REA_FIX_CONV(r,g,b)`,
  `REA_FIX_MASK`, `REA_HALF_DARK(v)`. 565 by default, `REA_FIX_555=1` selects 555 (all large art in the
  .gfx is 24-bit and converts through REA_FIX_CONV, so 565/555 only affects small native-16-bit pictures;
  the cursor looks right with 565).
- src/game/shp.c:
  - d2_ShpBlit: inline 32-bit run blitter for type-1 shapes on 32-bit surfaces (clip to
    [ClipLeft, ClipRight) x [ClipTop, ClipBottom)). Run format per row: a run-count dword, then runs of
    [skip2, npix, npix 16-bit pixels]; skip2 is twice the number of transparent pixels between the END of
    the previous run and this one (the asm adds it to the running destination pointer).
  - d2_ShpBlit16Rows: 32-bit variant of all three modes (mode 3 degrades to copy).
  - d2_ShpBlit16HalfDark: 32-bit variant, grey from the expanded channels.
  - d2_ShpBlit16Scaled / ScaledLit: 32-bit variants.
  - d2_FillRect: 32-bit darken (REA_HALF_DARK, dword pitch) instead of d2_ShpDarken16.
  - d2_MakeLightTable / d2_ShpConvert8To16 / d2_ShpConvert24To16 (both branches): fixed-format values
    via REA_FIX_CONV; mode-1 half dark via REA_FIX_MASK.
- src/game/gfx.c: local copy of REA_EXPAND; d2_BlitPartMode's two memcpy fallbacks expand 16->32 per
  pixel (bits-only 16-bit surfaces such as the item lists and the paintbox).
- uv run build: `REA_FIX_32BPP=1` (and `REA_FIX_555=1`) adds /DREA_FIX_32BPP to shp.c and gfx.c.

Build: `REA_FIX_32BPP=1 uv run build build && uv run link --nonmatching`.
Run: `powershell -File tools/run_hybrid.ps1 -Exe build/link/GildeGold.exe -Seconds 12`; the menu is up
after ~8 s without intro. The exe path needs the DWM8And16BitMitigation compat shim (memory
`gildegold-compat-shim`; the entry for this worktree's build/link/game/hybrid.exe exists).

## The black / offset lines: SOLVED 2026-10-25

Symptom: static horizontal black line segments in the menu background and "offset lines" in the button
texts. Root cause: d2_ShpBlit's 32-bit walk computed every run's position as `x + skip` (from the shape
origin) instead of from the end of the previous run, so in rows with several runs every run after the
first landed too far left and the row's tail stayed black. Fixed with a running `pos`. Verified with
the frame dumps below: the surface right after the walk equals the Python decode of the shape data in
all 768 rows, and the flipped frame differs only where the buttons and cursor are drawn.

Wrong theories from the first session, for the record: the menu background is NOT a D3D flyover; it is
the 24-bit run-coded picture 1944 (_MENUE_BACKGROUND_1024, 1024x768, 3123 runs) converted by
d2_ShpConvert24To16 and drawn ONCE through d2_DrawPic -> d2_DrawShape -> d2_ShpBlit when the menu opens,
then kept in d2_BackSurf and restored per frame with two 1024x768 d2_BlitPartMode hardware blits
(plus one d2_DrawFades per frame). No d3_EndScene happens at the menu. The earlier ddlog that showed no
big ShpBlit missed the single draw.

## Verification without screenshots: REA_FRAME_DUMP (uv run framedump)

`REA_FRAME_DUMP=1` adds /DREA_FRAME_DUMP to dd_vesa.c, d3_d3d.c, shp.c, gfx.c, d2.c (uv run build).
Everything lives in include/rea_dump.h as macros because the hybrid link places no new functions or
data (`uv run link`: only symbols with an address in the original resolve; the exe's CRT also lacks fscanf and
remove). State is the trigger file.
- `dump.trigger` in the game directory (a frame count) arms the dumps; dd_Flip dumps the DirectDraw back
  buffer as `dump/t<tick>_final<k>.raw` and counts the file down; d3_EndScene dumps `_scene<k>` while
  armed. `run_hybrid.ps1 -TriggerAt N -DumpFrames K` writes the trigger at second N.
- Always-on trace of the big 2D draws (w > 300) in `dump/trace.txt`: ShpBlit (with clip rect and
  surface), the four 32-bit blitters, ClipY/ClipXY, FillRect, Conv24 (which also writes the 24-bit source
  shape and the converted 16-bit shape as `dump/shape24src_*.bin` / `dump/shape16_*.bin`), BlitPart,
  DrawPic, Dissolve, Fade. d2_ShpBlit also dumps the destination surface right after a big 32-bit walk
  (`_shpblit<k>`).
- `uv run framedump list | png | rows | diff | pixel`: pure Python, PNG via zlib; the PNG can be
  viewed directly. Shape .bin files decode with the loop in this note's history (header 0x32 bytes,
  per row a count dword, runs [skipbytes, npix, pixels]; 24-bit skips are byte counts /3, 16-bit /2).

Typical check: `REA_FIX_32BPP=1 REA_FRAME_DUMP=1 uv run build build && uv run link
--nonmatching && powershell -File tools/run_hybrid.ps1 -Exe build/link/GildeGold.exe -Seconds 12
-TriggerAt 10` then `uv run framedump list` and `png`.

## Still open

1. **Modes 1-5 of the d2_ShpBlit 32-bit walk** are implemented (mirroring d2_ShpBlit16ClipY: 1 / 4 =
   REA_HALF_DARK(dest) + REA_EXPAND(src), 2 = REA_HALF_DARK(dest), 3 = dest through d2_ShpLightTable via
   REA_PACK, 5 = REA_EXPAND(d2_Mode5Colour low half); 2, 3, 5 ignore the source pixels), d2_SetMode5Colour
   stores a fixed-format colour and d2_MakeLightTable splits its index in the fixed format under the flag,
   d2_ShpBlit16Rows' 32-bit modes 2 / 3 were fixed too. The menu (mode 0 glyphs) is unchanged by this; the
   other modes are NOT yet verified in-game: hover a menu button (modes 1-4), look at coloured text (mode 5)
   and the light-table highlights (mode 3) with the frame dumps.
2. **The dissolve** (d2.c d2_Unk476110 + the asm blit consumer of d2_SetBlitParams): uses dd_PitchPixels
   (2048 on a 32-bit surface vs the correct 1024 dwords) and 16-bit memcpy in its final step. Transitions
   only; the trace shows when it runs.
3. **The gfx.c d2_BlitPartMode fallback fix** (16->32 expansion) is not user-verified; its trigger
   (16-bit bits-only surfaces: item lists, paintbox) does not occur at the menu.
4. The default build stays byte-identical (`uv run build` check: no REGR); the pre-commit gates pass.
