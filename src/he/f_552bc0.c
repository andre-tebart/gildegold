/* 0x552bc0, the run handler of the end-of-game style event (only with 0x6f25ac set; ROUGH): state 0 -> 1 or 2 by 0x577740, state 1 -> 2 when 0x575860 says so, state 2 shuts down (0x577820, 0x578150), frees the record, sets the flag 0x6f259c and continues in 0x52fb50 (0x53bb20) */

void he_Unk552bc0(int ev)
{
    if (*(unsigned char *)(unsigned)&ai_GameState == 0)
        return;
    switch (*(int *)(ev + 0x70)) {
    case 0:
        *(int *)(ev + 0x70) = (ai_Unk577740() != 0) + 1;
        break;
    case 1:
        if (ai_Unk575860() != 0)
            *(int *)(ev + 0x70) = 2;
        break;
    case 2:
        ai_Unk577820();
        ai_Unk578150();
        he_Unk53e1f0((int *)ev);
        U_I((unsigned)&ai_GameOver) = 1;
        sv_PopTo((void *)(unsigned)&main_GameLoop);
        break;
    }
}
