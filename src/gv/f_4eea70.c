/* 0x4eea70, the message 'nCTa' (0x4c6230) with the id at +1 of the object o */

void unk_Unk4eea70(char *o)
{
    int m[2];

    m[0] = 0x6154436e;
    m[1] = *(int *)(o + 1);
    ai_Unk4c6230(m);
}
