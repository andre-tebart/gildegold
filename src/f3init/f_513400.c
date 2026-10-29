/* 0x513400, finishes the scene of the building b (init_FinishBuildingScene): one of the "contact_EINKAUF*" shop
   counter dummies ("contact_EINKAUF", then _METALL / _SKRIPTE / _HOLZ / _STEIN) is shown for the local player and
   its state (+0x224) set, else the shopping script of the building is loaded (init_LoadShopScript, also for the
   class 5 buildings without a contact dummy); then the sound hook of the "sfx_" objects is registered (mask 0x20),
   the graveyard ambience played, the texture-set slot hook registered for the buildings that are no home type
   (mask 0xc0), the detail level set (d3_Terrain), the season textures switched and the sky colours updated */


void init_FinishBuildingScene(unsigned char *b, unsigned short *o)
{
    INIT_BLD *bd = (INIT_BLD *)b;
    SCENEOBJ *d;
    char *p;

    p = ai_BTypes + (unsigned)*b * 0x24d;
    if (o == 0)
        o = (unsigned short *)gm_Query(bd->objList, 2, 6, 0, 0xfd);
    d = (SCENEOBJ *)d3_FindObject(0, 0xa0, 0, "contact_EINKAUF");
    if (d == 0)
        d = (SCENEOBJ *)d3_FindObject(0, 0xa0, 0, "contact_EINKAUF_METALL");
    if (d == 0)
        d = (SCENEOBJ *)d3_FindObject(0, 0xa0, 0, "contact_EINKAUF_SKRIPTE");
    if (d == 0)
        d = (SCENEOBJ *)d3_FindObject(0, 0xa0, 0, "contact_EINKAUF_HOLZ");
    if (d == 0)
        d = (SCENEOBJ *)d3_FindObject(0, 0xa0, 0, "contact_EINKAUF_STEIN");
    if (d != 0) {
        if (bd->owner == ai_PlayerId)
            d3if_Show((struct D3OBJECT *)d, 0);
        else {
            d3if_Show((struct D3OBJECT *)d, 1);
            init_LoadShopScript(b, o);
        }
        d->mask = 1;
    } else if (*p == 5 && bd->owner != ai_PlayerId)
        init_LoadShopScript(b, o);
    d3_ForEachObject2(d3_DefaultScene, 0, 0x20, init_SoundObjectHook);
    init_PlayGraveyardSound((char *)b);
    if (gm_IsHomeType(b) == 0)
        d3_ForEachObject(d3_DefaultScene, 0, 0xc0, init_BuildingTextureSetHook, b);
    init_SetDetailLevel((int *)d3_Terrain, 0);
    init_SetSeasonTextures();
    d3if_Prerender(0);
    sv_UpdateSkyColors();
}
