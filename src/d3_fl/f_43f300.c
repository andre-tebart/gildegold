/* 0x43f300, 256 texture triangles: 4 fixed ones covering the inner square (f..1-f), then 63 x 4 randomly
   placed, scaled (0.9..1.23) and rotated copies of the same four triangles
   @deep B: read the whole fixed part and loop against the original disassembly 0x43f300..0x43f88c: the values match (f = 1/d3fl_DetailScale, g = 1-f-f, the 12 fixed u/v entries, then 63 iterations of 4 groups x (2 rand offsets, scale rand/3+0.9, z angle 2pi, d3_MatFromAngles and the six m._11/_12/_21/_22 terms) - the loop stride is 0x60 in both (the original's esi points to data+0x20 and the stores are esi-0x20.., not a wider block); the difference is that the original keeps f, g and the intermediate u/v in stack slots (frame 0x84 vs 0x60) and spills/reloads them around each call while ours keeps them in FPU registers (fsts/fld st(n)), plus the FPU scheduling; no missing write, wrong constant or wrong term */

void d3fl_MakeJitter(float *out)
{
    D3FLUV *t = (D3FLUV *)out;
    float f, g, ox, oy, s;
    D3VECTOR4 rot;
    D3MATRIX m;
    int i;

    f = 1.0f / d3fl_DetailScale;
    g = 1.0f - f - f;
    t[0].u = f;
    t[0].v = g + f;
    t[1].u = f;
    t[1].v = f;
    t[2].u = g + f;
    t[2].v = f;
    t[3].u = g + f;
    t[3].v = f;
    t[4].u = g + f;
    t[4].v = g + f;
    t[5].u = f;
    t[5].v = g + f;
    t[6].u = f;
    t[6].v = f;
    t[7].u = g + f;
    t[7].v = g + f;
    t[8].u = f;
    t[8].v = g + f;
    t[9].u = f;
    t[9].v = f;
    t[10].u = g + f;
    t[10].v = f;
    t[11].u = g + f;
    t[11].v = g + f;
    for (i = 1, t += 12; i < 64; i++, t += 12) {
        ox = (float)rand() / RAND_MAX;
        oy = (float)rand() / RAND_MAX;
        s = (float)rand() / RAND_MAX / 3.0f + 0.9f;
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = rand() * D3FL_2PI / RAND_MAX;
        d3_MatFromAngles(&rot, &m);
        t[0].u = (0.0f * m._11 + 1.0f * m._21 + 0.0f * m._31) * s + ox;
        t[0].v = (0.0f * m._12 + 1.0f * m._22 + 0.0f * m._32) * s + oy;
        t[1].u = ox;
        t[1].v = oy;
        t[2].u = (1.0f * m._11 + 0.0f * m._21 + 0.0f * m._31) * s + ox;
        t[2].v = (1.0f * m._12 + 0.0f * m._22 + 0.0f * m._32) * s + oy;

        ox = (float)rand() / RAND_MAX;
        oy = (float)rand() / RAND_MAX;
        s = (float)rand() / RAND_MAX / 3.0f + 0.9f;
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = rand() * D3FL_2PI / RAND_MAX;
        d3_MatFromAngles(&rot, &m);
        t[3].u = (1.0f * m._11 + 0.0f * m._21 + 0.0f * m._31) * s + ox;
        t[3].v = (1.0f * m._12 + 0.0f * m._22 + 0.0f * m._32) * s + oy;
        t[4].u = (1.0f * m._11 + 1.0f * m._21 + 0.0f * m._31) * s + ox;
        t[4].v = (1.0f * m._12 + 1.0f * m._22 + 0.0f * m._32) * s + oy;
        t[5].u = (0.0f * m._11 + 1.0f * m._21 + 0.0f * m._31) * s + ox;
        t[5].v = (0.0f * m._12 + 1.0f * m._22 + 0.0f * m._32) * s + oy;

        ox = (float)rand() / RAND_MAX;
        oy = (float)rand() / RAND_MAX;
        s = (float)rand() / RAND_MAX / 3.0f + 0.9f;
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = rand() * D3FL_2PI / RAND_MAX;
        d3_MatFromAngles(&rot, &m);
        t[6].u = ox;
        t[6].v = oy;
        t[7].u = (1.0f * m._11 + 1.0f * m._21 + 0.0f * m._31) * s + ox;
        t[7].v = (1.0f * m._12 + 1.0f * m._22 + 0.0f * m._32) * s + oy;
        t[8].u = (0.0f * m._11 + 1.0f * m._21 + 0.0f * m._31) * s + ox;
        t[8].v = (0.0f * m._12 + 1.0f * m._22 + 0.0f * m._32) * s + oy;

        ox = (float)rand() / RAND_MAX;
        oy = (float)rand() / RAND_MAX;
        s = (float)rand() / RAND_MAX / 3.0f + 0.9f;
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = rand() * D3FL_2PI / RAND_MAX;
        d3_MatFromAngles(&rot, &m);
        t[9].u = ox;
        t[9].v = oy;
        t[10].u = (1.0f * m._11 + 0.0f * m._21 + 0.0f * m._31) * s + ox;
        t[10].v = (1.0f * m._12 + 0.0f * m._22 + 0.0f * m._32) * s + oy;
        t[11].u = (1.0f * m._11 + 1.0f * m._21 + 0.0f * m._31) * s + ox;
        t[11].v = (1.0f * m._12 + 1.0f * m._22 + 0.0f * m._32) * s + oy;
    }
}
