/* 0x5506c0, starts the "Sensenmann" flight (ROUGH, disassembled): needs the lamp of the object +0xc8 and of the scene record +0xb4 (0x4fea50); the target position (0x458a20 of the record's object, + the lamp height +0x1d4 + 80.0) and the current one give a vector, 0x453b40 normalises it against the object's +0x84 vector, the object's +0x84 .. is cached in +0xe0 .. and the target in +0xd0 .., +0xc4 = 1; returns 1 or 0 */

int he_Unk5506c0(int ev)
{
    int sc = U_I((unsigned)&d3_CurrentScene);
    int o = 0, a;
    float t[3], c[3], d[3];

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
    d3_VecAngle2(d, *(int *)(ev + 0xc8) + 0x84);
    *(int *)(ev + 0xe0) = *(int *)(*(int *)(ev + 0xc8) + 0x84);
    *(int *)(ev + 0xe4) = *(int *)(*(int *)(ev + 0xc8) + 0x88);
    *(int *)(ev + 0xe8) = *(int *)(*(int *)(ev + 0xc8) + 0x8c);
    *(float *)(ev + 0xd0) = t[0];
    *(float *)(ev + 0xd4) = t[1];
    *(float *)(ev + 0xd8) = t[2];
    *(int *)(ev + 0xc4) = 1;
    d3if_SelectScene(sc, 1);
    return 1;
fail:
    d3if_SelectScene(sc, 1);
    return 0;
}
