/* 0x4e5770, the init of the scene code: the party records are cleared (id -1) and the scene kinds are registered: 0 the picture show (0x4e0010), 1 the council
   votes (0x4d8e20), 2 the trial (0x4db700), 3 the fight (0x4d63e0), 4 the duel, 5 the execution, 6 the wedding, 7 the birth, 8 the death, 9 the bankruptcy, 10
   the auction (0x4e1a80), 11 the sale (0x4e2860) */

void ai_Unk4e5770(void)
{
    int i;

    memset(ai_Unk925520, 0, 0x6780);
    for (i = 0; i < 96; i++)
        ai_Unk925520[i].id = -1;
    ai_Unk4e5730(0, (int)ai_Unk4e0010, (int)d3io_NoopCallback, 0, 0, 0);
    ai_Unk4e5730(1, (int)ai_Unk4d8e20, (int)ai_Unk4dada0, (int)ai_Unk4dae90, 0, 0);
    ai_Unk4e5730(2, (int)ai_Unk4db700, (int)ai_Unk4dd960, (int)ai_Unk4de340, (int)ai_Unk4daf80, 0);
    ai_Unk4e5730(3, (int)ai_Unk4d63e0, (int)ai_Unk4d8b10, 0, (int)ai_Unk4d60b0, 1);
    ai_Unk4e5730(4, (int)ai_Unk4ded10, (int)ai_Unk4dfff0, (int)ai_Unk4dff80, (int)ai_Unk4dfa20, 0);
    ai_Unk4e5730(5, (int)ai_Unk4e01e0, 0, 0, (int)ai_Unk4e0040, 0);
    ai_Unk4e5730(6, (int)ai_Unk4e0770, 0, 0, (int)ai_Unk4e0580, 0);
    ai_Unk4e5730(7, (int)ai_Unk4e0f70, 0, 0, (int)ai_Unk4e0e90, 0);
    ai_Unk4e5730(8, (int)ai_Unk4e1250, 0, 0, (int)ai_Unk4e1220, 0);
    ai_Unk4e5730(9, (int)ai_Unk4e1700, 0, 0, 0, 0);
    ai_Unk4e5730(10, (int)ai_Unk4e1a80, (int)ai_Unk4e24c0, (int)ai_Unk4e26d0, (int)ai_Unk4e19a0, 1);
    ai_Unk4e5730(11, (int)ai_Unk4e2860, 0, 0, 0, 0);
    ai_Unk4e36a0(0);
}
