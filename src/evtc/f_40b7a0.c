/* 0x40b7a0, script command FindScript: returns the script record of that name (evt_FindByName), 0 if no
   script of the name is registered */

int evtc_FindScript(char **name)
{
    return (int)evt_FindByName(*name);
}
