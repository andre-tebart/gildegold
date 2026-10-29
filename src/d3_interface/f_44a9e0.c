/* 0x44a9e0, maps a scene file version (d3_SaveScene writes 0x3a6c00bb) to a format level 0 / 4 / 6 / 7;
   presumably used by the scene loader after this range */

int d3_SceneVersion(unsigned int version)
{
    if (version >= 0x3a6c00ba)
        return 7;
    if (version >= 0x3a6c00a5)
        return 6;
    return version < 0x3a6c00a1 ? 0 : 4;
}
