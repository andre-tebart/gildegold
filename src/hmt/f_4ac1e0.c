/* 0x4ac1e0, record k, 0 if out of range */

G11REC *hmt_GetRecord(unsigned short k)
{
    if (k >= 0x33)
        return 0;
    return &hmt_Records[k];
}
