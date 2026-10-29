/* 0x5020a0, writes the numbers of all building kinds with the type byte t into out and returns their count */

int gm_ListBuildingsOfType(unsigned t, int *out)
{
    int i, k, n;

    n = 0;
    k = 0;
    i = 0;
    do {
        if ((unsigned char)ai_BTypes[i] == t) {
            out[n] = k;
            n++;
        }
        i += 0x24d;
        k++;
    } while (i < 0xca78);
    return n;
}
