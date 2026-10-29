/* 0x575710, sets / clears the bit of the message kind id in the filter mask 0x6f263c (0x605 the bit 1, 0x606 2,
   0x607 4, 0x608 8, 0x609 0x10, 0x60a 0x20, 0x60b 0x40, 0x60c 0x80, 0x60d 0x100): `on` 0 clears it, else sets it.
   The setter side of he_IsMessageKindFiltered; the jump table 0x575830. Ends 0x57582f, the table runs to 0x575854, fill to 0x575860 */

void he_SetMessageKindFiltered(int id, int on)
{
    switch (id) {
    case 0x605:
        if (on)
            U_I((unsigned)&full_data_6f263c) |= 1;
        else
            U_I((unsigned)&full_data_6f263c) &= ~1;
        break;
    case 0x606:
        if (on)
            U_I((unsigned)&full_data_6f263c) |= 2;
        else
            U_I((unsigned)&full_data_6f263c) &= ~2;
        break;
    case 0x607:
        if (on)
            U_I((unsigned)&full_data_6f263c) |= 4;
        else
            U_I((unsigned)&full_data_6f263c) &= ~4;
        break;
    case 0x608:
        if (on)
            U_I((unsigned)&full_data_6f263c) |= 8;
        else
            U_I((unsigned)&full_data_6f263c) &= ~8;
        break;
    case 0x609:
        if (on)
            U_I((unsigned)&full_data_6f263c) |= 0x10;
        else
            U_I((unsigned)&full_data_6f263c) &= ~0x10;
        break;
    case 0x60a:
        if (on)
            U_I((unsigned)&full_data_6f263c) |= 0x20;
        else
            U_I((unsigned)&full_data_6f263c) &= ~0x20;
        break;
    case 0x60b:
        if (on)
            U_I((unsigned)&full_data_6f263c) |= 0x40;
        else
            U_I((unsigned)&full_data_6f263c) &= ~0x40;
        break;
    case 0x60c:
        if (on)
            U_I((unsigned)&full_data_6f263c) |= 0x80;
        else
            U_I((unsigned)&full_data_6f263c) &= ~0x80;
        break;
    case 0x60d:
        if (on)
            U_I((unsigned)&full_data_6f263c) |= 0x100;
        else
            U_I((unsigned)&full_data_6f263c) &= ~0x100;
        break;
    }
}
