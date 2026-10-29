/* 0x478930, 0 if the two colours are equal, else 1 */

int d2_CompareColour(char *a, char *b)
{
    if (a[0] == b[0] && a[1] == b[1] && a[2] == b[2])
        return 0;
    return 1;
}
