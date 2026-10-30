/* 0x4486e0, the screen rectangle covered by an object: a square around the centre for type 0, the projected
   bounding box (clipped to the viewport) for meshes */

char d3if_ScreenBox(D3OBJECT *obj, D3RECT *r)
{
    D3VECTOR4 v;
    D3BOXVERT *b;
    D3OBJINFO *info;
    float f, x, y, s;
    int i, ix, iy;

    if (r && obj) {
        if (obj->type == 0) {
            d3_PointToRefD(obj, d3_Camera, &obj->pos, &v);
            f = 1.0f / v.z;
            x = d3_ScaleX * v.x * f + d3_CenterX;
            y = d3_ScaleY * v.y * f + d3_CenterY;
            s = d3_ScaleX * D3IF_OBJ068(obj) * f * 0.5f;
            r->left = (int)(x - s);
            r->right = (int)(x + s);
            r->top = (int)(y - s);
            r->bottom = (int)(y + s);
            return 1;
        }
        if (D3_MESH(obj) && !(D3_MESH(obj)->flags & 0x40) && (info = obj->info) && info->mesh) {
            d3_UpdateWorldMatrix(obj, (obj->state & 0x80) ? 0 : d3_Camera, 0);
            b = D3_OPS(info)->GetBox(obj, info, D3_MESH(obj)->cameraMatrix);
            r->top = 0x10000;
            r->left = 0x10000;
            r->bottom = -0x10000;
            r->right = -0x10000;
            for (i = 0; i < 8; i++) {
                f = 1.0f / b[i].z;
                ix = (int)(d3_ScaleX * b[i].x * f + d3_CenterX);
                r->left = min(r->left, ix);
                r->right = max(r->right, ix);
                iy = (int)(d3_ScaleY * b[i].y * f + d3_CenterY);
                r->top = min(r->top, iy);
                r->bottom = max(r->bottom, iy);
            }
            r->left = max(r->left, d3_Viewport.left);
            r->right = min(r->right, d3_Viewport.right - 1);
            r->top = max(r->top, d3_Viewport.top);
            r->bottom = min(r->bottom, d3_Viewport.bottom - 1);
            return 1;
        }
    }
    return 0;
}
