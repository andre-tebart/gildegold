/* 0x404200, loads the named animations of the character once (and releases them again) */

void ch_PreloadAnims(CHARACTER *ch, int n, ...)
{
    va_list ap;
    int i;
    char *anim;
    char walk;
    CHANIMFLAGS f;
    char name[0x100];
    char path[0x100];

    va_start(ap, n);
    for (i = 0; i < n; i++) {
        anim = va_arg(ap, char *);
        if (*anim) {
            walk = 0;
            if (!_strcmpi(anim, "bewegung/gehen") || !_strcmpi(anim, "bewegung/karren_ziehen"))
                walk = 1;
            sprintf(path, "character/%s/%s_%s.baf", ch->body, anim, ch->body);
            sprintf(name, "%s_%s", anim, ch->body);
            vfs_ToSlashes(name);
            if (!d3_FindAnim(name)) {
                d3_LoadAnimStream(path, name, walk);
                memset(&f, 0, sizeof(f));
                f.u.all = 0;
                f.b8 = 1;
                f.u.c.b1 = 0x18;
                f.u.c.b0 = 0;
                f.b = 0;
                f.a = 1;
                d3anim_StartAnim(ch->obj->material->animCtl, name, f);
                d3_ReleaseTexture(ch->obj->material->animCtl, name);
            }
        }
    }
    va_end(ap);
}
