/* 0x43b6e0, marks all objects of the default scene for an update. */

void d3_UpdateAll(unsigned char full)
{
    d3_ForEachObject(d3_DefaultScene, 0, 0x1ff, (D3OBJECTCB)d3_MarkDirtyCB, (void *)full);
    if (d3_Sky)
        d3sky_Init(d3_Sky, 0);
    if (d3_Terrain)
        ((unsigned char *)d3_Terrain)[0x1c70] |= 1;
    if (d3s_Object && d3s_Object->material) {
        D3_MESH(d3s_Object)->surf[0].info.ntris = 0;
        D3_MESH(d3s_Object)->surf[0].info.nkeys = 0;
        d3s_Object->state |= 4;
        d3s_Object->flags &= 0xfe;
        d3s_NumUVs = 0;
        d3s_NumVerts = 0;
    }
}
