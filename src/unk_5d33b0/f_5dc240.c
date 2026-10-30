/* 0x5dc240, builds a cannon ball in flight (341 B; am.c's shot callback 0x4a6ae0 calls it per ball): the
   ball's sphere (ai_SphereVolume of density and size: mass = volume * density, am.c passes 7874, the density
   of iron) gives the speed charge / mass * 0.1; the velocity is the barrel direction dir times it (also kept
   as the start velocity), the position and the start position are pos, the wind is ai_DefaultEffect with
   its scale clamped to 0 .. 60 (ai_WindGain) and the vector scaled by it; the surface and the mass, alive 1,
   the clock d3_Time (ms) and the dead flag 0 complete the record. Returns 1. @deep body complete and read against 0x5dc240..0x5dc395 (all 113 ins accounted for: GetDefaultEffect, SphereVolume(density, size), dir copy, speed = charge / reach * 0.1, vel scale, vel0 copy, wind copy + clamp 0..60 (MAX(MIN)) + scale, pos / pos0, area / mass, alive 1, d3_Time, dead 0, return 1). Left is VC6 placement: (1) our compiler sinks the call ai_GetDefaultEffect (same TU, known side-effect free) to the wind copy at +0x79 where the original calls it first and keeps the pointer in edx across ai_SphereVolume (interprocedural regs); this shifts the whole +0x09..+0xe8 block (76 ins); (2) the vel scale: the original reloads the speed (fld [ecx+0xc]) after each store, ours keeps it on the FPU (fld st(0)); (3) the last hunk loads surface / reach into swapped registers. Tried: the wind pointer initialised at its declaration, compound *=, SHOTPOS-typed velocity (the dir copy as a struct assign), none moved the call */

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

int ai_InitShot(SHOTREC *shot, SHOTPOS *pos, SHOTPOS *dir, float charge, float size, float density)
{
    EFFECTREC *wind = (EFFECTREC *)ai_GetDefaultEffect();
    SPHEREVOLUME ball;

    ai_SphereVolume(density, size, (float *)&ball);
    shot->vel.v = *dir;
    shot->vel.speed = charge / ball.reach * 0.1f;
    shot->vel.v.p[0] = shot->vel.speed * shot->vel.v.p[0];
    shot->vel.v.p[1] = shot->vel.speed * shot->vel.v.p[1];
    shot->vel.v.p[2] = shot->vel.speed * shot->vel.v.p[2];
    shot->vel0 = shot->vel;
    shot->wind = *wind;
    shot->wind.scale = MAX(MIN(shot->wind.scale, ai_WindGain), 0.0f);
    shot->wind.pos[0] = shot->wind.scale * shot->wind.pos[0];
    shot->wind.pos[1] = shot->wind.scale * shot->wind.pos[1];
    shot->wind.pos[2] = shot->wind.scale * shot->wind.pos[2];
    shot->pos = *pos;
    shot->pos0 = *pos;
    shot->area = ball.surface;
    shot->mass = ball.reach;
    shot->alive = 1;
    shot->time = d3_Time;
    shot->dead = 0;
    return 1;
}
