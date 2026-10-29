/* 0x52c4a0, releases the speech bubble of the object o (the object field sv_BubbleObjects of its bubble record) */

void sv_ReleaseBubble(int o)
{
    int i;
    int *p;

    i = 0;
    p = sv_BubbleObjects;
    do {
        if (*p == o) {
            sv_BubbleObjects[i * 0x43] = 0;
            return;
        }
        p += 0x43;
        i++;
    } while ((int)p < (int)(sv_BubbleObjects + 0x40 * 0x43));
}
