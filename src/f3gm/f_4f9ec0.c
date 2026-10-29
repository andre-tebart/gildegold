/* 0x4f9ec0, the income rating 0 .. 1: a weighted sum over the income kinds 9 .. 0xf of the table 0x4f2720
   (weights 0.3 0.25 0.12 0.1 0.05 0.08 0.1), each value normalized by its record's range:
   sum of (1 - (v - lo) / (hi - lo)) * weight
   @deep behaviour: 47/49 instructions, 13 differing - 9 fpu + 2 code + 6 fill. The whole delta is the FPU-stack form of the accumulation: the original computes the term (st0), rotates it to st1 (fstp st(1)), loads the sum (fld [sum]), adds (fadd st,st(1)) and stores (fstp [sum]), leaving the term on the stack so the loop tail pops it (fstp st(0)) and the final exit returns sum + the last term (fadd [sum]); ours does fadd [sum] / fstp [sum] / fstp st(0) and returns the sum alone (fld [sum]) - the 2 code instructions are the loop entry jmp / fstp st(0) that follow from it. The value expression (fild/fsub/fdivp/fsubr 1.0/fmul w[i-9]) is identical. Meaning-class leads that MATCHED and are kept: the loop bound is i <= 0xf (cmp 0xf / jle, was i < 0x10) and the term is a named local x with sum = sum + x (17 -> 13 differing, 48171 -> 48168 bytes in the unit). Measured and NOT it (byte-identical): sum = (float)(sum + ...), sum += ..., the swapped operand order, dropping the (float) cast. Q1/Q5/Q6 done this session (gm_IncomeRating, ROUGH gone, description rewritten); Q2..Q4, Q7 pass. */

float gm_IncomeRating(void)
{
    float w[7];
    float sum;
    float x;
    int rec[9];
    int i;

    w[0] = 0.3f;
    w[1] = 0.25f;
    w[2] = 0.12f;
    w[3] = 0.1f;
    w[4] = 0.05f;
    w[5] = 0.08f;
    w[6] = 0.1f;
    sum = 0.0f;
    for (i = 9; i <= 0xf; i++) {
        gv_GetTableRecord(i, rec);
        x = (1.0 - ((double)rec[6] - rec[1]) / ((double)rec[2] - rec[1])) * w[i - 9];
        sum = sum + x;
    }
    return sum;
}
