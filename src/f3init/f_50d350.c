/* 0x50d350, the destroy callback of a scene object: removes its gfx infos and it from the quad tree d3_QuadTree */


void init_SceneObjectDestroyCB(int o)
{
    he_RemoveGfxInfos(o);
    if (d3_QuadTree != 0)
        d3q_RemoveObject((D3OBJECT *)o, (D3QNODE *)d3_QuadTree);
}
