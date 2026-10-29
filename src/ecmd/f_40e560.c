/* 0x40e560, d3_ForEachObject callback of SelectAllTextureSets */

/* ---- 0x40e560.. (worker ocmd2) ---- */

char d3if_SelectTextureSet(struct D3OBJECT *obj, struct D3SURF *surf, unsigned char set, char reload); /* 0x446f30 */
char d3_SetLightSet(unsigned int set, float t, float scale, char calc);                /* 0x449de0 */
char d3_SetFogSet(unsigned int a, unsigned int b, float t);                            /* 0x44a170 */
extern unsigned char d3_D3DActive;   /* 0x63345c, d3_engine.c */
/* 0x63345d is the unaligned dword right after d3_D3DActive (0x63345c, d3_engine.c): a view of the
   byte's storage, not a second definition inside the unit's block. */
#define d3_NoTextures (*(int *)((char *)&d3_D3DActive + 1))

char ocmd_SelectTextureSetCB(D3OBJECT *obj, OCMDTEXSEL *ctx)
{
    if (obj->info) {
        if (strstr(obj->name, ctx->name))
            d3if_SelectTextureSet(obj, obj->info, ctx->set, 1);
    }
    return 1;
}
