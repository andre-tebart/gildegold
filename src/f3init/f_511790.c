/* 0x511790, prepares the town scene: the flag 4 on every building scene object of the class 6, the hook 0x511700 on all "gb_" objects, the torches (0x5116b0), and the scene's tree is rebuilt */


void unk_Unk511790(void)
{
    char *b, *o;

    b = (char *)gm_BuildQuery(1, 6);
    while (b != 0) {
        o = *(char **)(b + 0x61);
        if (o != 0) {
            o[0x212] = (o[0x212] & 0xf7) | 4;
            sv_Unk528490(b);
        }
        b = (char *)gm_BuildNext();
    }
    d3_ForEachObject(d3_DefaultScene, 0, 0x40, init_TownBuildingHook, 0);
    unk_Unk5116b0();
    ai_CheckBuildSites();
    d3sm_Build(*(int *)((char *)d3_DefaultScene + 0xb0), ai_Unk633738I);
}
