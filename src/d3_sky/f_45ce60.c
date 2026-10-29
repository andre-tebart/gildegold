/* 0x45ce60 */
char d3sky_DrawLayer(D3SKY *s, D3SKYLAYER *l)
{
    union {
        unsigned long d;
        D3COLOR c;
    } col;
    float scale;
    int i;

    if (!s || s->num_buffer <= 0 || !l || !l->t)
        return 0;
    scale = (l->scale * 0.5f) / 3.1415927f;
    if (!l->alpha)
        return 1;
#line 38 "D:\\develop\\Units\\d3\\d3_sky.c"
    D3DCHECK(d3d_Device->lpVtbl->SetTexture(d3d_Device, 0, l->t->surftext));
    d3_SetBlendState(l->alpha < 0xff, l->flags.b[2] & 1, (l->flags.b[2] >> 1) & 1);
    if (l->flags.b[2] & 1) {
        col.c.r = s->color.r * s->color.a >> 8;
        col.c.g = s->color.g * s->color.a >> 8;
        col.c.b = s->color.b * s->color.a >> 8;
    } else
        col.d = RGBA_MAKE(s->color.r, s->color.g, s->color.b, l->alpha);
    for (i = 0; i < s->num_buffer; i++) {
        if (s->buffer[i].visible) {
            s->buffer[i].color = col.d;
            s->buffer[i].tu = (s->dir[i].w + s->rot) * scale + l->offset;
            s->buffer[i].tv = (-(l->vscale * s->dir[i].y)) + l->voffset;
        }
    }
    if (s->num_buffer && s->num_indices) {
#line 67
        D3DCHECK(d3d_Device->lpVtbl->DrawIndexedPrimitive(d3d_Device, D3DPT_TRIANGLELIST, D3D_VERTEX_STRUCT, (void*)&s->buffer, s->num_buffer, s->indices, s->num_indices, D3DDP_DONOTUPDATEEXTENTS | D3DDP_DONOTLIGHT | D3DDP_DONOTCLIP));
        return d3d_hr == 0;
    }
    return 1;
}
