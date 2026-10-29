/* 0x4b62e0, saves the mission kind and the running missions to f */

unsigned vfs_fwrite(void *buf, int size, int n, struct VFSFILE *f);                             /* 0x430e30 */

int ai_Unk4b62e0(void *f)
{
    int i;

    if (f == 0)
        return 0;
    if (vfs_fwrite(&ai_MissionMode, 1, 1, f) == -1)
        return 0;
    for (i = 0; i < 0x80; i++) {
        if (vfs_fwrite(&ai_Missions[i].type, 4, 1, f) == -1)
            return 0;
        if (vfs_fwrite(&ai_Missions[i].owner, 4, 1, f) == -1)
            return 0;
        if (vfs_fwrite(&ai_Missions[i].time, 0x0e, 1, f) == -1)
            return 0;
        if (vfs_fwrite(&ai_Missions[i].unk18, 4, 1, f) == -1)
            return 0;
        if (vfs_fwrite(&ai_Missions[i].unk1c, 4, 1, f) == -1)
            return 0;
        if (vfs_fwrite(&ai_Missions[i].unk20, 1, 1, f) == -1)
            return 0;
    }
    return 1;
}
