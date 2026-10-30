/* 0x4ee360, the explosion of the part p (+0x30 .. +0x38 the position, +0x58 the strength, +0x40 / +0x48 / +0x4c the fire direction): finds the (up to 3) buildings (the 256 records of 0xa9 bytes at ai_Blds)
   whose box the point lies in (state 1, weight 1 / (1 + 0.005 d)) or near (state 2, weight 1 / (1 + (0.0002 d + 0.005) d)), keeps those with the largest strength * weight > 100, starts the particle effects
   (0x416340 / 0x4168e0 / 0x416040) and the sound "Explosion" (0x426030), and when w is set damages the buildings' owners (state 1: message 0x4f via 0x4c4b70; state 2: skill 0x5c -= value); the hits go to *out (0xe ints).
   ROUGH, the floats of the box tests and the argument values of the effect calls are as read from the disassembly. */

/* unlinked */
static __inline int unk_fb(float f)
{
    return *(int *)&f;
}

typedef struct UNKHIT {                 /* 0x10 bytes: a building hit by the explosion */
    int id;
    float dist;
    int state;                          /* 1 inside the box, 2 in the margin */
    float val;
} UNKHIT;
typedef struct UNKHITS {                /* 0x38 bytes, the slot's record at +0x16c (0x6f0e48 + 0x38 * n) */
    int h0, h1;
    UNKHIT e[3];
} UNKHITS;

void unk_Unk4ee360(int w, int sub, char *p, UNKHITS *out)
{
    UNKHITS h;
    float pos[3], half[3], mn[3], hi[3], lo[3];
    float cx, cy, cz;
    float fv, fd, w1, w2, m, best, cnt;
    int i, k, sel, nstate;
    char *o, *bo;
    char *obj;
    float dir[3];
    float a, b, c;
    char msg[0xb4];
    char dmg;
    int idx;

    h.h1 = sub;
    cnt = 0;
    bo = 0;
    best = 100000.0f;
    for (i = 0; i < 3; i++) {
        h.e[i].id = -1;
        h.e[i].dist = 1000000.0f;
        h.e[i].state = 0;
        h.e[i].val = 0.0f;
    }
    cx = *(float *)(p + 0x30);
    cy = *(float *)(p + 0x34);
    cz = *(float *)(p + 0x38);
    for (idx = 0; idx < 0x100; idx++) {
        sel = -1;
        m = *(float *)(p + 0x58) * 0.1f;
        o = ai_Blds + idx * 0xa9;
        if (*(int *)(o + 1) != -1 && *o != 0 && (obj = *(char **)(o + 0x61)) != 0) {
            d3_PointToWorld(obj, obj + 0x4c, pos);
            fd = (float)sqrt((cx - pos[0]) * (cx - pos[0]) + (cy - pos[1]) * (cy - pos[1]) + (cz - pos[2]) * (cz - pos[2]));
            if (bo == 0 || fd < best) {
                best = fd;
                bo = o;
            }
            w1 = 1.0f / (fd * 0.005f + 1.0f);
            w2 = 1.0f / ((fd * 0.0002f + 0.005f) * fd + 1.0f);
            d3t_GetMeshBounds(mn, half, *(int *)(o + 0x61));
            half[0] = (half[0] - mn[0]) * 0.5f;
            half[1] = (half[1] - mn[1]) * 0.5f;
            half[2] = (half[2] - mn[2]) * 0.5f;
            lo[1] = pos[1] - half[1];
            lo[2] = pos[2] - half[2];
            hi[0] = half[0] + pos[0];
            hi[1] = half[1] + pos[1];
            hi[2] = half[2] + pos[2];
            if (cx < pos[0] - half[0] || cx > hi[0] || cy < lo[1] || cy > hi[1] || cz < lo[2] || cz > hi[2]) {
                m = m + m;
                if (m + cx < pos[0] - half[0] || cx - m > hi[0] || m + cy < lo[1] || cy - m > hi[1] || m + cz < lo[2] || cz - m > hi[2])
                    continue;
                nstate = 2;
                fv = w2;
            } else {
                nstate = 1;
                fv = w1;
            }
            fv = fv * *(float *)(p + 0x58);
            if (fv > 100.0f) {
                fv = fv * 0.01f;
                cnt = (float)((int)cnt + 1);
                if (100.0f <= fv)
                    fv = 100.0f;
                for (k = 0; k < 3; k++) {
                    if (h.e[k].id == -1) {
                        h.e[k].val = fv;
                        h.e[k].state = nstate;
                        h.e[k].id = *(int *)(o + 1);
                        h.e[k].dist = fd;
                        bo = o;
                        goto next;
                    }
                }
                for (k = 0; k < 3; k++) {
                    if (h.e[k].val < fv && (sel == -1 || h.e[k].val < h.e[sel].val))
                        sel = k;
                }
                bo = o;
                if (sel != -1) {
                    h.e[sel].val = fv;
                    h.e[sel].state = nstate;
                    h.e[sel].id = *(int *)(o + 1);
                    h.e[sel].dist = fd;
                }
            }
        }
    next:;
    }
    m = *(float *)(p + 0x58) * 0.1f;
    a = *(float *)(p + 0x4c);
    dir[0] = a * *(float *)(p + 0x40);
    dir[2] = a * *(float *)(p + 0x48);
    dir[1] = 0.0f;
    cy = cy + 10.0f;
    d3t_CreateSmoke(0, &cx, 200, 0x41c80000, 0x428c0000, unk_fb(m * 0.25f), 0x44610000, 0x42c80000, 0x428c0000, dir, 0x5dc, 7, 0x7fc0c0c0);
    d3t_CreateEarth(0, &cx, 100, 0x40a00000, 0x40200000, 0x3f800000, 0x40400000, 0xc0400000, 0x40800000, 0x40e00000, 0x41000000, 0x7fc0c0c0);
    if (cnt == 0.0f) {
        a = m * 0.01f;
        b = m * 0.3f;
        c = m * 0.1f;
    } else {
        a = m * 0.03f;
        b = m * 0.6f;
        c = m * 0.2f;
    }
    d3t_CreateExplosion(0, &cx, 0x96, unk_fb(c), unk_fb(b), unk_fb(a), 0x42a00000, 200);
    k = sb_GetSample(0, "Explosion");
    if (k != 0)
        d3snd_PlayAt(k, *(int *)(bo + 0x61), 0x6b, 0x463ea000);
    if (w != 0) {
        for (i = 0; i < 3; i++) {
            if (h.e[i].id != -1 && (gm_FindById(&obj, 0, 0, h.e[i].id), obj != 0)) {
                if (h.e[i].state == 1) {
                    msg[4] = 0x4f;
                    if (*(unsigned short *)(obj + 0x27) == 0xffff)
                        *(int *)(msg + 8) = *(int *)(ai_PlayerCharC + 4);
                    else
                        *(int *)(msg + 8) = *(int *)(ai_Chars[*(unsigned short *)(obj + 0x27)] + 4);
                    *(int *)(msg + 0xc) = *(int *)(obj + 1);
                    *(int *)(msg + 0x28) = *(int *)ai_Time;
                    *(int *)(msg + 0x2c) = *(int *)(ai_Time + 4);
                    *(int *)(msg + 0x30) = *(int *)(ai_Time + 8);
                    *(short *)(msg + 0x34) = *(short *)(ai_Time + 12);
                    msg[0x36] = 2;
                    *(int *)(msg + 0x58) = *(int *)(obj + 1);
                    *(int *)(msg + 0x5c) = (int)h.e[i].val;
                    msg[0xb0] = 1;
                    cm_SendLongMessage(msg);
                } else if (h.e[i].state == 2) {
                    dmg = (char)(int)(-h.e[i].val);
                    cm_BeginFieldBlock(obj, *(int *)(obj + 1));
                    cm_AddFieldValues(1, 1, 0x5c, &dmg);
                    cm_SendFieldDeltas();
                }
            }
        }
    }
    if (out != 0)
        memcpy(out, &h, 0x38);
}
