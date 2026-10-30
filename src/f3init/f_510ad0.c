/* 0x510ad0, initialises the town of the city file `city` (the start of a game): reads the city INI into the
   player slot table (init_LoadCityINI), loads the town scene "scenes/*stadt_%s.ed3" (init_LoadTownScene) and
   returns 1 when it does not exist, empties the offices (amt_InitTable) and the graveyard (gv_ClearGraveyard),
   allocates the buildings and the site names of the scene objects (init_AllocSceneBuilding /
   init_SceneNameToPrototype), inherits the owners (init_InheritOwner) and merges the 16 building site groups
   (init_LoadSiteScene). Then it creates the bank (character kind 10, named "STADT", 200000000 for every player
   slot in use), the opening objects and stalls (init_CreateOpeningObjects / init_StockMarketStalls), hands the
   type 0x1c buildings to the players (gm_AssignBuilding), updates and initialises the player records
   (gm_UpdatePlayers / gm_InitPlayerRecord) and syncs the market (gm_SyncMarket). At the end the display scene
   is selected again and the shadow map of the terrain (0x40 / 0x80 cells) is built (d3sm_Create) and stored in
   the scene (hmap). Every step waits for the network group. Returns 0. */


int init_InitTown(char *city)
{
    char ok;
    int h, i;
    short *p;
    INIT_CHAR *ch;
    char *pc;
    int bank;

    ai_NextObjectId = 1;
    init_LoadCityINI(city, 0);
    d3if_SelectScene(0, 0);
    ok = init_LoadTownScene(city);
    if (ok == 0)
        return 1;
    amt_InitTable();
    gv_ClearGraveyard();
    d3_ForEachObject2(d3_DefaultScene, 0, 0x140, init_AllocSceneBuilding);
    d3_ForEachObject2(d3_DefaultScene, 0, 0x40, init_SceneNameToPrototype);
    d3_ForEachObject2(d3_DefaultScene, 0, 0x20, init_InheritOwner);
    init_LoadSiteScene("bk_BRUNNEN");
    init_LoadSiteScene("bk_GEFAENGNIS");
    init_LoadSiteScene("bk_HANDWERK");
    init_LoadSiteScene("bk_KIRCHE_KL");
    init_LoadSiteScene("bk_KIRCHE_MT");
    init_LoadSiteScene("bk_KIRCHE_GR");
    init_LoadSiteScene("bk_RATHAUS");
    init_LoadSiteScene("bk_STADTWACHE");
    init_LoadSiteScene("bk_WIRTSHAUS_KL");
    init_LoadSiteScene("bk_WIRTSHAUS_GR");
    init_LoadSiteScene("bk_WOHNSITZ_KL");
    init_LoadSiteScene("bk_WOHNSITZ_GR");
    init_LoadSiteScene("bk_ZUNFTHAUS");
    init_LoadSiteScene("bk_RAUBRITTERFESTE");
    init_LoadSiteScene("bk_DOMIZIL");
    init_LoadSiteScene("bk_STATUEN");
    cm_BeginSendGroup();
    cm_SendMessage10(0x44, -1, 0, 0);
    cm_SendMessage10(0x45, -1, 0, 0);
    bank = ((int (*)(int, int, int, int, int, int, int, int))cm_SendCreateCharacter)(10, -1, -1, 0x12, -1, 0, 0, 1);
    cm_EndSendGroup();
    i = cm_SendGroupState();
    while (i == 0) {
        cm_FlushSendQueue();
        cm_ReceiveMessages();
        cm_ExecCommands();
        i = cm_SendGroupState();
    }
    cm_BeginSendGroup();
    ch = (INIT_CHAR *)cm_GetSlotValue(bank);
    ai_PlayerCharC = (char *)ch;
    strcpy(ch->name, "STADT");
    cm_SendMoveGoods(((INIT_CHAR *)ai_PlayerCharC)->netId, -1, ai_OwnPlayer, 200000000);
    i = cm_SendGroupState();
    while (i == 0) {
        cm_FlushSendQueue();
        cm_ReceiveMessages();
        cm_ExecCommands();
        i = cm_SendGroupState();
    }
    cm_EndSendGroup();
    i = 1;
    p = init_SlotCurrency;
    do {
        if (*p != 0)
            cm_SendMoveGoods(((INIT_CHAR *)ai_PlayerCharC)->netId, -1, i, 200000000);
        p += 0x17a;
        i++;
    } while ((int)p < (int)INIT_SLOT_CURRENCY_END);
    init_CreateOpeningObjects();
    cm_BeginSendGroup();
    init_StockMarketStalls();
    cm_EndSendGroup();
    i = cm_SendGroupState();
    while (i == 0) {
        sv_PumpNet();
        i = cm_SendGroupState();
    }
    h = gm_BuildQuery(1, 5, 0x1c);
    while (h != 0) {
        gm_AssignBuilding(h, ((INIT_CHAR *)ai_PlayerCharC)->id, ((INIT_CHAR *)ai_PlayerCharC)->id);
        h = gm_BuildNext();
    }
    gm_UpdatePlayers();
    i = 0;
    pc = ai_PlayerSlotsV[0];
    do {
        if (*pc == 0)
            break;
        gm_InitPlayerRecord(i);
        pc += 0x2f4;
        i++;
    } while ((int)pc < (int)INIT_PLAYER_SLOTS_END);
    cm_BeginSendGroup();
    gm_SyncMarket(ai_OwnPlayer);
    cm_EndSendGroup();
    i = cm_SendGroupState();
    while (i == 0) {
        sv_PumpNet();
        i = cm_SendGroupState();
    }
    d3if_SelectScene(1, 0);
    d3if_SelectScene(0, 0);
    if (((INIT_TERRAIN *)d3_Terrain)->size != 0x80 && ((INIT_TERRAIN *)d3_Terrain)->size == 0x40) {
        d3_DefaultScene->hmap = (struct D3HMAP *)d3sm_Create(d3_Terrain, 0x80, 2);
        return 0;
    }
    d3_DefaultScene->hmap = (struct D3HMAP *)d3sm_Create(d3_Terrain, 0x100, 2);
    return 0;
}
