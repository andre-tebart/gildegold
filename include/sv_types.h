/* The weather record of sv.c (SVWEATHER, sv_Weather), the rubber band (SVBAND), the book (SVBOOK) and the speech bubble record (SVBUBBLE) for the units that declare it without the rest of sv.h:
   types only. sv.h includes it. */
#ifndef SV_TYPES_H
#define SV_TYPES_H

typedef struct SVWEATHER {              /* 0x188 bytes at 0x760488: the day's weather by the hour (sv_SetupWeather rolls it,
                                           ls_WriteEnvironment / ls_ReadEnvironment save it as one record per hour) */
    unsigned clouds[24];                /* +0x000, the clouds of the hour: 0, 0x95 / 0x96, 499 / 500 or 999 / 1000 (the
                                           low bit halves the rain under snow) */
    unsigned storm[24];                 /* +0x060, the thunder of the hour (0 .. 2, 3 = frequent; only at 999 / 1000
                                           clouds out of winter weather) */
    int thunderStart;                   /* +0x0c0, d3_Time of the last thunder */
    int thunderLen;                     /* +0x0c4, its length (8 .. 15) */
    float windX[24];                    /* +0x0c8, the wind direction of the hour: sin of the angle */
    float windY[24];                    /* +0x128, cos of the angle */
} SVWEATHER;

typedef struct SVBAND {                 /* 0x10 bytes at 0x760444: the rubber band selection (sv_StartBand / sv_UpdateBand) */
    int x0, y0;                         /* +0x00, the corner where the band started (the mouse at the start, clamped to d3_Viewport) */
    int x1, y1;                         /* +0x08, the corner at the mouse (not ordered against x0 / y0) */
} SVBAND;

typedef struct SVBOOK {                 /* 0x274 bytes, an open book ("f3_sv:OpenBook", sv_OpenBook) */
    struct D3OBJECT *obj;               /* +0x000, the book model shown */
    struct D3OBJECT *hidden;            /* +0x004, the model found by name, hidden while its copy obj stands at the
                                           scene object (0 = obj is the model itself) */
    char pad008[0x220];
    int views[16];                      /* +0x228, the page views (d2_LoadView), npages of them */
    int btnNext;                        /* +0x268, the forward button object (-1 = none) */
    int btnPrev;                        /* +0x26c, the backward button object (-1 = none) */
    char page;                          /* +0x270, the first page shown (the left of the open pair) */
    char npages;                        /* +0x271 */
    unsigned char zbuffer;              /* +0x272, d3d_ZEnabled at the opening (restored by the close) */
    char pad273;
} SVBOOK;

typedef struct SVBUBBLE {               /* 0x10c bytes, a speech bubble over a character (sv_Bubbles) */
    int text;                           /* +0x000, the d2_Strings id shown when name is empty */
    int time;                           /* +0x004, d3_Time when it was shown (freed 300 ms later) */
    struct CHARACTER *ch;               /* +0x008, the character it belongs to, 0 = free */
    char name[0x100];                   /* +0x00c, its own text, "" = the d2_Strings line */
} SVBUBBLE;

#endif
