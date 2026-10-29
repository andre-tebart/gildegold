/* 0x58fe10, the nearest object of a 0x100-entry list within 100.0 (157 B): the horizontal distance of the
   position ref (x, z) to each object's position (+0x4c) picks the nearest below 100.0; 0 when none */

int ai_FindNearestObject(float *ref, int *list)
{
    int i;
    int obj;
    float best;
    float p[4];
    float pos[4];
    float d;

    obj = 0;
    best = 1e+08f;
    pos[0] = ref[0];
    pos[2] = ref[2];
    for (i = 0; i < 0x100; i++) {
        if (list[i] != 0) {
            d3_PointToWorld((struct D3OBJECT *)list[i], (struct D3VECTOR *)(list[i] + 0x4c),
                            (struct D3VECTOR *)p);
            p[1] = 0.0f;
            d = (float)sqrt((p[0] - pos[0]) * (p[0] - pos[0]) + (p[2] - pos[2]) * (p[2] - pos[2]));
            if (d < best && d < 100.0) {
                obj = list[i];
                best = d;
            }
        }
    }
    return obj;
}
