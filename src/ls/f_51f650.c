/* 0x51f650, reads the two relation maps ai_Relations / ai_RelationMemory (n x n bytes, a row of 0x300: n = 0x100 below 0x1003a, 0x100 below 0x10042 else 0x300; the second map from 0x10036). Returns 1 */


int ls_ReadRelations(int f)
{
    int n, i, j;
    char *p;

    if (ls_Version < 0x1003a)
        n = 0x100;
    else
        n = ((ls_Version >= 0x10042) - 1 & 0xffffff00) + 0x300;
    i = 0;
    if (n > 0) {
        p = (char *)ai_Relations;
        do {
            j = 0;
            do {
                LS_R(j + (int)p, 1)
                j++;
            } while (j < n);
            p += 0x300;
            i++;
        } while (i < n);
    }
    if (ls_Version >= 0x10036 && (i = 0, n > 0)) {
        p = (char *)ai_RelationMemory;
        do {
            j = 0;
            do {
                LS_R(j + (int)p, 1)
                j++;
            } while (j < n);
            p += 0x300;
            i++;
        } while (i < n);
    }
    return 1;
}
