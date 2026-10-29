/* 0x56f170, 1 when a running event of the type 0x99 has the owner +0xb0 == `owner` (the search 0x53e400 / 0x53e300) */

int he_Unk56f170(int owner)
{
    int r;

    for (r = (int)he_FindEvent(1, 0, 0x99); r != 0; r = (int)he_NextEvent()) {
        if (HM_I(r, 0xb0) == owner)
            return 1;
    }
    return 0;
}
