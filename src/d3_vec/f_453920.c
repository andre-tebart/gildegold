/* 0x453920, fills d3fl_Shade with 1 - 2/pi * acos(i / 1024) and sets the per-frame light key
 * budgets for lights that are culled (out of view) and for the ones that pass the clip test */
void d3_ShadeInit(unsigned int visible_budget, unsigned int culled_budget)
{
    int i;

    for (i = 0; i < 0x400; i++)
        d3fl_Shade[i] = 1.0f - (float)acos(i * 0.0009765625f) * 0.63661975f;
    d3_LightKeyBudgetVisible = visible_budget;
    d3_LightKeyBudgetCulled = culled_budget;
    d3_LightKeysVisible = 0;
    d3_LightKeysCulled = 0;
    d3_LightGeneration = 0;
}
