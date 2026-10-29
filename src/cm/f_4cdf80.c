/* 0x4cdf80, the record of the character c in the first (ai_Unk6f0d04 + 0x30) cutscene slots */

int *ai_Unk4cdf80(char *c)
{
    int n = *(unsigned char *)(ai_Unk6f0d04 + 0x30);
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < 16; j++) {
            if (*(int *)(ai_Unk92cae0[i] + 0x94 + j * 0x2c) == *(int *)(c + 4))
                return (int *)(ai_Unk92cae0[i] + 0x94 + j * 0x2c);
        }
    }
    return 0;
}
