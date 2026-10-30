/* 0x5dc4a0, moves a cannon ball (1318 B; gv.c 0x4edf80 calls it every frame): 0 unless the ball is in flight
   (state 1). Off the height map (d3_GetHeightAt fails) the state becomes 3 and the energy 0. Otherwise one
   step of ai_ShotStep (1/74 s) is made per millisecond since the last one: the force on each axis is the
   drag 0.5 * ai_AirDensity * ai_ShotDrag * area * v^2 (signed with v) plus the same drag of the wind, and
   gravity pulls y; the velocity takes Heun's average of the change at the old and the predicted velocity,
   the position moves by velocity * step in inches. When ai_ShotHitsGround finds the move from the old to the
   new position hits the ground and the ball made more than 50 steps, it lands there: state 2, the energy
   0.5 * mass * v^2, returns 0. Returns 1 while it flies. @deep body reconstructed from the whole of 0x5dc4a0..0x5dc9c5 (354 ins): fall = step * g and perMass = step / mass before the state check, init_SceneHeightMaps[0], ticks = d3_Time - time, d3_GetHeightAt fail -> state 3 / energy 0, the three wind drags, the per-ms loop (v copy, drag x stored / y z on the FPU, a = (d + wind) * perMass (- fall on y), predicted v, second drags, next = pos, time++ / steps++, b, vel += (b + a) * 0.5, next += vel * 39.37 * step, ai_ShotHitsGround with the cells px / pz -> npx / npz, steps > 50 -> pos = next, state 2, energy 0.5 m v^2, return 0), return 1. dist 311 / 354 (fpu 150, code 285). Left: (1) the drag product order: the original emits sign, area, Cd, rho, v, v, 0.5 (wind x / z: area, w, w, sign, Cd, rho, 0.5), our VC6 moves the area factor ([esi+0x50]) after v*v for every grouping tried (plain chain, (area*Cd), (sign*area), (v*v)); (2) the frame: original sub esp,0x98, ours 0x50 - the original keeps more stage temps in memory (a / b / pos delta), which shifts every esp slot and shows as code; (3) loop regs ebp = ticks / ebx = i, ours swapped; (4) the second-stage sums are interleaved differently on the FPU. Tried: D3VECTOR v / next locals (struct copies through a pointer temp, worse: the original copies field by field), per-field next copy (kept) */

#define SIGN(v) ((v) > 0.0f ? 1.0f : -1.0f)
#define DRAG(v) (SIGN(v) * shot->area * ai_ShotDrag * ai_AirDensity * (v) * (v) * 0.5f)

int ai_StepShot(SHOTREC *shot)
{
    float fall = ai_ShotStep * ai_Gravity;
    float perMass = ai_ShotStep / shot->mass;
    D3SM *map = (D3SM *)init_SceneHeightMaps[0];
    int ticks, i, px, pz, npx, npz;
    float height;
    float windX, windY, windZ;
    float vx, vy, vz, dx, dy, dz, ax, ay, az, bx, by, bz;
    D3VECTOR next;

    if (shot->state != 1)
        return 0;
    ticks = d3_Time - shot->time;
    if (!d3_GetHeightAt(map, &shot->pos, &px, &pz, &height)) {
        shot->state = 3;
        shot->energy = 0;
        return 0;
    }
    windX = DRAG(shot->wind.pos[0]);
    windY = DRAG(shot->wind.pos[1]);
    windZ = DRAG(shot->wind.pos[2]);
    for (i = 0; i < ticks; i++) {
        vx = shot->vel.v.x;
        vy = shot->vel.v.y;
        vz = shot->vel.v.z;
        dx = DRAG(vx);
        dy = DRAG(vy);
        dz = DRAG(vz);
        ax = (dx + windX) * perMass;
        ay = (dy + windY) * perMass - fall;
        az = (dz + windZ) * perMass;
        vx = ax + vx;
        vy = ay + vy;
        vz = az + vz;
        dx = DRAG(vx);
        dy = DRAG(vy);
        dz = DRAG(vz);
        next.x = shot->pos.x;
        next.y = shot->pos.y;
        next.z = shot->pos.z;
        shot->time++;
        shot->steps++;
        bx = (dx + windX) * perMass;
        by = (dy + windY) * perMass - fall;
        bz = (dz + windZ) * perMass;
        shot->vel.v.x += (bx + ax) * 0.5f;
        shot->vel.v.y += (by + ay) * 0.5f;
        shot->vel.v.z += (bz + az) * 0.5f;
        next.x += shot->vel.v.x * ai_InchPerMeter * ai_ShotStep;
        next.y += shot->vel.v.y * ai_InchPerMeter * ai_ShotStep;
        next.z += shot->vel.v.z * ai_InchPerMeter * ai_ShotStep;
        if (ai_ShotHitsGround(map, &shot->pos, &next, px, pz, &npx, &npz) && shot->steps > 50) {
            shot->pos = next;
            shot->state = 2;
            shot->energy = (shot->vel.v.x * shot->vel.v.x + shot->vel.v.y * shot->vel.v.y
                + shot->vel.v.z * shot->vel.v.z) * shot->mass * 0.5f;
            return 0;
        }
        shot->pos = next;
        px = npx;
        pz = npz;
    }
    return 1;
}
