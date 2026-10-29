/* 0x56f3b0, 1 when no running event of the type 0x9b has +0xac == the dword at a + 4; 0 when one does */

int he_Unk56f3b0(int a)
{
    int r;

    for (r = (int)he_FindEvent(1, 0, 0x9b); r != 0; r = (int)he_NextEvent()) {
        if (HM_I(r, 0xac) == *(int *)(a + 4))
            return 0;
    }
    return 1;
}
