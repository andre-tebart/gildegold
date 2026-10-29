# d3_sky.c

## Unit
Migrated from the notes/tus.md table on 2026-10-23 (the row's prose, unchanged):
- Name (leaked): D:\develop\Units\d3\d3_sky.c
- Boundaries: separated (proven): d3s.c tag / d3_sm.c tag
- Range then: 0x45ce60..0x45db20

## Open work
- match 2026-10-23 (3rd): d3sky_CreateLayer MATCHED; d3_sky.c fully matched.
- match 2026-10-23: d3sky_FadeLayer MATCHED: `l->alpha0 = l->alpha` in both arms (not hoisted above the `time != 0.0f` test), if arm `alpha0; fadespeed = 1.0f / time; alpha1 = alpha; fade = 0`, else arm `alpha0; fade = 1.0f; fadespeed = 0; alpha1 = l->alpha` (vargen perm of both arms, 576 variants; the zero register appears with the alpha0 store duplicated in the arms).
- match 2026-10-22 (2nd pass), no gain: d3sky_FadeLayer: `s != NULL && l != NULL`, `time != 0` (int zero), `alpha1 = alpha0` in the else arm, a `s != l` third test: the zero register (`xor edx,edx; cmp eax,edx`) never appears (notes/vc6-matching.md: it needs a zero that is not constant-propagated).
- match 2026-10-22, no gain: d3sky_FadeLayer (original keeps a zero register, `xor edx,edx; cmp eax,edx` and zero stores through it, and
  `mov al,[l->alpha]` after the fnstsw; tried `!= 0` forms, `0.0f` stores, `alpha1 = alpha0`, an `int zero` local, alpha1 hoisted);
  d3sky_CreateLayer (pure register map: original s edi / tex ebp / l esi and no ebx at all, ours ebx / ebp / esi).
- match 2026-10-23: d3sky_CreateLayer 23 B / 24 lines, only the register map. c2prio (copy the file to src/game/zz_tmp.c with the `#line`
  lines deleted and D3DCHECK( stripped, since the /FAs listing cannot open D:\develop\...; delete it after): l 133 esi, const#0 14 edi,
  tex 12 ebx, s 10 ebp. The original (s edi, tex ebp, ebx unused) needs s > const#0 > tex, i.e. about +5 for s. Tried, no change of
  priorities or code: decl order of l/ret/p (all 12 spellings), dead `p = s->layers` at 5 positions, `s && tex && s`-style redundant tests,
  `s ? scale : 0`, tex_Load / flags-copy at every position of the init block (worse). A real extra s reference in a block with P>=2 is
  what is missing.

## match 2026-10-23 (3rd)
- d3sky_CreateLayer: root cause of the register map (s edi / tex ebp / no ebx vs ours s ebp / tex ebx) was C2's priority order, not the stores: uv run c2prio showed l 133, const#0 14, tex 12, s 10 (esi, edi, ebx, ebp in that order; the original has s above const#0 and tex below, const#0's ebx never materialises so ebx is not pushed). Fix: `s->layers = l;` written in both arms (`if (!first) { l->next = 0; if (s->layers) { ...; return l; } s->layers = l; } else { l->next = s->layers; s->layers = l; }`) instead of once after the if: s 10 -> 14, ties const#0, wins as the later candidate. Found by a 15-variant branch-structure family (uv run vargen text alternatives).


## match-base 2026-10-23 (match worker)
- Renames (whole word, all units): 0x4385a0 -> d3_BeginScene, 0x438780 -> d3_EndScene (their bodies are the BeginScene / EndScene wrappers with the scene state) and 0x4387b0 -> d3_SetBlendState (the alpha / glow / colour-key render states); the local macro dances (`d3_Unk4385a0_int`, `d3_Unk4387b0_int`, `d3h_*`) follow as d3_SetBlendState_int / d3h_*. d3sky_Draw now calls d3_BeginScene(d3_FogOn, d3_FogColor, 0), 0x6334a4 / 0x6334a0 under the d3_engine.h names (the local extern changed type to char / unsigned long and stayed byte).
- d3sky_DrawLayer, d3sky_Draw, d3sky_DeleteLayer, d3sky_SetLayerSpeed, d3sky_SetLayerTex, d3sky_InitColors: byte + readable. d3sky_InitColors' image-range literals became D3SKY_COLOR0 / _EDGE / _INNER / _CENTER in d3_sky.h (slot 0 = 0x40 is not one of the shades; nothing in the drafted code reads d3sky_Colors).
- d3sky_Create escalated for readability Q4 only: `*(unsigned long *)&s->color = 0xffffffff` needs the dword view of the D3COLOR (b, g, r, a); the original is one `mov DWORD [s+0xaa4],0xffffffff`, four typed byte stores give 85 differing bytes and `memset(&s->color, 0xff, 4)` 66, and an `unsigned long` member in D3COLOR raises its alignment from 1 to 4.
