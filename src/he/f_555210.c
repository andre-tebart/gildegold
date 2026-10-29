/* 0x555210 (ROUGH; the second and third parameters are unused), collects the transporter dummies of the object `obj` (0x438d40 (scene 0x5fb5b0, obj, 0x100, 0x5551c0, list), the object's +0x1f0 is cleared during the walk unless flag 1 of +0x210 is set) and returns a random one of them (0x42ec00 (n) with the count n passed as the fourth parameter), 0 when n = 0 */

int he_Unk555210(char *obj, int unused1, int unused2, int n)
{
    int list[0x21];
    int save;

    memset(list, 0, sizeof(list));
    if (obj != 0 && (*(unsigned char *)(obj + 0x210) & 1) == 0) {
        save = *(int *)(obj + 0x1f0);
        *(int *)(obj + 0x1f0) = 0;
        d3_ForEachObject(U_I((unsigned)&ai_Unk5fb5b0), obj, 0x100, (void *)he_Unk5551c0, list);
        *(int *)(obj + 0x1f0) = save;
    } else {
        d3_ForEachObject(U_I((unsigned)&ai_Unk5fb5b0), obj, 0x100, (void *)he_Unk5551c0, list);
    }
    if (n == 0)
        return 0;
    return list[(unsigned short)util_Random(n)];
}
