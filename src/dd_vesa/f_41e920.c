/* 0x41e920, shift and lost bits of each channel of a pixel format */

void dd_GetMaskShifts(DWORD rmask, DWORD gmask, DWORD bmask, unsigned char *rloss, unsigned char *rshift,
                      unsigned char *gloss, unsigned char *gshift, unsigned char *bloss, unsigned char *bshift)
{
    unsigned char bs = 0, gs = 0, rs = 0;
    unsigned char rb, gb, bb;
    int i;

    for (i = 32; i; i--) {
        if (!(rmask & 1)) {
            rmask >>= 1;
            rs++;
        }
        if (!(gmask & 1)) {
            gmask >>= 1;
            gs++;
        }
        if (!(bmask & 1)) {
            bmask >>= 1;
            bs++;
        }
    }
    rb = gb = bb = 0;
    for (i = 32; i; i--) {
        if ((rmask & 1) == 1) {
            rmask >>= 1;
            rb++;
        }
        if ((gmask & 1) == 1) {
            gmask >>= 1;
            gb++;
        }
        if ((bmask & 1) == 1) {
            bmask >>= 1;
            bb++;
        }
    }
    *rloss = 8 - rb;
    *gloss = 8 - gb;
    *bloss = 8 - bb;
    *rshift = rs;
    *gshift = gs;
    *bshift = bs;
}
