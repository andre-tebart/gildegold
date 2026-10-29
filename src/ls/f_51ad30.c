/* 0x51ad30, the index of the action queue entry p (0x631050, 0x194 bytes each, ch.h: CHACTION *ch_Queue), -1 for NULL */


unsigned ls_ActionIndex(char *p)
{
    return p ? (unsigned)(p - ls_ActionQueue) / 0x194 : 0xffffffff;
}
