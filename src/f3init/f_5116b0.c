/* 0x5116b0, collects the torch objects of the scene (0x511600) and re-creates the quad tree 0x6f1c28 */


void unk_Unk5116b0(void)
{
    d3_ForEachObject2(d3_DefaultScene, 0, 0xc0, unk_Unk511600);
    if (d3_QuadTree != 0) {
        d3q_FreeNode((D3QNODE *)d3_QuadTree);
        d3_QuadTree = (int)d3q_Create(0, 0x40, 8, 7);
    }
    d3_LightInvalidate(1);
}
