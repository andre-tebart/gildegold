/* 0x420990, counts the n pixels into the finest level, sums their colors up to the root and puts every
   leaf into the min-heap on its pixel total
   @deep behaviour: spellings only - grade.py sib 8, code 6; unitdiff 7 B / 10 asm lines (2026-10-26) - base/index swapped in the qnt_Level[lvl][idx] addressing ([eax+ecx+0x10] vs [ecx+eax+0x10]), mov esi,eax before vs after pop ebp, test esi,esi vs test eax,eax; no meaning error; 800 access spellings tried, and a for / while+dec / do shape, a fresh block variable k and idx / lvl declared inside the if block all compile identically (2026-10-26), so the entry test register and the pop ebp position are allocator-level (notes/units/qnt.md) */

int qnt_Build(unsigned char *rgb, unsigned int n)
{
    unsigned int i, j, c, idx;
    int lvl;

    qnt_Heap = malloc(0x20004);
    if (!qnt_Heap)
        return 0;
    for (; n > 0; n--) {
        qnt_Level[5][qnt_R[rgb[0]] + qnt_G[rgb[1]] + qnt_B[rgb[2]]].count++;
        rgb += 3;
    }
    qnt_HeapCount = 0;
    for (i = 0; i < 0x8000; i++) {
        c = qnt_Level[5][i].count;
        if (c) {
            qnt_HeapCount++;
            qnt_Heap[qnt_HeapCount].level = 5;
            qnt_Heap[qnt_HeapCount].node = i;
            qnt_Level[5][i].total = c;
            j = i >> 2;
            qnt_Level[5][i].r = (((((j & 0x1000 | i & 0x800) >> 2 | i & 0x100) >> 2 | i & 0x20) >> 1 | (i & 4) << 1) + 4) * c;
            qnt_Level[5][i].g = ((((j & 0x800 | i & 0x400) >> 2 | i & 0x80) >> 2 | (i & 2) << 2 | i & 0x10) + 4) * c;
            qnt_Level[5][i].b = ((((j & 0x400 | i & 0x200) >> 2 | i & 0x40) >> 1 | ((i & 1) << 2 | i & 8) << 1) + 4) * c;
            idx = i;
            for (lvl = 4; lvl >= 0; lvl--) {
                j = idx & 7;
                idx >>= 3;
                qnt_Level[lvl][idx].total += c;
                qnt_Level[lvl][idx].mask |= 1 << j;
            }
        }
    }
    for (i = qnt_HeapCount; i > 0; i--)
        qnt_Sift(i);
    return 1;
}
