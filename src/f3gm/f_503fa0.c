/* 0x503fa0, the building type of the job of the character kind x (0x504ad0 maps x to the job number first; 0 for none) */

int gm_BuildingTypeOfJob(int x)
{
    int k;

    k = gm_TitleGroup(x);
    if (k != 0)
        return gm_JobBuildingType(k);
    return 0;
}
