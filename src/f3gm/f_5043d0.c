/* 0x5043d0, the title number of the rank r in the job group a: the last title number of the group (1 -> 0x33, 2 -> 0x3f, 3 -> 0x45 .. 0xf -> 0x5e) minus (r - 1); 0 for group 0 or an unknown one */


int gm_TitleNumber(int a, int r)
{
    if (a != 0) {
        r = r - 1;
        switch (a) {
        case 1:
            return 0x33 - r;
        case 2:
            return 0x3f - r;
        case 3:
            return 0x45 - r;
        case 4:
            return 0x4b - r;
        case 5:
            return 0x27 - r;
        case 6:
            return 0x18 - r;
        case 7:
            return 0x12 - r;
        case 8:
            return 0x1e - r;
        case 9:
            return 0xc - r;
        case 0xa:
            return 0x39 - r;
        case 0xb:
            return 6 - r;
        case 0xc:
            return 0x2d - r;
        case 0xd:
            return 0x51 - r;
        case 0xe:
            return 0x58 - r;
        case 0xf:
            return 0x5e - r;
        }
    }
    return 0;
}
