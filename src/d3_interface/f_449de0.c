/* 0x449de0, sets the day-time light set `set` blended towards the next one by t (0..1): the lights,
   the ambient colour (times scale), the sun direction, the fog sets and the sky colour */

char d3_SetLightSet(unsigned int set, float t, float scale, char calc)
{
    float time, u;
    D3VECTOR4 d;
    unsigned int n;
    int j;

    u = 0;
    if (set < 7 && t >= 0.0f && t <= 1.0f) {
        time = set + t;
        d3_ForEachObject(d3_DefaultScene, 0, 0x1e, (D3OBJECTCB)d3_SetLightKeyCB, &time);
        if (scale != 1.0f)
            d3_ForEachObject(d3_DefaultScene, 0, 8, (D3OBJECTCB)d3_ScaleLightCB, &scale);
        n = (set + 1) % 7;
        d.x = d3o_Sets[n].amb.x - d3o_Sets[set].amb.x;
        d.y = d3o_Sets[n].amb.y - d3o_Sets[set].amb.y;
        d.z = d3o_Sets[n].amb.z - d3o_Sets[set].amb.z;
        d3o_Amb.x = d.x * t + d3o_Sets[set].amb.x;
        d3o_Amb.y = d.y * t + d3o_Sets[set].amb.y;
        d3o_Amb.z = d.z * t + d3o_Sets[set].amb.z;
        d3o_Amb.x *= scale;
        d3o_Amb.y *= scale;
        d3o_Amb.z *= scale;
        d3if_Gray = d3o_Amb.x * 0.3f + d3o_Amb.y * 0.59f + d3o_Amb.z * 0.11f;
        d.x = d3o_Sets[n].dir.x - d3o_Sets[set].dir.x;
        d.y = d3o_Sets[n].dir.y - d3o_Sets[set].dir.y;
        d.z = d3o_Sets[n].dir.z - d3o_Sets[set].dir.z;
        d3o_Dir.x = d.x * t + d3o_Sets[set].dir.x;
        d3o_Dir.y = d.y * t + d3o_Sets[set].dir.y;
        d3o_Dir.z = d.z * t + d3o_Sets[set].dir.z;
        u = 1.0f - t;
        for (j = 0; j < 6; j++) {
            D3FOGSET *a = &d3o_Sets[set].fog[j], *b = &d3o_Sets[n].fog[j];
            d3o_Fog[j].col.r = (int)(a->col.r * u + b->col.r * t);
            d3o_Fog[j].col.g = (int)(a->col.g * u + b->col.g * t);
            d3o_Fog[j].col.b = (int)(a->col.b * u + b->col.b * t);
            d3o_Fog[j].start = u * a->start + t * b->start;
            d3o_Fog[j].end = u * a->end + t * b->end;
        }
        d3_LightInvalidate(calc);
        d3o_LightSet = set;
        if (d3_Sky) {
            ((unsigned char *)d3_Sky)[0xaa6] = (int)(d3o_SkyCol[n].r * t + d3o_SkyCol[set].r * u);
            ((unsigned char *)d3_Sky)[0xaa5] = (int)(d3o_SkyCol[n].g * t + d3o_SkyCol[set].g * u);
            ((unsigned char *)d3_Sky)[0xaa4] = (int)(d3o_SkyCol[n].b * t + d3o_SkyCol[set].b * u);
        }
        return 1;
    }
    return 0;
}
