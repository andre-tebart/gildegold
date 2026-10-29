/* 0x53e780, adds a gfx info (the first free of 0x40 entries at 0x7588a0) to the event `rec` for the object `obj`: 3D model `name` (only with the 3D option byte 0x6f361b) */

void he_Unk53e780(int rec, char *name, int obj)
{
    int sv = U_I((unsigned)&d3_CurrentScene);
    int i;
    int *p;
    int *ent;
    int o;
    char buf[0x200];

    if (*(char *)((unsigned)&ai_GameSwitches + 0x3) != 0) {
        i = 0;
        p = (int *)((unsigned)&he_RunHandlers + 0x2a4);
        do {
            if (*p == 0)
                break;
            p += 4;
            i++;
        } while ((int)p < ((unsigned)&he_RunHandlers + 0x6a4));
        if (i != 0x40) {
            ent = (int *)(((unsigned)&he_RunHandlers + 0x2a0) + i * 16);
            d3if_SelectScene(0, 1);
            o = d3_FindObject(0, 0x40, obj, 0);
            if (o == 0) {
                sprintf(buf, "he_CreateGfxInfo(): invalid parent :%s");
            } else {
                ent[1] = rec;
                ent[2] = 0;
                ent[0] = *(int *)(rec + 4);
                *(int **)(rec + 0x88) = ent;
                he_Unk53e5f0((int)ent, (int)name, obj);
            }
            d3if_SelectScene(sv, 1);
        }
    }
}
