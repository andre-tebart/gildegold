/* 0x580a50, resets the three 0x38-byte shot records of 0x6f9f60 (57 B): each record: +0 = -1 and its three
   0x10-byte entries: +8 = -1, +0xc = 100000.0, +0x10 = 0, +0x14 = 0 */

void ai_Unk580a50(void)
{
    int i;
    int j;

    for (i = 0; i < 3; i++) {
        *(int *)(0x6f9f60 + i * 0x38) = -1;
        for (j = 0; j < 3; j++) {
            *(int *)(0x6f9f68 + i * 0x38 + j * 0x10) = -1;
            *(float *)(0x6f9f6c + i * 0x38 + j * 0x10) = 100000.0f;
            *(int *)(0x6f9f70 + i * 0x38 + j * 0x10) = 0;
            *(int *)(0x6f9f74 + i * 0x38 + j * 0x10) = 0;
        }
    }
}
