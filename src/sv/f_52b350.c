/* 0x52b350, adds a record (a, name[0x10], c, d, e) to the 0x20-byte table 0x75ec80 (0x20 entries; a == 0 marks a free entry); returns 1 or 0 when full */

int sv_Unk52b350(int a, char *name, int c, int d, int e)
{
    int i;
    int *p;

    i = 0;
    p = (int *)(unsigned)&full_data_75ec80;
    do {
        if (*p == 0) {
            if (name != 0)
                strcpy((char *)((unsigned)&full_data_75ec84 + i * 0x20), name);
            ((int *)(unsigned)&full_data_75ec80)[i * 8] = a;
            ((int *)(unsigned)&full_data_75ec80)[i * 8 + 5] = c;
            ((int *)(unsigned)&full_data_75ec80)[i * 8 + 6] = d;
            ((int *)(unsigned)&full_data_75ec80)[i * 8 + 7] = e;
            return 1;
        }
        p += 8;
        i++;
    } while ((int)p < (unsigned)((unsigned)full_data_75ec80 + 0x600));
    return 0;
}
