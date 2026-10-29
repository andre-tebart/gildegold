/* 0x403950 */

D3ANIMSLOT *ch_AttachMotion(CHARACTER *ch, int motion)
{
    CHANIMFLAGS f;
    D3ANIMSLOT *s;
    char name[0x110];
    char path[0x110];
    char msg[0x200];

    sprintf(path, "character/%s/%s_%s.baf", ch->body, ch_Motions[motion].name, ch->body);
    sprintf(name, "%s_%s", ch_Motions[motion].name, ch->body);
    vfs_ToSlashes(name);
    if (path[0]) {
        memset(&f, 0, sizeof(f));
        f.u.all = 0;
        f.b8 = 1;
        f.u.c.b1 = 0x18;
        f.u.c.b0 = 0;
        f.b = 0;
        f.a = 1;
        if (!d3_FindAnim(name))
            d3_LoadAnimStream(path, name, 1);
        s = (D3ANIMSLOT *)d3anim_StartAnim(ch->obj->material->animCtl, name, f);
        if (!s) {
            sprintf(msg, "ch_AttachMotion(): Could not attach ani: %s", path);
            return 0;
        }
    }
    if (!d3t_SceneIndex(ch->scene))
        s->timeout = -1;
    else
        s->timeout = ch_MotionTimeout;
    return s;
}
