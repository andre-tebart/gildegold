/* 0x414c60: n commands follow: 0 count | 1 count duration | 2 windx windz duration | 3 unk @deep Q2: D3TSNOW.unk54 is only written (CreateSnow sets 0, SetSnow command 3 keeps max(unk54, arg*2500)) and never read in src - no role can be stated (the same gap the d3t_CreateSnow escalation records); Q6 description is already in the address comment */

void d3t_SetSnow(D3TSNOW *snow, int n, ...)
{
    int *args = &n;
    int k = 0;
    int i, j, cmd, cap;
    float f;
    D3TSNOWFLAKE *flakes, *flake;

    for (i = 0; i < n; i++) {
        cmd = args[++k];
        switch (cmd) {
        case 0:
        case 1:
            if (args[++k] >= snow->capacity) {
                cap = args[k] + 100;
                flakes = mem_Alloc(cap * sizeof(D3TSNOWFLAKE), "d3t:SnowFlakeList");
                if (!flakes)
                    return;
                memcpy(flakes, snow->flakes, snow->capacity * sizeof(D3TSNOWFLAKE));
                mem_Free(snow->flakes);
                snow->flakes = flakes;
                snow->capacity = cap;
                flake = &flakes[snow->count];
                for (j = snow->count; j < cap; flake++, j++) {
                    flake->pos.x = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
                    flake->pos.y = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
                    flake->pos.z = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
                    flake->pos.w = (float)rand() / RAND_MAX * 0.5f + 0.75f;
                    flake->windScale = (float)rand() / RAND_MAX * 0.01f + 0.025f;
                    flake->fallScale = (float)rand() / RAND_MAX * 0.01f + 0.025f;
                }
            }
            if (cmd == 0) {
                snow->count = args[k];
            } else {
                snow->oldcount = snow->count;
                snow->newcount = args[k];
                snow->time60 = snow->time5c;
                snow->time64 = snow->time5c + args[++k];
            }
            break;
        case 2:
            snow->oldwindx = snow->windx;
            snow->oldwindz = snow->windz;
            snow->newwindx = args[++k] * 0.001f;
            snow->newwindz = args[++k] * 0.001f;
            snow->time68 = snow->time5c;
            snow->time6c = snow->time5c + args[++k];
            break;
        case 3:
            j = args[++k];
            snow->unk54 = max(snow->unk54, j * 2500.0f);
            break;
        }
    }
}
