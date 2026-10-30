# d3_vec.c

Unit notes: range and status are computed (`uv run ledger units`), the typed facts (name source, boundary proof) are in build/state/units.csv; the evidence behind them is **Unit** below. **Open work** (matching debt, header cleanup,
leftovers) comes first; below it the reports of the old worker rounds about this unit (merged 2026-10-22), one
section per worker, in address order of their scopes, original text. Old "round k" marks mean nothing now.

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (guess): vector / angle / matrix helpers, light budget counters
- Boundaries: separated (plausible): end: d3_Unk454780 belongs to d3_mir.c (codegen, layout 2026-10-22); name is a guess
- Range then: 0x453920..0x454780
- Was: unk_453920.c (round 12)

## Open work
- **headers (2026-10-30, typing pass 2)**: head.c includes d3.h, d3_vec.h, f3init.h; no @original cast or view.
- match-tu 2026-10-23 (4th run): d3_VecAngle MATCHED: `(float)(vb.z * va.z)` on the first product of the acos dot orders the three products (2 B left), then a dead store `va.y = 0.0f;` placed before `vb.y = 0.0f; va.y = 0.0f;` ranks va.y before vb.y in the second product (VC6 drops the store; a dead store at the very top or of vb.y instead: 8 lines). d3_MatFromAngles MATCHED: `(float)cz * cy` in _11 and `(float)a * cx` in _13 (2^7 family over the mixed double * float products: exactly those two). d3_MatInverse 67 -> 63 lines by respell's `paren` rule (L19.paren.2, climb stalls after it). d3_MakeBasis 2 B unchanged: `(float)` and `(double)` casts on the four operands of _11 (31 variants), `((float *)m)[k]` index spellings (16), dead reads of m->_22 into t / d at four positions (12), dead stores m->_22 = 0 (code, worse), the sqrt test order: all identical; fields through a pointer are not ranked by these. Lead: the rank of pointer fields may follow the FIRST OCCURRENCE of the field in the function (_33 is stored before _22 is); a form that defines _22 before _33 without changing the stores (e.g. the dir copy as a struct assignment so that _31.._33 have no field occurrences) is untried.
- match 2026-10-23 (7th), no gain: d3_MatFromAngles (94 B): cx / cy as `float cc[2]` (two init orders), as a struct `c.x / c.y`, only one of them as a one-element array: all worse (132-145 B, 22-44 lines); the CSE'd `a` / `t` inline in the m12 / m13 / m22 / m23 statements: worse; moving `m->_11` / `m->_21` to every position of the store block and `cz * cy` / `cy * cz`: 91 B at best (m11 at the front of the block, 31 lines), not kept.
- match 2026-10-23 (6th): d3_MatInverse 1035 B / 328 asm lines -> 879 B / 67 lines (same instruction count as the original, 444). What did it: (1) the source rewritten from the original's FPU code (symbolic stack simulation of the exe; the original is `det = D0*m0 - E1*m4 + E2*m8 - E3*m12` with left-to-right cofactor chains `A*m1 - B*m9 + C*m13`, outputs `-(chain * det)` / `chain * det`, o[0] = D0 * det); (2) the operand order of products is a per-element rank that depends on HOW m[k] is spelled: each element as `m[k]` or `src->_ij` (D3MATRIX field) changes which operand is loaded first; a hill climb over 16 spellings for the det statement and 16 for the 16 output statements (compile, count asm lines) took 328 -> 67 lines (the spelling mix in the source is that result; it has no meaning). Left (67 lines): the CSE'd 2x2 minors t1..t3 / u1..u3 held on the FPU are multiplied `cse * m[k]` (fld st(i); fmul mem) in ours, `m[k] * cse` (fld mem; fmul st,st(i)) in the original, plus (7,9) in two output rows. Tried without change: named float locals for t1..u3 (spilled: `fst` per local, worse), only d0 / u3 as locals (the original spills exactly D0 at [esp+0] and u3 at [esp+8]; climb 269 -> 125 lines, worse than 67), locals e1/e5/e9/e13 for the first factors (loaded eagerly), a dead sum of all elements at the top, output stores through `dst->_ij` (same 67 lines), operand swaps / parenthesis (canonicalised). Also no gain: d3_MakeBasis (2 B) parenthesis variants of the three `_11.._13` statements (every operand plain / `(x)`), d3_VecAngle (4 B) 729 spellings of the acos dot product (operands plain / `(x)` / `*(&x)`). Next ideas: climb over per-statement spellings (not per element), `(float)` casts / `+ 0.0f` on the CSE'd operand, c2prio on the FPU candidates.
- match 2026-10-22 (2nd pass), no gain: d3_VecAngle (4 B, the first of two products `fld [esp+8] * [esp+0x18]` comes first in the original): all 63 operand orders of the three c[] rows, all 47 orders of the d3_YAxis dot, 141 orders of the acos dot product (both / either use), and 448 `-(B - A)` negated spellings of the rows: none better (negated rows 75 B worse).
- match 2026-10-23 (4th): renames d3_Unk454040 -> d3_MatTranspose, d3_Unk454070 -> d3_MatInverse, d3_Unk454510 -> d3_MatIdentity, d3_Unk4545c0 -> d3_MatFromAngles, d3_Unk454690 -> d3_MatToAngles, d3_Unk454700 -> d3_TriNormal. d3_MatFromAngles (94 B): the original computes `cz * cy` as `fld st(1); fmul [cy]` (float operand second), ours `fld [cy]; fmul st,st(2)`: operand swaps of every product (275 sampled of 729), `(double)` / `(float)` casts on the float operands, a = sy * cz / t = sy * sz: all identical bytes (VC6 canonicalises the commutative order); the float locals cx / cy are spilled in both.
- Round 12 leftovers, d3_vec.c: d3_VecAngle 0x4539f0 (4/336) and d3_MakeBasis 0x453b70 (2/496), operand order only
  (notes/units/d3_vec.md); d3_Unk454070 (4x4 inverse, 1035/1184, m[k] load order), d3_Unk4545c0 (94/208),
  and d3_Unk454780 moved to d3_mir.c (layout 2026-10-22: 234 -> 5 bytes there; notes/units/d3_mir.md).
- Names: 0x453e80 is d3_VecEqual (d3.h) and d3_PtEq (d3_io_t.h); 0x4538a0 is d3_LightCalc4538a0(unsigned char)
  and d3_engine.h's d3_Unk4538a0(int) (its callers in d3_engine.c / d3_interface.c need the names.csv name for
  a real link); d3_engine.h sees d3_mir's D3MIRROR at 0xfb9900 as separate globals d3_Unkfb9900/04/08/28.

## vec (round 12, Sonnet): 0x4538a0..0x454040 [vec]

### Report
Scope: 11 functions in src/game/unk_453920.c (header include/unk_453920.h).

| addr | name | state |
|------|------|-------|
| 0x4538a0 | d3_LightCalc4538a0 | matched (last function of the d3_light TU, see below) |
| 0x453920 | d3_ShadeInit | matched (fills d3fl_Shade[i] = 1 - 2/pi * acos(i / 1024), sets the light budgets) |
| 0x4539f0 | d3_VecAngle | nonmatching, 4/336 bytes (see below) |
| 0x453b70 | d3_MakeBasis | nonmatching, 2/496 bytes (see below) |
| 0x453d60 | d3_UnwrapAngles | matched (Ghidra's prototype is wrong: (float *a, float *b, float *ra, float *rb)) |
| 0x453e20 | d3_Hermite | matched, missed by Ghidra (p0, p1, m0, m1, t on the stack, result on the FPU) |
| 0x453e80 | d3_VecEqual | matched (= d3_PtEq of unk_44cd40.h, same address, same code) |
| 0x453ee0 | d3_MatMul43 | matched (4x3 affine matrix product) |

TU boundaries / orchestrator joins:
- 0x4538a0 is the tail of the d3_light TU (src/game/unk_451e80.c), written here in unk_453920.c for the join.
  Its argument is a **byte compared unsigned** (`cmp al,1; jbe`): `unsigned char on`. d3_engine.h declares
  d3_Unk4538a0(int) and unk_40d680.h / unk_448ac0.c d3_LightCalc4538a0(char); both are the same address and
  the same cdecl call, but the unsigned compare only comes out with an unsigned char parameter. names.csv uses
  d3_LightCalc4538a0. The scalar/pointer type of the callbacks: d3_Unk453280 is called through
  d3_ForEachObject2 with a `void (*)(D3OBJECT *)` (it returns char in unk_451e80.c, d3.h says void).
- 0x453920.. is a separate helper TU (light budget counters 0x633b08..10 / 0x5fcc6c / 0x5fcc70, vector / matrix
  helpers); it continues past 0x454040 (vec2's scope: d3_Normalize 0x454560, d3_Unk454510, face normal 0x454700).
  The orchestrator can join vec2's file after unk_453920.c.
- Names: d3_PtEq (unk_44cd40.h) and d3_VecEqual (d3.h) are the same function 0x453e80; I defined it as
  d3_VecEqual (D3VECTOR *, matching d3.h). d3_VecAngle is declared locally in unk_402140.h, d3_sky.c, d3_snd.c.
- d3fl_Shade (0xfb9940) is declared in unk_43eab0.h; my header redeclares it (same type) to stay independent.

Shared-header additions: none. New header include/unk_453920.h (local prototypes of d3_Unk452e40, d3_Unk453280,
d3fl_MakeLight, d3_Unk454510 by address; extern d3_YAxis / d3_ZAxis, which are also in other TU headers).
The file includes only <math.h>, <windows.h>, <d3d.h>, d3.h (d3.h needs windows.h + d3d.h first).

### VC6 lessons
- Association of a sum of products is fixed by splitting it: `r = A + B; r += C; r += D;` matched
  where one expression `A + B + C + D` (in all 24 term orders) did not (d3_Hermite). Source order of terms in a
  single expression is canonicalized by ranks.
- A cross product that is dotted with a memory vector: with scalar locals (cx, cy, cz) the compiler folds the
  axis into each term; the original keeps the three differences on the FPU stack and loads the axis with
  `fld [axis]; fmul st,st(n)`. A `float c[3]` array (constant indices) gives that shape; `D3VECTOR4`
  locals with the stack slots in the order of first use (vb at 0, va at 0x10, nb at 0x20).
- `0.0f < expr` (fcompp with a loaded 0.0) vs `expr > 0.0f` (`fcomp [0.0]`): write the second.
- `acos(x) - 6.2831855f` WITHOUT a (float) cast gives a QWORD constant `fsub QWORD` (double arithmetic);
  `(float)acos(...)` gives the DWORD form.
- A 4x3 row product with a zero store per row: statement order `_11, _12, _13, _14 = 0` per row matched, the
  natural-looking `_11, _12, _14 = 0, _13` did not (the constant store is scheduled between the loads of the
  following statement).
- Struct member access `m->_31` / `m->_21` (no pointer locals) gives the CSE'd `edi = &m->_31`, `ebx = &m->_21`
  of the original (the pointer is passed to d3_Normalize); local float pointers made it far worse.
- Order of the stores into a result matrix vs. computing an intermediate local vector first (u before the
  copy of dir) changes the schedule of the whole prologue; try both.
- A goto-shaped if / else-if (`if (x == 0) { if (!y) goto done; } else if (x > 1) { ...; goto done; }`) matched a
  nested ternary-looking test that compiled to `setne / sbb`.

### Nonmatching: attempts
#### d3_VecAngle 0x4539f0 (4 bytes: the order of two terms of the dot product)
Source as committed: layout matches (D3VECTOR4 va, vb, nb), cross in `float c[3]`, `> 0.0f`, acos without cast.
The only difference: the dot `vb.z*va.z + vb.y*va.y + vb.x*va.x` is evaluated y, z, x in ours; the original loads
`vb.z*va.z`, then `vb.y*va.y` (vb.y first), then x. Tried (no gain): all 6 term orders x two operand
spellings, parentheses on any of the 6 operands (64 variants), a `float dot` local before / inside the if,
10 + 80 orders of the six init statements (the committed order `va.x, va.z, vb.y, va.y, vb.x, vb.z` is the best:
others change the frame or the cross products).

#### d3_MakeBasis 0x453b70 (2 bytes: the operand order of `m->_22 * m->_33` in _11)
Everything else matches (up vector `u` as D3VECTOR4 local computed before the copy of `dir`; member accesses;
fallbacks with d3_YAxis / d3_ZAxis; `(float)sqrt(..) < 1e-7f`). The spellings
`m->_22 * (m->_33) - (m->_32) * m->_23` / `m->_31 * m->_23 - m->_21 * (m->_33)` fix the other four operand
orders. Tried (no gain): parentheses on all 2^4 x 2^4 x 2^4 operand combinations of the three cross rows, the
six statement orders of the three rows, local copies of r.y / r.z, `float *f / *r` pointer locals.

## vec2 (round 12): 0x454040..0x454890, file src/game/unk_454040.c [vec2]

### Report
Functions: 0x454040 d3_Unk454040 (transpose), 0x454510 d3_Unk454510 (identity), 0x454560 d3_Normalize,
0x454690 d3_Unk454690 (matrix to angles), 0x454700 d3_Unk454700 (face normal).
Nonmatching (3): 0x454070 d3_Unk454070 (4x4 inverse, singular case tail-calls the transpose),
0x4545c0 d3_Unk4545c0 (angles to matrix, 94 bytes differ), 0x454780 d3_Unk454780 (hull edge picker,
register allocation only).
TU: the scope ends at 0x454890, which is the next function (not in scope). The TU start is not known; the file
is named unk_454040.c and joins the helpers of worker vec. No gaps found in the scope (8 Ghidra starts, all covered).
Shared headers: none changed. Header include/unk_454040.h (prototypes of this TU; d3.h already declares
d3_Normalize and d3_Unk4545c0, d3_anim.h/unk_450860.h the face normal).
Names: d3_Unk454780 takes (n, used[n*n], pts[n], &count, out) and returns char 1; it writes pairs of points
(edges of a convex hull, nrm . p >= -0.01 for every other point) and marks used[i*n+j] and used[j*n+i].

### Lessons
- sin/cos/atan2 from math.h compile to inline fsin/fcos/fpatan in this toolchain (no calls); do not hand-roll asm.
- `D3_ZERO_AT(m, n)` (param named directly in the asm) matched an identity-matrix function; the 1.0 stores
  come in the order _44, _33, _22, _11.
- 454040: a transpose written as `((float *)dst)[i*4+j] = ((float *)src)[j*4+i]` with plain for loops matches;
  pointer-walk spellings gave the right code with eax/ecx swapped.
- Face normal: `D3VECTOR4 u, v` locals (the 16-byte frame, v.z stays on the FPU) matched; scalar locals did not.
- d3_Unk454690 (z = 0 branch): order the stores x, y, z=0 (the compiler moves the 0 store after the fpatan).
- 4x4 inverse: the det and each output element are written out in full, no named cofactor temps (the original's
  repeated sub-expressions are VC6 CSE temps, kept on the FPU stack; named float temps change the whole shape).

### Tries on the nonmatching ones
- d3_Unk4545c0: sines as double locals, cx/cy as float locals, a = cz*sy and t = (float)(sz*sy) computed before the
  zero stores (needed for the CSE order), zero store order _34,_24,_14,_43,_42,_41. Left: two memory-operand load
  orders (cz*cy loads cy first; a*cx loads cx first and leaves a on the stack). Not fixed by operand swaps,
  parenthesization, declaration order (~20 variants), inline cos/sin in every expression (no CSE of the calls),
  hand-written __asm fsin helpers (inlined, but arguments go through temps).
- d3_Unk454780: frame and code identical apart from register allocation (orig: ebp=i, esi=count, ebx=pts, edi=j; ours
  count not enregistered). Not fixed by count/pts local copies, declaration orders, explicit row pointers,
  `out[*count]` spellings, `register`.
- d3_Unk454070: frame size and structure match; only the load order of the m[k] memory operands differs (not reacting to
  operand swaps in the text; the order seems tied to the first occurrence of each element in evaluation order).
  Tried: named temps (wrong shape), full inline (right shape), global operand flip (no effect).

## Also about this unit
- notes/units/d3_par.md, section [pfix]

## match-base 2026-10-23
- The light budget globals are named from the reading (renamed in d3_vec.c, d3_engine.c, d3_light.c,
  include/d3_engine.h): d3_Unk5fcc6c -> d3_LightKeyBudgetCulled, d3_Unk5fcc70 -> d3_LightKeyBudgetVisible,
  d3_Unk633b08 -> d3_LightKeysCulled, d3_Unk633b0c -> d3_LightKeysVisible, d3_Unk633b10 -> d3_LightGeneration.
  Evidence: d3_DrawObjectCB (d3_engine.c 0x43a160) relights a fully clipped light (D3CLIP::out, clip & 0x40)
  while d3_LightKeysCulled < budgetCulled, and one that passed the clip test while d3_LightKeysVisible <
  budgetVisible, adding s->info.nkeys to the counter; d3_LightCalc4538a0 (the relight pass) increments the
  generation and d3_Unk453280 stamps it into D3MESH::unk95c; d3_Render zeroes both counters per frame; the
  scene setup sets the budgets (f3init 0x513050: visible 0x800; town end: 0x400 / culled 0x40);
  d3_ShadeInit's only caller d3_Init passes (0x200, 1).
- d3_ShadeInit / d3_ShadeReset / d3_Normalize byte + readable (d3_ShadeInit fills the 1024 shade entries,
  d3_ShadeReset clears budgets, counters and generation).
