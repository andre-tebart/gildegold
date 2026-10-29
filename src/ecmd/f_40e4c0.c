/* 0x40e4c0, d3_ForEachObject callback of DetachAnimFromAll: releases the textures of every object whose name
   contains a->pattern; 1 (keeps the walk going) */

char ocmd_DetachAnimCB(D3OBJECT *obj, OCMDANIM *a)
{
    if (strstr(obj->name, a->pattern))
        d3t_ReleaseObjectTextures(obj);
    return 1;
}
