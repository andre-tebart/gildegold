/* 0x4025c0 */

void ch_SetPos(CHARACTER *ch, D3VECTOR *pos, int unk)
{
    d3_SetObjectPos(ch->obj, pos);
    if (unk)
        d3_RelightObject(ch->obj);
    ch_PlaceOnGround(ch, 0);
    if (ch->transport) {
        ch_PlaceTransport(ch, 0);
        if (unk)
            d3_RelightObject(*ch->transport);
    }
    if (ch->lowPolyBody)
        ch_UpdateLowPoly(ch);
}
