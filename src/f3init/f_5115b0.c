/* 0x5115b0, collects the name s into the list r (count at r + 0x80, at most 0x20) when it contains "ub_FACKEL_" */


char unk_Unk5115b0(int s, int r)
{
    if (strstr((char *)s, "ub_FACKEL_") != 0) {
        *(int *)(r + *(int *)(r + 0x80) * 4) = s;
        *(int *)(r + 0x80) = *(int *)(r + 0x80) + 1;
    }
    return *(int *)(r + 0x80) < 0x20;
}
