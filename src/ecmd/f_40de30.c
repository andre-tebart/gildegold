/* 0x40de30, waits until the animation slots of the object are free */

int ocmd_AttachAnim(D3OBJECT **pobj, char **file, int *once)
{
    CHANIMFLAGS f;
    char path[0x100];
    char name[0x100];
    D3OBJECT *obj;
    int i;

    obj = *pobj;
    if (evt_CurCall && evt_CurCall->fn == ocmd_AttachAnim) {
        for (i = 0; i < 3; i++) {
            if (obj->material->tex[i].name) {
                evt_Cur->waitfn = ocmd_AttachAnim;
                return 0;
            }
        }
        return 0;
    }
    strcpy(path, *file);
    strcat(path, ".baf");
    strcpy(name, path);
    util_BaseName(name);
    if (path[0] && obj && obj->info) {
        if (!d3_FindAnim(name))
            d3_LoadAnimStream(path, name, 0);
        memset(&f, 0, sizeof(f));
        f.u.all = 0;
        f.u.c.b1 = *once ? 0 : 8;
        f.b8 = 1;
        f.u.c.b1 |= 0x10;
        f.u.c.b0 = 0;
        f.b = 0;
        f.a = 1;
        d3anim_StartAnimCh(obj->info, name, f);
    }
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = ocmd_AttachAnim;
    return 0;
}
