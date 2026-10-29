/* 0x417170, fades the sun rays in: SunFade = 1 from now and the value d3t_FadeOutSunRays stored is cleared @deep Q2 d3t_Unk631128: the same write-only global (FadeOut stores its argument, this function 0); no read in src, no caller of either function, no role evidence - match-deep's name (Q6 fixed here: the 0x417170 comment now describes the fade-in). Byte otherwise */

void d3t_FadeInSunRays(D3OBJECT *unused)
{
    d3t_SunFade = 1;
    d3t_SunFadeStart = d3_Time;
    d3t_Unk631128 = 0;
}
