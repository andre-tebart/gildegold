/* 0x40e2b0, attaches a->file to every object whose name contains a->pattern */

char ocmd_AttachAnimCB(D3OBJECT *obj, OCMDANIM *a)
{
    CHANIMFLAGS f;
    char path[0x100];
    char name[0x100];
    char base[0x100];
    D3ANIMSLOT *s;
    char *p;

    if (strstr(obj->name, a->pattern)) {
        strcpy(path, a->file);
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
            f.u.c.b1 = a->n == 1 ? 0x10 : 0;
            f.u.c.b0 = a->n;
            f.b = 0;
            f.a = 1;
            s = (D3ANIMSLOT *)d3anim_StartAnim(obj->info, name, f);
            if (s && a->rnd) {
                s->frame = rand() % (s->data->nframes - 1);
                s->nextFrame = s->frame + 1;
            }
        }
    }
    return 1;
}
