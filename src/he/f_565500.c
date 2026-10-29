/* 0x565500, the allocation handler of the "player lost" event (ROUGH): the player record 0x505df0 (+0xac), the object 0x502020 (+0x10), the follow-up 0x4fe980 (+0xc4), 0x524260 (obj, follow, player id, ev); the value +0xb0 = 200, the clocks +0xb4 / +0x44 / +0x52 / +0x60 = now, the next run 400 units ahead */

void he_Unk565500(int ev)
{
    unsigned short *p;
    int o, f;

    f = 0;
    p = (unsigned short *)gm_FindChar(*(int *)(ev + 0xac));
    o = gm_FindBuildingById(*(int *)(ev + 0x10));
    if (*(int *)(ev + 0xc4) != -1)
        f = gm_FindObjectInst(*(int *)(ev + 0xc4));
    sv_MoveToGroup(o, f, *p, ev);
    *(int *)(ev + 0xb0) = 200;
    *(HETIME *)(ev + 0xb4) = ai_Time;
    *(HETIME *)(ev + 0x44) = ai_Time;
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(HETIME *)(ev + 0x60) = ai_Time;
    gm_AddTime(ev + 0xb4, 0, 400, 0);
}
