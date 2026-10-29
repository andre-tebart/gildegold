/* 0x4172e0, updates the sun rays each frame: follows the camera position, turns toward the light "pLicht"
   (0.0002 rad steps while more than 0.01 off), fades by the camera's yaw / pitch angle to the sun
   (d3t_SmoothStep) and by the 500 ms fade in / out, then sets the object alpha (128 * fade * the two angles) */

void d3t_UpdateSunRays(D3OBJECT *obj)
{
    D3VECTOR4 pos;
    D3OBJECT *light;
    D3TEXFLAGS f;
    float d, a, s1, s2, fade;

    if (!obj || !obj->info || !d3_Camera)
        return;
    if (!d3_VecEqual(D3T_VSET(pos, d3_Camera->pos.x, d3_Camera->pos.y, d3_Camera->pos.z), &obj->pos, 0.0001f))
        d3_SetObjectPos(obj, (D3VECTOR *)&pos);
    light = d3_FindObject(0, 8, 0, "pLicht");
    if (light) {
        d = light->rotation.y - obj->rotation.y;
        if (fabs(d) >= (double)0.01f) {
            D3VECTOR rot;
            d = (d > 0.0f ? 0.0002f : -0.0002f) + obj->rotation.y;
            d3_SetObjectRot(obj, D3T_VSET(rot, 0.0f, d, 0.0f));
        }
    }
    a = (float)fmod(obj->rotation.y - d3_Camera->rotation.y, 6.2831855f);
    if (a < 0.0f)
        a += 6.2831855f;
    a = (float)fmod(a, 6.2831855f);
    s1 = d3t_SmoothStep(1.0995574f, 3.1415927f, a > 3.1415927f ? 3.1415927f - (a - 3.1415927f) : a);
    a = (float)fmod(d3_Camera->rotation.x, 6.2831855f);
    a = (float)fmod(a, 6.2831855f);
    s2 = d3t_SmoothStep(-1.1780972f, 0.0f, a);
    if (d3t_SunFade == 1) {
        fade = ((float)d3_Time - d3t_SunFadeStart) * 0.002f;
        if (fade >= 1.0)
            d3t_SunFade = 0;
    } else if (d3t_SunFade == -1) {
        fade = 1.0f - ((float)d3_Time - d3t_SunFadeStart) * 0.002f;
        if (fade <= 0.0f) {
            d3t_SunFade = 0;
            d3t_RemoveSunRays(obj);
            d3t_SunRays = 0;
            return;
        }
    } else {
        fade = 1.0f;
    }
    f.all = 0;
    f.b[2] = 1;
    f.b[0] = (unsigned char)(fade * s2 * s1 * 128.0f);
    d3_SetObjectAlpha(obj, obj->info, f);
}
