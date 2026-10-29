/* 0x4eae60, the entry of the recipe prototype table (ai_RecipePrototypes, count ai_RecipePrototypeCount) with the id; 0 when there is none */

AHMGRID *ahm_FindRecipe(int id)
{
    int i;
    AHMGRID *p;

    i = 0;
    p = ai_RecipePrototypes;
    if (ai_RecipePrototypeCount > 0) {
        do {
            if (p->rcp.id == id)
                return ai_RecipePrototypes + i;
            i++;
            p++;
        } while (i < ai_RecipePrototypeCount);
    }
    return 0;
}
