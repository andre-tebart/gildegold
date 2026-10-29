/* 0x447800, d3_ForEachObject2 callback: precaches the textures of the object's mesh */

typedef struct D3IFTEX {          /* view of a texture slot: D3TEXTURE with the flags as unsigned char */
    char pad00[0x64];
    void *d3dtex;                   /* 0x64 */
    unsigned char state;            /* 0x68, bit 7 = precached */
} D3IFTEX;

void d3if_PrecacheCB(D3OBJECT *obj)
{
    D3OBJINFO *info;
    D3IFTEX **tex;
    int i, n;

    info = obj->info;
    if (info && info->mesh) {
        n = D3_OPS(info)->count;
        tex = (D3IFTEX **)((int *)info)[5];
        for (i = 0; i < n; i++)
            if (tex[i] && tex[i]->d3dtex && !(tex[i]->state & 0x80)) {
                d3d_PrecacheTexture(tex[i]->d3dtex);
                tex[i]->state |= 0x80;
            }
    }
}
