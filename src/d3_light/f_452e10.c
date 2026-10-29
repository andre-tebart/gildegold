/* 0x452e10, the per-frame light step: resets the vertex colours of the lit objects and then adds the
   flickering contribution (both over the default scene's light objects) */

void d3_LightUpdate(void)
{
    d3_ForEachObject2(d3_DefaultScene, 0, 4, d3_LightResetVertexColors);
    d3_ForEachObject2(d3_DefaultScene, 0, 4, d3_LightFlicker);
}
