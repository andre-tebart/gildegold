/* 0x5a84d0, the character block of a good (102 B): the byte is searched in the 3 records of 0x15 bytes at
   0x5eed94 (for the bytes ' ', 0x1e, 0x1f) and then in the 37 records at 0x61a348; returns the record or 0 */

unsigned char *ai_Unk5a84d0(char *param_1)
{
    char c;
    unsigned char *q;
    int i;

    c = *param_1;
    if (c == ' ' || c == 0x1e || c == 0x1f) {
        i = 0;
        for (q = (unsigned char *)(unsigned)&full_rdata_5eed94; (int)q < (int)((unsigned)&full_rdata_5eed94 + 0x3f); q += 0x15, i++)
            if ((char)*q == c)
                return (unsigned char *)(unsigned)&full_rdata_5eed94 + i * 0x15;
    }
    i = 0;
    for (q = (unsigned char *)(unsigned)&ai_Unk61a348; (int)q < (int)((unsigned)&ai_Unk61a348 + 0x309); q += 0x15, i++)
        if ((char)*q == c)
            return (unsigned char *)(unsigned)&ai_Unk61a348 + i * 0x15;
    return 0;
}
