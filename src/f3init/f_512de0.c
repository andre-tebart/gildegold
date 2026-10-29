/* 0x512de0, for the scene objects "pfl_" / "vg_" / "!vg_": registers them (0x446f30) with their flags (+0x1cc) in the list 0x6f1c24 */


void init_PlantTextureSetHook(char *o)
{
    if (strncmp(o, "pfl_", 4) != 0 && strncmp(o, "vg_", 3) != 0 && strncmp(o, "!vg_", 4) != 0)
        return;
    d3if_SelectTextureSet(o, *(int *)(o + 0x1cc), init_TextureSet, 1);
}
