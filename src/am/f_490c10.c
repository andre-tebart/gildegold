/* 0x490c10, registers three actions */

int ai_Unk490c10(int a)
{
    AIACT act;

    AI_ZERO(&act, sizeof(act));
    act.id = 0x3d;
    strcpy(act.name, "Graveyard Owner");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk490690;
    act.exec = ai_Unk490830;
    act.flags = 0x1c3;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x3e;
    strcpy(act.name, "Handle Grave");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_Unk4908d0;
    act.exec = ai_Unk490b90;
    act.flags = 1;
    if (!ai_Unk495520(&act, a))
        return 0;
    AI_ZERO(&act, sizeof(act));
    act.id = 0x3f;
    strcpy(act.name, "Use Seance");
    act.eval = ai_Unk4a49a0;
    act.decide = ai_SeanceDecide;
    act.exec = ai_SeanceExec;
    act.flags = 1;
    return ai_Unk495520(&act, a) != 0;
}
