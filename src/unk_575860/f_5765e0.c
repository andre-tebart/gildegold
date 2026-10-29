/* 0x5765e0, 1 for the kind 0x1a with a record */

int ai_Unk5765e0(int *p)
{
    if (p[0] == 0x1a && p[1])
        return 1;
    return 0;
}
