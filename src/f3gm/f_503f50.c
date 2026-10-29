/* 0x503f50, 0 for the kind numbers 1 .. 4, 7 .. 0xc and 0xf .. 0x18, else 2 */

int gm_Unk503f50(int x)
{
    switch (x) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 7:
    case 8:
    case 9:
    case 10:
    case 0xb:
    case 0xc:
    case 0xf:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
        return 0;
    default:
        return 2;
    }
}
