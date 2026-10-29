/* d3_sky.c -- sky dome drawn as a fan of screen-space quads (allocation tags "d3_sky:..."). Names are ours,
   except where the D3DCHECK texts of the original give them (s->buffer, s->num_buffer, s->indices,
   s->num_indices, l->t->surftext). */
#ifndef D3_SKY_H
#define D3_SKY_H

#define D3SKY_ROWS 6
#define D3SKY_COLS 8

/* the six values d3sky_InitColors stores in d3sky_Colors: 0 is not one of the shades, 1 .. 5 run
   edge (rose), inner (light gray), centre (white) and back; nothing reads the array in the drafted code */
#define D3SKY_COLOR0 0x40
#define D3SKY_COLOR_EDGE 0xe08080
#define D3SKY_COLOR_INNER 0xe0e0e0
#define D3SKY_COLOR_CENTER 0xffffff

typedef struct D3SKYVERTEX {        /* 0x20 bytes, a D3TLVERTEX whose specular colour holds flags */
    float sx, sy, sz, rhw;
    unsigned long color;            /* 0x10 */
    unsigned char spec0, spec1;
    unsigned char visible;          /* 0x16, above the horizon */
    unsigned char fog;              /* 0x17 */
    float tu, tv;                   /* 0x18 */
} D3SKYVERTEX;

typedef struct D3SKYLAYER {         /* 0x2c bytes */
    float scale;                    /* 0x00, u repeats per turn */
    float offset;                   /* 0x04, u scroll position 0..1 */
    float speed;                    /* 0x08, u scroll per ms */
    float fade;                     /* 0x0c, 0..1 from alpha0 to alpha1 */
    float fadespeed;                /* 0x10, per ms */
    float voffset;                  /* 0x14 */
    float vscale;                   /* 0x18 */
    D3TEXHANDLE *t;                 /* 0x1c */
    D3TEXFLAGS flags;               /* 0x20, b[0] initial alpha, b[2] 1: modulate with the sky colour alpha, 2: ? */
    unsigned char alpha0;           /* 0x24 */
    unsigned char alpha1;           /* 0x25 */
    unsigned char alpha;            /* 0x26 */
    char pad27;
    struct D3SKYLAYER *next;        /* 0x28 */
} D3SKYLAYER;

typedef struct D3SKY {              /* 0xacc bytes */
    D3SKYVERTEX buffer[D3SKY_ROWS * D3SKY_COLS];    /* 0x000 */
    D3VECTOR4 dir[D3SKY_ROWS * D3SKY_COLS];         /* 0x600, view direction per vertex; w = its angle */
    unsigned short indices[(D3SKY_ROWS - 1) * (D3SKY_COLS - 1) * 6];   /* 0x900 */
    D3COLOR color;                  /* 0xaa4 */
    int num_buffer;                 /* 0xaa8, -1: to be rebuilt */
    int num_indices;                /* 0xaac */
    float rot;                      /* 0xab0, camera heading */
    float horizon;                  /* 0xab4 */
    float fogstart;                 /* 0xab8 */
    float fogscale;                 /* 0xabc */
    D3SKYLAYER *layers;             /* 0xac0 */
    unsigned long lasttime;         /* 0xac4 */
    float dt;                       /* 0xac8 */
} D3SKY;

void d3sky_Init(struct D3SKY *s, char build);                          /* 0x45d690, places the vertices on the
                                                                   screen; build also for a new sky */

#endif
