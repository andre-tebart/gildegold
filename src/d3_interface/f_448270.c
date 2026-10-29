/* 0x448270, draws sprite `t` centred at screen position x, y, rotated by `angle`, scaled by sx, sy, with
   alpha 0..1, over the whole screen */

#define D3D_VERTEX_STRUCT D3DFVF_TLVERTEX

char d3if_DrawSprite(D3IFSPRITE *t, int x, int y, float angle, float sx, float sy, float alpha)
{
    unsigned char a;
    float w, h, vx, vy;
    float size, s, c;
    D3DTLVERTEX TurboBuffer[4];
    float corner[4][4];
    unsigned long color;
    int i;

    a = alpha * 255.0;
    if (!t || t->frames <= 0 || !d3d_Device || !d3_Hardware || d3_Busy > 0)
        return 0;
    d3_Busy++;
    w = d3d_ViewW;
    h = d3d_ViewH;
    vx = d3d_ViewX;
    vy = d3d_ViewY;
    d3d_SetViewport((float)dd_Width, (float)dd_Height, 0, 0, d3d_Near, d3d_Far);
    d3_BeginScene(0, 0, 0);
    D3_ZERO(TurboBuffer, sizeof TurboBuffer);
#line 973
    D3DCHECK(d3d_Device->lpVtbl->SetTexture(d3d_Device, 0, t->surftext));
    d3_SetBlendState(t->colorkey < 0xff, t->flags & 1, (t->flags >> 1) & 1);
    sx *= 0.5f;
    sy *= 0.5f;
    size = (float)t->size;
    corner[0][0] = size * sx;
    corner[0][1] = size * sy;
    corner[0][2] = 1.0f;
    corner[0][3] = 1.0f;
    corner[1][0] = -(size * sx);
    corner[1][1] = size * sy;
    corner[1][2] = 0.0f;
    corner[1][3] = 1.0f;
    corner[2][0] = -(size * sx);
    corner[2][1] = -(size * sy);
    corner[2][2] = 0.0f;
    corner[2][3] = 0.0f;
    corner[3][0] = size * sx;
    corner[3][1] = -(size * sy);
    corner[3][2] = 1.0f;
    corner[3][3] = 0.0f;
    color = (a << 24) | D3IF_RGB_MASK;
    s = (float)sin(angle);
    c = (float)cos(angle);
    for (i = 0; i < 4; i++) {
        TurboBuffer[i].sx = c * corner[i][0] - s * corner[i][1] + x;
        TurboBuffer[i].sy = s * corner[i][0] + c * corner[i][1] + y;
        TurboBuffer[i].sz = 0;
        TurboBuffer[i].rhw = 1.0f;
        TurboBuffer[i].color = color;
        TurboBuffer[i].tu = corner[i][2];
        TurboBuffer[i].tv = corner[i][3];
    }
#line 1008
    D3DCHECK(d3d_Device->lpVtbl->DrawPrimitive(d3d_Device, D3DPT_TRIANGLEFAN, D3D_VERTEX_STRUCT, (void*) &TurboBuffer, 4, D3DDP_WAIT));
    d3_EndScene();
    d3d_SetViewport(w, h, vx, vy, d3d_Near, d3d_Far);
    d3_Busy--;
    return 1;
}
