/* 0x40e0e0, attaches the animation *file to *obj, looped *n times (b1 0x10 when *n is 1); while the object's
   textures are still loaded the command waits (evt_Cur->waitfn) and returns; a script error when the file is
   missing; 0 */

int ocmd_AttachAnimLooped(D3OBJECT **pobj, char **file, int *n)
{
    CHANIMFLAGS f;
    char path[0x100];
    char name[0x100];
    char base[0x100];
    D3OBJECT *obj;
    char *p;
    int i;

    obj = *pobj;
    if (evt_CurCall && evt_CurCall->fn == ocmd_AttachAnimLooped) {
        for (i = 0; i < 3; i++) {
            if (obj->material && obj->material->tex[i].name) {
                evt_Cur->waitfn = ocmd_AttachAnimLooped;
                return 0;
            }
        }
        return 0;
    }
    strcpy(path, *file);
    strcat(path, ".baf");
    strcpy(name, path);
    p = util_BaseName(name);
    if (p)
        strcpy(base, p);
    if (path[0] && obj && obj->info) {
        if (!d3_FindAnim(name))
            d3_LoadAnimStream(path, name, 0);
        memset(&f, 0, sizeof(f));
        f.u.all = 0;
        f.b8 = 1;
        f.u.c.b1 = *n == 1 ? 0x10 : 0;
        f.u.c.b0 = *n;
        f.b = 0;
        f.a = 1;
        if (!d3anim_StartAnimCh(obj->info, name, f))
            evt_Error(evt_Cur, evt_Cur->pc, "AttachAnimLooped(): Could not find animation-file...");
    }
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = ocmd_AttachAnimLooped;
    return 0;
}
