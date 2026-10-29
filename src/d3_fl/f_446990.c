/* 0x446990, distance along the view rays to the edge of the map volume (4 planes through the corners) */

/* ---- 0x446990..0x446e10, the end of the TU (worker d3if; was unk_446990.c until round 9). The next file,
   d3_interface.c, starts at 0x446e10 (its first string, the SelectTextureSet message at 0x5fbd08, follows
   the 1.0f constant of this TU). ---- */

float d3fl_FarDist(D3FLMAP *map, D3VECTOR *pos, D3MATRIX *m)
{
    D3VECTOR4 pl[4];
    D3VECTOR4 u, v;
    float dx, dy, dz, f, best, zmin, t, t4, t5, t6, one = -1.0f;
    int i, j;
    D3VECTOR4 *r;

    dx = map->orgx - pos->x;
    dy = map->orgy - pos->y;
    dz = map->orgz - pos->z;
    u.x = dy * m->_21 + dz * m->_31 + dx * m->_11;
    u.y = dy * m->_22 + dz * m->_32 + dx * m->_12;
    u.z = dx * m->_13 + dy * m->_23 + dz * m->_33;
    t4 = m->_21 * 0.0f;
    pl[0].x = (m->_11 * 0.0f + m->_31 * one) + t4;
    t5 = m->_22 * 0.0f;
    pl[0].y = (m->_12 * 0.0f - m->_32 * one) + t5;
    t6 = m->_23 * 0.0f;
    pl[0].z = (m->_13 * 0.0f - m->_33 * one) + t6;
    pl[0].w = pl[0].x * u.x + pl[0].y * u.y + pl[0].z * u.z;
    pl[1].x = (m->_31 * 0.0f + t4) + m->_11;
    pl[1].y = (m->_32 * 0.0f + t5) + m->_12;
    pl[1].z = (m->_33 * 0.0f + t6) + m->_13;
    pl[1].w = pl[1].x * u.x + pl[1].y * u.y + pl[1].z * u.z;
    f = (float)(map->size - 4);
    v.x = f * map->scale + map->orgx;
    v.y = f * map->xAxisY + map->orgy;
    v.z = f * map->xAxisZ + map->orgz;
    v.x = f * map->zAxisX + v.x;
    v.y = f * map->zAxisY + v.y;
    v.z = f * map->negScale + v.z;
    v.x -= pos->x;
    v.y -= pos->y;
    v.z -= pos->z;
    u.x = v.y * m->_21 + v.z * m->_31 + v.x * m->_11;
    u.y = v.y * m->_22 + v.z * m->_32 + v.x * m->_12;
    u.z = v.x * m->_13 + v.y * m->_23 + v.z * m->_33;
    pl[2].x = (m->_31 * 0.0f + t4) + m->_11 * one;
    pl[2].y = (m->_32 * 0.0f + t5) - m->_12 * one;
    pl[2].z = (m->_33 * 0.0f + t6) - m->_13 * one;
    pl[2].w = pl[2].x * u.x + pl[2].y * u.y + pl[2].z * u.z;
    pl[3].x = (m->_11 * 0.0f + t4) + m->_31;
    pl[3].y = (m->_12 * 0.0f + t5) + m->_32;
    pl[3].z = (m->_13 * 0.0f + t6) + m->_33;
    pl[3].w = pl[3].x * u.x + pl[3].y * u.y + pl[3].z * u.z;
    zmin = min(pl[0].w, min(pl[1].w, min(pl[2].w, pl[3].w)));
    best = 1e10f;
    for (r = d3fl_Rays, i = 0; i < 5; i++, r++)
        for (j = 0; j < 4; j++) {
            t = pl[j].x * r->x + pl[j].z * r->z + r->y * pl[j].y;
            if ((t < -1e-07f || t > 1e-07f) && (t = pl[j].w / t, t > 0.0f) && (t > zmin && t < best))
                best = t;
        }
    if (best == 1e10)
        return fabs(zmin);
    return best;
}
