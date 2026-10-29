/* 0x5124d0, the first building of the type 0xb: 0x511e60 */


void unk_Unk5124d0(void)
{
    int b;

    b = (int)gm_BuildQuery(1, 5, 0xb);
    if (b != 0)
        init_BuildingSceneUniverse(b);
}
