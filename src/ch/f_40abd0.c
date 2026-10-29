/* 0x40abd0, action callback 0x37: shows (args[0] != 0) or hides the character */

void ch_ActSetVisible(CHACTION *a)
{
    if (!a->count) {
        ch_SetVisible(a->ch, a->args[0]);
        ch_RemoveAction(a);
    }
}
