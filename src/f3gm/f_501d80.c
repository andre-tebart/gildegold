/* 0x501d80, the class of a building kind (the type byte of the kind record): 3 for 1 / 3 / 6 / 0xf, 6 for 2 / 0x1e, 8 for 4 / 5 / 9, 4 for 7, 1 for 8 / 0xe / 0x12 / 0x14 .. 0x16 / 0x1d / 0x1f / 0x20, 2 for 0xb .. 0xd, 7 for 0x13, 5 for 0x17 .. 0x1a, 9 for 0x21, else 0 */

int gm_BuildingClass(int kind)
{
    switch (ai_BTypes[kind * 0x24d]) {
    case 1:
    case 3:
    case 6:
    case 0xf:
        return 3;
    case 2:
    case 0x1e:
        return 6;
    case 4:
    case 5:
    case 9:
        return 8;
    case 7:
        return 4;
    case 8:
    case 0xe:
    case 0x12:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x1d:
    case 0x1f:
    case 0x20:
        return 1;
    default:
        return 0;
    case 0xb:
    case 0xc:
    case 0xd:
        return 2;
    case 0x13:
        return 7;
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1a:
        return 5;
    case 0x21:
        return 9;
    }
}
