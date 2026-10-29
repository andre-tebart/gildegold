/* 0x4538a0, last function of the TU: invalidates the cached scene lighting (light generation ++, all light
   records dropped, the lights-changed flag set) and rebuilds the terrain light map. on > 1 relights every
   object of the default scene at once; on 0 it does nothing unless 0x633438 is set.
   @deep Q2: 0x633438 has no evidenced name. The whole exe references it four times (4 raw hits): d3_Init only sets it 0; it gates the three orthographic view cameras Oben/Vorne/Seite in d3if_CreateCameras (0x448910) and makes type-6 spawns type 5 in d3_SpawnObject (0x43d..., case 'r'); here it skips the relight when on == 0. No code writes it to 1, so what the condition stands for (editor/view mode?) is unproven. Q1 fixed (d3_LightInvalidate), the other identifiers named (633738 -> d3_Terrain, 633b14 -> d3_LightsChanged). */

void d3_LightInvalidate(unsigned char on)
{
    d3_Busy++;
    d3_LightGeneration++;
    if (on == 0) {
        if (d3_Unk633438 == 0)
            goto done;
    } else if (on > 1) {
        d3_ForEachObject2(d3_DefaultScene, 0, 0x40, (void (*)(D3OBJECT *))d3_RelightObject);
        goto done;
    }
    d3_ForEachObject(d3_DefaultScene, 0, 2, (D3OBJECTCB)d3_LightRemoveRecords, 0);
    d3_LightsChanged = 1;
done:
    if (d3_Terrain)
        d3fl_MakeLight(d3_Terrain);
    d3_Busy--;
}
