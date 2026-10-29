/* 0x575540, the middle outcome of the event 0xa4: per +4 is charged 3 + random (4) (0x50cc50) */

void he_Unk575540(char *per)
{
    gm_SendMoneyDelta(HM_I(per, 4), (util_Random(4) & 0xffff) + 3);
}
