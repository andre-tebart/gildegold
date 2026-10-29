# gildegold

A decompanion target project for `GildeGold.exe` (The Guild Gold, GOG 2.06, German), target `exe`. Run
`uv run dcp setup`, then follow `uv run dcp status`.

## Origin

The sources were migrated from the first attempt, `../rea` at commit 8cef7d5b, by `tools/import_rea.py`. That
script is deterministic. It runs in rea's venv and refuses any rea HEAD other than the pinned commit, so check
out 8cef7d5b first (rea's HEAD is the freeze commit 08b43d57, which only changes AGENTS.md):

    uv run --directory C:/Users/andre/Documents/rea python C:/Users/andre/Documents/gildegold/tools/import_rea.py

How rea's files map here:

| In rea | Here |
|---|---|
| unit file `src/game/<unit>.c` | `src/<unit>/` with `head.c`, `f_<addr>.c`, `u_<name>.c` and `tail.c` |
| rea's source order, where it is not the address order (25 units) | `src/<unit>/order` |
| unit tags `@named_by`, `@boundary`, `@bytes` | `decl/text.dcl` lines, each with a reason naming rea's commit |
| `notes/units/<unit>.md` | `src/<unit>/open.md` |
| the other notes | `notes/rea/` |
| zlib 1.1.3 and minizip | one unit per file: `adler32`, …, `unzip` |
| zlib's headers | `include/` |

zlib's configuration branches that crossed a function boundary were reduced to the branch VC6 takes. `CL /EP`
checks that the preprocessed output is unchanged.

The migration checked byte parity against rea's baseline: 2664 of 2664 byte-matched functions stay byte-matched,
and all 92 units compile. Readability is recomputed by dcp's rules.

## Open

`dcp check` passes (gate=ok): no conflict error, and the link resolves every name. mss32.lib's code member
(`MSS_auto_cleanup` and its exit callback) is the library unit `src/mss32lib/` (decl/text.dcl `libunit`): both
byte, and the link adds its object to the generated mss32.lib, so LINK places it among the library
contributions. Left:

- **The `AIL_get_timer_highest_delay` thunk at 0x42c3a0** (a `jmp` through the import slot, the end of `sfx`'s
  range) has no source: callers reach the stdcall import, and the linker supplies the thunk elsewhere.

Game runs, the debugger and frame dumps stay in rea (`uv run play`, `uv run dbg`, `uv run shot`) until decompanion
has a run command.
