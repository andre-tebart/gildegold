/* 0x519aa0, after a save: the buildings' objects point back to their buildings (+0x200) and the ids of the references table are turned into pointers again (0x505df0 character, 0x4fe980 object, 0x502020 building, 0x4e50a0 kind 9) @deep B: 40 code hunks, read in asmdiff (the equal stretches around them checked too): the kind order 1, 3, 2, 9, the offsets and the calls 0x505df0 / 0x4fe980 / 0x502020 / 0x4e50a0 match the original at 0x519aa0 exactly; the hunks are shapes - ours keeps the loop counter in another register of the first loop, the second loop's entry compares through ecx with jge where the original compares esi with jl, and the original reloads [esi] before the kind==3 / kind==2 pushes where ours forwards a register. Shape is match-deep's. Q2/Q7: the callees and globals are unnamed; Q6 is described. */


void unk_Unk519aa0(void)
{
    int saved = d3_CurrentScene;
    int k;
    int *p;
    char *b;

    d3if_SelectScene(0, 1);
    for (k = 0; k < 0xa900; k += 0xa9) {
        b = ai_Blds + k;
        if (*b != 0 && *(int *)(b + 0x61) != 0)
            *(char **)(*(int *)(b + 0x61) + 0x200) = b;
    }
    d3if_SelectScene(saved, 1);
    for (p = ls_RefTargets; (int)p < (int)ls_RefTargets + 0x50000; p = (int *)((char *)p + 10)) {
        if (*p != 0) {
            if (*((char *)p - 5) == 1)
                *p = (int)gm_FindChar(*p);
            if (*((char *)p - 5) == 3)
                *p = gm_FindObjectInst(*p);
            if (*((char *)p - 5) == 2)
                *p = gm_FindBuildingById(*p);
            if (*((char *)p - 5) == 9)
                *p = cm_PartyById(*p);
        }
    }
}
