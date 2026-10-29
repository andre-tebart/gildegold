/* 0x417990, detaches every used sound slot */

void d3snd_DetachAll(void)
{
    int i;

    for (i = 0; i < d3snd_Max; i++)
        if (d3snd_List[i].obj)
            d3snd_Detach(&d3snd_List[i]);
}
