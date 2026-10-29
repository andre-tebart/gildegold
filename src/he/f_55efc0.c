/* 0x55efc0, the run handler of the daily routine of a figure ("Go to work" / "Go to wirtshaus" / "Go home" debug texts, "dummy_EINGANG" entrance dummies; ROUGH skeleton, 2656 bytes): by the season / time of day (gm_ClockSeason) the figure +0xac either walks to its workplace (0x55ea60 contact, 0x4c5b90 / 0x4c52e0 / 0x4c51b0 / 0x4c5150, the entrance 0x458a20 positions), to the tavern or home (gm_IsHomeType), and the routine event is posted again (0x4fe1e0 clocks); 0x55eba0 selects the scene of a watched figure */

void he_Unk55efc0(int ev)
{
    int p, c;

    p = gm_FindChar(*(int *)(ev + 0xac));
    if (p == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    c = he_Unk55ea60(p);
    ai_Unk4c5b90(*(int *)(p + 4));
    ai_Unk4c52e0(*(int *)(p + 4), c, -1, 0, 0, "Go to work");
    cm_RequestChrMoveToUniverse(*(int *)(p + 4), -1, c, "dummy_EINGANG");
    he_Unk55eba0(p);
    he_Unk55ee40(p);
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0x1e, 0);
}
