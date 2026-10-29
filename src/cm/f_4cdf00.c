/* 0x4cdf00, the record of the character c in the 16 records (0x2c bytes, id at +0) of rec + 0x94, else a free one
   (id -1), 0: none */

int *ai_Unk4cdf00(char *rec, char *c)
{
    int id = *(int *)(c + 4);
    int i;

    for (i = 0; i < 16; i++) {
        if (*(int *)(rec + 0x94 + i * 0x2c) == id)
            return (int *)(rec + 0x94 + i * 0x2c);
    }
    for (i = 0; i < 16; i++) {
        if (*(int *)(rec + 0x94 + i * 0x2c) == -1) {
            memset(rec + 0x94 + i * 0x2c, 0, 0x2c);
            *(int *)(rec + i * 0x2c + 0x98) = 0;
            *(int *)(rec + 0x94 + i * 0x2c) = *(int *)(c + 4);
            return (int *)(rec + 0x94 + i * 0x2c);
        }
    }
    return 0;
}
