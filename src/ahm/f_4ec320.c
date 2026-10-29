/* 0x4ec320, the value of the walk step p (as ahm_LoadIni stored them), used by the tutorial results window (0x57bf00 passes it to the text builder
   0x5176d0): the four attribute bytes (each / 252 = 0x5ecee0) squared and added, plus the two values of the recipe entry of the step's id (ahm_FindRecipe)
   when there is one, both times 500 (0x5ed098), as an int shifted by 5. The two sums are added, not chosen (the 0.0 at 0x5eb788 is the FPU value the
   branch does or does not replace).
   Find (B3 draft): same statements as the original, but VC6 schedules the four squares differently - the original squares each attribute right after loading it
   and folds it into the sum (`fld st(0); fmul st,st(1); faddp st(2),st; fstp st(0)`, sum in an FPU register at st(2)), ours keeps the four scaled values on
   the FPU and squares them at the end (`fld st(3); fmul st,st(4)` ...), 88 of 176 bytes. Spelling leads tried, all giving the deferred form: the sum as one
   expression, as four `sum = sum + x * x` statements interleaved with the loads, and parenthesised left-associative. The rest of the body (the call, the
   test, the v branch, the __ftol) matches. Q1..Q7 pass.
   @deep behaviour draft (B3): 88 of 176 bytes, all from the FPU scheduling of the four squares - the original squares each attribute right after loading it and folds it into the sum in an FPU register (fld st(0); fmul st,st(1); faddp st(2),st; fstp st(0)), ours keeps the four scaled values on the FPU and squares them at the end (fld st(3); fmul st,st(4)), plus the temp slot ([esp+8] vs the original's [esp+4]) and the pop esi position that follow from it. Read: disasm 0x4ec320..0x4ec3cf, the caller 0x57bf00 (its result goes to the text builder 0x5176d0) and the constants 0x5eb788 0.0 / 0x5ecee0 1/252 / 0x5ed098 500.0. Meaning fixed in this session: the recipe values are a second sum (v) added to the squared attributes, not a replacement (the 0.0 branch). Spelling leads tried, one build each, all giving the deferred form: one sum expression, four sum = sum + x*x statements interleaved with the loads, parenthesised left-associative. Q1..Q7 pass (typed AHMSTEP.grid.plr view, named after its role). */

int ahm_StepValue(char *p)
{
    AHMSTEP *s = (AHMSTEP *)p;
    float v, sum, a, b, c, d;
    AHMGRID *g;

    v = 0.0f;
    g = ahm_FindRecipe(s->grid.plr.id);
    a = (float)s->grid.plr.attr[0] * 0.003968254f;
    b = (float)s->grid.plr.attr[1] * 0.003968254f;
    c = (float)s->grid.plr.attr[2] * 0.003968254f;
    d = (float)s->grid.plr.attr[3] * 0.003968254f;
    sum = ((a * a + b * b) + c * c) + d * d;
    if (g != 0)
        v = g->rcp.b + g->rcp.a;
    return (int)(v * 500.0f + sum * 500.0f) << 5;
}
