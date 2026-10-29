/* 0x550370, per-tick move of the "Sensenmann" object (+0xc8) of the event record towards the scene record of +0xb4 (ROUGH, from the disassembly; the vector math is approximate): cached object missing or without lamp -> 0; when the object flag +0x1d0 of the target lamp is set the clock +0xec is refreshed and 1 returned; the horizontal distance (0x458a20 positions) below 45.0 ends the movement (+0xc4 = 0, returns 2), else the object is moved by the time since +0xec times a random wobble (0x4c3f80, 0x454560 normalises, 0x45f170 reads the ground height, + 80.0 above it) and 1 is returned */

int ai_Unk4c3f80();

int he_Unk550370(int ev)
{
    int sc = U_I((unsigned)&d3_CurrentScene);
    int out[2];
    int o, a, rv;
    float A[3], B[3], d[3], n[3], np[3], dist, dt, w, h, ground;

    out[0] = 0;
    out[1] = 0;
    d3if_SelectScene(0, 1);
    if (*(int *)(ev + 0xc8) == 0 || *(int *)(*(int *)(ev + 0xc8) + 0x1cc) == 0 || *(int *)(*(int *)(*(int *)(ev + 0xc8) + 0x1cc) + 0x10) == 0)
        goto fail;
    gm_FindById(out, 0, 0, *(int *)(ev + 0xb4));
    o = out[0];
    if (o == 0)
        goto fail;
    a = *(int *)(o + 0x61);
    if (a == 0 || *(int *)(a + 0x1cc) == 0 || *(int *)(*(int *)(a + 0x1cc) + 0x10) == 0)
        goto fail;
    if (*(int *)(*(int *)(ev + 0xc8) + 0x1d0) != 0) {
        *(int *)(ev + 0xec) = U_I((unsigned)&ai_Unk6333cc);
        d3if_SelectScene(sc, 1);
        return 1;
    }
    d3_PointToWorld(a, a + 0x4c, A);
    d3_PointToWorld(*(int *)(ev + 0xc8), *(int *)(ev + 0xc8) + 0x4c, B);
    d[0] = A[0] - B[0];
    d[2] = A[2] - B[2];
    d[1] = 0.0f;
    dist = (float)sqrt(d[0] * d[0] + d[2] * d[2]);
    d3_GetHeightAt(U_I((unsigned)&ai_Unkfd7c50), B, &ground, 0, 0);
    h = ground + 80.0f;
    n[0] = dist - B[0];
    n[1] = h - B[1];
    n[2] = d[1] - B[2];
    if (*(float *)&out[1] == 45.0f) {
        *(int *)(ev + 0xc4) = 0;
        rv = 2;
        d3if_SelectScene(sc, 1);
        return rv;
    }
    dt = (float)((double)U_I((unsigned)&ai_Unk6333cc) - (double)*(unsigned int *)(ev + 0xec));
    w = (float)ai_Unk4c3f80() * 0.25f + 0.5f;
    *(int *)(ev + 0xec) = U_I((unsigned)&ai_Unk6333cc);
    d3_Normalize(np);
    np[0] = np[0] * w * dt + n[0];
    np[1] = np[1] * w * dt + n[1];
    np[2] = np[2] * w * dt + n[2];
    d3_GetHeightAt(U_I((unsigned)&ai_Unkfd7c50), np, &ground, 0, 0);
    if (!(ground + 80.0f > np[1])) {
        np[1] = dt * 0.978 + np[1];
        np[0] = dt * n[0] * 0.1f + np[0];
        np[2] = dt * n[2] * 0.1f + np[2];
    }
    *(int *)(ev + 0xe0) = *(int *)(*(int *)(ev + 0xc8) + 0x84);
    *(int *)(ev + 0xe4) = *(int *)(*(int *)(ev + 0xc8) + 0x88);
    *(int *)(ev + 0xe8) = *(int *)(*(int *)(ev + 0xc8) + 0x8c);
    *(float *)(ev + 0xd0) = np[0];
    *(float *)(ev + 0xd4) = np[1];
    *(float *)(ev + 0xd8) = np[2];
    d3_SetObjectPos(*(int *)(ev + 0xc8), np);
    d3if_SelectScene(sc, 1);
    return 1;
fail:
    d3if_SelectScene(sc, 1);
    return 0;
}
