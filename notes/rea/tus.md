# Translation units: file names and where they come from

Every file in src/game is one translation unit (TU). Its name has a source, strongest first:
- `leaked`: the source path is in the binary (`__FILE__` / assert strings, config/symbols/tu_anchors.csv). Final.
- `tag`: a module tag in the TU's strings (allocation tags like `"d3_io_fl:"`, error-message prefixes).
- `guess`: from what the functions do.

**Where the facts live** (since 2026-10-23): in the sources. Every file's header comment carries the unit tags
`@named_by` (leaked / tag / guess / -) and `@boundary` (proof of the boundary at the unit's start: proven /
plausible / open); every function has its address in the comment directly above it, with `@deep` / `@stuck` when
it is match-deep work.
`build/state/units.csv` and `build/state/names.csv` are generated from them (`uv run build`,
uv run gen) into build/ as the working state and are not committed; `state_baseline/names.csv` is the
committed baseline of the names and grades (promoted on main with `uv run ledger promote`). Everything else is
computed:
`uv run ledger units` prints every unit with its range, function counts, both boundary proofs and its
**status**; `uv run ledger check` is the gate the pre-commit hook runs. The evidence behind a row is prose
in `notes/units/<unit>.md`, section **Unit**.

**Bytes** (computed, rules R1..R7 in `uv run ledger --help`): every byte of the game code is a matched
function's compiled section, a draft's code walk, fill (the four forms the exe contains), an import thunk, bytes a
unit header declares with `@bytes 0xSTART..0xEND why`, or undrafted. Nothing is classified by a heuristic: what no
rule covers is undrafted, and undrafted bytes inside a unit keep it open until they are drafted or declared.

**Status** (computed, see uv run ledger):
- `separated (proven)`: both boundaries proven (recorded .data / padding / codegen evidence, or leaked / tag
  names on both sides) and nothing undrafted in the range.
- `separated (plausible)`: both boundaries at least plausible (a coherent group with its own string block or none,
  no contrary evidence) and nothing undrafted. Proof is often impossible (the binary leaks nothing more), and a wrong guess
  is cheap: relocated addresses are masked, so only codegen coupling between neighbours matters, and it shows up
  as a function that matches only with a neighbour's code above it; then the files are joined.
- `open`: a boundary recorded `open` (contrary evidence: string blocks interleave, a function needs a neighbour's
  code), undrafted bytes in the range, or the last unit (no neighbour after it yet).
Both `separated` kinds go to match sessions (skill `match`, queue `uv run ledger grade --queue --debt`).

A stronger source replaces a weaker name; renames happen in layout sessions when the evidence turns up (layout
skill, **Naming units**). Files with no fitting name stay `unk_<startaddr>.c`.
