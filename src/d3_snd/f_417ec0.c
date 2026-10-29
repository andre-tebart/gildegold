/* 0x417ec0 */

/* Water ambience ("d3sndw"): rays are cast around the camera to find the nearest water of each type;
   up to two water sounds play at once in d3sndw_Snd. */
static float *d3sndw_Sin = 0;            /* 0x631134, sin of each ray angle */
static int *d3sndw_Type = 0;             /* 0x631138, water type hit by each ray */
static int *d3sndw_State = 0;            /* 0x63113c, per ray: 0 done, 1 searching, 2 hit, 3 ... */
static unsigned short d3sndw_N = 0;      /* 0x631140, number of rays */
static D3SOUND d3sndw_Snd[2] = {0};        /* 0x631148 */

int d3sndw_Init(unsigned short n)
{
    unsigned short i;
    float a, step;

    if (n > 4) {
        d3sndw_N = n;
        d3sndw_Sin = mem_Alloc(n * sizeof(float), "d3sndw_Init");
        if (d3sndw_Sin) {
            d3sndw_Type = mem_Alloc(n * sizeof(int), "d3sndw_Init");
            if (d3sndw_Type) {
                D3_ZERO_AT(d3sndw_Type, n * sizeof(int));
                d3sndw_State = mem_Alloc(n * sizeof(int), "d3sndw_Init");
                if (d3sndw_State) {
                    a = 0.0f;
                    step = 6.2831855f / n;
                    D3_ZERO_AT(d3sndw_State, n * sizeof(int));
                    for (i = 0; i < n; i++) {
                        d3sndw_Sin[i] = sin(a);
                        a += step;
                    }
                    return 1;
                }
                mem_Free(d3sndw_Type);
                d3sndw_Type = NULL;
                mem_Free(d3sndw_Sin);
                d3sndw_Type = NULL;
                d3sndw_Sin = NULL;
                return 0;
            }
            mem_Free(d3sndw_Sin);
            d3sndw_Sin = NULL;
        }
    }
    return 0;
}
