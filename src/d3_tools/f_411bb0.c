/* 0x411bb0, the index of a scene slot that is neither loaded nor dirty, or -1 */

int d3t_FindFreeScene(void)
{
    int i;
    for (i = 0; i < D3_MAX_SCENES; i++) {
        if (!d3_Scenes[i].loaded && !d3_Scenes[i].dirty)
            return i;
    }
    return -1;
}
