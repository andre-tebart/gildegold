/* 0x513050, enters the scene of the building b (init_ShowBuildingScene): the room object o (the slot of b's object
   list, or 0xfd = the building's own scene) selects the universe (init_ObjectUniverse / init_BuildingUniverse),
   the scene object ai_SceneObject / the building ai_SceneBuilding are set, the detail tree "ordner_DETAILS" is
   shown, the group / dialog state (ch_CurrentGroup / evt_NoGroup) and the fog / light / season defaults are set
   (d3o_FogScale = 1.0, d3_LightKeyBudgetVisible = 0x800, init_TextureSet = season), the scene callbacks are
   registered for home-type buildings (init_PlantTextureSetHook / init_TextureSetHook, mask 0xc0), for the gates
   (init_GateObjectHook, 0x20) and for the scene records (init_SyncSceneObjects, 0x1e0), the objects of the
   building / of its person are shown and the characters near it placed (at most 4, or 8 for the class 0x16;
   ch_List first, then the models of the ai_Chars records)
   @deep behaviour + readable, meaning read whole against 0x513050..0x5133f6 and fixed: the room object's proto is read once (an unsigned short local from a plain unsigned short *o; the old char temp made the table byte sign-extend after the merged call), the room record's own id is at +2 not +4 (*(INIT_OBJ *)o)->objId), the ai_Chars model walk re-reads *q before each use, the two loop ends are signed int compares, b is an INIT_BLD (id +1, objList +0x5d, plants +0x71), sv_PanelShown is an INIT_OBJ * (objId), and the 3 loops use SCENEOBJ (group 0x2c / scriptGroup 0x30) + the INIT_PANELCHAR chain view (+0x3b). Left: allocation / scheduler shape - 13 hunks / 94 differing instructions, none a meaning difference: (1) VC6 merges the valid room branch's ai_MsgNoOp() (0x4b70f0, a bare ret) with the invalid branch's, the original calls it twice (tried: statement order, polarity, goto, call before the load - always merged); (2) d3if_ShowTree(d, ai_GfxDetail != 0) - the original materializes the char bool with test/jne + push 0/push 1, ours with setne cl/push ecx (tried ternary, !!, & 1, > 0, a cast, an int parameter - no change); (3) the else arm's redundant t->scene == d3_DefaultScene re-test, which VC6 folds for every cast form tried; (4) register allocation of the d3_ForEachObject2 pair (edx/eax vs ecx/edx) and of the two character walks. */


void init_ShowBuildingScene(unsigned char *b, int slot)
{
    INIT_BLD *bd = (INIT_BLD *)b;
    unsigned short *o;
    SCENEOBJ *t;
    SCENEOBJ **q;
    struct D3OBJECT *d;
    int season, cls, i, uni, limit;
    unsigned short proto;
    char *p;

    season = gm_ClockSeason(ai_Time);
    cls = gm_BuildingClass(*b);
    p = ai_BTypes + (unsigned)*b * 0x24d;
    o = (unsigned short *)gm_Query(bd->objList, 1, 0, slot);
    if (o == 0 || (proto = *o) == 0xfd) {
        uni = init_BuildingUniverse[(unsigned char)ai_BTypes[*b * 0x24d]];
        ai_SceneObject = 0;
        ((int (*)())ai_MsgNoOp)();
    } else {
        ai_SceneObject = o;
        uni = init_ObjectUniverse[proto];
        ((int (*)())ai_MsgNoOp)();
    }
    if (d3_CurrentScene == uni && (ai_PlayerBuilding == 0 || ai_PlayerBuilding == (int)b) && (ai_FallbackBuilding == 0 || ai_FallbackBuilding == (int)b)) {
        d = d3_FindObject(0, 0x180, 0, "ordner_DETAILS");
        if (d != 0)
            d3if_ShowTree(d, ai_GfxDetail.byte[0] != 0);
        ch_CurrentGroup = bd->id;
        if (sv_PanelShown == 0)
            evt_NoGroup = bd->id;
        else
            evt_NoGroup = sv_PanelShown->objId;
        if (season == 1)
            init_TextureSet = 1;
        else if (season == 0)
            init_TextureSet = 0;
        else if (season == 2)
            init_TextureSet = 2;
        else if (season == 3)
            init_TextureSet = 3;
        d3_FogByDistance = 0;
        d3o_FogScale = 1.0f;
        d3_LightKeyBudgetVisible = 0x800;
        if (gm_IsHomeType(b) != 0) {
            d3_ForEachObject2(d3_DefaultScene, 0, 0xc0, init_PlantTextureSetHook);
            d3_ForEachObject2(d3_DefaultScene, 0, 0xc0, init_TextureSetHook);
        }
        if (*b == 0x1e && slot == 0xfd)
            veg_ShowPlants((struct G15PLANT *)bd->plants);
        ai_SceneBuilding = b;
        sv_CollectGroupCharacters((char *)b);
        d3_ForEachObject2(d3_DefaultScene, 0, 0x1e0, init_SyncSceneObjects);
        d3_ForEachObject2(d3_DefaultScene, 0, 0x20, init_GateObjectHook);
        gm_ShowBuildingObjects(b);
        i = 0;
        if (*p == 0x16)
            limit = 8;
        else
            limit = (*p != 1) * 4 + 4;
        q = (SCENEOBJ **)ch_List;
        do {
            t = *q;
            if (t != 0 && d3t_SceneIndex(t->scene) == uni) {
                if (t->group == bd->id || t->scriptGroup == ((INIT_OBJ *)o)->objId) {
                    ch_SetVisible((struct CHARACTER *)t, 1);
                    ch_PlaceOnGround((struct CHARACTER *)t, 0);
                } else
                    ch_SetVisible((struct CHARACTER *)t, 0);
            }
            q++;
        } while ((int)q < (int)(ch_List + INIT_CHARACTER_COUNT));
        q = &((INIT_CHAR *)ai_Chars)->model;
        do {
            if (*q != 0 && d3t_SceneIndex((*q)->scene) == uni) {
                t = *q;
                if ((t->group == bd->id || t->scriptGroup == ((INIT_OBJ *)o)->objId) && i < limit) {
                    ch_SetVisible((struct CHARACTER *)t, 1);
                    ch_PlaceOnGround(*q, 0);
                    gm_SelectModelVariant((char *)q - 0x184);
                    gm_ChooseHeadIcon((unsigned short *)((char *)q - 0x184));
                    i++;
                } else
                    ch_SetVisible((struct CHARACTER *)t, 0);
            }
            q += 0x87;
        } while ((int)q < (int)((char *)ai_Chars + 0x184 + INIT_CHARACTER_COUNT * 0x21c));
        t = (SCENEOBJ *)gm_Query(0, 2, 7, 4, 0x1d);
        while (t != 0) {
            t = ((INIT_PANELCHAR *)t)->next;
            if (t->scene == d3_DefaultScene) {
                if (t->group == bd->id) {
                    ch_SetVisible((struct CHARACTER *)t, 1);
                    ch_PlaceOnGround((struct CHARACTER *)t, 0);
                    ch_PlaceTransport((struct CHARACTER *)t, 1);
                } else if ((int)t->scene == (int)d3_DefaultScene)
                    ch_SetVisible((struct CHARACTER *)t, 0);
            }
            t = (SCENEOBJ *)gm_QueryNext();
        }
    }
}
