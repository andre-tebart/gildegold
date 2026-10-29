/* 0x420b90, sift-down of heap entry k (heap on the nodes' total, 1-based) */

void qnt_Sift(unsigned int k)
{
    unsigned char level;
    unsigned int node;
    unsigned int half, v, j;

    level = qnt_Heap[k].level;
    node = qnt_Heap[k].node;
    half = qnt_HeapCount >> 1;
    v = qnt_Level[level][node].total;
    while (k <= half) {
        j = k * 2;
        if (j < qnt_HeapCount && QTOTAL(qnt_Heap[j]) > QTOTAL(qnt_Heap[j + 1]))
            j++;
        if (v <= QTOTAL(qnt_Heap[j]))
            break;
        qnt_Heap[k] = qnt_Heap[j];
        k = j;
    }
    qnt_Heap[k].level = level;
    qnt_Heap[k].node = node;
}
