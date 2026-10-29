/* 0x45b510, a shadow mesh over the map cells in rect @deep B code hunks 546 with slots 4: every hunk read is the same load / store / FP operation on another stack slot ([esp+0x10..0x50] vs [esp+0x10..0x38], frame 0x24 vs 0x14) with the same struct offsets and literals on both sides (multiset comparison) and no call in the function; the vertices / polygons are computed in other temporaries - no missing statement or wrong constant found */

typedef struct TSTEXSIZE {          /* the parts of a texture slot (ts_texture.h: TSTEXTURE) used here */
    char pad00[0x74];
    unsigned int size;              /* 0x74 */
    char pad78[0x4];
    unsigned char bpp;              /* 0x7c */
} TSTEXSIZE;

void d3_DirToWorld(struct XOBJ *obj, float *in, float *out);                          /* 0x458810 */
unsigned char d3_ClipBox(void *box, float *zmin, float *zmax);                                           /* 0x439650 */
void dd_FillSurface(struct IDirectDrawSurface4 *s, int a, int b);                         /* 0x41e0a0 */
void * dd_LockSurfaceFlags(LPDIRECTDRAWSURFACE4 surf, DWORD flags);                               /* 0x41de30 */
void dd_UnlockSurface(LPDIRECTDRAWSURFACE4 surf);                                       /* 0x41e740 */
void d3d_BuildMipmaps(LPDIRECTDRAWSURFACE4 tex, DDSURFACEDESC2 *ddsd);                                /* 0x4371d0 */
void ts_CacheFetch(D3TEXHANDLE *tex, int a, int b);                                     /* 0x45f8d0 */

const float d3s_Dist[2] = { 60.0f, 200.0f };  /* 0x5ebbd8 */
const unsigned char d3s_Palette[] = { 0, 0, 0, 0xff, 0xff, 0xff, 0, 0 };  /* 0x5ebbe0, palette */
extern unsigned int d3_FrameCount;          /* 0x63343c */
extern char d3_Hardware;                    /* 0x633450 */
extern D3VECTOR d3_ZAxis;                   /* 0x5ebb74 */

float * d3s_Verts = 0;  /* 0xfb9258, 6 floats each */
int d3s_NumUVs = 0;  /* 0xfb925c */
unsigned int d3s_TexSize = 0;  /* 0xfb9260 */
unsigned int d3s_NumLights = 0;                 /* 0xfb9264 */
char d3s_Byte = 0;  /* 0xfb9268 */
int d3s_ShadowRadius = 0;  /* 0xfb926c, rows / pixels the marked span widens around the shadow */
D3SPANPIX * d3s_SpanRow = 0;  /* 0xfb9270, current row of the span buffer */
unsigned char * d3s_Dst = 0;  /* 0xfc2940 */
int d3s_EdgeC[3] = {0};  /* 0xfc29b0 */
int d3s_SlopeC = 0;  /* 0xfc2dd0, slope of the interpolated value along the left edge */
int d3s_SlopeL = 0;  /* 0xfc2de4 */
int d3s_CL = 0;  /* 0xfc2de8, interpolated value at the left edge */
int d3s_SlopeR = 0;  /* 0xfc2df8 */
int d3s_EdgeX[4] = {0};  /* 0xfc50d0 */
int d3s_EdgeY[4] = {0};  /* 0xfc50e0 */
int d3s_XL = 0;  /* 0xfc5334 */
int d3s_XR = 0;  /* 0xfc533c */
int d3s_SpanLen = 0;  /* 0xfe73a0 */
unsigned short * d3s_Dst16 = 0;  /* 0xfe73b0 */
int d3s_Area = 0;  /* 0xfe73b8 */
int d3s_Fill = 0;  /* 0xfe75ac */

char d3s_MeshFromMap(int *rect, int stride, unsigned char *map, float *origin, float *cell, float *uv,
                     D3SBUF *buf, D3TEXHANDLE *tex, unsigned long color)
{
    D3SVERT *v, *row;
    D3SPOLY *p;
    float *pos, *t;
    float add, x, z, u, w, a, b, c;
    int nx, nz, i, k, n, x0, z0;
    char steep;

    x0 = rect[0];
    nx = rect[1] - x0;
    z0 = rect[2];
    nz = rect[3] - z0;
    row = &buf->verts[buf->nverts];
    pos = &d3s_Verts[d3s_NumVerts * 6];
    t = &d3s_UVs[d3s_NumUVs * 6];
    add = d3d_ZEnabled ? 2.0f : 1.0f;
    z = z0 * cell[2] + origin[2];
    v = row;
    for (k = z0, map += z0 * stride; k <= rect[3]; k++, map += stride) {
        x = x0 * cell[0] + origin[0];
        for (i = rect[0]; i <= rect[1]; i++, v++) {
            pos[1] = (map[i] + add) * cell[1] + add + origin[1];
            pos[0] = x;
            pos[2] = z;
            v->c1 = color;
            v->pos = pos;
            pos += 6;
            d3s_NumVerts++;
            buf->nverts++;
            x += cell[0];
        }
        z += cell[2];
    }
    w = uv[2];
    p = &buf->polys[buf->npolys];
    for (k = nz; k > 0; k--, row += nx + 1) {
        u = uv[0];
        for (i = 0; i < nx; i++) {
            p->v[0] = &row[i];
            a = row[i + nx + 1].y;
            p->v[1] = &row[i + 1];
            b = row[i + 1].y;
            p->v[2] = &row[i + nx + 1];
            c = row[i + nx + 1].y;
            p->uv = t;
            p->tex = tex;
            p->z = -1.0f;
            steep = fabs(a - b) > 25.0f || fabs(a - c) > 25.0f ? 1 : 0;
            p->flags = (steep << 1 ^ p->flags) & 2 ^ p->flags;
            t[0] = u;
            t[1] = w;
            t[2] = u + uv[1];
            t[3] = w;
            t[4] = u;
            t[5] = w + uv[3];
            d3s_NumUVs++;
            buf->npolys++;
            p++;
            p->v[0] = &row[i + 1];
            p->v[1] = &row[i + nx + 2];
            p->v[2] = &row[i + nx + 1];
            p->uv = t + 6;
            p->tex = tex;
            p->z = -1.0f;
            a = row[i + nx + 2].y;
            steep = fabs(a - b) > 25.0f || fabs(a - c) > 25.0f ? 1 : 0;
            p->flags = (steep << 1 ^ p->flags) & 2 ^ p->flags;
            t[6] = u + uv[1];
            t[7] = w;
            t[8] = u + uv[1];
            t[9] = w + uv[3];
            t[10] = u;
            t[11] = w + uv[3];
            d3s_NumUVs++;
            buf->npolys++;
            p++;
            t += 12;
            u += uv[1];
        }
        w += uv[3];
    }
    return 1;
}
