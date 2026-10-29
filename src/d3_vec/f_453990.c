/* 0x453990, clears the light key budgets and the per-frame key counters and resets the lightmap generation */

void d3_ShadeReset(void)
{
    d3_LightKeyBudgetCulled = 0;
    d3_LightKeyBudgetVisible = 0;
    d3_LightKeysVisible = 0;
    d3_LightKeysCulled = 0;
    d3_LightGeneration = 0;
}
