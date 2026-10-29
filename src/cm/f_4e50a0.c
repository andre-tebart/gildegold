/* 0x4e50a0, the used party with the id: its record, 0 without */

int cm_PartyById(int id)
{
    int i;

    for (i = 0; i < 96; i++) {
        if (ai_Unk925520[i].used != 0 && ai_Unk925520[i].id == id)
            return (int)&ai_Unk925520[i];
    }
    return 0;
}
