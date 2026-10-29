/* 0x411960, starts the animation <name> on obj: an already loaded one by its base name, else the file
   <d3_AnimPath><name>.oam. */

int d3t_PlayAnim(D3OBJECT *obj, char *name, unsigned int flags)
{
    char msg[512];
    char buf[256];
    char *base;
    D3ANIMFLAGS af;
    D3ANIM *anim;

    if (!obj || !name)
        return 0;
    strcpy(buf, name);
    base = util_BaseName(buf);
    if (!d3_FindAnim(base))
        /* d3_LoadAnimStream's first parameter is the file name, the second the animation lookup name */
        d3_LoadAnimStream(name, base, 0);
    D3_ZERO(&af, sizeof(af));
    if (flags & 2)
        af.b[1] |= 0x10;
    if (flags & 1)
        af.b[1] &= ~0x10;
    if (flags & 4)
        af.b[1] |= 2;
    if (flags & 0x10)
        af.b[1] |= 8;
    if (flags & 0x20)
        af.b[1] |= 4;
    af.b[8] |= 2;
    if (obj->material && d3anim_StartAnim(obj->material->animCtl, base, af))
        return 1;
    sprintf(msg, "%s%s.oam", d3_AnimPath, name);
    anim = (D3ANIM *)d3_LoadObjectAnim(msg, obj, 1);
    if (!anim)
        return 0;
    if (flags & 2)
        anim->flags2d |= 0x10;
    if (flags & 1)
        anim->flags2d &= ~0x10;
    if (flags & 4)
        anim->flags2d |= 2;
    if (flags & 0x10)
        anim->flags2d |= 8;
    if (flags & 0x20)
        anim->flags2d |= 4;
    return 1;
}
