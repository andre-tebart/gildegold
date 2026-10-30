/* 0x4d1c20, places the characters ids[16] (side of the fight: dummies "dummy_<side>_NAHK_%02i" / "_FERN_%02i" for the
   melee / ranged kinds 0x174 0x176 0x15e 0x160) on the ground at their dummy (0x45f170 / 0x45f100 / 0x4072a0 /
   0x4025c0 / 0x43b860). Rough draft. */

void ai_Unk4d1c20(char *side, int id)
{
    char buf[0x60];
    float v[4], w[4], u[4];
    int a, b, h, n1 = 1, n2 = 1, i;
    char *sc = *(char **)(ai_Unk5fb5b0 + 0xb0);
    char *c, *k, *o;
    int *ids = (int *)side;

    for (i = 0; i < 16; i++) {
        if (ids[i] == -1)
            continue;
        c = ai_Unk4ce130(ids[i]);
        if (c == 0)
            continue;
        k = ai_Unk4cda50(c);
        if (k != 0 && (*(int *)k == 0x174 || *(int *)k == 0x176 || *(int *)k == 0x15e || *(int *)k == 0x160))
            sprintf(buf, "dummy_%s_FERN_%02i", (char *)id, n2++);
        else
            sprintf(buf, "dummy_%s_NAHK_%02i", (char *)id, n1++);
        o = (char *)d3_FindObject(0, 0x100, 0, buf);
        if (o == 0)
            continue;
        d3_PointToWorld(o, o + 0x4c, v);
        if ((char)d3_GetHeightAt(sc, v, &a, &b, &h) == 0)
            return;
        ch_FindFreeCell(sc, *(char **)(*(char **)(c + 0x184) + 0x34) + 0x4c, *(char **)(*(char **)(c + 0x184) + 0x34) + 0x4c, a, b, w, 2, 0);
        d3sm_GetVertex(sc, a, b, w);
        ch_SetPos(*(char **)(c + 0x184), u, 1);
        u[0] = 0;
        u[1] = 0;
        u[2] = 0;
        d3_DirToWorld(o, (unsigned)&ai_AxisPair, v);
        d3_VecAngle((unsigned)&ai_AxisPair, v);
        d3_SetObjectRot(*(char **)(*(char **)(c + 0x184) + 0x34), u);
    }
}
