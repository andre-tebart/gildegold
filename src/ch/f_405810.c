/* 0x405810, queues the animation name n times at the given speed */

/* ---- 0x405810..0x406e40 (worker ch3) ---- */

CHACTION *ch_QueueAnimSpeed(CHARACTER *ch, char *name, int n, float speed)
{
    CHACTION *a = ch_Unk40b270(ch);

    if (!a)
        return 0;
    a->args[0] = n;
    a->func = (CHACTFUNC)ch_ActPlay;
    a->type = 0x2e;
    a->speed = 1.0f;
    a->next = 0;
    a->unk10 = 0;
    strcpy(a->u.name, name);
    a->speed = speed;
    return a;
}
