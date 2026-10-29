/* 0x4ecdb0, the 'SmSa' handler: adds m->value.f to the value at +4 of the record rec, clamped to 0 .. 1 @deep B3 draft, 13 differing instructions (44 -> 41 differing bytes this session): the clamp semantics are proven against the original (0 <= f < 1 store f, f >= 1 store 1.0f, f < 0 / NaN store 0.0f; the original compares f with 0.0, 1.0, then 0.0 again). The remaining hunks are the test schedule: the original emits the 0.0 fcom/and-eax,0x4100/jne test and the 1.0 fcom/test-ah,0x5/jp test up front, our form evaluates the 1.0 test with jnp at the L1 join (same operations, other block placement). spell.py menu dup-tail at 4ecdbe: all three prescriptions tried, one build each - statements in both arms (nested if/else with the store in each arm: 15 differing instructions), pull them after the branch (12-13, kept: the current form), swap the branch polarity (the f >= 0.0f outer / f < 1.0f inner nested form: 17). Also tried the f <= 0.0f / f >= 1.0f clamp (15) and the two early returns (18). Readable; Q1..Q7 hold. */

int ahm_AddValue(int *p, char *rec)
{
    AHMORDER *m = (AHMORDER *)p;
    AHMREC *r = (AHMREC *)rec;
    float f;

    f = m->value.f + r->value;
    if (!(f > 0.0f))
        f = 0.0f;
    if (!(f < 1.0f))
        f = 1.0f;
    r->value = f;
    return 1;
}
