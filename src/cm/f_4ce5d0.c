/* 0x4ce5d0, a bomb: the first free slot of the 32 (0x10 bytes) of ai_Unk935a00 gets the object "ob_BOMBE" at the
   position p (y + 60.0) and a particle block thrown towards the cell (x, y); (object, effect, x, y) */

char *ai_Unk4ce5d0(float *p, int x, int y)
{
    float o[3];
    float d[3];
    char s[0x160];
    int i;
    char *obj, *slot;
    char *g = *(char **)(ai_Unk5fb5b0 + 0xb0);
    float pos[3];

    o[0] = 0;
    o[1] = 0;
    o[2] = 0;
    for (i = 0; i < 32; i++) {
        if (*(int *)(ai_Unk935a00 + i * 0x10) == 0)
            break;
    }
    if (i >= 32)
        return 0;
    pos[0] = p[0];
    pos[1] = (float)(p[1] + 60.0);
    pos[2] = p[2];
    obj = d3_CreateObject(0, pos, o, "ob_BOMBE");
    *(char **)(ai_Unk935a00 + i * 0x10) = obj;
    obj[0x211] |= 4;
    d3sm_GetVertexI(g, x, y, d);
    d[0] = d[0] - p[0];
    d[1] = d[1] - p[1];
    d[2] = d[2] - p[2];
    memset(s, 0, sizeof(s));
    *(int *)(s + 0x00) = 0x12;
    *(int *)(s + 0x58) = 0x12;
    *(float *)(s + 0x5c) = (float)(d[0] * 0.5);
    *(float *)(s + 0x60) = (float)(d[1] * 0.5 + 50.0);
    *(float *)(s + 0x64) = (float)(d[2] * 0.5);
    *(float *)(s + 0x6c) = 3.1415927f;
    *(float *)(s + 0x70) = 3.1415927f;
    *(float *)(s + 0x74) = 3.1415927f;
    *(int *)(s + 0xb0) = 1;
    d[1] = (float)(d[1] - 60.0);
    *(float *)(s + 0xb4) = d[0];
    *(float *)(s + 0xb8) = d[1];
    *(float *)(s + 0xbc) = d[2];
    *(float *)(s + 0xc4) = 3.1415927f;
    *(float *)(s + 0xc8) = 3.1415927f;
    *(float *)(s + 0xcc) = 3.1415927f;
    *(float *)(s + 0x10c) = d[0];
    *(float *)(s + 0x110) = d[1];
    *(float *)(s + 0x114) = d[2];
    *(float *)(s + 0x11c) = 3.1415927f;
    *(float *)(s + 0x120) = 3.1415927f;
    *(float *)(s + 0x124) = 3.1415927f;
    slot = ai_Unk935a00 + i * 0x10;
    *(int *)(slot + 4) = ((D3ANIM *(*)(void *, void *, int, void *))d3anim_CreatePath)(*(int *)slot, o, 4, s);
    *(int *)(slot + 0xc) = y;
    *(int *)(slot + 8) = x;
    return slot;
}
