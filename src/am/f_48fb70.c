/* 0x48fb70, registers five actions (0x40..0x43, 0x48) */

int ai_Unk48fb70(int a)
{
    AIACT act;

    AI_ZERO(&act, sizeof(act));
    act.id = 0x40;
    strcpy(act.name, "Build Domizil");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk48e120;
    act.exec = ai_Unk48e370;
    act.flags = 0x1c3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x41;
    strcpy(act.name, "UseSecretRoom");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk48e470;
    act.exec = ai_Unk48eab0;
    act.flags = 0x1c3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x42;
    strcpy(act.name, "UseCannonTower");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk48f010;
    act.exec = ai_Unk48f1e0;
    act.flags = 0x1c1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x43;
    strcpy(act.name, "BuildSculpture");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk48f2b0;
    act.exec = ai_Unk48f500;
    act.flags = 3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x48;
    strcpy(act.name, "UseAlchemistryItems");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk48f600;
    act.exec = ai_Unk48f990;
    act.flags = 0x1c3;
    return ai_Unk495520(&act, a) != 0;
}
