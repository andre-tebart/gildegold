/* 0x4fdcb0, the home of the character c: its building (+0x170) with the object of 0x4fdc10 (out3 = its id, -1 without) and out2 = the building id (+1); without one the next building (starting at 0x6f1bb4, 256 records of 0xa9 bytes) of kind 3 / 0x16 / 0x0f. Returns the building, 0 when none */

char *unk_Unk4fdcb0(char *c, int *out2, int *out3)
{
    unsigned i;
    unsigned char k;
    char *o;
    int n;

    i = ai_Unk6f1bb4;
    if (*(int *)(c + 0x170) != 0) {
        o = unk_Unk4fdc10(*(unsigned char **)(c + 0x170));
        if (o == 0)
            *out3 = -1;
        else
            *out3 = *(int *)(o + 2);
        *out2 = *(int *)(*(int *)(c + 0x170) + 1);
        return *(char **)(c + 0x170);
    }
    for (n = 0xff; n != 0; n--) {
        k = ai_Blds[i * 0xa9];
        if (k != 0 && (ai_BTypes[k * 0x24d] == 3 || ai_BTypes[k * 0x24d] == 0x16 || ai_BTypes[k * 0x24d] == 0x0f)) {
            ai_Unk6f1bb4 = i + 1;
            *out3 = -1;
            *out2 = *(int *)(ai_Blds + i * 0xa9 + 1);
            return ai_Blds + i * 0xa9;
        }
        i = (i + 1) % 256;
    }
    return 0;
}
