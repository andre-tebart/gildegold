/* 0x412550, mean terrain height over the 8x8 cells around pos */

typedef struct D3HMAP {             /* terrain height map */
    char pad00[0x4];
    float base;                     /* 0x04, height of value 0 */
    char pad08[0xc];
    float scale;                    /* 0x14, height per unit */
    char pad18[0x8];
    int size;                       /* 0x20 */
    char pad24[0x4];
    unsigned char *data;            /* 0x28 */
} D3HMAP;

char d3_GetHeightAt(struct D3SM *h, struct D3VECTOR *pos, int *px, int *pz, float *py);              /* 0x45f170 */

float d3t_GetMeanHeight(D3VECTOR *pos)
{
    D3HMAP *hm;
    int x, z, i, j;
    float h;
    float sum = 0.0f;

    if (d3_DefaultScene && (hm = d3_DefaultScene->hmap)) {
        if (d3_GetHeightAt(hm, pos, &x, &z, &h)) {
            for (i = -4; i < 4; i++) {
                for (j = -4; j < 4; j++) {
                    if (z + i < hm->size - 1 && x + j < hm->size - 1 && z + i > 0 && x + j > 0)
                        sum += hm->data[(z + i) * hm->size + x + j] * hm->scale + hm->base;
                    else
                        sum += h;
                }
            }
            sum /= 64;
        }
        return sum;
    }
    return 0.0f;
}
