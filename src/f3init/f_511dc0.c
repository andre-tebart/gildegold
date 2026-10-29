/* 0x511dc0, the universe of the scene of the object prototype p (a new one marked dirty (0xfd7f75) is only reserved, named after the prototype, and loaded later; the current prototype 0x6f2310 is loaded at once by 0x50d7f0) */


int init_ObjectSceneUniverse(unsigned short *p)
{
    int uni;
    char *d, *s;
    char c;

    if (init_ObjectUniverse[*p] == -1) {
        if (p == (unsigned short *)sv_PanelShown) {
            init_LoadObjectScene(*p);
            return init_ObjectUniverse[*p];
        }
        uni = d3t_FindFreeScene();
        if (uni == -1)
            return -1;
        init_ObjectUniverse[*p] = uni;
        init_SceneDirty[uni * 0x3e0] = 1;
        strcpy(init_SceneName + uni * 0x3e0, ai_Goods + *p * 0x41 + 1);
    }
    return init_ObjectUniverse[*p];
}
