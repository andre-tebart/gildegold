/* 0x4e7040, collects up to max offices of the column col (1 .. 6: the first 30 records, 5: all 42) that have an owner who exists, from the last backwards, into out;
   returns their number
   @deep B3 code 61 hunks / 33 differing of 68 instructions, one merged hunk. Meaning read against the whole original 0x4e7040..0x4e70e6 and complete: col guards (0 none, <=6 -> 30, col 5 -> 42), n = 0, for (i--; i >= 0; i--), if (n >= max) break (the original jge targets the shared epilogue; the break spelling took 103 -> 61 hunks and made the callee-saved pushes shrink-wrap like the original), the col test, gm_FindChar(owner), the 0x18 copy. Menu/leads tried one build each: dup-tail (return 0 in both guard arms: identical code, reverted to the goto none the original block layout shows; break: kept), i-- as for-init vs a separate statement (same code), out++/n++ statement order (-1 byte, kept), comma-init for (n = 0, i--; ...) (same code). Left, all allocation/scheduling: (1) loop-entry test ebp,ebp; jl vs our js; (2) the out parameter home-slot init mov eax,[out]; mov [out],eax and its double reload in the copy block (spell reload: the parameter is used directly, no local to drop); (3) the rep movs block regs/schedule (n in ecx after the copy vs eax before, edi vs eax for out, rep placement). Q1-Q7 pass. */

int amt_GetColumnOfficesOwned(int col, int max, AMTREC *out)
{
    int n, i;

    if (col == 0)
        goto none;
    if (col <= 6)
        i = 30;
    else {
        if (col == 5)
            i = 42;
        else
            goto none;
    }
    n = 0;
    for (i--; i >= 0; i--) {
        if (n >= max)
            break;
        if (amt_Kinds[amt_Table[i].kind].col == col && gm_FindChar(amt_Table[i].owner) != 0) {
            *out = amt_Table[i];
            out++;
            n++;
        }
    }
    return n;
none:
    return 0;
}
