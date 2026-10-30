/* 0x45e620, (re)builds the map from the terrain and the objects of the default scene @deep behaviour: 669 code hunks (frame sub esp 0x110 vs 0x108, the pos/rot copies and corner[8]/box/mn/mx/uv locals land on other stack slots; the asmdiff mnemonics differ as well: 7 je vs 1, 7 jp vs 2, fdiv vs fdivr, extra and/lea - the short-circuit chains and the FP divisions are compiled in another form); readability: Q2 three callees in other units still unnamed (d3_RadixSort 0x43b300 sorts d3_SortBuf, d3_TransformMeshes 0x43d840, d3s_ShadowTriangle 0x45a2e0) and the field rotation, Q4 two raw dereferences ('*(D3VECTOR *)&d3_Camera->pos' / '->rotation', fixable to plain struct copies but Q2 stays); needs the whole toolbox and the other units' names */

void d3sm_Build(D3SM *h, D3TERRAIN *t)
{
    D3VECTOR4 corner[8];
    D3VECTOR pos, rot;
    D3VECTOR4 box[2];
    D3VECTOR4 mn, mx;
    D3VECTOR4 nrm;
    float uv[3][2];
    unsigned char hs[3];
    float ax, ay, az, f, lo, hi, x0, y0, z0;
    unsigned int k, top, bot;
    int i, j, radius, last;
    D3SMTRI *tri;
    D3SMBLOCK *blk;

    if (!h || h->size <= 0 || !h->data || !d3_Camera)
        return;
    d3_Busy++;
    pos = *(D3VECTOR *)&d3_Camera->pos;
    rot = *(D3VECTOR *)&d3_Camera->rotation;
    d3_SetObjectPosXYZ(d3_Camera, 0, 0, 0);
    d3_SetObjectRotXYZ(d3_Camera, 0, 0, 0);
    d3_TransformMeshes();
    box[0].x = 1e10f;
    box[0].y = 1e10f;
    box[0].z = 1e10f;
    box[1].x = -1e10f;
    box[1].y = -1e10f;
    box[1].z = -1e10f;
    d3_ForEachObject(d3_DefaultScene, 0, 0x40, (D3OBJECTCB)d3sm_BoundsCB, box);
    if (h->entries) {
        D3_ZERO(h->entries, h->size * h->size * sizeof(D3SMENTRY));
        for (i = 0; i < h->size * h->size; i++)
            h->entries[i].type = 1;
    }
    if (t) {
        top = 0;
        bot = 0xff;
        blk = &t->blocks[0][0];
        for (i = 8; i; i--) {
            for (j = 8; j; j--, blk++) {
                top = max(top, blk->hi);
                bot = min(bot, blk->lo);
            }
        }
        corner[0].x = bot * t->axisy.x + t->origin.x;
        corner[0].y = bot * t->axisy.y + t->origin.y;
        corner[0].z = bot * t->axisy.z + t->origin.z;
        f = (float)t->size;
        corner[1].x = (ax = f * t->axisx.x) + corner[0].x;
        corner[1].y = (ay = f * t->axisx.y) + corner[0].y;
        corner[1].z = (az = f * t->axisx.z) + corner[0].z;
        corner[2].x = f * t->axisz.x + corner[0].x;
        corner[2].y = f * t->axisz.y + corner[0].y;
        corner[2].z = f * t->axisz.z + corner[0].z;
        corner[3].x = corner[2].x + ax;
        corner[3].y = corner[2].y + ay;
        corner[3].z = corner[2].z + az;
        f = (float)(int)(top - bot);
        for (i = 0; i < 4; i++) {
            corner[i + 4].x = f * t->axisy.x + corner[i].x;
            corner[i + 4].y = f * t->axisy.y + corner[i].y;
            corner[i + 4].z = f * t->axisy.z + corner[i].z;
        }
        mn.x = 1e10f;
        mn.y = 1e10f;
        mn.z = 1e10f;
        mx.x = -1e10f;
        mx.y = -1e10f;
        mx.z = -1e10f;
        for (i = 0; i < 8; i++) {
            mn.x = min(corner[i].x, mn.x);
            mn.y = min(corner[i].y, mn.y);
            mn.z = min(corner[i].z, mn.z);
            mx.x = max(corner[i].x, mx.x);
            mx.y = max(corner[i].y, mx.y);
            mx.z = max(corner[i].z, mx.z);
        }
        box[0].y = min(mn.y, box[0].y);
        box[1].y = max(mx.y, box[1].y);
        box[0].x = mn.x;
        box[1].x = mx.x;
        box[0].z = mn.z;
        box[1].z = mx.z;
    }
    h->origin.y = box[0].y + 1.0f;
    h->origin.x = box[0].x;
    h->origin.z = box[0].z;
    f = (float)h->size - 1.75f;
    h->cell.x = (box[1].x - box[0].x) / f;
    h->cell.y = (box[1].y - box[0].y) * (1.0f / 253.0f);
    h->cell.z = (box[1].z - box[0].z) / f;
    radius = (int)(50.0f / h->cell.x + 0.5f) / 4;
    if (radius < 1)
        radius = 1;
    else if (radius > 4)
        radius = 4;
    if (t)
        d3sm_Sample(t, h);
    else
        D3_ZERO(h->data, h->size * h->size);
    d3_SortCount = 0;
    d3_SortPtrV = d3_SortBufV;
    d3_ForEachObject(d3_DefaultScene, 0, 0x40, (D3OBJECTCB)d3sm_ShadowCB, h);
    d3_RadixSort(d3_SortCount, 0);
    for (k = 0; k < (unsigned int)d3_SortCount; k++) {
        tri = d3_SortBufV[k].tri;
        if (tri->solid) {
            hi = -1e10f;
            lo = 1e10f;
            for (i = 0; i < 3; i++) {
                f = tri->v[i]->y;
                uv[i][0] = tri->v[i]->gx;
                uv[i][1] = tri->v[i]->gz;
                hs[i] = tri->v[i]->h;
                if (f > hi) {
                    hi = f;
                    last = i;
                }
                if (f < lo) {
                    lo = f;
                    j = i;
                }
            }
            d3_TriNormal(tri->v[0], tri->v[1], tri->v[2], &nrm);
            if (fabs(tri->v[d3_TriEdge[j][1]]->y + tri->v[d3_TriEdge[j][0]]->y - (lo + lo))
                >= fabs(tri->v[d3_TriEdge[last][1]]->y + tri->v[d3_TriEdge[last][0]]->y - (hi + hi)))
                j = last;
            x0 = nrm.x * 0.875f;
            uv[j][0] -= x0;
            z0 = nrm.z * 0.875f;
            uv[j][1] -= z0;
            uv[(j + 1) % 3][0] += x0;
            uv[(j + 1) % 3][1] += z0;
            uv[(j + 2) % 3][0] += x0;
            uv[(j + 2) % 3][1] += z0;
            for (i = 0; i < 3; i++) {
                y0 = (float)(h->size - 1);
                uv[i][0] = max(0.0f, min(y0, uv[i][0]));
                uv[i][1] = max(0.0f, min(y0, uv[i][1]));
                hs[i] = tri->v[i]->h;
            }
        } else {
            for (i = 0; i < 3; i++) {
                y0 = (float)(h->size - 1);
                uv[i][0] = max(0.0f, min(y0, tri->v[i]->gx));
                uv[i][1] = max(0.0f, min(y0, tri->v[i]->gz));
                hs[i] = tri->v[i]->h;
            }
        }
        d3s_ShadowTriangle(&uv[0][0], hs, h->data, h->size, tri->v[0]->flags, h->entries, radius);
    }
    d3_SortCount = 0;
    d3_SortPtrV = d3_SortBufV;
    if (h->entries) {
        d3sm_Replace(h, 0xb, 0xc);
        for (i = 0; i < h->size; i++) {
            h->entries[i].type = 0xd;
            h->entries[(h->size - 1) * h->size + i].type = 0xd;
            h->entries[h->size * i].type = 0xd;
            h->entries[h->size * (i + 1) - 1].type = 0xd;
        }
    }
    d3_SetObjectPos(d3_Camera, &pos);
    d3_SetObjectRot(d3_Camera, &rot);
    d3_Busy--;
}
