/* 0x404810 */

int ch_ClearActions(CHARACTER *ch)
{
    CHACTION *a;

    while (a = CH_ACTIONS(ch)) {
        if (a->type == '-' && a->u.walk.path) {
            mem_Free(a->u.walk.path);
            a->u.walk.path = 0;
        }
        ch_RemoveAction(a);
    }
    return 1;
}
