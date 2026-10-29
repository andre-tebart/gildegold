/* 0x45c9f0, collects the shadow casting lights into d3s_Lights */

void d3s_CollectLights(void)
{
    d3s_NumLights = 0;
    d3_ForEachObject(d3_DefaultScene, 0, 0x10, d3s_LightCB, 0);
}
