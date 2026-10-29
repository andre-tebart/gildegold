/* 0x53e400, starts a search of the records: n pairs (kind, value): 0 type (byte), 1 owner (+4), 2 player (+8, word), 3 argument (+0x10); returns the first match (0x53e300 gives the next) */

char *he_FindEvent(int n, ...)
{
    va_list ap;
    int kind;
    char *pc;
    int i;

    U_I((unsigned)&full_data_625708) = 0xff;
    U_I((unsigned)&full_data_62570c) = -1;
    U_I((unsigned)&full_data_625710) = 0xffff;
    U_I((unsigned)&full_data_625714) = -1;
    *(char *)(unsigned)&full_data_6f2624 = 1;
    U_I((unsigned)&full_data_6f2628) = 0;
    va_start(ap, n);
    for (i = n; i > 0; i--) {
        kind = va_arg(ap, int);
        switch ((char)kind) {
        case 0:
            U_I((unsigned)&full_data_625708) = (unsigned char)va_arg(ap, int);
            break;
        case 1:
            U_I((unsigned)&full_data_62570c) = va_arg(ap, int);
            break;
        case 2:
            *(unsigned short *)(unsigned)&full_data_625710 = (unsigned short)va_arg(ap, int);
            break;
        case 3:
            U_I((unsigned)&full_data_625714) = va_arg(ap, int);
            break;
        }
    }
    va_end(ap);
    pc = (char *)(unsigned)&he_Events;
    i = 0;
    U_I(((unsigned)&he_QuickList + 0xaaa0)) = (unsigned)&he_Events;
    if (*(char *)(unsigned)&he_Events == 0) {
        do {
            if (i > 0x3ff)
                goto L_none;
            pc += 0x14c;
            i++;
        } while (*pc == 0);
        U_I(((unsigned)&he_QuickList + 0xaaa0)) = (int)pc;
        if (i > 0x3ff) {
L_none:
            U_I(((unsigned)&he_QuickList + 0xaaa0)) = 0;
        }
    }
    return he_NextEvent();
}
