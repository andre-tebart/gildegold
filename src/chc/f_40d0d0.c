/* 0x40d0d0, where is passed as the string itself (not a pointer to it) */

int chc_AttachObject(CHARACTER **ch, char **where, char **obj)
{
    int hand;

    if (!*ch)
        return 0;
    if (!strcmp((char *)where, "d3_LeftHand"))
        hand = 1;
    else if (!strcmp((char *)where, "d3_RightHand"))
        hand = 2;
    else if (!strcmp((char *)where, "d3_Head"))
        hand = 3;
    ch_SetHandObject(*ch, hand, *obj);
    return 1;
}
