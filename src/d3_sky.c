/* d3_sky.c -- original path D:\develop\Units\d3\d3_sky.c
   Extent in GildeGold.exe: 0x45ce60..0x45db40 (start: first D3DCHECK at line 38 of the original file;
   .data from 0x5fce40). Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/d3_sky.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
/* d3.h declares these with int parameters; the callers here pass chars */
#define d3_BeginScene d3h_BeginScene
#define d3_SetBlendState d3h_SetBlendState
#include "d3.h"
#undef d3_BeginScene
#undef d3_SetBlendState
#include "d3_sky.h"

#define D3D_VERTEX_STRUCT D3DFVF_TLVERTEX

void d3_BeginScene(unsigned char fog, DWORD fogcol, unsigned char zbuffer);                                      /* 0x4385a0 */
void d3_SetBlendState(unsigned char balpha, unsigned char glow, unsigned char colorkey);                                             /* 0x4387b0 */
void d3_Lerp3(float *a, float *b, float t, float *out);                     /* 0x4539b0 */
float d3_VecAngle(D3VECTOR *a, D3VECTOR *b);                                            /* 0x4539f0 */
void d3_MatTranspose(D3MATRIX *src, D3MATRIX *dst);                                        /* 0x454040 */
extern D3VECTOR d3_ZAxis;                   /* 0x5ebb74, (0, 0, 1) */
extern D3VECTOR4 d3_Frustum[5];             /* 0xfc2960, corner directions of the view */
extern char d3_FogOn;                       /* 0x6334a4, d3_engine.h */
extern unsigned long d3_FogColor;           /* 0x6334a0, d3_engine.h */
extern unsigned char d3_Hardware;                    /* 0x633450 */

extern int d3_Busy;                         /* 0x633b04, also counted by d3sm_Build */
D3SKY *d3_Sky = 0;                          /* 0x633dd4 */
int d3sky_Colors[6] = {0}; /* 0xfb91c0 */
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
/* 0x45d500, draws the sky: advances every layer's fade and texture offset by the elapsed time, picks the lowest layer that is fully opaque and not additive, and draws it and its successors between BeginScene and EndScene (not while another sky draw is in flight); returns 0 without a device, hardware or sky, else 1 */

char d3sky_Draw(D3SKY *s)
{
    D3SKYLAYER *l, *base;
    int a;

    if (!d3d_Device || !d3_Hardware || !s)
        return 0;
    if (s->num_buffer == -1)
        d3sky_Build(s);
    s->dt = (float)(d3_Time - s->lasttime);
    s->lasttime = d3_Time;
    if (s->layers) {
        for (l = s->layers; l; l = l->next) {
            if ((l->fade = l->fadespeed * s->dt + l->fade) > 1.0) {
                l->fadespeed = 0;
                l->fade = 1.0f;
            }
            a = (int)((float)l->alpha0 + ((float)l->alpha1 - (float)l->alpha0) * l->fade);
            if (a > 0xff)
                a = 0xff;
            l->alpha = a;
            l->offset = (float)fmod(l->speed * s->dt + l->offset, 1.0);
        }
        base = s->layers;
        for (l = base->next; l; l = l->next) {
            if (l->alpha == 0xff && !(l->flags.b[2] & 1) && !(l->flags.b[2] & 2))
                base = l;
        }
        if (base && s->num_buffer > 0 && d3_Busy <= 1) {
            d3_Busy++;
            d3_BeginScene(d3_FogOn, d3_FogColor, 0);
            for (l = base; l; l = l->next)
                d3sky_DrawLayer(s, l);
            d3_EndScene();
            d3_Busy--;
        }
    }
    return 1;
}
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
/* 0x45d800, unlinks the layer l from the sky's list (the head or after its predecessor), releases its texture and frees the layer; does nothing without a sky or layer */

void d3sky_DeleteLayer(D3SKY *s, D3SKYLAYER *l)
{
    D3SKYLAYER *p;

    if (s && l) {
        if (s->layers == l)
            s->layers = l->next;
        else {
            for (p = s->layers; p && p->next != l; p = p->next)
                ;
            if (p && p->next == l)
                p->next = l->next;
        }
        tex_Release(l->t);
        mem_Free(l);
    }
}
/* 0x45d880, creates a sky layer with texture tex and links it into s: at the head when first, else at the
   tail; returns the layer, 0 when s or tex is missing or the allocation fails */

D3SKYLAYER *d3sky_CreateLayer(D3SKY *s, int first, char *tex, D3TEXFLAGS flags, float scale, float offset,
                              float vscale, float voffset)
{
    D3SKYLAYER *l, *ret = 0;
    D3SKYLAYER *p;

    if (s && tex) {
      if (l = mem_Alloc(sizeof(D3SKYLAYER), "d3_sky:CreateLayer")) {
        l->scale = scale;
        l->offset = offset;
        l->speed = 0;
        l->vscale = vscale;
        l->voffset = voffset;
        l->alpha0 = flags.b[0];
        l->alpha1 = flags.b[0];
        l->fade = 1.0f;
        l->fadespeed = 0;
        l->flags = flags;
        l->t = (D3TEXHANDLE *)tex_Load(tex, flags, 0, 0);
        if (l->t)
            tex_Reload(l->t, 0);
        if (!first) {
            l->next = 0;
            if (s->layers) {
                for (p = s->layers; p->next; p = p->next)
                    ;
                p->next = l;
                return l;
            }
            /* s->layers = l in both arms, not once after the if: the extra s references give s edi (VC6) */
            s->layers = l;
        } else {
            l->next = s->layers;
            s->layers = l;
        }
      }
      ret = l;
    }
    return ret;
}
/* 0x45d960, sets the scroll speed of the layer (the caller's units scaled by -1e-06 into the layer's per-ms speed); 0 stops it */

void d3sky_SetLayerSpeed(D3SKY *s, D3SKYLAYER *l, float speed)
{
    if (s && l) {
        if (speed != 0.0f)
            l->speed = speed * -1e-06f;
        else
            l->speed = 0;
    }
}
/* 0x45d9a0, fades the layer to alpha in time ms */

void d3sky_FadeLayer(D3SKY *s, D3SKYLAYER *l, unsigned char alpha, float time)
{
    if (s && l) {
        /* alpha0 is stored in both arms and the statement order is needed for VC6's registers */
        if (time != 0.0f) {
            l->alpha0 = l->alpha;
            l->fadespeed = 1.0f / time;
            l->alpha1 = alpha;
            l->fade = 0;
        } else {
            l->alpha0 = l->alpha;
            l->fade = 1.0f;
            l->fadespeed = 0;
            l->alpha1 = l->alpha;
        }
    }
}
/* 0x45d9f0, replaces the layer's texture: releases the old one, loads name with the layer's flags and reloads it; the layer keeps no texture when the load fails */

void d3sky_SetLayerTex(D3SKYLAYER *l, char *name)
{
    tex_Release(l->t);
    l->t = (D3TEXHANDLE *)tex_Load(name, l->flags, 0, 0);
    if (l->t)
        tex_Reload(l->t, 0);
}
/* 0x45da30, allocates a sky (0xacc bytes, colour white), stores the horizon and fog parameters, initialises its vertex grid and returns it; 0 when the allocation fails @deep readability Q4: *(unsigned long *)&s->color = 0xffffffff needs the dword view of D3COLOR (b,g,r,a): the original is a single mov DWORD [s+0xaa4],0xffffffff, while four typed byte stores and memset(&s->color,0xff,4) both change its code (85 / 66 bytes), and an unsigned long member in D3COLOR raises its alignment from 1 to 4 (layout risk for every embedder); behaviour met */

D3SKY *d3sky_Create(float horizon, float fogstart, float fogscale)
{
    D3SKY *s = mem_Alloc(sizeof(D3SKY), "d3_sky:Create");

    if (s) {
        *(unsigned long *)&s->color = 0xffffffff;
        s->layers = 0;
        s->horizon = horizon;
        s->lasttime = d3_Time;
        s->dt = 0;
        s->fogstart = fogstart;
        s->fogscale = fogscale;
        d3sky_Init(s, 1);
    }
    return s;
}
/* 0x45daa0 */

void d3sky_Destroy(D3SKY *s)
{
    if (s) {
        while (s->layers)
            d3sky_DeleteLayer(s, s->layers);
        mem_Free(s);
    }
}
/* 0x45dae0, stores the six sky shades in d3sky_Colors (edge, inner, centre, inner, edge; slot 0 is not one of them) */

void d3sky_InitColors(void)
{
    d3sky_Colors[0] = D3SKY_COLOR0;
    d3sky_Colors[1] = D3SKY_COLOR_EDGE;
    d3sky_Colors[2] = D3SKY_COLOR_INNER;
    d3sky_Colors[3] = D3SKY_COLOR_CENTER;
    d3sky_Colors[4] = D3SKY_COLOR_INNER;
    d3sky_Colors[5] = D3SKY_COLOR_EDGE;
}
/* 0x45db20 */

void d3sky_Free(void)
{
    if (d3_Sky) {
        d3sky_Destroy(d3_Sky);
        d3_Sky = 0;
    }
}
