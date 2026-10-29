/* 0x413b30, allocates and initialises the rain system for count drops: the D3TRAIN and its drop
   list, the counts, wind vectors and times from d3_Time, the saved camera pose, and per drop a random
   position in [-1,1], tail length 0.25..0.5 and wind / fall scales 0.25..0.3; 0 on allocation failure. */

D3TRAIN *d3t_CreateRain(int count)
{
    D3TRAIN *rain;
    D3TRAINDROP *drop;
    int i;

    rain = mem_Alloc(sizeof(D3TRAIN), "d3t:RainInfo");
    if (!rain)
        return 0;
    rain->count = count;
    rain->capacity = count;
    rain->oldcount = count;
    rain->newcount = count;
    rain->oldwindx = 1.0f;
    rain->oldwindz = 0.0f;
    rain->windx = 1.0f;
    rain->windz = 0.0f;
    rain->newwindx = 1.0f;
    rain->newwindz = 0.0f;
    rain->time2c = d3_Time;
    rain->time30 = d3_Time;
    rain->time34 = d3_Time;
    rain->time38 = d3_Time;
    rain->time3c = d3_Time;
    if (d3_Camera) {
        rain->campos.x = d3_Camera->rotation.x;
        rain->campos.y = d3_Camera->rotation.y;
        rain->campos.z = d3_Camera->rotation.z;
        rain->cam04c.x = d3_Camera->pos.x;
        rain->cam04c.y = d3_Camera->pos.y;
        rain->cam04c.z = d3_Camera->pos.z;
    }
    if (count < 1)
        count = 1;
    rain->drops = mem_Alloc(count * sizeof(D3TRAINDROP), "d3t:RainDropList");
    if (!rain->drops) {
        mem_Free(rain);
        return 0;
    }
    drop = rain->drops;
    for (i = 0; i < rain->count; drop++, i++) {
        drop->pos.x = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
        drop->pos.y = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
        drop->pos.z = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
        drop->pos.w = (float)rand() / RAND_MAX * 0.25f + 0.25f;
        drop->windScale = (float)rand() / RAND_MAX * 0.05f + 0.25f;
        drop->fallScale = (float)rand() / RAND_MAX * 0.05f + 0.25f;
    }
    return rain;
}
