/* 0x4b63b0, loads the mission kind and the running missions from f (cf. 0x4b62e0) */

int ls_Read(void *buf, int size, int n, int f);                             /* 0x51c920, ls.c: true when the fread returned a positive count */

int ai_Unk4b63b0(void *f)
{
    int i;

    if (f == 0)
        return 0;
    if (!ls_Read(&ai_MissionMode, 1, 1, f))
        return 0;
    for (i = 0; i < 0x80; i++) {
        if (!ls_Read(&ai_Missions[i].type, 4, 1, f))
            return 0;
        if (!ls_Read(&ai_Missions[i].owner, 4, 1, f))
            return 0;
        if (!ls_Read(&ai_Missions[i].time, 0x0e, 1, f))
            return 0;
        if (!ls_Read(&ai_Missions[i].unk18, 4, 1, f))
            return 0;
        if (!ls_Read(&ai_Missions[i].unk1c, 4, 1, f))
            return 0;
        if (!ls_Read(&ai_Missions[i].unk20, 1, 1, f))
            return 0;
    }
    return 1;
}
