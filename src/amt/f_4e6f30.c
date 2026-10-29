/* 0x4e6f30, copies the office with the index i to *out; 1 when there is one @deep B3: 4 code hunks left of 51 instructions (one differing branch, the same block-order item as amt_GetFirstOffice 0x4e6ec0). The whole original 0x4e6f30..0x4e6f73 was read against the source: the idx scan over the record byte from amt_Table (named bound &amt_Table[42] = 0x925510), the found check n >= 42 (the original cmp ecx,0x2a), the 0x18-byte copy *out = amt_Table[n] (rep movs 6 dwords) and both return 0 blocks match; the unsigned char idx gives the original's byte compare. Left: the n >= 42 check's block placement - the original emits cmp ecx,0x2a; jl <copy> with the return 0 in the fallthrough, ours emits cmp; jge <out-of-line return 0> (VC6 merges it with the loop-exit return). Menu leads tried one edit one build each: if (n < 42) { copy; return 1; } return 0; (no change), if (n >= 42) return 0; copy; return 1; (no change) - the dup-tail prescriptions are used up. Q1-Q7 pass (amt_GetOffice). */

int amt_GetOffice(unsigned char i, AMTREC *out)
{
    AMTREC *p;
    int n;

    n = 0;
    p = amt_Table;
    do {
        if (p->idx == i) {
            if (n >= 42)
                return 0;
            *out = amt_Table[n];
            return 1;
        }
        p++;
        n++;
    } while ((int)p < (int)&amt_Table[42]);
    return 0;
}
