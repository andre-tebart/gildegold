/* 0x4eb180, the research state of the object o: the first research event (kinds 4, 0x95 .. 0x97) of the event list 0x53e400 (1, 3, *(o + 1)) decides: 4 (only with
   flag: *a = 0, *f = 0.0), 1 (the share of the research done in *f, 0 .. 1), 2 / 3 (the same, with the recipe / share id (+0xb0) in *a); 0 when there is none.
   Only the clamp value v differs: the original keeps it in a stack temp ([esp+8] in case 0x95, [esp+4] in 0x96 / 0x97: `fdiv st,st(1); fstp temp; fstp st(0);
   fld temp`) and compares/reloads it from there, ours keeps it on the FPU and compares st(0)/st(1) directly (143 code hunks). Every statement, constant,
   offset, the case order and the three compare forms (1.0f < / v <= 0.0f / 1.0f >) match.
   @deep B3 draft: the value v in memory instead of the FPU is the allocation choice: leads tried are the case order (default body first, case 4 last - gained,
   kept), the constant-left comparisons (1.0f >= v: 191 -> 144, kept), the negated `!(1.0f < v)` (test ah,0x5 form: 144 -> 143, kept), the early-return clamp
   form (143 -> 181, WORSE, reverted), `volatile float v` (185, reverted), block-scoped v per case (same 143). Q1..Q7 pass. Left: FPU vs stack-temp allocation. */

int ahm_ResearchState(char *o, int *a, float *f, int flag)
{
    AHMRESEARCH *ev;
    float v;

    ev = (AHMRESEARCH *)he_FindEvent(1, 3, ((AHMOBJ *)o)->id);
    while (ev != 0) {
        switch (ev->kind) {
        default:
            ev = (AHMRESEARCH *)he_NextEvent();
            break;
        case 0x95:
            v = ((float)ev->total - (float)ev->amount) / (float)ev->total;
            *f = v;
            if (!(1.0f < v)) {
                if (v <= 0.0f) {
                    *f = 0.0f;
                    return 1;
                }
                if (!(1.0f < v)) {
                    *f = v;
                    return 1;
                }
            }
            *f = 1.0f;
            return 1;
        case 0x96:
            v = ((float)ev->start - (float)ev->total) / (float)ev->start;
            *f = v;
            if (1.0f >= v) {
                if (v <= 0.0f)
                    v = 0.0f;
                else if (1.0f >= v)
                    goto set96;
            } else
                v = 1.0f;
set96:
            *f = v;
            *a = ev->id;
            return 2;
        case 0x97:
            v = ((float)ev->start - (float)ev->total) / (float)ev->start;
            *f = v;
            if (1.0f >= v) {
                if (v <= 0.0f)
                    v = 0.0f;
                else if (1.0f >= v)
                    goto set97;
            } else
                v = 1.0f;
set97:
            *f = v;
            *a = ev->id;
            return 3;
        case 4:
            if (flag == 0)
                return 0;
            *f = 0.0f;
            *a = 0;
            return 4;
        }
    }
    return 0;
}
