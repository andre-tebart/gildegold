/* 0x40e5a0, switches every object whose name contains *name to the texture set *set (ocmd_SelectTextureSetCB
   through d3_ForEachObject); 0 */

int ocmd_SelectAllTextureSets(char **name, int *set)
{
    OCMDTEXSEL ctx;

    strcpy(ctx.name, *name);
    ctx.file[0] = ch_EmptyName[0];
    ctx.n = 0;
    ctx.rnd = 0;
    ctx.set = *set;
    if (ctx.name[0])
        d3_ForEachObject(d3_DefaultScene, 0, 0xc0, ocmd_SelectTextureSetCB, &ctx);
    return 0;
}
