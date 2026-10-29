/* 0x43b950, sets the view rectangle, the depth range and the projection scale. @deep Q2: d3_Unkfc2df0 / d3_Unkfc2df4 (0xfc2df0, 0xfc2df4) are write-only - stored from the last two int parameters and passed back unchanged by d3_SetFogRange and d3_SetFog, nothing else reads them, no initial value and no caller varies them; d3_Unkfe73a4 (0xfe73a4 = (int)(65536.0f * (h + y))) and d3_Unkfc2ddc (0xfc2ddc, set to 1.0f) have no reader at all: the Ghidra exports 43a610 / 43a6f0 / 43b950 / 43bb70 / 43bc20 / 43c240 are the only references to these addresses, so no role name can be evidenced. Everything else in the body is byte and named (d3_Unk4391d0 -> d3_SetupFrustum, d3_Unk63345c -> d3_D3DActive this session). */

void d3_SetViewport(float w, float h, float x, float y, float zn, float zf, float scale, int a, int b)
{
    int left, top, right, bottom;
    float hw, hh;

    if (d3_ViewW != w || d3_ViewH != h || d3_Viewport.left != x || d3_Viewport.top != y || d3_ScaleX != scale
        || d3_ZNear != zn || d3_ZFar != zf || d3_Unkfc2df4 != a || d3_Unkfc2df0 != b) {
        if (d3_D3DActive)
            d3d_SetViewport(w, h, x, y, zn, zf);
        d3_HalfW = hw = w * 0.5f;
        d3_Unkfc2df4 = a;
        d3_Unkfc2df0 = b;
        d3_ViewW = w;
        d3_CenterX = hw + x;
        d3_ViewH = h;
        d3_Unkfe73a4 = (int)(65536.0f * (h + y));
        d3_HalfH = hh = h * 0.5f;
        d3_ZNear = zn;
        d3_ZFar = zf;
        d3_CenterY = hh + y;
        d3_InvZFar = 1.0f / zf;
        left = (int)(x + 0.5f);
        top = (int)(y + 0.5f);
        right = (int)(w + x);
        bottom = (int)(h + y);
        if (left != d3_Viewport.left || right != d3_Viewport.right || top != d3_Viewport.top || bottom != d3_Viewport.bottom) {
            d3_Viewport.left = left;
            d3_Viewport.top = top;
            d3_Viewport.right = right;
            d3_Viewport.bottom = bottom;
            dd_Cleared = 0;
            dd_ZCleared = 0;
        }
        d3_ScaleY = scale * -1.0f;
        d3_ViewX = x;
        d3_ViewY = y;
        d3_Unkfc2ddc = 1.0f;
        d3_ScaleX = scale;
        d3_SetupFrustum();
        d3_UpdateAll(0);
    }
}
