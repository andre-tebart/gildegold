/* 0x420870, allocates the six octree levels (8^n nodes of 0x18 bytes); 0 on failure */

int qnt_Alloc(void)
{
    int i;

    qnt_Level[0] = calloc(sizeof(QNODE), 1);
    qnt_Level[1] = calloc(sizeof(QNODE), 8);
    qnt_Level[2] = calloc(sizeof(QNODE), 64);
    qnt_Level[3] = calloc(sizeof(QNODE), 512);
    qnt_Level[4] = calloc(sizeof(QNODE), 4096);
    qnt_Level[5] = calloc(sizeof(QNODE), 32768);
    for (i = 0; i <= 5; i++)
        if (!qnt_Level[i])
            return 0;
    qnt_Colors = 0;
    return 1;
}
