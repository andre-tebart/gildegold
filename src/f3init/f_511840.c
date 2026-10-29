/* 0x511840, frees and re-creates the quad tree 0x6f1c28 from the scene's objects (0x511810) */

void unk_Unk511840(void)
{
    if (d3_QuadTree != 0) {
        d3q_FreeNode((D3QNODE *)d3_QuadTree);
        d3_QuadTree = 0;
    }
    d3_ForEachObject2(d3_DefaultScene, 0, 0x40, ai_Unk511810);
    d3_QuadTree = (int)d3q_Create(0, 0x40, 8, 7);
}
