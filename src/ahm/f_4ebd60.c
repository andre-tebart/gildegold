/* 0x4ebd60, writes the prototypes to "/gamedata/ai/alchemy.dfn" in the format 0x4eba80 reads; 1 when all was written */

int ahm_SaveDfn(void)
{
    void *f;
    int magic, i;

    magic = 0x61486d52;
    f = vfs_Tfopen("/gamedata/ai/alchemy.dfn", "wb", vfs_Root);
    if (f != 0) {
        if (vfs_fwrite(&magic, 4, 1, f) > 0 && vfs_fwrite(&ai_PlayerPrototypeCount, 4, 1, f) > 0) {
            for (i = 0; i < ai_PlayerPrototypeCount; i++) {
                if (vfs_fwrite(&ai_PlayerPrototypes[i].plr.attr[0], 1, 1, f) <= 0 ||
                    vfs_fwrite(&ai_PlayerPrototypes[i].plr.attr[1], 1, 1, f) <= 0 ||
                    vfs_fwrite(&ai_PlayerPrototypes[i].plr.attr[2], 1, 1, f) <= 0 ||
                    vfs_fwrite(&ai_PlayerPrototypes[i].plr.attr[3], 1, 1, f) <= 0 ||
                    vfs_fwrite(&ai_PlayerPrototypes[i].plr.attr[4], 1, 1, f) <= 0 ||
                    vfs_fwrite(&ai_PlayerPrototypes[i].plr.id, 4, 1, f) <= 0)
                    goto bad;
            }
            if (vfs_fwrite(&ai_RecipePrototypeCount, 4, 1, f) > 0) {
                for (i = 0; i < ai_RecipePrototypeCount; i++) {
                    if (vfs_fwrite(&ai_RecipePrototypes[i].rcp.id, 4, 1, f) <= 0 ||
                        vfs_fwrite(&ai_RecipePrototypes[i].rcp.a, 4, 1, f) <= 0 ||
                        vfs_fwrite(&ai_RecipePrototypes[i].rcp.b, 4, 1, f) <= 0)
                        goto bad;
                }
                vfs_fclose(f);
                return 1;
            }
        }
bad:
        vfs_fclose(f);
    }
    return 0;
}
