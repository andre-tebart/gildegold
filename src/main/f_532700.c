/* 0x532700, starts a city game: the scene / lists are reset, the town `name` is initialised, the city file "gamedata/cities/<name>.CTY" (".NET" for a network game: net != 0) is written through ls_SaveGame
   after the game clock is set to the start (year 0, hour 6, minute 0), the characters, buildings and objects are cleared and the scene "scenes/*ChooseCity.ed3" (language 0) or
   "scenes/*ChooseCity_Europa.ed3" is loaded */

void main_StartCityGame(char *name, int net)
{
    char path[260];

    d3if_SelectScene(0, 0);
    d3if_Reset();
    init_InitTown(name);
    if (net != 0)
        sprintf(path, "%s/%s.NET", "gamedata/cities", name);
    else
        sprintf(path, "%s/%s.CTY", "gamedata/cities", name);
    ai_Time.year = 0;
    ai_Time.hour = 6;
    ai_Time.min = 0;
    ls_SaveGame(path, "city", 2);
    gm_ResetCharacters();
    gm_ClearBuildings();
    gm_ClearObjects();
    init_FreeScenes();
    d3if_SelectScene(0, 0);
    d3if_Reset();
    if (ls_GetLanguage() == 0) {
        d3_LoadScene("scenes/*ChooseCity.ed3", 0, 0);
        return;
    }
    d3_LoadScene("scenes/*ChooseCity_Europa.ed3", 0, 0);
}
