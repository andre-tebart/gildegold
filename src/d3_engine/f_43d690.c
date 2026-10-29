/* 0x43d690, applies the alpha flags f to obj and its group in the default scene (object mask 0x240;
   d3_SetAlphaCB sets the alpha of every surface). */

void d3_SetAlpha(D3OBJECT *obj, D3TEXFLAGS *f)
{
    if (obj)
        d3_ForEachObject(d3_DefaultScene, obj, 0x240, (D3OBJECTCB)d3_SetAlphaCB, f);
}
