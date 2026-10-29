/* 0x43e110, d3_ForEachObject callback (Ghidra missed it): switches the type 5 lights named "r..." to type 6
   (on) and the type 6 ones back to type 5; the light is updated either way and a switched-off one gets
   D3OBJX.offTime = d3_Time, a switched-on one loses its light records. */

char d3_ToggleRLightCB(D3OBJECT *obj, char on)
{
    if (on) {
        if (obj->type == 5 && obj->name[0] == 'r') {
            obj->type = 6;
            D3_X(obj)->offTime = d3_Time;
            d3_UpdateObject(obj, 0);
        }
    } else if (obj->type == 6) {
        obj->type = 5;
        d3_LightRemoveRecords(obj, 0);
        d3_UpdateObject(obj, 0);
    }
    return 1;
}
