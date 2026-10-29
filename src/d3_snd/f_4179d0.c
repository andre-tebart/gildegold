/* 0x4179d0 */

void d3snd_Free(D3SOUND *s)
{
    if (s)
        d3snd_Detach(s);
}
