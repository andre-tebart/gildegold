/* 0x43deb0, loads the scene data and sets d3_SceneFlags to ~flags & 3: flag 2 inflates the terrain tiles
   (d3fl_Inflate), flag 1 inflates every object's geometry (d3_InflateObject). The inverse of
   d3_DeflateScene; when the load leaves no scene flag set and the default scene is not the terrain scene,
   the scene's map is released. */

char d3_InflateScene(unsigned int flags)
{
    char msg[0x100];

    sprintf(msg, "iu %i %s (%i df: %i) -> %i", d3_FrameCount, d3_DefaultScene, d3_CurrentScene, d3_SceneFlags, flags);
    if ((flags & 2) && d3_Terrain)
        d3fl_Inflate(d3_Terrain);
    if (flags & 1)
        d3_ForEachObject2(d3_DefaultScene, 0, 0x1ff, d3_InflateObject);
    d3_SceneFlags = ~flags & 3;
    if (!d3_SceneFlags && d3sm_Shadow && !d3_Terrain)
        d3sm_Build(d3sm_Shadow, 0);
    return 1;
}
