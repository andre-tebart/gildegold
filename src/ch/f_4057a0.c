/* 0x4057a0, queues the animation name n times */

int ch_QueueAnim(CHARACTER *ch, char *name, int n)
{
    CHACTION *a = ch_Unk40b270(ch);

    if (!a)
        return 0;
    a->args[0] = n;
    a->ch = ch;
    a->count = 0;
    a->func = (CHACTFUNC)ch_ActPlay;
    a->type = 0x2e;
    a->speed = 1.0f;
    a->next = 0;
    a->unk10 = 0;
    strcpy(a->u.name, name);
    return 1;
}
