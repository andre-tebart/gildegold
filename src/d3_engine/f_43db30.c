/* 0x43db30, unloads the scene data and sets d3_SceneFlags to flags: flag 2 frees the terrain tiles
   (d3fl_FreeTiles), flag 1 deflates every object's geometry (d3_DeflateObject). The inverse of
   d3_InflateScene. The sprintf target is a debug trace left over from the original. */

char d3_DeflateScene(unsigned int flags)
{
    char msg[0x100];

    sprintf(msg, "du %i %s (%i df: %i) -> %i", d3_FrameCount, d3_DefaultScene, d3_CurrentScene, d3_SceneFlags, flags);
    if ((flags & 2) && d3_Terrain)
        d3fl_FreeTiles(d3_Terrain);
    if (flags & 1)
        d3_ForEachObject2(d3_DefaultScene, 0, 0x1ff, d3_DeflateObject);
    d3_SceneFlags = flags;
    return 1;
}
