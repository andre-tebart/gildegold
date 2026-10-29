/* 0x59c6e0, number of used records of a 7-int list (32 B): the records whose control (+0) is not -1 */

int ai_UsedRecordCount(int *recs)
{
    int n;
    int i;

    n = 0;
    for (i = 0; i < 16; i++)
        if (recs[i * 7] != -1)
            n++;
    return n;
}
