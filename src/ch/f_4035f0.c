/* 0x4035f0 */

void ch_DetachTransport(CHARACTER *ch)
{
    if (ch->transport) {
        mem_Free(ch->transport);
        ch->transport = 0;
    }
}
