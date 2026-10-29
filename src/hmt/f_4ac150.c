/* 0x4ac150, a random record other than k */

unsigned short hmt_RandomRecord(short k)
{
    unsigned short r;

    do
        r = util_Random(0x33);
    while ((short)r == k);
    return r;
}
