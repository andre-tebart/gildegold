/* 0x504050, the building type of the job number x (1 -> 0x12, 2 -> 0x14, 3 -> 0x15, 4 -> 0x16, 5 -> 0xe, 6 -> 8, 7 -> 7, 8 -> 9, 9 -> 5, 10 -> 0x13, 0xb -> 4, 0xc -> 0x10, 0xd -> 0x1d, else 0) */

int gm_JobBuildingType(int x)
{
    switch (x) {
    case 1:
        return 0x12;
    case 2:
        return 0x14;
    case 3:
        return 0x15;
    case 4:
        return 0x16;
    case 5:
        return 0xe;
    case 6:
        return 8;
    case 7:
        return 7;
    case 8:
        return 9;
    case 9:
        return 5;
    case 10:
        return 0x13;
    case 0xb:
        return 4;
    case 0xc:
        return 0x10;
    case 0xd:
        return 0x1d;
    default:
        return 0;
    }
}
