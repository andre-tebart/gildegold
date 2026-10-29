/* 0x5dd090, the 0 .. 60 clamp divided by 60 (69 B): 0 when <= 0, 1 when >= 60, else value / 60.
   Every comparison is written negated (`!(value < 60.0f)`, `!(clamped > 0.0f)`): the positive forms
   make VC6 emit `and eax,0x100` where the original tests the C0/C2 bits (`test ah,0x5; jp`), and the
   divisions stay float (a double / 60.0 becomes an fmul of the reciprocal).
   @deep behaviour: 34 code hunks remain (56 at session start), first at +0x10; the meaning matches (0 .. 60 clamp, the select, the three returns) and the remaining hunks are VC6 float-compare idioms / block layout: the original keeps the selected value on the FPU stack across fcomp 0.0, tests C0|C3 (and eax,0x4100; jne) and puts the return-0 block out of line, ours folds the select and uses test ah,0x41; jp. Leads tried, one build each: positive vs negated comparisons (x >= 60 / !(x < 60) / ternary select), v <= 0 vs !(v > 0), float vs double division (double becomes fmul of the reciprocal; the float fdiv is the original). Kept: negated first test and float divisions (56 -> 34 hunks). Left: FPU/branch scheduling - deep */

float ai_NormalizeEffectScale(float value)
{
    float clamped;

    clamped = value;
    if (!(value < 60.0f))
        clamped = 60.0f;
    if (clamped <= 0.0f)
        return 0.0f / 60.0f;
    if (!(value < 60.0f))
        return 60.0f / 60.0f;
    return value / 60.0f;
}
