/* 0x4f4d10, runs 0x4f4970 for each of the 768 characters */

void unk_Unk4f4d10(void)
{
    int i;

    i = 0;
    do {
        unk_Unk4f4970(i);
        i++;
    } while (i < 0x300);
}
