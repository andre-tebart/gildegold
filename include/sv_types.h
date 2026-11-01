/* The weather record of sv.c (SVWEATHER, sv_Weather) for the units that declare it without the rest of sv.h:
   types only. sv.h includes it. */
#ifndef SV_TYPES_H
#define SV_TYPES_H

typedef struct SVWEATHER {              /* 0x188 bytes at 0x760488: the day's weather by the hour (sv_Unk525510 rolls it,
                                           ls_WriteEnvironment / ls_ReadEnvironment save it as one record per hour) */
    unsigned clouds[24];                /* +0x000, the clouds of the hour: 0, 0x95 / 0x96, 499 / 500 or 999 / 1000 (the
                                           low bit halves the rain under snow) */
    unsigned storm[24];                 /* +0x060, the thunder of the hour (0 .. 2, 3 = frequent; only at 999 / 1000
                                           clouds out of winter weather) */
    int thunderStart;                   /* +0x0c0, ai_ClockMs of the last thunder */
    int thunderLen;                     /* +0x0c4, its length (8 .. 15) */
    float windX[24];                    /* +0x0c8, the wind direction of the hour: sin of the angle */
    float windY[24];                    /* +0x128, cos of the angle */
} SVWEATHER;

#endif
