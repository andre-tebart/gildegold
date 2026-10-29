/* 0x50d460, marks every scene universe as not loaded; returns 0 */


char init_ResetSceneUniverses(void)
{
    memset(init_ObjectUniverse, -1, 0x2dc);
    memset(init_BuildingUniverse, -1, 0x22);
    return 0;
}
