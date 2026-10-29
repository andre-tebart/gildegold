/* 0x45d9f0, replaces the layer's texture: releases the old one, loads name with the layer's flags and reloads it; the layer keeps no texture when the load fails */

void d3sky_SetLayerTex(D3SKYLAYER *l, char *name)
{
    tex_Release(l->t);
    l->t = tex_Load(name, l->flags, 0, 0);
    if (l->t)
        tex_Reload(l->t, 0);
}
