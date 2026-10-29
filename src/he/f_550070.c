/* 0x550070, a Ghidra miss (disassembled): creates the "Sensenmann" (grim reaper) object between the scene records of the objects +0xb0 and +0xb4 (0x4fea50, both need +0x61 with the object +0x1cc / +0x10 of the lamp): the start (+0xe0 / +0xd0 cached when +0xc4 is set) and the vector to the end are built from their positions (0x458a20 + the lamp height +0x1d4 + 0x5eebf8) and the length 0x453b40, the object is created (0x446e10 (0, .., the vectors, "Sensenmann")), cached (+0xc4 = 1, +0xc8 = it, +0xd0 .. +0xe8 the positions, +0xec = 0x6333cc), its flags set (+0x211 &= ~2, +0x212 |= 0xc, +0x213 &= ~4) and 0x411960 called; 1 = created, 0 = failed (ROUGH) */

int d3t_PlayAnim();
float d3_VecAngle2();

int he_Unk550070(int ev)
{
    int sc = U_I((unsigned)&d3_CurrentScene);
    int o1 = 0, o2 = 0, a, b;
    float p1[3], p2[3], v[3], q[3], len;
    int obj;

    d3if_SelectScene(0, 1);
    gm_FindById(&o1, 0, 0, *(int *)(ev + 0xb0));
    if (o1 != 0 && *(int *)(o1 + 0x61) != 0 && *(int *)(*(int *)(o1 + 0x61) + 0x1cc) != 0 && *(int *)(*(int *)(*(int *)(o1 + 0x61) + 0x1cc) + 0x10) != 0) {
        gm_FindById(&o2, 0, 0, *(int *)(ev + 0xb4));
        if (o2 != 0 && *(int *)(o2 + 0x61) != 0 && *(int *)(*(int *)(o2 + 0x61) + 0x1cc) != 0 && *(int *)(*(int *)(*(int *)(o2 + 0x61) + 0x1cc) + 0x10) != 0) {
            if (*(int *)(ev + 0xc4) == 0) {
                d3_PointToWorld(*(int *)(o1 + 0x61), *(int *)(o1 + 0x61) + 0x4c, p1);
                p1[1] = p1[1] + *(float *)(*(int *)(*(int *)(*(int *)(o1 + 0x61) + 0x1cc) + 0x10) + 0x1d4) + *(float *)((unsigned)&ai_Unk5eeab8 + 0x140);
                d3_PointToWorld(*(int *)(o2 + 0x61), *(int *)(o2 + 0x61) + 0x4c, p2);
                p2[1] = p2[1] + *(float *)(*(int *)(*(int *)(*(int *)(o2 + 0x61) + 0x1cc) + 0x10) + 0x1d4) + *(float *)((unsigned)&ai_Unk5eeab8 + 0x140);
                v[0] = p2[0] - p1[0];
                v[1] = p2[1] - p1[1];
                v[2] = p2[2] - p1[2];
                q[0] = 0.0f;
                len = d3_VecAngle2(v, 0);
                q[1] = len;
                q[2] = 0.0f;
            } else {
                p2[0] = *(float *)(ev + 0xe0);
                p2[1] = *(float *)(ev + 0xe4);
                p2[2] = *(float *)(ev + 0xe8);
                p1[0] = *(float *)(ev + 0xd0);
                p1[1] = *(float *)(ev + 0xd4);
                p1[2] = *(float *)(ev + 0xd8);
                d3_PointToWorld(*(int *)(o2 + 0x61), *(int *)(o2 + 0x61) + 0x4c, q);
                q[1] = q[1] + *(float *)(*(int *)(*(int *)(*(int *)(o2 + 0x61) + 0x1cc) + 0x10) + 0x1d4) + *(float *)((unsigned)&ai_Unk5eeab8 + 0x140);
                v[0] = q[0] - p1[0];
                v[1] = q[1] - p1[1];
                v[2] = q[2] - p1[2];
            }
            obj = d3_CreateObject(0, p1, v, "Sensenmann");
            *(int *)(ev + 0xc8) = obj;
            if (obj != 0) {
                *(int *)(ev + 0xc4) = 1;
                *(int *)(ev + 0xe0) = *(int *)(obj + 0x84);
                *(int *)(ev + 0xe4) = *(int *)(obj + 0x88);
                *(int *)(ev + 0xe8) = *(int *)(obj + 0x8c);
                *(int *)(ev + 0xd0) = *(int *)(obj + 0x4c);
                *(int *)(ev + 0xd4) = *(int *)(obj + 0x50);
                *(int *)(ev + 0xd8) = *(int *)(obj + 0x54);
                *(unsigned char *)(obj + 0x211) = *(unsigned char *)(obj + 0x211) & 0xfd;
                *(unsigned char *)(*(int *)(ev + 0xc8) + 0x212) = *(unsigned char *)(*(int *)(ev + 0xc8) + 0x212) | 0xc;
                *(unsigned char *)(*(int *)(ev + 0xc8) + 0x213) = *(unsigned char *)(*(int *)(ev + 0xc8) + 0x213) & 0xfb;
                d3t_PlayAnim(*(int *)(ev + 0xc8), "*sensenmann.baf");
                *(int *)(ev + 0xec) = U_I((unsigned)&ai_Unk6333cc);
                d3if_SelectScene(sc, 1);
                return 1;
            }
        }
    }
    d3if_SelectScene(sc, 1);
    return 0;
}
