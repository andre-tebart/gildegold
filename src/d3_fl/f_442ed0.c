/* 0x442ed0, animates n water regions: texture frame, texture offsets and the 16 wave vectors */

void d3fl_UpdateWater(unsigned long time, unsigned int n, D3FLREGION *r)
{
    unsigned int k;
    int i, dt;
    float t, *w;
    D3VECTOR4 v;
    double d;

    for (k = 0; k < n; k++, r++) {
        if (r->tex && r->tex->frames > 0 && r->tex->speed > 0)
            r->tex2 = tex_GetFrame((D3TEXTURE *)r->tex - d3_Textures,
                                   time / d3_AnimDiv[r->tex->speed] % r->tex->frames);
        dt = time - r->time;
        if (dt > 0) {
            t = (float)dt;
            r->uOfs = (float)fmod(t * r->scrollSpeedU + r->uOfs, 1.0);
            r->vOfs = (float)fmod(t * r->scrollSpeedV + r->vOfs, 1.0);
            for (i = 0; i < 4; i++)
                r->wavePhase[i] = (float)fmod(t * r->speed[i] + r->wavePhase[i], D3FL_2PI);
            for (i = 0; i < 16; i++) {
                w = r->wave[i >> 2][i & 3];
                v.x = (float)sin(cos(2.7 * i) * D3FL_2PI + r->wavePhase[0] + i);
                v.y = (float)cos(sin(3.1415927f - 2.4 * i) * D3FL_2PI + r->wavePhase[1] + i);
                v.z = (float)sin(i - cos(2.5 * i) * D3FL_2PI + r->wavePhase[2] + 2.2);
                d = cos(i - sin(2.6 * i + 3.1415927f) * D3FL_2PI + r->wavePhase[3] + 4.0);
                w[0] = v.x * r->amp[0];
                w[1] = v.y * r->amp[1];
                w[2] = v.z * r->amp[2];
                w[3] = (float)(d * r->amp[3]);
            }
            r->time = time;
        }
    }
}
