/* 0x4bdb30, a mark (picture 0x7bb + the index of p in list) on window win at building p->buildingId; -1 if it has none @deep behaviour: every statement read against the disassembly (0x502020, 0x458a20, 0x45f170, 0x5a6b70, 0x4731d0, the mark bookkeeping) is already in the C; the 151 code hunks are stack slots: our frame 0x20 vs the original's 0x28, v at +0x24 vs +0x14, pt at +0x1c vs +0x24, px/pz swapped - VC6 local allocation (declaration order/sizes) and deferred arg cleanup, not a meaning error. Q1/Q2 (gm_FindBuildingById, ai_Unk5a6b70, ai_Unk7e422c, ai_Unkfd7c50, d2_WinOpenPic) stay open */

int rt_Unk4bdb30(char *win, int b, RTPOINT *list, RTPOINT *p)
{
    void *h;
    int x, z;
    char *bld;
    float v[4];
    float pt[2];
    int px, pz;
    float y;
    int o;
    int k;

    h = ai_Unkfd7c50;
    x = *(int *)(ai_Unk7e422c + ai_OwnPlayer * 0x2f4);
    z = *(int *)(ai_Unk7e422c + 4 + ai_OwnPlayer * 0x2f4);
    bld = gm_FindBuildingById(p->buildingId);
    if (bld == 0)
        return -1;
    d3_PointToWorld(*(char **)(bld + 0x61), *(char **)(bld + 0x61) + 0x4c, v);
    if (!d3_GetHeightAt(h, v, &px, &pz, &y))
        return -1;
    ai_Unk5a6b70(pt, x, z);
    o = d2_WinOpenPic((int)pt[0] - 0x20, (int)pt[1] - 0x20, rt_PointIndex(list, p) + 0x7bb, *(int *)(win + 8));
    rt_Marks[rt_MarkCount].obj = o;
    *(int *)(d2_Objs + o * 0x2ec + 0x48) = 1;
    if (rt_MarkGroup == -1)
        rt_MarkGroup = d2_CreateRadioGroup(1, rt_Marks[rt_MarkCount].obj);
    else
        d2_RadioAdd(rt_MarkGroup, rt_Marks[rt_MarkCount].obj);
    rt_Marks[rt_MarkCount].p = p;
    k = rt_MarkCount++;
    return rt_Marks[k].obj;
}
