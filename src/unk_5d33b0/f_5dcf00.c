/* 0x5dcf00, the ground test of a cannon ball's flight step (390 B; ai_StepShot / ai_StepShotSlow): the cell
   of to (d3_GetHeightAt into npx / npz). When it is at most one cell from the cell px / pz of from, the
   step hits when to lies less than ai_ShotHitHeight above the ground there. A longer step is walked from
   from to to in steps of 1 / (the cell distance): the first point less than ai_ShotHitHeight above the
   ground is the hit, to is moved there. 1 on a hit, else 0. @deep body complete, size 390 = 390, 134 = 134 ins, every statement in the original's order (end cell, abs cell distance, <= 1 short test, step = 1 / sqrt, p = from field by field, d = to - from then *= step, the walk with d3_GetHeightAt into cx / cz, the hit copy, t += step while t < 1). D3VECTOR4 locals give the original's 0x24 frame. Left 44 ins: (1) the commutative fadds of the walk: the original does fld p.y; fadd d.y / fld p.z; fadd d.z / fld t; fadd step, ours loads the other operand first (same for +=, p = p + d); (2) the from->z copy is scheduled after the subtractions in the original, before them in ours; (3) the lea / push order of the walk's GetHeightAt args (ecx / edx / eax rotation) and the hit copy's register rotation (regs eax->edx ecx->eax edx->ecx); (4) slots t / step swapped ([esp+0x40] / [esp+0x50]) */

int ai_ShotHitsGround(D3SM *map, D3VECTOR *from, D3VECTOR *to, int px, int pz, int *npx, int *npz)
{
    float height, t, step;
    int dx, dz, cx, cz;
    D3VECTOR4 p, d;                     /* the engine's padded vectors (the original's frame holds 16 bytes each) */

    d3_GetHeightAt(map, to, npx, npz, &height);
    dx = abs(*npx - px);
    dz = abs(*npz - pz);
    if (dx <= 1 && dz <= 1) {
        if (ai_ShotHitHeight + height > to->y)
            return 1;
        return 0;
    }
    t = 0;
    step = 1.0f / (float)sqrt((float)(dz * dz) + (float)(dx * dx));
    p.x = from->x;
    p.y = from->y;
    p.z = from->z;
    d.x = to->x - from->x;
    d.y = to->y - from->y;
    d.z = to->z - from->z;
    d.x *= step;
    d.y *= step;
    d.z *= step;
    do {
        p.x += d.x;
        p.y += d.y;
        p.z += d.z;
        d3_GetHeightAt(map, (D3VECTOR *)&p, &cx, &cz, &height);
        if (ai_ShotHitHeight + height > p.y) {
            to->x = p.x;
            to->y = p.y;
            to->z = p.z;
            return 1;
        }
        t += step;
    } while (t < 1.0f);
    return 0;
}
