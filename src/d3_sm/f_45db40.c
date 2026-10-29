/* 0x45db40, ground type at (x, z) of the terrain */
int d3sm_GetGround(unsigned int x, unsigned int z, D3TERRAIN *t)
{
    int idx, i, k;

    idx = (t->size - 1 & z) * t->size + (t->size - 1 & x);
    if (t->types[idx] & 0x80)
        return 0;
    if (d3sm_Last != t) {
        for (i = 0; i < 8; i++) {
            d3sm_Ground[i] = 1;
            if (D3TERRAIN_NAMES(t)[i][0]) {
                for (k = 0; k < 15; k++) {
                    if (strstr(_strupr(D3TERRAIN_NAMES(t)[i]), d3sm_GroundNames[k])) {
                        d3sm_Ground[i] = k;
                        break;
                    }
                }
            }
        }
        d3sm_Last = t;
    }
    return d3sm_Ground[t->types[idx]];
}
