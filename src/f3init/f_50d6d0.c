/* 0x50d6d0, loads the town scene "scenes/*stadt_%s.ed3" (name n) into the current scene; the callback 0x50d730 adjusts its lights. 0 when it does not exist */


char init_LoadTownScene(char *n)
{
    char buf[0x100];

    sprintf(buf, "scenes/*stadt_%s.ed3", n);
    if (d3_LoadScene(buf, 0, 0) == 0)
        return 0;
    d3_ForEachObject2(d3_DefaultScene, 0, 6, init_AdjustSceneLights);
    return 1;
}
