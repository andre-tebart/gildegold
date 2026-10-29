/* 0x531690, shuts the screen / render side down: closes the video mode flag, marks the d2 layer active, frees the item
   list surfaces, saves the options, exits the 2D layer, deselects the scene, frees the snow mipmaps and shuts the 3D
   engine down, then releases the direct input devices */

void main_ShutdownScreen(void)
{
    main_ScreenOpen = 0;
    d2_CutsceneActive = 1;
    d2_FreeItemListSurfaces();
    ai_SaveOptions();
    d2_Exit();
    d3if_SelectScene(0, 1);
    d3t_FreeSnowMipmaps();
    d3_Shutdown();
    di_Exit();
}
