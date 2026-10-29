/* 0x4eba80, reads the prototypes from "/gamedata/ai/alchemy.dfn" (the magic 0x61486d52, the counts 1 .. 100, the player entries (5 bytes + the id) and the recipe
   entries (3 ints)); 1 when all was read, else the tables are freed and 0 */

int ahm_LoadDfn(void)
{
    void *f;
    int magic, i;

    f = vfs_Tfopen("/gamedata/ai/alchemy.dfn", "rb", vfs_Root);
    if (f != 0) {
        if (0 < vfs_fread(&magic, 4, 1, f) && magic == 0x61486d52 && 0 < vfs_fread(&ai_PlayerPrototypeCount, 4, 1, f) &&
            ai_PlayerPrototypeCount >= 1 && ai_PlayerPrototypeCount <= 0x64) {
            if (ai_PlayerPrototypes != 0) {
                mem_Free(ai_PlayerPrototypes);
                ai_PlayerPrototypes = 0;
            }
            ai_PlayerPrototypes = mem_Alloc(ai_PlayerPrototypeCount * 12, "ahm_AllocPlayerPrototypes");
            if (ai_PlayerPrototypes != 0) {
                for (i = 0; i < ai_PlayerPrototypeCount; i++) {
                    if (vfs_fread(&ai_PlayerPrototypes[i].plr.attr[0], 1, 1, f) <= 0 ||
                        vfs_fread(&ai_PlayerPrototypes[i].plr.attr[1], 1, 1, f) <= 0 ||
                        vfs_fread(&ai_PlayerPrototypes[i].plr.attr[2], 1, 1, f) <= 0 ||
                        vfs_fread(&ai_PlayerPrototypes[i].plr.attr[3], 1, 1, f) <= 0 ||
                        vfs_fread(&ai_PlayerPrototypes[i].plr.attr[4], 1, 1, f) <= 0 ||
                        vfs_fread(&ai_PlayerPrototypes[i].plr.id, 4, 1, f) <= 0)
                        goto bad;
                }
                if (0 < vfs_fread(&ai_RecipePrototypeCount, 4, 1, f) && ai_RecipePrototypeCount >= 1 && ai_RecipePrototypeCount <= 0x64) {
                    if (ai_RecipePrototypes != 0) {
                        mem_Free(ai_RecipePrototypes);
                        ai_RecipePrototypes = 0;
                    }
                    ai_RecipePrototypes = mem_Alloc(ai_RecipePrototypeCount * 12, "ahm_AllocRecipePrototypes");
                    if (ai_RecipePrototypes != 0) {
                        for (i = 0; i < ai_RecipePrototypeCount; i++) {
                            if (vfs_fread(&ai_RecipePrototypes[i].rcp.id, 4, 1, f) <= 0 ||
                                vfs_fread(&ai_RecipePrototypes[i].rcp.a, 4, 1, f) <= 0 ||
                                vfs_fread(&ai_RecipePrototypes[i].rcp.b, 4, 1, f) <= 0)
                                goto bad;
                        }
                        vfs_fclose(f);
                        return 1;
                    }
                }
            }
        }
bad:
        vfs_fclose(f);
    }
    if (ai_PlayerPrototypes != 0) {
        mem_Free(ai_PlayerPrototypes);
        ai_PlayerPrototypes = 0;
    }
    if (ai_RecipePrototypes != 0) {
        mem_Free(ai_RecipePrototypes);
        ai_RecipePrototypes = 0;
    }
    ai_RecipePrototypeCount = 0;
    ai_PlayerPrototypeCount = 0;
    return 0;
}
