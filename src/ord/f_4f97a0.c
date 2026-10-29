/* 0x4f97a0, byte i (below 5) of 0x91b460, 0x7e for others */

unsigned char ord_GetRatingByte(int i)
{
    if (i >= 5)
        return 0x7e;
    return ord_RatingBytes[i];
}
