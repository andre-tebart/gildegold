/* 0x413d30: n commands follow: 0 count | 1 count duration | 2 windx windz duration */

void d3t_SetRain(D3TRAIN *rain, int n, ...)
{
    int *args = &n;
    int k = 0;
    int i, j, cmd, cap;
    D3TRAINDROP *drops, *drop;

    for (i = 0; i < n; i++) {
        cmd = args[++k];
        switch (cmd) {
        case 0:
        case 1:
            if (args[++k] >= rain->capacity) {
                cap = args[k] + 100;
                drops = mem_Alloc(cap * sizeof(D3TRAINDROP), "d3t:RainDropList");
                if (!drops)
                    return;
                memcpy(drops, rain->drops, rain->capacity * sizeof(D3TRAINDROP));
                mem_Free(rain->drops);
                rain->drops = drops;
                rain->capacity = cap;
                drop = &drops[rain->count];
                for (j = rain->count; j < cap; drop++, j++) {
                    drop->pos.x = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
                    drop->pos.y = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
                    drop->pos.z = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
                    drop->pos.w = (float)rand() / RAND_MAX * 0.25f + 0.25f;
                    drop->windScale = (float)rand() / RAND_MAX * 0.05f + 0.25f;
                    drop->fallScale = (float)rand() / RAND_MAX * 0.05f + 0.25f;
                }
            }
            if (cmd == 0) {
                rain->count = args[k];
            } else {
                rain->oldcount = rain->count;
                rain->newcount = args[k];
                rain->time30 = rain->time2c;
                rain->time34 = rain->time2c + args[++k];
            }
            break;
        case 2:
            rain->oldwindx = rain->windx;
            rain->oldwindz = rain->windz;
            rain->newwindx = args[++k] * 0.001f;
            rain->newwindz = args[++k] * 0.001f;
            rain->time38 = rain->time2c;
            rain->time3c = rain->time2c + args[++k];
            break;
        }
    }
}
