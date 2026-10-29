/* 0x42b010 */

void evt_UnloadAll(void)
{
    int i;

    for (i = 0; i < 128; i++)
        if (evt_Scripts[i].name[0] || evt_Scripts[i].id != -1)
            evt_Unload(&evt_Scripts[i]);
}
