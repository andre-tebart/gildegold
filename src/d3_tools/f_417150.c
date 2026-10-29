/* 0x417150 @deep Q2 d3t_Unk631128: write-only (FadeOut stores the argument, FadeIn 0); no read anywhere in src and no caller of either fade function exists, so no role can be stated - the name needs the original's reader (match-deep). Byte otherwise */

/* Sun rays ("sonnenstrahlen"): a camera-attached object pointing at the light "pLicht", faded in
   and out over 500 ms. */

void d3t_FadeOutSunRays(int unused, int b)
{
    d3t_SunFade = -1;
    d3t_SunFadeStart = d3_Time;
    d3t_Unk631128 = b;
}
