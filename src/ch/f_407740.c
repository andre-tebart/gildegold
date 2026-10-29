/* 0x407740, converts cell x/z of map h to the cell of map to */

int ch_CellToCell(D3SM *h, int x, int z, D3SM *to, int *px, int *pz)
{
    D3VECTOR4 v;

    if (d3sm_GetVertex(h, x, z, (D3VECTOR *)&v)) {
        ch_PosToCell(to, (D3VECTOR *)&v, px, pz);
        return 1;
    }
    return 0;
}
