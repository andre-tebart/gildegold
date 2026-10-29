/* 0x417650 */
int d3snd_Init(int max)
{
    int ret = 0;

    if (!d3snd_List) {
        if (max)
            d3snd_Max = max;
        d3snd_List = mem_Alloc(d3snd_Max * sizeof(D3SOUND), "d3snd_Init");
        if (d3snd_List) {
            D3_ZERO_AT(d3snd_List, d3snd_Max * sizeof(D3SOUND));
        } else
            ret = -1;
    } else
        ret = -1;
    return ret;
}
