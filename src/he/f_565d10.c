/* 0x565d10, the allocation handler "he_AllocKillPlayer" (ROUGH): the player 0x505df0 (+0xac) must exist (else the error message and free); when another event of the type 0x72 already kills him this one is freed, else
   +0xb4 / +0xb0 / +0x84 = -1 and the run clock is the start clock (+0x44) */

void he_Unk565d10(int ev)
{
    int p, o;

    p = gm_FindChar(*(int *)(ev + 0xac));
    if (p == 0) {
        err_Message("he_AllocKillPlayer: player not found!");
        he_Unk53e1f0((int *)ev);
        return;
    }
    for (o = (int)he_FindEvent(1, 0, 0x72); o != 0; o = (int)he_NextEvent()) {
        if (o != ev && *(int *)(o + 0xac) == *(int *)(ev + 0xac)) {
            he_Unk53e1f0((int *)ev);
            return;
        }
    }
    *(int *)(ev + 0xb4) = -1;
    *(HETIME *)(ev + 0x52) = *(HETIME *)(ev + 0x44);
    *(int *)(ev + 0xb0) = -1;
    *(int *)(ev + 0x84) = -1;
}
