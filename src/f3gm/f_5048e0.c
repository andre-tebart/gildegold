/* 0x5048e0, the job group of a building type (the inverse of 0x504050; 4 -> 0xb, 5 -> 9, 7 -> 7, 8 -> 6, 9 -> 8, 0xe -> 5, 0x10 -> 0xc, 0x12 -> 1, 0x13 -> 10, 0x14 -> 2, 0x15 -> 3, 0x16 -> 4, 0x1d -> 0xd, 0x1f -> 0xe, 0x20 -> 0xf, else 0) */


int gm_BuildingJob(int t)
{
    switch (t) {
    case 4:
        return 0xb;
    case 5:
        return 9;
    case 7:
        return 7;
    case 8:
        return 6;
    case 9:
        return 8;
    case 0xe:
        return 5;
    case 0x10:
        return 0xc;
    case 0x12:
        return 1;
    case 0x13:
        return 0xa;
    case 0x14:
        return 2;
    case 0x15:
        return 3;
    case 0x16:
        return 4;
    case 0x1d:
        return 0xd;
    case 0x1f:
        return 0xe;
    case 0x20:
        return 0xf;
    default:
        return 0;
    }
}
