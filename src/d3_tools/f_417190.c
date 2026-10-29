/* 0x417190, creates the "sonnenstrahlen" object at the camera position, rotated to the light "pLicht" (yaw of
   the sun), marks it kind 5 / mask 1 / state flags 0xc and fades it in; returns it */

void d3t_UpdateSunRays(D3OBJECT *obj);
void d3t_RemoveSunRays(D3OBJECT *obj);

#define D3T_VSET(v, a, b, c) ((v).x = (a), (v).y = (b), (v).z = (c), (D3VECTOR *)&(v))

D3OBJECT *d3t_CreateSunRays(void)
{
    D3VECTOR rot;
    D3VECTOR4 pos;
    D3OBJECT *light;

    light = d3_FindObject(0, 8, 0, "pLicht");
    d3t_SunRays = d3_CreateObject(0, D3T_VSET(pos, d3_Camera->pos.x, d3_Camera->pos.y, d3_Camera->pos.z),
                                        D3T_VSET(rot, 0.0f, light->rotation.y, 0.0f), "sonnenstrahlen");
    d3t_SunRays->stateFlags |= 0xc;
    d3t_SunRays->objectFlags &= ~2;
    d3t_SunRays->kind = 5;
    d3t_SunRays->mask = 1;
    d3t_SunRays->flags &= ~4;
    d3t_FadeInSunRays(d3t_SunRays);
    d3t_UpdateSunRays(d3t_SunRays);
    return d3t_SunRays;
}
