/* 0x565b00, the allocation handler "he_AllocDiePlayer": the player 0x505df0 (+0xac) must exist (else the error message and free); his flag byte +8 is cleared, the next run is now */

void he_Unk565b00(int ev)
{
    int p;

    p = gm_FindChar(*(int *)(ev + 0xac));
    if (p == 0) {
        err_Message("he_AllocDiePlayer: player not found!");
        he_Unk53e1f0((int *)ev);
        return;
    }
    *(char *)(p + 8) = 0;
    *(HETIME *)(ev + 0x52) = ai_Time;
}
