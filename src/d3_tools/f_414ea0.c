/* 0x414ea0. NONMATCHING: 33 bytes, the integer loads of d3_Viewport.right/left for hw are scheduled
   two FPU instructions later than in the original; everything else is identical. The locals x and y
   keep the x update on the FPU stack and fix the sy2 operand order; (snow->windz) fixes where the
   move.z store is scheduled. Initializing fall at the top puts it below wind on the stack. */

void d3t_UpdateSnow(D3TSNOW *snow, float dt)
{
    D3VECTOR4 d, o, move, wind, fall;
    D3MATRIX m;
    float ex, ey, ez, wx, wz;
    float hw, hh, size, cx, cy, f, s, x, y;
    float gy;                        /* -1.0f: a literal -1.0f * x is folded, the original multiplies */
    D3TSNOWFLAKE *flake;
    int i;

    fall.x = 0.0f;
    fall.y = -1.0f;
    fall.z = 0.0f;
    d.x = snow->campos.x - d3_Camera->rotation.x;
    d.y = snow->campos.y - d3_Camera->rotation.y;
    d.z = snow->campos.z - d3_Camera->rotation.z;
    d3_MatFromAngles(&d, &m);
    snow->campos.x = d3_Camera->rotation.x;
    snow->campos.y = d3_Camera->rotation.y;
    snow->campos.z = d3_Camera->rotation.z;
    o.x = 0.0f * m._11 + 0.0f * m._21 + 1.9f * m._31;
    o.y = 0.0f * m._12 + 0.0f * m._22 + 1.9f * m._32;

    ex = snow->cam04c.x - d3_Camera->pos.x;
    ey = snow->cam04c.y - d3_Camera->pos.y;
    ez = snow->cam04c.z - d3_Camera->pos.z;
    move.x = ex * d3_Camera->matrix._11 + ey * d3_Camera->matrix._21 + ez * d3_Camera->matrix._31;
    move.y = ex * d3_Camera->matrix._12 + ey * d3_Camera->matrix._22 + ez * d3_Camera->matrix._32;
    move.z = ex * d3_Camera->matrix._13 + ey * d3_Camera->matrix._23 + ez * d3_Camera->matrix._33;
    move.x *= 0.0025f;
    move.y *= 0.0025f;
    move.z *= 0.0025f;
    snow->cam04c.x = d3_Camera->pos.x;
    snow->cam04c.y = d3_Camera->pos.y;
    snow->cam04c.z = d3_Camera->pos.z;

    wx = snow->windx;
    wz = (snow->windz);
    wind.x = wx * d3_Camera->matrix._11 + 0.0f * d3_Camera->matrix._21 + wz * d3_Camera->matrix._31;
    wind.y = wx * d3_Camera->matrix._12 + 0.0f * d3_Camera->matrix._22 + wz * d3_Camera->matrix._32;
    wind.z = wx * d3_Camera->matrix._13 + 0.0f * d3_Camera->matrix._23 + wz * d3_Camera->matrix._33;
    gy = -1.0f;
    fall.x = 0.0f * d3_Camera->matrix._11 + gy * d3_Camera->matrix._21 + 0.0f * d3_Camera->matrix._31;
    fall.y = 0.0f * d3_Camera->matrix._12 + gy * d3_Camera->matrix._22 + 0.0f * d3_Camera->matrix._32;
    fall.z = 0.0f * d3_Camera->matrix._13 + gy * d3_Camera->matrix._23 + 0.0f * d3_Camera->matrix._33;

    hw = (d3_Viewport.right - d3_Viewport.left) >> 1;
    hh = (d3_Viewport.bottom - d3_Viewport.top) >> 1;
    size = hw > hh ? hw : hh;
    cx = d3_Viewport.left + hw;
    cy = d3_Viewport.top + hh;

    flake = snow->flakes;
    for (i = 0; i < snow->count; flake++, i++) {
        x = flake->pos.x = (wind.x * flake->windScale + fall.x * flake->fallScale) * dt + flake->pos.x + move.x + o.x;
        while (x < -1.0)
            flake->pos.x = x += 2.0;
        while (flake->pos.x >= 1.0)
            flake->pos.x -= 2.0;

        flake->pos.y = (wind.y * flake->windScale + fall.y * flake->fallScale) * dt + flake->pos.y + move.y + o.y;
        while (flake->pos.y < -1.0)
            flake->pos.y += 2.0;
        while (flake->pos.y >= 1.0)
            flake->pos.y -= 2.0;

        flake->pos.z += (wind.z * flake->windScale + fall.z * flake->fallScale) * dt + move.z * 2.0f;
        while (flake->pos.z < -1.0f)
            flake->pos.z += 2.0f;
        while (flake->pos.z >= 1.0f)
            flake->pos.z -= 2.0f;

        f = size * 3.0f / (flake->pos.z * 2.0f + 3.0f);
        flake->sx1 = flake->pos.x * f + cx;
        flake->sy1 = y = cy - flake->pos.y * f;
        s = ((1.0f - flake->pos.z) * 13.5f + 1.0f) * flake->pos.w;
        flake->sx2 = flake->sx1 + s;
        flake->sy2 = y + s;
    }
}
