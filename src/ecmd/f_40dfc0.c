/* 0x40dfc0, loads the animation by attaching and releasing it */

int ocmd_PreloadAnim(D3OBJECT **pobj, char **file)
{
    CHANIMFLAGS f;
    char path[0x100];
    char name[0x100];
    D3OBJECT *obj;
    char *base;

    obj = *pobj;
    strcpy(path, *file);
    strcat(path, ".baf");
    strcpy(name, path);
    base = util_BaseName(name);
    if (path[0] && obj && obj->info) {
        if (!d3_FindAnim(name))
            d3_LoadAnimStream(path, name, 0);
        memset(&f, 0, sizeof(f));
        f.u.all = 0;
        f.b8 = 1;
        f.u.c.b1 = 0x10;
        f.u.c.b0 = 0;
        f.b = 0;
        f.a = 1;
        d3anim_StartAnimCh(obj->info, base, f);
        d3_ReleaseTexture(obj->info, base);
    }
    return 0;
}
