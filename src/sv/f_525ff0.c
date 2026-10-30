/* 0x525ff0, the mouse at the screen border starts a camera move to one of the 5 preset views (+0x5c / 0x90 the default, +0xb4 / +0xcc left / right of the window, +0xe4 / +0xfc above / below, each position + direction
   triple): the border state 0x6f22cc / 0x6f22d0 (-1, 0, 1) steps by the mouse position against the window 0x6310b0 / 0x6310b4 .. 0x5f3238 / 0x5f323c; the view is applied (0x75fb80 / 0x75fb90, 0x433b10, 0x411860 (cam,
   pos, dir, 0x623814, 8)) unless the camera is already within 0.2 of it (0x453e80) */

void sv_Unk525ff0(void)
{
    int top, bot;
    int pos[3], dir[3];
    int zero[4];
    int my, off;

    bot = di_MaxY - 1;
    top = di_MinY + 1;
    zero[0] = zero[1] = zero[2] = zero[3] = 0;
    if (ai_Unk6f2378 != 0) {
        if (dm_L.x < di_MinX + 1)
            return;
        if (di_MaxX - 1 < dm_L.x)
            return;
        if (dm_L.y < top)
            return;
        if (bot < dm_L.y)
            return;
        ai_Unk6f2378 = 0;
        return;
    }
    my = dm_L.y;
    if (top < my && my < bot) {
        if (dm_L.x < di_MinX + 1) {
            if (ai_Unk6f22cc < 0)
                return;
            ai_Unk6f22cc--;
        } else {
            if (dm_L.x <= di_MaxX - 1)
                return;
            if (0 < ai_Unk6f22cc)
                return;
            ai_Unk6f22cc++;
        }
        ai_Unk6f22d0 = 0;
        if (ai_Unk6f22cc == -1) {
            off = 0xe4;
            if (d3_VecEqual(ai_Unkfc2dec + off, zero, 0.2f) != 0)
                return;
            goto load;
        }
        if (ai_Unk6f22cc != 0) {
            if (ai_Unk6f22cc == 1) {
                off = 0xfc;
                if (d3_VecEqual(ai_Unkfc2dec + off, zero, 0.2f) != 0)
                    return;
                goto load;
            }
            goto apply;
        }
    } else {
        if (my < top) {
            if (ai_Unk6f22d0 < 0)
                return;
            ai_Unk6f22d0--;
        } else if (bot < my) {
            if (0 < ai_Unk6f22d0)
                return;
            ai_Unk6f22d0++;
        }
        ai_Unk6f22cc = 0;
        if (ai_Unk6f22d0 != 0) {
            if (ai_Unk6f22d0 == -1) {
                off = 0xb4;
                if (d3_VecEqual(ai_Unkfc2dec + off, zero, 0.2f) != 0)
                    return;
                goto load;
            } else if (ai_Unk6f22d0 == 1) {
                off = 0xcc;
                if (d3_VecEqual(ai_Unkfc2dec + off, zero, 0.2f) != 0)
                    return;
                goto load;
            }
            goto apply;
        }
    }
    off = 0x5c;
    memcpy(pos, ai_Unkfc2dec + 0x5c, 12);
    memcpy(dir, ai_Unkfc2dec + 0x90, 12);
    goto apply;
load:
    memcpy(pos, ai_Unkfc2dec + off, 12);
    memcpy(dir, ai_Unkfc2dec + off + 0xc, 12);
apply:
    ai_Unk75fb80 = pos[0];
    ai_Unk75fb84 = pos[1];
    ai_Unk75fb88 = pos[2];
    ai_Unk75fb90 = dir[0];
    ai_Unk75fb94 = dir[1];
    ai_Unk75fb98 = dir[2];
    d3anim_FreePath(ai_Unkfc2dec);
    ai_Unk6310e4 = 0;
    d3t_CamMoveI = 0;
    d3t_PlaceObjectV(ai_Unkfc2dec, pos, dir, ai_CameraStep, 8);
    ai_Unk6f2378 = 1;
}
