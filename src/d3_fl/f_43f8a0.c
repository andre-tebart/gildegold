/* 0x43f8a0 @deep B: code hunk 0 (51 instructions) - the complete 40-instruction original at 0x43f8a0..0x43f907 was read against ours (42 instructions): every operation, constant, call and store is present; the original keeps n in eax (compared, shifted at the end) and the clamped min(n,0x40) in edx, ours allocates the clamp to eax and n to edx, so ours has one extra mov eax,edx before the n>>7 shift and the same block is scheduled around it. Tried the ternary, if/else on m, m=n+if, both store orders of DetailShift/DetailSizeShift and an early-return guard: the clamp value always gets eax (c2prio: m priority 38-40, n 11-15; C2 hands the scratch registers out by priority). Same operation, other register spelling - needs c2prio/vargen */

void d3fl_SetDetail(unsigned int n)
{
    unsigned int m;
    int s;

    if (n <= 0x100 && n >= 4) {
        m = n > 0x40 ? 0x40 : n;
        s = 0;
        while (!(m & 1)) {
            m >>= 1;
            s++;
        }
        d3fl_DetailSizeShift = 2;
        d3fl_DetailScale = 1 << s;
        d3fl_DetailShift = 6 - s;
        if ((unsigned char)(n >> 7) <= 2)
            d3fl_DetailSizeShift = (unsigned char)(n >> 7);
        d3fl_MakeJitter(d3fl_Jitter);
        d3fl_Flush();
    }
}
