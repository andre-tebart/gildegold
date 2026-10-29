/* 0x43d840, transforms every mesh object (type 4, the 0x40 type mask) of the default scene through
   d3_TransformSurf. */

void d3_TransformMeshes(void)
{
    d3_ForEachObject2(d3_DefaultScene, 0, 0x40, d3_TransformSurf);
}
