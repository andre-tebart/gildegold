/* 0x43cfb0, gives obj an empty mesh block (D3MESH, 0x970 bytes, "d3:AddObjectDrawData") when it has none:
   the four surfaces and four shadow slots are initialized, the counters, mirror pointers and the copy
   pointers are cleared, frameStamp is -1 (never transformed) and flags gets 0x40 (no vertex animation). */

void d3_AddObjectDrawData(D3OBJECT *obj)
{
    int i;

    if (!obj->material) {
        obj->material = mem_Alloc(0x970, "d3:AddObjectDrawData");
        for (i = 0; i <= 3; i++)
            d3_InitSurf(&D3_MESH(obj)->surf[i], obj);
        for (i = 0; i < 4; i++) {
            D3_MESH(obj)->sub[i].dir.x = 0;
            D3_MESH(obj)->sub[i].dir.y = 0;
            D3_MESH(obj)->sub[i].dir.z = 0;
            D3_MESH(obj)->sub[i].light = 0;
            D3_MESH(obj)->sub[i].tex = 0;
        }
        D3_MESH(obj)->nsurf = 0;
        D3_MESH(obj)->freeCallback = 0;
        D3_MESH(obj)->mirrorPoints = 0;
        D3_MESH(obj)->mirrorPolys = 0;
        D3_MESH(obj)->mirrorSource = 0;
        D3_MESH(obj)->groundOffsetRaw = 0;
        D3_MESH(obj)->lightScale = 1.0f;
        D3_MESH(obj)->lightGeneration = 0;
        D3_MESH(obj)->frameStamp = -1;
        D3_MESH(obj)->flags |= 0x40;
    }
}
