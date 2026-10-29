/* 0x504290, the first title number of the job group that 0x504ad0 gives for x (1 -> 0x29, 2 -> 0x2f, 3 -> 0x32 .. 0xf -> 0x51), 0 for none */


int gm_FirstTitleNumber(int x)
{
    int k;

    k = gm_TitleGroup(x);
    if (k != 0) {
        switch (k) {
        case 1:
            return 0x29;
        case 2:
            return 0x2f;
        case 3:
            return 0x32;
        case 4:
            return 0x35;
        case 5:
            return 0x21;
        case 6:
            return 0x17;
        case 7:
            return 0x14;
        case 8:
            return 0x1a;
        case 9:
            return 0xe;
        case 0xa:
            return 0x2c;
        case 0xb:
            return 0xb;
        case 0xc:
            return 0x27;
        case 0xd:
            return 0x48;
        case 0xe:
            return 0x4e;
        case 0xf:
            return 0x51;
        }
    }
    return 0;
}
