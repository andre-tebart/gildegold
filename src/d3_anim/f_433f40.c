/* 0x433f40, the cached animation with this name, 0 if not loaded. */

int d3_FindAnim(char *name)
{
    D3ANIMDATA *a;

    for (a = d3anim_Head.next; a != &d3anim_Tail; a = a->next)
        if (!_strcmpi(name, a->name))
            return (int)a;
    return 0;
}
