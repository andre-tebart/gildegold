/* 0x4183d0, once per frame: the water ambience (up to two looping sounds, d3sndw_Snd[]). Takes the map region
   under the camera (region water kind 1..3) and probes ahead along the view direction for more water regions
   to pick volume and pan of the sounds; both sounds are stopped (label off) when the scene changed or the
   camera / map / sound system is not ready
   @deep B: 143 code / 52 order / 36 regs / 14 fpu hunks, no meaning error: the sampled code hunks are the same instructions with a different callee-saved register assignment (original keeps the zero in ebx, ground in edi, three zero webs ebx/esi/ebp; ours ebp/esi) and the FPU hunks at 0x418515 and the cmp reg,0 vs test forms follow from it; the earlier 4 rounds read the whole 152-line diff (notes/units/d3_snd.md Open work) and tried declaration orders and zero-store orders without change. Register allocation / c2prio work is match-deep's */

/* Terrain map (d3_Terrain), as far as d3sndw_Update uses it */
typedef struct D3MAPREGION {        /* 0x158 bytes */
    char pad000[0x154];
    int water;                      /* 0x154, water type, 0 = none */
} D3MAPREGION;

typedef struct D3MAP {
    char pad0000[0x19e0];
    D3MAPREGION *regions;           /* 0x19e0 */
} D3MAP;

char map_Pick(struct D3FLMAP *map, struct D3VECTOR *pos, char lod, int *x, int *y, float *h);          /* 0x4463b0 */
int map_Region(D3MAP *map, int x, int y);                                              /* 0x446910, index into regions or < 0 */

extern char snd_Unk6f0b60;                  /* 0x6f0b60 */
float d3sndw_Range[] = { 0.0f, 30.0f, 25.0f, 20.0f, 15.0f, 10.0f };  /* 0x5f38a4, per water type, in meters */
unsigned int d3sndw_Vol[] = { 0, 70, 70, 70, 70, 60, 0 };  /* 0x5f38bc, per water type */
extern char d3sndw_Names[6][25];  /* 0x5f38d8, defined below (declaration order = section order) */
char d3sndw_Names[6][25] = { "", "FliesstSchnell_l", "FliesstMittel_l", "FliesstLangsam_l", "Platsch_s", "Platsch_s" };  /* 0x5f38d8, sample per water type; TODO define here once .data is matched */
static unsigned long d3sndw_LastSplash = 0;     /* 0x6311f0 */
static D3SCENE *d3sndw_Scene = 0;               /* 0x6311f4 */
/* d3_Time * 13, spelled so that the volatile is read three times */
#define D3SNDW_TIME ((d3_Time << 3) + (d3_Time << 2) + d3_Time)

typedef struct D3SNDWHIT {
    int type;
    int vol;
    int pan;
} D3SNDWHIT;

void d3sndw_Update(void)
{
    D3SCENE *sc;
    float dist;
    int slot;
    int nfound;
    int k;
    int ground;
    D3VECTOR4 campos;
    D3SNDWHIT found[2];
    D3VECTOR4 probe;
    int gy, gy2;
    char unk[4];
    int gx2, gx;
    D3VECTOR4 dir;
    D3VECTOR4 rot, rel;
    D3MATRIX m;
    float a, sum;
    int i, j, n, idx, type, cnt;
    unsigned long t;
    D3SOUND *s;

    if (!d3_Terrain || !d3sndw_Sin || !d3_Camera || !snd_Unk6f0b60)
        goto off;
    sc = d3_DefaultScene;
    if (!d3sndw_Scene)
        d3sndw_Scene = sc;
    else if (d3sndw_Scene != sc) {
        d3sndw_Scene = sc;
        goto off;
    }
    campos.x = d3_Camera->pos.x;
    campos.y = d3_Camera->pos.y;
    campos.z = d3_Camera->pos.z;
    ground = (int)(d3_Camera->pos.y - d3t_GetMeanHeight(&d3_Camera->pos));
    if (ground > 2000)
        ground = 2000;
    if (!map_Pick(d3_Terrain, (D3VECTOR *)&campos, 1, &gx, &gy, unk))
        return;
    nfound = 0;
    dist = 39.37f;
    found[0].type = 0;
    found[1].pan = 0;
    found[0].vol = 0;
    found[0].pan = 0;
    found[1].type = 0;
    found[1].vol = 0;
    d3_MatTranspose(&d3_Camera->matrix, &m);
    dir.x = d3_ZAxis.x * m._11 + d3_ZAxis.y * m._21 + d3_ZAxis.z * m._31;
    dir.y = 0.0f;
    dir.z = d3_ZAxis.x * m._13 + d3_ZAxis.y * m._23 + d3_ZAxis.z * m._33;
    d3_Normalize(&dir);
    idx = map_Region(d3_Terrain, gx, gy);
    if (idx < 0 || (type = ((D3MAP *)d3_Terrain)->regions[idx].water) > 3 || type == 0) {
        for (i = 0; i < d3sndw_N; i++)
            d3sndw_State[i] = 1;
    } else {
        found[0].type = type;
        found[0].vol = (int)((d3sndw_Vol[type] - d3sndw_Vol[type] * 0.0f / (d3sndw_Range[type] * d3sndw_Range[type]))
                             * (2000 - ground) * 0.0005f);
        nfound = 1;
        found[0].pan = 63;
        for (i = 0; i < d3sndw_N; i++)
            d3sndw_State[i] = 3;
    }
    for (k = 1; k < d3sndw_Range[1] && nfound < 2; k++) {
        probe.x = dir.x * dist + campos.x;
        probe.y = dir.y * dist + campos.y;
        probe.z = dir.z * dist + campos.z;
        for (i = 0; i < d3sndw_N; i++) {
            if (d3sndw_State[i]) {
                if (map_Pick(d3_Terrain, (D3VECTOR *)&probe, 0, &gx2, &gy2, unk)) {
                    idx = map_Region(d3_Terrain, gx2, gy2);
                    if (idx >= 0)
                        d3sndw_Type[i] = ((D3MAP *)d3_Terrain)->regions[idx].water;
                    else
                        d3sndw_Type[i] = 0;
                    if (d3sndw_Type[i]) {
                        if (d3sndw_State[i] == 1)
                            d3sndw_State[i] = 2;
                        else if (d3sndw_State[i] == 2)
                            d3sndw_State[i] = 3;
                    } else
                        d3sndw_State[i] = 1;
                } else
                    d3sndw_State[i] = 0;
            }
            /* turn the probe around the camera to the next ray */
            rel.x = probe.x - campos.x;
            rel.y = probe.y - campos.y;
            rel.z = probe.z - campos.z;
            a = 6.2831855f / d3sndw_N;
            rot.x = rel.x * cos(a) + rel.z * sin(a);
            rot.y = rel.y;
            rot.z = rel.z * cos(a) - rel.x * sin(a);
            probe.x = rot.x + campos.x;
            probe.y = rot.y + campos.y;
            probe.z = rot.z + campos.z;
        }
        for (j = 0; j < d3sndw_N && nfound < 2; j++) {
            if (d3sndw_State[j] == 2 && k <= d3sndw_Range[d3sndw_Type[j]]) {
                /* merge all rays that hit the same water type */
                cnt = 1;
                sum = d3sndw_Sin[j];
                for (n = 0; n < d3sndw_N; n++) {
                    if (n != j && d3sndw_Type[j] == d3sndw_Type[n]) {
                        d3sndw_Type[n] = 0;
                        cnt++;
                        sum += d3sndw_Sin[n];
                    }
                }
                type = d3sndw_Type[j];
                if (type < 4) {
                    found[nfound].type = type;
                    found[nfound].vol = (int)((d3sndw_Vol[type] - d3sndw_Vol[type] * k * k
                                               / (d3sndw_Range[type] * d3sndw_Range[type]))
                                              * (2000 - ground) * 0.0005f);
                    found[nfound].pan = (int)((sum / cnt + 1.0f) * 63.0f);
                    nfound++;
                } else {
                    /* splashes: at most one per 60..120 s */
                    t = D3SNDW_TIME;
                    t -= d3sndw_LastSplash;
                    if (t > 60000 && t < 120000) {
                        sb_PlaySampleI(0, d3sndw_Names[type], 1,
                                      (int)((sum / cnt + 1.0f) * 63.0f),
                                      (int)((d3sndw_Vol[type] - d3sndw_Vol[type] * k * k
                                             / (d3sndw_Range[type] * d3sndw_Range[type]))
                                            * (2000 - ground) * 0.0005f), 0, 0);
                        d3sndw_LastSplash = D3SNDW_TIME;
                    } else if (t > 120000)
                        d3sndw_LastSplash = D3SNDW_TIME;
                }
            }
        }
        dist += 39.37;
    }
    if (nfound == 2 && found[0].type == found[1].type && abs(found[0].pan - found[1].pan) < 31)
        nfound = 1;
    slot = -1;
    switch (nfound) {
    case 0:
        if (d3sndw_Snd[0].snd)
            sb_Stop(d3sndw_Snd[0].snd, 0);
        if (d3sndw_Snd[1].snd)
            sb_Stop(d3sndw_Snd[1].snd, 0);
        return;
    case 1:
        s = d3sndw_GetSound(found[0].type, &slot);
        if (s) {
            if (s->snd) {
                sb_SetVolume(s->snd, found[0].vol);
                sb_SetPanH(s->snd, found[0].pan);
            }
            if (!d3snd_IsPlaying(s))
                d3snd_Play(s, 0, 0, 0);
        }
        if (d3sndw_Snd[slot == 0].snd)
            sb_Stop(d3sndw_Snd[slot == 0].snd, 0);
    case 2:
        slot = -1;
        for (i = 0; i < nfound; i++) {
            s = d3sndw_GetSound(found[i].type, &slot);
            if (s) {
                if (s->snd) {
                    sb_SetVolume(s->snd, found[i].vol);
                    sb_SetPanH(s->snd, found[i].pan);
                }
                if (!d3snd_IsPlaying(s))
                    d3snd_Play(s, 0, 0, 0);
            }
        }
    }
    return;
off:
    if (d3sndw_Snd[0].snd) {
        sb_Stop(d3sndw_Snd[0].snd, 0);
        sb_SetRestartC((char *)d3sndw_Snd[0].snd, 0);
        d3sndw_Snd[0].snd = NULL;
    }
    if (d3sndw_Snd[1].snd) {
        sb_Stop(d3sndw_Snd[1].snd, 0);
        sb_SetRestartC((char *)d3sndw_Snd[1].snd, 0);
        d3sndw_Snd[1].snd = NULL;
    }
}
