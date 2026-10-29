/* 0x4e6ec0, copies the first office of the kind k to *out; 1 when there is one @deep B3: 5 code hunks left of 36 instructions (one differing branch). The whole original 0x4e6ec0..0x4e6f09 was read against the source: the kind scan over the record kind byte from &amt_Table[0].kind (named bound &amt_Table[42].kind, jl), the found check i >= 42 (the original cmp ecx,0x2a), the 0x18-byte copy *out = amt_Table[i] (rep movs 6 dwords) and the return 1 / loop-exit return 0 all match; Q3 0x925518 replaced by the bound. Left: the i >= 42 check's block placement - the original emits cmp ecx,0x2a; jl <copy> with the return 0 in the fallthrough, ours emits cmp; jge <out-of-line return>. Menu leads tried one edit one build each: if (i < 42) { copy; return 1; } with the return after it (9 -> 5 hunks), the same with an explicit else (no change), the break spelling (no change) - the dup-tail prescriptions are used up, the rest is VC6's block ordering. Q1-Q7 pass (amt_GetFirstOffice, the kind byte pointer, the named table bounds). */

int amt_GetFirstOffice(unsigned int k, AMTREC *out)
{
    unsigned char *pk;
    int i;

    i = 0;
    pk = &amt_Table[0].kind;
    do {
        if (*pk == k) {
            if (i >= 42)
                break;
            *out = amt_Table[i];
            return 1;
        }
        pk += sizeof(AMTREC);
        i++;
    } while ((int)pk < (int)&amt_Table[42].kind);
    return 0;
}
