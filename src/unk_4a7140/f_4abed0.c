/* 0x4abed0, registers the actions 0x44 "Blutrache", 0x45 "Verfluchen", 0x46 "Theater" and 0x47 "Ehrerbietung" */

int ai_Unk4abed0(int a)
{
    AIACT act;

    AI_ZERO(&act, sizeof(act));
    act.id = 0x44;
    strcpy(act.name, "Blutrache");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk4ab3e0;
    act.exec = ai_Unk4ab6b0;
    act.flags = 0x1c3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x45;
    strcpy(act.name, "Verfluchen");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk4ab7f0;
    act.exec = ai_Unk4ab9f0;
    act.flags = 0x1c3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x46;
    strcpy(act.name, "Theater");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk4abac0;
    act.exec = ai_Unk4abc60;
    act.flags = 1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x47;
    strcpy(act.name, "Ehrerbietung");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk4abd00;
    act.exec = ai_Unk4abe60;
    act.flags = 3;
    return ai_Unk495520(&act, a) != 0;
}
