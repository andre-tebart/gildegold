/* 0x575640, 1 when the message filter mask 0x6f263c has the bit of the message kind id (0x605 the bit 1, 0x606 2, 0x607 4, 0x608 8,
   0x609 0x10, 0x60a 0x20, 0x60b 0x40, 0x60c 0x80, 0x60d 0x100); else 0 */

int he_IsMessageKindFiltered(int id)
{
    switch (id) {
    case 0x605:
        return (U_I((unsigned)&full_data_6f263c) & 1) != 0;
    case 0x606:
        return (U_I((unsigned)&full_data_6f263c) & 2) != 0;
    case 0x607:
        return (U_I((unsigned)&full_data_6f263c) & 4) != 0;
    case 0x608:
        return (U_I((unsigned)&full_data_6f263c) & 8) != 0;
    case 0x609:
        return (U_I((unsigned)&full_data_6f263c) & 0x10) != 0;
    case 0x60a:
        return (U_I((unsigned)&full_data_6f263c) & 0x20) != 0;
    case 0x60b:
        return (U_I((unsigned)&full_data_6f263c) & 0x40) != 0;
    case 0x60c:
        return (U_I((unsigned)&full_data_6f263c) & 0x80) != 0;
    case 0x60d:
        return (U_I((unsigned)&full_data_6f263c) & 0x100) != 0;
    }
    return 0;
}
