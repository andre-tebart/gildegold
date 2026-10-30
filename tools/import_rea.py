"""import_rea.py -- convert the first-attempt decomp (rea, one file per unit) into this decompanion project
(one file per function). Historical: it writes decompanion's fragment layout of before docs/plan.md section 19
(2026-10-30), which `dcp unit fold` converted into today's one file per unit; rerun, it needs that fold after it.
A migration tool: it reads rea's sources at a pinned commit with rea's own parser
(tools/gildegold/core/gen.py) and writes src/, include/, decl/text.dcl and notes/rea/. Deterministic: two runs
on the same rea commit write the same bytes. It owns what it writes and replaces it on every run.

Run it in rea's venv (it imports rea's parser):

    uv run --directory C:/Users/andre/Documents/rea python C:/Users/andre/Documents/gildegold/tools/import_rea.py

Per rea unit src/<dir>/<stem>.c -> src/<stem>/ (minizip/unzip.c -> src/unzip/):
  head.c       everything before the first function's address comment (header prose without the unit tags)
  f_<addr>.c   the function's address comment (moved to line 1), then everything between the previous
               function and it (data, prototypes, comments), then the function
  u_<name>.c   an unlinked function with its comment; without an order file the text before it moves on to
               the next linked fragment (or, after the last one, to the end of the previous linked fragment)
               so the data order and every declaration stay ahead of their use (u_ files come last)
  order        rea's source order, written when it differs from the amalgamation's default (linked by address,
               then unlinked by name); the text then stays with its function, u_ files included
  tail.c       the text after the last function
__inline helpers (no symbol, no address) stay text where rea has them. The unit tags (@named_by, @boundary,
@bytes) become decl/text.dcl lines with a reason naming the rea commit. A cut inside an #if group (zlib's
configuration branches) is resolved by keeping the branch the compiler takes (marker lines + CL /EP; the
preprocessed output must stay the same). It refuses to guess: a cut it cannot resolve, a function the parser
does not annotate, or an __inline function with an address stops the run with the list.
"""
import argparse
import csv
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

from gildegold.core.gen import Source

PIN = "8cef7d5b"
GAME_END = 0x5dd3e0          # rea config/symbols/game_range.toml: game code 0x401000..0x5dd3df
INLINE = re.compile(r"\b(?:__inline|__forceinline|inline)\b")
COND = re.compile(r"^[ \t]*#[ \t]*(if|ifdef|ifndef|endif)\b", re.M)
UNIT_TAG = re.compile(r"^[ \t]*(?:\*[ \t]*)?@(named_by|boundary|bytes)\b[^\n]*\n?", re.M)
EXTERN_LINE = re.compile(r"extern [^\n;]*;[ \t]*")
INCLUDE_FIX = {'#include "../src/zlib/zlib.h"': '#include "zlib.h"',
               '#include "../src/minizip/unzip.h"': '#include "unzip.h"'}


def git(rea: Path, *args) -> str:
    return subprocess.run(["git", "-C", str(rea), *args], check=True, capture_output=True, text=True).stdout


def line_start(text: str, off: int) -> int:
    return text.rfind("\n", 0, off) + 1


def line_end(text: str, off: int) -> int:
    """the offset just past the newline of the line holding off (or the end of the text)"""
    k = text.find("\n", off)
    return len(text) if k < 0 else k + 1


def cond_depth(code: str, upto: int) -> int:
    """the #if nesting depth at offset upto (comments and strings already blanked in code)"""
    d = 0
    for m in COND.finditer(code, 0, upto):
        d += -1 if m.group(1) == "endif" else 1
    return d


def dedupe_externs(text: str) -> str:
    """drop exact repeats inside a run of consecutive `extern ...;` lines (rea's full-rebuild blocks)"""
    out, seen = [], set()
    for line in text.split("\n"):
        if EXTERN_LINE.fullmatch(line):
            if line in seen:
                continue
            seen.add(line)
        else:
            seen = set()
        out.append(line)
    return "\n".join(out)


def fix_includes(text: str) -> str:
    for a, b in INCLUDE_FIX.items():
        text = text.replace(a, b)
    return text


def ensure_nl(text: str) -> str:
    return text if not text or text.endswith("\n") else text + "\n"


class UnitOut:
    def __init__(self, unit: str):
        self.unit = unit
        self.files: dict[str, str] = {}
        self.linked: list[int] = []
        self.unlinked: list[str] = []
        self.reordered: list[int] = []
        self.tu_order: list[str] = []     # fragment files in rea's source order


def split_unit(rea: Path, rel: str, text: str, unit: str, problems: list, spots: list | None = None,
               in_place: bool = False) -> tuple[UnitOut, Source]:
    spots = [] if spots is None else spots
    S = Source(rea / rel, text=text)
    out = UnitOut(unit)
    defs = []
    for off, name, addr, ci in S.defs:
        if INLINE.search(S.code, line_start(S.code, off), S.code.find("(", off)):
            if addr in (None, "unlinked"):
                continue             # an __inline helper: no symbol, it stays text where rea has it
            problems.append(f"{rel}: {name} is __inline but has an address")
            spots.append(off)
            continue
        if addr is None or ci is None:
            problems.append(f"{rel}: {name} has no address comment")
            spots.append(off)
            continue
        cs, ce, _ = S.comments[ci]
        brace = S.code.find("{", off)
        close = S.brace_end(brace)
        if close is None:
            problems.append(f"{rel}: {name}: unclosed body")
            nxt = min([o for o, *_ in S.defs if o > off] + [len(text)])
            spots += [g["branches"][1][0] for g in cond_groups(S.code)
                      if off < g["start"] < nxt and len(g["branches"]) > 1]   # #else branches that unbalance it
            continue
        defs.append((addr, name, cs, ce, line_end(text, close)))
    if not defs:
        problems.append(f"{rel}: no functions")
        return out, S
    # cut points: each function's comment line start and its body end must sit outside every #if
    for addr, name, cs, ce, end in defs:
        for k in (line_start(text, cs), end):
            if cond_depth(S.code, k):
                problems.append(f"{rel}: {name}: a cut at line {S.line_of(k)} lies inside an #if (split by hand)")
                spots.append(k)
    linked = [a for a, *_ in defs if a != "unlinked"]
    if linked != sorted(linked):     # reported: the amalgamation puts them in address order (user decision)
        out.reordered = [b for a, b in zip(linked, linked[1:]) if b < a]
    first = line_start(text, defs[0][2])
    head = text[:first]
    note = f"   unit of rea {PIN} ({rel}); boundaries in decl/text.dcl, notes in src/{unit}/open.md"
    if S.comments and S.comments[0][1] <= first and S.tags and text[:S.comments[0][0]].strip() == "":
        cs, ce, body = S.comments[0]
        kept = [l for l in UNIT_TAG.sub("", body.rstrip() + "\n").split("\n")]
        while kept and not kept[-1].strip():
            kept.pop()
        head = text[:cs] + "/*" + "\n".join(kept) + "\n" + note + " */" + text[ce:first]
    else:
        head = f"/* {unit}\n{note} */\n" + head
    out.files["head.c"] = ensure_nl(head)
    carry = ""                       # text waiting for the next linked fragment
    prev_end = first
    last_linked = None
    for addr, name, cs, ce, end in defs:
        cl = line_start(text, cs)
        between = text[prev_end:cl]
        comment = text[cs:ce]
        after = text[ce:end]
        if after.startswith("\n"):
            after = after[1:]
        elif after[:1].isspace():
            after = after.lstrip(" \t")
            if after.startswith("\n"):
                after = after[1:]
        if addr == "unlinked" and in_place:
            fname = f"u_{name}.c"
            out.files[fname] = ensure_nl(comment + "\n" + between + after)
            out.unlinked.append(name)
            out.tu_order.append(fname)
        elif addr == "unlinked":
            carry += between
            fname = f"u_{name}.c"
            out.files[fname] = ensure_nl(comment + "\n" + after)
            out.unlinked.append(name)
            out.tu_order.append(fname)
        else:
            fname = f"f_{addr:x}.c"
            out.files[fname] = ensure_nl(comment + "\n" + carry + between + after)
            carry = ""
            out.linked.append(addr)
            out.tu_order.append(fname)
            last_linked = fname
        prev_end = end
    tail = text[prev_end:]
    if carry:
        if last_linked is None:
            problems.append(f"{rel}: text before an unlinked function and no linked fragment to hold it")
        else:
            out.files[last_linked] = ensure_nl(out.files[last_linked]) + carry
    if tail.strip():
        out.files["tail.c"] = ensure_nl(tail)
    for k in list(out.files):
        out.files[k] = dedupe_externs(fix_includes(out.files[k]))
    return out, S


DIRECTIVE = re.compile(r"^[ \t]*#[ \t]*(if|ifdef|ifndef|elif|else|endif)\b", re.M)


def cond_groups(code: str) -> list[dict]:
    """every #if group of the (comment-blanked) code: start (its #if line), end (past its #endif line),
    branches [[directive line start, content start, content end]]"""
    groups, stack = [], []
    for m in DIRECTIVE.finditer(code):
        ls, le = line_start(code, m.start()), line_end(code, m.start())
        d = m.group(1)
        if d in ("if", "ifdef", "ifndef"):
            stack.append({"start": ls, "branches": [[ls, le, None]]})
        elif d in ("elif", "else") and stack:
            stack[-1]["branches"][-1][2] = ls
            stack[-1]["branches"].append([ls, le, None])
        elif d == "endif" and stack:
            g = stack.pop()
            g["branches"][-1][2] = ls
            g["end"] = le
            groups.append(g)
    return groups


def preprocess(rea: Path, rel: str, text: str, tmp: Path) -> str:
    """CL /EP of text as if it were rea's rel (its directory and rea's cflags on the include path)"""
    from gildegold.core.build import cflags_for, vc6
    f = tmp / Path(rel).name
    f.write_text(text, newline="\n")
    flags = [x for x in cflags_for(rea / rel) if x != "/c"]
    r = vc6(["/EP", *flags, f"/I{(rea / rel).parent}", str(f)], cwd=rea)
    if r.returncode:
        raise SystemExit(f"import_rea: CL /EP {rel} failed:\n{r.stderr}{r.stdout[-2000:]}")
    return r.stdout


def normalised(pp: str) -> list[str]:
    return [" ".join(l.split()) for l in pp.split("\n") if l.strip()]


def resolve_conditionals(rea: Path, rel: str, text: str, spots: list, tmp: Path, report: list) -> str:
    """Keep only the branch the compiler takes in the outermost #if groups holding a problem spot (a cut point,
    a definition without an address); the compiler decides (marker lines + CL /EP), and the preprocessed
    output must stay the same."""
    S = Source(rea / rel, text=text)
    hit = [g for g in cond_groups(S.code) if any(g["start"] <= k < g["end"] for k in spots)]
    hit = [g for g in hit if not any(h is not g and h["start"] <= g["start"] and g["end"] <= h["end"] for h in hit)]
    if not hit:
        return text
    marks, inserts = {}, []
    for gi, g in enumerate(hit):
        for bi, (ds, cs, ce) in enumerate(g["branches"]):
            tok = f"__import_rea_mark_{gi}_{bi}__"
            marks[tok] = (gi, bi)
            inserts.append((cs, tok + "\n"))
    marked = text
    for at, ins in sorted(inserts, reverse=True):
        marked = marked[:at] + ins + marked[at:]
    pp = preprocess(rea, rel, marked, tmp)
    taken = {gi: bi for tok, (gi, bi) in marks.items() if tok in pp}
    out = text
    for gi, g in sorted(enumerate(hit), key=lambda x: -x[1]["start"]):
        keep = ""
        if gi in taken:
            ds, cs, ce = g["branches"][taken[gi]]
            keep = text[cs:ce]
        out = out[:g["start"]] + keep + out[g["end"]:]
        report.append(f"{rel}:{S.line_of(g['start'])}: #if group resolved to branch "
                      f"{taken.get(gi, 'none')} of {len(g['branches'])} (CL /EP)")
    if normalised(preprocess(rea, rel, out, tmp)) != normalised(preprocess(rea, rel, text, tmp)):
        raise SystemExit(f"import_rea: {rel}: resolving the #if groups changed the preprocessed output")
    return out


def default_order(u: UnitOut) -> list[str]:
    """the amalgamation's order without an order file: linked by address, then unlinked by name"""
    return [f"f_{a:x}.c" for a in sorted(u.linked)] + [f"u_{n}.c" for n in sorted(u.unlinked)]


def first_prose(S: Source) -> str:
    if not S.comments:
        return ""
    body = UNIT_TAG.sub("", S.comments[0][2])
    lines = [l.strip(" *\t") for l in body.split("\n")]
    s = " ".join(l for l in lines if l)
    s = re.sub(r"\s+", " ", s)
    return s[:300].rstrip() + ("..." if len(s) > 300 else "")


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--rea", default="C:/Users/andre/Documents/rea")
    ap.add_argument("--out", default=str(Path(__file__).resolve().parents[1]))
    ap.add_argument("--pin", default=PIN)
    args = ap.parse_args()
    rea, out = Path(args.rea).resolve(), Path(args.out)
    head = git(rea, "rev-parse", "HEAD").strip()
    if not head.startswith(args.pin):
        print(f"rea HEAD is {head[:8]}, pinned {args.pin}", file=sys.stderr)
        return 1
    if git(rea, "status", "--porcelain").strip():
        print("rea has uncommitted changes", file=sys.stderr)
        return 1

    names = list(csv.DictReader(open(rea / "state_baseline/names.csv")))
    units_csv = {r["file"]: r for r in csv.DictReader(open(rea / "state_baseline/units.csv"))}
    first_addr = {}
    for r in names:
        a = int(r["address"], 16)
        first_addr[r["source"]] = min(a, first_addr.get(r["source"], a))

    sources = sorted(p.relative_to(rea).as_posix() for p in (rea / "src").rglob("*.c"))
    problems, units, resolved = [], {}, []
    tmp = Path(tempfile.mkdtemp(prefix="import_rea_"))
    for rel in sources:
        stem = Path(rel).stem
        if stem in units:
            problems.append(f"{rel}: unit name {stem} taken twice")
            continue
        text = (rea / rel).read_text()
        for _ in range(8):           # resolve the #if groups that block the split, outermost first
            mine, spots = [], []
            u, S = split_unit(rea, rel, text, stem, mine, spots)
            if u.tu_order != default_order(u):   # an order file keeps rea's order: the text stays in place
                mine, spots = [], []
                u, S = split_unit(rea, rel, text, stem, mine, spots, in_place=True)
            if not spots:
                break
            new = resolve_conditionals(rea, rel, text, spots, tmp, resolved)
            if new == text:
                break
            text = new
        problems += mine
        units[stem] = (rel, u, S)
    if problems:
        print("import_rea: refused, fix these first:", file=sys.stderr)
        for p in problems:
            print("  " + p, file=sys.stderr)
        return 1

    for d in ("src", "include", "notes/rea"):
        if (out / d).exists():
            shutil.rmtree(out / d)
    for stem, (rel, u, S) in units.items():
        base = out / "src" / stem
        base.mkdir(parents=True)
        if u.tu_order != default_order(u):
            u.files["order"] = (f"# the TU order of rea {args.pin} ({rel}): the source order, which differs from the address\n"
                                f"# order (a callee defined above its caller changes the caller's code; declarations precede use)\n"
                                + "".join(f + "\n" for f in u.tu_order))
        for fname, body in sorted(u.files.items()):
            (base / fname).write_text(body, newline="\n")
        note = rea / "notes/units" / f"{stem}.md"
        open_md = note.read_text() if note.exists() else f"# {stem}\n"
        (base / "open.md").write_text(ensure_nl(open_md), newline="\n")

    inc = out / "include"
    inc.mkdir(parents=True)
    headers = sorted((rea / "include").glob("*.h")) + sorted((rea / "src").rglob("*.h"))
    for h in headers:
        if (inc / h.name).exists():
            print(f"import_rea: header {h.name} twice", file=sys.stderr)
            return 1
        (inc / h.name).write_text(fix_includes(h.read_text()), newline="\n")

    notes = out / "notes/rea"
    for p in sorted((rea / "notes").rglob("*.md")):
        r = p.relative_to(rea / "notes")
        if r.parts[0] == "units" and p.stem in units:
            continue
        (notes / r).parent.mkdir(parents=True, exist_ok=True)
        (notes / r).write_text(p.read_text(), newline="\n")
    for d in ("zlib", "minizip"):
        for p in sorted((rea / "src" / d).iterdir()):
            if p.suffix not in (".c", ".h"):
                (notes / d).mkdir(parents=True, exist_ok=True)
                (notes / d / p.name).write_text(p.read_text(), newline="\n")

    # decl/text.dcl: the unit boundaries and the declared dead bytes
    order = sorted((first_addr[rel], stem) for stem, (rel, u, S) in units.items() if rel in first_addr)
    lines = []
    for i, (lo, stem) in enumerate(order):
        rel, u, S = units[stem]
        hi = order[i + 1][0] if i + 1 < len(order) else GAME_END
        tags = S.tags
        by = tags.get("named_by", "-")
        proof = tags.get("boundary", "?")
        why = f"rea {args.pin}: @boundary {proof}; {first_prose(S)}"
        byw = f" by={by}" if by in ("leaked", "tag", "guess") else ""
        lines.append((lo, 1, f"{lo:#010x} boundary start{byw} tu={stem} -- {why}"))
        lines.append((hi, 0, f"{hi:#010x} boundary end tu={stem} -- {why}"))
        for b in tags.get("bytes", []):
            m = re.match(r"(0x[0-9a-fA-F]+)\.\.(0x[0-9a-fA-F]+)\s+(.*)", b)
            if not m:
                print(f"import_rea: {rel}: unreadable @bytes {b!r}", file=sys.stderr)
                return 1
            a, z = int(m.group(1), 16), int(m.group(2), 16)
            lines.append((a, 2, f"{a:#010x}..{z:#010x} bytes dead -- rea {args.pin} @bytes: {m.group(3)}"))
    dcl = out / "decl/text.dcl"
    keep = [l for l in dcl.read_text().split("\n") if l.startswith("#")] if dcl.exists() else []
    dcl.write_text("\n".join(keep + [l for _, _, l in sorted(lines)]) + "\n", newline="\n")

    # the report
    nl = sum(len(u.linked) for _, u, _ in units.values())
    nu = sum(len(u.unlinked) for _, u, _ in units.values())
    nd = sum(len(S.data) for _, _, S in units.values())
    rea_funcs = {int(r["address"], 16) for r in names}
    ours = {a for _, u, _ in units.values() for a in u.linked}
    print(f"import_rea: rea {head[:8]} -> {out}")
    print(f"  units={len(units)} linked={nl} unlinked={nu} data_decls={nd} headers={len(headers)} dcl_lines={len(lines)}")
    for r in resolved:
        print("  " + r)
    for stem, (rel, u, S) in sorted(units.items()):
        if u.reordered:
            print(f"  {stem}: TU order differs from address order before " + ", ".join(f"{a:#x}" for a in u.reordered))
    print(f"  names.csv functions={len(rea_funcs)} missing={len(rea_funcs - ours)} extra={len(ours - rea_funcs)}")
    for a in sorted(rea_funcs - ours)[:20]:
        print(f"  missing {a:#x}")
    for a in sorted(ours - rea_funcs)[:20]:
        print(f"  extra {a:#x}")
    return 0 if rea_funcs == ours else 1


if __name__ == "__main__":
    sys.exit(main())
