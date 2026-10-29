/* 0x434d60, per key frame: vertex normals (3 bytes, 0..255), radius and bounding box; the boxes are
   then widened to cover the neighbouring frames.
   @deep B draft: 34 code hunks, all of one form - [eax+esi+0x3c] vs [esi+eax+0x3c], the SIB base/index roles of the same frame-array address computation; every operation and constant is identical on both sides, so this is a VC6 addressing-mode choice (the d3path_Init note records the same pattern resisting (p->keys + i)-> / i[p->keys]), not a meaning error. Readable otherwise. */

typedef struct D3ANIMBOX {
    D3VECTOR4 min, max;
} D3ANIMBOX;

void d3anim_CalculateAnimNormals(D3ANIMDATA *a, D3ANIMMESH *mesh)
{
    D3VECTOR4 *points;
    D3VECTOR4 *normals;
    D3ANIMBOX *bbox;
    D3ANIMFRAME *fr;
    D3VECTOR4 n;
    D3VECTOR4 mn, mx;
    int f, i, k;

    points = mem_Alloc(mesh->nverts << 4, "d3_anim:CalculateAnimNormals(points)");
    normals = mem_Alloc(mesh->nfaces << 4, "d3_anim:CalculateAnimNormals(normals)");
    for (f = 0; f < a->nframes; f++) {
        fr = &a->frames[f];
        fr->normals = mem_Alloc(mesh->nverts * 3, "d3_anim:CalculateAnimNormals");
        if (fr->pos) {
            for (i = 0; i < a->nverts; i++) {
                points[i].x = mesh->verts[i].pos.x + fr->pos[i].x;
                points[i].y = mesh->verts[i].pos.y + fr->pos[i].y;
                points[i].z = mesh->verts[i].pos.z + fr->pos[i].z;
            }
        } else {
            for (i = 0; i < a->nverts; i++) {
                n.x = (float)fr->packed[i * 3] * fr->scale.x + fr->base.x;
                n.y = (float)fr->packed[i * 3 + 1] * fr->scale.y + fr->base.y;
                n.z = (float)fr->packed[i * 3 + 2] * fr->scale.z + fr->base.z;
                points[i].x = n.x + mesh->verts[i].pos.x;
                points[i].y = n.y + mesh->verts[i].pos.y;
                points[i].z = n.z + mesh->verts[i].pos.z;
            }
        }
        fr->radius = mesh->radius - d3anim_MaxRadius(points, mesh->nverts);
        for (i = 0; i < mesh->nfaces; i++)
            d3_TriNormal(&points[mesh->faces[i].v[0]], &points[mesh->faces[i].v[1]],
                         &points[mesh->faces[i].v[2]], &normals[i]);
        mn.z = mn.y = mn.x = 1e10f;
        mx.z = mx.y = mx.x = -1e10f;
        for (i = 0; i < a->nverts; i++) {
            mn.x = min(mn.x, points[i].x);
            mn.y = min(mn.y, points[i].y);
            mn.z = min(mn.z, points[i].z);
            mx.x = max(mx.x, points[i].x);
            mx.y = max(mx.y, points[i].y);
            mx.z = max(mx.z, points[i].z);
            n.z = n.y = n.x = 0.0f;
            for (k = 0; k < mesh->nfaces; k++) {
                if (mesh->faces[k].v[0] == i || mesh->faces[k].v[1] == i || mesh->faces[k].v[2] == i) {
                    n.x += normals[k].x;
                    n.y += normals[k].y;
                    n.z += normals[k].z;
                }
            }
            d3_Normalize(&n);
            n.x += 1.0f;
            n.y += 1.0f;
            n.z += 1.0f;
            fr->normals[i * 3] = (unsigned char)(n.x * 127.5);
            fr->normals[i * 3 + 1] = (unsigned char)(n.y * 127.5);
            fr->normals[i * 3 + 2] = (unsigned char)(n.z * 127.5);
        }
        fr->min.x = mn.x;
        fr->min.y = mn.y;
        fr->min.z = mn.z;
        fr->max.x = mx.x;
        fr->max.y = mx.y;
        fr->max.z = mx.z;
    }
    mem_Free(normals);
    mem_Free(points);
    bbox = mem_Alloc(a->nframes << 5, "d3_anim:CalculateAnimNormals(bbox_points)");
    for (f = 0; f < a->nframes; f++) {
        bbox[f].min.x = a->frames[f].min.x;
        bbox[f].min.y = a->frames[f].min.y;
        bbox[f].min.z = a->frames[f].min.z;
        bbox[f].max.x = a->frames[f].max.x;
        bbox[f].max.y = a->frames[f].max.y;
        bbox[f].max.z = a->frames[f].max.z;
    }
    for (f = 0; f < a->nframes; f++) {
        if (f > 0) {
            bbox[f].min.x = min(bbox[f].min.x, a->frames[f - 1].min.x);
            bbox[f].min.y = min(bbox[f].min.y, a->frames[f - 1].min.y);
            bbox[f].min.z = min(bbox[f].min.z, a->frames[f - 1].min.z);
            bbox[f].max.x = max(bbox[f].max.x, a->frames[f - 1].max.x);
            bbox[f].max.y = max(bbox[f].max.y, a->frames[f - 1].max.y);
            bbox[f].max.z = max(bbox[f].max.z, a->frames[f - 1].max.z);
        }
        if (f < a->nframes - 1) {
            bbox[f].min.x = min(bbox[f].min.x, a->frames[f + 1].min.x);
            bbox[f].min.y = min(bbox[f].min.y, a->frames[f + 1].min.y);
            bbox[f].min.z = min(bbox[f].min.z, a->frames[f + 1].min.z);
            bbox[f].max.x = max(bbox[f].max.x, a->frames[f + 1].max.x);
            bbox[f].max.y = max(bbox[f].max.y, a->frames[f + 1].max.y);
            bbox[f].max.z = max(bbox[f].max.z, a->frames[f + 1].max.z);
        }
    }
    for (f = 0; f < a->nframes; f++) {
        a->frames[f].min.x = bbox[f].min.x;
        a->frames[f].min.y = bbox[f].min.y;
        a->frames[f].min.z = bbox[f].min.z;
        a->frames[f].max.x = bbox[f].max.x;
        a->frames[f].max.y = bbox[f].max.y;
        a->frames[f].max.z = bbox[f].max.z;
    }
    mem_Free(bbox);
}
