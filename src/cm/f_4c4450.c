/* 0x4c4450, resolves the ids -2 / -3 / -4 */

int ai_Unk4c4450(int a, int id)
{
    if (id == -2)
        return ai_Unk60bd88;
    if (id == -3)
        return ai_Unk60bd8c;
    if (id == -4)
        id = ai_Unk60bd90;
    return id;
}
