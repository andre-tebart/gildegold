/* 0x4e5050, the used party of the kind a with the thing b: its record, 0 without */

int ai_Unk4e5050(int a, int b)
{
    int i;

    for (i = 0; i < 96; i++) {
        if (ai_Unk925520[i].used != 0 && ai_Unk925520[i].kind == a && ai_Unk925520[i].thing == b)
            return (int)&ai_Unk925520[i];
    }
    return 0;
}
