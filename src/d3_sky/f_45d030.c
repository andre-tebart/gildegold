/* 0x45d030, rebuilds the directions, fog and indices from the camera */

void d3sky_Build(D3SKY *s)
{
    D3MATRIX m;
    D3VECTOR4 dir;
    D3VECTOR4 corner[4];
    D3VECTOR4 edge[2];
    int x, y, n;

    if (!s)
        return;
    dir.x = d3_ZAxis.x * d3_Camera->matrix._11 + d3_ZAxis.y * d3_Camera->matrix._21 + d3_ZAxis.z * d3_Camera->matrix._31;
    dir.y = d3_ZAxis.x * d3_Camera->matrix._12 + d3_ZAxis.y * d3_Camera->matrix._22 + d3_ZAxis.z * d3_Camera->matrix._32;
    dir.z = d3_ZAxis.x * d3_Camera->matrix._13 + d3_ZAxis.y * d3_Camera->matrix._23 + d3_ZAxis.z * d3_Camera->matrix._33;
    s->rot = (float)fmod(d3_VecAngle(&d3_ZAxis, (D3VECTOR *)&dir), 6.2831855f);
    s->num_buffer = D3SKY_ROWS * D3SKY_COLS;
    d3_MatTranspose(&d3_Camera->matrix, &m);
    corner[0].x = d3_Frustum[3].x * m._11 + d3_Frustum[3].y * m._21 + d3_Frustum[3].z * m._31;
    corner[0].y = d3_Frustum[3].x * m._12 + d3_Frustum[3].y * m._22 + d3_Frustum[3].z * m._32;
    corner[0].z = d3_Frustum[3].x * m._13 + d3_Frustum[3].y * m._23 + d3_Frustum[3].z * m._33;
    corner[1].x = d3_Frustum[2].x * m._11 + d3_Frustum[2].y * m._21 + d3_Frustum[2].z * m._31;
    corner[1].y = d3_Frustum[2].x * m._12 + d3_Frustum[2].y * m._22 + d3_Frustum[2].z * m._32;
    corner[1].z = d3_Frustum[2].x * m._13 + d3_Frustum[2].y * m._23 + d3_Frustum[2].z * m._33;
    corner[2].x = d3_Frustum[1].x * m._11 + d3_Frustum[1].y * m._21 + d3_Frustum[1].z * m._31;
    corner[2].y = d3_Frustum[1].x * m._12 + d3_Frustum[1].y * m._22 + d3_Frustum[1].z * m._32;
    corner[2].z = d3_Frustum[1].x * m._13 + d3_Frustum[1].y * m._23 + d3_Frustum[1].z * m._33;
    corner[3].x = d3_Frustum[0].x * m._11 + d3_Frustum[0].y * m._21 + d3_Frustum[0].z * m._31;
    corner[3].y = d3_Frustum[0].x * m._12 + d3_Frustum[0].y * m._22 + d3_Frustum[0].z * m._32;
    corner[3].z = d3_Frustum[0].x * m._13 + d3_Frustum[0].y * m._23 + d3_Frustum[0].z * m._33;
    for (y = 0; y < D3SKY_ROWS; y++) {
        d3_Lerp3(&corner[0], &corner[2], y * 0.2f, &edge[0]);
        d3_Lerp3(&corner[1], &corner[3], y * 0.2f, &edge[1]);
        for (x = 0; x < D3SKY_COLS; x++) {
            d3_Lerp3(&edge[0], &edge[1], x * 0.14285715f, &s->dir[y * D3SKY_COLS + x]);
            d3_Normalize(&s->dir[y * D3SKY_COLS + x]);
            s->buffer[y * D3SKY_COLS + x].visible = s->dir[y * D3SKY_COLS + x].y > s->horizon;
            s->buffer[y * D3SKY_COLS + x].fog = (unsigned char)min(255.0f, max(0.0f, (s->dir[y * D3SKY_COLS + x].y - s->fogstart) * s->fogscale));
        }
    }
    n = 0;
    for (y = 0; y < D3SKY_ROWS - 1; y++) {
        for (x = 0; x < D3SKY_COLS - 1; x++) {
            if (s->buffer[y * 8 + x].visible && s->buffer[y * 8 + x + 1].visible
                && s->buffer[y * 8 + x + 8].visible && s->buffer[y * 8 + x + 9].visible
                && (s->buffer[y * 8 + x].fog > 0 || s->buffer[y * 8 + x + 1].fog > 0
                    || s->buffer[y * 8 + x + 8].fog > 0 || s->buffer[y * 8 + x + 9].fog > 0)) {
                s->indices[n++] = x + y * 8;
                s->indices[n++] = x + 1 + y * 8;
                s->indices[n++] = x + 8 + y * 8;
                s->indices[n++] = x + 1 + y * 8;
                s->indices[n++] = x + 9 + y * 8;
                s->indices[n++] = x + 8 + y * 8;
            }
        }
    }
    s->num_indices = n;
}
