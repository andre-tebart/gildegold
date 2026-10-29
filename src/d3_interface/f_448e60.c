/* 0x448e60, the texture coordinates at a screen point: of the polygons of `obj` textured with `tex` it takes the
   four vertices nearest to the screen corners (d3_CornerCB), picks three of them by their (normalized) uv and
   intersects the view ray through (sx, sy) with the plane of the triangle, once for each of the two triangles
   of the quad; returns 1 and the uv when the ray hits one
   @deep B: fixed the real meaning error on the way (float vs double literal types: the original's constants 0x5eb780 = 1.0f, 0x5eb788 = 0.0f, 0x5eb948 = 0.33333334f, while 0x5eb8a8 stays a double 1.0 for r+s<=1.0 and the outv fsubr; 15 QWORD constant uses removed, unitdiff 1472->1451 bytes). Remaining 10 diffclass hunks: the frame 0xc4 vs 0x108 (every esp-relative operand shifted by 0x44; the original keeps every float temp in its own stack slot, ours reuses px/bx/best_c slots for s/r/ic) plus FPU scheduling (fxch order, faddp pairing); read all hunks, no missing statement/constant/field/condition left */

char d3_ScreenToUV(float sx, float sy, D3OBJECT *obj, void *tex, float *outu, float *outv)
{
    D3CORNERCTX c;
    float mn[2];
    float best_a, best_b, best_c, f;
    float px, py, pz, ax, ay, az, bx, by, bz, nx, ny, nz, rx, ry, den, t, hx, hy, s, m, r;
    int i, j, ia, ib, ic, id;

    if (tex) {
        c.tex = tex;
        c.pt[0].x = dd_Width - 1;
        c.pt[0].y = dd_Height - 1;
        c.found = 0;
        c.pt[1].x = 0;
        c.pt[1].y = dd_Height - 1;
        c.pt[2].x = dd_Width - 1;
        c.pt[2].y = 0;
        c.pt[3].x = 0;
        c.pt[3].y = 0;
        d3_ForEachObject(d3_DefaultScene, obj, 0x40, (D3OBJECTCB)d3_CornerCB, &c);
        if (c.found) {
            mn[0] = 1e10f;
            mn[1] = 1e10f;
            for (i = 0; i < 4; i++) {
                if (c.uv[i][0] <= mn[0])
                    mn[0] = c.uv[i][0];
                if (c.uv[i][1] <= mn[1])
                    mn[1] = c.uv[i][1];
            }
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 2; j++)
                    c.uv[i][j] = c.uv[i][j] - mn[j];
                c.uv[i][1] = 1.0f - c.uv[i][1];
            }
            ia = -1;
            ib = -1;
            ic = -1;
            best_c = 1e10f;
            best_b = -1e10f;
            best_a = -1e10f;
            for (i = 0; i < 4; i++) {
                f = (float)fabs(c.uv[i][1]) - (float)fabs(c.uv[i][0]);
                if (best_a < f) {
                    ia = i;
                    best_a = f;
                }
                f = (float)fabs(c.uv[i][0]) + (float)fabs(c.uv[i][1]);
                if (best_b < f) {
                    ib = i;
                    best_b = f;
                }
                if (f < best_c) {
                    ic = i;
                    best_c = f;
                }
            }
            if (ia != -1 && ib != -1 && ic != -1 && ia != ib && ia != ic && ib != ic) {
                for (id = 0; id == ib || id == ia || id == ic; id++)
                    ;
                px = c.v[ia].x;
                py = c.v[ia].y;
                pz = c.v[ia].z;
                ax = px - c.v[ib].x;
                ay = py - c.v[ib].y;
                az = pz - c.v[ib].z;
                bx = px - c.v[ic].x;
                by = py - c.v[ic].y;
                bz = pz - c.v[ic].z;
                nx = bz * ay - by * az;
                ny = bx * az - bz * ax;
                nz = by * ax - bx * ay;
                sx = sx - d3_CenterX;
                ry = -(sy - d3_CenterY);
                den = sx * nx + ry * ny + d3_ScaleX * nz;
                if (1e-07 < fabs(den)) {
                    t = ((nz + ny + nx) * 0.0f + nx * px + ny * py + nz * pz) / den;
                    hx = px - t * sx;
                    hy = py - t * ry;
                    s = (hx * ay - hy * ax) / (bx * ay - by * ax);
                    m = -s;
                    r = ((bx * m + hx) / ax + (by * m + hy) / ay + (m * bz + (pz - t * d3_ScaleX)) / az) * 0.33333334f;
                    if (r + s <= 1.0) {
                        if (0.0f <= r && r < 1.0f && 0.0f <= s && s < 1.0f) {
                            *outu = ((float)fabs(c.uv[ib][0]) - (float)fabs(c.uv[ia][0])) * r + (float)fabs(c.uv[ia][0]);
                            *outv = 1.0 - (((float)fabs(c.uv[ic][1]) - (float)fabs(c.uv[ia][1])) * s + (float)fabs(c.uv[ia][1]));
                            return 1;
                        }
                    } else {
                        px = c.v[id].x;
                        py = c.v[id].y;
                        pz = c.v[id].z;
                        ax = px - c.v[ib].x;
                        ay = py - c.v[ib].y;
                        az = pz - c.v[ib].z;
                        bx = px - c.v[ic].x;
                        by = py - c.v[ic].y;
                        bz = pz - c.v[ic].z;
                        nx = az * by - ay * bz;
                        ny = ax * bz - az * bx;
                        nz = ay * bx - ax * by;
                        den = sx * nx + ry * ny + d3_ScaleX * nz;
                        if (1e-07 < fabs(den)) {
                            t = ((nz + ny + nx) * 0.0f + nx * px + ny * py + nz * pz) / den;
                            hx = px - t * sx;
                            hy = py - t * ry;
                            s = (hx * by - hy * bx) / (ax * by - ay * bx);
                            m = -s;
                            r = ((ax * m + hx) / bx + (ay * m + hy) / by + (m * az + (pz - t * d3_ScaleX)) / bz) * 0.33333334f;
                            if (0.0f <= r && r < 1.0f && 0.0f <= s && s < 1.0f) {
                                *outu = (1.0f - r) * ((float)fabs(c.uv[ib][0]) - (float)fabs(c.uv[ia][0])) + (float)fabs(c.uv[ia][0]);
                                *outv = 1.0 - ((1.0f - s) * ((float)fabs(c.uv[ic][1]) - (float)fabs(c.uv[ia][1])) + (float)fabs(c.uv[ia][1]));
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}
