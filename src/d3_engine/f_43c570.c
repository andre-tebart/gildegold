/* 0x43c570, allocates an object; types >= 5 are lights, the kind taken from the name's first letter. @deep Q2: d3_Unk633438 (0x633438) has no evidenced name: the exe references it four times only (raw .text scan in the d3_light.c note at d3_LightInvalidate): d3_Init sets it 0 and nothing writes it 1; it gates the three orthographic view cameras Oben / Vorne / Seite in d3if_CreateCameras, makes the type-6 ("r") spawns type 5 here, and makes d3_LightInvalidate(on=0) relight. The role (editor / view mode?) is not provable from these uses. The other identifier unk194 is D3LIGHTINFO.time now (D3LDATA.time in d3_light.h). */

D3OBJECT *d3_SpawnObject(int type, char *name)
{
    D3OBJECT *obj = mem_Alloc(0x228, "d3:SpawnObject");

    if (type >= 5)
        D3_X(obj)->light = mem_Alloc(0x1ac, "d3:SpawnObject(lightinfo)");
    d3_InitObject(obj);
    if (type >= 5) {
        switch (name[0]) {
        case 'p':
            obj->type = 7;
            break;
        case 'r':
            obj->type = 6;
            break;
        case 's':
            obj->type = 8;
            break;
        default:
            obj->type = 5;
            break;
        }
        if (d3_Unk633438 && obj->type == 6)
            obj->type = 5;
    } else
        obj->type = type;
    switch (obj->type) {
    case 6:
        D3_X(obj)->light->time = 0;
        break;
    case 8:
        obj->objectFlags |= 4;
        break;
    }
    D3_X(obj)->type0 = obj->type;
    strncpy(obj->name, name, 0x40);
    obj->scene = d3_DefaultScene;
    return obj;
}
