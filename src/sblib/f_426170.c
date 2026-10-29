/* 0x426170 */

void sb_SetRestart(SBSAMPLE *s, int on)
{
    if (on)
        s->flags |= 0x10;
    else
        s->flags &= ~0x10;
}
