/* 0x438c60, whether an object type is selected by a d3_ForEachObject mask. */

char d3_TypeInMask(int type, unsigned int mask)
{
    switch (type) {
    case 0:
        return (mask & 0x20) == 0x20;
    case 3:
        return (mask & 0x01) == 0x01;
    case 4:
        return (mask & 0x40) == 0x40;
    case 1:
        return (mask & 0x80) == 0x80;
    case 2:
        return (mask & 0x100) == 0x100;
    case 5:
        return (mask & 0x02) == 0x02;
    case 6:
        return (mask & 0x04) == 0x04;
    case 7:
        return (mask & 0x08) == 0x08;
    case 8:
        return (mask & 0x10) == 0x10;
    }
    return 0;
}
