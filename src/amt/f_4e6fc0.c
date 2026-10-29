/* 0x4e6fc0, collects up to max offices of the column col (1 .. 6: the first 30 records, 8: all 42), from the last backwards, into out; returns their number @deep B3: 13 code hunks (13 differing of ~61 instructions), all in the prologue/epilogue. The whole original 0x4e6fc0..0x4e702d was read against the source and matches: the col guards (col == 0 none, col <= 6 -> 30, col == 8 -> 42, else none), n = 0, the for (i--; i >= 0; i--) walk backwards over amt_Table, the n >= max return n, the kind column test amt_Kinds[kind].col == col, the 0x18-byte copy *out = amt_Table[i] with out++ and the shared return 0 (the goto none label reproduces the original's out-of-line return 0 used by both col guards). Left: the callee-saved push placement (the original shrink-wraps ebp/esi/edi to just before the loop and separates dec ebx from a test ebx,ebx/jl; ours pushes them in the function prologue and fuses dec/js) - VC6 shrink-wrapping, no menu rule. Menu leads tried one edit one build each: the col bound spellings (col < 7 / col != 8 -> col <= 6 / col == 8 else) fixed those hunks; the while + break form (47 code hunks) -> for (i--; i >= 0; i--) with return n (13); the early returns inline vs the shared goto none label (45/47 -> 13). Q1-Q7 pass (amt_GetColumnOffices). */

int amt_GetColumnOffices(int col, int max, AMTREC *out)
{
    int n, i;

    if (col == 0)
        goto none;
    if (col <= 6)
        i = 30;
    else {
        if (col == 8)
            i = 42;
        else
            goto none;
    }
    n = 0;
    for (i--; i >= 0; i--) {
        if (n >= max)
            return n;
        if (amt_Kinds[amt_Table[i].kind].col == col) {
            n++;
            *out = amt_Table[i];
            out++;
        }
    }
    return n;
none:
    return 0;
}
