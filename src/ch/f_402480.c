/* 0x402480 */

void ch_SetVisible(CHARACTER *ch, int show)
{
    if (ch && ch->obj) {
        d3if_ShowTree(ch->obj, show);
        if (ch->rightHand)
            d3if_Show(ch->rightHand, show);
        if (show)
            d3_RelightObject(ch->obj);
        if (ch->transport) {
            d3if_ShowTree(*ch->transport, show);
            if (show)
                d3_RelightObject(*ch->transport);
        }
        if (ch->lowPolyBody)
            d3if_Show(ch->lowPolyBody, show);
        if (show)
            ch->flags.hidden = 0;
        else
            ch->flags.hidden = 1;
        ch_UpdateLowPoly(ch);
    } else
        err_Message("ch_SetVisible(): invalid character");
}
