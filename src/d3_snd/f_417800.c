/* 0x417800, the first free slot of d3snd_List (obj == NULL), NULL if none */

D3SOUND *d3snd_Alloc(void)
{
    int i = 0;
    D3SOUND *s = NULL;

    if (d3snd_List) {
        for (; i < d3snd_Max; i++)
            if (!d3snd_List[i].obj) {
                s = &d3snd_List[i];
                break;
            }
    }
    return s;
}
