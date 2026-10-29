/* 0x4e5120, the first free party record (id -1), 0 without */

int ai_Unk4e5120(void)
{
    int i;

    for (i = 0; i < 96; i++) {
        if (ai_Unk925520[i].id == -1)
            return (int)&ai_Unk925520[i];
    }
    return 0;
}
