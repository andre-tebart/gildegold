/* 0x519b50, after a load: the buildings' objects point back to their buildings, the scene is restored (0x44aa10) and the pointers are set again @deep B: 16 code hunks, all one register-order choice: the same addressing modes with the index and base registers swapped (ours [edx+eax*1] / [ecx+edx*1], the original [eax+edx*1] / [edx+ecx*1] - k lives in eax in the original and in edx/ecx in ours); every other instruction is equal (8 + 12 + 29 equal). No meaning difference: the two loops, 0x44aa10(0, f), the restores and the return 1 match the original at 0x519b50. Register assignment is match-deep's (vargen ren). Q2/Q7: the unnamed callees/globals; Q6 is described. */


int unk_Unk519b50(int f)
{
    int saved = d3_CurrentScene;
    int k;
    char *b;

    d3if_SelectScene(0, 1);
    for (k = 0; k < 0xa900; k += 0xa9) {
        b = ai_Blds + k;
        if (*b != 0 && *(int *)(b + 0x61) != 0)
            *(int *)(*(int *)(b + 0x61) + 0x200) = *(int *)(b + 1);
    }
    d3_SaveScene(0, f);
    for (k = 0; k < 0xa900; k += 0xa9) {
        b = ai_Blds + k;
        if (*b != 0 && *(int *)(b + 0x61) != 0)
            *(char **)(*(int *)(b + 0x61) + 0x200) = b;
    }
    d3if_SelectScene(saved, 1);
    return 1;
}
