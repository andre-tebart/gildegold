/* 0x532700, starts a city game: the scene / lists are reset (0x4478e0, 0x4473f0), the city file "<gamedata/cities>\<name>.CTY" (or ".NET" for a network game: n != 0) is loaded through 0x5190b0, the world
   is initialised (0x503e80, 0x4fe7f0 / 0x4fe830, 0x50d490) and the scene "scenes/*ChooseCity.ed3" / "scenes/*ChooseCity_Europa.ed3" (by 0x5190a0) is opened */

void main_Unk532700(int name, int net)
{
    char path[260];

    d3if_SelectScene(0, 0);
    d3if_Reset();
    init_InitTown(name);
    crt_Sprintf(path, net == 0 ? "%s/%s.CTY" : "%s/%s.NET", "gamedata/cities", name);
    U_I((unsigned)&ai_Time) = 0;
    *(unsigned short *)(unsigned)&ai_TimeHour = 6;
    *(unsigned short *)((unsigned)&ai_Time + 0x6) = 0;
    ls_SaveGame(path, (char *)(unsigned)&full_data_624b10, 2);
    gm_Unk503e80();
    gm_ClearBuildings();
    gm_ClearObjects();
    init_FreeScenes();
    d3if_SelectScene(0, 0);
    d3if_Reset();
    if (ls_GetLanguage() == 0) {
        d3_LoadScene("scenes\\ChooseCity_ed3", 0, 0);
        return;
    }
    d3_LoadScene("scenes\\ChooseCity_Europa_ed3", 0, 0);
}
