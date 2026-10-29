/* 0x4fbd10, puts the character id (0x505df0, it must have no model yet) on the stage at the object of the group (1, 1, a) of its building (the group of the mission object when b is -1): its scene (0x502520 / 0x511dc0 / 0x511e60), the dummy "dummy_TUER" (or the object own dummy 0x406790, else "dummy_EINGANG"), the model by 0x4fbb80, the animation "bewegung/gehen"; stores the building (+0x2c) and b (+0x30) in the model and hides it (0x402480) when the shown building (0x6f2308 / 0x6f230c) is another. Returns the model or 0. ROUGH */

int gm_PlaceOnStage(int id, int a, int b, int *p4, int *p5)
{
    int saved;
    char *c;
    int o, o2, sc;
    char *dummy;
    int uVar7;

    saved = d3_CurrentScene;
    o2 = 0;
    c = gm_FindChar(id);
    if (c == 0)
        return 0;
    if (*(int *)(c + 0x184) != 0)
        return 0;
    o = gm_BuildQuery(1, 1, a);
    if (o == 0)
        return 0;
    if (b != -1) {
        o2 = (int)gm_Query(*(int *)(o + 0x5d), 1, 1, b);
        if (o2 == 0)
            return 0;
    }
    sc = gm_IsHomeType(o);
    if (sc == 0) {
        if (o2 == 0) {
            sc = 0;
            goto go;
        }
        sc = init_ObjectSceneUniverse(o2);
    } else
        sc = init_BuildingSceneUniverse(o);
    if (sc < 0)
        return 0;
go:
    d3if_SelectScene(sc, 1);
    if (o2 == 0) {
        uVar7 = *(int *)(o + 0x61);
        dummy = "dummy_TUER";
        p5 = 0;
        p4 = 0;
    } else {
        dummy = (char *)ch_FindDummy();
        if (dummy == 0) {
            dummy = "dummy_EINGANG";
            uVar7 = 0;
        } else {
            uVar7 = 0;
        }
    }
    unk_Unk4fbb80((unsigned short *)c, sc, p4, p5, uVar7, dummy);
    if (*(int *)(c + 0x184) != 0)
        ch_PreloadAnims(*(int *)(c + 0x184), 1, "bewegung/gehen");
    d3if_SelectScene(saved, 1);
    if (*(int *)(c + 0x184) == 0)
        return 0;
    *(int *)(*(int *)(c + 0x184) + 0x2c) = a;
    *(int *)(*(int *)(c + 0x184) + 0x30) = b;
    if ((ai_PlayerBuilding != 0 && ai_PlayerBuilding != o) || (ai_FallbackBuilding != 0 && ai_FallbackBuilding != o))
        ch_SetVisible(*(int *)(c + 0x184), 0);
    return *(int *)(c + 0x184);
}
