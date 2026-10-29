/* 0x4324e0, the next frame after frame (reverse, loop and ping-pong flags in f.b[1]). */

int d3anim_NextFrame(D3ANIMFLAGS f, int frame, int a, int b, int c)
{
    if (f.b[1] & 2) {
        if (frame > a)
            return frame - 1;
        return a + 1;
    }
    if (frame >= b) {
        if (f.b[1] & 0x10) {
            if (frame + 1 <= c - 1)
                return frame + 1;
            return c - 1;
        }
        if (f.b[1] & 1)
            return frame - 1;
        return a;
    }
    return frame + 1;
}
