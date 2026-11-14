# VC6 SP5+PP /O2 – matching notes

Observed while matching; each item was confirmed by compiling variants (`uv run try`).

## Floating point
- Division by a constant becomes multiplication by its reciprocal, and constants are merged:
  `(float)rand() / RAND_MAX * 0.25f` compiles to one `fmul` by 0.25/32767. Any natural spelling matches.
- `x * 2.0f` becomes `fadd st, st`.
- Same-constant terms are factored: `0.0f*a + 0.0f*b + 2.5f*c` becomes `(a + b) * 0.0 + c * 2.5`.
  So `(m._11 + m._21) * 0.0` in a disassembly is a plain vector-times-matrix with zero components.
- Operand order of commutative ops is canonicalized; swapping operands in the source changes nothing.
  The order follows the stack layout, so fix layout problems first.
- Comparisons against `double` literals (`x < -1.0`) show up as `fcom QWORD PTR`.

## Stack layout
- Local struct slots are ordered by first use in the hot code (loop body), not by declaration
  order or name. A swapped pair of slots usually means the expressions differ, not the declarations.
- Slot SHARING (which locals the packer puts into one slot) does not follow names, declaration order or
  scope either (bmp_Load, 2026-10-23: all three tested, no effect). What moved it: (1) an inline-asm block's
  destination assigned to an existing function-level pointer inside the macro block shares the slot of a
  loop variable that is dead in memory there; a block-local temp of the macro never did; (2) a loop with
  several asm copies gets the original's pairs when each copy sits in its own block with fresh locals, not
  with two loop-wide pointer variables. A "mystery unreferenced slot" can be the symptom of wrong sharing
  elsewhere (bmp_Load's rle[6] stand-in became rle[2] once the sharing was right).
- Values copied to scalar locals (e.g. `wx = rain->windx;`) stay on the FPU stack; the original loading
  a struct field once and reusing it via `fld st(n)` is a hint for such a local.

## Arguments and loops
- A variadic function that walks `int *` past its last named argument, and re-reads the current argument
  from the stack after a call: written as `int *args = &n; ... args[++k]`. VC6 strength-reduces the index,
  so the base `&n` is computed in the loop preheader. A plain `p = &n; *++p` or `va_arg` does not match.
- `for (...; drop++, i++)` vs `i++, drop++` changes the increment order at the loop end.

## Globals and macros
- A global re-loaded from memory at every use, with no intervening store or call, is `volatile`
  (e.g. the frame timer `d3_Time`).
- `__LINE__` is pushed as an immediate and is not masked: pin it with `#line N` before the statement.
  Line numbers recovered so far tell us where code sat in the original file (d3_tools.c: 2343, 2348).
- Struct member store order follows source order; when only the order of a few `mov [x+off]` differs,
  permute the assignments with uv run try.

- A global written through an absolute-address macro (`*(int *)0x6ed938 = x`) schedules differently from a named `extern int g;` (the compiler hoists / sinks the loads and the `add esp` after a call): the same function matched only after the macro was replaced by the extern (0x52fa10, 0x52fa90, layout 2026-10-23). Convert before matching.
- A sign test on a byte (`if (b & 0x80)`) gives `test al,al; jns`, `(char)b >= 0` gives `jge` (0x5315c0). `(unsigned short)x != 0xffff` keeps 0xffff in a register for several compares, `(short)x != -1` does not (0x52c420).
## Engine conventions (GildeGold)
- Most engine vectors are 16 bytes (`D3VECTOR4`, x/y/z/w); a 4-byte gap after a vector in a struct
  or between stack slots is the `w` component.

## More from the snow functions
- `max(a, b)` from stdlib.h with an int converted inside it: `j = args[++k]; x = max(x, j * 2500.0f);`
  spills the int to a stack slot before `fild` (`mov edx,[..]; mov [esp+n],edx; fild [esp+n]`), and the
  comparison comes out as `fld a; fcomp st(1); and eax,0x4100; jne`. A plain `if` or a direct
  `fild [mem]` means no macro.
- A literal `1.0f * x` (or `-1.0f * x`) is folded away. If the original multiplies by 1.0, the constant
  went through a variable (`gy = -1.0f; ... gy * m._21`); constant propagation then emits it as
  `fld x; fmul 1.0; fsubp`.
- Slot order of two structs written in the same block can be fixed with an early dead initialization of
  the one that must come first (`fall.x = 0.0f; ...` at the top). Declaration order has no effect.
- `D3_ZERO` (include/d3.h) is an inline-asm clear, 284 copies in the exe: `lea ecx,[edi+edi*2]; and ecx,3`
  aligns, then `rep stosb/stosd/stosb`. The inline asm forces an ebp frame on the containing function.
  A 4-byte flags struct modified with `or dl,2` on byte +2 is `flags.b[2] |= 2` (D3TEXFLAGS).

## Operand order and ranks (rain/snow update functions)
- Commutative operands are not ordered by source position but by an internal rank. For two named
  scalar locals, the one whose first textual occurrence comes later is loaded first; even a dead store
  near the top (`s = 0.0f;`) moves a variable's rank. A compiler CSE temp ranks above named locals:
  `flake->sy1 = cy - y0 * f; ... flake->sy2 = flake->sy1 + s;` loads the CSE first, the original
  needed a named local (`flake->sy1 = y = ...; flake->sy2 = y + s;`). Declaration order has no effect.
- The rank also decides how `a + b + c + mem` chains are associated, so a change that only touches
  ranks can move a `fadd [mem]` to a different position in an unrelated statement.
- Parentheses are not neutral: `(drop->pos.w)` and `x = (p->x += 2.0)` compile differently from the
  bare form. A parenthesized member read gets its own rank; in d3t_UpdateRain it is the only spelling
  that loads `drop->pos.w` before the local `vel.x`.
- Any such change can also flip the stack slot order of two equally used locals (wind/fall). Restore it
  with an early store to one of them; in d3t_UpdateRain a partial initializer `fall = {0.0f, -0.75f}`
  works, a full one does not. Treat these as tie-breaks, not as evidence of the original spelling.

## Keeping a value on the FPU stack across a wrap loop
- `p->x = expr; while (p->x < -1.0) p->x += 2.0;` spills the value (`fst tmp; fstp [p]; fld tmp; fcomp`).
  The original's `fcom; fst [p]` with the value kept on the stack, and a fresh `fld [p]` for the next
  loop, comes from `x = p->x = expr; while (x < -1.0) p->x = x += 2.0;` (d3t_UpdateSnow).
- Ranks also break ties in the instruction scheduler: `wz = (snow->windz);` moves an unrelated
  `fstp` two instructions later. Integer loads (d3_Viewport) did not react to any parenthesization.

## Particle callbacks (d3t_EmitterCB, d3t_BloodCB)
- `p->x = (x = expr);` compiles to `fst x; fstp [p->x]` (local first). Without the parentheses,
  `p->x = x = expr` stores the local with `fstp` and copies it with integer moves, and
  `x = p->x += expr` gives `fst [p->x]; fstp x`.
- A float local the original keeps in memory where ours stays on the FPU stack (`fst z` / later
  `fmul z`, and `fld z; fmul z` for `z * z`) can be declared `volatile`.
- Constant multiplies are moved to the end of a product: `(float)rand() / RAND_MAX * (r + r)` becomes
  `rand * (r + r) * (1/RAND_MAX)`. Casting the quotient, `(float)((float)rand() / RAND_MAX)`, keeps it first.
- `(float)(rand() / (float)RAND_MAX * 10.0) - 5.0f` gives `fmul QWORD 10.0; fsub DWORD 5.0`; without the cast
  the 5.0f is widened to a QWORD constant.
- A store that is overwritten before any read is dropped and the expression moved to the second store,
  which can move a call (rand) past unrelated stores. If the original keeps both, look for a different
  expression shape rather than for aliasing.
- `(float)sqrt(a) <= b` compiles to `fcomp b` / `test ah,0x41; jp`; the double compare `sqrt(a) <= b` loads b and uses `fcompp`.
- The order of a dot product `n.x * v.x + n.y * v.y + ...` follows ranks like scalar locals do;
  parenthesizing one term, `((n.x * v.x) + ...)`, moved it first.
- Two stack slots swapped between a counter and a parameter-slot local: initialize them in the other order.
- Loop-invariant expressions are hoisted into the loop preheader, after the `count > 0` guard. A value
  computed there in the original (alpha, a size factor kept on the FPU stack) was written inside the
  loop body. `t = 0` in the preheader with `t += delay` per iteration is `i * delay` strength-reduced.
- `p = ps->particles` as part of the `for` initializer (`for (i = 0, p = ps->particles; ...)`) rather
  than before the loop changed which variable got the parameter's stack slot (d3t_SmokeCB).
- A byte from a struct parameter loaded as `mov ebx,[esp+n+3]; and ebx,0xff` before an unrelated call
  is an `int` local assigned from the byte (`mask = color.a;`), not a direct use.
- VC6 does reuse stack slots: locals with disjoint lifetimes share one, and dead parameter slots hold
  locals. Which one goes where did not react to declaration order.

## Sun rays (d3t_CreateSunRays, d3t_UpdateSunRays, d3t_SmoothStep)
- Float member copies done with `fld/fstp` instead of integer moves, and the stores interleaved with
  pushing the arguments of the call that uses the vector, come from a comma-expression macro used as
  the argument: `D3T_VSET(v, a, b, c)` = `((v).x = (a), (v).y = (b), (v).z = (c), &(v))`. A plain
  statement-level `v.x = w.x` is always an integer copy.
- `min(max(expr, 0.0f), 1.0f)` with the expression inside the macro (CSE'd) gives `fld st(0); fcomp`;
  with a variable `t` it gives `fcom`.
- `a = fmod(a, ...); f(a)` vs `f(fmod(a, ...))`: the former stores the argument after the caller pops the
  previous call's arguments (`add esp,8; fstp [esp]`).
- A local used only inside one `if` block shares its stack slot with temporaries when declared in that
  block; the frame size told us which vectors were 16 bytes (D3VECTOR4) and which 12.
- `uv run ledger check` does not rebuild; run `uv run build` first.
- `a == b` vs `b == a` sets which operand is loaded first in an integer compare (d3t_UpdateCamera).

## Repeated tests become CSE temps (d3t_PlayAnim)
- `flags & N` values kept in dead parameter / asm-pointer slots while the pointer parameters stay in
  esi/ebx: the original repeats the test (`if (flags & 2) af...; ... if (flags & 2) anim->...`), and the
  CSE temps rank below named locals for registers. Named locals (`if (loop = flags & 2)`) took esi/ebx.
- D3ANIMFLAGS.b[1] carries the same bits as D3ANIM.flags2d (0x10 loop, 0x08, 0x04, 0x02).

## d3t_PlaceObject (nonmatching, 0x411680)
- Layout and data flow match. Remaining: the two flag-byte RMWs right after D3_ZERO are serialized
  through cl in ours (orig: al and dl in parallel), and `dir.x/dir.y -= cam` are stored with `fst` and
  forwarded on the FPU stack to keys[1].rot in the original, where ours reloads them as integers.
  Not fixed by: statement order (all 144 init orders, all 720 key copy orders), bitfields, named
  members, comma/VSET macros, scalar temps, struct copies, `(dir.x -= ...)` forms.

## Bounding helpers (0x411c70..0x412090)
- Early `return 1` guards vs. nested `if`s with one final `return 1`: the original nests (one shared
  epilogue at the end); guards give one epilogue per return.
- A loop bound re-read from the struct (`i < obj->unk1cc->nkeys`) instead of a local `n`: the compiler
  can't thread the first loop's exit into the second loop's guard, so both exits jump to a shared `cmp`.
- `max(0, x0)` gives `setl` (x0 < 0 ? 0 : x0), `max(x0, 0)` gives `setle`.
- A 16-byte frame holding only x and z of a sum (y kept on the FPU) was a D3VECTOR4 local.
- `r = max(d, r)` with `volatile float d` reproduced `fstp d` / two loads of d, except that the original
  compares with `fcom d` (mem operand) where ours does `fld d; fcomp st(1)`. The sum loop's pointer
  base (orig &v[1], ours &v[1].z with `v[i + 1]`) is fixed by an explicit `w = v + 1; ...; w++` pointer.
  A `double r` keeps a non-volatile float d in memory (it must be rounded) but still loads it first,
  and makes the 0.0 constant a QWORD.
- d3t_GetTriRange: an unexplained 0x20 frame with gaps between int slots was `int x[3], z[3]`: elements
  with constant indices are enregistered individually, but the array keeps its full stack slot.
- `fild a; fild b; fdivp` (both ints converted, then divided) is a double division:
  `(double)(a) / (double)(b)`. `(float)a / (float)b` and `(float)a / b` both compile to `fidiv`.
  The exact spelling in d3t_GetTriRange was `((a) * 1.0) / (b)` (the `* 1.0` folds away): same
  instructions, but the `fdivp` is scheduled one or two integer instructions earlier. Without the
  parentheses around the product it behaves like the cast form.
- The order `xl = x; dl = ...; xr = x; dr = ...;` (instead of `xl = xr = x;` up front) moved a
  temp into the slot later reused by dr, as in the original.
- A value loaded before a branch and returned on one side, used as a loop accumulator on the other
  (d3t_GetMeanHeight): the original had one `return sum;` after `if (ok) { loop; sum /= 64; }`;
  an early `return sum;` instead swapped two stack slots.
- d3t_GetQuadRange: `if (r1 && r2) return 1; if (!r1 && !r2) {...} else if (!r1) {...} else {...}`
  was the only shape that put the blocks in the original order.
- d3t_InTriBox (matched): what looked like the same pointer loaded twice into edx and ecx was two
  different parameters. `[esp+0x2c]` before and after a `pop esi` are different slots; always
  recompute parameter offsets after pushes/pops. The mn tests read the third ("unused") argument.
- d3t_QueryHeight: the 8-vertex bbox loop is `for (i = 1, v++; i < 8; i++, v++)` with `min(mn.x, v->x)`
  / `max(mx.x, v->x)` (accumulator first). `v[i]` indexing or a `v++` at the loop top folds the first
  step into `[ecx+0x50]`; explicit `if (v->x <= mn.x)` stores flip the compare flag tests. In a
  `for (...; i++, tri++)` loop the pointer step comes first in the original: `tri++, i++`.

## d3t_DropObject (0x412a70)
- VC6 /O2 does save callee-saved registers mid-function (push/pop around the region where they live);
  that alone is no hint of block-scoped locals or inlining.
- An unreferenced 4-byte gap at the top of the frame was a D3VECTOR4 local of which only x/y/z are used.
- Repeating the cleanup (`if (parent) d3_SetParent(...); return 1;`) at each failure site instead of a
  shared `goto fail` changed both the block layout (which duplicate restore block survives tail
  merging) and the register ranking (parent ebx, loop accumulator ebp).
- `if (p = obj->parent) {...} else p = 0;` is the source of a `jmp; xor reg,reg` pair.
- A sibling walk that keeps the current object in the register being dereferenced
  (`mov edi,eax; mov eax,[edi+0x1f4]`) came from a small __inline helper; written inline, VC6
  dereferences the copy instead.
- uv run asmdiff --regs a=b,... renames our registers before diffing, to separate register
  allocation from everything else.

## d3t_MoveCameraPath (0x4136d0)
- 16 individual zero stores with 0 kept in a register (`push edx` for 0 arguments, `cmp cl,dl` in strcpy),
  where the first four use `eax` and come before the rest: `objs[16] = {12 explicit zeros}`. The explicit
  part is stored from a zero register, the implicit tail separately. `= {0}` gives `rep stosd`, 16 explicit
  zeros puts all stores after the first test.
- Here declaration order of initialized locals does matter: `int count = 0;` before the array moved its store
  ahead of the array stores.
- Varargs walked with `lea ebx,[ebp+0xc]; mov eax,[ebx+4]; add ebx,4` and the loop counter in memory:
  `char **args = (char **)&n; ... args[++k]` (k a second counter), as in the snow/rain setters.
- A key pointer based one key too far (`[ecx-0x5c]` for the time) was `keys[k - 1].time`.
- D3_ZERO twice in one function needs per-line asm labels (C2412 otherwise); the macro pastes __LINE__.

## d3t_MoveObjectPath (0x413960, nonmatching)
- Data flow, layout and stack slots match (i [ebp-4], step [ebp-8], k in the dead `time` slot). Left: the
  original caches i in edx and k in ecx (ours the other way). Not changed by: declaration order or
  initialized declarations, preamble statement order (71 permutations incl. the af ops), loop-body order,
  `k - 1` vs pre-incremented k, a key pointer, `o` vs `path[i]`, `time / n` as a CSE instead of `step`,
  `n > i`, unsigned/long counters, `for (i = 0, k = 1; ...)`, or reusing the `time` parameter as k
  (that one does fix the init order `xor edx,edx; mov ecx,1`, but not the registers).
- The D3_ZERO size stored before the pointer was the macro itself: `int _zn = (size); void *_zp = (dst);`
  (size declared first). All other D3_ZERO users still match with it; CreateSnowMipmaps improved too.

## d3t_CreateSnowMipmaps (nonmatching, 0x416980)
- `#line` numbers give the source order of the D3DCHECKs (Lock 3749, GetPalette 3758, Unlock 3762,
  GetEntries 3768). The original repeats `tex_Release(snow_tex); snow_tex = 0;` in each failure branch
  of nested ifs, then `if (!snow_tex) { fallback }`; VC6 tail-merges the copies into one block and moves
  the GetEntries `else` out of line. The goto-based single-release version is 80 asm lines worse.
- `for (i...) arr[i].field = ...` on a global array strength-reduces to a pointer at one member
  (here &dwWidth or &dwHeight); a local `d = &arr[i]` pointer gave the wrong base.
- Blur loop: explicit `up/row/down = &tmp[(y-1 | y | y+1) * size]` per row gives the original's three
  pointer IVs and frame size. The original re-tests the x-loop guard inside the y loop; `x < 1 << i`
  instead of `x < size` reproduces that (a bound the compiler can't prove equal to the y bound).
- Separate `w = h = 1 << i` variables are copy-propagated and make it worse; `volatile int n` too.

## d3t_EmitterCB (0x4156a0, matched)
- `(float)sqrt((double)(a*a + b*b + c*c))`: the explicit double cast of the sum changes the order the
  squares are added in (x, z, y here). Without it the order is canonical and no permutation moves it.
- `fsqrt; fstp st(3); fstp st(0); fstp st(0); fstp [speed]` (dead loads popped before the store) where a
  direct `speed = sqrt(...)` stores first and pops after: the root goes through another local,
  `s = (float)sqrt(...); speed = s;`.

## Session 6 leftovers (attempts that did not help)
- d3t_EarthCB: original pairs slots alpha<->s and dead<->len (ours alpha<->len, dead<->frames). Reusing `s`
  for alpha fixes dead's slot but loses the volatile memory shape; a volatile `s` breaks the first block
  (`fadd st(0),st` needs s non-volatile). Variable names, statement order, `dead` placement/type, alpha
  spellings and inlining s had no effect.
- d3t_CreateSmoke: the `now` load is scheduled before `rand() & 7` is used; all term orders, casts,
  separate statements, volatile now/i, `t += delay` and loop-increment orders tried.
- d3t_UpdateSnow: moving the viewport lines, `/ 2`, casts and parenthesization don't move the int loads.
- d3t_CreateSnowMipmaps: all 120 orders of the 8-bit mip loop statements, chained width/height, memcpy
  for the ddpf copy and pitch/alloc spellings don't give the `[edi-0x40]` height store.

## d3_snd.c (0x417650..0x418ce0, session 7)
- A function whose success return is `0` held in a callee-saved register (`xor esi,esi` at entry,
  `mov eax,esi` at the end, `or eax,-1` on failure) is `int ret = 0; if (...) {...} else ret = -1; ...
  return ret;` with every failure as an `else ret = -1`. Early `return -1`s give `xor eax,eax` instead.
- D3_ZERO on a global destination: the original names the global in the asm (`mov edi, ds:global`), no
  pointer temp. `D3_ZERO_AT(var, size)` in d3.h.
- A tail call that rewrites only the first stack argument (`mov [esp+4],eax; jmp`) of a callee taking
  two arguments: the caller has the second parameter too and passes it through (d3snd_Stop).
- Out-of-line blocks (code after the function's main `ret`) are usually the `else` of an `if` whose
  test was written the other way round: `if (s->snd) {...} else {create}` (d3snd_UpdateVolume),
  `if (!a || !b) {fail} else {ok}` (d3sndw_GetSound, where the ok tail is merged with another case).
- `x = sqrt(...) * k` puts the `fmul` before the dead x/y/z pops; the original multiplies after them:
  `x = sqrt(...); x = x * k;`.
- `unsigned short` loop counter against an `unsigned short` bound: the compare stays on the word
  parameter (`cmp WORD PTR [ebp+8],0`) and the trip count goes into a separate int temp.
- A volatile read once per use inside a multiply-by-constant lea chain (`mov eax,T; mov ecx,T;
  lea edx,[ecx+eax*2]; mov eax,T; lea eax,[eax+edx*4]`) is three references in the source, e.g.
  `(T << 3) + (T << 2) + T`; `T * 13` reads T once. Subtracting from it in the same expression lets
  VC6 reassociate; the original assigns first (`t = ...; t -= last;`).
- d3sndw_Update (nonmatching, 169 asm lines, same size and layout): the original keeps constant 0 in
  ebx only up to the main loop (immediates afterwards) and has ground in edi, nfound in ebp; ours keeps
  0 in ebp for the whole function. Tried: zeroing spellings (chained, via a variable), statement
  orders around the init, scene-check structure, dir.y computed then cleared, operand orders.
  A D3VECTOR4 `rel` (not scalar dx/dy/dz) for the probe offset fixed the rotation's FPU schedule.

## From worker d3_d3d (d3_anim.c tail, d3_d3d.c, d3_engine.c head)
- VC6 knows which registers a function *defined earlier in the same file* leaves untouched and keeps
  caller-saved values (e.g. edx) live across calls to it. A callee defined in the TU vs only declared
  changes the caller's code (d3anim_SetFrame needs d3_Unk4324e0 above it); usable as TU-boundary evidence.
- `if (c) D3DCHECK(a); else D3DCHECK(b);` needs braces around each branch for the original block layout.
- A 1-bit bitfield assignment compiles to `add cl,cl; xor; and; xor`.
- A self tail-call is turned into a loop.
- Chained assignments `a = b = c = v` store right to left. In an if/else-if chain VC6 merges trailing
  branches that share such a chain; separate per-branch statements keep them apart as in the original.
- A `min()`-style conditional expression gives the fld/fstp float copy; `if (...) x = y;` copies as integer.
- Locals declared in inner blocks change which of them reuse dead parameter slots; declaration order
  alone does not change the frame.
- Inline-asm copy macro temps: declare size, src, dst in that order; VC6 does not substitute a plain
  variable into the asm, and loop-invariant temps are hoisted.

## From worker dd_vesa (dd_vesa.c, unk_41ebc0.c)
- `while (1)` and `for (;;)` differ: `for (;;)` is rotated (loop head duplicated).
- Same-TU register knowledge again: edx stayed live across a call to dd_BppToDDBD; the match loads
  `hw->dcmColorModel` into a local before the call.
- Source order of switch cases decides block layout and which copy of a shared tail survives
  (dd_Exit 0,1,3; dd_SetMode 3/4, 1/2, 0).
- `strcmp("literal", buf)` is inlined as `repz cmpsb` with the literal's length (operand order matters);
  `memcmp` with a size that is a multiple of 4 gives `cmpsd`.
- An `__inline` function consisting of `__asm` that returns in al is inlined (cpuid MMX test); VC6 knows
  the registers an asm block clobbers.
- Byte-wide read-modify-write of flags means `unsigned char` bitfields; `unsigned int` ones give dword ops.
- A COM pointer declared inside the `if` that uses it shares the loop counter's slot (dd_DlgProc). Hoisting
  `SendMessage(GETCURSEL)` out of a nested call into a variable changes the push order.
- Loop rotation: `x0 = x1; x1 = ...` at the top of the body (not the bottom) gives the loop-entry `jmp`
  (surf_Shrink16). The init order of a group of globals decides which one is enregistered in a later loop.

## From worker net (msx.c, net.c and unnamed TUs 0x41f000..0x426000)
- Locals land in dead parameter slots: mov_WndProc matched only with GetEvent's two out-parameters as real
  `long` locals instead of `&wp`/`&lp`.
- An if/else whose branches share a tail (LeaveCriticalSection) gets the tail duplicated into both
  branches (msx_Stop); a single early return does not.
- Value returned from a callee-saved register on several paths: `int id = 0; ... return id;`, with any
  `goto fail` target that returns 0 placed after the final return.
- A hoisted byte shift count (`mov dl,8; sub dl,cl`) is `x >> (BYTE)(8 - n)`.
- `while (m) { m >>= 1; n++; }` gives `shr; inc; test`; `for (; m; m >>= 1) n++` gives `inc; shr`.
- 24-bit pixels as `p[x*3]`, `p[x*3+1]`, `p[x*3+2]` give the original's base+1 pointer; `p += 3` does not.
- Zero-macro temps: their declaration order decides store order (the memory TU wants dst before size);
  mem_Alloc needed the asm to name plain variables (`n = size; p = malloc(n);`).
- `e++, i++` vs `i++, e++` in pointer+counter loops differs per function; try both.
- Volatile time product: assign then subtract (`el = TIME; el -= start;`), as in d3_snd.c.
- Unsolved: a constant 0 kept in a register and returned via `mov eax,ecx` (mov_Find).

## From round 2 (workers d3_anim, d3_engine, sblib, script)
Control flow and block layout
- One `return` at the end of the function: VC6 then moves the callee-saved pushes after an early
  `if (!x) return y;` and still copies the epilogue to several `ret`s. Several `ret`s in the original do not
  mean several returns in the source (fixed 5 sb_* functions; vfs_ftell needed it for its tail calls).
- The out-of-line block is the `else` of the if: swapping the branches fixed several evt_* functions.
- `while (1) { if (c) break; ... }` keeps the test at the top; `while (!c)` gets rotated. A loop-invariant
  test in the condition (`for (i = 0; f && i < 8; i++)`) also gives an unrotated loop, with i zeroed before
  the test. `for (k = s, len = strlen(p); ...)` and `len = strlen(p);` before the loop shape differently.
- `box = 0; if (a && b) box = f(); if (!box) ...` lays out differently from `if (a && b && (box = f()))`.
- Empty cases (`case X: break;`, `return;`, `(void)0;`, a dead store) are dropped before the switch tree is
  built; no body was found that keeps them (d3d_Error 0x418e90 nonmatching because of this).
Registers, locals and stack slots
- A value returned from a callee-saved register on failure paths: `else s = NULL;` + shared `return s;`
  (`xor esi,esi; mov eax,esi`), or break out of the loop, then `if (!v) error; return v;`.
- An uninitialized local that is returned (e.g. after `|= 0x40`), or left unset in an `__asm int 3`
  branch, gets a dead parameter slot / its own stack slot.
- A value stored with `fst` and reused later is a float local: `d3_HalfW = hw = w * 0.5f; d3_CenterX = hw + x;`.
- In functions with `__asm`, a `ret = -1` local lives in memory; the original hoists -1 into eax instead.
- Strength reduction over `obj->material->tex[i]` only happens with the material held in a local.
  Indexing `v[i].field` instead of `v++` moves the loop pointer to the most-used field. Re-reading the
  loop bound in the test fixed the register layout of a strcmp search loop.
- In varargs functions, `va_arg` (not walking `&n`) gives the original push order.
Expressions and types
- Vector math `v = M*d + pos` matches only with the position added in separate statements
  (`v.x = dot; ... v.x += pos.x;`); spline sums need `*= 0.5f` as its own statement.
- Copy 12-byte vectors field by field; a struct assignment gives lea-pointer copies.
- `and eax,1; cmp al,1` is an int parameter tested as `(flags & 1) == 1`. `(b & 1) || (b & 0x10)` keeps
  0x11 as an immediate; `b & 0x11` hoists it into a register.
- `or bl,0xff; cmp [..],bl` is a signed `char` compared with -1 (`unsigned char` + 0xff gives `mov bl,0xff`).
  On a signed char, `x |= 0x80` and `*(unsigned char *)&x |= 0x80` compile differently.
- Loops over a global array comparing with `jl` were index loops; pointer loops give `jb`.
- `(unsigned)((char *)p - (char *)base) / sizeof(T)` gives the unsigned divide-by-multiply sequence.
- `min()`/`max()` macros on globals give compare-then-conditional-store. `abs(0x7f) / 2` is not folded.
- Initialized local char arrays in an inner block are filled where that block starts, in declaration order.
- A global `char[]` (not a literal) is why `strlen` of it is computed at run time.
- Ghidra's `(int)(float)` can be an `fstp` into a float union member.
- Passing a char through a `char c` local copied as `*(int *)&c` stopped two identical branches from
  being merged (script builtins take pointers to argument slots).
- The position of a constant store relative to a call (`n = 1;` before vs after `evt_Error(...)`) moves
  where the constant is loaded.
Caveat
- Near-misses can flip with unrelated code above them in the file: d3_CloneObject's reload order changed
  when d3_RadixSort (12 asm labels) was added and changed back later. Judge them once the file is complete.
- Round 3 again: d3_CloneObject regressed after the d3_left merge and matched again after d3_engine2's merge;
  adding 1-7 unused `__inline` functions above it changed nothing, so the trigger is not the definition count.

## From round 3 (workers evt, d3_left, d3_engine2, start401)
Control flow and return shape
- `int ret = 0; if (...) {...} else ret = -1; return ret;` with every nested failure an `else ret = -1` is
  the shape of all 25 MSS wrappers; the zero in the ret register is reused for zero arguments and stores.
  Likewise `ret = 1; ... return ret;` gives ret-in-bl / `mov al,1`, an explicit `return 1` does not.
- `if (i < 256) return &a[i]; return 0;` and the inverted form choose different fall-through returns.
- `if (!a || x) f();` with the call setup duplicated is `if (a) { if (x) f(); } else f();`.
- Splitting an `||` test into two ifs, each with its own cleanup + goto fail, keeps the fail block inline.
- A `found` flag or `goto next` in a duplicate-search loop matches; `if (i == n)` after the loop adds a cmp.
- `min(64, max(16, n))` gave the original clamp block order; `and eax,0x4100; je` / `test ah,5; jnp` is
  `a = max(a, 0.0f); a = min(a, 1.0f);` as two statements.
Stores, zeros and order
- Several late zero stores from one `xor eax,eax` were `memset(&s, 0, 12)` (the globals are one struct);
  separate `= 0` stores keep 0 in a register for the whole function and wreck the schedule. Likewise
  `*(int *)&f = 0` can fold into a zero kept in ebx; `memset(&f, 0, 4)` avoids it.
- `p = &tab[i]; if (p->f & 1)` puts the store before the test; the store after it was
  `if (tab[i].f & 1) { p = &tab[i]; ... }`.
- Inlined strcpy of short literals and struct member stores interleave in source order: brute-force all
  orders with a generator script for `uv run try` instead of trusting Ghidra's order.
- A local `&arr[i]` into a global array is folded back into `[i*4+arr]`; writing `arr[i]` everywhere gave
  the original's `lea` precomputation.
- Locals assigned before an `__asm` block are computed at function entry; moving such an assignment moves
  the `lea` around neighbouring stores.
Types and expressions
- `GetBValue/GetGValue/GetRValue` give `mov al,ah` + byte load at +2; `(unsigned char)(c >> 8)` gives `shr`.
- On a signed char, `!(f & 0x80)` is `test; js`, `f >= 0` is `jl`. Flag bytes updated in place with
  `and/or BYTE PTR [esi+x]` were unsigned char bitfields (CHARACTER+0x8c).
- `fild g; fisub [m]` is `(float)g - intfield`.
- `mov edi,[ecx+4]; add ecx,4` with the base `lea` in the preheader after `nargs > 0` is `va_arg`.
- A 12-byte D3VECTOR argument (not D3VECTOR4) fixed a frame; one D3VECTOR4 local reused for two groups of
  values explained a frame size and which components stay on the FPU.
- "Load the pointer once, store twice" comes from a pointer local or an 8-byte struct copy; field-by-field
  code through `t->v[i]` reloads after each store.
Stack frames and asm
- Frames with gaps: `int w[4], d[4]`; one local array reused for unrelated values; an index loop with the
  row pointer in memory was a 2D array `tiles[5][5]`, not a `t += 5` pointer.
- The parameter order of an inline-asm helper decides its spill slots ((float, int *) vs (int *, float)
  moved 70 asm lines). Declaration order of macro temps decides store order and slots.
- `emms` without an ebp frame: declare `void _m_empty(void); #pragma intrinsic(_m_empty)` by hand (no
  mmintrin.h); `__asm emms` forces an ebp frame. All-`__asm` functions (MMX, `align 16`) match directly.
- `__inline` functions containing asm were not inlined; asm copies that address plain variables need
  explicit pointer variables in the source.

## From round 4 (workers sfx, d3fl, ch, ch2)
Operand order and compares
- `max(0, y - n)` is `sub; sets; dec; and`, `max(y - n, 0)` is `test; setle`; `max(1, v)` jae vs `max(v, 1)`
  jbe; same for min and for clamp ifs (jg/jge, which operand is loaded first). Try both orders per statement.
- On an unsigned char `x < 0xff` is `cmp; jae`, `x == 0xff` is `jne`: the original mixes both spellings.
  Byte compares need both operands of the same signedness, else movsx/movzx appear.
- Floats: `!(a < b)` is `test ah,5; jnp`, `a >= b` is `and eax,0x100`. `d > (k + a) + b` keeps the local k
  on the FPU and adds the members in source order.
- A value tested twice in a row in the original is a re-read field (`if (CH_SLOT(ch) && ...)`); a local
  lets VC6 drop the second test.
Control flow
- One shared `return 1` at the end keeps all callee-saved pushes at entry; a `return 1` inside a branch can
  defer them (shrink-wrapping).
- Repeating the cleanup at each failure site (instead of `goto fail`) matched: VC6 tail-merges the copies,
  register choice follows source order. To keep two identical blocks apart, reorder the tail
  (`if (!x) return -1; return y;`).
- Search loops: guard `if (t && t->list)`, loop `for (r = t->list; r->y >= 0; r++)`, hit via
  `p = ...; break;` and a single `return p`; or the success path inside `if (i < MAX) {...}`.
- Unresolved: `cmp eax,imm; jne next` with an empty fall-through (empty if/self-assign/empty asm all drop
  the compare).
Types, arguments, stores
- `or/and BYTE PTR [p+1],imm` on a flag word is an `unsigned short` bitfield (VC6 narrows word ops to a byte
  op); `unsigned char` flags give load/op/store. CHARACTER+0x8c is a 16-bit bitfield.
- A by-value 12-byte flags struct whose middle dword stays in a register has separate fields (a bitfield
  int at +4); `unsigned char b[12]` keeps it in memory.
- `g((T *)f(x), 1)` pops f's args at once; `a = f(x); g(a, 1);` defers all pops into one `add esp`.
- An uninitialized local set on one path only is read from a parameter's stack slot on the other path
  (looks like an extra argument the callers don't push).
- `strcpy(dst, q + 1)` after `if (strlen(q + 1))` CSEs `q + 1`; `memcpy(dst, src, strlen(src) + 1)` gives
  `repnz scasb; rep movsd/movsb`.
- `for (i = 0; i < n; i++) va_arg(...)` gives the countdown in a stack slot; `while (n-- > 0)` does not.
- 2D arrays through index locals (`map->tiles[ty][tx]`) keep `base + idx*size` as a CSE with the field
  as displacement; a `t = &tiles[..]` pointer folds the offset into the lea.
- Order of independent loads and of struct field stores follows the source; parenthesizing a cast
  expression can change the store schedule. `near` is a windows.h macro (cannot name a local).

## From round 5 (workers ch3, ch4, txt, map)
Toolchain / workflow
- An undeclared function compiles silently (implicit int, C4013) and changes the caller's code: one missing
  prototype was 64 of d3fl_UpdateTiles' differing bytes. Grep build logs for C4013.
- Ghidra misses starts after short functions and callbacks only reached via pointers (action callbacks,
  timeSetEvent procs); check nop padding in `uv run disasm`. A Ghidra "thunk" can be a real tail call.
- `uv run try`: a replacement with empty new text (`===` directly followed by `>>>`) swallows the next block; put
  a comment in as the new text.
Control flow
- `test; je; push K1; jmp; push K2` + one call is `if (c) f(.., K1); else f(.., K2);` (tail-merged);
  `f(.., c ? K1 : K2)` gives setcc/sbb. An argument shared by both arms must be in a variable first.
- `if (!x && c) f(0,0); else if (c) f(v,1);` tests x once and threads the repeated cmp; `else if (x && c)`
  keeps an extra test.
- A pointer-returning builder (`test eax,eax; jne; ret`, eax live to the end) is
  `if (!a) return 0; ...; return a;`.
- A 0 failure return from a register later reused for another variable is a `type = 0;` statement at that
  point + nested ifs + one `return type;`; `int type = 0;` zeroes a different register at entry.
- Search that counts the slot it stops on: `while (t[i++].p && i < 32); if (i > 32) ...; i--; t[i]` (the
  `i--` gives `lea [ecx+ecx*2-3]`; `t[i - 1]` folds into the displacement).
- `{ if (*p == c) *p = d; p++; }` in a counted loop gives `dec ecx; jne`; indexing `p[i]` gives `cmp/jl`.
- Unsigned count: `test; jbe` guard. Two loops over one grid may use separate column counters.
Locals, stores, types
- A global incremented as load/inc/store (not `inc [mem]`) is volatile.
- Repeated `int i = 0;` + `for (i = 0; ...)` fixed a three-register rotation (`for (; ...)` did not).
- Zero-initialized local vectors are stored in declaration order; `memset(&s, 0, sizeof s)` is one
  `rep stosd`, `= {0}` stores the first dword separately.
- Byte copies with both loads (second zero-extended) before the stores go through int locals.
- A 12-16 bytes larger frame with rounded float temps in memory: a `D3VECTOR4` local (`volatile float`
  reuses dead parameter slots instead). Writes through a `D3MATRIX *` cast of `&map->orgx` keep the
  reloads after stores.
- A real member array (`ch->contacts[i]`) folds the offset into each lea; a cast macro gets CSE'd into a
  register.
- Two address-taken `int x, z` locals with swapped stack slots: declaration order, names, scope did not
  help; only a dead `z = 0;` or storing z first flipped them (both add code). Unresolved (ch4, 3 functions).
Floating point
- `s = x < 0 ? -1.0f : 1.0f; return s * x * x;` keeps the constant loaded first.
- `(float)(d * 0.01f) * 0.5f` keeps two `fmul DWORD`; `d * 0.01f * 0.5f` folds into one QWORD multiply.
- Two square roots of sums stay separate when both reuse the same dx/dy/dz locals.

## From round 7 (workers ch7, evtc, d3fl4, d3if)
Control flow and returns
- One `return a;` at the end (not an early return in the if) lets a variable live in two registers
  (returned on one path, call result on the other). `if (!p) error(); else p->x = y; return p;` returns p
  from esi on both paths; an early `return p` in the error branch becomes `xor eax,eax`.
- Callee-saved pushes: one final return plus early `return 0` (or an if/else-if strcmp chain) delays the
  pushes past the argument check; separate `if (...) { ...; return 0; }` blocks push at entry.
  `if (a = list) {...}` pushes at entry, `a = list; if (a)` inside the branch.
- The original's fall-through block is the `if` body: if it handles the flipped case, test `x & 1`, not
  `!(x & 1)`. An out-of-line block is the `else` of the reversed test.
- Tails that appear twice in the original are written twice. A call VC6 tail-merges between two branches:
  write it at the end of each branch (the first copy survives).
- Two failure tests reaching one sprintf: `for (...) {...} if (!replaced) sprintf(...)` (jump threading).
Integers and bytes
- `if (x & 0x10) return 1; return 0;` gives `xor edx,edx; mov dl,[m]; shr; and`; every expression form
  (`>>`/`&`, bitfields, `!= 0`, ternary, inline helper) gives the shorter `mov al,[m]`.
- `unsigned char` compared `cmp BYTE PTR [x],1; jae` is `x < 1`; `!x`, `== 0`, `<= 0` give `test`.
  `d3_Busy < 1` with zero in ebx is `d3_Busy <= 0` (`cmp [mem],ebx; jg`).
- A byte flag set/tested in a loop keeps 0x80 in bl only for `unsigned char` (signed: `test; js`).
- `memset(&f, 0, sizeof f); f.bit = 0;` keeps the byte read-modify-write; `*(int *)&f = 0` folds it.
  A word bitfield `&= ~8` is narrowed to `and BYTE PTR`.
- `min(0xff, iv)` gives `jle`, `min(iv, 0xff)` `jl`. `k << 16 | j` and `k * 0x10000 | j` allocate
  registers differently for the whole function.
- `if (!t->a) s = m->lod ? m->lod : 1; else s = t->a;` reproduces a byte test with a threaded `mov ebp,1`.
Stack and locals
- Frame bigger than the used locals: look for an array a callee fills (`CHARACTER *others[32]` for
  ch_FindNear) or a `D3VECTOR4` local (16-byte slot); dummy arrays get dropped.
- Recount parameter offsets after each push: a missed third parameter showed as a pointer held in edi.
- A counter zeroed before an unrelated call: `i = 0;` before the call, then `for (; i < n; ...)`.
- `angle = f(); g((int)(angle * k));` merges the argument pops into one `add esp`; `g(f() ...)` does not.
- A masked copy of a coordinate held elsewhere is a new variable (`ix = x & ~(s - 1)`).
- `*py = ...` without a local y lets VC6 forward the stored value; with a local it reloads.
Floating point
- Pass unsigned ints straight into a macro that uses them three times; a float local first spills (`fst`)
  and reorders the axes (~1000 asm lines in d3fl_Draw).
- `t = c ? x : 1.0f - x` keeps x on the FPU; `t = x; if (!c) t = 1 - t;` stores and reloads.
- `!(t > 0.0f)` is `and eax,0x4100; je`; `!(t < 1.0f)` is `test ah,5; jnp`. Float literals in compares give
  `fcom DWORD`, double literals QWORD.
- `f = (float)sqrt(..); dist = f;` gives fsqrt, pop the dead values, then the store. `(float)sqrt(..) > x`
  pops before the compare.
- Named float temps (`t4 = m21 * 0`) stop VC6 factoring `a*0 + b*0`; they land in dead parameter slots.
  `float one = -1.0f; x * one` gives `fmul 1.0; fsubr` (+1.0f is folded away).
- A float global assigned 0 or 200.0f is stored as an immediate only if declared `float`.
Arrays and globals
- Index loop over a global struct array: countdown `dec ecx; jne` based at the second member; pointer
  loop `p < arr + n`: `jb` with a different base. `((T *)base)[i].f` folds base+offset into the
  displacement; a pointer local adds the base first; repeat the macro to keep reloads after asm blocks.
- D3_COPY / D3_COPY_TO / D3_COPY3 are told apart by their stack temp pattern.
Tools
- asmdiff shows a bogus -/+ pair for an instruction with two relocations; trust `uv run asmdiff --bytes`/`check`.
- d3if wrote statement-move beam search helpers (copied to build/helpers/ in the main checkout, uncommitted): one
  hill climb took d3if_SelectScene from 101 to 50 differing bytes.

## From round 8 (workers ocmd, ocmd2, sndreg, d3if2)
Control flow and returns
- Shrink-wrapped pushes: `if (guard) return 0; s = f(); if (s) {...} else g(); return s;` pushes the
  callee-saved registers after the guard; a `return s;` inside the success branch pushes them at entry.
- `char ret = 0; if (..) {..; ret = 1;} return ret;` gives the early `xor al,al`; `mov al,imm; ret` means a
  char return type.
- A two-case switch is `dec eax; je; dec eax; jne`. `if (on) Acquire else Unacquire` picks the fall-through.
Bytes, flags and stores
- Byte fields of one flags struct are still order-sensitive: `f.b1 = x ? 0 : 8; f.b8 = 1; f.b1 |= 0x10;`
  gives `setne; dec; and 8; or 0x10`; the folded spellings give `sete; lea [ecx*8+0x10]`.
- `*(int *)&f = 0; f.b[0] = 0x7f; f.b[2] |= 3;` keeps the read-modify-write; `|=` before the store folds.
- Ghidra's `(BYTE)(a ^ b) ^ b` is `((a ^ b) & 0xff) ^ b` on unsigned ints; plain bitfield assignment gives
  the `(v<<n ^ old) & mask ^ old` sequence.
- `to->last->next = obj; obj->prev = to->last;` reloads; the original reads `to->last` into a local once.
- `if (!mem_IsValid(*h))` matched only with a named local `em = *h`.
Floating point
- int-to-float member stores: convert into float locals first, then store, for the "convert all, then store"
  shape (compute order matters).
- `(float)sqrt(d) < r` gives `fcomp mem; test ah,5`; without the cast `fcompp; and eax,0x4100`.
- Named float locals on the FPU stack pop at block end; merged temps/parameters pop at last use (`fmulp`).
  A parameter scaled once and used once is folded into its use; writing `size * sx` out at every use (or an
  aggregate initializer) reproduces the early multiply.
- `min/max` clamps inline in the call argument matched; a float local changed the compares.
Pointers and arrays
- `p = buf + wsprintf(buf, ..)` keeps p in eax. Indexing `b[i].x` bases the pointer at the middle field;
  walking `b++` bases it at the last.
- An uninitialized local flags struct may sit in a dead parameter slot.

## From round 9 (workers dinput, d3if3, d3o, d3o2)
Control flow
- `while (1)` is not rotated, `for (;;)` is.
- `if (d & 0x80) {..} if ((d & 0x80) == 0) {..}` without local or else keeps `and edi,0x80` and separate
  calls; a local or an `else` merges the tails and hoists 0x80 into a register.
- `T *obj = 0; if (ver >= X) {...} return obj;` keeps the pushes at entry with the early exit jumping into
  the shared epilogue; an early `return 0` shrink-wraps them.
- Callback with a shared `p == s` tail: `if (name) { two arms } else if (p == s) found = s;`, name arm first.
- `type = obj->type; switch (type)` right after a call loads the type early; `switch (obj->type)` late.
  Switch blocks are laid out in source case order.
- `int now = tb_Calls;` before an early return reproduces a hoisted volatile load.
- A parameter slot read on a NULL path (`mov edi,[esp+0x20]`) is an uninitialized local.
Floating point
- A 4-byte colour argument pushed before the FPU work of the other arguments: the parameter is a by-value
  4-byte struct (not `*(unsigned long *)&col`).
- Lerp with a frame 0x10 bigger and the third difference spilled: a D3VECTOR4 difference local.
- `float t = *time;` before `(unsigned)t` and `t - key` keeps t on the FPU (`fld st(0)`).
- `(int)(k * C * data)` gives `fild; fimul; fmul`; `fild; fmul const; fimul` needs
  `(int)((float)(k * C) * (int)data)`.
- `d = min(1.0f, max(-1.0f, d))` gives the triple test; `mov edx,[d]; mov [d],edx` is a second float local
  sharing d's slot.
- `expr > 0.0f` compares against a DWORD constant only when the whole sum is float.
- Product order in `(int)(a * u + b * t)` changed only on tie-breaks (dead `u = 0;`, pointer locals,
  parentheses), never on operand swaps.
Registers, slots and loops
- `l->k[key].pos[i]` in the loop (not pointer locals) gives a byte-offset counter based at the middle field.
- An unrelated earlier loop's index name changed `[iv+light]` vs `[light+iv]` (rank via variable order).
- Locals declared inside a block take the slots nearest ebp and share the D3_ZERO temps; a char used in one
  block can take a dead parameter slot.
- Initialized locals are stored in declaration order. A struct field copy with the zero store last reserves
  the zero register; the store itself is still hoisted.
- Deltas first, then write-backs (`S.x += pt.x - L.x` pairs) fixed an ordering difference.
- VC6's global allocator may promote a hot 16-bit global into bx and displace a hoisted constant 0 (unsolved,
  di_UpdateButtons).
Small patterns
- `isdigit(p[1])` on plain char gives movsx; `unsigned char > 0` gives `test; jbe`; a signed-char sign test
  `test al,al; jns` is `flag & 0x80` (`flag < 0` gives cmp/jge).
- `x |= (x & 1) << 1` on a 2-bit bitfield: `and 4; add; or`. Inline strcpy is a byte loop, strcat `repnz scasb`.
- Member-to-member vector copy field by field: loads first, then stores; struct assignment copies via pointer.
- Inlined `strcmp` vs a `_strcmpi` call: the same source with the compare swapped.
TU boundaries
- A function defined earlier in the same file can change a caller's code: rd_object matched alone but swapped
  two address operands once d3_SceneVersion (which it calls) was defined above it. A regression on joining
  files is evidence of a TU boundary between them.

## From round 10 (workers d3io, d3io2, d3io3, vfs3)
Return shape
- Single exit is the vfs author's style: memory stores to `ret` before each epilogue, `mov eax,edi` on paths
  that look like constant returns, `or eax,-1` at entry. Write `ret = x;` in every branch, one `return ret;`.
- Several `return 1` branches plus one shared `return 0` at the end (d3io); a tested-then-returned failure
  value needs two variables: `t = f(x); if (!t && (t = f(y), !t)) { ret = ...; return ret; } return t;`.
- A conditional assigned to a fresh local gives `mov eax,2; sub eax,ecx; jmp; mov eax,ecx`.
Registers, slots and loops
- Locals read through `bio_rd_i(f, &x)` stay in memory; the original reuses them as loop counters. A separate
  register counter steals the file pointer's register (d3io LoadScene, d3io2 LoadTexSet).
- A pointer parameter copied into a local (`VFSDIR *p = d; if (!p) return 0;`) gives `mov eax,[esp+4];
  test eax,eax; push ebx; mov ebx,eax`, and the null path returns eax without xor.
- A zero kept in a variable (`int z = 0; ... <= z`) gives `cmp reg,reg` and a zero register from entry.
- A dead slot `and eax,0xff; mov [slot],eax` is `int ok = 0; if (path) ok = f(); if (!ok)`.
- `v = m->verts; v0 = v;` vs `v0 = m->verts; v = v0;` choose different loop pointer bases.
- Reusing a variable for an unrelated role moves stack slots; a loop present in both arms of an if/else in
  the asm was written twice in the source.
- Frame gaps are again enregistered struct members (D3VECTOR4 read buffers / min-max locals).
Small patterns
- `char buf[N] = "";` at function scope is zero-filled at entry, in an inner block where that block starts.
- `mov ecx,[g]; and ecx,0x7f; cmp cl,1` is an int global with `(g & 0x7f) == 1`.
- Right-to-left argument evaluation: `line = ReadLine(f); atof(strtok(line, " "))` needs the local.
- Chained `sx = sy = sz = 0` reverses the FPU store order; separate statements keep it.
- `len = strlen(s) + 1; d->n = 0;` stores the zero left by `repnz scasb`; the store first uses the zero reg.
- `p->keys[n]` and `(*keys)[n]` schedule the pointer load differently.
- A float temp copy is an integer mov when a register is free, fld/fstp in a register-starved loop.

## From round 11 (workers agi2, anim, mesh, d3x)
Stack slots and registers
- One memory loop counter used by two unrelated loops: split it into two variables (moved one slot into place
  and fixed seven others).
- `a->n = n; ... mem_Alloc(a->n * sz)` forwards the stored register; using `n` again reloads its slot.
- A flag the original keeps in memory is a `volatile` local; repeated flag-byte tests in a loop become hoisted
  boolean temps in dead parameter slots only when written as char locals at the top (`char keep = !(fl & 1)`).
- 4/16-byte frame gaps were D3VECTOR4 locals (a scalar inside an accumulator's slot was its `.y`).
- fistp-style helper `(float f, int *dst)` put the temps in the original slots; `(int *, float)` swapped two.
- Callee-saved pushes move after a null test only with `if (!p) return 1;` plus one final return; any extra
  `return` inside a branch puts them back at entry.
- `push 0xff` needs an `unsigned char` parameter; a `char` gives `push -1`.
Loops
- `cnt = n->n; for (i = 0, p = n->v; i < cnt; i++, p++)` gives the countdown loop; setting p before swaps regs.
- `for (i = nv; i; i--, p++)` inside `if (nv > 0)` keeps the pointer load after the guard.
- A hoisted local base plus index (`p[i].v[0]`, `k = info->keys` before each loop) gives the original's biased
  strength-reduction base and integer float copies; `info->keys[i]` directly gives fld/fstp copies.
- `i++, vt++` order matters; index-form `strlen(d->mats[i])` gives two induction pointers.
Floating point
- Min/max: VC6 keeps the ternary's operand order. `mn = p->x > mn ? mn : p->x` gives `fld p; fcomp mn;
  and eax,0x4100; je`; `p->x < mx ? mx : p->x` gives `test ah,5; jnp`; `!(mx > p->z) ? p->z : mx` with mx
  on the FPU gives `fld st(1); fcompp`. `m = x; if (!(m > y)) m = y;` gives `and eax,0x4100; je`, the `<=`
  form `test ah,0x41; jp`. `if (v > b)` / `if (v < b)` choose the compare form for max / min.
- `x = min(255.0f, e)` keeps e on the FPU (`fld 255; fcomp st(1)`).
- `(w * 255.0f) / v` keeps fmul before fdiv (without parentheses: `255 / v * w`); `(float)(1.0f / x) * s`
  keeps `fdivr 1.0` (without the cast it folds to `s / x`); `(float)(a * c) * b` keeps the constant first.
- `fld k; fmul r` needed k as a new variable first appearing after r.
- `c.r = c.g = c.b = c.a = v` stores a, b, g, r.
Small patterns
- A byte shifted after `bio_rd_c` must be `unsigned char` (cast plain-char struct fields).
- Two explicit recursive calls (`!n` arm first), not a ternary argument.
- A bit copy between bytes is a struct of eight 1-bit fields (`a.b2 = b.b4`).
- `D3TEXFLAGS f = {0}; f.b[0] = 0xff;` gives a dword zero store plus a byte store to the same slot.
- `(char *)t - (char *)base >> 7` gives `sub; sar`.

## From round 12 (workers vec, vec2, geo, link; link conventions in notes/link.md)
FPU operand order
- `m[k]` memory-operand order does not react to textual swaps or global flips (rank-based). For a dot product's
  load order, brute-force every subset of the six operands in parentheses; term orders alone change nothing.
- A sum of products gets the original association only split up: `r = A+B; r += C; r += D;` (d3_Hermite).
- A cross product dotted with a memory vector keeps the three differences on the FPU only as a `float c[3]`.
- `expr > 0.0f` gives `fcomp [0.0]`; `0.0f < expr` does not.
- `acos(x) - 6.2831855f` without a `(float)` cast gives the QWORD `fsub` constant.
- `sin` / `cos` / `atan2` from math.h compile to inline `fsin` / `fcos` / `fpatan` (no asm helpers needed).
- `g = min(g, z)` gives the fld/fstp copy with `and eax,0x4100; je`; a plain `if` copies with integer moves.
- A scale consumed by the last `fmul [mem]` is a float declared inside the if block.
- 4x4 inverse: write the determinant and each element out in full; named cofactor temps change the shape.
Locals and frames
- A frame 4 bytes short, with a colour copied by integer moves and only .y/.z in memory: a D3VECTOR4 local
  filled field by field, not a D3VECTOR struct assignment. `D3VECTOR4 u, v` locals gave the face normal's frame.
- Plain `m->_31` member access reproduces a pointer CSE in edi / ebx; pointer locals made it worse.
- Member access vs the same field through a `(char *)` cast macro changes register ranks (d3_Unk453110).
- Reloading `k = info->keys` inside each branch fixed a flag test's register and a loop pointer bias.
- A local named `ne` cannot be used inside `__asm` (asm keyword).
Statement order and control flow
- Matrix stores per row `_11, _12, _13, _14=0` matched; identity = `D3_ZERO_AT(m, 64)` then 1.0 to _44.._11.
- A transpose as `dst[i*4+j] = src[j*4+i]` on float pointers matched; pointer walks swap eax / ecx.
- A search loop with the return inside the body: `for (...) if (hit) goto found; return; found:`.
- `if (A) {T; redo = 1;} if (redo) {...} else {...}` kept the block order an else-if with goto did not.
Link (round 12 probe; `uv run build` masks all of these, uv run link catches them)
- Two globals in a commutative expression can be swapped with no byte change; one `/* 0xADDR */` comment per
  declaration, giving the address of the symbol's first byte.
- String literals and float constants must have the original text and bits (D3DCHECK text, `__FILE__` via
  `#line N "D:\develop\..."`); statics / globals in a TU's .bss keep declaration order only with `= 0`.

## From round 13 (workers par, par2, d3io4, lfix; link in notes/link.md)
FPU and expressions
- min()/max() from windows.h reproduce mixed fcomp / fcompp forms that hand-written ternaries do not
  (d3q_GetBounds); an inline `max(0.0f, min(255.0f, expr))` clamp stays on the FPU and lets branches tail-merge.
- A conditional float copy from a member at offset 0 (`[reg]`, no displacement) compiles to integer moves; for
  the original's fld/fstp write the arm as `(p->x * 1.0f)` or `(p->x + 0.0f)` (unary + does nothing).
- A dead store (`rz = 0.0f;` at block top) is a free rank tie-break for the load order of a product.
- A block-local `float f = rand-expr;` stays on the FPU and is popped after ftol (the original's `fstp st(0)`).
- An `__inline` helper with float parameters gives loads-first-then-stores for a 3-float copy.
- For a sum of globals the source order is the load order.
Registers, stack and stores
- The base / index order in `[r1+r2]` did not react to pointer arithmetic, `k[arr]`, byte sums or parentheses
  (par_Create, d3_LoadBinaryAnimStream); what decides it is the loop form: see round 14 (pfix).
- Spilled int locals' slot order did not react to declaration order, names, block scope or initialisation.
- `volatile` parameters reproduce an original that reloads them from the stack (a workaround, d3q_Build).
- `a = b = c = k` stores right to left; two independent stores come out in reverse source order (d3_Render).
- `if (!a || !b || !c) return 1;` plus one final return shrink-wraps the callee-saved push.
- `x <= 1` lets VC6 reuse a register that already holds 1. `return !(a && b);` returns full eax; an early
  `return 1` from a char function is `mov al,1`.
- Explicit pointer locals (`v1 = v + 1`) stop VC6 from merging byte read-modify-writes through one base.
Link
- "Matched" names can still sit on swapped struct fields (D3DCARDINFO, DIMOUSEST): the link gate finds them.
- Ghidra misses callbacks reached only through function pointers; an 8 KB "function" was four (par).

## From round 14 (workers q1, q2, q3, pfix)
Registers, stack and stores
- `[base+index+disp]`: an index VC6 turns into its own induction variable (`a[i]`, `i * sizeof`) is emitted
  as the base; an explicit byte-offset variable (`(char *)p + j`, `j += 0x54`) is the index. Zeroing the
  offset after the loop guard needs a guarded do-while (`if (i < n) { j = 0; do {...} while (i < n); }`).
- Which local gets a dead parameter slot or a frame slot follows reference counts after optimisation: a real
  extra use moves a variable (`p = res[j]; q = res[j + 1];` locals for repeated loads), dead or folded uses
  (`if (0)`, `* 1.0f`) do not. Four scalar floats instead of `float[4]` moved a 16-byte block in the frame.
- Two byte bitfields through one base (`t[0]`, `t[1]`): VC6 hoists the second load above the first store; a
  `t1 = t + 1` pointer, propagated away at no code cost, keeps the source order.
- A 16-byte local where x / y stay on the FPU and z goes through memory: `struct { float x, y; volatile float
  z; float w; } t`.
- One statement per operation (`t.x = t.x - a.x;` for x / y / z, then `- b.x` for all three) reproduces a
  store / reload pattern that `(t.x - a.x) - b.x` does not.
- A local for a repeated pointer load (`D3ANIMDATA *d = s->data;`) fixed a register assignment; an index form
  `bytes[i * 3]` / `f->normals[i*3+k]` reproduced loop registers that a separate counter or a hoisted
  `p = normals + i*3` did not.
- `memset(arr, 0, 16); count = 0;` gives `xor` stores for the array and an immediate for the counter;
  separate `= 0` stores keep a zero register live for the whole function.
- Stores into a by-value flags struct after `memset(&f, 0, 4)` come out in source order (`f.b[0] = 0x7f;`
  before `f.b[2] |= 2;`).
Branches and calls
- The out-of-line block is the `else` of the `if`: swap the branches to move it (458ce0, 458650).
- `!(c & 0x80)` on an unsigned char is `test; js`; `(signed char)c < 0` is `jl`. `unsigned x > 0` is
  `test; jbe`. `if (A > B) {...} else {...}` gave `and eax,0x4100; jne`, the flipped `<=` `test ah,0x41; jp`.
- A callee that returns in al and whose result the caller passes straight on must be declared `unsigned int`;
  with `unsigned char` VC6 adds `and eax,0xff`.
- Constant arguments pushed after an inner call: make the inner call its own statement.
- `for (it = first; it; prev = it, it = it->next) if (it->obj == obj) break; if (!it) return 1;` gives the
  unrotated search loop. `if (n == 0) return 0;` before the body keeps the early return with its own epilogue.
- For `depth + 1` from a volatile parameter, call arguments are read right to left; a block local
  `d = depth + 1` restores the original load order.
FPU
- `h = a - b; ...; h *= 0.5f;` as separate statements lets two half extents be computed in parallel before a
  call (fsub, fsub, fxch, fmul).
- `(int)(v * 65536.0f)` with the literal gives `fld v; fmul const`; a global holding 65536.0f loads first.
- `h * cy + (oy + 1.0f)` reproduces a hoisted `oy + 1.0`. `min(acc, v)` / `max(acc, v)`, accumulator first,
  give the original compare forms.
- `PAR_RND()` vs `(float)rand() / RAND_MAX` moves an unrelated lea / fstp around the call: try per component.
- Operand order inside dot products is rank-based: 3072 parenthesization / permutation variants per row did
  not move d3_GetBoxVerts' last bytes.
Inline asm
- Fixed-point helpers are `__inline` functions with `__asm` bodies (`imul edx; shrd eax,edx,N`; a 64/32
  divide `mov edx,eax; sar edx,16; shl eax,16; idiv ebx`): no C spelling gives them, `__int64` calls
  `__allmul` / `__alldiv`. Inlined, their arguments go through stack temps (an expression gets a temp, a
  global is addressed directly). Span fills are `rep stosb` / `rep stosw` in `__asm` fed from a `char *` local.
- A reverse walk over x / y pairs as `p[4 - i*2]` / `p[5 - i*2]` keeps two pointer temps that `q -= 2` merges.

## From round 15 (workers z1, z2, z3, shfix)
Loads, operands and globals
- A global array indexed again in every statement (`d2_Pics[idx].x = ..`) is reloaded after each store / call in
  the original; a local `p = &d2_Pics[idx]` keeps it in a register. Write the index out per use (z1, z2).
- Integer `cmp` operand order follows the source: `x > W`, not `W < x`. `p->x <= 0` gives `cmp 1; jge`, `< 1` does not.
- Unsolved (three d2 functions): which member of `w->x + w->w` / `o->w - p0->w` is loaded first; operand order,
  parentheses, casts, temps, dead references and a nested x/y/w/h struct had no effect.
- A byte local compared with an int global spilled; reading the byte twice through the struct pointer matched.
- A callee with an `unsigned char` last parameter gets a global byte as `mov dl,[g]; push edx` (no extension).
Registers, slots and frames
- Which variable is the modified parameter decides register roles: `a = idx; idx = base[idx]; shift = a - idx;`.
- A value tested in a register but re-read from its stack slot at the last use: `volatile int hv; hv = hi = g;`.
- One counter or one tx/ty pair reused for unrelated loops / branches shares their stack slots: separate
  variables put ~3000 B of slot differences into place (z3). Three `unsigned char` colour locals took three slots,
  `unsigned char rgb[3]` gave the original frame.
- x/y parameters stored into shorts are `int` parameters; `short` moved a dead parameter slot.
- Repeating `in->mult * in->value` at each use (no named local) kept it out of the loop counter's slot.
Control flow and calls
- Ghidra-shaped do-while loops lose to plain `for` loops; a `for` whose counter is unused in the body becomes the
  `dec; jne` countdown (shfix).
- `if ((g = expr) > 0)` with no local gives a global store followed by a register test.
- `f(.., dd_RGB(r, g, b), ..)` pops the inner call's arguments at once; `c = dd_RGB(..); f(.., c, ..)` merges them.
- `fopen(path ? path : def, ..)`: two calls in an if/else matched, the ternary did not.
- Inline error block with a shared `or eax,-1` tail: `if (n >= MAX) { err } else { ...; return obj; } return -1;`.
- A branch ending in a plain `mov ecx,eax` points to a separate variable assigned in each branch.
- A hoisted `neg w` used as a loop step is `j * w` strength-reduced, not `i -= w`.
Inline asm and FPU
- An asm block with four stack temps was an `__inline` function, not a macro; its parameters are the temps,
  evaluated right to left.
- `if (g >= z) g = z;` gives `fld g; fcomp st(1); test ah,5; jnp` with an fld/fstp copy; `z <= g` gives fcom / jp.
- 16-byte vector locals with only some members `volatile` reproduce "x, y on the FPU, z through memory".
Workflow
- Mask esp offsets before trying variants on a big function: z3's 4.7 KB function had 6 real instruction
  differences; everything else was stack slots.
- Brute-force the init-store order with a generator script (fixed d2_OpenInputIcon).

## From round 16 (workers d2j, d2b, d2c, d2d)
Registers and ranks
- A dead constant store at the function top (`base = 0;`) is a rank tie-break that flipped `[ecx+ebx]` to the
  original `[ebx+ecx]` (d2_Exit); dead loads, and dead stores to other locals, did nothing in d2_Unk46bf60.
- Five `= 0` member stores keep a zero register live for the whole function; `memset(s, 0, sizeof *s)` gives
  `xor eax,eax` plus the stores at the site (d2_RestoreShapes).
- Register roles follow assignment order: `oldx = w->x; oldy = w->y;` vs the reverse swaps ebx / ebp.
- A size local computed before a block of stores moved registers; computing it after the stores, or inline, matched.
- Two field reads from call results through one pointer local (`part = f(p, 4); fy = part->h;`) fixed a
  register rotation over ~1 KB (d2_Unk46d3d0).
- Unsolved (d2_DrawInput, d2_Unk46aa00, d2_Unk46bf60): `w->x` / `w->w` load order; ~60 variants (operand order,
  parentheses, dead stores / loads, `d2_Objs[id].` access, casts, declaration order) did not move it.
Types and calls
- K&R definitions in the d2 TU: `int f(x, y, pic) int x; int y; int pic; {..}` with a header prototype using
  `short` parameters reproduces the thunk `mov ecx,[esp+8]; mov edx,[esp+4]; mov [esp+8],ecx; mov [esp+4],edx;
  jmp` (0x46cd80). New-style short parameters give movsx; matching prototypes a bare jmp.
- A pass-through wrapper whose stack slots are reloaded and restored before the `jmp`: `unsigned char`
  parameters in wrapper and callee. A `short` prototype parameter passes a short expression without extension;
  an `unsigned short` glyph index from an `unsigned char` local gives `xor dx,dx; mov dl,bl`.
- `test al,al; jns` comes from `x & 0x80`; `(char)x < 0` gives `jge`. `ret = global = -1;` gives `or eax,eax`
  after the store. A 17-byte record array (`shl ecx,4; [ecx+eax+base]`) needs `#pragma pack(1)`.
- `movsx edx,BYTE [o+0x1c0]; and edx,0x10`: solved in round 17, see there (d2_Unk46c6c0, d2_Unk46e460).
Control flow and loops
- A loop from a computed index needs its own start: `for (i = top; ..)` matched, `i = expr; for (; ..)` not.
- `f(x, i == sel)` gives `sete`; two calls with `push 0` / `push 1` are `if (i != sel) f(x, 0); else f(x, 1);`.
- Varargs stored by index (`objs[j] = va_arg(ap, int)`) matched; a pointer with a countdown did not.
- A search loop with `global = o->f; break;` and a final `return global;` (global reloaded) differs from an
  early `return o->f;`.
- A loop over a global struct array compiled to a pointer at an inner field with signed `jl` was an index loop;
  a real pointer loop gives `jb`. `for (unsigned i ..; i < strlen(s); ..)` gives `repnz scasb` per iteration + `jb`.
- "Last used slot + 1, else 0": `for (i = 0x2ff; i >= 0; i--) if (a[i]) { i++; if (i >= 0) goto found; break; }
  i = 0; found:` (d2_Unk46c9f0). `int *r = &x->refs; if (*r > 0) (*r)--; else err(msg);` gives a tail jmp.
- Switch case order in the source = jump-table block order; duplicated calls in if/else get their tails merged.

## From round 17 (workers d2k, d2e, d2f, d2g)
Registers and order
- `movsx r,BYTE [m]; and r,imm; shr r,n` comes from `if ((char)x & imm) v = 1; else v = 0;` (or `return 1;
  return 0;`): VC6 turns the if/else into and/shr after the sign extension. Casts, shifts, ternaries, bitfields,
  volatile and inline functions all narrow to a byte op. A dword load then `movsx r,al` means the flags sit in an
  int local first (d2k).
- Functions defined earlier in the same file change a caller's allocation: VC6 knows which globals they write,
  so a global can stay in a register across the call. A test copy of d2_FindPic above d2_Unk46fba0 cut its diff
  772 -> 700 asm lines: judge near-misses only in the joined TU (d2e).
- Jump threading: `sel = cur` lets VC6 skip the compare when the loop is skipped; reload the value instead (d2k).
- Before declaring a "new" global, grep its address in include/: 0x6333cc was d3.h's `volatile d3_Time`, and the
  volatile fixed the reload order (d2k).
- Some d2 functions re-read a global array after every store: write `d2_Objs[obj].f = ..` per statement, not
  through a cached pointer; one function can mix both forms. `d2_Inputs[k].f` through a cast-array macro gives
  `[reg+abs]` (d2g, d2e).
- Ghidra's `iVar = win * 0x3c4` is a byte-offset int local (`[esi+const]`); `&d2_Wins[win]` gives
  `lea esi,[ecx*4+X]`. Both occur, per function (d2f).
- `if (y + o.h > w->m) w->m = y + o.h;` with no temp matched; a temp did not (d2g).
- A parameter used only late is loaded late and its stack home reused for locals / fild temps: do not copy it to
  a local at the top (d2f). Local declaration order had no effect on slots in d2_Unk46fba0 (16 orders) (d2e).
- `k = d2_FindPic(..)` assigned first merges the argument pops of the outer call (d2e).
- Two stored `pos++` (`inc; mov [pos]` twice) came from one `pos++` before and one after an inner block that
  holds an initialized local array (d2e).
Types and calls
- A prototype seen before a definition decides how calls inside and after it pass arguments; a redeclaration just
  before the definition lets earlier callers keep another prototype (C4028 is only a warning). d2_Unk46cda0:
  int for d2_CenterObj, short for its own recursion and 0x46f840 (d2k).
- A `short x, short y` prototype makes VC6 narrow the argument arithmetic to 16 bit (d2_Unk4731d0); an
  unprototyped `int f();` declaration passes ints (K&R callers) (d2f).
- Dead parameters reused as out-pointers: `f(w, &x, &win)` (d2g).
Control flow
- A first-placed `return -1` is `if (x == 0) return -1;` before the body. An early `return 0` plus
  `if (!active) return 0;` gives inline `xor eax,eax; pop; ret` copies; one `if (active) {..return 1;} return 0;`
  shares the tail (d2g, d2f).
- `f(x, c ? 1 : 0)` gives setcc; `if (c) f(x, 0); else f(x, 1);` the push 0 / jmp / push 1 shape (d2f).
- `if (w->font != 0) font = w->font; else font = def;` matched; the swapped branches did not (d2g).
- Strength reduction: a tile loop written `blit(xx + i * tw, yy + j * th)` becomes the original loop (d2e).
Floating point
- `fild; fidiv; fadd dword` is `(int)((float)a / b + 0.5f)`; a double gives `fadd qword` (d2f).
Unsolved
- d2_Unk472fd0: i / j swapped between esi / ecx (~30 variants, 120 declaration orders); d2_Unk471a90: two stack
  slots swapped (120 orders); d2_Unk472760: `add [mem],ecx` needs a pointer local, which breaks the layout;
  d2_Unk46d040: x / y between esi / edi; d2_CenterObj: obj kept in ebx, pushed late (K&R short: 116 -> 111).

## From round 18 (workers d2l, d2m, d2n, d2o)
- A zero register held through a whole function can come from a run of `g = 0` stores to adjacent globals: the
  original is one `memset(&g0, 0, 16)` (stores from the memset's eax). Cutting chunks out of a standalone copy with
  `uv run try` finds such causes fast (d2_Open).
- Stack placement of local arrays does not follow declaration order or names (720 permutations, no change); it
  follows reference counts, and moving an array into the block that uses it changes it (d2_Unk46fba0).
- `f(.., i == 0 ? i : sz, ..)` gives a mask / temp; `if (i == 0) f(0, i, ..); else f(0, sz, ..);` matches (VC6
  merges the two calls). An out-of-line `xor eax,eax` return at the end: nest the body in
  `if (x != 0) { ...; return r; } return 0;` instead of an early return.
- `abs()` (stdlib.h) is an intrinsic (cdq / xor / sub); a ternary is not.
- A local pointer assigned in both arms of an if (`w = &d2_Wins[..]`) keeps the `*0x3c4` per branch; repeated
  `d2_Objs[obj].f = ..` reloads the base pointer per store, a local `D2OBJ *o` does not.
- `!(fl & 0x80)` gives `test bl,bl; jns`, `(char)fl >= 0` gives `jge`. `(short)w * (short)a` vs the swapped order
  swaps the imul operands.
- Clamp with the else-assignment inline: `if (!(a > 0.0f)) a = 0.0f; else if (!(a < 1.0f)) a = 1.0f;`. A double
  literal compares with `fcomp QWORD`, a float literal with `fcomp DWORD`. `(float)(unsigned)x` is `fild QWORD` with
  a zero high dword (a zero kept in ebp for the whole function is a local set to 0 at the top).
- A global read twice in the original is read twice in the source: assign the local after the early-outs and
  `switch` on the global itself.
- A function defined with 5 parameters and called with 2 elsewhere in the TU: declare it unprototyped (`void f();`)
  before both (d2_Unk475cb0); same trick as d2_Unk472390.

## From round 19 (workers d2p, d2q, d2r, d2s, hv1, shfix2)
- TU probe: VC6 keeps a global in a register across a call to a function already compiled in the same file that
  writes no global (d2_Unk477a70 / 477ed0 cache d2_Unk6ed9dc across d2_Unk475bf0 once 475bf0 is defined above
  them; the original reloads it, so 475bf0 is in another TU). Defining a pure callee above a caller is a cheap
  boundary test; d2_Unk478320 the other way round only matches with d2_Unk476c90 defined above it (edx kept).
- A global the original re-reads everywhere must stay a global read: a local copy cost 539 bytes (d2_Unk477ed0).
- `x->f += call()` computes the lvalue address before the call. Storing `in->value = val - step*q` and re-reading
  it avoids the `neg; imul; add` form.
- Reading a struct field into a local before a branch schedules the load above another parameter's test; read it
  inline in each case. Three member stores then a member test follow source order only once the tested member is
  read into a local first (d2_Unk475cb0).
- `case 0: return 0;` keeps the `sub ecx,0` compare an empty `case 0:` drops; case bodies follow source order
  while the compare chain is sorted. A dense 0..3 switch is a jump table, so a compare chain there is an if/else.
- `if (ok) { ...; switch (...) { ... break; } return 1; } return 0;` defers `push ebx` (d2_Unk4799e0); a `return 1`
  per case puts all pushes at entry. Separate `return 1;` per case can also be right (d2_Unk47bd70): try both.
- `memset(&fx, 0, sizeof fx); fx.dwFillColor = 0; fx.dwSize = 100;` gives the DDBLTFX extra store.
- Rect field store order decides scheduling (brute-force it). Clip on the parameter itself
  (`if (x < s->left) x = s->left;`), not on a copy. Compare operand order follows the source: `h + y > s->bottom`
  gives `cmp reg,mem; jle`, `s->bottom < h + y` flips it.
- `(unsigned char)b >= 1` on an int parameter gives `mov al,[b]; cmp al,1; jb`; `(char)flags & 0x40` gives
  `mov bl,[..]; test bl,0x40`; a `char fl` with `fl & 0x80` gives `test al,al; jns`; `unsigned > 0` gives `jbe`.
- `volatile` on a global counter gives load / dec / store instead of `dec [mem]`.
- `3*w & 3` becomes `(-w) & 3` unless `w*3` is a shared subexpression (named local `w3`).
- An if/else-selected string argument with side effects is a ternary with a comma expression.
- Index loops end in a signed `jl` (also on a strength-reduced pointer), pointer loops in `jb`;
  `while (1) { if (i >= n) break; ...; i++; }` is the unrotated loop. `for (m = 0; m < *p; m++)` gives the
  `cmp mem,0; jbe` guard; an extra `if (*p != 0)` doubles it. A row pointer created only after the loop's entry
  check is strength reduction of `base[i * stride + j]`.
- `else if (A && B)` and nested ifs choose different duplicated tails; VC6 does not merge duplicated tail code
  the original has twice. Writing a member through another access form than its loads (index vs byte-offset
  macro) blocks CSE and gives `add [mem],reg` (d2_Unk472760).
- K&R int parameter copied into a narrower local, one variable per branch: changes which locals take the dead
  parameter slots (d2_Unk475730). Locals passed by address (`unsigned char r, g, b`) also land in dead parameter
  slots; a local whose address is passed must not double as a loop counter. A parameter slot holding a running
  value (a minimum) is the parameter reused.
- FPU (shfix2): `((float *)p)[i]` ranks differently from the member access and changed operand order where 3000+
  textual permutations had not; `(a * b) + ...` evaluates the first product first; a float the original stores
  and re-reads is a volatile local, a member loaded for every product a volatile read; `(float)a + b` with int b
  is `fild` + `fiadd`; an int add then a conversion from unsigned is `fild QWORD`.
- An index loop with `sl = &arr[k]` puts the induction pointer at the most-used member, a pointer loop at the
  struct start. A 16-byte frame hole only partly used is a D3VECTOR4 / `float[4]`.
- Counters reloaded after a `rep movs` record copy: counter and array are members of one global struct.


## From round 20 (workers d2t, d2u, f1, f2, f3, hv2)
- TU evidence from .data (d2t): each obj has one plain .data block of its initialised globals, then its string
  literals as COMDATs in function order; /O2 implies /GF, so identical strings are pooled across TUs. A new
  non-string block marks a new obj; a string sitting in an earlier obj than its only user means a removed
  function of that obj used it. `__LINE__` constants date a file (0x43f needs ~1100 lines of the same file
  before it). A callee defined above its caller is a two-way probe even for nonmatching callers (a big drop in
  differing bytes = same TU); "no change" proves nothing.
- Inlined memset: `or reg,-1` + four dword stores is `memset(p, 0xff, 16)` (hv2: 626 -> 172 diff lines);
  `memset(&arr[n], 0, 0x11)` is four dword + one byte store through one pointer (d2u, f1 16 bytes);
  `memset(&tab[i], 0, sizeof tab[i])` instead of separate `= 0` stores keeps a zero register from being hoisted
  over an earlier test (d2t).
- Callee parameter types decide the caller's loads: `char` / `unsigned char` gives `mov cl,[mem]; push ecx`
  with no `xor` before, `unsigned short` gives `mov bx,[mem]` (and `and eax,0xffff` at an int compare), `short`
  `mov dx; add edx,..; push edx`. Declare the callee's prototype narrow when the caller pushes a bare byte/word.
- Compare spelling (f1, f2, f3, hv2): operand order decides `cmp a,b / jl` vs `cmp b,a / jg`; `a >= b` vs `b <=
  a`, `!(v < m)` vs `v >= m`, `max(5, v / 4)` (jge) vs `max(v / 4, 5)` (jg) all differ. A `uv run try` list of
  equivalent spellings finds the right one in one run (0x4836a0: 125 -> 0 bytes). `(unsigned char)i > bank->n`
  gives the `xor dx,dx; mov dl` compare. Of a float-product compare, `!(A > B)` and `A <= B` evaluate and spill
  different sides first.
- Clamps: `min(max(0.0, expr), 1000.0)` (windows.h macros) reproduces double-constant compares with `fld
  st(0)` copies (f1); an int clamp as `m = ..; s->f = m; if (m >= lim) m = lim; s->f = m;` matched where a
  direct member store did not (f2).
- Named locals for intermediates take a callee-saved register and spill a loop counter: write the expression
  inline (f2); reload `a->frame` instead of caching it (d2u). Testing `arr[idx].id != 0xffff` before `c =
  &arr[idx]` fixed 47e560's registers (f1). One variable reused for an ftol result and a later delta shrinks
  the frame by 4 (f1). Two locals sharing a dead parameter slot are one source variable (f3).
- Loops: a strength-reduced search pointer compared with `jge` is an int-index loop (a pointer loop gives
  `jae`); `j = 0` stored before a `jne` on a found flag is `for (j = 0; !found && ...)` with no `if (!found)`
  around it; the base of a strength-reduced pointer follows the member the source references (hv2). `y + i`
  in a call is strength-reduced after the loop guard, a running `j = y` is loaded before it (d2t). `ok = 1;
  while (i < N) {...}` with a bottom test is `for (i = 0; ok && i < N; ..)` (f3).
- If/else layout: an if/else chain without `return` hoists a shared `push` (f1); `if (!f()) { if (g()) ..
  } else ..` puts the success block after the second call (f2).
- Inline asm (d2u, f1): hand-written drawers (`push edi, esi, eax, ebx, edx, ecx, ebp` after the prologue) are
  `__asm` bodies; the compiler adds push / pop of the ebx/esi/edi the asm names. A `jmp` with displacement 0 is
  an `_emit` sequence, one `_emit` per line (several on one line emit only the first). 0xcc padding after a
  function vs our nops cannot be reproduced: the TU ends there (0x47d210 / 0x47d710 are code-identical).
- `char tag[] = "SHAPBANK"; memcpy(bank, tag, 8)` gives the three loads at function start (d2u). A tick
  difference compared with 1 matches only with the previous-tick variable unsigned (d2u). `util_Random` result
  kept in an `int`; `(uint + ushort) % 3` is `xor edx,edx; div` (f2).

## From round 21 (workers ai, f4, f5, f6, f7, hv3)
- Tie-breaks in nonmatching functions also depend on what the headers declare (e.g. `<windows.h>` moved
  0x483a60 by 5 B), not only on the functions above. A join can move near-misses by a few bytes with no TU
  meaning; if bisecting by deleting functions gives non-monotonic results, it is noise (ai).
- A loop that reads a field into a local and also uses the field directly gets a different strength-reduced
  base than one that uses the field everywhere: using `ai_Entries[i].unk3c` everywhere took 0x48bc80 from
  3352 to 1414 B (ai). The base register of a strength-reduced table loop is the field referenced most often (f5).
- Two cast views of one global array are two induction variables: one view per loop, or the bias changes. A
  pointer induction variable with a constant start puts the bias on the field with the largest offset (f7).
- Index loops over a global array: `arr[i].field` directly (macro), no pointer local; unrotated shape
  `while (1) { if (i >= N) break; ...; i++; if (found) break; }` (f6). A loop comparing the biased induction
  register with a constant (`cmp esi,const`) is an index do-while `i = 0; do { c = &arr[i]; ... } while (++i < N)`
  (f7). A rotated `while (!found)` with an in-body `if (j >= n) break;` is written so, with `j = 0` before the
  guard (f7).
- VC6 C accepts anonymous structs / unions silently and without code change: one field can carry two names in a
  shared header (ai).
- A six-term `||` chain gives the same blocks as if/else, if/continue or negated; source branch order does not
  decide that layout (ai, f5 0x487410).
- Brute-forcing the statement order of a message-field block (`uv run try`, 720-5040 variants, 1-2 min) matched
  0x488740 / 0x4884b0: independent statement order sets scheduling and register roles (f6).
- `switch (r) { case 0: A; break; case 1: A; break; }` with duplicated bodies gives `sub eax,0; je; dec eax;
  jne` (bodies merged); `case 0: case 1:` gives a range test (f6).
- Byte parameter pushed as `xor ecx,ecx; mov cl,[m]; push ecx` / `and eax,0xff; push eax`: the callee takes
  `int` (f5, ai). A ushort passed to an int-prototyped callee adds `and eax,0xffff` and changes register roles
  (f6 0x4888c0).
- `fidiv` from a stack copy with a dead zero store: the divisor is `unsigned int` (f5). `and eax,0x4100; je` on
  a float is `!(x > c)`; `!(x <= 0)` gives `jp`, `x <= 0` `jnp` (f5, f7). `const > f` (constant first) gives
  `fld c; fcomp mem` (f6). `x*c*c` folds into one multiply; split it (f6). `if ((g = f()) < best)` gives
  `fld st(0); fcomp`, `g = f(); if (g < best)` gives `fcom` (hv3). `x = (float)(p->m - 2.0)` stores x before the
  next call's cleanup (hv3).
- `fl |= 0x40; store` reuses the register for a later `test cl,2`; `store (fl | 0x40)` gives `mov eax,ecx;
  or al,0x40` (f5). `volatile` on a byte struct read gave `lea eax,[..]; mov al,[eax]` (f6 0x489660).
- Clamps: min/max macros with `>> 1` (f5); `max(1, n)` / `if (n < 1) n = 1` give `jge`, `max(n, 1)` gives `jg`
  (hv3).
- Frame size: `char buf[1028]` costs 8 B more than `buf[1024]`; Ghidra's local size includes a saved slot (f7).
  Out-structs declared inside the one if-block that fills them share slots with arrays of other branches (hv3).
  Block-scoping a loop counter shrinks its live range so it stops spilling (f6). Two AIMSG locals stay separate.
- Several `= 0` stores plus `push 0` make VC keep a zero register for a whole loop and push the counter into
  memory (f7 0x489cb0, unsolved).
- Nested calls `f(g(h(1)->x)->y, byteglob)` push the byte global before the inner calls; temporaries put the
  inner calls first and merge the argument pops (hv3). `if (c) *out = 2; else *out = 0; return 1;` keeps separate
  return loads that two `return 1;` would merge (hv3).
- Linker-folded functions (ICF, e.g. a `xor eax,eax; ret` stub at 0x52a040): two names folded to one address give
  two `mov [..],imm` stores; one name assigned twice gives one `mov eax,imm` + register stores (hv3).
- Ghidra misses callback-only functions: hv3's two "heavy" ranges (9152 / 5552 B) were 30 and 20 functions
  (decision-table and action callbacks). Check heavy-queue entries with disasm before sizing them.

## From round 22 (workers ai2, g1, g2, g3, g4, hv4)
- Shared `return 0` epilogue: decide callbacks get the single `xor eax,eax` epilogue from nested ifs (or
  `if (a && b && ...) {...}`) plus one final `return 0;`; early-return guards copy the epilogue per site. Matched
  0x499660 / 0x49a020, 0x499c10 718 -> 35 B (hv4); a trailing `goto ok; ... ok: return 0x40;` merges success tails (g1).
- One final `return 1` after an if/else whose branches set the same field shrink-wraps a late `push ebp`; two
  returns in the branches push it at entry (g3, 0x493200).
- Statement or call in both arms of an if/else: VC6 merges the copies, but scheduling and push hoisting follow
  the duplicated form (argument pushes before a test = call written in each arm). Hoisting needs identical
  first instructions and one predecessor per arm; a shared call block (from `||`) never hoists (ai2).
- No statement order reproduces a store order: read the fields into locals first (ai2, 0x48a380 36 -> 9).
- Stack-slot tie-breaks move with identifier names (renaming four functions moved 0x483a60 38 -> 33 B) (ai2).
- `a < b` vs `b > a` sets cmp operand order and can shrink an asm diff a lot at the same byte count (ai2).
  Float flag tests by spelling: `x * 1.1f > d * 0.9f` (not `d * 0.9f < x * 1.1f`), `!(x >= 1.0f)` gives
  `and eax,0x100; je` while `x < 1.0f` gives `test ah,5; jp`; `test ah,5; jp` = `x < c` ok-branch, `test ah,0x41`
  = `x <= c`, `and eax,0x4100; je` = `x > c` (g1, g4, hv4). `if (10000.0 > d) d = 10000.0;` gives `fld; fcomp st(1)`.
- Clamps are nested standard min / max macros with the expression repeated: `x = min(max(0.0f, x), 1.0f)`,
  `max(min(base - d, 2), -2)`; the redundant third test stays; a named clamped local changes the layout (g4).
- Constant folding: parentheses keep both fmuls (`(d / x * 0.1f) * 32.0`); `(float)(x * -0.5f) + 0.25f` keeps
  fmul/fadd where VC6 otherwise folds to fsubr; `util_Random(n) + 10.0` is `fadd qword`, `10.0f` is `fadd dword`
  (g2, g4, hv4).
- Call order: `f(a) - f(b) + 10` evaluates b first; split into locals. To get `add eax,edi` (second result
  first), hold the first in a temp and write `second + first` (g1, g4).
- Dead parameter slots: a callee given the address of one of the function's parameters puts that variable in
  the dead slot (g1); a block-scoped out-variable `{ int t; f(&t); ... }` or `int neg = -1` takes parameter
  home slots (g3); an uninitialised `int v;` read on a switch default path loads the dead slot (g4).
- Two locals of one type in one struct reproduce the original slot order (g2, 0x48f2b0).
- `memset(a, 0, n); a[i].f = c;` gives `rep stos` plus constants stored between the argument pushes; an array
  initializer gives single zero stores (g2). `q = &arr[id]` for several fields gives `lea esi,[eax*4+base]`.
- A 64-bit argument built as a LARGE_INTEGER goes through ecx:eax across two calls; an `__int64` constant is
  two pushed immediates (hv4).
- Subtraction shape: `v - lim` used directly in a test and a store (CSE) gives `mov ecx,eax; sub ecx,edx`; a
  separate `d = v - lim` gives `sub eax,ecx; mov ecx,eax` (hv4). A named local for `&ai_Chars[x]` can change the
  whole function's register choice (hv4, 0x497af0).
- Search loops: the found-code inside the loop body (with return) gives the direct exit; `break` + `if (i >= n)`
  re-tests the counter (hv4).
- Switch cases with a shared tail but different call constants: write the call per case (VC6 cross-jumps);
  separate `return 1` per case (g3, 0x4937a0). `if (uchar > 0)` gives `test; jbe`, `!= 0` does not (g3).
- Unsolved: constant arguments loaded into registers before the pushes (`xor ecx,ecx; mov eax,0x7b4c4a0;
  push ...`, `mov ebx,0xe07c43b4; mov ebp,1` before a call) in the ai_Unk4a63f0 callers 0x48eb50 / 0x48ec90 /
  0x48edc0 / 0x48ef10 / 0x48f010 / 0x48ff50: locals, inlined static wrapper, casts, varargs, function pointer
  and ~180 store orders did not reproduce it (g1). Possibly a LARGE_INTEGER-style argument (hv4's lesson).
- Ghidra misses callbacks reached only through pointers: g1 6, g2 5, g3 1, hv4's two heavy ranges 19 functions.
  The AIACT registration code (eval / decide / exec at -0x78 / -0x74 / -0x70, e.g. 0x4a1cd6..0x4a2576) lists them.
- Constants pushed through registers (`xor ecx,ecx` / `mov ebp,1` / `mov eax,imm; push eax`) before a call: an
  __int64 argument built in a union / LARGE_INTEGER (`m.LowPart = k; m.HighPart = 1; f(p, 6, m.QuadPart, ...)`),
  not separate int arguments (layout 2026-10-22: 0x49dd90, 0x48f010 matched).
- A caller doing `xor eax,eax; mov ax,[x]; push eax` passes an unsigned short to an `int` parameter; with an
  `unsigned short` parameter VC6 pushes eax without the xor.
- A float compare whose result presets a flag in the temp slot (`mov [slot],1; fcomp; and eax,0x100; je` then
  `mov [slot],0`) was `w = (float)a >= b ? 1 : 0;`; an `if (...) w = 0;` gives `test ah,5; jp`. In && chains,
  `(float)a >= b` gives `and eax,0x100; jne`, `!(a < b)` does not (layout 2026-10-22: 0x4a1000, 0x4a1170).
- Two callbacks with one address stored in one record without a register CSE (`mov [x],imm` twice): two
  identical functions folded by the linker (/OPT:ICF), not one symbol (0x4a16f0, unresolved).
- Message / record fills (AIMSG, AIACT): the field stores keep source order around the scheduled loads; when a
  store sits early or late in the diff, move that statement (0x4aaa10, 0x4ab9f0, 0x4abed0: `act.id` before the
  strcpy). A clamp the original computes between two stores is a temp in source order there:
  `*(m+4) = ..; k = r > 1 ? r : 1; *(m+0xc) = r; *(m+0x10) = k * 32;` (0x4ab7f0). `n > 2` and `n >= 3` give
  `jle`/`jl` with different immediates (0x4a94e0); `k * 0.25` is a QWORD constant, `0.25f` a DWORD (0x4aa580).
- layout 2026-10-22 (session 4): a float clamp that re-evaluates its expression in every arm is the windows.h
  macros; the nesting / operand order decides the branch layout (0x4ac200: `min(16000.0f, max(1600.0f, X))`, found by
  `uv run try` over the 8 spellings). File helpers whose result is tested with `test; ja/jbe` treat the vfs_fread / fwrite
  count as unsigned: `(unsigned int)vfs_fwrite(..) > 0` (a macro) matched (0x4ac8a0, 0x4acae0, 0x4adb20). An
  unsigned table index guard `cmp eax,3; jb` + `xor eax,eax; ret` first is `if ((unsigned)i >= 3) return 0;`.
  A callee's argument pushed without masking after `util_Random(..) + k` means an unsigned short parameter
  (0x4c3d00 / 0x4c4650); count the pushes before the first call: they may belong to the outer call (0x4ad0a0).
- Ghidra renders D3_ZERO / AI_ZERO with a constant size as three zero loops with counts 0, n/4, 0; the original
  then has an ebp frame. memset gives neither (layout 2026-10-22, unk_4adf40.c: 0x4af090 matched with AI_ZERO).
- A join can regress matched callers when a far-off draft of their callee lands above them (VC6 uses what the
  draft's body clobbers): keep that callee at the end of the file until it matches (am.c, 0x495a50).
- Two counters zeroed before a loop: `for (i = 0, n = 0; ..)` zeroes the loop index first, `n = 0; for (i = 0; ..)`
  the other one first (layout 2026-10-22, unk_4b39f0.c 0x4b3c40 / 0x4b4670). A count after the next-call of an
  iterator: put `n++` into the for increment after the call (`b = next(), n++`, 0x4b51f0).
- AI_ZERO's copy twin (rep movsb / movsd) comes with the source as an immediate (`mov esi, OFFSET g`), as a
  variable or with all three temps; pick the macro shape from the `mov esi` line (hst.c G12_COPY / _P / 2).
- LCG helpers `(seed >> 16) % 0x7fff % n`: write `% 0x7fffu` (the promoted unsigned short is an int, so `% 0x7fff`
  is `cdq; idiv`; the original has `xor edx; div`; layout 2026-10-22, 0x4b3a10 / 0x4bb7d0).
- An inner call in an argument list of a variadic call leaves the outer call's earlier-pushed argument on the stack
  (`add esp,8` after a 3-push sequence): `f(buf, id, g(1, k), s)` (unk_4b6480.c 0x4b8950).

- A pointer loop over a global array that ends in a signed `jl`: `for (m = arr; (int)m < (int)(arr + n); m++)`
  (rt.c 0x4bdae0 / 0x4bdca0 / 0x4bf3b0; the plain pointer compare gives `jb`).

- Copying an odd-sized block between two buffers (7 / 9 / 10 / 12 / 15 bytes) with interleaved load / store pairs is
  a struct assignment of a `struct { char b[N]; }`; field-by-field copies load everything first (cm.c 0x4c58a0..).
- Message senders (`G17MSG m; m.type = ..; fields; send(&m)`): the field store order is the source order; try
  Ghidra's order first, then offset order (cm.c: both occur).
- A caller that zero-extends a ushort before the push (`xor ecx,ecx; mov cx,[..]; push ecx`) calls a prototype with
  an int parameter; a bare `mov cx; push ecx` is a short parameter (cm.c 0x4c6f60).
- `cmp x,1; setle` is `x <= 1` (`x < 2` gives `cmp x,2; setl`) (cm.c 0x4c67c0).
- A local whose slot is a parameter's (e.g. an out argument `&x` at [esp+argN]) means the parameter is dead there;
  read the out-argument order of a call from the push sequence, not from the slots (cm.c's 0x4fea50 callers).

- Switch jump tables (layout 2026-10-22, 0x4cc7b0): blocks are laid out in source order, so a permuted original table needs the cases written in memory order of the blocks; equal-shaped blocks pass the byte check but `uv run link` reports "local reloc .. expected". A one-instruction `jmp X` function is `return f(args);` (sibling call).

- (layout 2026-10-22, session 11) A 12-byte vector out-parameter local that the compiler seems to enlarge by 4 bytes: declare it as float v[4]
  (D3VECTOR4) when the original frame is 4 B bigger (0x4d02d0, 0x4d0a80, 0x4d25a0). A callback comparing with `strcmp(o, "lit")` that calls the CRT
  instead of repz cmpsb is `_stricmp`; 0x5dfb40 is `strncmp`, 0x5eaac0 `_strnicmp` (`uv run link` conflicts show a wrong name). `p += n` as the for increment
  (`for (..; j++, row += n)`) changes register choice (0x454780). Bash heredocs through this harness turn `\` into `\`: write C with backslash strings through a file tool.
- `*(unsigned short *)p` passed to an `int` parameter: `xor reg,reg; mov reg16,[p]`; to a `short` parameter VC6 drops the xor (layout
  session 12, 0x4d3ea0 / 0x4d3d10 via an int view cast macro).
- `(int)(double)i` with an int `i` is folded to `i`; `(int)(double)(float)i` keeps `fild` + `__ftol` (0x4d4340).
- `x * 0.01 * n` vs `n * (x * 0.01)`: the second gives fmul-then-fimul order (0x4d3d10); `if (x < 1.0f) x = 1.0f` gives fcom, `if (1.0f > x)`
  gives fld / fcomp st(1) (0x4d3ea0). `v < 0 ? 0 : v` gives the setl / dec / and form, `v < K ? v : K` cmp / jl with the store first.

- layout 2026-10-22 (session 13): a float constant compared with `< 0.2` is a double compare (`fcomp QWORD`); `0.2f` gives `fcomp DWORD`. `(unsigned int)rand > 7` gives `jbe`
  (unsigned). An expression for a `short` callee parameter, cast to `short` at the call (through a cast of the K&R function), leaves the upper bits unmasked
  (`imul cx,cx,10` / `sub edx,ecx`). Two calls in if / else that share one `call` and `mov [g],eax` after it: write both calls in the branches (a ternary argument
  gives the `mov eax,..; jb` shape). A `while` loop over `ai_Unk5309a0(fn, 0xfd6c6)` with a counter tests the callee's result first (no do-while).

- layout 2026-10-22 (blocked-TU pass): a pointer loop `pp < base + n` compiles to an unsigned `jb`; the original's signed `jl` needs `(int)pp < (int)(base + n)`. A wrapper that only forwards its
  arguments (`if (flag) return 0; return f(a, b);`) compiles to one `jmp f` (also with a changed argument written into the incoming slot); a function that ends without `return` keeps eax from the last
  call / test (`if (r == 0) { ...; return f(x); }` with nothing after it gives the original's bare `ret`). `if ((double)t >= lim)` gives `fcomp; and eax,0x100; jne` where `!(t < lim)` gives
  `test ah,5; jnp`. `or ch,0x80` is `| 0x8000`, `or dl,0x80` is `| 0x80`. do { } while after an `if (first call)` gives the original shape for "run the wait loop at least once".

## layout 2026-10-22 (session 14)
- AI_ZERO / D3_ZERO clears a global array only with the `_AT` form (`offset table`): the pointer-temp form adds a local; same for 0x4e5cd0 / 0x4e4060.
- A prototype with an `unsigned short` parameter pushes the word without `xor eax,eax`: where the shared header says int, call through a cast macro `((int (*)(unsigned short))f)`.
- `strcat(buf, tmp)` inside a loop: the pointer increment written after the strcat schedules like the original (before it, it moves between the scas pairs).
- A table walk written as `for (i = 0; i < n; i++) tab[i].f` gives the signed `jl` pointer compare of the original; `for (p = tab; p < tab + n; p++)` gives unsigned `jb`.
- `(float)(unsigned)x * k` is `fild qword` + `fmul dword`; `(double)x * k` with a float literal is `fmul qword`. `x % 0x7fffu` on a word makes `div`, not `idiv`.
- Two-step returns: `if (!f(x)) return 0; return 1;` is folded to `neg / sbb / neg` (return f(x) != 0); the original `test / je / mov eax,1` is `if (f(x)) return 1; break; ... return 0;`.
- A `test eax,eax` followed by `ja` / `jbe` (instead of `jne` / `je`) on a call result means the callee returns unsigned: call it through `((unsigned (*)())f)`; `if (n != 0)` on an unsigned local compiles to `jbe`-free `je`, write `if (n > 0)` to get `jbe`.
- `x * 0.001` (no `f`) is `fmul qword`, `x * 0.001f` is `fmul dword`: check the operand size in the diff before touching anything else. A zeroed `int m[6]` message is `memset(m, 0, 24)` (rep stos), not six stores.
- A local-by-value copy of a 4 / 8 / 0x14-byte record loop that stores before the pointer add (`*f = ..; j--; f += 3;` vs `f += 3; j--`) changes where `fstp` sits; the original `do { *d++ = *s++; } while (--n)` shape matches `for (i = n; i != 0; i--) *d++ = *s++;`.
- Per-block statement permutations (`uv run try` generator over the 4-24 stores of one child box) found d3q_Build after hundreds of whole-function orders had not: permute small blocks on their own, with the rest fixed (d3q_Build child 2, match 2026-10-22).
- A call used as an argument (`f(a, g(x) + 1, buf)`) pushes the later arguments BEFORE the call; when the original calls g first and then pushes everything, store the call result in a local first (anm_UpdateCat / anm_UpdateSheep, match 2026-10-22). A byte-sized callee parameter: declare the callee's parameter `unsigned short` when the original pushes the register without `and reg,0xffff` (util_Random in anm.h).

## layout 2026-10-22 (session 17)
- A callee defined EARLIER in the same file changes the caller's bytes: VC6 knows it does not clobber the message memory, so `m[0]` stays in a register across the call (one more callee-saved register, `push ebp`); the original reloads it after the call. 0x4f5d50 / 0x4f5f00 (callee 0x4f2870) matched only after the handlers moved to another file: a codegen boundary proof (the callee is in another TU).
- A shared return value kept in a register (`int r = 2; ... return r;`) is constant-propagated away unless it is assigned on a path that reaches the common `return r`: write `if (k == 3 && player) { r = f(c, m); r |= 2; } else if (k == 1 && player) text(...); return r;` (0x4f4ff0, 0x4f5c30, 0x4f6b40); an early `return f() | 2;` gives `or al,2` without the register.
- The 3 / 1 and 4 / 2 order handlers: `if (m[0] == 4 || m[0] == 2) { if ((flags & 0x100) == 0 || (t = *(char **)(m + 0x87)) == 0) return 1; ...; return 0xa; } return 1;` (the first checks share one `return 1`, the later job check keeps its own) is the original's shape (0x4f66b0, 0x4f67c0, and the 8 job handlers).
- `memset(buf, 0, sizeof buf)` written INSIDE the branch that uses the buffer gives `mov ecx,N; rep stos` there; `= {0}` at the declaration puts it at the top.
- An `if / else if / else` chain with one call per branch (`v = f(0, 0, "a")` ...) is merged by VC6 into one call with the string pushed per branch; the ternary-argument form is not (0x4f8200). A scene pointer read from a global first into a local (`s = unk_Ptrfd7c50;`) is what the original does when it uses it twice (0x4f86a0); a pointer parameter used after a call with `&param` taken needs a local copy to stay in a register (0x4f8300).
- A prototype with an `unsigned short` first parameter pushes `mov cx,[esi]; push ecx` (0x4f5a80, 0x4fa700).
- Not solved: `(c[9] != 0 ? 10 : 0) + 0x114 + level` (and the 0x28 / 0x253 variants) compiles to `lea reg,[a+b+const]`, the original is `and; add reg,0x114; add reg,job` (about 15 functions of ord.c); tried: operand orders, parenthesis, temp variables, a prototype with `...`.

## From match 2026-10-22 (second pass)
- `*a = 1.0f - r` with a `volatile float r` loads r first (`fld r; fsubr 1.0f`); `1.0f - *(float *)&r` (non-volatile read of the same
  slot) gives the original's `fld 1.0f; fsub [r]` (d3_Unk4591a0).
- Stores sink behind argument pushes when the call arguments are recomputed from the pointer: `fread((char *)(d + 1), d->len, 1, f)`
  after `d->len = ...; d->data = (char *)(d + 1)` instead of `fread(d->data, hdr.len, 1, f)` (sb_LoadEntry).
- `s->flags |= 8; s->fadestart = 0; s->flags &= ~3;` merges into one `and / or / store`, and so does a temp copy (`fl = s->flags | 8; s->flags = fl; ...; s->flags = fl & ~3;`): VC6 drops the first store. The original's two stores (sb_Update) come from a **volatile first store**: `*(volatile unsigned int *)&s->flags = fl;`. `& ~3` gives `and al,-4`, `& 0xfc` gives `and eax,0xfc`.
- A first `prev = NULL;` inside an `if` moves to the declaration (`SBBANK *b, *prev = NULL;`) when the original has `xor eax,eax`
  hoisted next to the first xor (sb_UnloadSampleBank).
- `int z = 0` / `i = 0` used in place of literal zeros is constant-propagated: it does not produce the original's zero register
  (cmp reg,reg), only a variable the compiler cannot fold does (di_UpdateButtons, evt_DeclareLocal, d3sky_FadeLayer all show it).
- `unsigned int` el used twice vs `now - s->fadestart` written twice: the second form moved sb_FadeTime to the original register.
- A function body `if (c) for (...) ...; return 0;` and `if (!c) return 0; for (...) ...; return 0;` differ in where the zero for the result is set: the early return puts `xor eax,eax` before the test (evt_FindById).
- A store kept after an unrelated later store (byte flags after a float fstp) while its load is hoisted early: read
  into a local before the other store, write that other store through a pointer local (`sz = &p->size; *sz = ..`,
  VC6 then assumes they may alias), store the local last (par_EmitterCB0).
- A compare VC6 always hoists to the top of its block (test of a register before a run of struct stores) moves
  down when one store goes through a copy pointer (`t1 = t + 1; ... t1->v[0] = v;`): it then lands right after
  that store (par_Draw). Try each store of the run through the pointer.
- A dead-parameter slot that the original gives to another variable: add references with no code, e.g. statements
  written in both arms of an if/else that VC6 tail-merges (par_EmitterCB1: speed vs spawned).

- (layout 2026-10-23) A constant compare follows the source constant: `i > 0xff` is `cmp 0xff / jle`, `i >= 0x100` is `cmp 0x100 / jl`; `k >= 10 && k <= 14` gives `cmp 0xa / jl` and `cmp 0xe / jg`, `9 < k && k < 0xf` the other pair. A conditional of two constants `r != -2 ? 0 : -3` is `sub / neg / sbb / and / add` (not `-(r != -2) & 3` written out). A float field read as `(float)(unsigned short)age < *(float *)(c + 0x20)` is `fild / fcomp [mem] / test ah,5 / jp`; written with the int field (`(float)*(int *)`) it is `fild / fcompp`. A float returned and ignored costs an `fstp`: declare float-returning callees as float even when the draft drops the value.

## From match 2026-10-23
- 2026-10-23: a function whose compiled section has NO trailing alignment nops (the exe shows the linker's 0xcc right after its
  last byte, while every /O2 function carries 0x90 nops inside its own section) was compiled with optimization off or for size:
  `#pragma optimize("", off)` (or `("s", on)`, same bytes) around the function reproduces it. Found on the two big inline-asm
  blitters d2_Unk47d210 / d2_Unk47d710; the 0xcc there is therefore not object-boundary evidence.
- A fixed-count inner loop `for (k = 0; k < 3; k++) t->v[k] = &pts[src->idx[k]]` gets the wrong induction-variable merge; `k = 0; do { ... } while (++k < 3);` gives the original's `dec edi / jne` loop with the pointer IV (d3_Unk4550e0). for / while / index spellings are all canonicalised to the same code.
- A struct-field store that VC6 merges away (`x = a; ...; x = b;` with other stores between) is kept by making the first store volatile: `*(volatile unsigned int *)&s->flags = fl;` (sb_Update). `& ~3` emits `and al,-4`, `& 0xfc` emits `and eax,0xfc`.
- Search loop with the found-body after it: write the body inside the loop (`for (b = head; b; b = b->next) { if (b == key) { ...; break; } prev = b; }`) to lose the `if (b)` test after the loop (sb_UnloadSampleBank).
- c2prio on a file with `#line N "D:\..."` / D3DCHECK fails (the /FAs listing cannot open the file): copy the file to src/game/zz_tmp.c with the #line lines deleted and the D3DCHECK( macro stripped, run c2prio on the copy, delete it.
- vargen: several `insert` / `alt` ops multiply (3^n variants, minutes each): one op per run, or hand-write the family with `uv run try`.
- A callee declared with a `char` parameter: the caller loads the byte with `mov al,[x]` and pushes eax without extending (a K&R / int callee gets `movsx` / `movzx`): declare the prototype when the original has the bare `mov al`. A function that returns `char` ends with `mov al,dl` (not `mov eax,edx`). (layout 2026-10-23, f3init.c 0x512de0 / 0x5115b0)
- A long run of field stores (a Create function with 18 stores incl. bitfields and zero stores) whose only difference is the order of stores / the zero-register position: do not reason it out, search it. vargen `move` on 3-6 lines of the block with `--max 1500` (49 -> 24 lines), then single-move hill climb (a script moving every line to every position, keep the best by lines, repeat), then `perm` of the remaining 6 lines exhaustively. The matched order was struct order with the bitfield members last (tex_Create / tex_CreateHi); VC6 sinks bitfield stores behind the plain stores.
- Narrow locals narrow loads of wider globals: `unsigned char bpp = c ? g16 : g8` (int globals) loads bytes; `int bpp = (unsigned char)(c ? g16 : g8)` loads dwords and does one `and eax,0xff` after the join (like the original).
- A dead local assigned in the code never gets a stack slot (dead-store elimination removes the variable), whatever its type (bmp_Load): a slot-only difference needs a real reference or a different object size.

- (layout 2026-10-23) `fread` / `fwrite` style callees return `size_t`: a call-site `jbe` / `ja` after `test eax,eax` means the result is `unsigned` and the check `n <= 0` / `n > 0` (a plain `== 0` gives `je`); Ghidra's `0x10012 < v` is `v >= 0x10013` in the source (`cmp v,0x10013; jl`), keep the constant the original compares against; an `int` global used as `(g & 0xff)` is a dword load + `and ecx,0xff`, a `char` global gives `xor ecx,ecx; mov cl,..`; `return p ? (unsigned)(p - base) / 0x194 : 0xffffffff;` in one expression gives the original register allocation of the table index helper.
- A temp in a fresh local with one definition is forward-substituted (same code as writing the expression out); the same temp in a local that the function reuses elsewhere (`i` of an earlier loop) stays a variable with its own register. ts_LoadSoft: `i = y * tex->size; dst[i + x] = remap[src[i + x]]` with the reused i keeps the remapped byte in bl (46 -> 11 B); a fresh `r` gives the unchanged 46 B. Index spellings (operand order, casts, `p[i]` / `*(p + i)` / `i[p]`, int vs unsigned, post-increments in the index) are all canonicalised: 300 variants, one result.
- A `for` loop with a `goto` out of its body stays top-tested (`cmp i,[mem]; jae out` ... `jmp top`); the same loop with `break` is rotated (guard `test; jbe`, bottom `cmp; jb`, the bound CSE'd in a register) and a test after it on the loop's exit condition is threaded on the normal exit only (the break path keeps a recheck). A search loop whose hit leaves the outer `do { } while (start)` running and whose miss ends it is `start = NULL; for (...) { if (hit) { ...; start = ...; break; } }` at the end of the do-body: the miss edge threads the constant start to the outer exit (`jmp done`), the hit falls into the `test start` (d3_mir d3_Unk454890: 168 -> 6 B).
- (layout 2026-10-23) A callback / helper whose original ends with `mov al,1` (and `mov al,dl` for a bool result) returns `char`: declaring it `int` costs a few bytes per return. A scan over a static pointer table that the original compiles to a pointer-increment loop with a `cmp ptr, end` bottom test (0x522100, 0x5223c0 in sv.c: `q = tab; do { if (*q != -1) ...; q++; } while ((int)q < end);`) matches only as a do-while over a pointer, not as an indexed `for`.
- (match 2026-10-23) A local that only caches a repeated subexpression (`half = c >> 1` used three times) takes a register of its own by its reference weight and can push the variable it was computed from into the wrong register; writing the subexpression out at each use (VC6 CSEs it into a cheap temp) gave the original's registers (qnt_MakePalette). Same family: replacing one use of a pointer local by the spelled-out index (`mark[b]` -> `used[nedge + b]`, d3_Unk454890) changes the weights of the address operands; a *added* local can also be right (`sc = d3_DefaultScene` hoisted the global load before the test, d3sndw_Update).
- (match 2026-10-23) `(unsigned char)i & 0x80` on an int register gives `mov cl,al; and ecx,0x80` (byte move) instead of `mov ecx,eax`; the same cast twice is shared as one `and edx,0xff`, so each further term needs a different cast type (`(char)i & 0x40`) (qnt_InitTables).
- (match 2026-10-23) A function whose empty-list path returns `mov eax,ecx` from a hoisted `xor ecx,ecx` while the loop exit returns eax: `m = list; if (m != NULL) { do { if (hit) break; m = m->next; } while (m != NULL); return m; } return NULL;` gives the shared loop-exit / found `ret` of the original; the hoisted zero is still missing (mov_Find).
- (match 2026-10-23, 5th) Operand order of address sums (`[ebx+edi]` vs `[edi+ebx]`) follows the rank of the variables' first occurrence: a dead store (`mark = 0; a = 0;`) at the top of the function fixed 2 of 4 swapped orders in d3_MirCreateOutline where every spelling of the expressions was canonicalised.
- (match 2026-10-23, 5th) A callee-saved register that the original gives to a constant (`xor ebx,ebx` + `cmp reg,ebx`, `mov [g],ebx`) is taken in ours by a CSE'd global read (di_UpdateButtons: `dm_S.x` cached in bx). A `volatile` read at a few uses (`(*(volatile short *)&dm_S.x)` in the four press-position stores) stops the CSE and gives the original's register map (252 -> 104 lines); do not make every use volatile.
- (match 2026-10-23, 5th) `x * 3`, `x * 5`, `x * 7` stored to different places compile to lea / imul forms; the original's chain `t = x + x; x += t; use; x += t; use; t += x; use` (3x, 5x, 7x by adds, a Floyd-Steinberg error split) needs exactly that source (qnt_Map: 524 -> 318 B). An expression the original computes once above a few calls (`lea edi,[ebx+ebx*2+6]` before malloc) is a local assigned there (`cnt = w * 3 + 6`).
- (match 2026-10-23, 5th) Three stores through two pointers into one buffer (`cl = clamp + 0x100`: `cl[k] = k; clamp[k + 512] = 255; cl[k - 256] = 0;`) change which family becomes the loop's induction variable and the register map after it (qnt_Map 822 -> 563 B); try the other pointer's spelling of a store (`clamp[k]` = `cl[k - 256]`) and the three orders. The base of such an IV (&t->volume in msx_Update, &sl->weight in d3_Unk459300, cl in qnt_Map) is still unexplained.
- (tools) vargen finds the function by a definition regex at column 0; a call line like `if (!f(x)) {` used to match first.
- (layout 2026-10-23, 6th) The 14-byte game clock copy `r->due = ai_Time` is a STRUCT assignment of a pack(2) struct {int; ushort; int; int} from the named extern `ai_Time` (0x770ae0): it compiles to `mov edx,[ai_Time]` + `lea ecx,[r+0x52]` + field moves through ecx, exactly as in the original; four separate field assignments from absolute addresses (`U_I(0x770ae0)`) give a different schedule. This is the opposite of the 12-byte vector rule above (those were field by field). (he.c 0x541200, 0x543020, 0x543180, 0x5436a0, 0x542960)
- (layout 2026-10-23, 6th) `if (x == 0) { switch .. }` with an unsigned char x compiles to `jne`, the original `test dl,dl` + `ja` is `if (x <= 0)` on an unsigned char (he.c 0x53ec40).
- (layout 2026-10-23, 6th) In a handler that stores a call result and a constant into the same record (`ev->h = -1; a = f(ev->x);`) the order of statements decides whether the constant store comes before the first call's push: write the store first (he.c 0x543180).
- Operand order of FPU products with `m[k]` memory operands is a per-node rank that depends on how the element is spelled (`m[k]` vs a struct field `src->_ij` vs `((float *)src)[k]`): a hill climb over the spelling of each of the 16 elements (statement block by statement block, compile + count asm lines, uv run try with a generated variants file) took d3_MatInverse 328 -> 67 lines where no textual swap / paren / cast changed anything. Also: a product the compiler CSEs (same text twice, no store between) keeps the operand order of the shared node, an unshared one is canonicalised differently (probe: `o[1] = m[2]*m[5]; o[0] = m[3]*m[1] - m[2]*m[5];` gives m2 first, alone m5 first). Stores through a `D3MATRIX *` field and a `float *` both kill the loads (no CSE across them).
- (match 2026-10-23, 7th) Three cheap rules from di_UpdateButtons (551 B -> 7 lines): (1) a goto-merged tail (`goto lA / lB`) hides a store the original has on one path: write the arms out in full (`else { x = 0; ...; }`) and let VC6 merge identical tails itself; (2) when a variable was just tested `== 0` VC6 stores its register for a later `= 0` of another global (`mov [g],ecx`), the original's immediate `mov [g],0` means the other store (here `mhold = 1`) came first in the source; (3) the stores around volatile reads: `t = now; px = (volatile)x; py = (volatile)y;` puts the time store between the reads and the position stores (a `perm` round of the three lines found it). A relational constant spelling (`> 15` vs `>= 16`) can change which register the compared value gets while the rest of the function is still wrong: try both spellings after each structural fix, the right one flips back. A pointer scan that compares signed (`jl`) and addresses a field (`cmp [ecx],0`) is an `int *` walked with `q -= 19` and `(int)q < (int)&a[0].f`.
- (match 2026-10-23, 8th) A statement-order problem with volatile stores in it (d3_Unk459300's slot setup: volatile `z` copies and volatile `weight` reads) is best attacked with a single-statement-move hill climb over the whole block (every statement to every position, valid orders only, metric = asm lines, 3 iterations: 198 -> 176 lines), not by guessing the original's grouping; the x / y components must stay `V.x = F.x; V.y = F.y; V.x = w * V.x;...` (copy + multiply) so that the two loads share the FPU stack, direct `V.x = w * F.x` multiplies from memory. `if (!(z < MaxZ)) MaxZ = <re-read of v->z>` (compare on the register value) beat the memory compare by 12 lines.
- (match 2026-10-23, 8th) Strength-reduction base of an array-of-struct loop (msx_Update volume vs fadepos, qnt_Map, d3_Unk459300 weight): toy loops over `T tr[10]` with `foo(t->x)` calls show the base is picked from the ORDER of the post-CSE references: distinct fields -> the 2nd referenced one (offset 0 never counts), repeats follow a count / recency rule (b b b c -> b, b b c c -> c, b c c b -> b, ecded -> e); dead loads / stores do not count. Not yet a usable rule; toy harness: write the loop into build/toy/x.c and run uv run listing (0.1 s per compile).
- (match-tu 2026-10-23) FPU operand ranks, two movers that matched: a `(float)` cast around the first product of a sum orders the products (d3_VecAngle), a `(float)` cast on the double operand of a mixed double * float product orders that product (d3_MatFromAngles: 2 casts of 7 candidates); a dead store `a = c;` placed before `b = c; a = c;` ranks a before b for a later `a * b` (VC6 drops the first store). None of the three reaches fields read through a pointer (d3_MakeBasis). uv run respell rules fcast / deadrank, idiom fpu_rank.
- (match-base 2026-10-23) A 16-bit parameter (`unsigned short n`) is pushed by the caller WITHOUT a zero-extend and masked by the callee with `and reg,0xffff` at the top; a 16-bit return type returns in `ax` without touching the upper half (`mov ax,ds:global; pop...; ret`). Changing path_Reverse's parameter to short alone cost 21 asm lines because its body then truncates an internal `last = p` assignment.
- (match-base 2026-10-23) A local `unsigned short` used as an array index compiles to `mov WORD PTR [slot],ax` at the assignment and `mov reg,DWORD PTR [slot]; and reg,0xffff` at the use (not movzx), and local stack slots are reused across disjoint live ranges (the same [esp+0x14] holds the short index first and the int result later); expressing the index as `(unsigned short)a` on an int local produces exactly that.
- (match-base 2026-10-23) `for (dir = 0; dir < 8; dir++)` with a signed `int` gives `inc; cmp eax,8; jl`; the `for (;;) { ... if (++dir > 7) return; }` form gives `cmp eax,0x7; ja`. `goto` to a label at the end of the body becomes `continue`.
- (match-base 2026-10-23) A float compared against the double literal `0.0` gives `fcomp QWORD PTR ds:0x5eb830`; `0.0f` gives `fcomp DWORD`. The flag test that follows is VC6's own choice (`and eax,0x100` vs `test ah,0x5; jnp` vs `and eax,0x4100`) and does not follow the source operator: the same `>= 0.0` in one function can get two of them, so do not chase it as a meaning error.
- (match-base 2026-10-23) Tables read with `fld DWORD [tab+eax*4]` / `fstp [global]` are float arrays, not int: check the load form before trusting an `int[]` extern in the header.
- (match-base 2026-10-23) The original's `isdigit(c)` is VC6's ctype.h macro, expanded on /O2 as `MB_CUR_MAX > 1 ? _isctype(c, _DIGIT) : _pctype[c] & _DIGIT`: `__mb_cur_max` is an int (`cmp DWORD [x],1; jle`) and `_pctype` an `unsigned short *` (a byte load at scale 2 for the `& 4` test). A `unsigned short` declaration of __mb_cur_max compiles to `cmp WORD ...,2` with the branches swapped; write the isdigit call and let ctype.h supply the declaration (io_text.c 0x514360, byte after the fix).
- (match-base 2026-10-23) Tail index of a copy: `len = n - i; memcpy(p, s + i, len + 1); p[len + 1] = 0;` makes VC6 compute the subtraction into n's register (`sub edi,ebx`) and move the result to eax; the original's `memcpy(p, s + i, n - i + 1); p[n - i + 1] = 0;` (no len local) keeps n in edi and computes n - i in eax, one code hunk fewer (io_text.c 0x514360, byte).
- (match-base 2026-10-24) An anonymous-union alias added to a shared struct (D3OBJINFO: `union { D3OBJECT *unk10; void *mesh; };`) changed the codegen of a byte-matched function in ANOTHER unit (d3_mir.c d3_Unk4550e0 lost 8 bytes at +0x1be). Reverted and re-done as a plain field rename with the original type (`D3OBJECT *mesh`), which is byte-neutral. So: prefer the rename; the union view is only for fields whose type really differs.
- (match-base 2026-10-24) A flag kept in a named local assigned inside an `||` condition gets a stack store; re-evaluating the same expression in the other branch lets VC6 CSE it into the register and reuse it, exactly as the original (surf_Blit: 21 code hunks -> identical instructions; the flat `else if` conversion chain instead of a nested else + return was part of it).
- (match-base 2026-10-24) Before inventing a name for a placeholder callee, check the OTHER units' headers for a named declaration of the same address: d3_Unk44c9d0 was already `d3io_LodName` in d3_int_io.h and d3_Unk5fbf78 already `d3io_Lod`; replacing the stale aliases with the owner's names fixed Q2 without a guess (and removed duplicate externs).
- (match-base 2026-10-24) A function's extent can end well before its next sibling: err_ExceptionFilter
  (0x4215c0) ends at 0x421c34 with `ret 4`, and the bytes after it are its 20-case exception switch jump
  table. uv run asmdiff keeps disassembling that data as code and reports sbb/inc/xchg hunks, which look
  like missing statements; check the function's real end (uv run disasm around the last `ret`) before
  reading such a diff as behaviour.
- (match-base 2026-10-24, 2nd) d3_Unk4550e0 (d3_mir.c, byte) is perturbed - the same 8 B at +0x1be, two
  parameter loads swap - by changes to shared struct BODIES in d3.h: (a) splitting D3SCENE's
  `char pad084[0x28]` into a 0x20 pad plus two pointer fields at the same offsets (typed or `void *`, both
  perturb), (b) replacing D3PARTSYS's union member `unk24` with `time` / `maxSpawn` (same offset and type).
  Both were reverted. Appending a comment line to d3.h does NOT perturb it, so it is the struct body, not
  the line numbers. Not isolated: whether the plain D3PARTICLE.unk50 -> frame rename alone is safe (the
  earlier D3OBJINFO note says a same-type rename is byte-neutral). Workaround used: leave d3.h alone and
  read the scene's 0xa4 / 0xa8 through a typed view struct (D3SCENEPAR) in d3_par.h, which only d3_par.c
  includes. Rule of thumb: treat the member set of the shared d3.h structs as frozen while a byte function
  of another unit reacts to it; put new fields into a view struct in the unit's own header.


## match-base 2026-10-24 (d3_par, mov)

- A typed field added to a shared struct in place of padding can change the codegen of OTHER functions:
  `unsigned long stamp;` at D3TEXTURE +0x54 (was pad44[0x14]) changed d3_Unk4550e0 (d3_mir.c) by 8 B, a
  load-order swap; reverted. Put the field into a view struct in the unit's own header instead
  (D3PARSTAMP). Same rule as the D3SCENE / D3PARTSYS note above.
- The raw-cast bitfield access `(*(PARVFLAGS *)&v->flags).b4` is part of the byte match: VC6 carries the
  stored byte in a register across a chain of such assignments. Naming the same bits through a union
  member of the struct (`v->bits.b4`) makes VC6 reload/re-register them (par_Draw 855 B, d3_CloneObject
  4 B, d3_Unk4550e0). Keep the cast and only give the macro a typed-pointer shape
  (`#define PAR_VF(v) ((PARVFLAGS *)&(v)->flags)`, used as `PAR_VF(v)->b4`): same code, no raw deref.
- A list search whose original has `xor ecx,ecx` after loading the head and `mov eax,ecx` on the
  empty-list return is a `found` variable initialized to NULL at entry and assigned m AFTER the loop
  (`found = m; return found;`), not a copy of m inside the loop and not `return m` / `return 0`.

## layout 2026-10-24 (map / production settings)

- Two spellings of one integer test are different bytes: `0x74 < x` gives `cmp x,0x74; setg`, `0x75 <= x` gives `cmp x,0x75; setge` (0x5a8b80, 2 B). When a compare is 2 bytes off, try the other canonical form (the original source wrote the constant +1 with `>=`).
- A condition can be redundant and still be in the bytes: 0x5a8d80 tests `kind == 6 || kind != 7` (a `cmp 6 / je body / cmp 7 / je skip`); the plain `kind == 6 || kind == 7` compiles to a `jne` at the second compare. Read the branch polarity from the disassembly, not from the likely intent.
- A first ROUGH draft of a very big function can make cl.exe spin: 0x5aab80 (5328 B) written out as its full statement list (a >4 KB frame with an `int arr[0x114]`-style copy, ~150 calls) ran >10 CPU minutes without finishing and had to be killed. Keep such a draft a small stub first, build it, then grow the body in pieces (each built); the stub compiles in seconds.

## layout 2026-10-24 (transport block)

- A table walk over a global array of fixed-size entries, written as BP-relative pointer code
  (`int *p = (int *)0x62c420; while (*p) { ... p = (char *)p + 0x1c; }`), compiles to `mov eax, 0x62c420`
  (the ADDRESS materialized) and loses the original's `mov eax, ds:0x62c420`. Write the walk as an INDEX loop
  over absolute addresses instead — `for (i = 0; U_I(0x62c420 + i * 0x1c) != 0; i++) if (U_I(0x62c420 +
  i * 0x1c) == id) return 0x62c420 + i * 0x1c;` — and VC6 strength-reduces the loads into a pointer register
  (ecx) while keeping the index (edx) for the return, exactly the original shape (0x5ba620 / 0x5ba660 matched
  first try, 64 / 80 B).
- The 14-byte game clock as ONE struct assignment from the named extern (`*(HETIME *)(r + 0x52) = ai_Time;`
  with a TU-local `#pragma pack(push, 2)` `struct {int; unsigned short; int; int;}`) is the form that matches
  when the original copies the four dwords / the word with an absolute source and a `lea` base for the
  destination (0x5ae870); the same code written as four `U_I(r + 0x52)` stores or through a local pointer
  (`clk[0] = ...`) compiles to `[eax+0x52]` stores with no base register and does not match. When the original
  copies the clock FIELD BY FIELD from `ds:0x770ae0` into a struct (0x5b5e80), the field stores are the right
  form instead — check the disassembly of the particular function.

## match 2026-10-24 (match-base, d3_anim.c)

- Header edits to d3.h can flip scheduling in unrelated functions: an anonymous union alias
  (`union { D3VECTOR unk084; D3VECTOR rot; }` in D3OBJECT) and a typed field replacing a pad
  (`struct D3PATH *path` at 0x1d0 with D3_OBJPATH as `(obj)->path`) each made d3_Unk4550e0 (d3_mir.c)
  nonmatching by 8 bytes - two `mov esi,[ebp+8]` / `mov edi,[ebp-4]` loads swapped - and the D3PATH field
  also moved 4 bytes in d3_anim.c. Both reverted; identifier-only renames (fields and functions) left every
  byte count unchanged.
- Renaming a function that is declared differently in several files (the X lines of uv run lint) must
  make every declaration agree in the same commit, or the pre-commit ledger gate fails with "declared
  differently here and elsewhere" (every X conflict is an error; nothing is parked).

## match-base 2026-10-24: a rename must not change the declared type

A project-wide field rename is byte-neutral only when the member keeps its type. Writing
`float fadeStart/fadeEnd` for D3ANIMSLOT 0x34/0x38 (the old header had `int unk34/unk38`) changed
d3_Unk432750 from 4480 to 4528 bytes and moved its frame from 0x70 to 0xbc - a real codegen change, caught by
`uv run unitdiff <file> --against` as WORSE while the ledger check count (2495) and `uv run ledger check --against` stayed
clean (the function is draft). Bisect: compile the one file with
`cl /nologo /c /O2 /Gy /Iinclude /Fo<dir>\ src\game\<unit>.c` and read the function's COMDAT section size from
the COFF symbol table; reverting name groups one at a time showed no single group, the type fix did.

## Address-taken parameter (unk_58bbd0.c 0x5c4100, layout 2026-10-24)
`f(param_1, &param_1)` followed by a later use of `param_1` (a K&R out / in-out parameter): the original
(0x5c4100) pushes esi, keeps `param_1` in esi, passes the address of the INCOMING slot ([esp+8] after
`push esi`), then reloads the slot after the call (`mov edx,[esp+8]`, `pop esi`, `mov [esp+4],edx`) and
tail-jumps. Our first draft kept the value only in its slot (no esi push, a plain tail call) - 35/48 bytes.
The direct form (`&param_1`, no local copy) is the one to try first; a `p = param_1;` local changes the
prologue but not the tail.

## match-base 2026-10-24: a struct-declaration change flips scheduling in functions that do not use it

Adding a member / anonymous union / bitfield view to a shared struct flips VC6's instruction order in byte
functions that do not touch the changed struct at all. d3_SetVertexColors (0x452fa0, d3_light.c) needed typed
field views for two bytes, and each attempt perturbed a different byte function by two swapped loads
(`mov esi,[ebp-4]` / `mov edi,[ebp+8]`, 8 bytes, asmdiff class `order`):
- `D3VECTOR light` at D3PVERT 0x30 (replacing pad20) broke d3_Unk4550e0 (0x4550e0, d3_mir.c);
- the `union { unsigned char flags; D3TEXBITS flagBits; }` at D3TEXTURE 0x68 + `D3TEXFLAGS loadFlags` at 0x6c
  broke d3_Unk4550e0;
- the `union { unsigned char drawFlags; D3TRIBITS drawBits; }` at D3PTRI 0x26 (plus moving D3TRIBITS above
  D3PTRI) broke d3_CloneObject (0x43d180, d3_engine.c);
- with the D3TEXBITS union present the tri union was harmless for d3_Unk4550e0 and fatal for d3_CloneObject;
  with it absent the reverse - the combination is not monotone, each change re-rolls a tie-break.
Isolation: HEAD headers + only the identifier renames reproduce every byte ("vD" in the session), so renames
are safe and the struct edits are not. Workaround that keeps the bytes: cast views local to the unit header
(D3BITS, D3TEXVIEW) and a union defined in the unit header (D3TEXPACK, for `*(int *)&f = 0`) - these are new
types in d3_light.h, not members of the shared structs, and the target function stays byte.
Also: `D3TEXFLAGS f = {0};` as a declaration initializer is NOT the same code as `*(int *)&f = 0;` (647 of
784 bytes differ); the union member `((D3TEXPACK *)&f)->i = 0` is.

## match-base 2026-10-24: a duplicated test in the original is a duplicated check in the source

d3s_GetGround (0x45bcd0): the original does `mov eax,[edi+0x1f8]; test eax,eax; je end; mov esi,eax;
test esi,esi; je end` - the `if (p)` test and the `while (p)` test both survive because the source checks the
expression twice: `if (obj->parent) { p = obj->parent; while (p) ... }`. Our `p = obj->parent; if (p) {
while (p) ... }` let VC6 merge the two tests into one (the pair of tests was 29 bytes of code hunks). Do not
"simplify" such a duplicated test away in the source; it is the original's shape.

Same function, local order: `float y;` declared before `D3VECTOR4 pos;` plus `char found;` with
`found = 0;` as a statement after `y = 0.0f;` reproduces the original's `mov [esp+0xc],0` before the argument
pushes and `xor bl,bl` after them; the former `char found = 0;` initializer put the `found` store first and
moved `y` into the vector's last slot ([esp+0x18] after the pushes). Also d3d_UsedCard.info.maxw (0xfe7998)
is the same global as an extern below a 0x338-byte D3DCARD struct - check data.csv aliases before inventing
a name.

## match-base 2026-10-24: read the displacement of a byte field as evidence of the field

d3s_Project / d3s_Update (0x45be10 / 0x45c620) read the terrain block byte at block+2 (0x13e): `lea
edi,[edi+eax*4+0x13e]`, and the value is used as a power-of-2 cell mask (`if (mh > 1) { m = ~(mh - 1); ... }`).
That is the block's `.lod` (d3s.h D3STBLOCK: lo 0, hi 1, lod 2), not `.hi` (block+1) as our source had. The
struct the code reached through (d3_sm.h D3SMBLOCK) simply lacked the field; adding `unsigned char lod` after
`hi` changed no byte of any unit. When a source/header disagrees with the original's displacement by one byte,
the field order in the header is the suspect, not the expression.

d3s_Project: `(int)((x1 - origin) / axis + 1.0)` with a double `1.0` compiles to `fdiv; fadd QWORD PTR
ds:__real@...; call _ftol`; our `(int)((x1 - origin) / axis) + 1` gave `call _ftol; inc eax` (same values for
all v). The double literal in the original is the tell: 0x5eb8a8 holds 1.0 as a qword. Same rule as the
double `0.0` in d3s_Update (`fcomp QWORD`).

## match-base 2026-10-24 (base-1 worker)

- `x - 2` written into a local (`m = h->size - 2; if (x > m) x = m;`) compiles to `mov eax,[..] / sub eax,2 /
  cmp x,m / jle`; the same bound written inline in the comparison (`if (h->size - 2 < x) x = h->size - 2;`)
  compiles to `mov ecx,[..] / add ecx,0xfffffffe / cmp m,x / jge` and reuses the loaded value for the assignment
  (VC6 CSEs the second read). Probes: a `- 2` on a *register* operand is `add reg,-2` too; on a pointer-loaded
  int it is `sub` unless the expression sits inside the comparison.
- A 16-bit table field loaded at a *constant* address is `mov ax,ds:addr` for VC6 in every source shape probed
  (for / while / pointer / inline expression; 2-byte structs, unsigned short id): `mov eax,ds:addr` + `test ax`
  in the original (rd.c veg_FindType 0x4bc5a0) has no found source spelling yet - a deep lead.

## match-base 2026-10-24 (base-0 worker, session 2)

- A shared header's struct *member list* is part of VC6's codegen input: adding an anonymous union at 0x954
  to D3MESH (d3_engine.h) and, separately, a field at 0x954 to D3MATERIAL (d3.h) each flipped d3_Unk4550e0
  (d3_mir.c, byte) by 8 bytes - two swapped `mov esi,[ebp+8]` / `mov edi,[ebp-4]` loads, nothing else - while
  a pure *rename* of the same fields (D3MESH.unk954 -> groundOffsetRaw) changed no byte. The same trap as the
  recorded D3PTRI/D3TEXTURE/D3PVERT views. Fix used: the typed field lives in a cast view of the unit's own
  header (d3s.h D3SMESH / D3S_MESH(obj->material)), d3_light.h's D3LOBJ pattern; d3_light.c's D3LOPS.radius
  (d3_light.h) is the same 0x1d4 the ground calculation reads, which also settled D3OBJECT.unk1d4 -> radius.
- Two stores to the same field with only the second's value changed: `fl = s->flags | 8; <volatile store of
  fl>; s->fadestart = 0; s->flags = fl & ~3;` is what keeps VC6 from merging the stores (`mov [esi+0x14],eax`
  then `and al,0xfc; mov [esi+0x14],eax`); a volatile *local pointer* to the field changes the scheduling
  (the second store's `and` and the fadestart store swap), it has to be a volatile access at the field itself.
  A union member `volatile unsigned int volatileFlags` over the same dword gives the same bytes as the raw
  `*(volatile unsigned int *)&` cast and keeps the access typed (SBSAMPLE in sblib.c).
- `if (found) return y;` followed by more code compiles to an inline `je`-over-epilogue; the original's
  `jne <shared epilogue>; jmp <continuation>` (the return block at the function end, the parent-test's else
  block laid out between them) comes from the compound `if (!found && obj->material) { ... } return y;` with a
  single final return (d3s_GetGround). The compound makes the found test a jump to the shared exit.
- Consecutive cdecl calls can share ONE `add esp` cleanup: VC6 pushes the next call's arguments before the
  previous call's arguments are popped (0x5cfc00: `call 0x5176d0` with 2 arguments, then a `call 0x53dda0`
  with 6, one `add esp,0x20` after it). Count every push between two stack adjustments, not only the ones
  directly before the call.
- A float argument to an UNPROTOTYPED callee is promoted to double (8 bytes); to pass the original's raw
  4-byte float, cast the function pointer at the call site (`((void (*)(int, int, float))0x4c4ac0)(...)`,
  layout 2026-10-24). The same applies when the header's view of the callee is int.

## match-base 2026-10-24 (base-1 worker, shard 1/5)

- D3MATERIAL 0x104 confirmed a member-list trap a second time: an anonymous union `ops` / `name`
  (char*, the value ch_TakeObject copies as the model name) in d3.h flips d3_Unk4550e0 (d3_mir.c,
  byte) by 8 bytes at +0x1be; reverted, the raw D3MATERIAL_NAME104 access stays (Q4 debt of
  chc_TakeObjectScript, escalated). Only plain renames of d3.h fields are byte-neutral.
- Q6 counts an address comment above a prototype *in any file*: vfs_fwrite (0x430e30) was readable
  through ls.c's foreign `/* 0x430e30, the file read / write ... */`; removing that declaration (the
  right call, ls.c now includes vfs.h) dropped it to draft until include/vfs.h described the
  prototype. When a foreign declaration goes, move its description to the defining unit's header.
- The saved-game writers of ls.c (layout drafts) are shape problems, not meaning ones: the count
  loop keeps n in a stack slot in the original (`inc DWORD PTR [esp+0x10]`), the record pointer and
  the file handle sit in edi / esi in ours and esi / edi in the original (0x519d20), the base
  pointer of 0x51a1a0 lands in eax instead of esi, and the original reloads a global after a store
  that our local copy lets VC6 keep (0x519a00). Constants, offsets and calls match - match-deep's
  vargen ren / slot work, all escalated with the evidence in the @deep tags.

## From the base-2 match-base session 2026-10-24 (f3snd.c)
- `memset(s, 0, CONST)` does not compile to the generic inline memset. VC6 folds the constant and emits unrolled
  dword stores (`mov [ecx],eax` x4) in a plain esi/edi frame. The generic form - value and size materialised in
  stack slots, `mov ah,al / shl eax,0x10 / mov ecx,eax / shr ecx,0x10 / add eax,ecx` (byte broadcast), the
  `lea ecx,[edi+edi*2] / and ecx,3 / sub ebx,ecx` alignment and `rep stosb / rep stosd / rep stosb` - is the same
  intrinsic expanded without folded arguments and brings an ebp frame. A function differing only there is a
  spelling job (snd_AddToQueue 0x4c0230, 90 B, escalated).
- The 13 * d3_Time idiom from msx.c / sblib.c (`((d3_Time << 3) + (d3_Time << 2) + d3_Time)`) is what makes the
  volatile read appear three times; a plain `d3_Time * 0xd` reads once and leaves regs/code hunks in every
  function that uses it (f3snd.c now has F3SND_TIME).
- A float min written as `min(n, 64.0f)` with int n comes out `fild n; fcom [64.0f]; jnp`, the original's
  `fild n; fld 64.0f; fcomp st(1); jp` is the other argument order (max(64.0f, n) - same value for ints).
- A type declaration added to a shared header can flip a byte function without touching any code: adding the
  D3OBJCOLOR view (a typedef only) to d3_engine.h recompiles d3_mir.c through its include and made
  d3_Unk4550e0 differ by 8 bytes (two `mov esi/edi` scheduling swaps), like the D3MESH/D3MATERIAL member
  changes of earlier sessions; the same typedef in the only user's .c (d3_engine.c) leaves d3_Unk4550e0
  matched. Unit-local views go into the unit's .c, not into a header other units include.

- Adding a function prototype to a widely included header can flip the codegen of a function in another TU:
  `void d3fl_FreeMap(D3FLMAP *);` in include/d3_fl.h (included by d3_tools.c) turned d3t_UpdateSunRays
  (0x4172e0, d3_tools.c) from byte into 125 differing bytes - the original's `fisub [d3t_SunFadeStart]` became
  `fild [d3_Time]; fsubp st(1),st` and the tail moved by 2 bytes. The same prototype declared in the only
  caller's .c (d3_interface.c) leaves it matched; d3_engine.c already declares it locally. Same family as the
  D3OBJCOLOR view in d3_engine.h (above). After any header edit, rebuild from a clean build/obj and check the
  whole exe, not only the defining unit.
- Two stores of the same value to two different globals can be emitted in either order; VC6's choice is not
  controllable from the C (every source order, chained assignment, types, names and declaration order gave the same
  order here). The masked byte compare masks exactly those addresses, so such a function reports "matched" while
  `uv run link`'s site inference sees the two relocations swapped ("matched sites disagree" for both symbols). A struct
  view of the two adjacent words (G16TRACKHANDLES { current; copy; }) is the fix: the stores then relocate to one
  base with +4 / +0 addends and both sites agree on the base whatever the order.
- A char flag can be read as a word in the original: `mov eax, ds:flag` (a1) followed by `and eax, 0xff` before a
  dword store, while other functions store/compare it as a byte. A union view at the address ({ char on; unsigned
  int word; }) reproduces it; declare the word member only where the word load happens (f3snd.c G16AMBIENTON).
- A struct member-list change in a header every unit includes flips unrelated functions even when the layout is
  identical: splitting D3OBJECT's `char pad068[0x1c]` into `pad068[0x10]` + `D3VECTOR posOffset` (0x78, same
  offsets) flipped d3t_UpdateSunRays (d3_tools.c) from byte to `fisub` vs `fild/fsubp`; changing
  D3MESH.unk008/unk048 from `char[0x40]` to `D3MATRIX` flipped d3_CloneObject. Renaming the same char arrays
  (no member-list change) and turning D3CLIP into a union / adding an anonymous union to D3PVERT stayed clean.
  Test every shared-header edit with the full `uv run ledger check`, not only the unit diff.
- The d2 object table 0xf8f080 is a POINTER global (d2_Objs), not an array: `U_I(0xf8f080 + off + id * 0x2ec)`
  compiles to an absolute store with a `mov eax, ds:0xf8f080` that the original does not have; write
  `U_I(U_I(0xf8f080) + off + id * 0x2ec)` (0x5d6f40 matched with it; the same holds for every control-record
  access of the front-end windows). Caching it in a local pointer (`char *obj = (char *)U_I(0xf8f080);`) gives one
  load for many accesses, which is what a window with several fields does.
- Screen metric globals are SHORTS: 0xfb9070 (height) / 0xfb9072 (width) load as `movsx r, WORD PTR ds:..`; read
  them as `*(short *)0xfb9072`, as dword reads leave 4 differing bytes per use (0x5d6fa0: 161 -> 91 differing B).
- Byte-sized globals: 0x6f25ac is a char (the original writes `mov BYTE PTR ds:0x6f25ac, 1`); `U_I` stores leave a
  dword store and a wrong length (0x5d7e90 matched after switching to U_B; 63 -> 0 differing B).
- A function whose original has no `xor eax, eax` before the ret is `void`: `int f() { ... return 0; }` adds the
  `xor eax, eax` (0x5da010 matched as void; the three contact listeners 0x5d9a80 / 0x5d9b20 / 0x5d9b90 are void
  too - their remaining mis-match is the building kept in a callee-saved register, not the return).
- A constant compare the original makes against an immediate (`cmp [p+0x20a], 0xffffffff`) can come out as a
  register compare (`cmp [p+0x20a], edx`) in ours when the constant is written inside an `&&` chain - one of the
  constant-web effects; not resolved for 0x5d73e0 (51/80).
- Four loads of distinct absolute globals followed by interleaved stores (the screen rectangle copy
  0xfb9060 .. 0xfb906c from 0x6248a4 .. 0x6248b0) are hoisted by the original's scheduler above the first store;
  every source order tried keeps ours load-store-load-store (0x5d7080 27/224 remains).
- Float vs double literals are a base-level type fix, not a spelling one. d3_ScreenToUV (d3_interface.c) had 15
  QWORD constant uses (`1.0`, `0.0`, `1e-07`, `0.33333334`); the original's .rdata shows DWORD constants
  0x5eb780 = 1.0f, 0x5eb788 = 0.0f, 0x5eb948 = 0.33333334f and a QWORD 1.0 (0x5eb8a8) only for
  `r + s <= 1.0` and the outv `1.0 - (...)`. Adding the f suffix where the constant is DWORD removed every
  QWORD fmul/fcomp/fsubr (1472 -> 1451 differing bytes). Check the constant's size, not only its value.
- A float field written to a file as its bit pattern (`*(int *)&x`) fails Q4; a unit-local union view
  (`typedef union D3IFBITS { float f; int bits; }`) with `((D3IFBITS *)&x)->bits` compiles byte-identical and
  passes the rule without touching the shared struct (d3_WriteMap, d3_SaveScene, D3IFREGION / D3IFMAP). A raw
  macro like D3_SCENE3C8 (a dereferenced cast in its body) is avoided the same way, through the typed
  D3IFSCENE view (mapSize 0xb4, events 0x3c8).

- A loop over a struct array: VC6 biases the induction variable by itself when the source is natural. The save
  game's character writer walks `ai_Chars` (0x21c bytes) and the original keeps `edi = ai_Chars + 2`, writing
  the 2-byte id as `[edi-2]` and the rest as `[edi+0x20a]` etc. Writing that bias into the C does not work:
  with `c = (char *)ai_Chars + 2` or a separate `extern char ai_CharKinds[]` view at 0x770b82, VC6 folds the
  `-2` back into a symbol addend and emits `mov esi, offset ai_Chars` + `[esi+0x20c]`. With the unbiased source
  (`c = (char *)ai_Chars`, fields `c+0x20c`, `c+2`, ...) VC6 produces `mov edi, offset ai_Chars+2` and the
  original's `[edi-2]` / `[edi+0x20a]` by itself (0x51a4b0). Write the natural indexing, not the bias.
- The same C shape can choose rep stosd or unrolled `mov DWORD PTR [esp+X],0` for a small zero local: a
  0x1c-byte zero buffer in the original (0x51ad80 at 0x51aea1) is 7 immediate stores, while `int zero[7] = {0}`
  (mov + rep stosd ecx=6), `char zero[0x1c]` + memset (rep stosd ecx=7), a zeroing loop (rep stosd), two arrays
  and a `struct { int a..g; } = {0}` (ebp) all gave something else here. The zero value and the written bytes are
  identical, so this is a vargen shape question (the stored bytes are the same; only the instructions differ).
- An `unsigned short`-returning callee gives different code with the local's type: assigned to a `short` local the
  value stays in ax (16-bit test / cmp), assigned to an `int` VC6 zero-extends it (`and eax,0xffff` right after the
  call) and tests the full register. The original's mask is the tell for the int local (snd_PlayWorkerClick,
  f3snd.c 0x4c2c70). The same function reads the character record through `unsigned short *` (zero-extension:
  `mov ax,[edx]` under an eax already masked, `xor ecx,ecx / mov cx,[edx]`); a `short *` would emit `movsx`.
- A wrong string literal in a strcpy can leave a function at `draft` with only inline-copy size differences: the
  original copies `mov ecx,<n>; rep movs dword` + tail, so the length jump is the literal length. Read the
  original string from .rdata at the `mov esi,<addr>`; `uv run link`'s byte compare does not see it (the literal is not
  a located data symbol). f3snd.c 0x4c28a0: three bank names were guessed wrong (GEHILFEN, ALCHIMIST, HANDWERK),
  fixing them made the function byte with no other change.

- Removing declarations from a header every unit includes flips byte functions in TUs that do not even use
  them: deleting the two d3_Unk4550e0 / d3_Unk4554b0 prototypes from d3_engine.h (to include d3_mir.h's
  D3MIRROR * versions) turned d3_Unk4550e0 from byte into 8 bytes off at +0x1be and d3_LoadScene44be60 into
  3 bytes off at +0x27f. Keeping the original header lines and routing the new view through
  `#define name name_mirview` around the conflicting include (d3_mir.c's pattern) kept 2503 byte matches.
  A header edit is safe only if the member/declaration list on the include path stays literally the same.
- A Q4 macro can stay byte-identical by moving its cast into an inline accessor: D3_OBJBITS(o) was
  `(*(D3OBJBITS *)&(o)->state)`; now an `__inline D3OBJBITS *d3_ObjBits(D3OBJECT *)` returns the cast pointer
  and the macro is `(*d3_ObjBits(o))`. VC6 inlined it to the same bitfield RMW (no byte changed anywhere).

- An anonymous union of three D3VECTOR views at D3OBJECT / D3OBJX 0x90 (`lightParams` / `angles` / `vec`)
  changed no layout but flipped d3_CloneObject (0x43d180) from byte to nonmatching - the same effect as the
  D3PTRI 0x26 / D3TEXTURE 0x68 unions. The field stays a plain D3VECTOR
  and the light code reads it through `D3_L()`; a shared-header field whose TUs include byte functions must
  not become a union without re-checking those functions.
- The original's inline failure epilogues (`ja 0x…; pop edi; pop esi; pop ebp; xor eax,eax; pop ebx; ret`
  after every `fwrite` check) come from the check being an early-return *statement*: write
  `if (vfs_fwrite(...) <= 0) return 0;` (the LS_W macro) at statement level. Wrapping the whole body in
  `if (vfs_fwrite(...) != 0) { ... } return 0;` makes VC6 merge every failure return into one shared end
  block (`jbe <end>` from each site). Same operations, 361 code hunks in 0x51b910 and 0x51c450 -> the
  early-return rewrite leaves the two loops there and makes 0x51c450 behaviour.
- VC6 re-centres a loop's induction variable on the middle accessed address by itself, whatever the source
  pointer type or spelling (char* byte offsets, int* elements, a plain int address, for/while/do-while all
  give the same shifted base): 0x51b910 `edi = p+4` against the original `edi = p`, 0x51c2f0 `esi = record+0x14`
  in the original but `record` in ours, 0x51c870 `esi = p+0x68`. When the original keeps the loop variable
  untouched, the base bias is a vargen shape (do not chase it with smaller rewrites).
- A record walk written as an index (`r = base + i * stride`, `i++`) gives the original's loop shape where an
  `off` accumulator does not: the index form puts the induction variable's init in the preheader after the
  guard (`xor ebx,ebx` after `jle`) and increments `i` before `off`; the accumulator form initialises before
  the guard and emits `add off` / `inc i` in the other order (0x51d3d0 went byte with the index form alone).
- A version/limit comparison keeps the source's constant, not just its truth value: the original's
  `cmp v,0x10033; jl` is `v >= 0x10033`, not `v > 0x10032` (the Ghidra pseudocode's `0x10032 < v` form gives
  the right test with the neighbouring constant and different bytes). Same for `0x1005a` vs `0x1005b`
  (0x51c950, 0x51cee0). Write the constant the compare uses.
- An unused local is not allocated, and a dead store to a dummy array is removed together with its slot - a big
  original frame is not a dummy buffer. Compute it from the slots the code touches: 0x4c2f30's 0xbc frame is
  x (4 bytes at R-0xbc) + `unsigned out[0x2e]` (0xb8 bytes at R-0xb8..R-9); the callees only use out[0]/out[1].
  Locals take the frame in declaration order from the lowest address, so x was declared before out.
- A local the original stores to its slot and reloads before every use can be reproduced with `volatile`
  (0x4c2f30 x: mov [slot],1 then mov eax,[slot] at each use). Matching trick only; comment it so it is not
  cleaned. Combined with the right out[] size it took the function from 55 to 6 differing asm lines.
- `unsigned short r = util_Random(n);` keeps r 16-bit in a register and masks at every 32-bit use
  (`and ebx,0xffff` before each push); `unsigned int r` zero-extends once after the call (mov bp,ax +
  and ebp,0xffff) and pushes the full register. The original's variation randoms are 32-bit (0x4c2f30 r in ebp,
  0x4c2d20 keeps it in ebp too).
- `f += ai_Unk50c550(...)` with f an int and a float-returning callee compiles to call / fiadd [f] / call
  __ftol per iteration (the int is added into the float, then truncated once); `f += (int)ai_Unk50c550(...)`
  truncates first and has no fiadd (0x4c2d20, 287 -> 178 asm lines).
- A nested call as the first argument of another call makes VC6 push the outer arguments early from scratch
  registers; the original's callee-saved register and push order come from evaluating the inner call into a
  temp first: `smp = sb_GetSample(...); d3snd_Attach(smp, o, ...)` (0x4c30e0, 33 code hunks -> byte).
- A typed view of a packed original record in a shared header needs #pragma pack(push,1): G16BLDVIEW
  (`char pad61[0x61]; char *object;`) puts object at 0x64 without it, so the byte match broke ([eax+0x64] vs
  [eax+0x61]) and the used unit got 2 asm lines worse until the pragma was added.
- A 0xffff sentinel is a 32-bit variable initialised with 0xffff (`mov ebx,0xffff`, `cmp ebx,0xffff`), not
  `w = -1` with an int (or -1 with a signed short: the pushes then sign-extend). For an unsigned word loaded
  from memory, zero-extend into a 32-bit local so the compares and pushes stay 32-bit (0x4c2d20).
- The same source meaning with slightly different shapes: two arms with a shared `if (call(...) && out[1] == 0)
  x = 1;` are not tail-merged by VC6; the ternary `special ? f(...) : g(...)` in one `if` gives the original's
  single shared test tail (0x4c2f30).
- (match-base 2026-10-24, base-3) An anonymous union member added to a struct in d3_engine.h (D3SURFTEX.flags,
  `union { int all; unsigned char b[12]; }`, layout unchanged) flipped d3_Unk4550e0 (d3_mir.c, byte) by 8 B at
  +0x1be. Splitting it back into plain int + byte fields was byte-neutral, and so were the same session's other
  D3SURF/D3MESH member changes (D3SURF.unk18 int -> `D3OBJECT *owner`, D3MESH.unk0b0 -> `frameStamp`, D3MESHSUB
  three ints -> `D3TEXHANDLE *tex` / `D3OBJECT *light` / `D3VECTOR dir`) and the renamed D3SURFTEX fields. So on
  the d3_engine.h -> d3_mir.c include path the union declaration is the perturbation, not the member renames or
  the member-set change; try the split plain fields first and keep a union only if the bytes stay clean.
- A declared extern symbol loads where `*(T *)0xADDR` does not: unk_58bbd0.c 0x58f980 with
  `ai_Unk438d40(U_I(0x5fb5b0), ...)` loads the scene at the call; with `extern struct D3SCENE *d3_DefaultScene`
  VC6 hoists `mov edx,ds:X` into the prologue (before the inlined memset) exactly like the original. The
  function went byte with that one change.
- Comparison operand order from the source: `(unsigned char)b <= i` emits `cmp edx,esi; jg`, `i >=
  (unsigned char)b` emits `cmp esi,edx; jl`; the original had the latter (0x58f980).
- The same call written in both branches with constant arguments (`if (c) f(x, 0); else f(x, 1);`) pushes the
  constants (`push 0x0` / `push 0x1`); `v = c ? 0 : 1; f(x, v)` materializes v in a register and pushes it
  (unk_58bbd0.c 0x58f980). Hoisting `v = list[i]` before the branches keeps one array load for both.
- `#pragma intrinsic(strlen)`, placed before or after <string.h>, does not inline strlen in VC6 SP5+PP /O2
  (the original's `or ecx,-1 / repnz scas / not ecx / dec ecx` inline needs /Oi for the TU; no per-function
  switch found).
- A struct view whose member follows a pad to an odd offset must be pack(1): with pack(2) VC6 aligns the
  `unsigned short owner` after `char pad0[0x27]` to 0x28 (AIBLD), one byte off the original's [obj+0x27].
- (match-base 2026-10-24) A typedef + cast macro added to d3.h (`D3OBJPOSE`/`D3_POSE`, nothing else changed) flipped
  d3_CloneObject (d3_engine.c, byte) by one instruction pair (`mov esi,[ebp-4]` vs `mov ebx,[ebp+8]` scheduled
  around it). Moving the view into d3_tools.c restored it. Same lesson as the anonymous-union entries above:
  anything added to a shared header can reschedule a byte function in another unit; keep per-unit views in the
  unit's .c and run the full check.
- (match-base 2026-10-24) `max(a, b)` argument order steers the FPU compare: `max(r, d)` compares the running
  maximum in st(0) with d in memory (`fcom [d]`), `max(d, r)` loads d first (`fld d; fcomp st(1)`). For
  d3t_GetBoundingSphere `max(r, d)` removed the compare hunk but changed 17 bytes elsewhere (unitdiff 978 -> 995),
  so it was reverted; the remaining 9-byte difference (fsqrt before the radius test) is still open.
- (match-base 2026-10-24) An unsigned guard spelled `x` compiles to `test; je`, `x > 0` to `test; jbe`
  (d3fl_UpdateTiles). Same value, different jcc spelling in the diff.
- (match-base 2026-10-24, base-1) A pointer built from a symbol plus a bias is not folded: `char *p =
  (char *)externdata + 2;` emits `mov reg,addr; add reg,2` where a symbol declared at the biased address loads
  with one `mov reg,addr`. ls.c's gm_PlayerRecordBytes (0x7d5f82, the +2 byte view of gm_PlayerRecords) was
  needed to keep 0x51a1a0 and 0x51db00 at their original shape.
- A constant-trip pointer loop is strength-reduced into an absolute end compare only in the do/while pointer
  form: `do { ...; p += 0xac; } while ((int)p < 0x7d6a42);` gives `add esi,0xac / cmp esi,0x7d6a42 / jl`
  (byte for 0x51db00), while `for (k = 0; k < 16; k++)` keeps a counter in a stack slot (a 4-byte larger frame)
  and `for (;;) { ...; if (p > end - 1) return 1; }` puts the return block after the loop body.
- A global compared with a constant: reading the global at each check emits `cmp DWORD PTR ds:X,imm`; a local
  `v = ls_Version;` used across calls lives in a callee-saved register (ebx) and reloads at other points. In
  0x51db00 dropping the local and reading ls_Version at every check reproduced the original's
  `mov eax,ds:0x623650 / cmp eax,imm` (a reload after each call, CSE inside a block) - 287 instructions became
  equal.
- The version-bound spelling: `x > 0x10027` emits `cmp x,0x10027 / jle`; the original's source is
  `x >= 0x10028` (`cmp x,0x10028 / jl`). Same for the setcc form: `(v > 0x10045)` gives `setg`,
  `(v >= 0x10046)` gives `setge`.
- An if/else body order is source order: `if (v >= 0x10056) { read } else { store }` puts the read inline and
  jumps over the out-of-line store; the negated condition puts the store inline instead (0x51db00's 0x5e arm).
- Q4 without an includable struct: a typed pointer view (`unsigned short *fl = (unsigned short *)(ent + 0x8c);`
  then `*fl &= 0xdffb;`, `unsigned char *flagByte = ...; *flagByte |= 0x20;`, `int *ref = (int *)(ent + 0x12c);
  *ref = v;`) passes Q4 (no dereferenced cast) and keeps the bytes - but the assignment placement matters:
  declaring / assigning the views right after the allocation hoisted the `lea ebx,[esi+0x8c]` and swapped ent
  and f between edi / esi (52 regs + 28 code hunks, behaviour fell to draft); assigning each view immediately
  before its first use restores the original (0x51e750, behaviour + readable).
- An extern at the address of a loop bound does not fold to the literal: `while ((int)p < (int)gm_PlayerRecordsEnd)`
  with the symbol at 0x7d6a42 gave 84 differing asm lines where the literal 0x7d6a42 is byte-identical (so Q3
  and byte identity conflict there; recorded as @deep on 0x51db00).
- The per-row pixel pointer `p = pix + ((r + y) * pitch + x) * n` is strength-reduced by VC6 /O2 into a base
  pointer advanced by pitch*n per row (the pitch*n sits in a slot). The original 0x47a460 / 0x47a7d0 /
  0x47ab30 / 0x47ad40 keep the imul per row. Removing locals, moving the pointer increment into the for
  header, or dropping the explicit (pitch & 0xffff) mask changed no byte (0x47a460: 774/912, 446 code hunks
  before and after). The trigger is not the source spelling tried so far; treat as deep.
- A parameter slot that is dead after the prologue is reused by VC6 for a loop counter: 0x47a460's copy loop
  keeps its row counter in the w slot, 0x47b350/0x47b640 keep acc in the x slot and m in the y slot. The
  source does not have to assign to the parameter for the slot to be reused; do not read `[esp+arg]` as the
  parameter without checking the writes.
- `(*run >> 1 + acc) % n` and `acc++; acc = (*run >> acc) % n` compile identically (the remainder overwrites
  acc), and both give the original's `inc ecx` + `shr eax,cl` at 0x47b350/0x47b640: the shift count is acc+1,
  the precedence trap is harmless to the bytes.
- A 16-bit compare block ending in `jb` can jump TO the reset block, not past it: in 0x47a460 / 0x47a7d0 /
  0x47ab30 `cmp ax,[minx]; jb reset` means reset when maxx < minx, which matches the C
  `(unsigned short)minx > (unsigned short)maxx || ...`; check the target before "fixing" the condition.

## match-base 2026-10-24 (base-3 worker, d3_engine.c)

- `memset(&f, 0, sizeof f); f.b[2] &= 0xfe;` reproduces the original's `mov [f],0; mov cl,[f+2]; and cl,0xfe;
  mov [f+2],cl` (d3_DeflateObject, 0x43da00). The inlined memset is expanded late, so VC6 cannot forward the
  dword store into the following byte load; `*(int *)&f = 0` and a union-member store both let it fold the AND
  to one byte store. Same idiom already byte in ch.c (ch_UpdateFades, ch_StartFade).
- Removing two prototypes (`void d3_Unk441db0(void *)`, `void d3_Unk441ea0(void *)`) from d3_engine.h flipped
  d3_Unk4550e0 (d3_mir.c, byte) to behaviour with an instruction-order hunk (`mov esi,[ebp+8]` and
  `mov edi,[ebp-4]` swapped at two joins), although d3_mir.c calls neither of them; putting the two lines back
  (as d3fl_FreeTiles / d3fl_Inflate) restored the bytes. Same include path and phenomenon as the D3SURFTEX union
  note above: a declaration-set change in d3_engine.h perturbs d3_mir.c. Judge such a header edit with the full
  ledger check, not with unitdiff.
- clang with -fms-compatibility treats `void f(struct TAG *);` written before the tag is declared as a different
  type from the later `void f(TAG *);` (lint: conflicting types), even when `TAG` is a tagged typedef. A
  `struct TAG;` forward declaration in the header (the pattern in d3_interface.h) makes the two compatible.

## match-base session 2026-10-24 (base-4 worker, shard 4/6, session 2)

- A wrong sprintf destination shows up as a frame difference, not as a call difference: 0x58fc40 wrote the
  debug text into `out` (the int * parameter) where the original has an unused `char text[0x100]` local. The
  0x100-byte `sub esp` was 73 of the 103 code hunks; the real destination is a local that is otherwise dead.
  Read the sprintf argument setup (`lea edx,[esp+X]` into the destination push) before believing a parameter.
- Local float triples: a `float pos[4]` of which only [0] and [2] are assigned reproduces an original frame
  with two holes (the [1] / [3] slots) and keeps the position alive as an array. Separate `float x, z` locals
  make VC6 reuse the dead parameter slot for another local (`best`), shrinking the frame by 12 bytes
  (0x58fe10: 0x18 vs 0x24). A local `float x[3]` is not the same thing: the frame grew but the holes sat
  elsewhere (0x20). A dead store to a separate `float y` is eliminated and takes no slot.
- `d < 100.0f` vs `d < 100.0`: the double literal is `fcom QWORD PTR ds:<const>`, the float one
  `fcom DWORD PTR [slot]`; 20 -> 4 fpu hunks in 0x58fe10 (same as the d2 lessons: keep the literal's type).
- A shared early-return block: two `return 1` statements in one function are NOT merged when the second
  follows an inverted test (`if (x == 0) return 1;`); the original layout (one return-1 block after the body,
  `je mismatch` / `jne mismatch`) comes from a positive body `if (x != 0) { ...; return 0; } return 1;`
  (0x58fba0: 26 -> 7 code hunks). A `goto mismatch` target is laid out before the body, not after it.
- A value that is live across a call (0x58fc40's `owner = rec->object;` held to the d3_PointToWorld call)
  costs a 4th callee-saved save; the original rematerialises the load into eax after the call and keeps rec in
  ebx. If the source reads `rec->object` twice (two short live ranges) VC6 gives the first callee-saved slot
  to a different variable (scene) instead - the two forms are 30 vs 86 code hunks, neither matches; register
  allocation, not source.
- Register roles follow the source statement order at the top of the function (the earlier `oldx`/`oldy`
  lesson): `obj = 0; best = 1e8f; x = ...; z = ...;` reproduces the original's store order; `x`, `z`, `best`
  gives the same instructions in another order (asmdiff `order`).
- A short global used in several comparisons of one `if` and again after a call: the original (0x477000) loads
  it into a 16-bit register (di/bx) *before* the first comparison, sign-extends with `movsx`, and reloads it
  after the call (di_ClearEvents) because the callee may change the global. Writing the globals directly gives
  that cache for one of the two (bx) but not the other; `short` locals give both early loads but no reload.

## Floating point max against a memory operand (d3t_GetBoundingSphere, 0x411eb0)
- `max(r, d)` with a plain float `d` gives `fld d; fld st(1); fcompp` (both on the FPU) or keeps the sum in a free
  FPU slot (`fstp st(3)`) and moves `r` to memory when a second value is live below (the centre y kept on the stack
  across the loop). The original stores the sum and compares it as a memory operand: `fstp [d]; fcom [d]; and
  eax,0x4100; je` (skip when r > d), the shape a byte-matched sibling shows with a shallow FPU (d3anim_MaxRadius,
  0x434d00, `float d; r = max(r, d);`).
- `volatile float d` forces the store and keeps r on the FPU, but then VC6 loads d for the compare
  (`fld d; fld st(1); fcompp`); a plain local lets the store disappear. The shape that reproduces both is a local
  union: store through a `volatile float` view, compare through a plain `float` view
  (`union { volatile float sum; float dist; } d; d.sum = ...; r = max(r, d.dist);`).
- Address-taken (`(void)&d`) does not force the memory form (the compiler drops it); a cast to strip the volatile
  (`*(float *)&d`) does the same as the union.
- A local buffer's real size is visible in the frame even when only a prefix is used: 0x520000
  (`tmp[0x400]` in the draft) has the original's `mov eax,0x1008; call __chkstk`, so the buffer is
  0x1000 - the frame size (and the __chkstk call for > 4 KB) is the evidence. 131 -> 95 code hunks.
- One loop can use two base pointers: 0x51f080 keeps the biased `s = 0x7e3f88` in a stack slot (reloaded
  into ebx every iteration) and `p = s - 0x48` in edi; the reads up to s+0x19 use s, everything from
  p+0x104 on uses p. Writing `p = s - 0x48;` as its own pointer reproduces the mixed displacements
  (180 -> 94 code hunks). Writing every read from the slot base instead re-bases the whole loop and is
  worse; and the loop bound stays on s, not on p (a p-based bound reads one slot too many).
- A `char` local the original keeps in memory shows as a byte store followed by a **dword** reload of the
  same slot (0x51e900's `col`: `mov BYTE PTR [esp+0x1c],al; ... mov ecx,DWORD PTR [esp+0x1c]`); declaring
  `int col` gives movsx/add in eax, `char col` at least the byte store. The 4-byte slot it occupies is what
  pushes the following locals and the buffers up (frame 0x2a4 vs 0x2a0).
- The inline-vs-shared return-0 epilogue stays open for ls.c: in 0x51f720 (31 failed reads) and 0x520200
  the original ends every failure with `pop edi; pop esi; pop ebp; pop ebx; ret` inline while VC6 merges
  ours into one shared block reached by `je`; the reads are `test eax,eax; jne next` (the `== 0` form), so
  it is not the `<= 0` / `ja` style known from the writers. Open lead for match-deep.
- A sum tested in an `if` and kept afterwards: `w = s->w; if (x <= d2_ClipRight && w + x >= d2_ClipLeft) {
  right = w + x; ...}` reproduces the original's `mov edx,[eax+6]; lea edi,[edx+ecx]` with the load hoisted
  into the branch shadow (0x47bd70, matched). `(right = s->w + x)` inside the && folds the load into right's
  register (`xor edi,edi; mov di; add edi,ecx`), and hoisting `right = ...` before the if computes the sum
  before the branch — both leave code hunks.
- `if (u != 0)` on an unsigned int gives `test/je`; `if (0 < u)` gives `cmp/jbe` (0x47c0e0, the pixel loop).
## match-base session 2026-10-24 (base-4 worker): stack, floats, typed views

- An automatic `__chkstk` (0x5de9c0, `mov eax,frame; call __chkstk`) is emitted for any large frame; a
  source-level call to 0x5de9c0 in a draft is a duplicated stack probe and shows as an extra `call`
  before the callee-saved pushes. Remove it (0x5d33b0's sibling 0x5d34b0, 268 -> 259 hunks).
- The stack packer reuses a dead *parameter* slot for a loop counter (0x5d44c0 / 0x5d33b0). Giving the
  counter an explicit `i = 0;` *before* the last use of the parameter makes the live ranges overlap and
  forces the original's own frame slot; a `for (i = 0; ...)` initialised after the parameter's last
  read gets the parameter slot instead (frame 0x24 vs 0x28, 62 hunks). The loop shape the original used
  was `if (i < n) { out = ...; do { ...; i++; } while (i < n); }` (Ghidra's if + do-while is a hint).
- Integer-to-double conversion forms: signed int -> `fild DWORD`; **unsigned int** -> zero-extend into
  a stack qword (`mov [esp+4],0; mov [esp],eax; fild QWORD`). A global read as `*(int *)` where the
  original zero-extends is an `unsigned int`.
- Float constants keep their type: `x * 0.00625f + 0.25` (int * float, then + double) gives
  `fild DWORD; fmul DWORD <float>; fadd QWORD <double>`; writing `0.00625` (double) gives `fmul QWORD`.
- A source `g - 0x40` emits `sub edx,0x40`; the original's `add edx,0xffffffc0` is the source spelling
  `g + -0x40` (a negative literal in the expression).
- Typed views of another unit's struct (d2.h's D2OBJ here, because d2.h's vfs_Root / vfs_Tfopen clash
  with vfs.h): the local view struct must be padded to the **full** size of the real struct. A view
  `{ char pad[0x78]; int input; }` is 0x7c bytes, so `d2_Objs[id]` scales by 0x7c instead of 0x2ec -
  it compiles, links and silently reads another record. Pad to the real sizeof (0x2ec) and document
  the view as type debt.
- Ghidra's `&DAT_xxxx` vs `DAT_xxxx`: `&DAT_00fa6030 + i*0x160` is an **array** base; `DAT_00fa6030`
  would be a pointer variable. `*(int *)(DAT_00fa6030) + i*0x160` (what a U_I(U_I(0xfa6030)+..) draft
  writes) is wrong unless the array starts with a pointer.

## match-base session 2026-10-25 (base-1 worker): missing arguments, block tails, unsigned sizes

- A call to a close / file helper without its handle where the original pushes one (`push edi` at 0x520a9e
  before 0x430aa0) is a missing argument, not scheduling. Adding it can still read WORSE when the original
  hoisted the push above the preceding `jne` and VC6 puts ours at the call site (0x520b80 475/114 ->
  469/120): fix the argument, but the hoisting is a separate shape for match-deep.
- A global at the tail of a byte block has the block's width: cm.h declares `char ai_Unk75e130[14]`, so
  its last field 0x75e13c is a `short`. An `int` view writes 4 bytes over the next block and compiles to
  a 4-byte store; the original loads and stores a word (`mov dx,WORD PTR ds:X`, 0x520cf9 / 0x520d14).
- Sizes and byte counts the original compares unsigned (`jae` / `jb`, 0x520ba1) are unsigned globals;
  a signed declaration gives `jge` and a permanent code hunk.
- A 16-bit field compared with -1 materialises -1 with `or ecx,-1`; the original's `mov ecx,0xffff`
  (0x520de9) is an unsigned-short test (`*(unsigned short *)p != 0xffff`).
- Never keep a `v = global` local for version gates: every check then compares the local (edi), while
  the original reads the global at each site (`mov eax,ds:X`, `cmp DWORD PTR ds:X,..`) - removing it took
  0x520b80 from 114 to 74 differing asm lines. The frame difference (0x100 vs 0xfc) is a separate item.

## match-base session 2026-10-25 (base-1 worker, continued): field renames and typed clears

- A 4-byte flags struct (`unsigned char b[4]`) gets an anonymous-union dword member (`unsigned int all`)
  when the code clears it with `*(int *)&flags = 0`: `flags.all = 0` is the typed form, compiles to the
  same dword store and clears Q4. It made 8 functions readable without a byte change.
- Renaming a struct field in a shared header is found exactly by the compiler: rename the header member,
  build, and every use of that struct's field fails with C2039. Source files with `#line` directives
  report the original file's numbering - map compiler line -> source line through the directive chain
  before editing (d3_tools.c's 3016 is source line 2030). Unrelated structs may have a same-named field
  (D3TSNOW.unk10): the C2039 list selects only the right ones.
- A function that is the tail of a byte block can be duplicated under two source spellings to dodge a
  wrong shared-header prototype (0x447190: `ai_Unk447190()` K&R in cm.h/gv.h and `void d3_Unk447190`
  in d3.h, while the definition returns char and d3_interface.c works around it with a #define). Merging
  them needs the header declaration fixed, the K&R declarations dropped and the int call sites cast -
  a wide cleanup, not a rename alone.

## match-base session 2026-10-25 (session 2): VC6 register/IV levers found in ls.c

- Signed division by a power of two: write `len / 0x80`, not `(len + ((len >> 31) & 0x7f)) >> 7`;
  VC6 emits the original's cdq / and edx,0x7f / add eax,edx / sar. Signed modulus `cnt % 16` with an
  int gives and 0x8000000f / jns / dec / or / inc; `cnt++ % 16` puts the increment in the loaded
  register and stores it before the branch, as the original does.
- Block-scope locals are memory-resident, function-scope locals are register candidates: moving `sent`
  into the `if` block spilled it to the dead parameter slot ([esp+0x18]) and freed ebx for the pointer -
  exactly the original's split. A block-scope initializer is placed after the branch.
- The statement order of two increments is code: `sent += 0x80; p += 0x80;` folded p into buf+sent
  (sent in ebx, lea per call); `p += 0x80; sent += 0x80;` produced the original's two adds and the
  whole loop matched (183 -> 8 differing bytes in one edit). Try the order swap on any loop whose two
  induction variables the original keeps apart.
- `*(unsigned short *)x != 0xffff` materialises 0xffff in a register (`mov edi,0xffff; cmp word [..],di`);
  `*(short *)x != -1` uses the immediate. The original's form shows which the source used (0x5206b0,
  0x520b80).
- A Q3 symbol bound can cost the induction-variable base: with the literal `0x74db40` VC6 kept pb, with
  `(int)he_QuickList` it re-based the IV to pb+0x70. Keep the int cast (pointer compares give jb where
  the original has jl) and record the re-base for match-deep.
- Scoping out-parameters into the loop that uses them shrank 0x5206b0's frame 0x14 -> 0xc and removed
  the frame hunk (the a/b/c slot offsets then still differ by the allocation order).
- A typed struct view of a 10-byte record array (packed with `#pragma pack(1)`) still re-bases the loop
  (186 asm lines vs 161 for the int view): the int-pointer view with a named symbol plus a typed
  `unsigned char *kind` assigned at the loop head keeps the code byte-neutral and clears Q4.
- Q2's placeholder regex flags `unkNN` *field* names used in a body, so an unknown field cannot be
  typed without a meaning; keep one raw access and escalate the field-name gap.

## match-base 2026-10-25 (base-1 worker)

- A Q3 pointer bound: `while ((int)c < (int)(ai_Chars + 768));` keeps the original's signed `jl`;
  `while (c < ai_Chars + 768)` gives the unsigned `jb` (only the jump opcode differs).
- Q4's `bi = *(int *)&best;` (a type-pun that forces VC6 to spill `best` and reload it as int) has a
  byte-identical union form: `union { float f; int i; } best;` with `best.f = 0.0; ... bi = best.i;`.
  The plain `bi = 0;` lets VC6 fold both and the spill disappears.
- A K&R declaration after a definition with another return type is a clang E error once both carry the
  same name (int f(); against char *f(void)): drop the redundant declaration, the definition above it
  is the prototype.
## match-base session 2026-10-25 (base-2 worker, shard 2/6): ahm.c

- A 16-bit parameter in the original TU: 0x42ec00 masks its argument (`test si,si; and esi,0xffff`) and
  the original caller loads a global count with `mov cx,WORD PTR ds:...`. That only happens when the TU
  saw `unsigned short f(unsigned short)`; with an `int` prototype the same source gives
  `mov eax,ds:...; and eax,0xffff; push eax` (code hunk) and `movzx` variants. Keep the 16-bit view as a
  local @original declaration in the .c; changing the shared header adds one lint conflict per other
  header that disagrees (six here).
- A redundant mask is one extra `and eax,0xffff`: `t = util_Random(8); ... ahm_Tab[t & 0xffff]` masks once
  for the unsigned-short return conversion and once for the explicit `& 0xffff`; `ahm_Tab[t]` matches.
- The loop counter's spill position is the loop form: `n = count * 2; if (n > 0) { do { ... } while (n != 0); }`
  spills n before the entry branch (`mov [esp+0],eax` before jle); `for (n = count * 2; n > 0; n--)`
  spills it after the prologue pushes (`mov [esp+0x10],eax`), which was the original's place - byte.
- Where the decrement sits in a counted while loop is code: `while (k != 0) { ...; k--; pos = ...; }`
  schedules dec before the modulo's cdq as the original does; the for-update `k--` was scheduled after
  the idiv (one code hunk). Moving the statement into the body matched.
- Re-derive a struct chain at the use instead of a local pointer: `((AHMCHAR *)c)->obj->rec->slots[i]`
  in both the test and the return reproduces the original's macro-style reloads (the entry base stays
  in eax); a `p = ...->slots;` local hoisted the table base into ebp before the loop and kept it there.
- A global read as a dword and written as a word gets a union (`union { int seed; unsigned short seed16; }`)
  instead of `*(unsigned short *)&g`: both widths keep their code and Q4 clears.
- Compare direction: `if (count <= index)` emits cmp eax,ebx / jg; the original's cmp ebx,eax / jl is
  `if (index >= count)`. A Q3 symbol bound can keep the literal's `cmp reg,imm` with
  `(int)s < (int)&count`; the natural pointer form `s < arr + 3` was WORSE (extra address arithmetic).
- A pointer-to-record field passed to a tail call: the original `mov ecx,[edx]; mov [esp+4],ecx; jmp`
  passes the record's first int, so the C argument is `rec->id`, not the record pointer; the pointer
  form compiles to a single `mov [esp+4],edx`.
- VC6 reassociates a subtraction chain and a mixed -/+ chain: `w->w - r->w - l->w` and
  `w->w - l->w - r->w` compile to identical bytes (d2_DrawButton 0x46c039), and
  `w->w - r->w + w->x` and `w->x + w->w - r->w` are identical too (0x46c091). Source operand order is
  therefore no lever for the d2 member-load-order family; the leftover hunk is register allocation.
- A callee with a 5-parameter definition whose call sites push 2 / 4 / 5 arguments is a K&R victim in
  the original: keep the full prototype in the unit header and tag the short call lines with
  `/* @original: this call site pushes N arguments (0xADDR: push ...) against the N-parameter definition */`
  (d2_RadioSelect, d2_SetObjValue) - that is what clears Q7 without touching the bytes.
## match-base session 2026-10-25 (session 3): symbol vs literal, byte bitfields

- **A named extern and its address literal are not the same code** (0x5d5e90): the same expression
  `x - 0x40` compiles to `sub edx,0x40` when x is `*(int *)0x6f3610` (the address folded into the
  expression) but to `add edx,0xffffffc0` when x is a declared `extern int`. The original has the add,
  so the Q3/Q4 fix (typed extern) also closed the last code hunk - try the symbol form before any
  spelling round on a `sub`/`add` constant difference.
- **Byte bitfields steer the load shape** (0x5d5f10): `struct { unsigned char a:8, b:8, c:8, d:8; }`
  makes VC6 load the containing dword once (`mov eax, DWORD PTR [g]`) and extract the second byte with
  `xor edx,edx; mov dl,ah`, while plain `unsigned char` fields emit one byte load per field. Both the
  wide load and the ah extract of the original came back from the bitfield spelling. The same struct's
  later fields are still byte loads, so one struct can mix both shapes.
- A +1 row pointer (`rp = src + 1; p = rp; p[-1] / p[0] / p[1]`) is rebased by VC6 to the base with the
  +1 folded into the three displacements (mov esi,edx; [esi+2]/[esi+1]/[esi]) even where the original
  keeps lea esi,[edx+0x1] and [esi-1]/[esi]/[esi+1]; `rp = src; rp++;` changes nothing (0x478bd0 /
  0x478c50, both escalated). With a function call taking the three pointers the rebase also moves the
  row-pointer slot into the dead dst parameter slot instead of the dead w slot.
- `(char)flags & 8` on an int parameter is folded to `and reg,8`; a `signed char` local
  (`f = (char)flags` or `f = flags`) is propagated away too. A signed char *parameter* does give the
  movsx bl, but VC6 then saves a 0x108 frame with byte reloads (0x478d19 `mov al,bl; and al,1`) and the
  callers compiled against the int prototype regress - not the answer when the original loads the
  parameter with a dword. The original's `movsx eax,bl; and eax,8; shl eax,8; or al,0x40` (0x478df1 /
  0x478e1e) has no found source lever; match-deep.
- Branch polarity is visible in the emitted je/jne: the original wrote `if (dstLocked != 0)
  d2_LockSurface(dst); else d2_UnlockSurface(dst);` (test; je to the unlock block, fall through to the
  lock call); the `== 0` spelling costs a code hunk at the same test (0x47942a).
## match-base session 2026-10-25 (base-0 worker, shard 0/6): loop rotation, byte compares, frame padding

- **VC6 rotates a `while` search loop** (`while (p->idx != x) { p++; i++; if (bound) return v; }`): it peels the first
  test with the constant address folded (`mov ecx,DWORD PTR ds:amt_Table; and ecx,0xff`), puts the test at the bottom
  and the bound check at the top of the emitted loop. The original's search loops test at the top and keep the bound
  at the bottom; writing them `do { if (p->idx == x) break; p++; i++; if ((int)p >= (int)&amt_Table[N]) return v; }
  while (1);` reproduces that shape exactly (0x4e6360 went byte this way, then the same idiom on 0x4e6430 / 0x4e6680 /
  0x4e6710 took each from 45-115 differing instructions to 0-16). The bound compare must be signed: a pointer `>=`
  gives `jae`, the `(int)p >= (int)&array[N]` form gives the original's `jl` / `jge`.
- **A `char` parameter compared with an `unsigned char` field costs a movsx/movzx + register compare**; typing the
  parameter (or the field) `unsigned char` gives the original's `cmp BYTE PTR [eax],dl` (0x4e6680, and AMTMSG.office
  for 0x4e6710).
- **A local frame 4 bytes larger than the struct**: the original's AMTMSG local reserves 0x10 for the 11-byte record
  because the 15-byte message copy (G17B15) reads the trailing 4 bytes; declaring them (`char pad[4]`) closes the last
  four `sub esp` / `add esp` hunks (0x4e6680 byte).
- **Values computed before a loop and stored into a struct after it belong in locals**: the original keeps the two
  netIds in callee-saved registers through the search and writes the AMTMSG fields at the end; assigning the struct
  fields directly makes VC6 store them before the loop (0x4e6680; with the locals the stores sink and the field order
  becomes office / a / d / e / b).
## match-base session 2026-10-25 (base-2, shard 2/6): the 16-byte section padding, min as a ternary

- **The object's function section is padded to 16 bytes, and `uv run build` compares `len(mine)` bytes**
  (uv run build `compare`): a function whose code is 225 .. 239 bytes long is compared against the
  original's *next function* bytes, which diffclass reports as an unexplained `code` hunk (nops on our
  side, real instructions on the original's). ahm_ResearchProgress (0x4eafd0) sat at 49 differing bytes
  that way; shrinking it by 2 bytes (224 = the original's exact size) turned the same function into
  9 bytes / `order` only. Check the function's length before reading a tail code hunk: if our side ends
  in nops that the original fills with the next function, the fix is one instruction shorter, not a
  meaning fix.
- **`m = t; if (n < t) m = n;` vs `m = n < t ? n : t;`**: the if form stores `m` on both paths (two
  `mov [m],eax` around the compare); the ternary gives the original's single sunk store
  (`cmp; jge; mov eax,ecx; mov [m],eax`) - 0x4eafd0 and 0x4eb0b0 both went byte with it.
- **A range clamp written as `if (f < 0.0f || f >= 1.0f) return 0.0f;`** compiles to the original's
  layout: `test ah,0x5; jnp` (f < 0) and `and eax,0x100; je` (f >= 1) jumping to a return-0 block laid
  out *after* the computation, and NaN passing both tests (0x4eb0b0 went byte). The
  `if (f >= 0.0f && f < 1.0f) { ... } return 0.0f;` spelling gives the same conditions but the
  return-0 block inline and one more instruction.
- **The compare's operand order follows the source**: `r->slots[i].id == m->id` emits `cmp [ecx],edx`
  and hoists the right-hand load before the left-hand `lea`; `m->id == r->slots[i].id` emits
  `cmp edx,[ecx]` and swaps the register allocation (0x4ecd10 byte after the flip).
- **An early `return 0;` vs the return after the if**: ahm_LoadPrototypes (0x4eb480) needed
  `if (ok) { ...; return x; } return 0;` (the zero block out of line, after the tail `jmp`) instead of
  `if (!ok) return 0;` (the zero return emitted inline at the test).
- **A call pulled out of an if/else can fix the register allocation** (0x46d220, d2_DrawObjCaption): with
  a d2_DrawTextBox call in both arms (the literal reading of the source) VC6 emitted 150 instructions and a
  different prologue; setting the two differing arguments (x, y) in the `style & 4` arm and calling once
  after the branch gave 131 instructions and the original's shape at once (116 -> 71 differing). The same
  lever on 0x46c1d0 (d2_CenterObj: compute `int x` per arm, call d2_MoveObj after the branch) took 116 ->
  59 of 144 bytes. Before running a spelling round on a call that sits in both arms, try the shared call
  with the arguments assigned in the arms.
- **A typed cast instead of a raw store keeps the address expression** (0x46e710, d2_RemoveObj): the Q4
  fix `((D2OBJ *)((char *)d2_Objs + off))->id = obj;` compiles to exactly the same bytes as
  `*(int *)((char *)d2_Objs + off) = obj;`, while `o->id = obj;` lets VC6 reuse the live object pointer
  and regresses the whole function (105 -> 124 differing). When Q4 is the only failure, spell the store
  through a typed cast of the same expression rather than through the already-live pointer.
- A `void` function with an early `return;` whose last statement is a call tail-jumps it (0x5d7320:
  `jmp vfs_fclose`), and its failure path returns through the last call's eax with no `xor eax,eax`
  (0x5d7430: every return path is a bare `pop esi; add esp,0x7c; ret`). A source that `return 0;`s
  instead costs a code hunk per return path.
- The source if/else order picks the fall-through branch of a two-armed screen/game test: 0x5d7430's
  original tests `flags & 4` and `je`s to the *out-of-line* normal-game block, i.e. the network arm was
  written first. Writing the normal arm first emitted the inverse (jne) and a different block order.
- A call that appears before the file-scope prototype makes VC6 assume an implicit `int f()`; the later
  prototype then fails with C2040 ('differs in levels of indirection from' two signatures that print the
  same). Put the prototype block above the first use, not next to the function that needs it (ecmd.c,
  2026-10-25).
- Two parameters with the same use count get edi / ebp by the allocator's tie-break and the choice is not
  steerable from the source found so far: 0x479bf0 has r in edi, g in ebp; our r/g land the other way round,
  and declaration order, parameter types, the loop variable and the branch polarity all leave the same
  12/224 bytes (match-deep).
- Office searches in amt.c: the note's `do { if (p->idx == x) break; p++; i++; if ((int)p >= (int)&amt_Table[N]) return v;
  } while (1);` idiom reproduces the original's rotated search exactly (`cmp [eax],dl; je; add; inc; cmp end; jl`) -
  with `while (p->idx != x && p < end)` VC6 instead emits a bottom-tested loop with a duplicated test (+3
  instructions). Also byte right: comparing a message byte with AMTREC.idx needs both operands unsigned char (a
  signed char gives movsx + a masked int compare).
- The 0x18-byte record copy (rep movs 6 dwords) that follows such a search: keep the result record in two pointer
  locals (`p1 = &amt_Table[i]; p2 = &amt_Table[j];` and use p1->owner / p2->kind ...) - that alone took
  0x4e6c50 from 60 differing instructions to 0 (the indexed `amt_Table[i].owner` form makes VC6 recompute
  lea/shl/lea per use and keeps an extra address register).
- Backwards walks with a pre-decrement go as `for (i--; i >= 0; i--)`: the init `i--` is emitted before the
  callee-saved pushes and the loop-top test is a separate test/jl, where `i--; while (i >= 0)` fuses dec/js.
  Plain range guards: `if (col <= 6)` gives `cmp eax,0x6; jg`, `if (col < 7)` gives `cmp eax,0x7; jge` - the
  original used the former.
- A function whose early guards and the tail return the same value (`return 0`) shares one out-of-line return 0
  block in the original: written as `goto none; ... none: return 0;` the block lands at the end as in the
  original; plain `return 0;` makes VC6 inline a copy at each guard.
- Near-miss left for match-deep (0x4e6ec0 / 0x4e6f30): the loop-found bound check `if (n >= 42) return 0;`. The
  original branches to the copy with `jl` and keeps the return 0 in the fallthrough; VC6 (for us) always inverts to
  `jge <out-of-line>`. if-else polarity, explicit else and break spellings all left the same 4-5 hunks.
- Two sibling functions with the same source text can need different branch forms: the human-player if/else-if
  block written as `if (f(1)) {...} else if (f(0)) {...}` is 10 bytes better in 0x47edb0 (1618 -> 1608) and 10
  bytes WORSE in its variant 0x47f700 (1867 -> 1877, reverted). The surrounding register roles differ; measure
  the sibling instead of copying the form (2026-10-25).

## From round 2026-10-25 (base-2 worker, ahm.c)
- Address-taken float: `ai_Table[i].a = *(int *)&r;` forces `r` into a stack slot (frame +4, every array
  reference shifted); a union field (`union { float f; int i; }`) assigned as `.f` and read as `.i` keeps the
  value on the FPU and stores directly (`fstp [base+edi+4]`). The frame size told us (ahm_LoadIni 0x304).
- Table entries are indexed directly, not through `char *e = base + i * 12`: the original reloads the table
  base for every field store (`mov ecx,ds:0x6f0e2c; mov [edi+ecx*1+4],al`), which is what `table[i].field`
  compiles to; a precomputed `e` gives `[edi+4]` and differs in every store.
- A clamp that calls the same conversion again per use is a min/max MACRO, not an inline helper: count the
  calls in the original (four `atof` per check here). The constants tell the expression's type: QWORD
  `fmul`/`fcomp` + a DWORD `fstp` at the end is a double expression narrowed at the store - the source had no
  `(float)` cast. `min(max(x, 0.0), 1.0)` in double gave the original's exact size (1472 bytes).
- An unsigned-return declaration view flips `if (x <= 0)` from `jle` to `test eax,eax; jbe` (and `x == 0` is
  still `je`): the call sites show which view the original's translation unit saw (vfs_fread here).
- A 14-byte struct copy: pack(2) (or any 2-aligned struct, 3 ints + short) copies 4/4/4/2 as the original
  does; with default pack(8) VC6 makes the tail a DWORD read that overshoots (the original's word store).
- Switch body order is source order: to get the original's jump table and body layout the labels must sit in
  the original order (`default` first, `case 4` last here), and that also changes the surrounding allocation.
- A byte-pointer walk of an int array (`int win = (int)wn->objs; ... d2_Objs[*(int *)win] ... win += 4;`) and
  the typed `int *p = wn->objs; ... d2_Objs[*p] ... p++;` compile to the same bytes (d2_ResizeWindow,
  0x472800, 2026-10-25: 438/608 bytes and 29 differing asm lines both ways). The Q4 raw-access rule can
  therefore be paid off for free on such a walk; only the D2WO byte-offset macros (a cast of
  (char *)d2_Wins + off) keep their form for the match (d2_CreateScrollButtons / d2_ScrollWindowBy: the
  typed d2_Wins[win] or a typed pointer regresses them).
- A layout draft can carry a pointer where a short argument belongs: 0x472390 had `win->w = win;` and
  `desc[1] = win;` where the original stores the width argument (mov WORD [esi+0x8],cx; movsx ecx,[esp+0x60]
  then desc[1]). Grep the draft for `->field = <a pointer>` and check the field's width against the original's
  store width (fixing both took that function 740 -> 638 differing bytes).
- **VC6 drops a call to a function whose body it has already seen when the body has no side effect**
  (/O2, same TU): a caller of a `{ return 0; }` stub defined *above* it is compiled without the call;
  defined below it the call stays. 2026-10-25, unk_5d33b0.c: the three contact listeners 0x5d9a80 /
  0x5d9b20 / 0x5d9b90 sat below the stubs 0x5d85e0 .. 0x5d9890 they open; their diffs read as
  register-allocation noise (the building in a callee-saved register, the click read once) but were
  missing calls. Moved above the stubs (out of address order, with a note), all three match byte for
  byte. A scratch build confirms the rule: same caller, callee above -> no call, callee below -> call.
  When a near-match's diff starts with a missing argument push or an unrelated-looking register
  difference, check whether the callee is a stub above the caller before reaching for byte tooling.
- A call whose argument is a float's **bit pattern** (`mov eax,ds:addr; push eax` instead of `fld` /
  `fstp [esp]`) means the caller was compiled against a K&R declaration or an int parameter: the
  source passed the int, not the float (0x5dbc20 -> 0x5dd090, matched: `((int (__cdecl *)())f)(bits)`
  with a float-returning wrapper; a K&R `float f()` declaration adds an unwanted `fstp st(0)`). The
  cleanup `pop ecx; ret` (instead of `add esp,4`) comes with that cast-call form.
- Float comparisons: the **negated** spelling is what produces the C0/C2 idiom. `if (!(x < 60.0f))`
  compiles to `fcomp; fnstsw ax; test ah,0x5; jp`, while `x >= 60.0f` (and the ternary `x < c ? x : c`)
  compiles to `and eax,0x100; je`. Likewise the selected value kept on the FPU stack across the next
  compare (`fcomp 0.0; and eax,0x4100`) needs the original's condition spelling - a folded select
  loses it (0x5dd090, 2026-10-25).
- `x / 60.0f` is compiled to `fmul` by the reciprocal constant only when the divisor is a double
  (`/ 60.0`); the float literal keeps `fdiv DWORD`. Read the divisor width in the original before
  choosing the literal type (0x5dd090).
- The FPU division operand order follows the source spelling: `a / r->div` (no parentheses) gives
  `fld a; fdiv [r+..]`, while `a / (r->div)` and the `*(float *)` macro give `fld [r+..]; fdivr a` -
  the same quotient, two different pop orders (0x5dc210, matched with the unparenthesised typed
  field).
## match-base 2026-10-25 (base-5 worker, shard 5/6): memset inlining, char returns, switch vs if-chain

- **A constant-size `memset` is inlined as rep stosd + a trailing stos word**: 0x50d460's original is
  `memset(init_ObjectUniverse, -1, 0x2dc); memset(init_BuildingUniverse, -1, 0x22); return 0;` and VC6
  emits `or eax,-1; mov ecx,0xb7; mov edi,base; rep stos dword` then `mov ecx,8; rep stos dword;
  stos word` (0x2dc = 0xb7 dwords, 0x22 = 8 dwords + 1 word). The hand-written pointer loops with a tail
  `*(short *)q = -1` compile to a 4-byte absolute `mov ds:0x76fa80,ax` instead of the stos word, and the
  function is 12 bytes shorter. A `char` return type + `return 0;` adds the original's `xor al,al`
  (a void function has no return value to set).
- **A char-returning callee tests al, an int one tests eax, and the `return 0` after the test follows**:
  0x50d6d0 needed the canonical `char d3_LoadScene(char *, void *, void (*)(int,int,int))` local prototype;
  with the K&R `int f()` the call's result came back as `test eax,eax` and the failure return emitted
  `xor al,al` where the original rets with al already 0 (the `if (!call) return 0;` shape drops the clear
  only when the compiler models the return as a char).
- **`switch (k)` over two consecutive case values is a dec/dec chain**: `dec eax; je case1; dec eax;
  jne common` with the kind==2 arm placed first as the fall-through, where the if/else-if spelling of the
  same test emits `cmp eax,1; jne` (0x50d380, 111 -> 8 code hunks with the switch).
- **The if/else polarity picks the out-of-line block**: after the switch, the original`s `cmp al,0xa;
  je zero-block` (with the !=10 body falling through) comes from writing `if (*b != 10) { body } else
  { zero }`; the literal `if (*b == 10) { zero } else { body }` gave `jne body` and the zero block inline
  (0x50d380 went byte with the flip).
- **A dword store, not four char stores**: `o[0x48]='G'; o[0x49]=0; o[0x4a]=0; o[0x4b]=2;` compiles to
  one `mov DWORD PTR [o+0x48],0x2000047` (VC6 merges the adjacent byte stores); the original's dword store
  wanted the single typed store (0x50d100).
- **A symbol in a loop bound is a relocation where the original has an immediate**: `(int)arr + 0x40 *
  0xf8` in a `while (p < ...)` made the linker see two disagreeing sites for the array symbol and the
  masked field differ; a `#define` with the numeric end keeps the immediate and passes Q3 (0x50d490).

## match-base session 2026-10-25 (base-0 worker, shard 0/6, session 4, amt.c)

- **The in-loop exit decides the callee-saved push placement**: an office scan with `if (n >= max) break;`
  shrink-wraps ebx/edi to after the column guards exactly like the original (amt_GetColumnOfficesOwned
  0x4e7040, 103 -> 61 code hunks), where `return n` kept them in the entry prologue (its own epilogue copy).
  The same break on the sibling 0x4e6fc0 was WORSE: its original keeps the in-loop return with a second
  epilogue (13 -> 21 asm lines, the function 16 bytes short). Read the original's `jge` target: the shared
  final epilogue = break, its own `mov eax,reg; pop...; ret` copy = a real return.
- **`if (n < 1)` vs `if (n <= 0)`**: the original's `cmp reg,0; jg body` is the `<= 0` spelling
  (amt_GetOfficeCandidates 0x4e7150 and its sibling 0x4e7320 both gained with it).
- **A float `>` in a sort guard is often the negated `<=`**: `a > b` compiles to `and eax,0x4100; je`,
  the original's `fcomp; fnstsw; test ah,0x41; jp` is `!(a <= b)` (true for greater or unordered) and is
  what a `continue`-guard source form (`if (level < lvl) continue; if (level == lvl && rate <= rate) continue;`)
  reproduces (both candidate functions, byte-neutral to -2 asm lines).
- **The indexed `table[j].field` walk vs a pointer walk must be measured per function**: the pointer form
  matched 0x4e7150 (the original's [ebp-8] = 0x925128 .. 0x9253f8, 207 -> 205 hunks) but was WORSE for its
  sibling 0x4e7320 (135 -> 139 asm lines, reverted) - the same source text, different roles.
- **A second guard written as an early return copies the epilogue per site**: `m = f(...); if (m == 0)
  return 0;` (loop inline after it) matches the original's `test eax,eax; jne; pop; add esp; ret`, where the
  nested `if (m != 0) { ... return cnt; } return 0;` merged the two return-0 sites into one shared block
  (amt_Unk4e76b0 0x4e76b0: 167 -> 116 differing bytes, 183 -> 56 asm lines). Its bound spelling is
  `if (n >= 6)` (`cmp esi,6; jl`), not `if (n > 5)` (`cmp esi,5; jle`).
- **A manual copy loop is not strcpy**: the original's `sub ecx,eax / mov dl,[eax] / mov [ecx+eax],dl /
  inc eax / test dl,dl` is VC6's inlined `strcpy`; an indexed spelling (`dst[i] = src[i]; i++`) compiles
  to a counter loop instead (ai_CalcMeisterRaeuber 0x482f00: 321 -> 207 code hunks, 26 -> 12 slots with
  `strcpy((char *)msg.arg, src)`). A pointer spelling (`*d++ = *p++`) can already produce the inline copy.
## match-base 2026-10-25 (base-2 worker, shard 2/6, ahm.c): struct-array loop index vs pointer, factored products

- **A struct-array walk only gets the original's induction-variable base in the direct index form**: the
  save/load pair 0x4ebfb0 / 0x4ec140 reads and writes nine members of `AHMSTEP[3]` per step, and the
  original keeps `edi = &step + 0x10` (`mov edi,0x6f0df0`, `cmp edi,0x6f0e38`). `char *s` with byte
  offsets, `AHMSTEP *p` with `p++`, and `p = &ai_WalkSteps[i]` all made VC6 re-centre the IV at
  step + 0x14 (59 and 25 differing bytes); only `for (i = 0; i < 3; i++) ... ai_WalkSteps[i].field`
  produced + 0x10 and matched both functions exactly. This confirms the "write the natural indexing, not
  the bias" rule for a pointer loop over the same array.
- **VC6 factors `a * k + b * k`**: `v = sum * 500.0f + sum2 * 500.0f;` compiled to one `fadd` then one
  `fmul` ((a + b) * 500) and left an `fstp st(0)` after the __ftol. The original does `fmul; fxch; fmul;
  faddp` - two statements with the operand already on the FPU top first (`v = sum2 * 500.0f;
  v = v + sum * 500.0f;`) keep both products (0x4ec3d0 went 20 -> 0 bytes). Splitting them the other way
  round (`v = sum * 500.0f; v = v + sum2 * 500.0f;`) leaves an extra `fxch st(1)`.
- **A far draft's frame size is a lead**: d2_OpenForm (0x4741f0) compiled to `sub esp,0x244` where the
  original has 0x248 - every local/parameter slot off by 4, and the whole allocation cascades (988 code
  hunks). One 4-byte local is missing; Ghidra's lowest local (local_248) confirms the size but not which
  variable. Fix the frame size first when the diff looks like two different programs.
- **Pointer view vs array index in a far draft**: in d2_OpenForm `((int *)ww)[0xe2] = x` matched, but
  rewriting it as `d2_Wins[d2_Views[view].wins[i]].bgpic = x` (same address, recomputed) changed the
  allocation and made the function worse (1651 -> 1659 bytes, 735 -> 900 asm lines). The existing pointer
  variable's typed view member (`ww->pic`, the D2WINF view of 0x388) is the byte-neutral form.
- **A hardcoded guard is not an array end**: d2_OpenForm's view scan compares to the literal 0xfb91e4
  (0x474376), which is not d2_Views + 0x30 (0xfb6060). A #define with the address and the instruction
  in its comment keeps the immediate and passes Q3 (same lesson as the 0x50d490 end marker).
- **Global array element vs pointer local**: for `arr[i].field` with a global struct array, VC6 keeps the byte
  offset (i * sizeof) in one register and folds the field into the absolute address (`[esi+0xf88a80]` = the
  capacity of entry 0); a pointer local `e = &arr[i]` gives `[esi+0x40]` instead. Which form appears is decided
  by whether the pointer is materialized (it is as soon as it is passed to a callee), so a source that passes
  `&arr[i]` to callees AND reads fields through the local can still produce either. Confirmed on ai.c 0x483fc0 /
  0x484a60 (2026-10-25): the original is the offset form for the fields and the pointer form for the arguments.
- **memset with a constant size on a struct member** compiles to `lea reg,[base]` + four stores through that
  register; the original's four separate `arr[i].slot[j] = -1` statements compile to four stores with the field
  offset folded into the absolute address (`[ecx+0xf88a44]` .. `[ecx+0xf88a50]`, ai.c 0x4854f0). The explicit
  form is not automatically better: on 0x4854f0 it lowered the differing bytes (2361 -> 2334) but raised the
  instruction diff (172 -> 244 lines), so measure both.
- **Init of many fields of a global array element inside a loop**: VC6 hoists one pointer PER FIELD into stack
  slots and bumps each by the element size (ai.c 0x4854f0: seven slot pointers at [esp+0x10] .. [esp+0x30], each
  +0x58 per iteration). A candidate source change for that shape is a loop over a pointer variable (the offset
  form) instead of indexed stores.

## match-base 2026-10-25 (base-2 worker, shard 2/6, ahm.c, 5th): an aggregate copy re-reads, member stores CSE

- **A 12-byte struct/union assignment of a struct member gives the original's `lea` + reload; two plain
  member stores do not**: ahm_SendCreateAlchemist (0x4ec490) copies a walk step's prototype entry into a
  message struct. With `m.proto = s->grid;` (AHMGRID, a 12-byte union) VC6 emits `lea ecx,[esi+4]` and
  reads `[ecx]` / `[ecx+4]` / `[ecx+8]`; with `m[3] = s->grid.w[0]; m[4] = s->grid.w[1]; m[5] =
  s->grid.w[2];` it folds the constant offsets to `[esi+4]` / `[esi+8]` / `[esi+0xc]` (3 code hunks,
  otherwise the same schedule). The aggregate assignment is also what keeps the id OUT of a callee-saved
  register: the argument `s->grid.plr.id` is a direct load used once, so no edi CSE across the following
  call (`push edi` disappears); a local `int *w = p + 1;` with `w[0..2]` is copy-propagated away and does
  not help. Same rule as the 14-byte clock copy (0x541200): write the aggregate assignment.

## match-base 2026-10-25 (base-2 worker, shard 2/6, ahm.c, 6th): a K&R function-pointer view vs the prototype is byte-neutral at == 0

- **Q7's "call through a K&R declaration" can be a macro, and removing it costs nothing here**: ahm.c
  called `vfs_fwrite` / `ls_Read` through `#define AHM_FW ((unsigned (*)())vfs_fwrite)` / `AHM_FR
  ((unsigned (*)())ls_Read)` - the unprototyped view made the return unsigned and every check `== 0` /
  `!= 0`, which is sign-agnostic (`test eax,eax; je/jne`). Calling the prototype'd functions directly
  (ahm.h declares vfs_fwrite as unsigned with @original and ls_Read as int) compiled to the same bytes:
  0x4ecf10 stayed 235/288 and 0x4ed040 251/320 differing bytes, both now readable. A macro that only
  casts the return type of a callee is not worth keeping when the comparisons are equality.
- **A visible side-effect-free body kills the call**: VC6 drops a call whose result is unused when the
  callee's body (empty or `return 0;`) is already visible above the caller. Move the stub definition
  below its caller (out of address order) - a prototype above does not help, only body invisibility.
  Same rule for the empty `void f(void) {}` stubs as for `int f() { return 0; }`.
- **A union member read can change the whole allocation**: reading `snd_AmbientOn.on` (char member of a
  union global) instead of a plain `unsigned char` lvalue at the same address made VC6 push ebp in the
  prologue and reallocate every stack slot of a 322-instruction function. The byte alias
  (`extern unsigned char snd_AmbientOnByte; /* 0x6f0b60 */`) keeps the bytes; check with asmdiff after
  every type change, even an apparently identical one.
- **A float clamp's operand order picks the instruction form**: `v > 252.0f ? 252.0f : v` compiles to
  `fcom DWORD PTR [252.0]` (the constant as the operand), `252.0f < v ? 252.0f : v` to `fld 252.0 /
  fcomp st(1)` - which is what the original of 0x50e330 has. Writing the constant on the left also changed
  the following `(unsigned char)` conversion: with the first spelling VC6 dropped the sign guard after
  __ftol, with the constant-first spelling (and the separate `v = (int)(...)` + `v < 0 ? 0 : v` pair) it
  emits the original's `test eax,eax; setl cl; dec cl; and ecx,eax` again.
- **A do-while that decrements reuses the dec's flags**: `do { ...; n--; q++; } while (n != 0);` emits
  `dec ebx; test ebx,ebx` where the original has only `dec ebp; mov esi,edx; jne` - the C was
  `} while (--n != 0);` (0x50e5a0, 37 -> 20 differing asm lines).
- **A local record's size shows in the frame**: the draft of 0x50e5a0 declared `char rec[0x20]` for
  amt_GetOffice's output; the frame was 8 bytes too big and every slot shifted. amt.h's AMTREC is 0x18 -
  with `char rec[0x18]` the function went 75 -> 37 differing asm lines in one edit.
- **A 16-bit local's store**: with `unsigned short r` assigned from `unsigned short util_Random(...)` VC6
  emits `mov esi,eax` (it knows the return's upper bits); the original's `mov si,ax` needs the value to
  pass through a wider type (tr.c's `int util_Random(unsigned short)`), which f3init.c cannot express
  without a conflicting declaration - the one line left in init_PlaceOffices.
- **`@deep` reasons are parsed by Q5**: a reason containing the word ROUGH makes the function fail the
  draft-marker rule (readability fell readable -> draft). Word the escalation reasons around the markers.
- **Unsigned `<= 0` vs `== 0` picks the branch**: `((AMTCHARVIEW *)a)->health <= 0` (unsigned char, AICHAR.health)
  gives `test cl,cl; jbe`, the `== 0` spelling `test cl,cl; je`, a signed `char` `jle` (0x4e81e6, 0x4e820f: the
  original has jbe in the else-if and je in the inner check - both are in its source).
- **Zeroing a small local array**: `for (i = 0; i < 6; i++) fl[i] = 0;` compiles to `rep stosd`, the chained
  `fl[0] = fl[1] = ... = 0;` stores in REVERSE order, six ascending single statements give the original's ascending
  stores (0x4e8160, 96 -> 62 differing lines on that edit). A declaration initializer `fl[6] = {0}` was worse (78).
  C89 has no mid-block declarations, so a second array initialized at declaration is stored before the following
  zeroing statement; moving its values to explicit statements after the zeroing reproduces the original's order.
- **A loop pointer vs the index decides the loop's base register**: `p = list; for (.., p++)` let VC6 keep
  `edi = &p->state` (all fields at -0x10 / -0xc / +0 / +4), the original keeps the record base (`[edi+0x10]` state).
  Writing the body as `list[j].field` flipped it, 52 -> 37 differing lines (0x4e8160).
- **A symbol-form loop bound is not uniformly worse**: `(char *)ai_Chars + 768 * 0x21c` for 0x7d5f80 cost +11 lines
  in amt_ElectOffices, but `(char *)ai_Chars + 2 + 768 * 0x21c` for 0x7d5f82 in amt_CheckOfficeColumns was
  byte-neutral (Q3 fixed). Measure per function, never carry the verdict over.
- **An index loop and a do-while pointer walk are not the same code**: ai_BuyUpgrade (0x487fb0) was written
  `p = &bt->slot[0]; do { ... i++; p++; } while (i < bt->nslots);` - VC6 spilled BOTH induction variables (i to
  the frame, p to a register) and the frame came out 4 bytes too big (0x200 vs 0x1fc). Rewritten as
  `for (i = 0; i < bt->nslots; i++) { ... bt->slot[i] ... }` (the index only, VC6 strength-reduces the pointer
  itself) the frame matched and the function went 99 -> 48 differing asm lines in one edit (code hunks 151 -> 52,
  all slots hunks gone). When the original's loop tail is `inc ebp` / `cmp dl,[ebx+0x22]` and the frame is one
  int too big, write the loop over the index, not over a hand-walked pointer.
- **The same function's `c->bld` mix is load-bearing**: using the `bld` local for the identity checks but
  `c->bld` for the later reads (ai_BuyUpgrade) gives 52 code hunks; all-`bld` gives 377, all-`c->bld` 81.
- **A NaN-catching clamp picks the test form**: for `f` clamped to 0 .. 1 the source `if (!(f > 0.0f)) f = 0.0f;
  else if (!(f < 1.0f)) f = 1.0f; r->value = f;` (ahm_AddValue 0x4ecdb0) is the and-idiom form VC6 compiles
  with `fcom / and eax,0x4100 / jne`; the plain `if (f > 0.0f) { .. } else ..` stores in both arms (3 tails)
  and the `f <= 0 / f >= 1` clamp re-orders the tests, while the `!(f > 0.0f)` form reaches 12 code hunks
  (44 -> 41 bytes) and leaves only the test schedule at the top.
- **The store order of two record fields decides the argument-load registers**: ai_NewMission (0x4b3ca0)
  had the mode-5 block and the free-slot loop storing `owner` before `type` in different orders; with
  `type` first in BOTH blocks VC6 loads type into eax first and the function matches byte (10 -> 0
  differing instructions). The two blocks are copies of the same three statements and must be spelled in
  the same order as the original's.
- **A volatile view can be typed instead of cast**: the 4 `(*(volatile short *)&dm_S.x / .y)` Q4 casts of
  di_UpdateButtons (they stop VC6's CSE of dm_S.x, which would take the zero register from ebx) are byte-for-byte
  replaceable by an anonymous-union view in the struct: `union { struct { short x, y, z; }; struct { volatile
  short vx, vy, vz; }; };` and `dm_S.vx` / `dm_S.vy` at the same uses. Same bytes, Q4 clean, no other unit's
  code changed (full check count unchanged). Prefer this over a per-use cast for a struct field that must be
  re-read at every use.
- **A constant initialised before a guard sinks**: `i = 31; if (changed && dm_Q[31].valid == 0) {...}` (and the
  `int i = 31` / declaration-init forms) sinks the constant into the loop preheader, so the guard's global load
  takes eax and the test uses eax; the original materialises 0x1f between the load and the test (load into ecx).
  Every form that forces the init before the guard test (for-init `for (i = 31, q = &..; *q == 0;)`, the comma
  form in the `&&`, the guard comma) fixes the tail but merges the loop head and costs a whole-function
  reallocation (di_UpdateButtons: 7 -> 180 asm lines; const#0 web -55 against _di_PressX -57 in c2prio).
  Allocation work, not a source spelling.
- **An inner call inside a call's argument orders the pushes**: `f(a, -1, g, (util_Random(n) & 0xffff) + k,
  own, 0)` makes VC6 evaluate the right-hand arguments (0, own) before the inner call - the original of
  0x50f0b0 pushes `push edi; push ecx; push n; call util_Random` and cleans 6 words. Writing the random as
  its own statement (`r = util_Random(n) & 0xffff; f(..., r + k, ...)`) pushes only `n` first and leaves one
  extra word (`add esp,0x1c` instead of 0x18) and the call no longer tail-merges with its sibling branch.
- **A switch over consecutive values is a subtract/dec chain**: `switch (t) { case 0x1e: ... case 0x20: }`
  compiles to `mov eax,t; sub eax,0x1e; je; dec eax; je; dec eax; jne`, where the if/else-if chain over the
  same values compares t in a register three times. 0x50f0b0: 210 -> 75 code hunks.
- **VC6 lays switch case bodies out in source order**: the block addresses of the jump-table targets give
  the original's case order back; 0x50f0b0's goods switch was written in the order the goods[] table is
  filled (0x1bd, 0x1bf, 0x1be, 0x1c0, then the 0x1b7.. group, then 0x1bb, 0x1bc) - reordering the cases
  took 59 -> 43 differing asm lines and the function to byte.
- **Unaligned int fields need `#pragma pack(push, 1)`**: ai.h's AIBLD has `id` at 0x01 and AIOBJ `objId`
  at 0x02; a local view without the pragma puts them at 0x04 (every access differs). ai.h wraps its
  structs in pack(1); a new view has to do the same.

## match-base 2026-10-25 (base-0 worker, shard 0/6, amt.c): the guarded while, a ternary of two constants, char parameters

- **A `&&`-guarded `while` does not rotate**: `while (r != -1 && cm_GetSlotState(r) == 0) sv_PumpNet();` keeps the test at the
  top (cmp r,-1 / je; push/call/test; jne; body; jmp back). `if (r != -1) { while (cm_GetSlotState(r) == 0) .. }` and a
  `for (;;) { if (..) break; .. }` rotate the loop (the test duplicated at the bottom): +29 bytes in amt_UpdateOffices.
- **`if ((char)f & 2) return 1; return 0;` gives `movsx r,BYTE [f]; and r,2; shr r,1`** - round 17's rule confirmed on an int
  flag slot; the algebraically equal `(f & 2) != 0` gives shr/and and `(f & 2) >> 1` shr/and too, so the if/else return is the
  only spelling that matches this shape (amt_IsColumnDecided).
- **A ternary of the two complete constants stops the lea fusion**: `(flag ? 0x27b : 0x253) + v` gives `and eax,0x28; add eax,0x253;
  add eax,esi` with v in the saved register; `(flag ? 0x28 : 0) + 0x253 + v` is reassociated into `lea eax,[eax+edx+0x253]` with
  v in a caller-saved register. The constant pair (0x253 / 0x27b) is the original's own spelling, not a trick (amt_SendOfficeNews,
  amt_SendOfficeLost).
- **CONFIRMED byte-level in ord.c (2026-10-26)**: 28 lines of the level/job handlers went byte on the complete-constant
  ternary alone (11 x `(c[9] != 0 ? 0x11e : 0x114) + level`, 17 x `(c[9] != 0 ? 0x27b : 0x253) + job` and the
  0x25e .. 0x2a0 variants); 15 functions matched at once.
- **A `unsigned char lv` local that would keep a byte in cl** across a byte compare does NOT survive when the same
  cl is needed for a constant (`cmp cl,6`): VC6 spills lv to a 4-byte frame slot. Reading the field twice
  (`c[0xd]`) gives the original's single `mov cl,[...]` and the smaller frame (ord.c 0x4f4f20).
- **A `char` parameter gets the byte pushed without an extension**: `mov dl,[x]; push edx` (int parameter: `xor edx,edx; mov dl,..`).
  amt_SendOfficeFree (0x4e65e0) takes a char - its own bytes stayed equal; the same idiom named the callee's type.
- **An anonymous union / struct member in a shared header can flip a byte function, even when the layout does
  not change**: adding `struct { unsigned char count; char pad59; unsigned short item; } order;` to the
  AIMSG.arg union (same 4-byte footprint, no other TU reads it) turned ai_Unk483a60 (0x483a60, byte) into 5
  differing bytes - the [esp+0x10]/[esp+0x18] slot swap. Measured twice (a previous session's `entryIndex`
  member did the same). AIMSG's union members are for match-deep's shared-header decision; the raw word /
  byte stores in ai_SendJobOrders stay until then. A plain field RENAME (AIENTRY.unk44 -> sellAmount,
  AISLOT.unk18 -> sellAmount, AIOBJ.unk12 -> fill, AIMAPENT restructured to the G15PLANT layout) is
  byte-neutral.
- **A stale alias rename must carry the canonical signature into every TU**: renaming ai_Unk4fe140 ->
  gm_AddTime (424 refs) left unk_5d33b0.c's `int gm_AddTime();` (K&R, int return) against the prototype -
  lint's cross-TU X conflict (every conflict is an error, so ledger check failed at the commit). Fill in the real
  prototype (with the file-scope `struct GCLOCK;`) instead of parking it.
- **A second `for` condition on the counter puts its test on the back edge only**: 0x4b4760's original
  tests `b` in the shared top block and `n < 3` right after the last call (then jumps back to that top
  test), and keeps the redundant post-loop `n >= 3` check with a second return-1 epilogue. Exactly that
  comes from `for (b = first; n < 3 && b != 0; b = next)` with the counted value updated at the body end:
  44 -> 5 differing asm lines. The other spellings (`b != 0 && n < 3`, the same as a `while`, the
  increment in the body plus `if (n >= 3) return 1;`, `continue` with an inverted tail) put the n test at
  the top or duplicate the loop test (33-62 differing instructions).
- **A literal used in two distant statements can get a callee-saved register** (c2prio's constant web):
  in 0x4b58b0's original the 1 pushed for `d2_SetForm(view, 1)` is still in ebx at
  `sv_PopRequest = 1` inside the loop; with two immediate uses ours never materialises the web. A local
  `ok = 1;` is constant-folded to the same immediates, so it does not reach the allocator.
- **`x = x + 1` vs `x++` through a pointer decides `inc [mem]` vs load/inc/store** (ai_CreditsScreen
  0x5d7ba0, matched byte this way): `d2_LastWindowPtr->scrollY++` compiles to `inc DWORD PTR [reg+0x248]`,
  `d2_LastWindowPtr->scrollY = d2_LastWindowPtr->scrollY + 1` to `mov eax,[reg+0x248]; inc eax; mov
  [reg+0x248],eax` - and the assignment form through the **global pointer** (not through a local copy of
  the same address) is also what makes VC6 reload the pointer global after the store, as the original
  does. The two loops of that function really are spelled differently; reading each loop's increment
  separately is the point.
- **A K&R definition with `short` parameters makes the caller push the low word**: with
  `int d2_CreateWindow(short x, short y, short w, short h, unsigned int flags);` in the caller's TU,
  VC6 knows the callee reads only the words and emits `mov cx,WORD PTR [..]` (upper half of ecx
  garbage); the original caller has `movsx ecx,WORD PTR [..]` and pushes the int. The caller-side
  prototype must use **int** parameters (what the original caller was compiled against, d2.h's K&R
  comment says "x, y, w, h short; flags"), tagged `@original` where the lint cross-TU check needs it.
- **A redundant `else if (x < 30.0f)` is a real third comparison**: MSVC spells `<` as
  `fnstsw ax; test ah,5; jp` and `>=` as `and eax,0x100; jne`; a two-branch `if (x >= 80) .. else if
  (x >= 30) .. else ..` gives two compares, the original's three (`>= 80`, `>= 30`, `< 30`) only comes
  back if the last branch is written as an explicit `else if (x < 30.0f)`.
- **An int parameter zero-extends an unsigned short argument; a short parameter does not**: at 0x50f42c the
  original pushes the third cm_SendExSellObject argument with `and eax,0xffff` (and the compare uses a 16-bit
  `cmp ax,0xffff`). That is a prototype with an `int` third parameter and an `unsigned short` value, while a
  `short` parameter lets VC6 pass the low half with the upper bits undefined (no `and`). f3init.c's local
  declaration went short -> int and got an @original tag (evidence = that call site); 28 -> 12 code hunks.
- **`unsigned short != -1` compares 32-bit, `!= 0xffff` compares 16-bit**: `if (*w != -1)` on an
  `unsigned short *` emits `and eax,0xffff; cmp eax,0xffff`, while `if (*w != 0xffff)` emits `cmp ax,0xffff`
  with the zero extension deferred to the call's argument push - which is what the original does (0x50f62b /
  0x50f6cd). The same value, one instruction less, and it unblocks the register schedule.
- **A local array initialised from a constant list is immediate stores, a memcpy source is not**: `char cls[0x22]
  = {0, 0, 3, ...}` compiles to 0x22 `mov BYTE PTR [esp+X],imm8` stores; keeping a separate `tab[0x22]` plus
  `memcpy(cls, tab, 0x22)` gives lea/lea and a call-shaped copy. Where the original has the byte stores, the
  initialised array IS the working array (0x50f910: 789 -> 761 differing lines).

## From 2026-10-26 (base-0 sv.c, base-2 unk_4b39f0.c, base-3 mem/qnt/txt, base-4 unk_5d33b0.c)
- **Identical call tails in sibling branches are tail-merged by VC6** (match-base 2026-10-26): `sv_CheckStock`
  (0x5210a0) and `sv_MessageBox` (0x521150) select a text id / a form name in three or five branches and then
  call `txt_Format` / `ai_Unk4741f0` - the original has ONE call instruction that every branch jumps to, with
  the constant pushed inside the branch (the argument setup `and esi,0xff; lea buf; push ..; push const` is
  duplicated, only the call merges). A source with `id = ...; goto show;` or a shared `name` variable compiles
  the other way round (`mov eax,0x76; jmp` + one shared setup). Write the call in each branch and let the merge
  happen: 0x5210a0 went 0 differing instructions this way.
- **A timeout compare on the ms clock is unsigned**: `if (now > t0 + 500)` in the original emits
  `cmp ecx,eax; jbe` (unsigned), with `int t0` it is `cmp eax,ecx; jge`. Declaring `t0` `unsigned` (or the
  clock unsigned at the read) is what flips it (0x521150).
- **A twin function in another unit is the best source-shape lead**: `sv_MessageBox` (0x521150) has its twin
  `ai_Unk4e5470` in cm.c (same message box, other globals; `timed = flags & 0x80`, `if (timed) bar = call; else
  bar = -1;`, `if (sv_RunFrame(..)) { quick = flags & 0x40; n = flags & 4; do { .. } while (sv_RunFrame(..)); }`).
  Copying the twin's statement shape took 0x521150 from 212 to 133 code hunks in one edit.
- **The clock's type decides a function's code**: renaming the int alias 0x6333cc to the `volatile unsigned
  long d3_Time` that cm.h/anm.h already declare flipped sv_Unk5263a0 (0x5263a0) from 1365 to 1458 differing
  bytes (`dt = (unsigned)(clock - last) * 0.4` compiles differently when the left operand is unsigned long),
  while the check count and uv run ledger check --against stayed clean - unitdiff is what showed it. Reverted; the
  unification is a shared-header decision.
- A store of a *literal* takes the register that already holds that value: ai_Unk4b3d50 has
  `r = 1; sv_PopRequest = 1;` and the second store compiles to `mov [sv_PopRequest],ebx`, not an immediate
  (the register holds the constant from `mov ebx,1` for `r = 1`). So an unexplained `mov [mem],ebx` after a
  `mov ebx,1` is a *variable* assignment to 1 in the same block, not a CSE of two immediates.
- Constants in a register: -1 becomes `or reg,-1` (3 bytes, not `mov reg,-1`), and a bit that sits inside a
  byte can take the byte form even for an int expression - `| 0x100` became `or bh,0x1` in one allocation
  (ai_MissionResultDialog with `char flags`), `| 0x80` on a renamed value becomes `or bl,0x80`.
- **A count-down loop's entry test register is allocation, not source**: `for (i = qnt_HeapCount; i > 0; i--)`,
  the `while (i > 0) { ...; i--; }` form, a do-while shape and a fresh block variable `k` all compile to the
  same 10-instruction diff at qnt_Build (0x420b72: the original `mov esi,eax; test eax,eax` where we get
  `test esi,esi`). VC6 canonicalises the loop shape before register allocation, so the `test eax` vs
  `test esi` choice needs the allocator toolbox, not another source spelling.
- **A dead compare cannot be reached back with an empty conditional**: txt_OpenText (0x42d2d6) has
  `cmp eax,0x71b; jne <next instruction>` (a branch to the next instruction, its flags unused). VC6 /O2
  drops the compare for `if (x == C) ;`, a one-case `switch (x) { case C: break; }`, `if (x == C) goto
  next; next:` and `while (x == C) break;` alike (all four compile identically, no compare). The source
  construct behind the original's residue is still unknown; do not spend builds on these shapes.
- **D3_ZERO, not memset, for the engine's inline clears**: `memset(p, 0, n)` folds to `mov ecx,n/4; xor
  eax,eax; lea edi,..; rep stosd` in an esp frame, while the engine's clears in the original are `D3_ZERO(p, n)`
  from d3.h (an asm macro): it gives the ebp frame with the size and pointer spilled to two temps (ebp-4 /
  ebp-8) and the byte-align / rep stosb / rep stosd / rep stosb chain. 0x5d8360's zeroed 0x50-byte request
  matches byte-for-byte only with D3_ZERO (memset: 113 code hunks, D3_ZERO: 24, then 0 after the pointer
  spelling). Grep the pattern `8d 0c 7f 83 e1 03` (lea ecx,[edi+edi*2]; and ecx,3) to tell which clears are
  D3_ZERO copies (329 in the game code).

## 2026-10-26 (base-5 f3init.c; base-7 / base-8 amt.c, unk_4b6480.c)

- Mixed 8-bit comparison: `unsigned char x == char y` promotes BOTH to int (VC6: `movsx` of the signed side,
  `and 0xff` of the unsigned, then a 32-bit cmp) where `char == char` or `unsigned char == unsigned char`
  gives the original's single byte `cmp`. When the hunk reading shows a movsx + 32-bit cmp where the
  original has `cmp dl,mem`, the source's two operands have different signedness - cast one side.
- A local pointer into a global heap block (`bb = ai_Bauplaetze + n * 0x60`) is CSE'd into one register,
  while the original's repeated global index expression (`((T *)ai_Bauplaetze + n)[i]` written out at every
  store) reloads the pointer AND the count from .data around every store (the store through the pointer can
  alias nothing, but VC6 reloads anyway). To reproduce the recomputation, write the global expression at
  each use, not through a local.
- **The compare spelled from the other side decides the operand order and the store order**: a vote loop
  `if (best < s) { best = s; pick = cp; }` compiles to `cmp ebx,eax; jge` (best in ebx, s in eax) with
  `best` stored before `pick`; the same test written `if (s > best) { pick = cp; best = s; }` gives
  `cmp eax,ebx; jle` and `mov esi,edi` before `mov ebx,eax`, which is what amt.c's office elections
  (0x4e8c20 byte, 0x4e88a0) have. When a compare hunk shows the two operands swapped, write the condition
  from the other side before looking at the registers.
- **Hoist the element pointer before the field test**: `k = ai_Chars[id][0x164]; if (k >= 1 && k <= 6) {
  for (i = 0; i < 16; i++) if (cand[i] == ai_Chars[id]) ..` recomputes `ai_Chars[id]` after the compare
  (`lea` after the branch); `ch = ai_Chars[id]; k = ((AMTCHARVIEW *)ch)->rank; if (..) { .. if (cand[i] ==
  ch) ..` gives the original's `lea ecx,[eax*4+X]` before the `cmp al,1/jb; cmp al,6/ja` (amt.c 0x4e88a0:
  658 -> 424 differing bytes, 0x4e8c20 likewise). A byte field compared `>= 1 && <= 6` is a byte range
  test (`cmp al,1/jb`, `cmp al,6/ja`), `!= 0 && < 7` is `test al,al/je; cmp al,7/jae`.
- **A redundant `if (n > 0)` around a for loop costs a second test**: the original's vote loop has one
  `test eax,eax; jle` (the for's own guard); `if (n > 0) { for (..) .. }` emits two (one on the register
  copy, one on the reloaded slot). Dropping the wrapper is meaning-neutral when the body only sets `pick`
  (amt.c 0x4e8c20: 12 -> 8 code hunks, then byte).
- **Two counters that the draft merged into one variable**: 0x4e8c20's kind-0x22 scan uses a separate
  `left` (initialised to 1 right before the scan) and the vote index `j` (zeroed in the prologue, never
  reset); the draft's shared `j = 1; .. j--; .. for (j = 0; ..)` leaves the original's `xor ebp,ebp` in the
  prologue unaccounted and orders the scan's `dec` wrongly (byte after the split).
- **A shared `return` block sits where the source's last `return` of that value sits, and `goto` puts it
  before the body**: unk_4b6480.c 0x4b6660 has one shared `return 0x400` epilogue reached by the guards'
  `je`s and by the loop's fall-through, with the found body *after* it (0x4b6743 vs 0x4b6753). The nested-
  `if` form (`if (a && b && ..) { loop; if (p) { body; return 0; } } return 0x400;`) and the inverted
  `if (p == 0) return 0x400;` form both put the block at the end and leave an extra `test p; je` after the
  loop (a `code` hunk, not `order`). Spelling the exits as `goto none` (the guards) and `goto found` (the
  two `p != 0` checks) reproduces the original's block order exactly, and `for (i = 0, p = 0; ..)` fixes the
  `xor` order: 23 differing instructions -> 0, byte. amt.c's `goto none` in 0x4e6fc0 is the same trick.

- A sum of a conditional small constant and an index, `(c ? a : b) + x`, keeps VC6's two adds (branchless
  `neg/sbb/and; add imm; add reg`); the flat `(c ? d : 0) + a + x` folds into one `lea reg,[r1+r2*1+disp]`
  (unk_4b6480.c ai_Unk4b82c0, byte with the ternary; a temp for the first two terms does not stop the lea).
- **A frame that is 4 bytes too big is often an FP temp, not a local**: `r > gm_Liking(..)` evaluates `r` to
  a float *before* the call and spills it (`fstp [esp+0x20]`), which costs a 4-byte slot and shifts every
  later offset; written `gm_Liking(..) < r` the call comes first, `r` is `fild`ed after it and `fcompp` pops
  both (unk_4b6480.c 0x4ba0c0: frame 0x128 -> 0x124, 126 -> 10 hunks, behaviour). The C is the same compare.
- **A shared exit reached from *inside* a loop needs `goto`, not nesting**: where the nested-`if` form cannot
  place the block last (in-loop guards), `goto none` on every guard plus one `return X` after the body gives
  the single shared epilogue the original has (unk_4b6480.c 0x4ba0c0, whose last guard VC6 otherwise inlines).
- **`msg.n = msg.fromId;` is one load where `msg.n = b->id;` is two**: a struct field assigned from another
  field of the same struct makes VC6 reuse the register across the fill (unk_4b6480.c 0x4ba0c0).
- **An out-parameter whose type is too small shows up as a frame difference and out-of-frame stores**: an
  `int` where 0x4fe2c0 (gm_ClockToDate) writes a 10-byte calendar record made the frame 8 bytes short
  (unk_4b6480.c 0x4bb250: `unsigned char d[10]`, f3gm.c's convention, then byte).
- **A displacement in a compare is a meaning error, not a spelling**: `((char *)&ai_Goods[id])[9] != 9`
  compiled to `[ecx+base+0x9]` against the original's `[ecx+base+0]` (the good's `type`); the address-mode
  difference also dragged a callee-saved register colour (unk_4b6480.c 0x4bacc0, byte with `ai_Goods[id].type`).

## match-base 2026-10-26 (amt.c session)

- **A "max so far" local must be `int`, not a byte**: `unsigned char best` gives `cmp bl,al; jae` (unsigned),
  `int best` with an `unsigned char lvl` operand gives the original's `cmp eax,esi; jle` (the lvl is
  zero-extended). amt_CheckOffices (0x4e9260) and amt_ExecutePriceMessage (0x4e9660) both had this.
- **The two-compare unsigned range guard is its own spelling**: `(unsigned)m[1] >= 1 && (unsigned)m[1] <= 7`
  compiles to `cmp eax,1; jb` + `cmp eax,7; ja`; `m[1] != 0 && (unsigned)m[1] < 8` compiles to
  `test eax,eax; je` + `cmp eax,8; jae` (same semantics, different bytes).
- **`for (p = list; n > 0; n--, p++)` vs `p++` in the body**: with the pointer step in the for-update clause the
  update block stays after the body and `continue` jumps forward; as a body statement VC6 rotates the loop
  (update block before the body, an entry `jmp` over it, and p/count in memory instead of a register). The
  same function can be 90 bytes off on this alone (amt_CheckOffices).
- **A `while` inside `if (r != -1)` is inverted, the guard as a second `&&` is not**:
  `if (r != -1) { while (cm_GetSlotState(r) == 0) sv_PumpNet(); }` compiles to test / jne out / body /
  duplicated test at the bottom; `while (r != -1 && cm_GetSlotState(r) == 0) sv_PumpNet();` gives the
  original's test / jne out / body / `jmp` back to the test (the hoisted invariant). amt_CheckOffices byte.
- **A double literal in a float expression is a different instruction**: `1.0f - (x + 0.2f)` gives
  `fadd DWORD` + `fsubr DWORD`; `1.0 - (x + 0.2)` gives `fadd QWORD` + `fsubr QWORD` (0.2 and 1.0 widened).
  The constants are the doubles 0.2 / 1.0 at 0x5eb7d0 / 0x5eb8a8 (amt_ExecutePriceMessage).
- **Comparing `unsigned short` with `short` widens (movsx), a cast keeps the 16-bit compare**:
  `o->owner != (unsigned short)*c` is `cmp ax,[mem]`; `o->owner != *c` loads `movsx edx,WORD [mem]` and
  compares dwords. On amt_ExecuteShareMessage this took the function from draft to behaviour in one edit.
- **A loop bound written as a pointer compare is unsigned, an int cast is signed**: `q < AMT_CHARS_END` gives
  `jb`, `(int)q < (int)AMT_CHARS_END` gives the original's `jl`.
- **Two independent builds of the same tree can disagree on one FP function** (observed once:
  ai_CondCharShare fild/fdivp vs fidiv after a full header-triggered rebuild, the match back on the next
  build with no source change). Rebuild before believing a regression report.
- **An invalid escape drops the backslash silently and shortens the literal**: `"misc\Messagebox"` written
  with one backslash is `"miscMessagebox"`, and `"\game.INI"` is the 9-byte `"game.INI"` (VC6 copies dword +
  dword + byte) where `"\\game.INI"` copies 10 bytes (dword + dword + word). The function still matches
  (relocations are masked) but the string data at 0x60ea68 differs and `link --nonmatching` reports the
  `??_C@...` symbol as a data conflict; check a strcpy hunk at the inlined copy before suspecting the source
  structure. Write `\\` in the C source.
- **A string choice in a ternary or in a `name` local does not match; one call per if-arm does**: VC6 pushes the
  immediate in each arm and tail-merges the shared `push 0 / push 0 / call` (0x5216a0, 0x521900).
- **A local that only caches a global read hides the original's reloads**: replacing `int cid` by AMT_NETID(c) at
  every use (the original reloads `[esi+4]`) took a function from 678 to 517 differing asm lines and shrunk the
  frame by 4 bytes to the original's 0x8dc (amt_ExecuteMayorPowerMessage).
- **A 9-int table record written into an 8-int buffer is a meaning bug**: gv_GetTableRecord copies 9 ints; the pay
  function read `buf[0]` where the original reads `buf[6]` (0x4ea990 byte-matched at once with `int buf[9]`).

## From worker base-2 (round 6, hst.c)

- A `return <constant>;` block is merged by VC6 into an earlier block with the same constant: ours gets
  `test; je <earlier block>`, the original usually keeps `test; jne <continue>` + its own copy. What breaks
  the fold is a *flow-graph* difference elsewhere, not the return's spelling: the same source function with
  the loop written `for (n = 0x300; n != 0; n--) { if (found) break; .. } if (n == 0) return 0;` keeps the
  copy (the `if (n == 0)` is provably true on the fallthrough, so the return is inline), while
  `for (;;) { ..; if (--n == 0) return 0; .. }` folds it. Every form of the return itself (if/if!, braces,
  if/else, while, do/while, switch, nested if, statement reorder, dead statements in between) still folds -
  reproduced on minimal functions, so don't spend builds on them.
- `ai_SplitTokenFormat(fmt, buf, &n1, &n2)` with `n2` **never initialised**: VC6 puts n2 in the dead `fmt`
  parameter slot (`lea ecx,[ebp+0x14]` as the 4th push) and only the AI_ZERO `_zn` temp gets a frame slot
  (frame 0x208). Writing `n2 = 0x200;` first forces n2 into the frame instead (0x20c/0x210).
- A destination pointer parameter that is stored through (`*dst++ = ..` in a loop) is reloaded from its
  parameter slot after every store (the store may alias the slot). A local copy read once (`d = dst;` and
  `*d++ = ..`) keeps it in a register - that alone fixed 0x4adf40 to byte.
- `c->health == 0` and `c->health <= 0` differ (`je` vs `jbe` on an unsigned char) - check the branch, not
  the field's apparent type: same field, different spelling per function.

## match-base 2026-10-26 (base-4 worker)

- **A local view of a packed struct needs its own `#pragma pack(1)`**: unk_58bbd0.c's local AIBLD mirror put
  `int objList` at +0x60 (aligned) where ai.h's packed AIBLD has it at +0x5d; the build read
  `mov ecx,[eax+0x60]` against the original's `[eax+0x5d]`. Splitting an existing pad array into
  `char pad[0x34]; int field;` keeps every other offset only when the struct is packed.
- **A rename must reach every call site in the same edit**: with the definition renamed but one caller still
  spelling the old name (or the reverse), C89's implicit `int f()` declaration compiles and the *named*
  function is unused; the byte check then compares the wrong symbol and reports dozens of code hunks that
  have nothing to do with the body. uv run refs OLD before and after the rename.
- **A store through a global pointer stops CSE of that pointer**: writing `ai_Bauplaetze[ai_NBauplaetze].x0 = ...`
  makes VC6 reload both globals for every following statement and every compare (`mov eax,ds:count;
  lea edx,[eax+eax*2]; mov eax,ds:arr; shl edx,5; mov [ecx+eax+off]`), because the store may alias them; a
  cached `float *bb = ...` gives 2 instructions per store and never matches (f3init.c 0x510770).
- **min/max macros on a global-array element**: `min(corner, ai_B[n].field)` (the corner first) compiles to the
  `fld corner; fld field; fcompp; and eax,0x4100; jne; fld corner; jmp; fld field; fstp field` select (min) and the
  `test ah,0x5; jp` variant (max); the `field < corner ? field : field` spelling with the destination first gives
  `fcomp mem`, one hunk per statement (f3init.c 0x510770, 108 -> 48 code hunks on the six statements).

## decomp session 2026-10-26 (orchestrator merge)

- **An unrelated header include can move a function's register allocation**: adding `#include "err.h"` to
  d3_int_io.c (one extra `void err_Message(char *)` plus five other declarations) turned d3_LoadScene from a
  1424-byte match into 3 differing bytes at +0x27f - two stack arguments loaded in the opposite order and the
  `lea` computed with swapped registers. The file's only need is err_Message; declaring that one prototype
  locally keeps the match, so check a matching function after adding an include to its TU even when the
  declarations are unrelated to it.

## match-base 2026-10-26 (base-1, ord.c)

- **A store to the memset's own base drops the whole inlined clear**: `memset(p, 0, 0x24)` followed by
  `*(int *)p = -1` (p a global array with a known address) loses the entire rep stos - not just the
  overlapping dword - and only 5 of the 6 clears survive. `volatile`, a local `int *p2 = (int *)p`, and a
  struct view all still lose it. A second extern name for the same address (a data.csv alias, e.g.
  `extern int ord_MessageRecordEnabled; /* 0x91b480 */`) is seen as a distinct object before the link, so
  the memset stays and the store still emits `mov ds:0x91b480,0xffffffff`; ord.c 0x4f82a0 matches so.

## match-base 2026-10-26 (base-1, second entry, shared headers)

- **Adding fields to a struct (same size, same offsets) can move another function's stack slots**: replacing
  AIGRID's `char pad05[0x13]` by `char pad05[3]; int average; float unk0c, unk10, unk14;` (still 24 bytes,
  every offset unchanged) turned ai.c's ai_StartBuildingWork (ai_Unk483a60, 0x483a60) from byte into a
  [esp+0x10] / [esp+0x18] slot swap. The ai.c functions that use ai_Grid do not read the new fields. Header
  type work on a shared header therefore needs a full `uv run ledger check` after the edit, not only the unit's
  functions: revert when a byte match falls. The same swap follows any change to AIMSG's or AITIME's
  declaration, even a layout-neutral anonymous union. The grid's +8 / +0xc / +0x10 / +0x14 fields are
  documented in gv.h's ORDCELL view instead (ord.c cannot include ai.h: both headers declare ai_Time /
  ai_Chars / ai_BTypes), the consuming unit gets a local view struct, and the AIGRID change was reverted.

## match-base 2026-10-26 (base-2 worker, hst.c text commands)

- **`if (cond) { body } return 0;` puts the return-0 block after the body; `if (!cond) return 0;` + a block
  puts it inline at the top.** The original of the hst.c text commands uses the second shape: with the first,
  VC6 schedules the whole body around the later return block and the diff explodes (0x4b00e0: 96 code hunks;
  the early-return form: 3). The declarations stay in a block right after the guard (C89), so the table
  initialisers still copy after the test, as the original does.
- **A cast result used in both arms is one variable**: `v = gm_GetGoodsValue(c); .. (int)(v * f)` twice gives
  an extra 4-byte local (frame 0x18 vs 0x14); `v = (int)(gm_GetGoodsValue(c) * f)` and passing v in both arms
  matches byte (0x4b0360).
- **min/max operand order shows in the `cmp`**: `max(min(d.max, n), d.min)` gives the original's
  `cmp eax,esi; jl` (destination-first min); `min(n, d.max)` gives `cmp esi,eax` and one extra hunk
  (0x4b09f0 byte only after the swap).
- **A field at a neighbouring offset is not "the same field"**: the 0x4e/0x50 event messages write 1000 at
  AIMSG+0x9c while another message writes unka0 at +0xa0; the classifier accepts the +4 operand as a `slots`
  hunk, so a wrong field offset can score `behaviour` - read the offset, not the class (0x4affc0/0x4b0050
  became byte with a local view).

## match-base 2026-10-26 (base-3 worker)

- **Q4's "dereferencing macro" can be spelled as a pointer + `->` without moving a byte**: `(*(T *)(base + off)).f`
  and `((T *)(base + off))->f` are the same AST (`p->f` is `(*p).f`), so VC6 emits the identical code - including
  the byte-offset addressing form (`lea/shl` from the offset, the offset kept in a register) that the index form
  `arr[i]` loses. Same for a subscript: `((int *)(p + 0x45))[i]` == `*(int *)(p + 0x45 + i * 4)`. This clears Q4
  where the old spelling was chosen *because* it was the byte form (d2.c d2_CreateScrollButtons / d2_ScrollWindowBy
  / d2_CloseView / d2_GetObjPicSize: 2610/4664 unchanged, no unitdiff WORSE).
- **The same trick does not fix an IV-offset access**: in d2_Exit the picture-table reads `*(int *)(i + 0x34 + base)`
  match the original's `[ebx+ecx*1+0x34]`; a local pointer or a typed field flips the sib base/index pair. The
  difference is that here the *sum* (i + base) is the address, not a struct base with a constant offset.
- **d3.h's content is load-bearing for d3_mir.c**: removing d3.h's `d3_RelightObject` line flips d3_MirDrawObject
  (esi/edi load order), and so does appending `#include "d3_light.h"` at the end of d3.h - the declaration has to
  be *absent from that header* for the canonical-header rule, so only re-matching d3_MirDrawObject unblocks it.
  Adding `#include "d3_light.h"` to d3_anim.c puts +2 instructions into d3anim_CreatePath and
  d3anim_CreateMorphAnim (re-confirmed this session, reverted).

## match-base 2026-10-26 (base-4 worker, second session)

- **A stray call argument is a source fact, not a cast**: the original's `ai_FillSaveSlots` call in
  0x5d4080 / 0x5d42b0 / 0x5d45d0 / 0x5d4920 pushes **six** arguments although 0x5d3ab0 never reads the
  sixth (its frame reads the five parameters only). A caller cannot add a push with a cast (a cast keeps
  the argument count), so the *definition* must take the extra parameter (`int reserved`, `(void)reserved;`
  in the body, tagged with the caller's pushes); only then do the frame size, the `add esp,0x40` and the
  whole setup block line up. Same for the 4-of-8 argument call to sv.c's slider builder 0x52e860: the
  definition stays canonical and the call goes through a function-pointer cast (see next rule).
- **A six-argument call to a two-parameter function, byte-neutrally**: keep the canonical prototype
  (`int d2_GetViewWin(int view, int i);`) so Q7 passes, and spell the stray-argument call
  `((int (*)(int, int, int, int *, int *, int *))d2_GetViewWin)(win, 1, 0x82, &a, &b, &c)` tagged
  `@original`. VC6 still emits a direct `call 0x475ff0` with all six pushes - byte-identical to the K&R
  form - while the ordinary two-argument calls become Q7-clean. Verified: 0x5d4080 / 0x5d42b0 kept their
  byte counts to the byte.
- **A struct index vs a char offset**: `ai_ScreenModes[i].w` (an 8-byte struct) compiles to
  `and eax,0xff; mov ecx,[eax*8+base]`, while the original of 0x5d5640 scales the index itself
  (`and eax,0xff; shl eax,3; mov ecx,[eax+base]`) - i.e. a char-offset index. The typed spelling cannot
  produce the `shl`; the char-offset one (`*(int *)((char *)table + i * 8)`) is a dereferenced pointer
  cast (Q4), so the two goals conflict (a shared-header view decision for match-deep).
- **The pending-argument cleanup split is the last hunk of the whole window family**: after a call whose
  arguments stay on the stack, VC6 may emit an `add esp,8` (or 0x40) at a *different* statement boundary
  than ours and compensate at the next call (`add esp,0x18` vs `0x20`). The same body shape gave
  ai_LoadGameWindow / ai_JoinGameWindow **behaviour** (the same instructions moved) but left
  ai_SaveOptionWindow / ai_SaveNameWindow / ai_LoadOptions with code hunks: only the immediates of the
  cleanup differ, so the grade's multiset rule sometimes accepts them and sometimes not. Nothing in the
  source chose it; the twins differ only in how many statements sit between the calls.

## From the 2026-10-27 match-base session (ord.c)
- The generic unaligned block copy has four operand shapes (size/dest through temps and source named directly;
  both through temps; both named directly; dest named directly): the shape tells where the copy came from. A unit
  that cannot include d3.h (its DDPIXELFORMAT needs <ddraw.h>/<d3d.h>) gets the small macro family locally
  (bmp.c's BMP_COPY is the precedent); in ord.c ORD_COPY / ORD_COPY3 / ORD_COPY_AT matched 0x4f9190, 0x4f9220 and
  0x4f9710. The inline asm forces the ebp frame even when the rest of the function wants an esp frame.
- A `switch` that returns 1 for every handled case and 0 for the default: writing `break` in the cases,
  `default: return 0;` and one `return 1` after the switch makes VC6 store the constants as immediates and
  duplicate `mov eax,1; ret` into the case tails (0x4f9710). A `return 1` inside each case instead CSEs the
  constant into eax and stores `mov [mem],eax`.
- A guard before a division should be written in the direction of the original's jump: `if (b != 0) return
  (float)a / b; return 0.0f;` gives `je` to the zero return and the division first (0x4f97c0); `if (b == 0)`
  inverts both and swaps the FPU blocks.
- A `gm_AddTime(c, hours, mins, secs)` call is visible in the stack cleanup (`add esp,0x10`): a draft that casts
  it to a 2-argument callee is a meaning bug, not a spelling (0x4f9220).
- Adding one extern declaration to gv.h can flip a function in another unit that includes it (ai_TimeHour flipped
  d2_DrawObjects 93 -> 97 asm). If only one unit needs the name, declare it in that .c as the other headers do.
- **The comparison idiom says where the operand lives**: `test ah,0x41; jnp` is `<=` against a *constant*
  (0x4f9919, 0x4f993b), while two *variables* give `and eax,0x4100; jne` + the reverse compare
  (`fld x; fcomp y` then `fld y; fcomp x; test ah,5; jnp`, 0x4f997f). If the running value is in st0 the same
  source compiles to one `fld y; fcomp st(1); test ah,5; jp` instead - so the idiom difference is an
  *allocation* hunk, not a source-operator difference. A `<=` min accumulator keeps the later column on a tie
  (`<=`, not `<`): with all four columns at 0 a `<` chain reports column 0 and a `<=` chain column 3.
- **`float x = 0;` vs a `sum = 0;` statement next to a table initializer**: a local array with an aggregate
  initializer shorter than the array zeroes the tail through a register (`xor eax,eax` once, then
  `mov [slot],eax`), while `w[10] = 0; w[11] = 0;` statements store immediates. The zero register then swallows
  a *later* `sum = 0;` (`mov [sum],eax`); declaring and initializing `float sum = 0;` before the table keeps the
  immediate `mov [sum],0` the original has (0x4f9d40, 40 -> 19 asm).
- **A 16-bit field test on a 32-bit copy**: for a 0x0e clock view (`int year; int hourWord; ...`),
  `(unsigned short)clock.hourWord > 6` keeps the original's `mov ecx,[mem]; cmp cx,6` (one 32-bit load reused
  for the full-int stamp copy) where a `*(unsigned short *)(p+4)` deref is a Q4 violation and a separate
  `unsigned short` extern makes a second load. Q4 clears with no byte change (0x4f9c00).
- **Q7 with a shared K&R declaration**: when the other units call the callee with an int address literal
  (`gm_MinutesBetween(rec, 0x770ae0)` in he.c / sv.c / gv.c) the shared header cannot carry a prototype (each
  TU would be a lint cross-TU conflict), and a prototype in one .c is one too. The exemption tag must sit on the
  *call line itself*: `x = f(a, b);  /* @original: ... */` clears Q7, a comment block above the line does not.

## match-base 2026-10-27 (hst.c text commands)

**A `code` hunk of `imul` vs `fild`+`fimul` is a source *type*, not allocation.** 0x4b0d40 had
`n = (int)(cnt * p * 0.01f)` compiling to `imul ecx,esi; mov [esp+0x18],ecx; fild; fmul 0.01f`, while the
original did `fild cnt; fimul p; fmul 0.01f` - an integer product can overflow to a different value, so this is
a meaning difference, and the extra `imul` result temp also shifted every slot (frame 0x124 vs 0x12c, d1/d2 at
0x10/0x14 vs 0x1c/0x18, msg at 0x2c vs 0x34). `(float)cnt * p * 0.01f` reproduced fild/fimul/fmul exactly and
the whole function fell from 149 to 6 differing instructions in one edit. Check the FPU opcode family before
calling an arithmetic hunk scheduling: `fimul`/`fidiv` mean the operand is an int in memory, `fmul` a float.

**The instruction diff cannot see which string constant an initializer copy uses.** 0x4b0d40's
`char name[2][16] = { "EVANGELISCH", "KATHOLISCH" }` compiled to the same shapes as the original's
`{ "KATHOLISCH", "EVANGELISCH" }` (only the `ds:0x601c20` source address differs, and asmdiff masks numbers
>= 0x400000). Read the .rdata source of each copy (`uv run dump` the addresses the prologue loads) and match
them to the array elements, then check what each element index *does* (here it set the religion byte
0x0c the loop counts, so the two commands were exchanged).

**`msg.time = ai_Time` compiles byte-identically to `TMX(msg) = NOWX`** (the 3-int + 1-short copy) and is
Q4-clean - it is the cheap way past the TMX deref-cast macro where the schedule allows it (0x4b0d40 and
0x4b0fd0 both byte-neutral). Try it before the AITIME/AITIMEX header work.

**A global 2D array whose row stride is not element_size * count needs a block struct.** `init_StockBlocks` is
0x2a90 bytes per player (0x10 header ints then 0x55 records of 0x80), so `G12STOCK [][0x55]` (stride 0x2a80)
reads the wrong player's records and compiles to a different address chain. Declare
`typedef struct { G12STOCK rec[0x55]; char pad[0x10]; } G12STOCKBLOCK;` - the records first keep the symbol
2 at 0x7d6a70, the tail pad gives the 0x2a90 stride, and the address chain then matches the original
(`0x7d6a70 + player * 0x2a90 + i * 0x80`): 330 -> 275 differing instructions.

**Passing `&e` instead of `e.field` costs 2 instructions but is a meaning bug.** 0x4b11e0's three stock loops
hand `e.unk08` (record + 8) to 0x4c56d0, which memcpy's 0x80 bytes: the original's `lea ecx,[esp+0x54]` with
8 bytes of pushes on the stack is e+0, and e's `rep movs` destination is [esp+0x4c]. Reconstruct the local's
base address (`lea edi,[esp+N]` of the copy) before comparing pointer arguments - a hunk that only differs in
`[esp+0x54]` vs `[esp+0x5c]` looks like a slot shift.

**`r = 2; return r;` inside a loop folds the constant; `r = 2; break;` keeps the original's copied epilogue.**
hst_NextEvent / hst_NextEventCmd (0x4b3280 / 0x4b35e0) end a do-while with `else { r = 2; ... }`. Written as
`return r`, VC6 forward-substitutes the 2 and the cold path becomes `pop edi / pop esi / mov eax,0x2`; the
original has `mov esi,2 / ... / mov eax,esi` (the value stays in the variable and the epilogue is duplicated
for the break path). `r = 2; break;` - the loop exit then loads r - reproduces it and turns the remaining
code hunks into moved instructions (B3 -> B2). Same lesson as the "one return at the end" rule above: a
shared `return r` after the loop is what the original has, and the break path gets a copied epilogue.

**A local `p = base + off + i` suppresses the per-call reload of a global that the original spells out.**
init_SetSeasonTextures (0x5128d0) walks d3_Terrain + 0x1a64 + i and calls strstr / tex_FindFile on each entry:
the original reloads `ds:0x633738` before every call (a call may change the global), ours kept the pointer in
a register, and two more local aliases (mark / fmt) turned the original's four near copies into one shared
loop. Writing `(char *)d3_Terrain + 0x1a64 + i` at every use and unrolling the four seasons took 511 -> 82
differing instructions. The same shape made the character loop of 0x512f30 pick the wrong registers.

## match-deep 2026-11-09 (d3sndw_Update, lobby_ShowPlayers, lobby_ShowCity, dd_SetMode)

- **A run of zero stores in their own register is an inline memset**: d3sndw_Update's original clears
  found[0].type from the function's zero register (ebx) and the next five ints from a fresh `xor esi,esi`
  (stores in the order last, first, second, ...; dir.y = 0.0f later reuses esi). Six `= 0` lines put all
  stores into one zero web and moved the whole function's allocation (151 differing -> 14); `found[0].type
  = 0; memset(&found[0].vol, 0, sizeof(found) - sizeof(int));` gave the original's code. When the zero
  stores of a block use another register than the rest of the function's zeros, try a memset over them.
- A non-const global holding 0.0f (`ai_FloatZero`, M10 "the original refers to the datum") instead of the
  literal also moved the schedule elsewhere in d3sndw_Update (a latch's reload order): write the object
  when M10 names it, even where the value is the same.
- **`u = a; if (c) u = b;`** loads the default before the test; the original's `test; mov eax,[a]; je; mov
  eax,[b]` (test first, then the default) is `if (!c) u = a; else u = b;` (lobby_ShowPlayers, byte).
  `if (c) u = b; else u = a;` and the ternary `c ? b : a` give a third form.
- **Callee-saved pushes shrink-wrapped after an if/else** in ours while the original pushes them in the
  prologue: lobby_ShowCity's `players = g;` after two identical arms; writing it at the end of each arm
  (VC6 merges the arms' tails) kept the pushes in the prologue - byte. At the start of each arm or before
  the if moved the load instead.
- dd_SetMode: `dd_RMask = r = ddpf.dwRBitMask;` funnels each mask through eax (the original's code);
  `dd_RMask = ddpf..; r = dd_RMask;` changes the two bit-scan loops' spill choice to the original's
  (counter in the dead bpp slot, r in [ebp+0x20]) but loses the funnel - the loop registers are still a
  renaming apart. Loop forms (up / down / while(bpp--)), counter variables and declaration moves compile
  to the same code there.

## match-deep 2026-11-09 (lobby_Screen, evt_Step, evt_Eval, rt_EditPointDialog)

- **A run of constant field stores whose schedule is off: write the fields in field order.** lobby_Screen's
  defaults (`reserved = -1; talents[0..4] = 0x5a, 0x46, 0x78, 0x46, 0x5a`) were grouped by value in the draft
  (both 0x46 first); the original's "load cm_NetGame.mode first, 0x5a in cl" came from plain field order.
  Generated all 720 orders with `dcp try` (17 s): 12 orders give the original's code, every one of them with
  `reserved` first and talents[0] before talents[2]. A decompiler grouping by value is the usual culprit.
- **One counter for two loops**: lobby_Screen's player pick (`for (i...) {... sel = i; break;}` then `for (n...)
  if (n != sel)`) kept sel in i's register; reusing `i` for the second loop and writing `sel != i` gave the
  original's `mov edi, ecx` copy and its register roles (draft 66 -> shape 2). Then `d2_ClickedId = -1;` after
  the last d2_Inputs field store (not between them) made it byte.
- **`if (!x || (f(), !x))` vs `if (x) f(); if (!x)`**: evt_Step's original keeps the global evt_Depth in edx and
  reloads it right after every call (a promoted global web); the comma form loads it once at the use. The split
  form gave the reloads. Writing the pop twice (no `goto pop` into the shared tail) gave the -1 constant its
  callee-saved register (`or edi,-1`, `cmp eax,edi`). The two copies then needed different spellings - early
  `cur = stack[d - 1]; d--; stack[d] = 0;`, at the end `d--; cur = stack[d]; stack[d] = 0;` - for VC6's
  cross-jump between them to land where the original's does (generated 49 pairs, one byte).
- **A switch-table entry to the switch end where we share a case**: evt_Eval's `case 1: case 7:` read a float
  variable as an int; the original's table sends type 7 to the end (val unchanged), a meaning fix. An empty
  `case 7: break;` is dropped and shrinks the table; `case 7: if (0) {} break;` keeps the entry (so does
  `val = val;`). `dcp try` folds such a variant into "= control" (its code hash ignores the tables): apply and
  build to see a table change.
- **Relative-addressing strcpy (`sub edx, src; mov [edx+eax], cl`) where the original copies with two
  pointers**: evt_Eval's `strcpy(evt_StrResult, (char *)(val = (int)(v->data.s + idx * 0x60)))`; writing the
  element twice as a row of a `char (*)[0x60]` view (`val = (int)((char (*)[0x60])v->data.s)[idx];
  strcpy(evt_StrResult, ((char (*)[0x60])v->data.s)[idx]);`) gave the two-pointer copy. The linear spelling
  `v->data.s + idx * 0x60` in the same two statements did not. A `char (*str)[0x60]` member added to the union
  in evt.h did the same but cost ai_TutorialInitChapter2 (sv_t) its byte (declaration set): the cast stays local.
- **Initialised locals' declaration order schedules the entry stores**: evt_Eval's `int next = 2; int val = 0;`
  (instead of val first) moved `xor edi,edi` after the flag load as in the original - byte. Generate all
  orders of the initialised declarations (and the statement form) when only an entry order hunk is left.
- **`lea r,[base+idx*4+disp]` where the original has `lea eax,[base+idx*4]; lea edi,[eax+0x23]`**: the record
  address is a named pointer and the array member is indexed through it: `bt = &ai_BTypesT[b->type];
  for (j = 0; ... bt->slot[j] ...)` (ai_HasThreeStockedBuildings, byte). A pointer walk `w = bt->slot; w++`
  folds the member offset again (= the one-lea code), and so does indexing `ai_BTypesT[b->type].slot[j]`
  without the record local (worse: two IVs).
- **One record's fields scaled into 16-byte locals**: d3_TransformVerts' weight block (`fld x; fld y; fld w;
  fmul st,st(2) ... fstp st(0); fstp st(0)`, z by integer mov) is closest without volatile hacks as
  `D3VECTOR4` locals filled field by field and then `s.x *= w` (frame equal); a `D3VECTOR` struct copy
  shrinks the frame by 0x18.
- **A loop-invariant call argument held in a local swaps two callee-saved webs**: the mission dialogs'
  `flags = id | 0x80; do {...} while (sv_RunFrame(fn, flags));` put c in ebp and flags in ebp's partner
  (`or ebp, 0x80`); the argument written inline, `sv_RunFrame(fn, id | 0x80)`, gives the original's `mov ebx,
  esi; or bl, 0x80` and the c/t colours (ai_MissionResultDialog, ai_MissionPartnerDialog byte;
  ai_MissionCompleteDialog draft -> shape, then byte from the zero-init order again, quit, nm). Look for the
  same local in sibling dialogs (`grep "flags = id | 0x80"`).

## match-deep 2026-11-09 (d3t_PlaceObject, d3t_MoveObjectPath, d3t_CreateSnowMipmaps, mov_Open)

- **Call-argument setup hoisted into the FPU result stores** (`lea eax,[keys]; push eax; push 2; sub esp,0xc`
  scheduled right after an early store, while the original finishes the field copies first): the early store
  into the array that is passed by address (`keys[0].time = unk`) sat in the source right before the call. A
  store into the passed array is the last thing VC6 orders before the pushes; move it next to the call
  (d3t_PlaceObject, 26 ins -> byte; an earlier worker had permuted every other store of the block).
  Sibling: the same move in cm_ThrowGrenade (all `keys[n].time` stores next to the call) went 48 -> 26 ins,
  still shape (left unapplied).
- **Same-GUID symbols of a library**: strmiids.lib defines CLSID_FilterGraph and CLSID_FilgraphManager with
  one value; an M8 naming the second means the original wrote that name (mov_Open). Same for the `""`
  window title: the original passes ch_EmptyName (M9), as other units do.
- **The zero constant shared with a return variable** (mov_Open: `id = 0`, every literal 0 and the final
  `mov eax, esi` in one callee-saved register; ours spills id to the frame): no declaration order, init
  placement, early-return form or id spelling made VC6 coalesce them. Not solved.

## match-deep 2026-11-10 (ahm_AddValue, ahm_FillSteps, ahm_StepValue, ahm_LoadIni)

- **Float clamp `fcom 0 / and 0x4100 / jne; fcom 1 / test ah,5 / jp; fcom 0 again`** (the 0 test repeated after
  the 1 test, the stores 0 / 1 / f in three tails): `f = min(max(f, 0.0f), 1.0f);` on a named local. With the
  sum inside the macro VC6 copies it (`fld st(0); fcomp 1.0`); `max(0.0f, f)` or `max(min())` reorders the tests;
  two `if (!(f > 0)) f = 0; if (!(f < 1)) f = 1;` statements give jnp at a join (ahm_AddValue, draft -> byte).
  The `ai_FloatZero` / `ai_FloatOne` M10s are the pooled literals: writing the objects was worse.
- **Strength-reduced loop base**: the pointer base VC6 picks for `ai_WalkSteps[k].field` follows the order of
  the stores after a struct copy: `grid = ...; on = 1; y = ..; x = ..` put the base at the record start
  (`mov ecx, ai_WalkSteps`, `lea edx,[ecx+4]` for the grid), `grid; y; x; on` put it at `.y`. A pointer loop
  (`s->`) never reached it; an index loop with the struct assignment `ai_WalkSteps[k].grid = tbl[idx++ % n]`
  (not three word copies) did. Permute the stores with the index loop (ahm_FillSteps, 30 ins -> byte).
- **Squares folded into the sum one by one** (`fld st(0); fmul st,st(1); faddp st(2),st; fstp st(0)` per
  term): one temporary reused, `x = ..; sum = x * x; x = ..; sum += x * x; ...`. Four locals a..d (any
  statement order or association) keep all four values on the FPU and square them at the end.
- **`v * 500 + sum * 500` factored to `(v + sum) * 500`** by VC6; the original's two fmuls come from
  `v = v * 500.0f; return (int)(v + sum * 500.0f) << 5;` (ahm_StepValue, 42 ins -> byte).
- **A failure cleanup in the middle of the function** (the free-both-tables block right after the last
  alloc test, before the last loop, the other failures jumping into it): the source had the cleanup inside
  `if (alloc == 0) { fail: ...; fail2: ...; return 0; }` with `goto fail` from the earlier tests. A cleanup
  written after the nested ifs is always placed last (ahm_LoadIni, 48 ins -> 4). Its `if (n > 0x12) {if (n
  >= 0x1e) ..} else n = 0x12` clamp with the else-arm out of line after the cleanup is
  `min(max(n, 0x12), 0x1e)` (4 -> byte).
- **A one-use local that takes a callee-saved register from a constant**: `k = (int)(f * 0.2f); ...
  util_Random(k)` gave k a register web that pushed the original's `xor ebx,ebx` constant 0 out of ebx for the
  whole function (immediates and `test` instead of `push ebx` / `cmp eax,ebx`, a pointer cached in ebx instead
  of reloaded); writing the call argument inline took ahm_CreateAlchemist from 104 to 13 ins. Look for such
  locals first when the original keeps 0 in a register and ours does not.
- **A global counter's store after unrelated field stores** (`mov [ai_NextObjectId], ecx` after the c->mask
  store, the load and `inc` before): write the `x++;` statement after those stores (the read `c->netId = x;`
  stays put). ahm_CreateAlchemist 5 -> 3 ins, gv_RaiseDead 103 -> 51. Moving the read line instead changes
  the meaning (netId + 1): check what a `--move-line` variant moved before applying it.
- **A dead store the original keeps** (`mov word [esi], 0xffff` before `c->id = n`): through the record
  pointer (`c->id = 0xffff`) VC6 drops it as dead, through the array (`ai_CharsT[n].id = 0xffff`) it keeps it.
- **Table search with two pointers** (an iterator in ecx, `mov edi, ecx` each iteration, `cmp [edi], ebp`
  reloading the field the loop test already loaded, and the first test on `[table]` before the loop): an index
  loop with a pointer copy, `for (n = 0; T[n].kind != 0; n++) { e = &T[n]; if (e->kind == ...) goto found; }`.
  A pointer loop, or `e = p` from a second pointer, coalesces both (snd_EnterBuilding, 38 ins -> byte).
- **Walk until the terminator, the test after the step**: `i = 0; do { ... } while (T[i++].kinds[0] != 0);`
  (snd_FindTrackKind, byte); a pointer `(l++)->kinds[0]` or a `k` pointer for the inner walk keep the
  terminator in a register.
- **Char array frame gaps**: VC6 rounds char[0x11..0x14] up to an 8-byte multiple in the frame (sfx[0x14]
  left a 4-byte hole and moved num/buf); the original's packing came from sfx[0x10] (snd_FindVoiceSample,
  measured with deep-tools framesearch.py over sizes in a few seconds).
- **One merged return block vs a test-then-return path** (`mov [eax+8],1` immediate on one path, `mov esi,1`
  only after the call on the other; `test ecx,ecx` re-testing a cached field after the first `if` fails):
  the first `if` is a combined `if (a == 0 && b < c) { ...; return; }` followed by `if (a != 0 && ...)`, and
  the constant store sits in both arms of the inner `if` (`{ play; x->playing = 1; } else x->playing = 1;`).
  snd_UpdateQueue 47 ins -> byte.
- **Independent bit-field unpack statements reorder the byte registers**: `mt.c5 = c2 >> 1 & 1;` before
  `fl.s.c1 = c2 & 1;` moved the bit insert into bl and fixed the later argument scheduling of two calls
  (d3_FindBGF 26 ins -> byte, with deep-tools permute.py over the four lines; 1 of 24 orders).
- **Init order of scalar locals** (`women = men = 0; w = m = 0xffff; f = 0`) steers which register copy
  (`mov edi, ebx`) VC6 schedules between the zero stores: permute the init block (snd_PlayFavourComment).
- **A fallback speaker written as `m = -5; f = 1;` falling into the shared `if (f == 0) .. else if (f == 1)`
  chain** gives the original's `mov edi, -5; push edi` and the per-branch reload of the bank global (a
  literal `snd_PlayVoice(-5, ...)` call keeps the global CSE'd in edx across the tail). snd_PlayFavourComment
  41 -> 10 ins.
- **Two float locals' frame order** (f / t swapped between [esp+0x10] and [esp+0x14]) did not move with 16
  extra references to f, but did when one branch's use of t got its own local (snd_UpdateRain: `tm` in the
  0x95 branch), 80 -> 33 ins. When reference counts do not move a slot pair, try splitting a variable whose
  uses are in disjoint branches.
- **A pointer that the original keeps in its own register after `e = d->entries = mem_Alloc(..)`** (ours
  e in eax, the original `mov ecx, eax` and every store through ecx): store the allocation to the field
  first and derive the others from the field, `d->entries = mem_Alloc(..); s = (char *)(d->entries + 1);
  e = d->entries;` (e assigned last). Splitting the chain the other way (`e = ..; d->entries = e;`) was
  = control. vfs_AddFile 29 ins -> byte.
- **Callee-saved set without ebx** (ours: the walk pointer `dir` in ebx, a copy pointer in esi, edi pushed
  and popped locally around a strlen; the original: dir esi, copy pointer edi, ebx never used): a second
  copy of the parameter into the local at the loop (`VFSDIR *dir = d; ... for (dir = d; (p = ..) && dir; )`)
  gives the parameter's own candidate the ebx that never materialises. Using `d` itself (no local) got the
  registers but loaded d late (per path); moving the single init around changed nothing. vfs_FindDir
  38 ins -> byte (an `@original` note keeps the redundant copy).
- **Flag locals initialized by statements, not initializers**: `read = 1; z = 1; detect = 1;` (read first)
  gave the original's `mov byte [read], 1` immediate where `unsigned char read = 1` shared z's `mov bl, 1`.
  A gzip detect `if (size <= 10 || m0 || m1) z = 0; else z = 1;` laid out as the original (the z = 0 path
  in the fall-through, merged with the later if (z)'s else, and the z / read slots swapped back) only as
  `if (size <= 10) z = 0; else if (m0 || m1) z = 0; else z = 1;` (&&, ?:, z = cond, reversed forms all
  = control). vfs_MemOpen 37 -> 2 ins. Left: an early `return 0` whose `xor eax, eax` the original hoists
  to the entry and joins past the final `mov eax, esi` (not reached by ret locals, f = 0 placement, or
  flat / nested / duplicated failure returns).

## match-deep 2026-11-10 (vfs_Tfopen, err_ExceptionFilter, hmt_Init, hmt_DecideElection + AIMSG sweep)

- vfs_Tfopen (draft 152 -> shape 44, stuck): the original's f has a stack home ([esp+0x24], stored at entry
  and after the zip pool_Alloc, reloaded in the not-found block) because VC6 cannot fold `return f` to 0
  there. Ours had `notfound: sprintf(..); return f;` reached by gotos from paths where only f = 0 reaches,
  so VC6 emitted `xor eax,eax` and its own epilogue and f lost its home. Rewritten as nested
  `if (e) { if (e->zip) {..; if (zip) {..; unzClose(zip);} } else {..; if (fp) {..; opened: ..; goto done;} } }
  sprintf(not found);` falling through to the final `return f;` (152 -> 106 -> 66), the write path's missing
  entry as the else of `if (e)` (-> 46, shape), `binary = 1;` before `read = 1;` (-> 44). Left: the cross-jump
  direction of the identical success tails (binary / compressed / count / return): the original keeps the read
  path's copy (falling into the epilogue the early argument checks reach) and the write success jumps to it;
  ours the reverse. Not moved by: read success `return f` (86-150), write success `return f` (53) / `goto
  done` (= control), the write branch first (352), `goto end` at the old label (239).
- err_ExceptionFilter (stuck, tie-break sensitive): the frame is 8 bytes larger with a separate `char win[7]`
  for the stack walk's code window; the original reads it into `text` (lea ebx,[esp+0x90] = text, inc ebx per
  shorter read; err_CheckCall(text + 7)). The registers then still follow the declaration set (2 codegens in
  32 samples).
- hmt_Init byte: returns hmt_ReadBinaryFile's result (eax live to the end gives the original's ecx / edx loads
  and their order; void + the same statements is hmt_Reset's code).
- hmt_DecideElection byte: `n >= 4`, `cand[i].score > best`, the AIMSG declared in the send branch, fill
  order type, to, time, fromId, flags, arg[5], arg[4]. Without the block scope the best of all 5040 fill
  orders left the `mov byte [msg.type],0xa5` before the time copy (2 ins); block-scoped it lands after it.
- The block-scope lever swept over 112 below-byte functions with a function-scope AIMSG (deep-tools
  blockscope.py writes one whole-function variant per inner block; variants whose block does not hold every
  use fail to compile): byte ai_MsgWorkerStop (stuck since session 3, ~250 orders tried), ai_StartBuildingWork,
  ai_UnloadCarts, he_PostWorkshopEvents98, ai_MsgFreeBuildingObjectType2e, ai_CabinetSpyOnCharacter,
  ai_CabinetSpyByMoney, ai_GraveAgeContact; plus he_RunFire and ai_BlackmailBuildingListener with one more
  lever each; ai_CabinetIntrigueWindow draft -> shape. Closer but below: gv_GraveyardTurn, he_RunHarvest,
  he_RunGraveyardWorkers, ai_StorageWindow, ai_CourtshipWindow. No gain (block scope = control or worse):
  the other ~95, e.g. the lc.c window functions' big fills.

## match-deep 2026-11-14 (d3anim_StepObject, d2_StartAnim, d2_ShpBlit16Scaled, d2_MakeLightTable, d2_ShpConvert24To16, d2_DrawShapeScaled)
- d3anim_StepObject (1578 ins, draft, stuck): frame 0x68 vs the original's 0xbc. Every 3-float group in the
  original's frame is followed by a 4-byte hole: all vectors were D3VECTOR4, declared in the blocks that use them
  (RootPosAt's out, the attach block's three, the path's v / w / tin / tout); 0x68 -> 0x9c, 1331 -> 1184 ins.
  A meaning bug hid among the 1074 "code" hunks: the end-tangent clear tested `P->next < P->key - 1` and
  zeroed keys[key - 1] / keys[key - 2], the original reads [esi] / [esi+4], i.e. `P->key < P->nkeys - 1` and
  keys[nkeys - 1] / keys[nkeys - 2] (-> 1144). The @deep note had called the function "allocation only":
  on a big draft, check the field offset of every load that feeds an index before believing that.
- d2_StartAnim byte: a hand pointer walk `for (i = 0, a = T; (int)a < (int)T + 0x110; i++, a++) if (!a->bank)
  { if (i >= 16) return -1; ..fill..; return i; }` was `for (i = 0; i < 16; i++) if (T[i].bank == 0) break;
  if (i >= 16) return -1; ..` (the amt_GetFirstOffice rule), then one of 120 store orders (bank before x).
- d2_ShpBlit16Scaled byte (24 ins, "allocation only"): `d = line;` BEFORE `first = row % n == 0;` at the row
  loop head. With first computed first, a split piece of the row counter got ebp (its own `xor ebp,ebp`, reloaded
  at the latch); swapped, row stays in memory and the remainder of the idiv takes ebp, as in the original.
  c2trace: the clipped loop's row piece had savings 4, prio 56 - a marginal candidate that one statement order
  flips. Both loops written the same way (the unclipped one compiles identically either way).
- d2_MakeLightTable (stuck at shape, 20 ins): a byte-flagged constant 0 that lives across a call can only get
  ebx, and so can a byte parameter (`add`) used in the loop. The zero (r = g = b = 0 stores, cmp eax,ebx after
  mem_Alloc) had prio 6 against add's 9, and lost ebx while a `lim` local (prio 35) existed; with `0xff - add`
  written at each compare (a CSE temp, prio 25) the zero gets ebx and add takes it after (bl). `unsigned char r
  = 0, g = 0, b = 0;` at the declaration gives the original's store order. Open: i (19) against the temp (25)
  for esi; flip.py says the temp has one compare too many.
- d2_ShpConvert24To16 byte (38 ins): (1) both arms begin with `r = mem_Alloc(s->size - s->npix, ..)`; the
  original loads npix and size BEFORE the branch (`mov edi,[npix]; cmp; mov eax,[size]; push str; je`) and
  subtracts in each arm. That is VC6 hoisting identical leading instructions of the two arms after allocation:
  they hoist only when both arms got the same registers. `n = s->size - s->npix; r = mem_Alloc(n, ..);` in each
  arm did it (locals for size / npix before the if were far worse). (2) a 3-byte pixel walk the original reads as
  [esi-1] / [esi] / [esi+1] with esi = run + 9: `for (k = 0; k < src->n; k++) .. pix[k * 3], pix[k * 3 + 1],
  pix[k * 3 + 2]` (VC6's middle-element IV, see d2_Convert16ToRgb), not a do-while with `pix += 3`. (3) the
  unrunned arm's `pix = base + (w * i + j) * 3; pix[0..2]` -> `pix = base; pix[(w * i + j) * 3 + 0..2]` gives the
  original's `lea ecx,[ebp+eax*2+0x32]; add eax,ecx`. (4) statement orders of the pointer init and the row latch.
- d2_DrawShapeScaled byte (66 ins, "pure allocation"): the original's `mov eax,1` right after the prologue (the
  mode >= 0 path returns it untouched, edi pushed for `old`): `int r = 1; .. case 1: if (mode < 0) { if (on)
  r = Lit(..); else r = Blit(..); } return r;`. Returning the calls directly (`return Lit(..)`) folds r away
  whatever the other returns say (8 combinations = one code).

## match-deep 2026-11-14 (second shp session): the grabbers' row pointer, the key word, sibling copies
- **The per-row pixel pointer that the original recomputes every row** (0x47a460 / 0x47ab30 / 0x47ad40: `imul`
  per row INSIDE the inner loop's `w > 0` guard, our row pointer strength-reduced over the rows into a running
  pointer + a pitch*n slot): compute the address inside the inner loop body, `for (c = 0; c < w; c++) { p = pix +
  ((r + y) * pitch + x + c) * 3; ..}` or the plain index `src[(y + i) * pitch + x + j]`. VC6 hoists the invariant
  row part into the inner loop's preheader, which sits behind the guard, so the outer loop never strength-reduces
  it (an explicit `if (w > 0)` around the row pointer shows the same mechanism). The row pointer assigned before
  the inner loop is SR'd whatever its spelling (the "not the source spelling" note above, line ~1637, was this).
  d2_GrabShape16NoRuns 183 -> 6 ins, d2_GrabShape24NoRuns 274 -> 34 ins.
- The copy loop of the same functions, whose original DOES strength-reduce (stride in a dead parameter slot,
  the row pointer computed behind the `s->h != 0` guard): `for (i = 0; i < s->h; i++) { p = src + (y + i) * pitch
  + x; memcpy(dst, p, s->w * n); dst += ..; s->npix += ..; s->size += ..; }` - not a hand-advanced `p += pitch`
  in a do-while (that puts the base computation before the guard). `dst +=` first, then the two counters.
- **A key compared / passed as `r` + the word `gb` (D2KEY, packed, gb at offset 1)**: the original loads gb with
  one `mov ax, word [key+1]` (al = g, ah = b, `mov cl, ah`) and r as a byte; ours splits gb into byte loads and
  merges r + g into one dword / word load. Every spelling of the access is one code (casts, `>> 8`, `/ 256`,
  LOBYTE / HIBYTE, bitfields g:8 b:8, a pointer, a word view at +1, locals), and D2KEY's gb as a union in the
  header too (that header change also cost sv_t's ai_TutorialInitChapter3 + 1 more their byte). A dcp try probe
  with gb as its own symbol (`{ extern unsigned short probeGB; .. }` or a struct whose word sits at offset 0)
  gives the original's instructions: VC6 splits a word it knows sits at an ODD offset of its symbol. So the
  original's declaration had gb at an even offset of some symbol - open, a data question (gapdata) for the five
  shape grabbers (16NoRuns 6 ins, 24NoRuns 34, 8Runs, 16Runs, 24Runs all read the key this way).
- **A run-length blit's grey value passed three times to dd_RGB(unsigned char, ..)** (d2_ShpBlit16HalfDark: the
  original pushes `_ftol`'s eax three times, no store): the grey expression written out in each argument (a macro
  in the original); VC6 CSEs the three copies into one `_ftol`. A named `unsigned char v` goes through a stack
  slot (`mov byte [v],al; mov eax,[v]`), an `int v` changes the allocation of the whole function (128 ins). The
  out-parameters of dd_SplitRGB were `unsigned char r, g, b` (the original's `mov eax,[r]; and eax,0xff` loads of
  aligned byte locals), not ints with `& 0xff`. 128 -> 2 ins; the last two are three split-piece reloads at one
  edge in ascending candidate id (c2trace), no spelling found.
- **A sibling already byte is the source**: d2_ShpConvert8To16 (224 ins) was d2_ShpConvert24To16's text with the
  palette lookup - `if (0 < *srccnt) do .. while (m < *srccnt)`, `src[1]` read at each use, the pointer chain
  `srccnt = src; dst = dstcnt; dstcnt + 1; src + 1` - plus `n = s->npix + s->size; r = mem_Alloc(n, ..)` (the
  inline sum put the size load into esi instead of edi and rotated every scratch register after it: 30 -> 0).
  d2_ShpBlit16ScaledLit (251 ins) was d2_ShpBlit16Scaled's text with the light table and only the top / bottom
  clips (`unsigned char n`, `int y`: the original's `and edi,0xff` / `idiv` / `jl`), cp / run / line in the old
  order: byte in two rounds; the hand-guarded do-whiles and the `next` pointer of the old text were the optimum
  of the wrong form.
- `maxy = maxx = 0;` stores maxx first (the original's order) where every order of two separate statements
  stored maxy first (d2_GrabShape16NoRuns).
- dcp try: a whole-function variant that starts at the definition line (no `/* 0xADDR */` comment) applied with
  --apply drops the function from the ledger (`dcp fn: no function`); put the comment back from git. Variant
  files with backticks in a `dcp stuck` text in bash: the shell runs them - quote the reason with single quotes
  or pass it through a file.

## match-deep 2026-11-14 (third shp session)
- **d2_ShpBlit16Rows byte (332 ins, draft, "codegen / register allocation only")**: four ordinary changes.
  (1) The goto-shared clip tails (`clip1:` / `clip2:`, a `rows` temp reused for the x clip) as plain if/else with
  `vis -= skipl; clipped = 1;` written in each arm (VC6 cross-jumps them into the original's shared tail) and the
  unclipped arms first (`if (clipped == 0) {..} else if ..`): 332 -> 146, but the frame 4 short (rows in a
  register). (2) No `w` local: `vis = s->w;` and vis in the x clip (`vis + x`, `vis - ClipRight + x`), `s->w *
  rows` in the clipped arms: the frame matched (vis and rows homed), 159 ins of register rotation left.
  (3) No `mode` local: `s->mode == 0 / 2 / 3` in both dispatch chains (flip.py: moving `mode = s->mode;` into
  one arm was the only mutant with the original's `xor edx,edx` zero): 159 -> 8. The original's `mov dl,[s+0xd]`
  hoisted above the clipped test is VC6's own hoist of the two chains' first load. (4) The zero inits first,
  then `vis = s->w; h = s->h;` (720 orders, 1 code at 4 ins), and in the clipped mode-3 arm the row latch
  `d += ..` before `p += skipr` (a reload register ebx / ebp): byte.
- **d2_GrabShape16Runs 334 -> 6 ins (stuck on the D2KEY word only; byte with a probe symbol for gb)**: the
  draft reused minx / miny / maxx as run counters and a `seg` pointer; written as d2_GrabShape8Runs' run loop
  (lines[] / line / run / col / t) with d2_GrabShape16NoRuns' scan it was 74 ins at once. Then (1) a skip loop
  over a hand pointer `p = pix + t + col + x; do {..; p++;} while (*p == key);` that the original strength-
  reduces itself: write the index, `while (pix[t + col + x] == key)`, no p - VC6 then sets its pointer up after
  the `jne` like the original, with p it hoists the address above the test (flip.py hit: an unrelated `x - t`
  in the test broke the CSE and moved it). (2) A literal-run loop whose entry test is unsigned (`cmp; jae`) and
  whose latch test is unsigned as well: `while (run[1] + col < s->w && pix[..] != key) run[1]++;` with run[1]
  unsigned (VC6's rotated entry test knows run[1] == 0). (3) maxx / maxy in swapped equal-density slots: the
  init and reset store orders (312 combinations, one byte: `maxx = 0; maxy = 0; cnt = 0; minx = w; miny = h;`
  and the reset minx, miny, maxx, maxy).
- d2_GrabShape24Runs (488 -> 325 ins, stuck): the 16Runs form transfers (scans from 24NoRuns, the run loop from
  16Runs with `d2_CompareColour(pix + (col + t + x) * 3, ..)` in the loop conditions and `col += run[1] - 1`
  unconditional) and gives the original's code shape, but the run pointer (c2trace prio 1086) is popped before
  s (1081) and the whole function's registers permute. Computing run inside the `w > 0` guard flips s to ebp
  but VC6 then folds run away into `[eax+ebp]` addressing. Open: which ordinary s reference the original had
  in the run loop (flip.py: duplicating `s->npix +=` or `s->size +=` flips it).
- **d2_PlaceWindow byte (15 ins, "CSE / tail-merge scheduling")**: two calls `if (f & 2) d2_MoveObj(x, H - h / 2,
  w->parent); else d2_MoveObj(x, w->y, w->parent);` against the original's two call sites with `movsx eax, word
  [w->y]` (an int conversion before a short parameter) and the parent loaded in each arm: the source had ONE call
  after an int `y` chosen by if / else (`y = ..; else y = w->y; d2_MoveObj(x, y, w->parent);`). VC6 duplicates the
  short call + epilogue into both arms itself. Reading rule: a widening `movsx` of a field pushed to a short
  parameter means an int local in between; identical call tails per arm can be VC6's duplication of one call.
- d2_DrawInput / d2_DrawSlider (stuck, 6 / 8 ins): `o->x - p1->w + o->w` and `w->x + w->w + 4` load the two
  short fields in the other order than the original; every spelling of the sum is one code and `headers sample`
  is insensitive, but flip.py hits on unrelated expressions elsewhere in the function (another `(a - b)` turned
  into `+`, a float constant changed): C1's canonical operand order depends on function-wide state (value numbers),
  not on the statement. Open.

## match-deep 2026-11-14 (fourth d2 session)
- **A short global loaded as a dword (`mov ecx, dword [d2_ScreenH]; .. sub ecx,eax; mov [m],cx`) where the original
  keeps `mov cx, word [..]; sub cx,ax`**: VC6 widens a 16-bit load-op-store chain to 32 bits only when it knows
  the global is 4-aligned, i.e. when the unit defines it (`short d2_ScreenH = 0;`). Probe: `extern short probeH;`
  inside the function in a dcp try variant gives the original's word load, and so does a tentative definition
  (`short d2_ScreenH;`, COMMON: byte, nothing else moved) - but dcp's D5 gate refuses a tentative definition
  at an uninitialised-data address ("write it `= 0`"), so it was reverted. Moving the `= 0` definitions below
  the function changes nothing (C2 sees the whole unit). Open: how the original's unit hid the alignment
  (the definition in another unit / a container object), a data question like the D2KEY word.
- **d2_ResizeWindow 32 -> 4 ins (stuck on the d2_ScreenH word above; byte with it tentative)**: (1) a record pointer `o = &d2_Objs[*p]` for
  the h / y reads instead of two locals `oh` / `oy` (`o->y + o->h`; 32 -> 18), (2) the object loop as the plain
  `for (i = 0; i < wn->nobjs; i++) .. d2_Objs[wn->objs[i]]` instead of the decompiled `n = wn->nobjs; if (n > 0)
  { p = ..; do { ..; p++; n--; } while (n); }` - VC6 makes the same countdown and pointer in the param slot
  itself, and the four clip-sum operand-order hunks far below (`wn->w + wn->x` loading x first, every spelling
  of the sum one code) went with it: the operand order of a commutative sum follows function-wide state, the
  same as d2_DrawInput / d2_DrawSlider's open hunks. Rule: fix the decompiled loop shapes before an operand
  order hunk elsewhere.
- d2_RaiseWindow (stuck): the plain `j = -1; for (..) { if (o) { if (o->id == parent) { j = i; break; } .. } }`
  gives the original's block layout but a whole-function register permutation (78 ins); flip.py: changing
  either search loop's start `0x2ff` to another value leaves 6 ins - the constant shared by two loops is an
  allocation candidate the original did not have. No spelling of it (0x2ffu, casts, 767, sizeof, other loop
  forms, other variables) separates the two.
- **d2_RemoveObj byte (75 ins, "constant-0 register race")**: (1) a search loop the original keeps top-tested
  and unstrength-reduced (`cmp i,n; jge` at the head, `mov edx,[w+0x18]; inc eax; mov edx,[edx+eax*4-4]; cmp; jne
  head`): the condition held both tests, `while (i < n && w->objs[i++] != obj) ;` - the decompiled `for (i = 0; i <
  n;) if (w->objs[i++] == obj) break;` is rotated and strength-reduced, and its extra candidates had moved the
  constant 0 from edi to ebp for the whole function (75 -> 14). (2) A pointer computed through a temp (`lea eax,..;
  shl eax,5; lea edi,[eax+T]`, ours straight into edi; one scratch register late in every later block): the
  address was written out a second time in the compare, `in = &d2_Inputs[k]; if (d2_FocusedInput ==
  &d2_Inputs[k])` (the d3io_AttachStock lever). Also plain `o = &d2_Objs[obj]` / `memset(&d2_Objs[obj], ..);
  d2_Objs[obj].id = obj;` for the decompiled `off` byte offset (same code, readable).
- **d2_RedrawView byte (84 ins, "whole-function allocation")**: two ordinary changes. (1) A texture tail whose
  original tests `tex1[0]` twice (`jne find; test tex2; je ret; test tex1; je ..`) and reloads t from its
  never-written slot on the tex1-empty path: `if (!tex1[0] && !tex2[0]) return; if (tex1[0]) {..}` and NO `t = 0`
  (the original reads an uninitialized t there, an @original quirk; 84 -> 62). (2) A one-register shift of the
  eax / ecx / edx rotation through every later switch arm: the caption position was an if / else assigning both
  values per arm, `if (o->captionX != -1) { ex = o->captionX; tx = o->w; } else { ex = o->x + 6 + tw; tx = o->w
  + tw * 3; }`, not `ex = o->captionX; tx = o->w; if (ex == -1) {..}` (VC6 then keeps `tw * 3` as its own lea).
  60 spellings of tw * 3 / statement orders inside the default-then-override form were one code.
- d2_DrawObjCaption (stuck, 85 -> 62): one call after `if (o->style & 4) { x = o->x; y = o->y + o->h; }` gives
  the original's allocation of the whole head (flags in the dead o slot, 0x20 in ebx, x / y in ebp / edi), the
  two-call form the original's two call sites; no form found that has both (VC6 does not duplicate the call +
  epilogue here as it did in d2_PlaceWindow). tools/c2dag (flip / vdiff / c2trace) cannot find a K&R definition
  (`void f(o) T *o; {`): write it ANSI first, VC6 compiles both alike for pointer parameters.
- d2_DrawObjects (stuck): a text arm's mode / x register race; flip.py's hits all sit in the window clip walk
  above it (the four clamps), not in the arm.
