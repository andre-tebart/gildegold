/* 0x43e180, switches all "r" lights on (on != 0) or off (d3_ToggleRLightCB per object) and relights the
   scene. */

void d3_SetRLights(unsigned char on)
{
    d3_ForEachObject(d3_DefaultScene, 0, 6, (D3OBJECTCB)d3_ToggleRLightCB, (void *)on);
    d3_LightInvalidate(1);
}
