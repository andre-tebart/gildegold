/* 0x5dc3a0, scatters a cannon ball's start velocity (254 B; am.c's shot callback 0x4a6ae0 calls it after
   ai_InitShot for each ball): 0 when the ball is dead. The velocity and the speed are scaled by
   (1.5 r - 1) * speedSpread + 1, then two random deviations are added to the velocity: (2 r - 1) * speed *
   sideSpread along side (am.c passes the vector the balls are set apart along) and ((2 r - 1) * speed *
   otherSpread + 1) along other (a second vector of the planned shot); the result is also the start
   velocity. Returns 1. r is util_RandomF each time. Reasoned: the spread names follow the formulas,
   am.c passes the same spread value for both deviations. @deep body complete, same 79 instructions as 0x5dc3a0..0x5dc49e and every operand accounted for (dead check, speed scale (1.5r-1)*spread+1 on v and speed, side deviation (2r-1)*speed*sideSpread, other deviation (2r-1)*speed*otherSpread+1, vel0.speed copy, the three sums kept as x / y / z with z through the dead arg slot [esp+0x20] like the original, vel0 = sums). Left 18 ins, all operand order / register choice: (1) both deviations: the original does fmul [esi+0xc] (speed) then fmul [esp+0x24|0x2c] (spread), ours the other way round for either source order of the factors; (2) the vel0.speed copy: original mov ecx,[esi+0xc] inside the second deviation and stores it after dother, ours mov eax at the start of it; (3) y / z sums: original fld dother; fadd v; fadd dside, ours fadd dside before v (x matches). Tried: speed*spread and spread*speed, the copy before / after the second deviation, chained assigns with ghidra's 0 / 2 / 1 store order (worse: z leaves the slot) */

int ai_ScatterShot(SHOTREC *shot, float sideSpread, SHOTPOS *side, float otherSpread, SHOTPOS *other,
                   float speedSpread)
{
    float k, x, y, z;
    SHOTPOS dside, dother;

    if (shot->dead != 0)
        return 0;
    k = (util_RandomF() * 1.5f - 1.0f) * speedSpread + 1.0f;
    shot->vel.v.p[0] = k * shot->vel.v.p[0];
    shot->vel.v.p[1] = k * shot->vel.v.p[1];
    shot->vel.v.p[2] = k * shot->vel.v.p[2];
    shot->vel.speed = k * shot->vel.speed;
    k = (util_RandomF() * 2.0f - 1.0f) * shot->vel.speed * sideSpread;
    dside.p[0] = k * side->p[0];
    dside.p[1] = k * side->p[1];
    dside.p[2] = k * side->p[2];
    k = (util_RandomF() * 2.0f - 1.0f) * shot->vel.speed * otherSpread + 1.0f;
    shot->vel0.speed = shot->vel.speed;
    dother.p[0] = k * other->p[0];
    dother.p[1] = k * other->p[1];
    dother.p[2] = k * other->p[2];
    x = dother.p[0] + shot->vel.v.p[0] + dside.p[0];
    shot->vel.v.p[0] = x;
    y = dother.p[1] + shot->vel.v.p[1] + dside.p[1];
    shot->vel.v.p[1] = y;
    z = dother.p[2] + shot->vel.v.p[2] + dside.p[2];
    shot->vel.v.p[2] = z;
    shot->vel0.v.p[0] = x;
    shot->vel0.v.p[1] = y;
    shot->vel0.v.p[2] = z;
    return 1;
}
