/* 0x4048b0, queues action type before the action "before"; the action's arguments follow */

CHACTION *ch_InsertAction(CHARACTER *ch, CHACTION *before, unsigned int type, ...)
{
    int arg0;
    CHACTION *a, *prev;
    int i, x, z, s;
    va_list args;

    if (!before)
        return 0;
    if (!ch_ActDefs[type].func) {
        arg0 = type;
        type = 0;
    }
    a = ch_GetFreeQueueEntry();
    if (!a) {
        err_Message("ch_InsertAction(): Could not ch_GetFreeQueueEntry()");
        return 0;
    }
    prev = before->prev;
    before->prev = a;
    a->prev = prev;
    a->next = before;
    if (prev)
        prev->next = a;
    else
        CH_ACTIONS(before->ch) = a;
    a->func = ch_ActDefs[type].func;
    a->ch = ch;
    a->type = type;
    a->count = 0;
    va_start(args, type);
    for (i = 0; i < ch_ActDefs[a->type].nargs; i++)
        a->args[i] = va_arg(args, int);
    if (type == 0x33) {
        if (!a->args[0] && a->args[1] != -1 || a->args[0] && a->args[1] == -1) {
            err_Message("ch_InsertAction(): move2universe: invalid combination of ids and universeindex");
            ch_RemoveAction(a);
            return 0;
        }
    } else if (type == 0x2d) {
        if (a->args[0] < 0 || a->args[0] > 256 || a->args[1] < 0 || a->args[1] > 256 || a->args[2] < 0
            || a->args[2] > 64 || d3_Scenes[a->args[2]].dirty) {
            ch_RemoveAction(a);
            return 0;
        }
        if (ch->scene->hmap && (a->args[0] > D3SCENE_UNKB4(&d3_Scenes[a->args[2]])
                                || a->args[1] > D3SCENE_UNKB4(&d3_Scenes[a->args[2]]))) {
            ch_RemoveAction(before);
            return 0;
        }
    } else if (type == 0)
        a->args[0] = arg0;
    a->unk10 = 0;
    ch_CheckQueue(ch, a);
    return a;
}
