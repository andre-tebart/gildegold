/* 0x420900, frees the octree levels and the heap @deep Q2: qnt_Unk632364 (0x632364, void * next to qnt_Heap) is never written in the binary - an exe scan finds the single reference to its address at 0x42097f, the free() in this function - so its role has no evidence; every other rule holds */

void qnt_Free(void)
{
    if (qnt_Level[0])
        free(qnt_Level[0]);
    if (qnt_Level[1])
        free(qnt_Level[1]);
    if (qnt_Level[2])
        free(qnt_Level[2]);
    if (qnt_Level[3])
        free(qnt_Level[3]);
    if (qnt_Level[4])
        free(qnt_Level[4]);
    if (qnt_Level[5])
        free(qnt_Level[5]);
    if (qnt_Heap)
        free(qnt_Heap);
    if (qnt_Unk632364)
        free(qnt_Unk632364);
}
