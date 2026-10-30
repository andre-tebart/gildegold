/* 0x5dc9d0, moves a cannon ball in slow motion (1318 B; gv.c 0x4edf80 calls it instead of ai_StepShot when the
   effect's +0x14 flag is set): ai_StepShot instruction for instruction with the step ai_ShotHalfStep (1/148 s)
   for ai_ShotStep, in the gravity, the per-mass factor and the position move. @deep the original is instruction for instruction 0x5dc4a0 ai_StepShot with 0x5eee04 (1/148) for 0x5eee00 (normalised disasm diff shows only those three operands); the body is ai_StepShot's with ai_ShotHalfStep and has the same 311-ins distance: the drag product order (area after v*v in ours), the 0x98 vs 0x50 frame, the ebp / ebx loop registers and the second-stage FPU interleave - see ai_StepShot's @deep note; whatever fixes one fixes both */

#define SIGN(v) ((v) > 0.0f ? 1.0f : -1.0f)
#define DRAG(v) (SIGN(v) * shot->area * ai_ShotDrag * ai_AirDensity * (v) * (v) * 0.5f)

int ai_StepShotSlow(SHOTREC *shot)
{
    float fall = ai_ShotHalfStep * ai_Gravity;
    float perMass = ai_ShotHalfStep / shot->mass;
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
        next.x += shot->vel.v.x * ai_InchPerMeter * ai_ShotHalfStep;
        next.y += shot->vel.v.y * ai_InchPerMeter * ai_ShotHalfStep;
        next.z += shot->vel.v.z * ai_InchPerMeter * ai_ShotHalfStep;
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
