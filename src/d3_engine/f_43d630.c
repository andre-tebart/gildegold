/* 0x43d630, d3_ForEachObject callback: sets the alpha of all surfaces. */

char d3_SetAlphaCB(D3OBJECT *obj, D3TEXFLAGS *f)
{
    unsigned int i;

    if (obj->material)
        for (i = 0; i < D3_MESH(obj)->nsurf; i++)
            d3_SetObjectAlpha(obj, (D3OBJINFO *)&D3_MESH(obj)->surf[i], *f);
    return 1;
}
