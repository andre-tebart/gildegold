/* 0x4287b0 */

EVTSCRIPT *evt_Load(char *name)
{
    char path[256];

    sprintf(path, "%s%s", evt_Path, name);
    return evt_LoadScript(path);
}
