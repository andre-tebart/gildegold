/* 0x414b50, frees the snow system: the flake list and texture, then resets the "DC_" textures'
   hicolor state (high colour when the format has no FOURCC and the file is above 8 bpp), flushes
   the texture cache and rebuilds the other scenes' tile maps; null snow is ignored.
   @deep Q2: D3TEXTURE.unk58 (0x58, TSTEXTURE.unk58) has no reader anywhere in src - every use is a zero store (the d3_fl.c 0x43faa0 @deep note records the same) - so no role can be stated; the other Q2 names are gone this session (refs/filebpp, D3SCENE.first, d3fl_ReloadTextures, d3d_UsedTexFormat.fourcc) */

void d3t_FreeSnow(D3TSNOW *snow)
{
    unsigned int i;
    int s;
    D3TEXTURE *tex;

    if (snow) {
        tex_Release(snow->Texture);
        if (snow->flakes) {
            mem_Free(snow->flakes);
            snow->flakes = 0;
        }
        tex = d3_Textures;
        for (i = 0; i < d3_TexCount; tex++, i++) {
            if (tex->refs > 0 && _strnicmp("DC_", tex->name, 3) == 0 && !(tex->flags & 2)) {
                if (!tex_SetHiColor(tex, !d3d_UsedTexFormat.fourcc && tex->filebpp > 8 ? 1 : 0))
                    tex_Unload(tex);
                tex->unk58 = 0;
            }
        }
        d3fl_Flush();
        for (s = 0; s < D3_MAX_SCENES; s++) {
            if (s != d3_CurrentScene && d3_Scenes[s].first && d3_Scenes[s].map)
                d3fl_ReloadTextures(d3_Scenes[s].map);
        }
        if (d3_Terrain)
            d3fl_ReloadTextures(d3_Terrain);
        mem_Free(snow);
    }
}
