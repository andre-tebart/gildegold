/* 0x4e4870, 1 when the character id is one of the party m (+0x30 count, +0x34 ids) and still exists */

int ai_Unk4e4870(int id, char *m)
{
    int i, n;

    n = *(unsigned char *)(m + 0x30);
    for (i = 0; i < n; i++) {
        if (*(int *)(m + 0x34 + i * 4) == id) {
            if (gm_FindChar(id) != 0)
                return 1;
            break;
        }
    }
    return 0;
}
