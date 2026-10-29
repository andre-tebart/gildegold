/* 0x45d690, places the vertices on the screen; build: also for a new sky */

void d3sky_Init(D3SKY *s, char build)
{
    D3VECTOR4 dir, edge[2];
    float sx, sy, dx, dy;
    int x, y;

    if (!s)
        return;
    s->num_buffer = -1;
    if (!build)
        return;
    dx = (d3_Viewport.right - d3_Viewport.left) * 0.14285715f;
    dy = (d3_Viewport.bottom - d3_Viewport.top) * 0.2f;
    sy = (float)d3_Viewport.top;
    for (y = 0; y < D3SKY_ROWS; y++) {
        d3_Lerp3(&d3_Frustum[3], &d3_Frustum[1], y * 0.2f, &edge[0]);
        d3_Lerp3(&d3_Frustum[2], &d3_Frustum[0], y * 0.2f, &edge[1]);
        sx = (float)d3_Viewport.left;
        for (x = 0; x < D3SKY_COLS; x++) {
            d3_Lerp3(&edge[0], &edge[1], x * 0.14285715f, &dir);
            d3_Normalize(&dir);
            s->dir[y * D3SKY_COLS + x].w = (float)atan2(dir.x, dir.z);
            s->buffer[y * D3SKY_COLS + x].sx = sx;
            s->buffer[y * D3SKY_COLS + x].sy = sy;
            s->buffer[y * D3SKY_COLS + x].sz = 1.0f;
            s->buffer[y * D3SKY_COLS + x].rhw = 1.0f;
            sx += dx;
        }
        sy += dy;
    }
}
