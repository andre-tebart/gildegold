/* 0x56f360, 1 when no running event of the type 0x9a has both +0xac == a and +0xb0 == the dword at b + 4; 0 when one does */

int he_Unk56f360(int a, int b)
{
    int r;

    for (r = (int)he_FindEvent(1, 0, 0x9a); r != 0; r = (int)he_NextEvent()) {
        if (HM_I(r, 0xac) == a && HM_I(r, 0xb0) == *(int *)(b + 4))
            return 0;
    }
    return 1;
}
