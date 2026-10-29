/* 0x420c90, merges the leaf with the fewest pixels into its parent until only ncolors leaves are left */

void qnt_Reduce(unsigned int ncolors)
{
    unsigned char lvl, plvl;
    unsigned int idx;
    unsigned int pidx;

    while (qnt_HeapCount > ncolors) {
        lvl = qnt_Heap[1].level;
        idx = qnt_Heap[1].node;
        plvl = lvl - 1;
        pidx = idx >> 3;
        if (qnt_Level[plvl][pidx].count) {
            qnt_Heap[1] = qnt_Heap[qnt_HeapCount];
            qnt_HeapCount--;
        } else {
            qnt_Heap[1].level = plvl;
            qnt_Heap[1].node = pidx;
        }
        qnt_Level[plvl][pidx].count += qnt_Level[lvl][idx].count;
        qnt_Level[plvl][pidx].r += qnt_Level[lvl][idx].r;
        qnt_Level[plvl][pidx].g += qnt_Level[lvl][idx].g;
        qnt_Level[plvl][pidx].b += qnt_Level[lvl][idx].b;
        qnt_Level[plvl][pidx].mask &= ~(1 << (idx & 7));
        qnt_Sift(1);
    }
}
