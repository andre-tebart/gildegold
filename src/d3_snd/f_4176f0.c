/* 0x4176f0, frees the sound slot array d3snd_Init allocated and clears it */

void d3snd_Exit(void)
{
    if (d3snd_List)
        mem_Free(d3snd_List);
    d3snd_List = NULL;
}
