/* 0x504c00, the job group of a job-pair number n (two numbers per group: 1 .. 2 -> 0xb, 3 .. 4 -> 7, ..) */


int gm_Unk504c00(int n)
{
    switch (n) {
    case 1:
    case 2:
        return 0xb;
    case 3:
    case 4:
        return 7;
    case 5:
    case 6:
        return 6;
    case 7:
    case 8:
        return 8;
    case 0xd:
    case 0xe:
        return 5;
    case 0xf:
    case 0x10:
        return 0xc;
    case 0x11:
    case 0x12:
        return 1;
    case 0x13:
    case 0x14:
        return 10;
    case 0x15:
    case 0x16:
        return 2;
    case 0x17:
    case 0x18:
        return 3;
    case 0x19:
    case 0x1a:
        return 4;
    case 0x1b:
    case 0x1c:
        return 0xd;
    case 0x1f:
    case 0x20:
        return 0xe;
    case 0x21:
    case 0x22:
        return 0xf;
    default:
        return 0;
    }
}
