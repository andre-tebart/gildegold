/* 0x4eaea0, a random entry of the recipe prototype table the character c has no place for yet (a random start and a step that is not a multiple of the count); 0 when
   c may not take more (ahm_HasFreeSlot) */

AHMGRID *ahm_RandomRecipe(char *c)
{
    unsigned int step, pos, k, r;

    if (ahm_HasFreeSlot(c) != 0) {
        step = (util_Random(ai_RecipePrototypeCount & 0xffff) + 1);
        r = (int)step % ai_RecipePrototypeCount;
        while (r == 0) {
            step++;
            r = (int)step % ai_RecipePrototypeCount;
        }
        pos = util_Random(ai_RecipePrototypeCount) & 0xffff;
        k = ai_RecipePrototypeCount;
        while (k != 0) {
            if (ahm_SlotOf(c, ai_RecipePrototypes[pos].rcp.id) == 0)
                return &ai_RecipePrototypes[pos];
            k--;
            pos = (int)(pos + step) % ai_RecipePrototypeCount;
        }
    }
    return 0;
}
