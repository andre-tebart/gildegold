# Binary facts (observed from PE headers, 2026-10-20)

Source: GOG "The Guild Gold" (gameId 1207658720), German build, patch 2.06.

## GildeGold.exe (primary target, DirectDraw renderer)
- PE32 i386, image base 0x400000, entry 0x5E0955, timestamp 2003-10-01 07:47:06 UTC
- Linker 6.0, no relocations, no debug directory, no RTTI strings (`.?AV` absent)
- Not packed: .text entropy 6.70, no DRM signatures found
- Sections: .text 0x1E9CB4, .rdata 0x5C7A, .data vsize 0xEFAE84 (~15 MB, mostly BSS), .rsrc 0x508
- Imports: KERNEL32(88) USER32(48) GDI32(7) ADVAPI32(5) ole32(3) WINMM(6) WS2_32(21) DINPUT(1) mss32(31) DDRAW(2)
- Rich header (prod:build x count): 12:7291x10 11:8047x1 14:7299x30 10:8047x136 28:9178x1
  25:9210x2 29:9178x1 4:8447x53 19:9049x3 93:2067x2 93:2179x13 1:0x191 48:9044x157 6:1735x1

### Ghidra 12.1.4 auto-analysis (2026-10-20)
- 3902 internal functions + 209 imports; 4792 defined strings. Function list: Ghidra's starts; the 279 outside
  the game code are kept in `config/symbols/unsourced.csv`, the game range in `config/symbols/game_range.toml`.
- Ghidra FID named only ~40 functions, all MSVC CRT (`_malloc`, `__ftol`, `_strlen`, `__alldiv`, ...).
- Layout (OBSERVED): game code 0x401000..0x5dd3df; then dinput.lib's c_dfDIKeyboard / c_dfDIMouse object
  tables 0x5dd3e0..0x5de44f (data in .text, byte-equal to the DX 6.1 dinput.lib); DINPUT/DDRAW import-lib stubs at
  0x5de450..0x5de491; then statically linked CRT from 0x5de4a0 to end of .text (279 fns, `entry` = 0x5e0955).
- Source is **C**, not C++: all leaked assert paths are `.c` (`D:\develop\Units\...`), consistent with no RTTI.
  Anchors per file in `config/symbols/tu_anchors.csv`; files appear in link order d3_tools < dd_vesa < msx < net
  < d3_d3d < d3_engine < d3_interface < d3_sky < ts_texture < zlib < GFX.C.
- zlib 1.1.3 + unzip 0.15a (Gilles Vollant) are compiled in **amid game code** (inflate around 0x4648xx..0x4656xx,
  right after ts_texture.c) with their strings in .data – i.e. built as part of the project, not a prebuilt lib.
  These are public sources: best first byte-matching targets to pin down compiler version/flags.
- zlib/unzip extent – MATCHED 2026-10-20 (`uv run scan`, all 85 functions byte-identical):
  **0x4616d0..0x4687a0**, objects in alphabetical link order adler32 < crc32 < deflate < gzio < infblock <
  infcodes < inffast < inflate < inftrees < infutil < trees < unzip < zutil, no gaps.
  So the zlib sources were in the project as plain .c files, sorted by name in the link.
  - deflate IS linked (deflateInit2_, deflate, deflateEnd + internals; also gzio write path); the copyright string
    is simply unreferenced. compress.c, uncompr.c and zip.c are not linked.
  - Not linked (eliminated by /OPT:REF): zlibVersion, zError, get_crc_table, inflateSync*, inflateSetDictionary,
    deflateCopy/Params/SetDictionary, _tr_tally, unzLocateFile, unzGetGlobalInfo/-Comment, unzeof,
    unzGetLocalExtrafield, unzStringFileNameCompare, gzdopen/gzgetc/gzgets/gzprintf/gzputc/gzputs/gzflush/gzeof/gzerror.
  - Deviations from upstream: zcalloc uses malloc; two extra unzip functions at 0x4686d0/0x468710
    (see src/zlib/UPSTREAM.md, src/minizip/UPSTREAM.md).
  - 0x4614c0..0x4616d0 (4 fns) precede zlib and belong to the previous game TU; 0x4687a0 starts the next one.
  - 0x5dead4 (called by zcfree) is `_free`.
- In-game version string says "Version 2.05" (0x624a4c) although GOG labels the patch 2.06.

## GildeGold_TL.exe (sibling "TnL" build, Direct3D 8 + DINPUT8)
- timestamp 2003-10-01 07:52:12 UTC (5 min later, same codebase), entry 0x6082A3
- Extra `.data1` section; .text 0x2136B5

## Toolchain – CONFIRMED 2026-10-20 by byte-matching zlib 1.1.3
- **VC6 SP5 + Processor Pack**, C compiler, **/O2** (== /Ox == /O2 /G5 for this code; /G6, /O1, /Oy- do NOT match).
- Matched byte-for-byte (relocated fields masked, `tools/fncmp.ps1`), compiled from unmodified zlib 1.1.3:
  `inflate` 0x465170, `inflate_trees_bits` 0x4655a0, `inflate_codes` 0x464540, `inflate_fast` 0x464c60.
- The same sources under SP6 (c2 12.00.9782) do NOT match (different block layout) – SP6 is unusable for matching.
- Rich-header cross-check: our c2.dll 13.00.9044 = game objects' 48:9044; SP5 LIBC.LIB objects carry 10:8047
  = the original's 136 CRT objects. Linker 6.00.8447 identical.
- Toolchain lives at `C:\Tools\MSVC600-SP5PP` (provenance in its PROVENANCE.txt); runs natively (Smart App
  Control must be off, it blocks VC6's unsigned DLLs; until 2026-10-21 it ran under Wine in WSL).

## Compiler hypothesis (superseded by the section above)
- Linker 6.00.8447 => Visual C++ 6.0 toolchain.
- 157 objects built by tool id 48 build 9044 – candidate: VC6 SP5 + Processor Pack cl 12.00.9044.
- 136 objects by tool id 10 build 8047 – likely prebuilt static libs (CRT/SDK).
- Must confirm by compiling probe functions and diffing against original before bulk matching.
