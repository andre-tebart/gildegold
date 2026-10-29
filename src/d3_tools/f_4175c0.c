/* 0x4175c0, frees the surfaces of the four software snow mipmap chains (8/16/32-bit and the dither noise,
   levels 1x1..256x256) and clears their pointers */

void d3t_FreeSnowMipmaps(void)
{
    int i;
    for (i = 0; i < 9; i++) {
        mem_Free(d3t_SnowMip8[i].lpSurface);
        d3t_SnowMip8[i].lpSurface = 0;
    }
    for (i = 0; i < 9; i++) {
        mem_Free(d3t_SnowMip16[i].lpSurface);
        d3t_SnowMip16[i].lpSurface = 0;
    }
    for (i = 0; i < 9; i++) {
        mem_Free(d3t_SnowMip32[i].lpSurface);
        d3t_SnowMip32[i].lpSurface = 0;
    }
    for (i = 0; i < 9; i++) {
        mem_Free(d3t_SnowDither[i].lpSurface);
        d3t_SnowDither[i].lpSurface = 0;
    }
}
