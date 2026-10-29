/* 0x4e50e0, the member slot (0x92cae0, 0x35c bytes; the id at +0x10) with the id */

int ai_Unk4e50e0(int id)
{
    int i;

    for (i = 0; i < 16; i++) {
        if (*(int *)(ai_Unk92cae0[i] + 0x10) == id)
            return (int)ai_Unk92cae0[i];
    }
    return 0;
}
