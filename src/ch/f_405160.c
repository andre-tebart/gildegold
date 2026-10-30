/* 0x405160, a random free map vertex near pos */

int ch_RandomNear(D3VECTOR *pos, D3VECTOR *out)
{
    D3SM *h = (D3SM *)d3_DefaultScene->hmap;
    int x, z;
    float y;
    int nx, nz;

    if (!h)
        return 0;
    d3_GetHeightAt(h, pos, &x, &z, &y);
    nx = util_RandomU(10) - 4 + x;
    nz = util_RandomU(10) - 4 + z;
    if (h->size - 2 < nx)
        nx = h->size - 2;
    if (nx < 2)
        nx = 2;
    if (h->size - 2 < nz)
        nz = h->size - 2;
    if (nz < 2)
        nz = 2;
    if (ch_FindFreeCell(h, pos, pos, nx, nz, &nx, &nz, 600, 0))
        d3sm_GetVertex(h, nx, nz, out);
    else
        d3sm_GetVertex(h, x, z, out);
    return 1;
}
