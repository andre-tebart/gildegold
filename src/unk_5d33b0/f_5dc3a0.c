/* 0x5dc3a0, scatters a cannon ball's start velocity (254 B; am.c's shot callback 0x4a6ae0 calls it after
   ai_InitShot for each ball): 0 once the ball made a flight step. The start velocity and speed are scaled by
   (1.5 r - 1) * speedSpread + 1, then two random deviations are added to the velocity: (2 r - 1) * speed *
   sideSpread along side (am.c passes the vector the balls are set apart along) and ((2 r - 1) * speed *
   otherSpread + 1) along other (a second vector of the planned shot); the result is also the current
   velocity. Returns 1. r is util_RandomF each time. Reasoned: the spread names follow the formulas,
   am.c passes the same spread value for both deviations. @deep body complete, same 79 instructions as 0x5dc3a0..0x5dc49e and every operand accounted for (steps check, speed scale (1.5r-1)*spread+1 on v and speed, side deviation (2r-1)*speed*sideSpread, other deviation (2r-1)*speed*otherSpread+1, vel.speed copy, the three sums kept as x / y / z with z through the dead arg slot [esp+0x20] like the original, vel = sums). Left 18 ins, all operand order / register choice: (1) both deviations: the original does fmul [esi+0xc] (speed) then fmul [esp+0x24|0x2c] (spread), ours the other way round for either source order of the factors; (2) the vel.speed copy: original mov ecx,[esi+0xc] inside the second deviation and stores it after dother, ours mov eax at the start of it; (3) y / z sums: original fld dother; fadd v; fadd dside, ours fadd dside before v (x matches). Tried: speed*spread and spread*speed, the copy before / after the second deviation, chained assigns with ghidra's 0 / 2 / 1 store order (worse: z leaves the slot) */

int ai_ScatterShot(SHOTREC *shot, float sideSpread, D3VECTOR *side, float otherSpread, D3VECTOR *other,
                   float speedSpread)
{
    float k, x, y, z;
    D3VECTOR dside, dother;

    if (shot->steps != 0)
        return 0;
    k = (util_RandomF() * 1.5f - 1.0f) * speedSpread + 1.0f;
    shot->velStart.v.x = k * shot->velStart.v.x;
    shot->velStart.v.y = k * shot->velStart.v.y;
    shot->velStart.v.z = k * shot->velStart.v.z;
    shot->velStart.speed = k * shot->velStart.speed;
    k = (util_RandomF() * 2.0f - 1.0f) * shot->velStart.speed * sideSpread;
    dside.x = k * side->x;
    dside.y = k * side->y;
    dside.z = k * side->z;
    k = (util_RandomF() * 2.0f - 1.0f) * shot->velStart.speed * otherSpread + 1.0f;
    shot->vel.speed = shot->velStart.speed;
    dother.x = k * other->x;
    dother.y = k * other->y;
    dother.z = k * other->z;
    x = dother.x + shot->velStart.v.x + dside.x;
    shot->velStart.v.x = x;
    y = dother.y + shot->velStart.v.y + dside.y;
    shot->velStart.v.y = y;
    z = dother.z + shot->velStart.v.z + dside.z;
    shot->velStart.v.z = z;
    shot->vel.v.x = x;
    shot->vel.v.y = y;
    shot->vel.v.z = z;
    return 1;
}
