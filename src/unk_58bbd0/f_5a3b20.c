/* 0x5a3b20, the dungeon listener (100 B): the pump 0x5309a0 (this, 0x67fff) looks up "ob_KERKERPLAN" (0x1440870,
   class 0x16) and opens 0x5a3a40 (the dungeon plan window) */

void ai_DungeonContact(int building)
{
    int plan;

    sv_ReleaseNamePlates();
    while (sv_RunFrame(ai_DungeonContact, 0x67fff) != 0) {
        plan = sv_RegisterNamePlate("ob_KERKERPLAN", ai_ContactTextDungeonPlan, 0x16);
        if (ai_ClickedObject != 0 && ai_ClickedObject == plan)
            ai_DungeonPlanWindow(building);
    }
}
