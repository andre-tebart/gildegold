/* 0x402540 */

void ch_Unk402540(CHARACTER *ch)
{
    if (!ch->obj->info)
        d3_InflateObject(ch->obj);
    if (ch->rightHand && !ch->rightHand->info)
        d3_InflateObject(ch->rightHand);
    if (ch->transport && *ch->transport && !(*ch->transport)->info)
        d3_InflateObject(*ch->transport);
    if (ch->lowPolyBody && !ch->lowPolyBody->info)
        d3_InflateObject(ch->lowPolyBody);
}
