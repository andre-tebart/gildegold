/* 0x4ebf60, frees the prototype tables and clears the counts */

void ahm_FreePrototypes(void)
{
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
}
