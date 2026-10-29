/* 0x43cd80, frees the mesh of obj: the nsurf loaded surfaces, surf[3] (the stock surface d3io_AttachStock
   fills, which is not counted in nsurf) and the mesh block itself (D3MESH, obj->material); obj->info is
   cleared. */

void d3_FreeMesh(D3OBJECT *obj)
{
    int i;

    if (obj->material) {
        for (i = 0; i < D3_MESH(obj)->nsurf; i++)
            d3_FreeSurf(&D3_MESH(obj)->surf[i]);
        d3_FreeSurf(&D3_MESH(obj)->surf[3]);
        D3_MESH(obj)->nsurf = 0;
        mem_Free(obj->material);
        obj->material = 0;
    }
    obj->info = 0;
}
