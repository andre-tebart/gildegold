/* 0x439870, updates the object's world matrix (mesh->worldMatrix) and its matrix relative to the
   camera (mesh->cameraMatrix) unless they are current. */

void d3_UpdateWorldMatrix(D3OBJECT *obj, D3OBJECT *camera, char force)
{
    D3MATRIX m;

    if (!(obj->stateFlags & 0x80) || force) {
        d3_WorldMatrixD(obj, 0, 0, D3_MESH(obj)->worldMatrix);
        obj->stateFlags |= 0x80;
    }
    if (!(obj->flags & 1) || force || D3_MESH(obj)->camera != camera) {
        if (camera && !(obj->state & 0x80)) {
            D3E_COPY3(&m, D3_MESH(obj)->worldMatrix, 0x40);
            m._41 -= camera->pos.x;
            m._42 -= camera->pos.y;
            m._43 -= camera->pos.z;
            m._41 -= camera->posOffset.x;
            m._42 -= camera->posOffset.y;
            m._43 -= camera->posOffset.z;
            d3_MatMul43(&m, &camera->matrix, (D3MATRIX *)D3_MESH(obj)->cameraMatrix);
        } else
            D3E_COPY3(D3_MESH(obj)->cameraMatrix, D3_MESH(obj)->worldMatrix, 0x40);
        D3_MESH(obj)->camera = camera;
        obj->flags |= 1;
    }
}
