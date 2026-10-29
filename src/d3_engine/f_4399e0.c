/* 0x4399e0, the object's clip code: clips its bounding box against the view frustum (0x40 = outside)
   and stores the box's z range in the mesh (zMin / zMax). */

unsigned char d3_ClipObject(D3OBJECT *obj, D3OBJINFO *info, D3OBJECT *camera)
{
    unsigned char clip;
    void *box;

    d3_UpdateWorldMatrix(obj, camera, 0);
    if (obj->objectFlags & 0x20) {
        clip &= ~0x40;
        clip |= 0x3f;
        return clip;
    }
    box = 0;
    if (info && info->mesh)
        box = D3_OPS(info)->GetBox(obj, info, D3_MESH(obj)->cameraMatrix);
    if (!box) {
        clip |= 0x40;
        return clip;
    }
    return d3_ClipBox(box, &D3_MESH(obj)->zMin, &D3_MESH(obj)->zMax);
}
