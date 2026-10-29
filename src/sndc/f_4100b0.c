/* 0x4100b0, script command PlaySample3D: plays the named sample of the default bank at the 3D object
   (volume 60, the given range) when the loop count is 1, else non-positional with that loop count; only
   while the running script is in no group; returns the sample or 0 */

struct SBSAMPLE *sndc_PlaySample3D(struct D3OBJECT **obj, int *range, char **name, int *loops)
{
    struct SBSAMPLE *s;

    if (evt_Cur->group != evt_NoGroup)
        return 0;
    s = sb_GetSample(0, *name);
    if (s) {
        if (*loops == 1)
            d3snd_PlayAt(s, *obj, 60, (float)*range);
        else {
            sb_SetLoops(s, *loops);
            sb_Play(s, 0, 0);
        }
    }
    return s;
}
