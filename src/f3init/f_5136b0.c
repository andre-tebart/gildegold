/* 0x5136b0, leaves the town scene: releases the hooks (0x5127c0), the sounds (0x512890), resets the frame limits and frees the scene handle 0x61ae00 */


void unk_Unk5136b0(void)
{
    int h;

    d3_ForEachObject2(d3_DefaultScene, 0, 0xa0, init_SoundObjectFreeHook);
    unk_Unk512890();
    d3_LightKeyBudgetVisible = 0x400;
    ai_Unk5fcc6c = 0x40;
    if (init_ShopScript != -1) {
        h = (int)evt_FindById(init_ShopScript);
        if (h != 0) {
            evt_Stop((EVTSCRIPT *)h);
            sv_Unk52bb70(*(int *)(h + 0x100));
            init_ShopScript = -1;
        }
    }
    ch_ClearSceneCallbacks(d3_DefaultScene);
    ch_CurrentGroup = -1;
}
