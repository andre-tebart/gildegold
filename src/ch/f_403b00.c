/* 0x403b00, loads the animation the character switches to next (ch_CheckAniMorph) */

D3ANIMDATA *ch_SetNextAnim(CHARACTER *ch, char *anim, char loops)
{
    int walk = 0;
    D3ANIMDATA *a;
    char name[0x110];
    char path[0x110];

    if (ch->next)
        return 0;
    vfs_ToSlashes(anim);
    sprintf(path, "character/%s/%s_%s.baf", ch->body, anim, ch->body);
    sprintf(name, "%s_%s", anim, ch->body);
    vfs_ToSlashes(name);
    a = (D3ANIMDATA *)d3_FindAnim(name);
    if (!_strcmpi(anim, "bewegung/gehen") || !_strcmpi(anim, "bewegung/karren_ziehen"))
        walk = 1;
    if (!a)
        ch->next = d3_LoadAnimStream(path, name, walk);
    else
        ch->next = a;
    if (!ch->next)
        return 0;
    ch->nextloops = loops;
    ch->next->refs++;
    return ch->next;
}
