# gildegold

A decompanion target project for `GildeGold.exe` (The Guild Gold, GOG 2.06, German), target `exe`. Run
`uv run dcp setup`, then follow `uv run dcp status`.

## Origin

The sources were migrated from the first attempt, `../rea` at commit 8cef7d5b, by `tools/import_rea.py`. That
script is deterministic and reruns in rea's venv:

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

## Open since the migration

These issues come from rea, or from the exe having no base relocations. `dcp check` shows them all.

- **Data debt.**
  - Tentative definitions (D5).
  - `__FILE__` literals: the original's are `D:\develop\Units\...` (M9).
  - Data order, sizes and contents (D4, D1, M.data.content).
- **One code-walk error (L1).** The walk swallows a call target at 0x4eba80.
- **Unresolved link names.**
  - CRT functions under placeholder names, and one ICF-folded stub; rea resolved these with alias tables in its `link.py`.
  - GUID and IID symbols: the `dxguid`, `strmiids` and `uuid` archives are not declared.
  - `MSS_auto_cleanup` and `AIL_get_timer_highest_delay`.

Game runs, the debugger and frame dumps stay in rea (`uv run play`, `uv run dbg`, `uv run shot`) until decompanion
has a run command.
