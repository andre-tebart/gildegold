/* 0x5a8940, the settings saved (213 B): the count, then every record +0 / +4 / +0x44 and the seven slots
   {good, weight} by 0x430e30 (the file param_1); 1 done, 0 on a write error */

int ai_Unk5a8940(void *param_1)
{
    int n;
    int *p;
    int i;

    n = 0;
    for (p = (int *)*(int *)((unsigned)&full_data_6f26c0); p != 0; p = (int *)p[0x12])
        n++;
    if (vfs_fwrite(&n, 4, 1, param_1) == 0)
        return 0;
    for (p = (int *)*(int *)((unsigned)&full_data_6f26c0); p != 0; p = (int *)p[0x12]) {
        if (vfs_fwrite(p, 4, 1, param_1) == 0)
            break;
        if (vfs_fwrite((char *)p + 4, 4, 1, param_1) == 0)
            break;
        if (vfs_fwrite((char *)p + 0x44, 4, 1, param_1) == 0)
            break;
        for (i = 0; i < 7; i++) {
            if (vfs_fwrite((char *)p + 0xc + i * 8, 4, 1, param_1) == 0)
                return 0;
            if (vfs_fwrite((char *)p + 0x10 + i * 8, 1, 1, param_1) == 0)
                return 0;
        }
        if (*(int *)((char *)p + 0x48) == 0)
            return 1;
    }
    return 0;
}
