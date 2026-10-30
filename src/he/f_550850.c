/* 0x550850, a Ghidra function that really starts here and ends at 0x5509e0 (ROUGH, disassembled): like 0x5506c0 the lamp heights of the object +0xc8 and of the record +0xb4 give the vector between the two (+ 80.0 on the target), 0x458810 / 0x453b40 turn it into an orientation, and 0x411860 (obj, pos + 0x84 .. , 0x78, 8) sets the object up; 1 = done, 0 = failed */

void d3_DirToWorld();
int d3t_PlaceObjectV(struct D3OBJECT *camera, struct D3VECTOR *pos, struct D3VECTOR *dir, int unk, char flags);

int he_Unk550850(int ev)
{
    int sc = U_I((unsigned)&d3_CurrentScene);
    int o = 0, a, obj;
    float t[3], c[3], d[3], r[3], pos[3];

    d3if_SelectScene(0, 1);
    if (*(int *)(ev + 0xc8) == 0 || *(int *)(*(int *)(ev + 0xc8) + 0x1cc) == 0 || *(int *)(*(int *)(*(int *)(ev + 0xc8) + 0x1cc) + 0x10) == 0)
        goto fail;
    gm_FindById(&o, 0, 0, *(int *)(ev + 0xb4));
    if (o == 0)
        goto fail;
    a = *(int *)(o + 0x61);
    if (a == 0 || *(int *)(a + 0x1cc) == 0 || *(int *)(*(int *)(a + 0x1cc) + 0x10) == 0)
        goto fail;
    d3_PointToWorld(a, a + 0x4c, t);
    t[1] = t[1] + *(float *)(*(int *)(*(int *)(*(int *)(o + 0x61) + 0x1cc) + 0x10) + 0x1d4) + *(float *)((unsigned)&ai_Unk5eeab8 + 0x140);
    d3_PointToWorld(*(int *)(ev + 0xc8), *(int *)(ev + 0xc8) + 0x4c, c);
    d[0] = t[0] - c[0];
    d[1] = t[1] - c[1];
    d[2] = t[2] - c[2];
    d3_DirToWorld(*(int *)(ev + 0xc8), (void *)(unsigned)&ai_AxisPair, r);
    d3_VecAngle2(r, d);
    obj = *(int *)(ev + 0xc8);
    if (obj == 0)
        goto fail;
    pos[0] = *(float *)(obj + 0x84);
    pos[1] = *(float *)(obj + 0x88);
    pos[2] = *(float *)(obj + 0x8c);
    pos[1] = pos[1] + t[1];
    d3t_PlaceObjectV(obj, r, pos, 0x78, 8);
    d3if_SelectScene(sc, 1);
    return 1;
fail:
    d3if_SelectScene(sc, 1);
    return 0;
}
