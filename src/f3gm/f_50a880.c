/* 0x50a880, sorts the n ids by their names (0x5143d0 gets the name of an id) */

void gm_Unk50a880(int n, int *ids, int flag)
{
    int i, j, t;
    char a[0x80], b[0x80];

    if (flag == 0 && n != 1 && n - 1 >= 0) {
        for (i = 1;; i++) {
            for (j = i; j < n; j++) {
                io_CharacterName(ids[i - 1], 1, 1, a);
                io_CharacterName(ids[j], 1, 1, b);
                if (strcmp(a, b) == 1) {
                    t = ids[i - 1];
                    ids[i - 1] = ids[j];
                    ids[j] = t;
                }
            }
            if (!(i < n - 1))
                break;
        }
    }
}
