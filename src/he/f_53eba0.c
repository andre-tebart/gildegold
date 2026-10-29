/* 0x53eba0, removes the gfx infos whose parent changed or when the 3D option is off */

void he_Unk53eba0(void)
{
    int *p = (int *)((unsigned)&he_RunHandlers + 0x2a0);

    do {
        if (p[1] != 0 && (*(int *)(p[1] + 4) != *p || *(char *)((unsigned)&ai_GameSwitches + 0x3) == 0))
            he_Unk53e840(p);
        p += 4;
    } while ((int)p < (int)((unsigned)&he_RunHandlers + 0x6a0));
}
