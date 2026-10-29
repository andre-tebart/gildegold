/* 0x40ddf0, loads the scene *name; a script error when it is not on disk; 1 */

int ocmd_LoadScene(char **name)
{
    if (!d3_LoadScene(*name, 0, 0))
        evt_Error(evt_Cur, evt_Cur->pc, "ecmd_LoadScene: Could not find scene on disk...");
    return 1;
}
