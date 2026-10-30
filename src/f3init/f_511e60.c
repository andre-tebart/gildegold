/* 0x511e60, the universe of the scene of the building b's type (0x76fa60): reserved (marked dirty, named after the type) when it is not the current building, else loaded at once by 0x50dc30; -1 when no universe is free @deep behaviour: 35 code hunks / 54 of 176 bytes, meaning read whole against 0x511e60..0x511f11 (the k = ai_BTypes[*b * 0x24d] lookup, the -1 test on init_BuildingUniverse[k], the fallback building / type 0x1e case that loads at once, the free scene, the dirty flag, the name strcpy, the two returns) - no meaning difference left. Meaning/type fixes this session: init_SceneName (was d3_Scenes), the t local removed so *b is read where the original re-reads it (0x511ec9), and k as unsigned with a (unsigned char) cast on the table byte: that is what the original does (xor ebx,ebx / mov bl,BYTE / mov esi,ebx, the byte compare cmp BYTE [esi+0x76fa60],0xff) and it took 144 -> 54 differing bytes / 84 -> 35 code hunks. Left (allocation): the inlined strcpy's registers - the original computes the name destination into ecx (lea ecx,[ecx+0xfd7ba0]) before re-reading *b and walks the source in eax/edx, ours keeps the destination in esi after re-reading *b; and the two returns read the byte from memory (movsx eax,BYTE [esi+0x76fa60]) where ours reuses al. Same instructions, only the register assignment and the block order. */


int init_BuildingSceneUniverse(unsigned char *b)
{
    unsigned k;
    int uni;

    k = (unsigned char)ai_BTypes[*b * 0x24d];
    if (init_BuildingUniverse[k] == -1) {
        if (b != (unsigned char *)ai_FallbackBuildingI && *b != 0x1e) {
            uni = d3t_FindFreeScene();
            if (uni == -1)
                return -1;
            init_BuildingUniverse[k] = uni;
            init_SceneDirty[uni * 0x3e0] = 1;
            strcpy(init_SceneName + uni * 0x3e0, ai_BTypes + *b * 0x24d + 1);
            return init_BuildingUniverse[k];
        }
        init_LoadBuildingScene(*b);
    }
    return init_BuildingUniverse[k];
}
