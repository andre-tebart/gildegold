/* 0x454f70: is the mirrored bounding box of obj inside the mirror's clipping planes? Widens d3_MinZ / d3_MaxZ. */

char d3_MirBoxInPlanes(D3OBJECT *obj, D3MIRROR *m)
{
    D3OBJINFO *info;
    D3MIRPLANES *pl;
    D3BOXVERT *box;
    D3VECTOR4 pts[8];
    D3VECTOR4 *p;
    float d;
    int i;
    unsigned int j, k;

    if (obj && (info = obj->info) && info->mesh && m && (pl = m->planes)) {
        if (obj->state & 0x10)
            box = D3_OPS(info)->GetBox(obj, info, D3_MESH(obj)->cameraMatrix);
        else
            box = (D3BOXVERT *)&info->keys[info->nkeys];
        for (i = 0; i < 8; i++, box++) {
            d = (box->x * m->plane.x + box->y * (m->plane.y) + (box->z) * (m->plane.z) - m->plane.w) * -2.0f;
            pts[i].x = d * m->plane.x + box->x;
            pts[i].y = d * m->plane.y + box->y;
            pts[i].z = d * m->plane.z + box->z;
        }
        for (j = 0; j < pl->n; j++) {
            for (k = 0; k < 8; k++)
                if (pts[k].x * pl->p[j].x + pts[k].y * pl->p[j].y + (pts[k].z * pl->p[j].z) >= pl->p[j].w)
                    break;
            if (k >= 8)
                return 0;
        }
        for (i = 0; i < 8; i++) {
            d3_MinZ = min(d3_MinZ, pts[i].z);
            d3_MaxZ = max(d3_MaxZ, pts[i].z);
        }
        return 1;
    }
    return 0;
}
