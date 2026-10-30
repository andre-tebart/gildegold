/* 0x4148f0, allocates and initialises the snow system for count flakes: the D3TSNOW and its flake
   list, counts, wind vectors and times from d3_Time, the saved camera pose, per flake a random
   position in [-1,1], size 0.75..1.25 and wind / fall scales 0.025..0.035, and loads the
   "Schneeflocke" texture; 0 on allocation failure.
   @deep Q2: D3TSNOW.unk10, unk14[9], unk54 and unk58 are only ever written (CreateSnow, SetSnow command 3) and never read in src - D3TSNOW has no user outside d3_tools.c - so no role can be stated; the Q6 description is in place */

typedef struct D3TSNOWFLAKE {        /* 0x28 bytes */
    D3VECTOR4 pos;                   /* w = size */
    float windScale;                 /* 0x10, weight of the wind velocity */
    float fallScale;                 /* 0x14, weight of the fall velocity */
    float sx1, sy1;                  /* 0x18, projected top left */
    float sx2, sy2;                  /* 0x20, projected bottom right */
} D3TSNOWFLAKE;

typedef struct D3TSNOW {             /* 0x94 bytes */
    int count;
    int capacity;
    int oldcount;
    int newcount;
    int unk10;
    int unk14[9];
    D3TSNOWFLAKE *flakes;            /* 0x38 */
    float oldwindx, oldwindz;
    float windx, windz;              /* 0x44 */
    float newwindx, newwindz;
    float unk54;
    int unk58;
    int time5c;                      /* last update */
    int time60, time64, time68, time6c;
    D3VECTOR4 campos;                /* 0x70 */
    D3VECTOR4 cam04c;                /* 0x80 */
    D3TEXHANDLE *Texture;            /* 0x90 */
} D3TSNOW;

D3TSNOW *d3t_CreateSnow(int count)
{
    D3TSNOW *snow;
    D3TSNOWFLAKE *flake;
    D3TEXFLAGS flags;
    int i;

    snow = mem_Alloc(sizeof(D3TSNOW), "d3t:SnowInfo");
    if (!snow)
        return 0;
    snow->count = count;
    snow->capacity = count;
    snow->oldcount = count;
    snow->newcount = count;
    snow->unk10 = 5;
    snow->unk14[0] = 0;
    snow->unk14[1] = 0;
    snow->unk14[2] = 0;
    snow->unk14[3] = 0;
    snow->unk14[4] = 0;
    snow->unk14[5] = 0;
    snow->unk14[6] = 0;
    snow->unk14[7] = 0;
    snow->unk14[8] = 0;
    snow->unk54 = 0;
    snow->unk58 = -1;
    snow->oldwindx = 1.0f;
    snow->oldwindz = 0.0f;
    snow->windx = 1.0f;
    snow->windz = 0.0f;
    snow->newwindx = 1.0f;
    snow->newwindz = 0.0f;
    snow->time5c = d3_Time;
    snow->time60 = d3_Time;
    snow->time64 = d3_Time;
    snow->time68 = d3_Time;
    snow->time6c = d3_Time;
    if (d3_Camera) {
        snow->campos.x = d3_Camera->rotation.x;
        snow->campos.y = d3_Camera->rotation.y;
        snow->campos.z = d3_Camera->rotation.z;
        snow->cam04c.x = d3_Camera->pos.x;
        snow->cam04c.y = d3_Camera->pos.y;
        snow->cam04c.z = d3_Camera->pos.z;
    }
    if (count < 1)
        count = 1;
    snow->flakes = mem_Alloc(count * sizeof(D3TSNOWFLAKE), "d3t:SnowFlakeList");
    if (!snow->flakes) {
        mem_Free(snow);
        return 0;
    }
    flake = snow->flakes;
    for (i = 0; i < snow->count; flake++, i++) {
        flake->pos.x = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
        flake->pos.y = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
        flake->pos.z = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
        flake->pos.w = (float)rand() / RAND_MAX * 0.5f + 0.75f;
        flake->windScale = (float)rand() / RAND_MAX * 0.01f + 0.025f;
        flake->fallScale = (float)rand() / RAND_MAX * 0.01f + 0.025f;
    }
    D3_ZERO(&flags, sizeof(flags));
    flags.b[2] |= 2;
    snow->Texture = (D3TEXHANDLE *)tex_Load("Schneeflocke", flags, 0, 0);
    if (snow->Texture)
        tex_Reload(snow->Texture, 0);
    return snow;
}
